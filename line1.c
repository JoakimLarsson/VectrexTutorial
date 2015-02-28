
/* Vectrex Tutorial VECPOS, based on http://vectrexmuseum.com/share/coder/html/line1.asm */

#include "vectrex.h"

int main()
{
  int8_t i;
  
  while (1) {   
    Wait_Recal();         /* wait for frame boundary (one frame = 30,000 CPU cyles@50fps) */

    VIA_t1_cnt_lo = 0x80; /* Set scaling to 80                                            */
    Moveto_d(0, 0);       /* Move beam to center                                          */
    Intensity_5F();       /* Set Intensity of vector beam to 0x5f                         */

    Vec_Misc_Count = 0;   /* in order for drawing only 1 vector, this must be set to 0    */
    Draw_Line_d(50, 100); /* Draw line from center to 50, 100                             */

  }
  return 0;               /* Will never happen due to the while (1) statement             */
}

