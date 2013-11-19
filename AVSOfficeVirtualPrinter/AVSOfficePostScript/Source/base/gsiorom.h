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

/* $Id: gsiorom.h 8022 2007-06-05 22:23:38Z giles $ */
/* Rom File System settings */

#ifndef gsiorom_INCLUDED
#  define gsiorom_INCLUDED

#define ROMFS_BLOCKSIZE 16384
#define ROMFS_CBUFSIZE ((int)((ROMFS_BLOCKSIZE) * 1.001) + 12)

/* enble the ROMFS_COMPRESSION as optional
  #define ROMFS_COMPRESSION
*/

/*
 * in memory structure is:
 *
 *	offset_to_next_inode (total length of this inode)	[32-bit big-endian]
 *	length_of_uncompressed_file				[32-bit big-endian]
 *	data_block_struct[]		count is (length+ROMFS_BLOCKSIZE-1)/ROMFS_BLOCKSIZE
 *	padded_file_name (char *)	includes as least one terminating <nul>
 *	padded_data_blocks
 */
/*
 *	data_block_struct:
 *	    data_length			(not including pad)	[32-bit big-endian]
 *	    data_block_offset		(start of each block)	[32-bit big-endian]
 */

#endif /* gsiorom_INCLUDED */
