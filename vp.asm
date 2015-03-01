;
;  Fixed to compile with AS6809 by Joakim Larsson Edström 2015
; 
;   Assembles with
;  /usr/local/bin/as6809 -l -og vp.asm
;  /usr/local/bin/aslink -m -nws -b .text=0x0 vp.rel
;  /usr/local/bin/srec_cat.exe vp.s19 -o vp.bin -binary
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
        .word      music                   ; music from the rom
        .byte      0xF8, 0x50, 0x20, -0x70 ; height, width, rel y, rel x
        .ascii     "POSITION SOME DOTS"    ; Banner text
	.byte      0x80	                   ; text ending with 0x80
                                
        .byte      0                       ; end of game header
;***************************************************************************
; CODE SECTION
;***************************************************************************
; here the cartridge program starts off
main:
                JSR     Wait_Recal              ; Vectrex BIOS recalibration

                ; prepare dot 0
                JSR     Intensity_5F            ; Sets the intensity of the
                                                ; vector beam to 0x5f
                JSR     Delay_3                 ; delay for 30 cycles
                JSR     Dot_here                ; Plot a dot here
                ; end of dot 0

                ; prepare dot 1
                LDA     #100                    ; load 100
                STA     VIA_t1_cnt_lo           ; 100 as scaling
                LDA     #-100                   ; relative Y position = -100
                LDB     #-50                    ; relative X position = -50
                                                ; register D = 256*A+B
                JSR     Moveto_d                ; move to position specified
                                                ; in D register
                JSR     Dot_here                ; Plot a dot here
                ; end of dot 1

                ; prepare dot 2
                LDA     #50                     ; load 50
                STA     VIA_t1_cnt_lo           ; 50 as scaling
                LDA     #100                    ; relative Y position = 100
                LDB     #50                     ; relative X position = 50
                                                ; register D = 256*A+B
                JSR     Moveto_d                ; move to position specified
                                                ; in D register
                JSR     Dot_here                ; Plot a dot here
                ; end of dot 2

                ; prepare dot 3
                LDB     #200                    ; scale factor of 200
                LDX     #position               ; load address of position
                JSR     Moveto_ix_b             ; move to position specified
                                                ; in address pointed to by X
                                                ; and set scaling factor found
                                                ; register B
                                                ; (befor positioning)
                JSR     Dot_here                ; Plot a dot here
                ; end of dot 3
                BRA     main                    ; and repeat forever
;***************************************************************************
; DATA SECTION
;***************************************************************************
position:
                .byte      100, 50                 ; relative Y, X position
;***************************************************************************
music:
                .word   0xfee8
                .word   0xfeb6
                .byte   0x0, 0x80
                .byte   0x0, 0x80

                .END  main
;***************************************************************************
