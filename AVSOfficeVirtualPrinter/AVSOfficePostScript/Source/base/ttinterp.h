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

/* $Id: ttinterp.h 8022 2007-06-05 22:23:38Z giles $ */

/* Changes after FreeType: cut out the TrueType instruction interpreter. */

/*******************************************************************
 *
 *  ttinterp.h                                              2.2
 *
 *  TrueType bytecode intepreter.
 *
 *  Copyright 1996-1998 by
 *  David Turner, Robert Wilhelm, and Werner Lemberg
 *
 *  This file is part of the FreeType project, and may only be used
 *  modified and distributed under the terms of the FreeType project
 *  license, LICENSE.TXT.  By continuing to use, modify, or distribute
 *  this file you indicate that you have read the license and
 *  understand and accept it fully.
 *
 *
 *  Changes between 2.2 and 2.1:
 *
 *  - a small bugfix in the Push opcodes
 *
 *  Changes between 2.1 and 2.0:
 *
 *  - created the TTExec component to take care of all execution
 *    context management.  The interpreter has now one single
 *    function.
 *
 *  - made some changes to support re-entrancy.  The re-entrant
 *    interpreter is smaller!
 *
 ******************************************************************/

#ifndef TTINTERP_H
#define TTINTERP_H

#include "ttcommon.h"
#include "ttobjs.h"


#ifdef __cplusplus
  extern "C" {
#endif
  
  /* Run instructions in current execution context */
  TT_Error  RunIns( PExecution_Context  exc );
 
#ifdef __cplusplus
  }
#endif

#endif /* TTINTERP_H */


/* END */
