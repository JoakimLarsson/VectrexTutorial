/* Vectrex Tutorial Bouncer4 by Joakim Larsson Edström, (c) 2015, GPLv3 */

/*
 * Bouncer4 improves/changes Bouncer3 by:
 * - Change naming convention from Cross to the more generic Vexprite ("TM")
 * - Support for non centered vector lists in runtime
 * - Support for viewing multiple different Vexprites simultaneously
 * - dynamic C++ object allocation by 'new' operator overload fixing below error:
 *   ?ASlink-Warning-Undefined Global __Znwm referenced by module bouncer4.cpp
 * - Added animation speed control
 */

#include "vectrex.h"

/*
 * vectorlist class
 * 
 * A class to keep track of the boundaries of a vector symbol. The constructor calculates the boundaries
 * when created by the 'new' operator and passed a Vectrex vectorlist. It only adds a RAM usage of 6 bytes 
 * per unique vectorlist, NOT per animated object! 
 */
class vectorlist {
public:
  vectorlist(int8_t *sym);
  int8_t *vec;      /* Vectrex vector list */
  int8_t  bleft;    /* Left boundary  */
  int8_t  bright;   /* Right boundary */
  int8_t  bup;      /* Up boundary    */
  int8_t  bdown;    /* Down boundary  */
};

/* 
 * The vectorlist constructor takes a parameter which should be a Vectrex vectorlist and calculates the maximum 
 * distance from the origo/center of the object to the edges in all four direction. It is needed to calculate the 
 * bounces to the walls.
 */
vectorlist::vectorlist(int8_t *sym)
{
  int8_t c, i, px, py;

  c = sym[0];  /* First byte of a Vectrex vector list is the number of vectors - 1 */
  c++;         /* So compensate to simplify the loop through */
  px = py = 0;
  i = 1;       /* Start from first coordinate pair of vector list */
  bright = bleft = bup = bdown = 0;
  vec = sym;

  while (c--)    
  {
    py += sym[i++];
    if ( py >  bup)    bup    =  py;
    if (-py >  bdown)  bdown  = -py;
    px += sym[i++];
    if ( px >  bright) bright =  px;
    if (-px >  bleft)  bleft  = -px;
  }
}

/* 
 * Operator overload to avoid the need of the C++ standard libraries when using the new operators below 
 * This particular function only supports the creation of instances of class vectorlist.
 */
#define SYMBOLS 2
#define NULL ((void *) 0L)
void* operator new(long unsigned int size) 
{
  static int i = 0; 
  static int8_t memory[sizeof(vectorlist) * SYMBOLS]; 
  vectorlist *v;

  v = (vectorlist *) ((i < SYMBOLS * sizeof(vectorlist)) ? &memory[i] : NULL); 

  if (v != NULL) 
    i += sizeof(vectorlist);

  return v;
}


/* 
 * The following vectorlists ends up readonly in flash memory and take NO RAM space 
 */

/* Simple square */
#define SIZE 3

int8_t box_line_list[] = 
  { 4,
     SIZE / 2, SIZE / 2,
    -SIZE,     0,
     0,       -SIZE,
     SIZE,     0,
     0,        SIZE    
  };

/* Turtle outline ripped from the line2 tutorial */
#define SPRITE_BLOW_UP 1

int8_t turtle_line_list[] = 
{   23,
     2 * SPRITE_BLOW_UP,  2 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP,  2 * SPRITE_BLOW_UP,
     2 * SPRITE_BLOW_UP,  1 * SPRITE_BLOW_UP,
     2 * SPRITE_BLOW_UP, -2 * SPRITE_BLOW_UP,
     0 * SPRITE_BLOW_UP,  2 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP,  1 * SPRITE_BLOW_UP,
     1 * SPRITE_BLOW_UP,  3 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP,  4 * SPRITE_BLOW_UP,
     1 * SPRITE_BLOW_UP,  0 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP,  1 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP,  0 * SPRITE_BLOW_UP,
    -3 * SPRITE_BLOW_UP,  2 * SPRITE_BLOW_UP,
    -3 * SPRITE_BLOW_UP, -2 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP,  0 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP, -1 * SPRITE_BLOW_UP,
     1 * SPRITE_BLOW_UP,  0 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP, -4 * SPRITE_BLOW_UP,
     1 * SPRITE_BLOW_UP, -3 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP, -1 * SPRITE_BLOW_UP,
     0 * SPRITE_BLOW_UP, -2 * SPRITE_BLOW_UP,
     2 * SPRITE_BLOW_UP,  2 * SPRITE_BLOW_UP,
     2 * SPRITE_BLOW_UP, -1 * SPRITE_BLOW_UP,
    -1 * SPRITE_BLOW_UP, -2 * SPRITE_BLOW_UP,
     2 * SPRITE_BLOW_UP, -2 * SPRITE_BLOW_UP 
};


/* 
 * Vexprites class
 *
 * A class to keep track of the position, direction and speed of an object. Each object requires 
 * only 6 bytes of RAM.
 */
class vexprite 
{
public:
  vectorlist   *sym;           /* vectorlist */
  int8_t        ox, oy;        /* Centerpoint of vexprite    */
  int8_t        xdir, ydir;    /* Speed and direction of vexprite */
  vexprite();
  void draw();
  void move();
};

/* The constructor sets up the basic data for the object */
vexprite::vexprite()
{
  /* Setup start position and directions */
  ox = oy = 0;
  xdir = 1; 
  ydir = 3;
}

/* The draw method draws all vectors for a specific object */
void vexprite::draw()
{
    int px, py;
    int i;

    /* Move beam (origin of vexprite) to new position in screen coordinates (Vectrex BIOS) */
    Moveto_d(ox, oy);

    /* Draw Symbol (Vectrex BIOS) */
    if (sym != NULL)
      Draw_VLc(sym->vec);  
}

/* The move method moves the object and checks for bounces against the bounding box */
void vexprite::move()
{
    /* Update position of vexprite */
    ox += xdir;
    oy += ydir;

#define BSIZE 127

    /* Check boundaries and bounce if needed, along X axis */
    if (ox > (BSIZE / 2 - sym->bright)){
        ox = (BSIZE / 2 - sym->bright) - (ox - (BSIZE / 2 - sym->bright));
        xdir = -xdir;
    }else if 
       (ox < -(BSIZE / 2 - sym->bleft)){
        ox = -(BSIZE / 2 - sym->bleft) + ( -(BSIZE / 2 - sym->bleft) - ox);
        xdir = -xdir;
    }

    /* Check boundaries and bounce if needed, along Y axis */
    if (oy > (BSIZE / 2 - sym->bup)){
        oy = (BSIZE / 2 - sym->bup) - (oy - (BSIZE / 2 - sym->bup));
        ydir = -ydir;
    }else if 
       (oy < -(BSIZE / 2 - sym->bdown)){
        oy = -(BSIZE / 2 - sym->bdown) + ( - (BSIZE / 2 - sym->bdown) - oy);
        ydir = -ydir;
    }
}

#define VEXPRITES 5
#define SCALE 200
#define SPEED 1

main()
{
  class vexprite vexprites[VEXPRITES];
  int8_t i;
  int8_t speed;
  class vectorlist *v1, *v2;

  /* Setup scale and beam intensity */
  Intensity_a(0x5f);
  VIA_t1_cnt_lo = SCALE;
  
  /* Initiate the vexprite vector lists */
  v1 = new vectorlist(box_line_list);
  v2 = new vectorlist(turtle_line_list);

  /* Make each vexprite a little different */
  for (i = 0; i < VEXPRITES; i++){

    /* Different symbols */
    vexprites[i].sym = i & 1 ? v1 : v2; 

    /* Different speed and directions */
    vexprites[i].xdir += i;
    vexprites[i].ydir -= i;
    if (i & 1)
      vexprites[i].ydir *= -1;
  }

  /* Set up the animation speed */
  speed = SPEED;

  while (1) {   
    /* Wait for frame boundary (one frame = 30,000 CPU cyles@50fps) */
    Wait_Recal();
    VIA_t1_cnt_lo = SCALE;

    /* Draw boundary box */
    Moveto_d(BSIZE / 2, BSIZE / 2);
    Draw_Line_d(-BSIZE, 0);
    Draw_Line_d(  0, -BSIZE );
    Draw_Line_d( BSIZE, 0);
    Draw_Line_d( 0,   BSIZE );

    /* Animate all the Vexprites */
    for (i = 0; i < VEXPRITES; i++)
    {
      /* Reset pen and scale */ 
      Reset0Ref();      
      VIA_t1_cnt_lo = SCALE;
      
      /* Draw Vexprites */
      vexprites[i].draw();

      /* Move Vexprites depending on animation speed */
      if (speed-- == 0)
      {
	vexprites[i].move();
	speed = SPEED;
      }
    }
  }
  return 0;               /* Will never happen due to the while (1) statement             */
}
