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

/* $Id: iminst.h 9043 2008-08-28 22:48:19Z giles $ */
/* Definition of interpreter instance */
/* Requires stdio_.h, gsmemory.h, iref.h, iapi.h */

#ifndef iminst_INCLUDED
#  define iminst_INCLUDED

#ifndef gs_main_instance_DEFINED
#  define gs_main_instance_DEFINED
typedef struct gs_main_instance_s gs_main_instance;
#endif

/*
 * Define the structure of a search path.  Currently there is only one,
 * but there might be more someday.
 *
 *      container - an array large enough to hold the specified maximum
 * number of directories.  Both the array and all the strings in it are
 * in the 'foreign' VM space.
 *      list - the initial interval of container that defines the actual
 * search list.
 *      env - the contents of an environment variable, implicitly added
 * at the end of the list; may be 0.
 *      final - the final set of directories specified in the makefile;
 * may be 0.
 *      count - the number of elements in the list, excluding a possible
 * initial '.', env, and final.
 */
typedef struct gs_file_path_s {
    ref container;
    ref list;
    const char *env;
    const char *final;
    uint count;
} gs_file_path;

/*
 * Here is where we actually define the structure of interpreter instances.
 * Clients should not reference any of the members.  Note that in order to
 * be able to initialize this structure statically, members including
 * unions must come last (and be initialized to 0 by default).
 */
struct gs_main_instance_s {
    /* The following are set during initialization. */
    gs_memory_t *heap;		/* (C) heap allocator */
    uint memory_chunk_size;	/* 'wholesale' allocation unit */
    ulong name_table_size;
    uint run_buffer_size;
    int init_done;		/* highest init done so far */
    int user_errors;		/* define what to do with errors */
    bool search_here_first;	/* if true, make '.' first lib dir */
    bool run_start;		/* if true, run 'start' after */
				/* processing command line */
    gs_file_path lib_path;	/* library search list (GS_LIB) */
    long base_time[2];		/* starting usertime */
    void *readline_data;	/* data for gp_readline */
    ref error_object;		/* Use by gsapi_*() */
#if 1
    /* needs to be removed */
    display_callback *display;	/* callback structure for display device */
#endif

    /* The following are updated dynamically. */
    i_ctx_t *i_ctx_p;		/* current interpreter context state */
};

/*
 * Note that any file that uses the following definition of default values
 * must include gconfig.h, because of SEARCH_HERE_FIRST.
 */
#define gs_main_instance_default_init_values\
  0, 20000, 0, 0, -1, 0, SEARCH_HERE_FIRST, 1
extern const gs_main_instance gs_main_instance_init_values;

#endif /* iminst_INCLUDED */
