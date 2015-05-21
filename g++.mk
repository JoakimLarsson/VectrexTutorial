#
# GNU g++ specifics to reduce code size, comments from 
#     https://gcc.gnu.org/onlinedocs/gcc-4.3.6/gcc/
#

#CFLAGS += -fno-enforce-eh-specs 
#    Don't generate code to check for violation of exception specifications at runtime. This option violates the 
#    C++ standard, but may be useful for reducing code size in production builds, much like defining `NDEBUG'. 
#    This does not give user code permission to throw exceptions in violation of the exception specifications; 
#    the compiler will still optimize based on the specifications, so throwing an unexpected exception will result 
#    in undefined behavior.

#CFLAGS += -fno-threadsafe-statics 
#    Do not emit the extra code to use the routines specified in the C++ ABI for thread-safe initialization of 
#    local statics. You can use this option to reduce code size slightly in code that doesn't need to be thread-safe. 

#CFLAGS += -fvisibility-inlines-hidden 
#    This switch declares that the user does not attempt to compare pointers to inline methods where the addresses of the two functions were taken in different shared objects.
#
#    The effect of this is that GCC may, effectively, mark inline methods with __attribute__ ((visibility ("hidden"))) 
#    so that they do not appear in the export table of a DSO and do not require a PLT indirection when used within the 
#    DSO. Enabling this option can have a dramatic effect on load and link times of a DSO as it massively reduces the 
#    size of the dynamic export table when the library makes heavy use of templates.
# 
#    The behavior of this switch is not quite the same as marking the methods as hidden directly, because it does not 
#    affect static variables local to the function or cause the compiler to deduce that the function is defined in only
#    one shared object.
#
#    You may mark a method as having a visibility explicitly to negate the effect of the switch for that method. For 
#    example, if you do want to compare pointers to a particular inline method, you might mark it as having default 
#    visibility. Marking the enclosing class with explicit visibility will have no effect.
#
#    Explicitly instantiated inline methods are unaffected by this option as their linkage might otherwise cross a 
#    shared library boundary.  

#CFLAGS += -fconserve-space 
#     Put uninitialized or runtime-initialized global variables into the common segment, as C does. 
#     This saves space in the executable at the cost of not diagnosing duplicate definitions. If you 
#     compile with this flag and your program mysteriously crashes after main() has completed, you may 
#     have an object that is being destroyed twice because two definitions were merged.
#
#    This option is no longer useful on most targets, now that support has been added for putting variables into BSS 
#    without making them common.

#CFLAGS += -fno-implement-inlines 
#    To save space, do not emit out-of-line copies of inline functions controlled by `#pragma implementation'. 
#    This will cause linker errors if these functions are not inlined everywhere they are called.

#CFLAGS += -fno-rtti
#    Disable generation of information about every class with virtual functions for use by the C++ runtime type 
#    identification features (`dynamic_cast' and `typeid'). If you don't use those parts of the language, you can save 
#    some space by using this flag. Note that exception handling uses the same information, but it will generate it as 
#    needed. The `dynamic_cast' operator can still be used for casts that do not require runtime type information, i.e.
#    casts to void * or to unambiguous base classes. 

