
/* Vectrex Tutorial VECPOS, based on http://vectrexmuseum.com/share/coder/html/line2.asm */

#include "vectrex.h"

#define SPRITE_BLOW_UP 25
int8_t turtle_line_list[] = { 23                           
                ,  2*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                ,  2*SPRITE_BLOW_UP,  1*SPRITE_BLOW_UP
                ,  2*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP
                ,  0*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP,  1*SPRITE_BLOW_UP
                ,  1*SPRITE_BLOW_UP,  3*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP,  4*SPRITE_BLOW_UP
                ,  1*SPRITE_BLOW_UP,  0*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP,  1*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP,  0*SPRITE_BLOW_UP
                , -3*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                , -3*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP,  0*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP, -1*SPRITE_BLOW_UP
                ,  1*SPRITE_BLOW_UP,  0*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP, -4*SPRITE_BLOW_UP
                ,  1*SPRITE_BLOW_UP, -3*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP, -1*SPRITE_BLOW_UP
                ,  0*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP
                ,  2*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                ,  2*SPRITE_BLOW_UP, -1*SPRITE_BLOW_UP
                , -1*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP
                ,  2*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP };
int main()
{
  int8_t i;
  
  while (1) {   
    Wait_Recal();       /* wait for frame boundary (one frame = 30,000 CPU cyles@50fps) */

    Moveto_d(0, 0);     /* Move beam to center */
    Intensity_5F();     /* Set Intensity of vector beam to 0x5f                         */
    Vec_Misc_Count = 0; /* in order for drawing only 1 vector, this must be set to 0 */

    VIA_t1_cnt_lo = 0x10; 
    Draw_VLc(turtle_line_list);

  }
  return 0; // Will never happen due to the while (1) statement
}

