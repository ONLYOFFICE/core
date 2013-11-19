#!/usr/bin/env python

#    Copyright (C) 2002 Artifex Software, Inc.
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

# $Id: gscheck_all.py 8409 2007-11-27 20:43:09Z giles $

# Run all the Ghostscript 'gscheck' tests.

from gstestutils import gsRunTestsMain

def addTests(suite, gsroot, now, options=None, **args):
    import gscheck_raster; gscheck_raster.addTests(suite,gsroot,now,options=options,**args)
    import gscheck_pdfwrite; gscheck_pdfwrite.addTests(suite,gsroot,now,options=options, **args)

if __name__ == "__main__":
    gsRunTestsMain(addTests)
