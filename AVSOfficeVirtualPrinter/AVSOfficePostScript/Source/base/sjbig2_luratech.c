/* Copyright (C) 2001-2008 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: sjbig2_luratech.c 8784 2008-05-28 18:16:26Z giles $ */
/* jbig2decode filter implementation -- hooks in luratech JBIG2 */

#include "memory_.h"
#include "malloc_.h"  /* should use a gs mem pointer */
#include "gserrors.h"
#include "gserror.h"
#include "gdebug.h"
#include "strimpl.h"
#include "sjbig2_luratech.h"

#include <ldf_jb2.h>

/* JBIG2Decode stream implementation using the Luratech library */

/* if linking against a SDK build that requires a separate license key,
   you can change the following undefs to defines and set them here. */
/***
#ifndef JB2_LICENSE_NUM_1
# undef JB2_LICENSE_NUM_1 
#endif
#ifndef JB2_LICENSE_NUM_2
# undef JB2_LICENSE_NUM_2 
#endif
***/

/* The /JBIG2Decode filter is a fairly memory intensive one to begin with,
   Furthermore, as a PDF 1.4 feature, we can assume a fairly large 
   (host-level) machine. We therefore dispense with the normal 
   Ghostscript memory discipline and let the library allocate all its 
   resources on the heap. The pointers to these are not enumerated and 
   so will not be garbage collected. We rely on our release() proc being 
   called to deallocate state.
 */
 /* TODO: check allocations for integer overflow */

/* create a gc object for our state, defined in sjbig2_luratech.h */
private_st_jbig2decode_state();	

#define JBIG2_BUFFER_SIZE 4096

/* our implementation of the "parsed" /JBIG2Globals filter parameter */
typedef struct s_jbig2decode_global_data_s {
    unsigned char *data;
    unsigned long size;
} s_jbig2decode_global_data;

/* create a global data struct and copy data into it */
int
s_jbig2decode_make_global_data(byte *data, uint size, void **result)
{
    s_jbig2decode_global_data *global = NULL;

    global = malloc(sizeof(*global));
    if (global == NULL) return gs_error_VMerror;

    global->data = malloc(size);
    if (global->data == NULL) {
	free(global);
	return gs_error_VMerror;
    }
    memcpy(global->data, data, size);
    global->size = size;

    *result = global;
    return 0;
}

/* free a global data struct and its data */
void
s_jbig2decode_free_global_data(void *data)
{
    s_jbig2decode_global_data *global = (s_jbig2decode_global_data*)data;

    if (global->size && global->data) {
	free(global->data);
	global->size = 0;
    }
    free(global);
}

/* store a global ctx pointer in our state structure */
int
s_jbig2decode_set_global_data(stream_state *ss, s_jbig2_global_data_t *gd)
{
    stream_jbig2decode_state *state = (stream_jbig2decode_state*)ss;
    if (state == NULL)
        return gs_error_VMerror;
    
    state->global_struct = gd;
    if (gd != NULL) {
        s_jbig2decode_global_data *global = (s_jbig2decode_global_data*)(gd->data);
        state->global_data = global->data;
        state->global_size = global->size;
    } else {
        state->global_data = NULL;
        state->global_size = 0;
    }
    return 0;
}

/* invert the bits in a buffer */
/* jbig2 and postscript have different senses of what pixel
   value is black, so we must invert the image */
static void
s_jbig2_invert_buffer(unsigned char *buf, int length)
{
    int i;
    
    for (i = 0; i < length; i++)
        *buf++ ^= 0xFF;
}

/** callbacks passed to the luratech library */

/* memory allocator */
static void * JB2_Callback
s_jbig2_alloc(unsigned long size, void *userdata)
{
    void *result = malloc(size);
    return result;
}

/* memory release */
static JB2_Error JB2_Callback
s_jbig2_free(void *ptr, void *userdata)
{
    free(ptr);
    return cJB2_Error_OK;
}

/* error callback for jbig2 codec */
static void JB2_Callback
s_jbig2_message(const char *message, JB2_Message_Level level, void *userdata)
{
    const char *type;

    if (message == NULL) return;
    if (message[0] == '\0') return;

    switch (level) {
	case cJB2_Message_Information:
            type = "info"; break;;
	case cJB2_Message_Warning:
	    type = "WARNING"; break;;
	case cJB2_Message_Error:
	    type = "ERROR"; break;;
	default:
	    type = "unknown message"; break;;
    }

    if (level == cJB2_Message_Error) {
	dprintf2("Luratech JBIG2 %s %s\n", type, message);
    } else {
	if_debug2('w', "[w]Luratech JBIG2 %s %s\n", type, message);
    }

    return;
}

/* compressed read callback for jbig2 codec */
static JB2_Size_T JB2_Callback
s_jbig2_read(unsigned char *buffer, 
	JB2_Size_T offset, JB2_Size_T size, void *userdata)
{
    stream_jbig2decode_state *const state = (stream_jbig2decode_state *) userdata;
    long available;

    /* return data from the Globals stream */
    if (offset < state->global_size) {
	available = state->global_size - offset;
	if (available > size) available = size;
	memcpy(buffer, state->global_data + offset, available);
	return available;
    }

    /* else return data from the image stream */
    offset -= state->global_size;
    available = state->infill - offset;
    if (available > size) available = size;
    if (available <= 0) return 0;

    memcpy(buffer, state->inbuf + offset, available);
    return available;
}

/* uncompressed write callback for jbig2 codec */
static JB2_Error JB2_Callback
s_jbig2_write(unsigned char *buffer,
		unsigned long row, unsigned long width,
		unsigned long bbp, void *userdata)
{
    stream_jbig2decode_state *const state = (stream_jbig2decode_state *) userdata;
    unsigned char *line = state->image + row*state->stride;
    long available = ((width - 1) >> 3) + 1; 

    if (row >= state->height) {
	dlprintf2("jbig2decode: output for row index %lu of %lu called!\n", row, state->height);
	return cJB2_Error_Invalid_Index;
    }

    memcpy(line, buffer, available);
    s_jbig2_invert_buffer(line, available);

    return cJB2_Error_OK;
}


static int
s_jbig2decode_inbuf(stream_jbig2decode_state *state, stream_cursor_read * pr)
{
    long in_size = pr->limit - pr->ptr;

    /* allocate the input buffer if needed */
    if (state->inbuf == NULL) {
        state->inbuf = malloc(JBIG2_BUFFER_SIZE);
        if (state->inbuf == NULL) return gs_error_VMerror;
        state->insize = JBIG2_BUFFER_SIZE;
        state->infill = 0;
    }

    /* grow the input buffer if needed */
    while (state->insize < state->infill + in_size) {
        unsigned char *new;
        unsigned long new_size = state->insize;

        while (new_size < state->infill + in_size)
            new_size = new_size << 1;

        if_debug1('s', "[s]jbig2decode growing input buffer to %lu bytes\n",
                new_size);
        new = realloc(state->inbuf, new_size);
        if (new == NULL) return gs_error_VMerror;

        state->inbuf = new;
        state->insize = new_size;
    }

    /* copy the available input into our buffer */
    /* note that the gs stream library uses offset-by-one
        indexing of its buffers while we use zero indexing */
    memcpy(state->inbuf + state->infill, pr->ptr + 1, in_size);
    state->infill += in_size;
    pr->ptr += in_size;

    return 0;
}

/* initialize the steam. */
static int
s_jbig2decode_init(stream_state * ss)
{
    stream_jbig2decode_state *const state = (stream_jbig2decode_state *) ss;

    state->doc = NULL;
    state->inbuf = NULL;
    state->insize = 0;
    state->infill = 0;
    state->image = NULL;
    state->width = 0;
    state->height = 0;
    state->row = 0;
    state->stride = 0;
    state->error = 0;
    state->offset = 0;

    return 0;
}

/* process a section of the input and return any decoded data.
   see strimpl.h for return codes.
 */
static int
s_jbig2decode_process(stream_state * ss, stream_cursor_read * pr,
		  stream_cursor_write * pw, bool last)
{
    stream_jbig2decode_state *const state = (stream_jbig2decode_state *) ss;
    long in_size = pr->limit - pr->ptr;
    long out_size = pw->limit - pw->ptr;
    long available;
    JB2_Error error;
    JB2_Scaling_Factor scale = {1,1};
    JB2_Rect rect = {0,0,0,0};
    ulong result;
    int status = last;
    
    if (in_size > 0) {
        /* buffer all available input for the decoder */
	result = s_jbig2decode_inbuf(state, pr);
	if (result) return ERRC;
    }

    if (last && out_size > 0) {

	if (state->doc == NULL) {

	    /* initialize the codec state and pass our callbacks */
	    error = JB2_Document_Start( &(state->doc),
		s_jbig2_alloc, ss,	/* alloc and its data */
		s_jbig2_free, ss, 	/* free and its data */
		s_jbig2_read, ss,	/* read callback and data */
		s_jbig2_message, ss);   /* message callback and data */
	    if (error != cJB2_Error_OK) return ERRC;

#if defined(JB2_LICENSE_NUM_1) && defined(JB2_LICENSE_NUM_2)
	    /* set the license keys if appropriate */
	    error = JB2_Document_Set_License(state->doc, 
		JB2_LICENSE_NUM_1, JB2_LICENSE_NUM_2);
	    if (error != cJB2_Error_OK) return ERRC;
#endif
	    /* decode relevent image parameters */
	    error = JB2_Document_Set_Page(state->doc, 0);
	    if (error != cJB2_Error_OK) return ERRC;
	    error = JB2_Document_Get_Property(state->doc, 
		cJB2_Prop_Page_Width, &result);
	    state->width = result;
	    error = JB2_Document_Get_Property(state->doc, 
		cJB2_Prop_Page_Height, &result);
	    if (error != cJB2_Error_OK) return ERRC;
	    state->height = result;
	    state->stride = ((state->width - 1) >> 3) + 1;
	    if_debug2('w', "[w]jbig2decode page is %ldx%ld; allocating image\n", state->width, state->height);
	    state->image = malloc(state->height*state->stride);

	    /* start image decode */
	    error = JB2_Document_Decompress_Page(state->doc, scale, rect,
		s_jbig2_write, ss);
	    if (error != cJB2_Error_OK) return ERRC;

	}

	/* copy any buffered image data out */
	available = state->stride*state->height - state->offset;
	if (available > 0) {
	    out_size = (out_size > available) ? available : out_size;
	    memcpy(pw->ptr+1, state->image + state->offset, out_size);
	    state->offset += out_size;
	    pw->ptr += out_size;
	}
	/* more data to output? */
	available = state->stride*state->height - state->offset;
	if (available > 0) return 1; 
	else return EOFC;
    }
    
    /* handle fatal decoding errors reported through our callback */
    if (state->error) return ERRC;
    
    return status;
}

/* stream release. free all our decoder state. */
static void
s_jbig2decode_release(stream_state *ss)
{
    stream_jbig2decode_state *const state = (stream_jbig2decode_state *) ss;

    if (state->doc) {
	JB2_Document_End(&(state->doc));
        if (state->inbuf) free(state->inbuf);
	if (state->image) free(state->image);
    }
    /* the interpreter calls jbig2decode_free_global_data() separately */
}

/* stream template */
const stream_template s_jbig2decode_template = {
    &st_jbig2decode_state, 
    s_jbig2decode_init,
    s_jbig2decode_process,
    1, 1, /* min in and out buffer sizes we can handle --should be ~32k,64k for efficiency? */
    s_jbig2decode_release
};



/** encode support **/

/* we provide a C-only encode filter for generating embedded JBIG2 image 
   data */

/* create a gc object for our state, defined in sjbig2_luratech.h */
private_st_jbig2encode_state();

/* helper - start up the compression context */
static int
s_jbig2encode_start(stream_jbig2encode_state *state)
{
    JB2_Error err;

    /* initialize the compression handle */
    err = JB2_Compress_Start(&(state->cmp),
	s_jbig2_alloc, state,	/* alloc and its parameter data */
	s_jbig2_free,  state,	/* free callback */
	s_jbig2_message, state);/* message callback */
    if (err != cJB2_Error_OK) return err;

            /* set the license keys if appropriate */
#if defined(JB2_LICENSE_NUM_1) && defined(JB2_LICENSE_NUM_2)
            err = JB2_Document_Set_License(state->cmp,
                JB2_LICENSE_NUM_1, JB2_LICENSE_NUM_2);
            if (err != cJB2_Error_OK) return err;
#endif

   /* set the image properties */
   err = JB2_Compress_Set_Property(state->cmp,
		cJB2_Prop_Page_Width, state->width);

   err = JB2_Compress_Set_Property(state->cmp,
		cJB2_Prop_Page_Height, state->height);
   if (err != cJB2_Error_OK) return err;

   /* we otherwise use the default compression parameters */

   return cJB2_Error_OK;
}

/* callback for compressed data output */
static JB2_Size_T JB2_Callback
s_jbig2encode_write(const unsigned char *buffer,
		JB2_Size_T pos, JB2_Size_T size, void *userdata)
{
    stream_jbig2encode_state *state = (stream_jbig2encode_state *)userdata;

    /* allocate the output buffer if necessary */
    if (state->outbuf == NULL) {
	state->outbuf = malloc(JBIG2_BUFFER_SIZE);
	if (state->outbuf == NULL) {
	    dprintf("jbig2encode: failed to allocate output buffer\n");
	    return 0; /* can't return an error! */
	}
	state->outsize = JBIG2_BUFFER_SIZE;
    }

    /* grow the output buffer if necessary */
    while (pos+size > state->outsize) {
	unsigned char *new = realloc(state->outbuf, state->outsize*2);
	if (new == NULL) {
	    dprintf1("jbig2encode: failed to resize output buffer"
		" beyond %lu bytes\n", state->outsize);
	    return 0; /* can't return an error! */
	}
	state->outbuf = new;
	state->outsize *= 2;
    }

    /* copy data into our buffer; there will now be enough room. */
    memcpy(state->outbuf + pos, buffer, size);
    if (state->outfill < pos + size) state->outfill = pos + size;

    return size;
}                             


/* initialize the steam. */
static int
s_jbig2encode_init(stream_state * ss)
{
    stream_jbig2encode_state *state = (stream_jbig2encode_state *)ss;

    /* null library context handles */
    state->cmp = (JB2_Handle_Compress)NULL;
    state->doc = (JB2_Handle_Document)NULL;

    /* width and height are set by the client */
    /* calculate a stride based on those values */
    state->stride = ((state->width - 1) >> 3) + 1;

    state->line = malloc(state->stride);
    if (state->line == NULL) return ERRC;
    state->linefill = 0;

    /* null output buffer */
    state->outbuf = NULL;
    state->outsize = 0;
    state->outfill = 0;
    state->offset = 0;

    return 0;
}

/* process a section of the input and return any encoded data.
   see strimpl.h for return codes.
 */
static int
s_jbig2encode_process(stream_state * ss, stream_cursor_read * pr,
                  stream_cursor_write * pw, bool last)
{
    stream_jbig2encode_state *state = (stream_jbig2encode_state *)ss;
    long in_size = pr->limit - pr->ptr;
    long out_size = pw->limit - pw->ptr;
    long available, segment;
    JB2_Error err;

    /* Be greedy in filling our internal line buffer so we always
       make read progress on a stream. */
    if (in_size > 0) {
	/* initialize the encoder if necessary */
	if (state->cmp == (JB2_Handle_Compress)NULL)
	    s_jbig2encode_start(state);

	available = in_size;

	/* try to fill the line buffer */
	segment = state->stride - state->linefill;
	if (segment > 0) {
	    segment = (segment < available) ? segment : available;
	    memcpy(state->line + state->linefill, pr->ptr+1, segment);
	    pr->ptr += segment;
	    available -= segment;
	    state->linefill += segment;
	}
	/* pass a full line buffer to the encoder library */
	if (state->linefill == state->stride) {
	    s_jbig2_invert_buffer(state->line, state->stride);
	    err = JB2_Compress_Line(state->cmp, state->line);
	    state->linefill = 0;
	    if (err != cJB2_Error_OK) return ERRC;
	}
	/* pass remaining full lines to the encoder library */
	while (available >= state->stride) {
	   memcpy(state->line, pr->ptr+1, state->stride);
	   s_jbig2_invert_buffer(state->line, state->stride);
	   err = JB2_Compress_Line(state->cmp, state->line);
	   pr->ptr += state->stride;
	   available = pr->limit - pr->ptr;
	   if (err != cJB2_Error_OK) return ERRC;
	}
	/* copy remaining data into the line buffer */
	if (available > 0) {
	    /* available is always < stride here */
	    memcpy(state->line, pr->ptr+1, available);
	    pr->ptr += available;
	    state->linefill = available;
	}
	if (!last) return 0; /* request more data */
    }

    if (last && state->outbuf == NULL) {
	/* convert the compression context to a document context */
	err = JB2_Compress_End(&(state->cmp), &(state->doc));
	if (err != cJB2_Error_OK) return ERRC;
	/* dump the compressed data out through a callback; 
	   unfortunately we can't serialize this across process calls */
	err = JB2_Document_Export_Document(state->doc,
	    s_jbig2encode_write, state,
	    cJB2_Export_Format_Stream_For_PDF);
	if (err != cJB2_Error_OK) return ERRC;
    }

    if (state->outbuf != NULL) {
	/* copy available output data */
	available = min(out_size, state->outfill - state->offset);
	memcpy(pw->ptr + 1, state->outbuf + state->offset, available);
	pw->ptr += available;
	state->offset += available;

	/* need further output space? */
	if (state->outfill - state->offset > 0) return 1;
	else return EOFC; /* all done */
    }

    /* something went wrong above */
    return ERRC;
}

/* stream release. free all our decoder state.
 */
static void
s_jbig2encode_release(stream_state *ss)
{
    stream_jbig2encode_state *state = (stream_jbig2encode_state *)ss;

    if (state->outbuf != NULL) free(state->outbuf);
    if (state->line != NULL) free(state->line);
}

/* stream template */
const stream_template s_jbig2encode_template = {
    &st_jbig2encode_state,
    s_jbig2encode_init,
    s_jbig2encode_process,
    1024, 1024, /* min in and out buffer sizes; could be smaller, but 
		   this is more efficient */
    s_jbig2encode_release
};
