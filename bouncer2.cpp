/* Vectrex Tutorial Bouncer2 by Joakim Larsson Edström, (c) 2015, GPLv3 */
/*
 * Bouncer2 uses a C++ class to keep track of multiple objects with different
 * behaviors. 
 *
 */

#include "vectrex.h"

typedef struct  {
  int8_t sx1, sy1, sx2, sy2;
} linetype;

/* Encapsulate knowledge about the cross in a C++ class */
class cross 
{
  linetype 	lin[2]; /* Two lines makes a cross */
public:
  int8_t        ox, oy; /* Centerpoint of cross    */
  int8_t    xdir, ydir; /* Speed and direction of cross */
  cross();
  void draw();
  void move();
};

#define SIZE 4

/* The constructor sets up the basic data for the object */
cross::cross()
{
  /* Setup the 2D plus sign */
  lin[0].sx1 = -SIZE;  lin[0].sx2 =  SIZE;  lin[0].sy1 =  0;   lin[0].sy2 =  0;
  lin[1].sx1 =  0;   lin[1].sx2 =  0;   lin[1].sy1 = -SIZE;  lin[1].sy2 =  SIZE;

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

    /* start always at 0,0 in cross coordinates */
    px = py = 0;

    for (i = 0; i < 2; i++){

      /* Move to start of this line (relative where we are) */
      Moveto_d(lin[i].sx1 - px, lin[i].sy1 - py);

      /* in order for drawing only 1 vector, this must be set to 0    */
      Vec_Misc_Count = 0;

      /* Draw */
      Draw_Line_d(lin[i].sx2 - lin[i].sx1, lin[i].sy2 - lin[i].sy1);

      /* Remember where we are */
      px = lin[i].sx2; py = lin[i].sy2;
    }
}

/* The move method moves the object and checks for bounces against the bounding box */
void cross::move()
{
    /* Update position of cross */
    ox += xdir;
    oy += ydir;

#define BSIZE 127

    /* Check boundaries and bounce if needed, along X axis */
    if (ox >= (BSIZE / 2 - SIZE)){
        ox = (BSIZE / 2 - SIZE) - (ox - (BSIZE / 2 - SIZE));
        xdir = -xdir;
    }else if 
       (ox < -(BSIZE / 2 - SIZE)){
        ox = -(BSIZE / 2 - SIZE) + ( -(BSIZE / 2 - SIZE) - ox);
        xdir = -xdir;
    }

    /* Check boundaries and bounce if needed, along Y axis */
    if (oy >= (BSIZE / 2 - SIZE)){
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
  Intensity_5F();
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

    /* Draw boundary box */
    VIA_t1_cnt_lo = SCALE;
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
