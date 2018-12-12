/* Vectrex Tutorial Bouncer6 by Joakim Larsson Edström, (c) 2018, GPLv3 */

/*
 * Bouncer6 improves/changes Bouncer5 by:
 * - adding depth dimension
 * - no turtles more boxes
 * - a few bugs fixed
 * - added support for BIOS calls with return values, e.g. Random()
 */

#include "vectrex.h"
#include "bouncer.h"

/* 
 * The vectorlist constructor takes a parameter which should be a Vectrex vectorlist and calculates the maximum 
 * distance from the drawing start point of the object to the edges in all four direction. This is needed to 
 * be able to calculate the bounces to the walls. 
 * Profile ROM:  6 RAM:   0 STACK:   4 CYCLES:       Bouncer5.vectorlist: On init 
 */
vectorlist::vectorlist(int8_t *sym)
{
  this->init(sym);
}

/* Because the gcc 3.4.x produces multiple constructors for each class and this one is rather big 
 * reducing the constructor to just calling init saves 80+ bytes. Weird but that is C++.
 */
void vectorlist::init(int8_t *sym)
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
 * Profile ROM:  38 RAM:  13 STACK:   2 Bouncer5.new: On init 
 */
#define VLISTS 1
#define VEXPRS VEXPRITES
#define NULL ((void *) 0L)
static int vli = 0; 
static int vxi = 0; 
static int8_t vlists[sizeof(vectorlist) * VLISTS]; 
static int8_t vexprite3ds[sizeof(vexprite3D) * VEXPRS]; 
void* operator new(long unsigned int size) 
{
  void *v = NULL;

  switch (size)
  {
  case sizeof(vectorlist):
    if (vli < VLISTS * sizeof(vectorlist))
    {
      v = (void *) &vlists[vli];
      vli += sizeof(vectorlist);
    }
    break;
  case sizeof(vexprite3D):
    if (vxi < VEXPRS * sizeof(vexprite3D))
    {
      v = (void *) &vexprite3ds[vxi];
      vxi += sizeof(vexprite3D);
    }
    break;
  default: break;
  }

  return v;
}

/* The constructor sets up the basic data for the object 
 * Profile ROM:  13 RAM:   0 STACK:   0 Bouncer5.vexprite: On init 
 */
vexprite3D::vexprite3D()
{
  /* Setup start position and directions */
  ox = oy = oz = 0;
  xdir = 1; 
  ydir = 3;
  zdir = -1;
}

/* The draw method draws all vectors for a specific object 
 * Profile ROM:  23 RAM:   0 STACK:   3 Bouncer5.vexprite.draw: On Update 
 */
void vexprite3D::draw()
{
    int px, py;
    int i;
    unsigned long tmp;

    VIA_t1_cnt_lo = SCALE  - 2 * (oz + ZSIZE / 2);
    
    /* Move beam (origin of vexprite) to new position in screen coordinates (Vectrex BIOS) */
    Moveto_d(ox, oy);

    /* Draw Symbol (Vectrex BIOS) */
    if (sym != NULL)
      Draw_VLc(sym->vec);  
}

/* The move method moves the object and checks for bounces against the bounding box 
 * Profile ROM: 199 RAM:   0 STACK:   0 Bouncer5.vexprite.move: On Update 
 */
void vexprite3D::move()
{
    /* Update position of vexprite */
    ox += xdir;
    oy += ydir;
    oz += zdir;

    /* Check boundaries and bounce if needed, along X axis */
    if (ox > (XSIZE / 2 - sym->bright)){
        ox = (XSIZE / 2 - sym->bright) - (ox - (XSIZE / 2 - sym->bright));
        xdir = -xdir;
    }else if 
       (ox < -(XSIZE / 2 - sym->bleft)){
        ox = -(XSIZE / 2 - sym->bleft) + ( -(XSIZE / 2 - sym->bleft) - ox);
        xdir = -xdir;
    }

    /* Check boundaries and bounce if needed, along Y axis */
    if (oy > (YSIZE / 2 - sym->bup)){
        oy = (YSIZE / 2 - sym->bup) - (oy - (YSIZE / 2 - sym->bup));
        ydir = -ydir;
    }else if 
       (oy < -(YSIZE / 2 - sym->bdown)){
        oy = -(YSIZE / 2 - sym->bdown) + ( - (YSIZE / 2 - sym->bdown) - oy);
        ydir = -ydir;
    }

    /* Check boundaries and bounce if needed, along Z axis */
    if (oz > (ZSIZE / 2)){
        oz = (ZSIZE / 2) - (oz - (ZSIZE / 2));
        zdir = -zdir;
    }else if 
       (oz < -(ZSIZE / 2)){
        oz = -(ZSIZE / 2) + ( - (ZSIZE / 2) - oz);
        zdir = -zdir;
    }

}

/*
 * main() inits the objects and drives the update loop
 * Profile ROM: 324 RAM:   0 STACK:  36 Bouncer5.main: On Update 
 */

main()
{
  class vexprite3D *vexprites[VEXPRITES];
  int8_t i;
  int8_t speed;
  class vectorlist *v1, *v2;

  /* Setup scale and beam intensity */
  Intensity_a(0x5f);
  VIA_t1_cnt_lo = SCALE;
  
  /* Initiate the vexprite vector lists */
  v1 = new vectorlist(box_line_list);
  //  v2 = new vectorlist(turtle_line_list);

  /* Create each vexprite and make it a little different */
  for (i = 0; i < VEXPRITES; i++){

    /* Creation */
    vexprites[i] = new vexprite3D();
    
    /* Vectors */
    vexprites[i]->sym = v1; 

    /* Different speed and directions */
    vexprites[i]->xdir = Random() / 64 - 2;
    vexprites[i]->ydir = Random() / 64 - 2;
    vexprites[i]->zdir = Random() / 64 - 2;
    vexprites[i]->ox = Random() - XSIZE / 2;
    vexprites[i]->oy = Random() - YSIZE / 2;
    vexprites[i]->oz = Random() - ZSIZE / 2;
  } 

  /* Set up the animation speed */
  speed = SPEED;

  volatile uint8_t *sp = &Vec_Seed_Ptr;
  *sp++ = 0x11;
  *sp++ = 0x22;
  *sp++ = 0x45;

  while (1) {   
    /* Wait for frame boundary (one frame = 30,000 CPU cyles@50fps) */
    Wait_Recal();

#if 0 // Disabled frame to avoid burnin on real Vectrex when left on for a long time.    
    /* Reset pen and scale */ 
    Reset0Ref();
    
    VIA_t1_cnt_lo = SCALE;

    /* Draw boundary box */
    Moveto_d(XSIZE / 2, YSIZE / 2);
    Draw_Line_d(-XSIZE, 0);
    Draw_Line_d(  0, -YSIZE );
    Draw_Line_d( XSIZE, 0);
    Draw_Line_d( 0,   YSIZE );
#endif

    for (int j = 0; j < VEXPRITES; j++)
    {
      /* Reset pen and scale */ 
      Reset0Ref();
      
      /* Draw Vexprite */
      vexprites[j]->draw();
	    
      /* Move Vexprite */
      vexprites[j]->move();
    }
  }
  return 0;               /* Will never happen due to the while (1) statement             */
}
