;  From http://vectrexmuseum.com/share/coder/html/line1.asm
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
        .byte      0xF8, 0x50, 0x20, -0x45 ; height, width, rel y, rel x
        .ascii     "SINGLE LINE"           ; Banner text
	.byte      0x80	                   ; text ending with 0x80
                                
        .byte      0                       ; end of game header
;***************************************************************************
; CODE SECTION
;***************************************************************************
; here the cartridge program starts off
main:
                JSR     Wait_Recal              ; Vectrex BIOS recalibration
                LDA     #0x80                    ; scaling factor of 0x80 to A
                STA     VIA_t1_cnt_lo           ; move to time 1 lo, this
                                                ; means scaling
                LDA     #0                      ; to 0 (y)
                LDB     #0                      ; to 0 (x)
                JSR     Moveto_d                ; move the vector beam the
                                                ; relative position
                JSR     Intensity_5F            ; Sets the intensity of the
                                                ; vector beam to 0x5f
                CLR     Vec_Misc_Count          ; in order for drawing only 1
                                                ; vector, this must be set to
                                                ; 0
                LDA     #100                    ; to 100 (y)
                LDB     #50                     ; to 50 (x)
                JSR     Draw_Line_d             ; draw the line now
                BRA     main                    ; and repeat forever
;***************************************************************************
                .END main
;***************************************************************************
