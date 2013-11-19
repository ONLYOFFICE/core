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

/* $Id: sjbig2.c 8456 2007-12-21 10:31:00Z ken $ */
/* jbig2decode filter implementation -- hooks in libjbig2dec */

#include "stdint_.h"
#include "memory_.h"
#include "stdio_.h" /* sprintf() for debug output */

#include "gserrors.h"
#include "gserror.h"
#include "gdebug.h"
#include "strimpl.h"
#include "sjbig2.h"

/* stream implementation */

/* The /JBIG2Decode filter is a fairly memory intensive one to begin with,
   particularly in the initial jbig2dec library implementation. Furthermore,
   as a PDF 1.4 feature, we can assume a fairly large (host-level) machine.
   We therefore dispense with the normal Ghostscript memory discipline and
   let the library allocate all its resources on the heap. The pointers to
   these are not enumerated and so will not be garbage collected. We rely
   on our release() proc being called to deallocate state.
 */

private_st_jbig2decode_state();	/* creates a gc object for our state, defined in sjbig2.h */

/* error callback for jbig2 decoder */
static int
s_jbig2decode_error(void *error_callback_data, const char *msg, Jbig2Severity severity,
	       int32_t seg_idx)
{
    stream_jbig2decode_state *const state = 
	(stream_jbig2decode_state *) error_callback_data;
    const char *type;
    char segment[22];
    int code = 0;

    switch (severity) {
        case JBIG2_SEVERITY_DEBUG:
            type = "DEBUG"; break;;
        case JBIG2_SEVERITY_INFO:
            type = "info"; break;;
        case JBIG2_SEVERITY_WARNING:
            type = "WARNING"; break;;
        case JBIG2_SEVERITY_FATAL:
            type = "FATAL ERROR decoding image:";
            /* pass the fatal error upstream if possible */
	    code = gs_error_ioerror;
	    if (state != NULL) state->error = code; 
	    break;;
        default: type = "unknown message:"; break;;
    }
    if (seg_idx == -1) segment[0] = '\0';
    else sprintf(segment, "(segment 0x%02x)", seg_idx);

    if (severity == JBIG2_SEVERITY_FATAL) {
	dlprintf3("jbig2dec %s %s %s\n", type, msg, segment);
    } else {
	if_debug3('w', "[w] jbig2dec %s %s %s\n", type, msg, segment);
    }

    return code;
}

/* invert the bits in a buffer */
/* jbig2 and postscript have different senses of what pixel
   value is black, so we must invert the image */
static void
s_jbig2decode_invert_buffer(unsigned char *buf, int length)
{
    int i;
    
    for (i = 0; i < length; i++)
        *buf++ ^= 0xFF;
}

/* parse a globals stream packed into a gs_bytestring for us by the postscript
   layer and stuff the resulting context into a pointer for use in later decoding */
int
s_jbig2decode_make_global_data(byte *data, uint length, void **result)
{
    Jbig2Ctx *ctx = NULL;
    int code;
    
    /* the cvision encoder likes to include empty global streams */
    if (length == 0) {
        if_debug0('w', "[w] ignoring zero-length jbig2 global stream.\n");
    	*result = NULL;
    	return 0;
    }
    
    /* allocate a context with which to parse our global segments */
    ctx = jbig2_ctx_new(NULL, JBIG2_OPTIONS_EMBEDDED, NULL, 
                            s_jbig2decode_error, NULL);
    
    /* parse the global bitstream */
    code = jbig2_data_in(ctx, data, length);
    
    if (code) {
	/* error parsing the global stream */
	*result = NULL;
	return code;
    }

    /* canonize and store our global state */
    *result = jbig2_make_global_ctx(ctx);
    
    return 0; /* todo: check for allocation failure */
}

/* release a global ctx pointer */
void
s_jbig2decode_free_global_data(void *data)
{
    Jbig2GlobalCtx *global_ctx = (Jbig2GlobalCtx*)data;

    jbig2_global_ctx_free(global_ctx);
}

/* store a global ctx pointer in our state structure */
int
s_jbig2decode_set_global_data(stream_state *ss, s_jbig2_global_data_t *gd)
{
    stream_jbig2decode_state *state = (stream_jbig2decode_state*)ss;
    state->global_struct = gd;
    state->global_ctx = (Jbig2GlobalCtx*)(gd ? gd->data : 0);
    return 0;
}

/* initialize the steam.
   this involves allocating the context structures, and
   initializing the global context from the /JBIG2Globals object reference
 */
static int
s_jbig2decode_init(stream_state * ss)
{
    stream_jbig2decode_state *const state = (stream_jbig2decode_state *) ss;
    Jbig2GlobalCtx *global_ctx = state->global_ctx; /* may be NULL */
    
    /* initialize the decoder with the parsed global context if any */
    state->decode_ctx = jbig2_ctx_new(NULL, JBIG2_OPTIONS_EMBEDDED,
                global_ctx, s_jbig2decode_error, ss);
    state->image = 0;
    state->error = 0;
    return 0; /* todo: check for allocation failure */
}

/* process a section of the input and return any decoded data.
   see strimpl.h for return codes.
 */
static int
s_jbig2decode_process(stream_state * ss, stream_cursor_read * pr,
		  stream_cursor_write * pw, bool last)
{
    stream_jbig2decode_state *const state = (stream_jbig2decode_state *) ss;
    Jbig2Image *image = state->image;
    long in_size = pr->limit - pr->ptr;
    long out_size = pw->limit - pw->ptr;
    int status = 0;
    
    /* there will only be a single page image, 
       so pass all data in before looking for any output.
       note that the gs stream library expects offset-by-one
       indexing of the buffers, while jbig2dec uses normal 0 indexes */
    if (in_size > 0) {
        /* pass all available input to the decoder */
        jbig2_data_in(state->decode_ctx, pr->ptr + 1, in_size);
        pr->ptr += in_size;
        /* simulate end-of-page segment */
        if (last == 1) {
            jbig2_complete_page(state->decode_ctx);
        }
	/* handle fatal decoding errors reported through our callback */
	if (state->error) return state->error;
    }
    if (out_size > 0) {
        if (image == NULL) {
            /* see if a page image in available */
            image = jbig2_page_out(state->decode_ctx);
            if (image != NULL) {
                state->image = image;
                state->offset = 0;
            }
        }
        if (image != NULL) {
            /* copy data out of the decoded image, if any */
            long image_size = image->height*image->stride;
            long usable = min(image_size - state->offset, out_size);
            memcpy(pw->ptr + 1, image->data + state->offset, usable);
            s_jbig2decode_invert_buffer(pw->ptr + 1, usable);
            state->offset += usable;
            pw->ptr += usable;
            status = (state->offset < image_size) ? 1 : 0;
        }
    }    
    
    return status;
}

/* stream release.
   free all our decoder state.
 */
static void
s_jbig2decode_release(stream_state *ss)
{
    stream_jbig2decode_state *const state = (stream_jbig2decode_state *) ss;

    if (state->decode_ctx) {
        if (state->image) jbig2_release_page(state->decode_ctx, state->image);
        jbig2_ctx_free(state->decode_ctx);
    }
    /* the interpreter takes care of freeing the global_ctx */
}

/* set stream defaults.
   this hook exists to avoid confusing the gc with bogus
   pointers. we use it similarly just to NULL all the pointers.
   (could just be done in _init?)
 */
static void
s_jbig2decode_set_defaults(stream_state *ss)
{
    stream_jbig2decode_state *const state = (stream_jbig2decode_state *) ss;
    
    /* state->global_ctx is not owned by us */
    state->global_struct = NULL;
    state->global_ctx = NULL;
    state->decode_ctx = NULL;
    state->image = NULL;
    state->offset = 0;
    state->error = 0;
}


/* stream template */
const stream_template s_jbig2decode_template = {
    &st_jbig2decode_state, 
    s_jbig2decode_init,
    s_jbig2decode_process,
    1, 1, /* min in and out buffer sizes we can handle --should be ~32k,64k for efficiency? */
    s_jbig2decode_release,
    s_jbig2decode_set_defaults
};
