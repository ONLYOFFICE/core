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

/* $Id: zfjbig2.c 9043 2008-08-28 22:48:19Z giles $ */

/* this is the ps interpreter interface to the jbig2decode filter
   used for (1bpp) scanned image compression. PDF only specifies
   a decoder filter, and we don't currently implement anything else */

#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "gstypes.h"
#include "ialloc.h"
#include "idict.h"
#include "store.h"
#include "stream.h"
#include "strimpl.h"
#include "ifilter.h"

#ifdef USE_LDF_JB2
#include "sjbig2_luratech.h"
#else
#include "sjbig2.h"
#endif

/* We define a structure, s_jbig2_global_data_t, 
   allocated in the postscript
   memory space, to hold a pointer to the global decoder
   context (which is allocated by libjbig2). This allows
   us to pass the reference through postscript code to
   the filter initializer. The opaque data pointer is not
   enumerated and will not be garbage collected. We use
   a finalize method to deallocate it when the reference
   is no longer in use. */
   
static void jbig2_global_data_finalize(void *vptr);
gs_private_st_simple_final(st_jbig2_global_data_t, s_jbig2_global_data_t,
	"jbig2globaldata", jbig2_global_data_finalize);


/* <source> /JBIG2Decode <file> */
/* <source> <dict> /JBIG2Decode <file> */
static int
z_jbig2decode(i_ctx_t * i_ctx_p)
{
    os_ptr op = osp;
    ref *sop = NULL;
    s_jbig2_global_data_t *gref;
    stream_jbig2decode_state state;

    /* Extract the global context reference, if any, from the parameter
       dictionary and embed it in our stream state. The original object 
       ref is under the JBIG2Globals key.
       We expect the postscript code to resolve this and call 
       z_jbig2makeglobalctx() below to create an astruct wrapping the
       global decoder data and store it under the .jbig2globalctx key
     */
    s_jbig2decode_set_global_data((stream_state*)&state, NULL);
    if (r_has_type(op, t_dictionary)) {
        check_dict_read(*op);
        if ( dict_find_string(op, ".jbig2globalctx", &sop) > 0) {
	    gref = r_ptr(sop, s_jbig2_global_data_t);
	    s_jbig2decode_set_global_data((stream_state*)&state, gref);
        }
    }
    	
    /* we pass npop=0, since we've no arguments left to consume */
    /* we pass 0 instead of the usual rspace(sop) which will allocate storage
       for filter state from the same memory pool as the stream it's coding.
       this causes no trouble because we maintain no pointers */
    return filter_read(i_ctx_p, 0, &s_jbig2decode_template,
		       (stream_state *) & state, 0);
}


/* <bytestring> .jbig2makeglobalctx <jbig2globalctx> */
/* we call this from ps code to instantiate a jbig2_global_context
   object which the JBIG2Decode filter uses if available. The
   pointer to the global context is stored in an astruct object
   and returned that way since it lives outside the interpreters
   memory management */
static int
z_jbig2makeglobalctx(i_ctx_t * i_ctx_p)
{
	void *global = NULL;
	s_jbig2_global_data_t *st;
	os_ptr op = osp;
	byte *data;
	int size;
	int code = 0;

	check_type(*op, t_astruct);
	size = gs_object_size(imemory, op->value.pstruct);
	data = r_ptr(op, byte);

 	code = s_jbig2decode_make_global_data(data, size,
			&global);
	if (size > 0 && global == NULL) {
	    dlprintf("failed to create parsed JBIG2GLOBALS object.");
	    return_error(e_unknownerror);
	}
	
	st = ialloc_struct(s_jbig2_global_data_t, 
		&st_jbig2_global_data_t,
		"jbig2decode parsed global context");
	if (st == NULL) return_error(e_VMerror);
	
	st->data = global;
	make_astruct(op, a_readonly | icurrent_space, (byte*)st);
	
	return code;
}

/* free our referenced global context data */
static void jbig2_global_data_finalize(void *vptr)
{
	s_jbig2_global_data_t *st = vptr;
	
	if (st->data) s_jbig2decode_free_global_data(st->data);
	st->data = NULL;
}
   
/* Match the above routine to the corresponding filter name.
   This is how our static routines get called externally. */
const op_def zfjbig2_op_defs[] = {
    {"1.jbig2makeglobalctx", z_jbig2makeglobalctx},
    op_def_begin_filter(),
    {"2JBIG2Decode", z_jbig2decode},
    op_def_end(0)
};
