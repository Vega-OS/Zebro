/*
 *  @description: Wrappers over system memory functions.
 *  @author: Ian Marco Moffett.
 */

#ifndef SYSDEPS_MEM_H
#define SYSDEPS_MEM_H

#if defined(__unix__)
# include <stdlib.h>
# define zebro_malloc(size) malloc(size)
# define zebro_calloc(nmemb, size) calloc(nmemb, size)
# define zebro_realloc(ptr, size) realloc(ptr, size)
# define zebro_free(ptr) free(ptr)
#elif defined(_WIN32) || defined(__CYGWIN__)
# error "Not supported on windows"
#endif

#endif
