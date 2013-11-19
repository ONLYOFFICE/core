/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/*$Id: gslibctx.h 8764 2008-05-21 18:27:42Z mvrhel $ */
#ifndef GSLIBCTX_H 
#define GSLIBCTX_H 

#include "std.h"
#include "stdio_.h"
#include "gs_dll_call.h"


typedef struct name_table_s *name_table_ptr;

typedef struct gs_lib_ctx_s
{  
    gs_memory_t *memory;  /* mem->gs_lib_ctx->memory == mem */
    FILE *fstdin;
    FILE *fstdout;
    FILE *fstderr;
    FILE *fstdout2;		/* for redirecting %stdout and diagnostics */
    bool stdout_is_redirected;	/* to stderr or fstdout2 */
    bool stdout_to_stderr;
    bool stdin_is_interactive;   
    void *caller_handle;	/* identifies caller of GS DLL/shared object */
    void *custom_color_callback;  /* pointer to color callback structure */
    int (GSDLLCALL *stdin_fn)(void *caller_handle, char *buf, int len);
    int (GSDLLCALL *stdout_fn)(void *caller_handle, const char *str, int len);
    int (GSDLLCALL *stderr_fn)(void *caller_handle, const char *str, int len);
    int (GSDLLCALL *poll_fn)(void *caller_handle);  
    ulong gs_next_id; /* gs_id initialized here, private variable of gs_next_ids() */
    void *top_of_system;  /* use accessor functions to walk down the system 
			   * to the desired structure gs_lib_ctx_get_*()
			   */
    name_table_ptr gs_name_table;  /* hack this is the ps interpreters name table 
				    * doesn't belong here 
				    */
    /* Define whether dictionaries expand automatically when full. */
    bool dict_auto_expand;  /* ps dictionary: false level 1 true level 2 or 3 */
} gs_lib_ctx_t;

/** initializes and stores itself in the given gs_memory_t pointer.
 * it is the responsibility of the gs_memory_t objects to copy 
 * the pointer to subsequent memory objects.
 */
int gs_lib_ctx_init( gs_memory_t *mem ); 

void *gs_lib_ctx_get_interp_instance( gs_memory_t *mem );

/* HACK to get at non garbage collection memory pointer 
 * 
 */
gs_memory_t * gs_lib_ctx_get_non_gc_memory_t(void);
 
#endif /* GSLIBCTX_H */
