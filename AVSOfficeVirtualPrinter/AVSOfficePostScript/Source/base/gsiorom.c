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

/* $Id: gsiorom.c 8878 2008-07-27 02:52:11Z alexcher $ */
/* %rom% IODevice implementation for a compressed in-memory filesystem */
 
/*
 * This file implements a special %rom% IODevice designed for embedded
 * use. It accesses a compressed filesytem image which may be stored
 * in literal ROM, or more commonly is just static data linked directly
 * into the executable. This can be used for storing postscript library
 * files, fonts, Resources or other data files that Ghostscript needs
 * to run.
 */

#include "std.h"
#include "stdint_.h"
#include "string_.h"
#include "gsiorom.h"
#include "gx.h"
#include "gpcheck.h"
#include "gserrors.h"
#include "gsstruct.h"
#include "gsutil.h"
#include "gxiodev.h"
#include "stream.h"
#include "stat_.h"
#include "zlib.h"

/* device method prototypes */
static iodev_proc_init(romfs_init);
static iodev_proc_open_file(romfs_open_file);
static iodev_proc_file_status(romfs_file_status);
static iodev_proc_enumerate_files(romfs_enumerate_files_init);
static iodev_proc_enumerate_next(romfs_enumerate_next);
static iodev_proc_enumerate_close(romfs_enumerate_close);
/* close is handled by stream closure */

/* device definition */
const gx_io_device gs_iodev_rom =
{
    "%rom%", "FileSystem",
    {romfs_init, iodev_no_open_device,
     romfs_open_file,
     iodev_no_fopen, iodev_no_fclose,
     iodev_no_delete_file, iodev_no_rename_file,
     romfs_file_status,
     romfs_enumerate_files_init, romfs_enumerate_next, romfs_enumerate_close, 
     iodev_no_get_params, iodev_no_put_params
    }
};

/* internal state for our device */
typedef struct romfs_state_s {
    int atblock;			/* for later when we decompress by blocks */
} romfs_state;

gs_private_st_simple(st_romfs_state, struct romfs_state_s, "romfs_state");

typedef struct romfs_file_enum_s {
    char *pattern;		/* pattern pointer    */
    int  list_index;		/* next node to visit */
    gs_memory_t *memory;	/* memory structure used */
} romfs_file_enum;

gs_private_st_ptrs1(st_romfs_file_enum, struct romfs_file_enum_s, "romfs_file_enum",
    romfs_file_enum_enum_ptrs, romfs_file_enum_reloc_ptrs, pattern);

static uint32_t get_u32_big_endian(const uint32_t *a);

static uint32_t
get_u32_big_endian(const uint32_t *a)
{
    uint32_t v;
    const unsigned char *c=(const unsigned char *)a;

    v = (c[0]<<24) | (c[1]<<16) | (c[2]<<8) | c[3];
    return v;
}

/* ------ Block streams, potentially compressed (read only) ------ */

/* String stream procedures */
static int
    s_block_read_available(stream *, long *),
    s_block_read_seek(stream *, long),
    s_block_read_close(stream *),
    s_block_read_process(stream_state *, stream_cursor_read *,
			  stream_cursor_write *, bool);

/* Initialize a stream for reading from a collection of blocks */
static void
sread_block(register stream *s,  const byte *ptr, uint len, const uint32_t *node )
{
    static const stream_procs p = {
	 s_block_read_available, s_block_read_seek, s_std_read_reset,
	 s_std_read_flush, s_block_read_close, s_block_read_process,
	 NULL		/* no read_switch */
    };
    s_std_init(s, (byte *)ptr, len, &p, s_mode_read + s_mode_seek);
    s->end_status = 0;
    s->file = (FILE *)node;	/* convenient place to put it for %rom% files */ 
    s->file_modes = s->modes;
    s->file_offset = 0;
    s->file_limit = max_long;
}

/* Return the number of available bytes */
static int
s_block_read_available(stream *s, long *pl)
{
    uint32_t *node = (uint32_t *)s->file;
    uint32_t filelen = get_u32_big_endian(node) & 0x7fffffff;	/* ignore compression bit */

    *pl = filelen - s->position - (sbufptr(s) - s->cbuf);
    if (*pl == 0 && s->close_at_eod)	/* EOF */
	*pl = -1;
    return 0;
}

/* Seek in a string being read.  Return 0 if OK, ERRC if not. */
static int
s_block_read_seek(register stream * s, long pos)
{
    uint32_t *node = (uint32_t *)s->file;
    uint32_t filelen = get_u32_big_endian(node) & 0x7fffffff;	/* ignore compression bit */
    uint end = s->srlimit - s->cbuf + 1;
    long offset = pos - s->position;

    if (pos < 0 || pos > filelen)
	return ERRC;
    if (offset < 0 || offset > end) {
	/* Need to pull a different block into the buffer */
	stream_cursor_write pw;

	/* buffer stays aligned to blocks */
	offset = (s->file_offset + pos) % ROMFS_BLOCKSIZE;
	s->position = pos - offset;
	pw.ptr = s->cbuf - 1;
	pw.limit = pw.ptr + s->cbsize;
	if ((s->end_status = s_block_read_process((stream_state *)s, NULL, &pw, 0)) == ERRC)
	    return ERRC;
	if (s->end_status == 1)
	    s->end_status = 0;
	s->srptr = s->cbuf - 1;
	s->srlimit = pw.ptr;		/* limit of the block just read */
    } 
    /* Now set the read pointer to the correct place in the buffer */
    s->srptr = s->cbuf + offset - 1;
    return 0;
}

static int
s_block_read_close(stream * s)
{
    gs_free_object(s->memory, s->cbuf, "file_close(buffer)");
    s->file = 0;			/* disconnect the node */
    /* Increment the IDs to prevent further access. */
    s->read_id = s->write_id = (s->read_id | s->write_id) + 1;
    return 0;
}

static int
s_block_read_process(stream_state * st, stream_cursor_read * ignore_pr,
		      stream_cursor_write * pw, bool last)
{
    int  code;
    stream *s = (stream *)st;	/* no separate state */
    uint32_t *node = (uint32_t *)s->file;
    uint max_count = pw->limit - pw->ptr;
    int status = 1;
    int compression = ((get_u32_big_endian(node) & 0x80000000) != 0) ? 1 : 0;
    uint32_t filelen = get_u32_big_endian(node) & 0x7fffffff;	/* ignore compression bit */
    uint32_t blocks = (filelen+ROMFS_BLOCKSIZE-1)/ ROMFS_BLOCKSIZE;
    int iblock = (s->position + s->file_offset + pw->ptr + 1 - s->cbuf) / ROMFS_BLOCKSIZE;
    unsigned long block_length = get_u32_big_endian(node+1+(2*iblock));
    unsigned const long block_offset = get_u32_big_endian(node+2+(2*iblock));
    unsigned const char *block_data = ((unsigned char *)node) + block_offset;
    int count = iblock < (blocks - 1) ? ROMFS_BLOCKSIZE : filelen - (ROMFS_BLOCKSIZE * iblock);

    if (count > max_count) {
	return ERRC;			/* should not happen */
    }
    if (block_data == NULL) {
	return EOFC;
    }
    if (s->file_limit < max_long) {
	long limit_count = s->file_offset + s->file_limit - s->position;

	if (count > limit_count)
	    count = limit_count;
    }

    if (count < ROMFS_BLOCKSIZE || iblock == (blocks - 1))
	status = EOFC;			/* at EOF when not filling entire buffer */
    /* get the block into the buffer */
    if (compression) {
	unsigned long buflen = ROMFS_BLOCKSIZE;

	/* Decompress the data into this block */
	code = uncompress (pw->ptr+1, &buflen, block_data, block_length);
	if (count != buflen) {
	    return ERRC;
	}
    } else {
	/* not compressed -- just copy it */
	memcpy(pw->ptr+1, block_data, block_length);
	count = block_length;
    }
    if (count < 0)
	count = 0;
    pw->ptr += count;
    process_interrupts(s->memory);
    return status;
}

static int
romfs_init(gx_io_device *iodev, gs_memory_t *mem)
{
    romfs_state *state = gs_alloc_struct(mem, romfs_state, &st_romfs_state, 
                                         "romfs_init(state)");
    if (!state)
	return gs_error_VMerror;
    iodev->state = state;
    return 0;
}

static int
romfs_open_file(gx_io_device *iodev, const char *fname, uint namelen,
    const char *access, stream **ps, gs_memory_t *mem)
{
    extern const uint32_t *gs_romfs[];
    int code;
    const uint32_t *node_scan = gs_romfs[0], *node = NULL;
    uint32_t filelen, blocks;
    int i;
    char *filename;
    char fmode[4] = "\000\000\000\000";

    /* return an empty stream on error */
    *ps = NULL;

    /* scan the inodes to find the requested file */
    for (i=0; node_scan != 0; i++, node_scan = gs_romfs[i]) {
	filelen = get_u32_big_endian(node_scan) & 0x7fffffff;	/* ignore compression bit */
	blocks = (filelen+ROMFS_BLOCKSIZE-1)/ ROMFS_BLOCKSIZE;
	filename = (char *)(&(node_scan[1+(2*blocks)]));
	if ((namelen == strlen(filename)) && 
	    (strncmp(filename, fname, namelen) == 0)) {
	    node = node_scan;
	    break;
	}
    }
    /* inode points to the file (or NULL if not found */
    if (node == NULL)
	return_error(gs_error_undefinedfilename);

    /* Initialize a stream for reading this romfs file using a common function */
    /* we get a buffer that is larger than what we need for decompression */
    /* we need extra space since some filters may leave data in the buffer when */
    /* calling 'read_process' */
    code = file_prepare_stream(fname, namelen, access, ROMFS_BLOCKSIZE+256, ps, fmode, mem);
    if (code < 0) 
	return code;
    sread_block(*ps, (*ps)->cbuf, (*ps)->cbsize, node);
    /* return success */
    return 0;
}

static int
romfs_file_status(gx_io_device * iodev, const char *fname, struct stat *pstat)
{
    extern const uint32_t *gs_romfs[];
    extern const time_t gs_romfs_buildtime;
    const uint32_t *node_scan = gs_romfs[0], *node = NULL;
    uint32_t filelen, blocks;
    int i;
    char *filename;
    uint namelen = strlen(fname);

    memset(pstat, 0, sizeof(struct stat));
    /* scan the inodes to find the requested file */
    for (i=0; node_scan != 0; i++, node_scan = gs_romfs[i]) {
	filelen = get_u32_big_endian(node_scan) & 0x7fffffff;	/* ignore compression bit */
	blocks = (filelen+ROMFS_BLOCKSIZE-1)/ ROMFS_BLOCKSIZE;
	filename = (char *)(&(node_scan[1+(2*blocks)]));
	if ((namelen == strlen(filename)) && 
	    (strncmp(filename, fname, namelen) == 0)) {
	    node = node_scan;
	    break;
	}
    }
    /* inode points to the file (or NULL if not found */
    if (node == NULL)
	return_error(gs_error_undefinedfilename);

    /* fill in the values used by zstatus */
    pstat->st_size = filelen;
    pstat->st_mtime = gs_romfs_buildtime;
    pstat->st_ctime = gs_romfs_buildtime;
    return 0;	/* success */
}

static file_enum *
romfs_enumerate_files_init(gx_io_device *iodev, const char *pat, uint patlen,
	     gs_memory_t *mem)
{
    romfs_file_enum *penum = gs_alloc_struct(mem, romfs_file_enum, &st_romfs_file_enum,
			   				"romfs_enumerate_files_init(file_enum)");
    if (penum == NULL)
        return NULL;
    memset(penum, 0, sizeof(romfs_file_enum));
    penum->pattern = (char *)gs_alloc_bytes(mem, patlen+1, "romfs_enumerate_file_init(pattern)");
    penum->list_index = 0;		/* start at first node */
    penum->memory = mem;
    if (penum->pattern == NULL) {
	romfs_enumerate_close((file_enum *) penum);
	return NULL;
    }
    memcpy(penum->pattern, pat, patlen);	/* Copy string to buffer */
    penum->pattern[patlen]=0;			/* Terminate string */

    return (file_enum *)penum;
}

static void
romfs_enumerate_close(file_enum *pfen)
{
    romfs_file_enum *penum = (romfs_file_enum *)pfen;
    gs_memory_t *mem = penum->memory;

    if (penum->pattern)
	gs_free_object(mem, penum->pattern, "romfs_enum_init(pattern)");
    gs_free_object(mem, penum, "romfs_enum_init(romfs_enum)");
}

static uint
romfs_enumerate_next(file_enum *pfen, char *ptr, uint maxlen)
{
    extern const uint32_t *gs_romfs[];
    romfs_file_enum *penum = (romfs_file_enum *)pfen;
    
    while (gs_romfs[penum->list_index] != 0) {
	const uint32_t *node = gs_romfs[penum->list_index];
	uint32_t filelen = get_u32_big_endian(node) & 0x7fffffff;	/* ignore compression bit */
	long blocks = (filelen+ROMFS_BLOCKSIZE-1)/ ROMFS_BLOCKSIZE;
	char *filename = (char *)(&(node[1+(2*blocks)]));

	penum->list_index++;		/* bump to next unconditionally */
	if (string_match((byte *)filename, strlen(filename),
			 (byte *)penum->pattern,
			 strlen(penum->pattern), 0)) {
	    if (strlen(filename) < maxlen) 
		memcpy(ptr, filename, strlen(filename));
	    return strlen(filename);	/* if > maxlen, caller will detect rangecheck */
	}
    }
    /* ran off end of list, close the enum */
    romfs_enumerate_close(pfen);
    return ~(uint)0;
}
