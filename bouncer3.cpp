/* Vectrex Tutorial Bouncer3 by Joakim Larsson Edström, (c) 2015, GPLv3 */
/*
 * Bouncer3 improves Bouncer2 by:
 * - Use references to Vectrex vector tables for objects in ROM   
 *
 */

#include "vectrex.h"

#define SIZE 4

/* A cross described as a Vectrex vector list */
/* First integer is the number of vectors - 1 */
int8_t cross_line_list[] = {  
     4,
     SIZE / 2,  SIZE / 2,
    -SIZE    , -SIZE,
     SIZE / 2,  SIZE / 2,
    -SIZE / 2,  SIZE / 2,
     SIZE    , -SIZE,
};

/* Encapsulate knowledge about the cross in a C++ class */
class cross 
{
  int8_t        *sym;          /* reference to vectorlist */
public:
  int8_t        ox, oy;        /* Centerpoint of cross    */
  int8_t        xdir, ydir;    /* Speed and direction of cross */
  int8_t        bleft, bright; /* Left and Right boundaries */
  int8_t        bup, bdown;    /* Up and down boundaries */
  cross();
  void draw();
  void move();
};

/* The constructor sets up the basic data for the object */
cross::cross()
{
  /* Setup symbol referens */
  sym = cross_line_list;

  /* Setup start position and directions */
  ox = oy = 0;
  xdir = 1; 
  ydir = 3;
}

/* The draw method draws all vectors for a specific object */
void cross::draw()
{
    int px, py;
    int i;

    /* Move beam (center of cross) to new position in screen coordinates */
    Moveto_d(ox, oy);

    Draw_VLc(sym);  /* Draw Symbol            */
}

/* The move method moves the object and checks for bounces against the bounding box */
void cross::move()
{
    /* Update position of cross */
    ox += xdir;
    oy += ydir;

#define BSIZE 127

    /* Check boundaries and bounce if needed, along X axis */
    if (ox > (BSIZE / 2 - SIZE)){
        ox = (BSIZE / 2 - SIZE) - (ox - (BSIZE / 2 - SIZE));
        xdir = -xdir;
    }else if 
       (ox < -(BSIZE / 2 - SIZE)){
        ox = -(BSIZE / 2 - SIZE) + ( -(BSIZE / 2 - SIZE) - ox);
        xdir = -xdir;
    }

    /* Check boundaries and bounce if needed, along Y axis */
    if (oy > (BSIZE / 2 - SIZE)){
        oy = (BSIZE / 2 - SIZE) - (oy - (BSIZE / 2 - SIZE));
        ydir = -ydir;
    }else if 
       (oy < -(BSIZE / 2 - SIZE)){
        oy = -(BSIZE / 2 - SIZE) + ( - (BSIZE / 2 - SIZE) - oy);
        ydir = -ydir;
    }
}

#define CROSSES 25
#define SCALE 200

main()
{
  class cross crosses[CROSSES];
  int8_t i;

  /* Setup scale and beam intensity */
  Intensity_a(0x5f);
  VIA_t1_cnt_lo = SCALE;
  
  /* make each cross a little different */
  for (i = 0; i < CROSSES; i++){
    crosses[i].xdir += i;
    crosses[i].ydir -= i;
    if (i & 1)
      crosses[i].ydir *= -1;
  }

  while (1) {   
    /* wait for frame boundary (one frame = 30,000 CPU cyles@50fps) */
    Wait_Recal();
    VIA_t1_cnt_lo = SCALE;

    /* Draw boundary box */
    Moveto_d(BSIZE / 2, BSIZE / 2);
    Draw_Line_d(-BSIZE, 0);
    Draw_Line_d(  0, -BSIZE );
    Draw_Line_d( BSIZE, 0);
    Draw_Line_d( 0,   BSIZE );

    /* Animate all the crosses */
    for (i = 0; i < CROSSES; i++)
    {
      /* Reset pen and scale */ 
      Reset0Ref();      
      VIA_t1_cnt_lo = SCALE;

      /* Draw and update object */
      crosses[i].draw();
      crosses[i].move();
    }

  }
  return 0;               /* Will never happen due to the while (1) statement             */
}
