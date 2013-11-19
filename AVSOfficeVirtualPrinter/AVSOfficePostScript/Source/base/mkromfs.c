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

/* $Id: mkromfs.c 9099 2008-09-18 18:23:22Z giles $ */
/* Generate source data for the %rom% IODevice */
/*
 * For reasons of convenience and footprint reduction, the various postscript
 * source files, resources and fonts required by Ghostscript can be compiled
 * into the executable.
 *
 * This file takes a set of directories, and creates a compressed filesystem
 * image that can be compiled into the executable as static data and accessed
 * through the %rom% iodevice prefix
 */
/*
 *	Usage: mkromfs [-o outputfile] [options ...] paths
 *	    options and paths can be interspersed and are processed in order
 *	    options:
 *		-o outputfile	default: obj/gsromfs.c if this option present, must be first.
 *		-P prefix	use prefix to find path. prefix not included in %rom%
 *		-X path		exclude the path from further processing.
 *              -d string       directory in %rom file system (really just a prefix string on filename)
 *		-c		compression on
 *		-b		compression off (binary).
 *
 *	    Note: The tail of any path encountered will be tested so .svn on the -X
 *		  list will exclude that path in all subsequent paths enumerated.
 */ 

#include "stdpre.h"
#include "stdint_.h"
#include "time_.h"
#include "gsiorom.h"
#include "gsmemret.h" /* for gs_memory_type_ptr_t */
#include "gsmalloc.h"
#include "gsstype.h"
#include "gp.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zlib.h>

/*
 * The rom file system is an array of pointers to nodes, terminated by a NULL
 */
/*
 * in memory structure of each node is:
 *
 *	length_of_uncompressed_file		[31-bit big-endian]
 *						high bit is compression flag 
 *	data_block_struct[]
 *	padded_file_name (char *)	includes as least one terminating <nul>
 *	padded_data_blocks
 */
/*
 *	data_block_struct:
 *	    data_length			(not including pad)	[32-bit big-endian]
 *	    data_block_offset		(start of each block)	[32-bit big-endian]
 */

typedef struct romfs_inode_s {
    unsigned long length;		/* blocks is (length+ROMFS_BLOCKSIZE-1)/ROMFS_BLOCKSIZE */
    char *name;				/* nul terminated */
    unsigned long *data_lengths;	/* this could be short if ROMFS_BLOCKSIZE */
					/* is < 64k, but the cost is small to use int */
    unsigned char **data;
} romfs_inode;

typedef struct Xlist_element_s {
	void *next;
	char *path;
    } Xlist_element;

byte *minimal_alloc_bytes(gs_memory_t * mem, uint size, client_name_t cname);
byte *minimal_alloc_byte_array(gs_memory_t * mem, uint num_elements,
			     uint elt_size, client_name_t cname);
void *minimal_alloc_struct(gs_memory_t * mem, gs_memory_type_ptr_t pstype,
	       client_name_t cname);
void minimal_free_object(gs_memory_t * mem, void * data, client_name_t cname);
void minimal_free_string(gs_memory_t * mem, byte * data, uint nbytes, client_name_t cname);

/*******************************************************************************
 * The following is a REALLY minimal gs_memory_t for use by the gp_ functions
 *******************************************************************************/
byte *
minimal_alloc_bytes(gs_memory_t * mem, uint size, client_name_t cname)
{
    return malloc(size);
}

byte *
minimal_alloc_byte_array(gs_memory_t * mem, uint num_elements,
			     uint elt_size, client_name_t cname)
{
    return malloc(num_elements * elt_size);
}

void *
minimal_alloc_struct(gs_memory_t * mem, gs_memory_type_ptr_t pstype,
	       client_name_t cname)
{
    return malloc(pstype->ssize);
}

void
minimal_free_object(gs_memory_t * mem, void * data, client_name_t cname)
{
    free(data);
    return;
}

void
minimal_free_string(gs_memory_t * mem, byte * data, uint nbytes, client_name_t cname)
{
    free(data);
    return;
}

void basic_enum_ptrs(void);
void basic_reloc_ptrs(void);

void basic_enum_ptrs() {
    printf("basic_enum_ptrs is only called by a GC. Abort.\n");
    exit(1);
}

void basic_reloc_ptrs() {
    printf("basic_reloc_ptrs is only called by a GC. Abort.\n");
    exit(1);
}


const gs_malloc_memory_t minimal_memory = {
    (gs_memory_t *)&minimal_memory, /* stable */
    { minimal_alloc_bytes, /* alloc_bytes_immovable */
      NULL, /* resize_object */
      minimal_free_object, /* free_object */
      NULL, /* stable */
      NULL, /* status */
      NULL, /* free_all */
      NULL, /* consolidate_free */
      minimal_alloc_bytes, /* alloc_bytes */ 
      minimal_alloc_struct, /* alloc_struct */
      minimal_alloc_struct, /* alloc_struct_immovable */
      minimal_alloc_byte_array, /* alloc_byte_array */
      minimal_alloc_byte_array, /* alloc_byte_array_immovable */
      NULL, /* alloc_struct_array */
      NULL, /* alloc_struct_array_immovable */
      NULL, /* object_size */
      NULL, /* object_type */
      minimal_alloc_bytes, /* alloc_string */
      minimal_alloc_bytes, /* alloc_string_immovable */
      NULL, /* resize_string */
      minimal_free_string, /* free_string */
      NULL, /* register_root */ 
      NULL, /* unregister_root */
      NULL /* enable_free */ 
    },    
    NULL, /* gs_lib_ctx */
    NULL, /* head */
    NULL, /* non_gc_memory */
    0, /* allocated */
    0, /* limit */ 
    0, /* used */ 
    0  /* max used */
};

void put_uint32(FILE *out, const unsigned int q);
void put_bytes_padded(FILE *out, unsigned char *p, unsigned int len);
void inode_clear(romfs_inode* node);
void inode_write(FILE *out, romfs_inode *node, int compression, int inode_count, int*totlen);
void process_path(char *path, const char *prefix, const char *add_prefix, Xlist_element *Xlist_head, int compression,
	int *inode_count, int *totlen, FILE *out);

/* put 4 byte integer, big endian */
void put_uint32(FILE *out, const unsigned int q)
{
#if ARCH_IS_BIG_ENDIAN
    fprintf (out, "0x%08x,", q);
#else
    fprintf (out, "0x%02x%02x%02x%02x,", q & 0xff, (q>>8) & 0xff, (q>>16) & 0xff, (q>>24) & 0xff);
#endif
}

/* write string as 4 character chunks, padded to 4 byte words. */
void put_bytes_padded(FILE *out, unsigned char *p, unsigned int len)
{
    int i, j=0;
    union {
	uint32_t w;
	struct {
	    unsigned char c1;
	    unsigned char c2;
	    unsigned char c3;
	    unsigned char c4;
	} c;
    } w2c;

    for (i=0; i<(len/4); i++) {
	j = i*4;
	w2c.c.c1 = p[j++];
	w2c.c.c2 = p[j++];
	w2c.c.c3 = p[j++];
	w2c.c.c4 = p[j++];
	fprintf(out, "0x%08x,", w2c.w);
	if ((i & 7) == 7)
	    fprintf(out, "\n\t");
    }
    w2c.w = 0;
    switch (len - j) {
      case 3:
        w2c.c.c3 = p[j+2];
      case 2:
        w2c.c.c2 = p[j+1];
      case 1:
        w2c.c.c1 = p[j];
	fprintf(out, "0x%08x,", w2c.w);
      default: ;
    }
    fprintf(out, "\n\t");
}

/* clear the internal memory of an inode */
void inode_clear(romfs_inode* node)
{
    int i, blocks = (node->length+ROMFS_BLOCKSIZE-1)/ROMFS_BLOCKSIZE;
    
    if (node) {
        if (node->data) {
            for (i = 0; i < blocks; i++) {
                if (node->data[i]) free(node->data[i]);
            }
            free(node->data);
        }
        if (node->data_lengths) free(node->data_lengths);
    }
}

/* write out and inode and its file data */
void
inode_write(FILE *out, romfs_inode *node, int compression, int inode_count, int *totlen)
{
    int i, offset;
    int blocks = (node->length+ROMFS_BLOCKSIZE-1)/ROMFS_BLOCKSIZE;
    int namelen = strlen(node->name) + 1;	/* include terminating <nul> */
    
    /* write the node header */
    fprintf(out,"    static uint32_t node_%d[] = {\n\t", inode_count);
    /* 4 byte file length + compression flag in high bit */
    put_uint32(out, node->length | (compression ? 0x80000000 : 0));
    fprintf(out, "\t/* compression_flag_bit + file length */\n\t");
    
    printf("writing node '%s' len=%ld", node->name, node->length);
#ifdef DEBUG
    printf(" %ld blocks %s", blocks, compression ? "compressed" : "binary");
#endif
    printf("\n");
    
    /* write out data block structures */
    offset = 4 + (8*blocks) + ((namelen+3) & ~3);
    *totlen += offset;			/* add in the header size */
    for (i = 0; i < blocks; i++) {
	put_uint32(out, node->data_lengths[i]);
	put_uint32(out, offset);
	offset += (node->data_lengths[i]+3) & ~3;
	fprintf(out, "\t/* data_block_length, offset to data_block */\n\t");
    }
    /* write file name (path) padded to 4 byte multiple */
    fprintf(out, "\t/* file name '%s' */\n\t", node->name);
    put_bytes_padded(out, (unsigned char *)node->name, namelen);
    
    /* write out data */
    for (i = 0; i < blocks; i++) {
	put_bytes_padded(out, node->data[i], node->data_lengths[i]);
	*totlen += (node->data_lengths[i]+3) & ~3;	/* padded block length */
    }
    fprintf(out, "\t0 };\t/* end-of-node */\n");
}


/* This relies on the gp_enumerate_* which should not return directories, nor */
/* should it recurse into directories (unlike Adobe's implementation)         */
void process_path(char *path, const char *prefix, const char *add_prefix, Xlist_element *Xlist_head,
		int compression, int *inode_count, int *totlen, FILE *out)
{
    int namelen, excluded, save_count=*inode_count;
    Xlist_element *Xlist_scan;
    char *prefixed_path;
    char *found_path, *rom_filename;
    file_enum *pfenum;
    int ret, block, blocks;
    romfs_inode *node;
    unsigned char *ubuf, *cbuf;
    unsigned long ulen, clen;
    FILE *in;

    prefixed_path = malloc(1024);
    found_path = malloc(1024);
    rom_filename = malloc(1024);
    ubuf = malloc(ROMFS_BLOCKSIZE);
    cbuf = malloc(ROMFS_CBUFSIZE);
    if (ubuf == NULL || cbuf == NULL || prefixed_path == NULL ||
		found_path == NULL || rom_filename == NULL) {
	printf("malloc fail in process_path\n");
	exit(1);
    }
    prefixed_path[0] = 0;	/* empty string */
    strcat(prefixed_path, prefix);
    strcat(prefixed_path, path);
    strcat(prefixed_path, "*");
    strcpy(rom_filename, add_prefix);
#if defined(__WIN32__) || defined(__OS2__)
    {
	int i;

	/* On Windows, the paths may (will) have '\' instead of '/' so we translate them */
	for (i=0; i<strlen(prefixed_path); i++)
	    if (prefixed_path[i] == '\\')
		prefixed_path[i] = '/';
	for (i=0; i<strlen(rom_filename); i++)
	    if (rom_filename[i] == '\\')
		rom_filename[i] = '/';
    }
#endif
    /* check for the file on the Xlist */
    pfenum = gp_enumerate_files_init(prefixed_path, strlen(prefixed_path),
		    	(gs_memory_t *)&minimal_memory);
    if (pfenum == NULL) {
	printf("gp_enumerate_files_init failed.\n");
	exit(1);
    }
    while ((namelen=gp_enumerate_files_next(pfenum, found_path, 1024)) >= 0) {
	excluded = 0;
	found_path[namelen] = 0;		/* terminate the string */
	/* check to see if the tail of the path we found matches one on the exclusion list */
	for (Xlist_scan = Xlist_head; Xlist_scan != NULL; Xlist_scan = Xlist_scan->next) {
	    if (strlen(found_path) >= strlen(Xlist_scan->path)) {
		if (strcmp(Xlist_scan->path, found_path+strlen(found_path)-strlen(Xlist_scan->path)) == 0) {
		    excluded = 1;
		    break;
		}
	    }
	}
	if (excluded)
	    continue;

	/* process a file */
	node = calloc(1, sizeof(romfs_inode));
	/* get info for this file */
	in = fopen(found_path, "rb");
	if (in == NULL) {
	    printf("unable to open file for processing: %s\n", found_path);
	    continue;
	}
	/* rom_filename + strlen(add_prefix) is first char after the new prefix we want to add */
	/* found_path + strlen(prefix) is the file name after the -P prefix */
	rom_filename[strlen(add_prefix)] = 0;		/* truncate afater prefix */
	strcat(rom_filename, found_path + strlen(prefix));
	node->name = rom_filename;	/* without -P prefix, with -d add_prefix */
	fseek(in, 0, SEEK_END);
	node->length = ftell(in);
	blocks = (node->length+ROMFS_BLOCKSIZE-1) / ROMFS_BLOCKSIZE + 1;
	node->data_lengths = calloc(blocks, sizeof(*node->data_lengths));
	node->data = calloc(blocks, sizeof(*node->data));
	fclose(in);
	in = fopen(found_path, "rb");
	block = 0;
	while (!feof(in)) {
	    ulen = fread(ubuf, 1, ROMFS_BLOCKSIZE, in);
	    if (!ulen) break;
	    clen = ROMFS_CBUFSIZE;
	    if (compression) {
		/* compress data here */
		ret = compress(cbuf, &clen, ubuf, ulen);
		if (ret != Z_OK) {
		    printf("error compressing data block!\n");
		    exit(1);
		}
	    } else {
		memcpy(cbuf, ubuf, ulen);
		clen = ulen;
	    }
	    node->data_lengths[block] = clen; 
	    node->data[block] = malloc(clen);
	    memcpy(node->data[block], cbuf, clen);
	    block++;
	}
	fclose(in);
	/* write out data for this file */
	inode_write(out, node, compression, *inode_count, totlen);
	/* clean up */
	inode_clear(node);
	free(node);
	(*inode_count)++;
    }
    free(cbuf);
    free(ubuf);
    free(found_path);
    free(prefixed_path);
    if (save_count == *inode_count) {
	printf("warning: no files found from path '%s%s'\n", prefix, path);
    }
}

int
main(int argc, char *argv[])
{
    int i;
    int inode_count = 0, totlen = 0;
    FILE *out;
    const char *outfilename = "obj/gsromfs.c";
    const char *prefix = "";
    const char *add_prefix = "";
    int atarg = 1;
    int compression = 1;			/* default to doing compression */
    Xlist_element *Xlist_scan, *Xlist_head = NULL;
    
    if (argc < 2) {
	printf("\n"
 		"	Usage: mkromfs [-o outputfile] [options ...] paths\n"
 		"	    options and paths can be interspersed and are processed in order\n"
 		"	    options:\n"
 		"		-o outputfile	default: obj/gsromfs.c if this option present, must be first.\n"
 		"		-P prefix	use prefix to find path. prefix not included in %%rom%%\n"
 		"		-X path		exclude the path from further processing.\n"
 		"		-d string       directory in %%rom file system (just a prefix string on filename)\n"
 		"		-c		compression on\n"
 		"		-b		compression off (binary).\n"
 		"\n"
 		"	    Note: The tail of any path encountered will be tested so .svn on the -X\n"
 		"		  list will exclude that path in all subsequent paths enumerated.\n"
	    );
	exit(1);
    }

#ifdef DEBUG
    printf("compression will use %d byte blocksize (zlib output buffer %d bytes)\n",
        ROMFS_BLOCKSIZE, ROMFS_CBUFSIZE);
#endif /* DEBUG */
    
    if (argc > 3 && argv[1][0] == '-' && argv[1][1] == 'o') {
	/* process -o option for outputfile */
	outfilename = argv[2];
	atarg += 2;
    }
#ifdef DEBUG
    printf("   writing romfs data to '%s'\n", outfilename);
#endif /* DEBUG */
    out = fopen(outfilename, "w");

    fprintf(out,"\t/* Generated data for %%rom%% device, see mkromfs.c */\n");
#if ARCH_IS_BIG_ENDIAN
    fprintf(out,"\t/* this code assumes a big endian target platform */\n");
#else
    fprintf(out,"\t/* this code assumes a little endian target platform */\n");
#endif
    fprintf(out,"\n#include \"stdint_.h\"\n");
    fprintf(out,"\n#include \"time_.h\"\n\n");
    fprintf(out,"    time_t gs_romfs_buildtime = %ld;\n\n", time(NULL));

    /* process the remaining arguments (options interspersed with paths) */
    for (; atarg < argc; atarg++) {  
	if (argv[atarg][0] == '-') {
	    /* process an option */
	    switch (argv[atarg][1]) {
	      case 'b':
	        compression = 0;
	        break;
	      case 'c':
	        compression = 1;
	        break;
	      case 'd':
		if (++atarg == argc) {
		    printf("   option %s missing required argument\n", argv[atarg-1]);
		    exit(1);
		}
		add_prefix = argv[atarg];
	        break;
	      case 'P':
		if (++atarg == argc) {
		    printf("   option %s missing required argument\n", argv[atarg-1]);
		    exit(1);
		}
		prefix = argv[atarg];
	        break;
	      case 'X':
		if (++atarg == argc) {
		    printf("   option %s missing required argument\n", argv[atarg-1]);
		    exit(1);
		}
		Xlist_scan = malloc(sizeof(Xlist_element));
		if (Xlist_scan == NULL) {
		    exit(1);
		}
		Xlist_scan->next = Xlist_head;
		Xlist_head = Xlist_scan;
		Xlist_head->path = argv[atarg];
	        break;
	      default:
	        printf("  unknown option: %s \n", argv[atarg]);
	    }
	    continue;
	}
	/* process a path */
	process_path(argv[atarg], prefix, add_prefix, Xlist_head, compression, &inode_count, &totlen, out);
    }
    /* now write out the array of nodes */
    fprintf(out, "    uint32_t *gs_romfs[] = {\n");
    for (i=0; i<inode_count; i++)
	fprintf(out, "\tnode_%d,\n", i);
    fprintf(out, "\t0 };\n");
    
    fclose(out);

    printf("Total %%rom%% structure size is %d bytes.\n", totlen);
    
    return 0;
}

