#
# GNU gcc specifics to reduce code size, comments from 
#     https://gcc.gnu.org/onlinedocs/gcc-4.3.6/gcc/
#

CFLAGS += -fno-inline
#     Don't pay attention to the inline keyword. Normally this option is used to keep the compiler from expanding any 
#     functions inline. Note that if you are not optimizing, no functions can be expanded inline. 
