#
# GNU gcc6809 target specific options, comments from 
#     <build catalog>/gcc/build-6809/m6809-unknown-none/g++ --target-help
#

#CFLAGS += -m6309                     
# Enable Hitachi 6309 extensions

#CFLAGS += -mabi_version=             
# Sets the calling convention

#CFLAGS += -margcount                 
# Push argument count

#CFLAGS += -mbss-section=             
# Sets the name of the section for uninitialized data

#CFLAGS += -mcasesi                   
# Enable the casesi pattern

#CFLAGS += -mcode-section=            
# Sets the name of the section for code

#CFLAGS += -mdata-section=            
# Sets the name of the section for initialized data

#CFLAGS += -mdirect                   
# Enable direct addressing

#CFLAGS += -mdret                     
# Put function call results in D, not X

#CFLAGS += -mfar-code-page=           
# Sets the far code page value for this compilation unit

#CFLAGS += -mfar-stack-param          
# Enable stack parameters to a farcall

#CFLAGS += -mint16                    
# Use 16-bit integers InverseMask(BYTE_INT)

CFLAGS += -mint8                     
# Use 8-bit integers

#CFLAGS += -mlong_size                
# Use 16-bit size_t

#CFLAGS += -mreg-args                 
# Use registers for function arguments

#CFLAGS += -mshort_size               
# Use 8-bit size_t

CFLAGS += -msoft-reg-count=0 
# Sets the number of soft registers that can be used

#CFLAGS += -mwpc                      
# Enable WPC platform extensions

