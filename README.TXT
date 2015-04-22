Vectrex C/C++ Tutorial
======================

Usage
-----
You need to compile the GCC6809 compiler by downloading the patched 
GCC 4.3.4 source from https://github.com/bcd/gcc To succeed you probably 
need to install a few utilities such as bison and flex etc. You may also 
need to apply the patch gcc6809c++.patch and compile it all with 
'sudo make everything'. 

If the build is successful the resulting binaries should be installed in 
/usr/local/bin and /usr/local/libexec/gcc/m6809-unknown-none/4.3.4/.

The tutorial also requires a package called 'srecord' for your host
and the asm6809 package from http://www.6809.org.uk/asm6809/

Check out the make file and the C files to get going. A 'make all' will 
produce .bin files in the bin catalog that can be loaded in a Vectrex or 
an emulator such as binary only ParaJVE: http://www.vectrex.fr/ParaJVE/ 
or from the opensource  Mame project http://mamedev.org/

Background
----------
A class mate of mine back then in the early 80:ies had a small business 
renting out Vectrexes. Around 1984 he got tired of it and let me  borrow 
two of them for while, no games just MineStorm. Many late nights playing
MineStorm later I took one to work and hooked it up to the Motorola dev
system we had there, the Exorciser, which just had got a new emulator 
pod for the 6809 installed. I desoldered the CPU and inserted a DIL socket 
so I could swap beck and forth between a real CPU and the emulator. I had
no particular motivation but curiousity I guess. When I several year later
turned the units back to my former class mate he went "uhhh, I'd forgot 
about those...." and ever since then I wanted them back so badly.

So now I figured it was time to pick it up again, so I have baught a
Vectrex with games on Ebay and some developers stuff like the magic
MATEOS VECTREX Developer KIT http://www.vectrex.hackermesh.org/index.php/en/

While waiting on the shipments to arrive I started to go over some coding 
examples on the net, but most examples seemed to be in Assembler, until I found 
the Bloxorz port to Vectrex by Frank Buss: http://www.frank-buss.de/vectrex/
This was a great starting point but I needed more since the code I intend to
port is actually based on C++.

I converted the compilation scripts of Bloxorz into a Makefile and started
to figure out a more efficient way to interface to the BIOS than going over
a separate layer of conversion. The result was an interface based on C macros 
with inline assembler. 

This has a number of advantages over an interface library
- Only BIOS calls that is used will add to code size
- Each BIOS call can be described to the compiler in terms of parameters,
  return values and what registers is clobbered.
- Avoiding one layer of subroutines will save both ROM and RAM

I then took the VECTREX.INC written by Bruce Tomlin and Christopher Salomon and
started to translate it into a H file. In fact I did a bulk conversion, easy thanks 
to emacs macros(!), which I am currently improving by going over the assembler 
tutorials at the Vectrex museum: http://vectrexmuseum.com/share/coder/index.htm 
and convert the examples to C. Please help out by submitting improvements back.

Have Fun!

Joakim Larsson
