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

/*$Id: gscdefs.h 8479 2008-01-07 20:58:40Z giles $ */
/* Prototypes for configuration definitions in gconfig.c. */

#ifndef gscdefs_INCLUDED
#  define gscdefs_INCLUDED

/*
 * This file may be #included in places that don't even have stdpre.h,
 * so it mustn't use any Ghostscript definitions in any code that is
 * actually processed here (as opposed to being part of a macro
 * definition).
 */

/* Miscellaneous system constants (read-only systemparams). */

extern const long gs_buildtime;
extern const char *const gs_copyright;
extern const char *const gs_product;
extern const char *const gs_productfamily;
extern const long gs_revision;
extern const long gs_revisiondate;
extern const long gs_serialnumber;

/* Installation directories and files */
extern const char *const gs_doc_directory;
extern const char *const gs_lib_default_path;
extern const char *const gs_init_file;
extern const char *const gs_dev_defaults;

/* Resource tables.  In order to avoid importing a large number of types, */
/* we only provide macros for some externs, not the externs themselves. */

#define extern_gx_device_halftone_list()\
  typedef DEVICE_HALFTONE_RESOURCE_PROC((*gx_dht_proc));\
  extern const gx_dht_proc gx_device_halftone_list[]

#define extern_gx_image_class_table()\
  extern const gx_image_class_t gx_image_class_table[]
extern const unsigned gx_image_class_table_count;

#define extern_gx_image_type_table()\
  extern const gx_image_type_t * const gx_image_type_table[]
extern const unsigned gx_image_type_table_count;

/* We need the extra typedef so that the const will apply to the table. */
#define extern_gx_init_table()\
  typedef init_proc((*gx_init_proc));\
  extern const gx_init_proc gx_init_table[]

#define extern_gx_io_device_table()\
  extern const gx_io_device * const gx_io_device_table[]
extern const unsigned gx_io_device_table_count;

/* Return the list of device prototypes, a NULL list of their structure */
/* descriptors (no longer used), and (as the value) the length of the lists. */
#define extern_gs_lib_device_list()\
  int gs_lib_device_list(const gx_device * const **plist,\
			 gs_memory_struct_type_t **pst)

/* find a compositor by name */
#define extern_gs_find_compositor() \
  const gs_composite_type_t * gs_find_compositor(int comp_id)

#endif /* gscdefs_INCLUDED */
