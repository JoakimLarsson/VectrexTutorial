
/* Vectrex Tutorial DOT2, based on http://vectrexmuseum.com/share/coder/html/dot2.asm */

#include "vectrex.h"

/* List of dot coordinates ordered as x, y pairs */
int8_t dotlist[] = { 30, -70, -40, 10, 0, 30, 40, 10, 10, 30, 5, 30, -10, 40};

int main()
{
  int8_t i;
  
  while (1) {   
    Wait_Recal();       /* wait for frame boundary (one frame = 30,000 CPU cyles@50fps) */
    Intensity_a(0x5f);  /* Set Intensity of vector beam to 0x5f                         */
    Delay_3();          /* Delay for 30 CPU cycles + time of calling                    */
    VIA_t1_cnt_lo = 50; /* Set scaling to 50                                            */

    /* Plot the dots, first argument is the list and the second is the number of coordinate minus one as         */
    /*  required by the Dot_List BIOS call.                                                                      */
    Dot_List(dotlist, 6); /* This is the orginal assemnbler equivalent, but lets take the advantage of C:        */

    /* C improvement                                                                                             */
    /*  By using the sizeof() operator the size of the list can be calulated automatically so you can change     */
    /*  the number of dots without cghaning in the function call. Sizeof() returns the number of numbers so      */
    /*  we also need divide it by 2 to get the number of coordinates (x, y pairs) and also minus one as required */
    /*  by the BIOS call. The calculation is done in compile time and generates no extra assembler code!!        */
    Dot_List(dotlist, (sizeof(dotlist) / 2) - 1); 

    /* Of course you can remove one of the Dot_List calls above */

  }
  return 0; // Will never happen due to the while (1) statement
}

