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

/* $Id: iestack.h 9043 2008-08-28 22:48:19Z giles $ */
/* Generic execution stack API */

#ifndef iestack_INCLUDED
#  define iestack_INCLUDED

#include "iesdata.h"
#include "istack.h"

/* Define pointers into the execution stack. */
typedef s_ptr es_ptr;
typedef const_s_ptr const_es_ptr;

/* Manage the current_file cache. */
#define estack_clear_cache(pes) ((pes)->current_file = 0)
#define estack_set_cache(pes,pref) ((pes)->current_file = (pref))
#define estack_check_cache(pes)\
  BEGIN\
    if (r_has_type_attrs((pes)->stack.p, t_file, a_executable))\
      estack_set_cache(pes, (pes)->stack.p);\
  END

#endif /* iestack_INCLUDED */
