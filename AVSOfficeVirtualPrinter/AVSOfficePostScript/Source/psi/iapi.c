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

/* $Id: iapi.c 9043 2008-08-28 22:48:19Z giles $ */

/* Public Application Programming Interface to Ghostscript interpreter */

#include "string_.h"
#include "ierrors.h"
#include "gscdefs.h"
#include "gstypes.h"
#include "iapi.h"	/* Public API */
#include "iref.h"
#include "iminst.h"
#include "imain.h"
#include "imainarg.h"
#include "gsmemory.h"
#include "gsmalloc.h"
#include "gslibctx.h"

/* number of threads to allow per process
 * currently more than 1 is guarenteed to fail 
 */
static int gsapi_instance_counter = 0;
static const int gsapi_instance_max = 1;

/* Create a new instance of Ghostscript. 
 * First instance per process call with *pinstance == NULL
 * next instance in a proces call with *pinstance == copy of valid_instance pointer
 * *pinstance is set to a new instance pointer.
 */
GSDLLEXPORT int GSDLLAPI 
NewInstance(void **pinstance, void *caller_handle)
{
    gs_memory_t *mem = NULL;
    gs_main_instance *minst = NULL;

    if (pinstance == NULL)
	return e_Fatal;

    /* limited to 1 instance, till it works :) */
    if ( gsapi_instance_counter >= gsapi_instance_max ) 
	return e_Fatal;
    ++gsapi_instance_counter;

    if (*pinstance == NULL)
	/* first instance in this process */
	mem = gs_malloc_init(NULL);
    else {
	/* nothing different for second thread initialization 
	 * seperate memory, ids, only stdio is process shared.
	 */
	mem = gs_malloc_init(NULL);
	
    }
    minst = gs_main_alloc_instance(mem);
    mem->gs_lib_ctx->top_of_system = (void*) minst;
    mem->gs_lib_ctx->caller_handle = caller_handle;
    mem->gs_lib_ctx->custom_color_callback = NULL;
    mem->gs_lib_ctx->stdin_fn = NULL;
    mem->gs_lib_ctx->stdout_fn = NULL;
    mem->gs_lib_ctx->stderr_fn = NULL;
    mem->gs_lib_ctx->poll_fn = NULL;

    *pinstance = (void*)(mem->gs_lib_ctx);
    return 0;
}

/* Destroy an instance of Ghostscript */
/* We do not support multiple instances, so make sure
 * we use the default instance only once.
 */
GSDLLEXPORT void GSDLLAPI 
DeleteInstance(void *lib)
{
    gs_lib_ctx_t *ctx = (gs_lib_ctx_t *)lib;
    if ((ctx != NULL)) {
   	gs_main_instance *minst = get_minst_from_memory(ctx->memory);

	ctx->caller_handle = NULL;
	ctx->stdin_fn = NULL;
	ctx->stdout_fn = NULL;
	ctx->stderr_fn = NULL;
	ctx->poll_fn = NULL;
	minst->display = NULL;
	
	/* Release the memory (frees up everything) */
        gs_malloc_release(minst->heap);
	
	--gsapi_instance_counter;
    }
}



/* Initialise the interpreter */
GSDLLEXPORT int GSDLLAPI 
InitWithArgs(void *lib, int argc, char **argv)
{
    gs_lib_ctx_t *ctx = (gs_lib_ctx_t *)lib;
    if (lib == NULL)
	return e_Fatal;
    return gs_main_init_with_args(get_minst_from_memory(ctx->memory), argc, argv);
}

GSDLLEXPORT int GSDLLAPI 
RunFile(void *lib, const char *file_name, 
	int user_errors, int *pexit_code)
{
    gs_lib_ctx_t *ctx = (gs_lib_ctx_t *)lib;
    if (lib == NULL)
	return e_Fatal;

    return gs_main_run_file(get_minst_from_memory(ctx->memory),
			    file_name, user_errors, pexit_code, 
			    &(get_minst_from_memory(ctx->memory)->error_object));
}


/* Exit the interpreter */
GSDLLEXPORT int GSDLLAPI 
Exit(void *lib)
{
    gs_lib_ctx_t *ctx = (gs_lib_ctx_t *)lib;
    if (lib == NULL)
	return e_Fatal;

    gs_to_exit(ctx->memory, 0);
    return 0;
}


/* end of iapi.c */
