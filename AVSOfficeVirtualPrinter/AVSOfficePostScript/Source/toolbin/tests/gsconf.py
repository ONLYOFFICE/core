#    Copyright (C) 2001 Artifex Software Inc.
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

# $Id: gsconf.py 9206 2008-11-06 23:51:27Z marcos $

# gsconf.py
#
# configuration file parser for regression tests

import os
import re
import sys
import time

configdir = os.path.dirname(sys.argv[0])
if len(configdir) > 0:
    configdir = configdir + "/"

def parse_config(file=configdir+"testing.cfg"):
    try:
        cf = open(file, "r")
    except:
        print "ERROR: Could not open config file '%s'." % (file,)
        return

    config_re = re.compile("^([^\s]+)\s+(.*)$")

    for l in cf.readlines():
        # strip off EOL chars
        while l and (l[-1] == '\r' or l[-1] == '\n'):
            l = l[:-1]

        # ignore comments and blank lines
        if not l or l[0] == '#':
            continue

        m = config_re.match(l)
        if m:
            sys.modules["gsconf"].__dict__[m.group(1)] = m.group(2)


def get_dailydb_name():
    return dailydir + time.strftime("%Y%m%d", time.localtime()) # mhw + ".db"

parse_config()
