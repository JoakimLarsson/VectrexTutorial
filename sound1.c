
/* Vectrex Tutorial VECPOS, based on http://vectrexmuseum.com/share/coder/html/sound1.asm */

#include "vectrex.h"

int main()
{
  int8_t i;
  
  Vec_Music_Flag = 1;   /* one means, we are about to start a piece of music */
  while (1) {   
    DP_to_C8();
    Init_Music_chk(music1);
    Wait_Recal();       /* wait for frame boundary (one frame = 30,000 CPU cyles@50fps) */

    Do_Sound();
  }
  return 0; // Will never happen due to the while (1) statement
}

