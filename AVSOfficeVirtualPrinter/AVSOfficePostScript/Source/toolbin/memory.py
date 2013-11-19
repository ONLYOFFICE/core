#!/usr/bin/env python

# Copyright (C) 2007 Aladdin Enterprises.  All rights reserved.

# This script analyzes the output of Ghostscript run with -Z67.
# Its primary purpose is detecting memory leaks.

# $Id: memory.py 8686 2008-05-02 01:42:14Z giles $

USAGE = """\
Usage: python memory.py z67trace > report
  where z67trace is the output of gs -Z67"""
HELP = """\
An example of usage:
	gs -Z67 somefile.ps >& somefile.log
        python memory.py somefile.log > somefile.report
"""

__author__ = 'L Peter Deutsch'

import re
from cStringIO import StringIO
from difflib import SequenceMatcher

#---------------- Memory representation ----------------#

class struct(object):
    # Instance variables:
    #   address (int) - the address of the object
    pass

class Store(object):

    def __init__(self):
        self.memories = []

    def totals(self):
        o = s = a = n = 0
        for memory in self.memories:
            for chunk in memory.chunks:
                o += chunk.otop - chunk.obot
                s += chunk.stop - chunk.sbot
                for obj in chunk.objects:
                    if not obj.isfree:
                        n += 1
                        a += obj.size
        return '%d object space (%d objects, %d total size), %d strings' % \
               (o, n, a, s)

    def compare(self, store):
        ml, sml = [[(m.address, m.space, m.level) \
                    for m in s.memories]
                   for s in [self, store]]
        if ml != sml:
            return 'Memory lists differ'
        buf = StringIO()
        for m, sm in zip(self.memories, store.memories):
            buf.write('Memory 0x%x, space = %d, level = %d:\n' % \
                      (m.address, m.space, m.level))
            buf.write(m.compare(sm))
        return buf.getvalue()

class Memory(struct):

    def __init__(self, store, address, space, level):
        self.address, self.space, self.level = address, space, level
        self.chunks = []
        if store: store.memories.append(self)

    def compare(self, memory):
        buf = StringIO()
        cdict = dict([(c.address, c) for c in self.chunks])
        mcdict = dict([(c.address, c) for c in memory.chunks])
        for a in cdict.keys():
            if a not in mcdict:
                buf.write('Freed: ')
                buf.write(cdict[a].listing())
        for a in mcdict.keys():
            if a not in cdict:
                buf.write('Added: ')
                buf.write(mcdict[a].listing())
        for a, c in cdict.items():
            if a in mcdict:
                buf.write(c.compare(mcdict[a]))
        return buf.getvalue()

class Chunk(struct):

    # obot, otop, sbot, stop correspond to chunk_t.cbase, cbot, ctop, climit.
    def __init__(self, memory, address, obot, otop, sbot, stop, cend):
        self.address = address
        self.obot, self.otop = obot, otop
        self.sbot, self.stop = sbot, stop
        self.cend = cend
        self.objects = []
        if memory: memory.chunks.append(self)

    def compare(self, chunk):
        buf = StringIO()
        o, s = self.otop - self.obot, self.stop - self.sbot
        co, cs = chunk.otop - chunk.obot, chunk.stop - chunk.sbot
        if co != o or cs != s:
            buf.write('objects %+d, strings %+d' % (co - o, cs - s))
        buf.write('\n')
        # Use difflib to find the differences between the two chunks.
        seq1 = [b.content for b in self.objects if not b.isfree]
        seq2 = [b.content for b in chunk.objects if not b.isfree]
        m = SequenceMatcher(None, seq1, seq2)
        pi = pj = 0
        for i, j, n in m.get_matching_blocks():
            while pi < i:
                buf.write('-   %s\n' % self.objects[pi])
                pi += 1
            while pj < j:
                buf.write('+   %s\n' % chunk.objects[pj])
                pj += 1
            pi, pj = pi + n, pj + n
        if buf.tell() > 1:
            return 'Chunk 0x%x: ' % self.address + buf.getvalue()
        else:
            return ''

    def listing(self):
        buf = StringIO()
        buf.write('chunk at 0x%x: %d used, %d free \n' % \
                  (self.address,
                   sum([o.size for o in self.objects if not o.isfree]),
                   sum([o.size for o in self.objects if o.isfree])))
        for obj in self.objects:
            buf.write('    %s\n' % obj)
        return buf.getvalue()

class block(struct):

    content = property(lambda b: (b.name, b.size))

    def __init__(self, chunk, address, size):
        self.address, self.size = address, size
        if chunk: chunk.objects.append(self)

    def __str__(self):
        return '0x%x: %s (%d)' % (self.address, self.name, self.size)

class Object(block):
    isfree = False
    def __init__(self, chunk, name, address, size):
        self.name = name
        block.__init__(self, chunk, address, size)

class Free(block):
    isfree = True
    name = '(free)'

#---------------- Log reader ----------------#

# Parse the log entries produced by -Z67.
res_hex = '0x([0-9a-f]+)'
res_dec = '([-0-9]+)'
re_memory = re.compile(r'validating memory %s, space %s, [^0-9]*%s' % \
                       (res_hex, res_dec, res_dec))
re_chunk = re.compile(r'validating chunk %s \(%s\.\.%s, %s\.\.%s\.\.%s\)$' % \
                      (6 * (res_hex,)))
re_object = re.compile(r'validating ([^(]+)\(%s\) %s$' % \
                       (res_dec, res_hex))
re_free = re.compile(r'validating \(free\)\(%s\) %s$' % \
                     (res_dec, res_hex))

class Log:

    def __init__(self):
        self.stores = []

    def readlog(self, fname):
        # Read a log produced by -Z67.  Each separate validation trace is a
        # separate instance of Store.  Note that each GC produces two
        # Stores, one from pre-validation, one from post-validation.
        f, store = file(fname), None
        memory = chunk = None
        for line in f:
            line = line.strip()
            if line.startswith('[6]validating memory '):
                addr, space, level = re_memory.match(line[3:]).groups()
                if not store: store = Store()
                memory = Memory(store, int(addr, 16), int(space), int(level))
                chunk = None
            elif line.startswith('[6]validating chunk '):
                cvalues = re_chunk.match(line[3:]).groups()
                chunk = Chunk(memory, *[int(v, 16) for v in cvalues])
            elif line.startswith('[7]validating (free)'):
                size, addr = re_free.match(line[3:]).groups()
                Free(chunk, int(addr, 16), int(size))
            elif line.startswith('[7]validating '):
                name, size, addr = re_object.match(line[3:]).groups()
                Object(chunk, name, int(addr, 16), int(size))
            elif line[2:].startswith(']validating'):
                print '**** unknown:', line
            elif _is_end_trace(line):
                self.stores.append(store)
                store = None
        f.close()

    def compare(self, which = slice(3, -2, 2)):
        buf = StringIO()
        stores = self.stores
        indices = range(*which.indices(len(stores)))
        for i1, i2 in zip(indices[:-1], indices[1:]):
            buf.write('Comparing %d and %d\n' % (i1, i2))
            for j in [i1, i2]:
                buf.write('%3d: %s\n' % (j, stores[j].totals()))
            buf.write(stores[i1].compare(stores[i2]))
            buf.write(64 * '-' + '\n')
        for j in [indices[0], indices[-1]]:
            buf.write('%3d: %s\n' % (j, stores[j].totals()))
        return buf.getvalue()

def _is_end_trace(line):
    return line.startswith('[6]---------------- end ') and \
           line.endswith('validate pointers ----------------')

#---------------- Main program ----------------#

def main(argv):
    args = argv[1:]
    if len(args) != 1:
        print 'Use --help for usage information.'
        return
    if args[0] == '--help':
        print USAGE
        print HELP
        return
    log = Log()
    log.readlog(args[0])
    print len(log.stores), 'stores'
    print log.compare()

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv) or 0)
