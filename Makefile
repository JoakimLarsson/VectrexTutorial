
LB=/usr/local/bin/aslib
LN=/usr/local/bin/aslink
AS=/usr/local/bin/as6809
AFLAGS=-l -og 
CC=/usr/local/libexec/gcc/m6809-unknown-none/4.3.4/cc1

CLEAN_LIST=*.bin *.s19 crt0.o dot2.s *.o *.rel *.lst *.map *~ *bin vectrexlib.a *.ram *.rom

.PHONY: clean all dot2 vecpos line1 line2 sound1

all: dot2 vecpos line1 line2 sound1

dot2: dot2.bin

vecpos: vecpos.bin

line1: line1.bin

line2: line2.bin

sound1: sound1.bin

%.bin: %.s19 %_ram.s19
	srec_cat $*_ram.s19 -offset -0xc880 -o $*.ram -binary || echo -n
	@touch $*.ram
	srec_cat $*.s19 -o $*.rom -binary
	cat $*.rom $*.ram > $*.bin

%.s19 %_ram.s19: %.o crt0.o 
	$(LN) -m -nws -b .text=0x0 $*.s19 crt0.o $*.o 

clean:
	@- $(RM) $(CLEAN_LIST)

%.o: %.asm
	$(AS) -l -og $<
	mv $*.rel $*.o

.s.o:
	$(AS) -l -og $<
	mv $*.rel $*.o

.c.o:
	$(CC) $< -dumpbase $* -mno-direct -mint16 -msoft-reg-count=0 -auxbase $* -o $*.s
	$(AS) -l -og $*.s
	mv $*.rel $*.o
