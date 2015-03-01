;
;  Fixed to compile with AS6809 by Joakim Larsson Edström 2015
; 
;   Assembles with
;  /usr/local/bin/as6809 -l -og ln1.asm
;  /usr/local/bin/aslink -m -nws -b .text=0x0 ln1.rel
;  /usr/local/bin/srec_cat.exe ln1.s19 -o ln1.bin -binary
;
; 
;***************************************************************************
; DEFINE SECTION
;***************************************************************************
                .INCLUDE "VECTREX.INC"     ; vectrex function includes

; start of vectrex memory with cartridge name...
		.area .text
;***************************************************************************
; HEADER SECTION
;***************************************************************************
        .ascii     "g GCE 1998"            ; 'g' is copyright sign
	.byte      0x80                    ; text ending with 0x80
        .word      music1                  ; music from the rom
        .byte      0xF8, 0x50, 0x20, -0x55 ; height, width, rel y, rel x
        .ascii     "VECTOR LIST TEST"      ; Banner text
	.byte      0x80	                   ; text ending with 0x80
                                
        .byte      0                       ; end of game header
;***************************************************************************
; CODE SECTION
;***************************************************************************
; here the cartridge program starts off
main:
                JSR     Wait_Recal              ; Vectrex BIOS recalibration
                LDA     #0x10                   ; scaling factor of 0x10 to A
                STA     VIA_t1_cnt_lo           ; move to time 1 lo, this
                                                ; means scaling
                LDA     #0                      ; to 0 (y)
                LDB     #0                      ; to 0 (x)
                JSR     Moveto_d                ; move the vector beam the
                                                ; relative position
                JSR     Intensity_5F            ; Sets the intensity of the
                                                ; vector beam to 0x5f
                LDX     #turtle_line_list       ; load the address of the to be
                                                ; drawn vector list to X
                JSR     Draw_VLc                ; draw the line now
                BRA     main                    ; and repeat forever
;***************************************************************************
SPRITE_BLOW_UP .EQU 25
turtle_line_list:
                .byte 23                           ; number of vectors - 1
                .byte  2*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                .byte  2*SPRITE_BLOW_UP,  1*SPRITE_BLOW_UP
                .byte  2*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP
                .byte  0*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP,  1*SPRITE_BLOW_UP
                .byte  1*SPRITE_BLOW_UP,  3*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP,  4*SPRITE_BLOW_UP
                .byte  1*SPRITE_BLOW_UP,  0*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP,  1*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP,  0*SPRITE_BLOW_UP
                .byte -3*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                .byte -3*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP,  0*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP, -1*SPRITE_BLOW_UP
                .byte  1*SPRITE_BLOW_UP,  0*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP, -4*SPRITE_BLOW_UP
                .byte  1*SPRITE_BLOW_UP, -3*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP, -1*SPRITE_BLOW_UP
                .byte  0*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP
                .byte  2*SPRITE_BLOW_UP,  2*SPRITE_BLOW_UP
                .byte  2*SPRITE_BLOW_UP, -1*SPRITE_BLOW_UP
                .byte -1*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP
                .byte  2*SPRITE_BLOW_UP, -2*SPRITE_BLOW_UP
;***************************************************************************
                .END main
;***************************************************************************
