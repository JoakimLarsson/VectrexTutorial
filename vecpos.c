
/* Vectrex Tutorial VECPOS, based on http://vectrexmuseum.com/share/coder/html/vecpos.asm */

#include "vectrex.h"

/* an x, y pair */
int8_t coord[] = { 100, 50 };

int main()
{
  int8_t i;
  
  while (1) {   
    Wait_Recal();       /* wait for frame boundary (one frame = 30,000 CPU cyles@50fps) */
//    Intensity_a(0x5f);  /* Set Intensity of vector beam to 0x5f                         */
    Intensity_5F();       /* Set Intensity of vector beam to 0x5f                         */
    Delay_3();          /* Delay for 30 CPU cycles + time of calling                    */
    Dot_here();         /* Set Dot 0 here                                               */

    VIA_t1_cnt_lo = 100; /* Set scaling to 100                                                 */
    Moveto_d(-50, -100); /* Move beam to relative X position = -50, relative Y position = -100 */
    Dot_here();          /* Set Dot 1 here                                                     */

    VIA_t1_cnt_lo = 50; /* Set scaling to 50                                                   */
    Moveto_d(50, 100);  /* Move beam to relative X position = -50, relative Y position = -100  */
    Dot_here();         /* Set Dot 2 here                                                      */

    Moveto_ix_b(coord, 200);  /* Move beam to rel X position and rel Y position =  data     */
    Dot_here();                  /* Set Dot 3 here                                             */

  }
  return 0; // Will never happen due to the while (1) statement
}

