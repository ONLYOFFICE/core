#!/usr/bin/env python

# Copyright (C) 2002-2007 Artifex Software, Inc.
#
# This software is provided AS-IS with no warranty, either express or
# implied.
# 
# This software is distributed under license and may not be copied,
# modified or distributed except as expressly authorized under the terms
# of the license contained in the file LICENSE in this distribution.

# script to analyse differences in Adobe Font Metrics files
# $Id: afmutil.py 8070 2007-06-20 19:46:52Z giles $

import string

class fontmetric:
    """A container for a parsed font metric file"""
    def __init__(self, filename):
        self.filename = filename
        self.mode = ['']	# this is a stack that keeps track of the parsing context
        self.info = {}		# global font info
        self.metrics = {}	# glyph metrics, index by postscript character name
        self.kerning = {}	# kerning data
    def parse(self):
        print
        print "loading metric data from '"+self.filename+"'"
        self.file = open(self.filename, 'r')
        line = self.file.readline()
        while line != '':
            self.parseline(string.strip(line))
            line = self.file.readline()
        self.file.close()
    def parseline(self, line):
        # skip blank lines
        if len(line) == 0: return
        #print "  parsing '"+line+"'"
        bits = string.split(line)
        key = bits[0]
        # handle context changes
        if key[:5] == 'Start':
            self.mode.append(key[5:])
            print " parsing " + self.mode[-1]
            return
        if key[:3] == 'End':
            self.mode.pop()
            return
        # parse inside the current context
        if self.mode[-1] == 'FontMetrics':
            # some keys we treat specially
            if key == 'FontBBox':
                value = [ bits[1], bits[2], bits[3], bits[4] ]
            elif key == 'VVector' or key == 'CharWidth':
                value = [ bits[1], bits[2] ]
            # all are a string or a single value we treat as such
            else:
                value = string.join(bits[1:])
            #print "  "+key+": "+str(value)
            self.info[key] = value
        elif self.mode[-1] == 'CharMetrics':
            if key != 'C' and key != 'CX':
                print "error: non metric data!"
                return
            # each line is a dictionary
            dict = {}
            for element in string.split(line, ';'):
                bits = string.split(element)
                if len(bits) > 0:
                    if bits[0] == 'B':
                        value = [ int(bits[1]), int(bits[2]), int(bits[3]), int(bits[4]) ]
                    else:
                        value = string.join(bits[1:])
                    dict[bits[0]] = value
            self.metrics[dict['N']] = dict
            del dict
    def print_info(self):
        keys = self.info.keys()
        keys.sort()
        for key in keys:
            print "  " + string.ljust(key, 18) + " " + str(self.info[key])
    def print_metrics(self):
        keys = self.metrics.keys()
        keys.sort()
        for glyph in keys:
            width = self.metrics[glyph]['WX']
            bbox = self.metrics[glyph]['B']
            print "     " + string.rjust(glyph, 12),
            print " " + string.rjust(width, 5),
            print "  %4d %4d %4d %4d" % (bbox[0],bbox[1],bbox[2],bbox[3])

def dump(file):
    afm = fontmetric(file)
    afm.parse()
    print "Font Information for " + afm.info['FullName'] + " (" + afm.filename + ")"
    afm.print_info()
    print "Font Metrics       [width]        [bbox]"
    afm.print_metrics()
    
def diff(a,b):
    one = fontmetric(a)
    one.parse();
    two = fontmetric(b)
    two.parse()
    print "differences between '"+a+"' and '"+b+"':"
    #print "  [not yet implemented. try --dump on each file and then running 'diff']"
    # break abstraction
    glyphs = one.metrics.keys()
    glyphs.sort()
    count = 0
    for glyph in glyphs:
      awx = one.metrics[glyph]['WX']
      try:
        bwx = two.metrics[glyph]['WX']
      except KeyError:
        continue
      if awx != bwx:
        count += 1
        print " ", glyph, awx, bwx
    print count, "advance width differences"

import sys, getopt

def usage():
    print "Usage: " + sys.argv[0] + " [options] <file1.afm> [<file2.afm>]"
    print "reads adobe font metric files an preforms various tasks"
    print " options:"
    print "   --dump prints the parsed data for manual comparison"
    print "   --diff prints a summary of the differences between two afm files"
    print "   --help this summary"
    print
        
def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hdxv", ['help', 'dump', 'diff', 'version'])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage();
            sys.exit();
        elif opt in ('-d', '--dump'):
            if len(args) > 0:
                for file in args: dump(file);
            else:
                print "    --dump requires at least one file as an argument"
                sys.exit(2)
        elif opt in ('-x', '--diff'):
            if len(args) == 2:
                diff(args[0], args[1])
            else:
                print "    --diff requires two files as arguments"
                sys.exit(2)


# call ourselves if we're a script
if __name__ == "__main__":
    main()
