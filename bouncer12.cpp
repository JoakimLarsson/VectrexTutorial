/* Vectrex Tutorial Bouncer2 by Joakim Larsson Edström, (c) 2015, GPLv3 */

#include "vectrex.h"

typedef int8_t MYint;

typedef struct  {
  MYint sx1, sy1, sx2, sy2;
} linetype;

/* Encapsulate knowledge about the cross in a C++ class */
class cross 
{
  linetype 	lin[2]; // Two lines makes a cross
  int           scale;
  int           ox, oy; 
public:
  int           xdir, ydir;
  cross();
  void draw();
};

cross::cross()
{
  /* Setup the 2D cross */
  lin[0].sx1 = -50;  lin[0].sx2 =  50;  lin[0].sy1 =  0;   lin[0].sy2 =  0;
  lin[1].sx1 =  0;   lin[1].sx2 =  0;   lin[1].sy1 = -50;  lin[1].sy2 =  50;

  /* Setup start position and directions */
  ox = oy = 0;
  xdir = 7;
  ydir = 3;
}

void cross::draw()
{
    int px, py;
    int i;

    /* Move beam (center of cross) to new position in screen coordinates */
    Moveto_d(ox, oy);
    VIA_t1_cnt_lo = 0x10;

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

    /* Update position of cross */
    ox += xdir;
    oy += ydir;


    /* Check boundaries and bounce if needed, along X axis */
    if (ox > 50){
      ox  = 50 - (ox - 50);
      xdir = -xdir;
    }else if (ox < -50){
      ox  = -50 + (-50 - ox);
      xdir = -xdir;
    }

    /* Check boundaries and bounce if needed, along Y axis */
    if (oy > 75){
      oy  = 75 - (oy - 75);
      ydir = -ydir;
    }else if (oy < -75){
      oy  = -75 + (-75 - oy);
      ydir = -ydir;
    }
}

#define CROSSES 1

main()
{
  class cross crosses[CROSSES];
  int8_t i;

  /* Setup scale and beam intensity */
  Intensity_5F();
  
  while (1) {   
    /* wait for frame boundary (one frame = 30,000 CPU cyles@50fps) */
    Wait_Recal();
    for (i = 0; i < CROSSES; i++)
      crosses[i].draw();
  }
  return 0;               /* Will never happen due to the while (1) statement             */
}
