#    Copyright (C) 2004 Artifex Software Inc.
#    All Rights Reserved.
#
# This software is provided AS-IS with no warranty, either express or
# implied.
#
# This software is distributed under license and may not be copied, modified
# or distributed except as expressly authorized under the terms of that
# license.  Refer to licensing information at http://www.artifex.com/
# or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
# San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.

# $Id: gsutil.py 8409 2007-11-27 20:43:09Z giles $

# gsutil.py
#
# this module contains utility routines used by the regression test scripts

import string

def check_extension(fn):
    f = string.lower(fn)
    if f[-3:] == '.ps' or f[-4:] == '.pdf' or f[-4:] == '.eps' \
       or f[-3:] == '.ai':
        return 1
    return 0
