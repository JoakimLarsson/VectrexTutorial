
/* Vectrex Tutorial VECPOS, based on http://vectrexmuseum.com/share/coder/html/sound1.asm */

#include "vectrex.h"

int main()
{
  int8_t i;
  
  Vec_Music_Flag = 1;       /* one means, we are about to start a new piece of music */
  while (1) {   
    DP_to_C8();             /* Set Direct Page to 0xc8 as required by Init_Music_chk() BIOS call */
    Init_Music_chk(music1); /* Prepares music data in 0xC83F-0xC84C                              */
    Wait_Recal();           /* wait for frame boundary (one frame = 30,000 CPU cyles@50fps)      */

    Do_Sound();             /* Feed the music engine from 0xC83F-0xC84C if needed                */
  }
  return 0;              /* Will never happen due to the while (1) statement             */
}

