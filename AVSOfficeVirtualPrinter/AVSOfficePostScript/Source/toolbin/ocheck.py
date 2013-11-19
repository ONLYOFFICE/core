#!/usr/bin/env python

# Copyright (C) 2006 Aladdin Enterprises.  All rights reserved.

# Analyze the module dependency structure of binary files.
# This script requires GNU-compatible 'nm' and 'objdump' programs:
# it has only been tested on (32- and 64-bit) x86 systems.

# $Id: ocheck.py 7940 2007-05-09 21:51:57Z giles $

USAGE = """\
Usage: python ocheck.py [<cwd>] <ld-script> (--exclude [from]:[to]* |
         --query [from]:[to]* | --cycles | --verbose)*
from and to are comma-separated lists of patterns, which are glob-style
expressions representing modules (if they contain a '.') or symbols (if
not).  An empty from or to is equivalent to '*'."""
HELP = """
An example from Ghostscript (to check references from library files to
PostScript interpreter files, which are not allowed):

    python ocheck.py gs gs/obj/ldt.tr \\
      --exclude :ptr_const_string_procs,ptr_string_procs,ptr_struct_procs \\
      gconfig.o: iconfig.o: :icc.o :ijs_client.o :inflate.o gs.o:imain*.o \\
      --query [gs]*.o:[iz]*.o
"""

__author__ = 'L. Peter Deutsch'

from os.path import basename, isdir, join
from subprocess import PIPE, Popen
import re, sys

_type2section = {'D': 'data', 'd': 'data',
                 'R': 'rodata', 'r': 'rodata',
                 'T': 'text', 't': 'text'}

class OFile:

    def __init__(self, name):
        self.name = name
        self.basename = basename(name)

    def __str__(self):
        return self.name

    def _readsymbols(self):
        # Return a mapping from symbol to (type, loc|None).
        nmlist, symbols = self._exec('nm'), {}
        for line in nmlist.split('\n'):
            parts = line.strip().split(' ')
            if len(parts) == 3:
                loc, type, sym = parts
                loc = int(loc, 16)
            elif len(parts) == 2 and len(parts[0]) == 1:
                loc = None
                type, sym = parts
            else:
                assert not line
                continue
            assert sym not in symbols   # ****** WRONG FOR LOCALS? ******
            symbols[sym] = (type, loc)
        self.symbols = symbols
        return symbols

    def _readrelocs(self):
        # Return a mapping from section name to a sorted list of
        # (offset, referenced symbol).  Note that the referenced symbols
        # may be defined in this file, i.e., not imported.
        odlist, relocs = self._exec('objdump', '-r'), {}
        # We don't know whether the output format of objdump is standard,
        # but we'll find out....
        for line in odlist.split('\n'):
            if line.startswith('RELOCATION RECORDS FOR [.'):
                assert line.endswith(']:')
                section = line[25:-2]
                assert section not in relocs
                rlist = relocs[section] = []
            elif line.startswith('OFFSET '):
                pass
            elif not line:
                rlist = None
            elif rlist is None:         # preamble
                pass
            else:                       # must be a real entry
                line = line.split(' ')
                loc, sym = line[0], line[-1]
                if sym[0] != '.':           # i.e., a real reference
                    sym = sym.split('+')[0] # might have an offset
                    rlist.append((int(loc, 16), sym))
        for rlist in relocs.values():
            rlist.sort(key = lambda (loc, sym): loc)
        self.relocs = relocs
        return relocs

    def _readreferences(self):
        # Return a mapping from referencing symbol (procedure/data) to
        # referenced imported symbols.  Note that the referencing symbol
        # may be local (not exported).
        syms = self.symbols
        relocs = dict([(section, list(rels)) for section, rels in
                       self.relocs.items()]) # copy rel lists
        refs = {}
        defs = [(sym, _type2section[type], loc) \
                for sym, (type, loc) in syms.items() \
                if loc is not None and type in _type2section]
        imports = dict([(sym, 1) for sym, (type, loc) in syms.items() \
                        if type == 'U'])
        # Sorting in reverse order turns out to simplify things.
        defs.sort(key = lambda (sym, sec, loc): -loc)
        for rels in relocs.values():
            rels.reverse()
        for sym, section, loc in defs:
            rels, rlist = relocs.get(section, ()), []
            while rels and rels[0][0] >= loc:
                ref = rels.pop(0)[1]
                if ref in imports and ref not in rlist: # quadratic, don't care
                    rlist.append(ref)
            if rlist:
                refs[sym] = rlist
        self.references = refs
        return refs

    def _exec(self, *cmd):
        return Popen(cmd + (self.name,), stdout = PIPE).communicate()[0]

    # Read information from file lazily, with no overhead afterwards.
    symbols = property(_readsymbols)
    relocs = property(_readrelocs)
    references = property(_readreferences)

class DGraph:

    def __init__(self):
        self.graph = {}

    def add(self, ofrom, oto):
        g = self.graph
        if not ofrom in g: g[ofrom] = [{}, {}]
        if not oto in g: g[oto] = [{}, {}]
        g[ofrom][1][oto] = 1
        g[oto][0][ofrom] = 1

    def remove(self, ofrom = None, oto = None):
        if ofrom is None:
            for obj in self.fromkeys():
                self.remove(obj, oto)
        elif oto is None:
            for obj in self.tokeys(ofrom):
                self.remove(ofrom, obj)
        else:
            g = self.graph
            if ofrom in g and oto in g:
                try:
                    del g[ofrom][1][oto]
                    del g[oto][0][ofrom]
                except KeyError:
                    pass

    def fromkeys(self, oto = None):
        if oto:
            return self.graph.get(oto, [{}, {}])[0].keys()
        return [key for key, (f, t) in self.graph.items() if t]

    def tokeys(self, ofrom = None):
        if ofrom:
            return self.graph.get(ofrom, [{}, {}])[1].keys()
        return [key for key, (f, t) in self.graph.items() if f]

    # Thanks to http://www.bitformation.com/art/python_toposort.html
    # for this algorithm.
        
    def toposort(self):
        g = self.graph
        roots = [obj for (obj, (f, t)) in g.items() if len(f) == 0]
        sorted = []
        while roots:
            root = roots.pop()
            sorted.append(root)
            for child in g[root][1]:
                del g[child][0][root]
                if len(g[child][0]) == 0:
                    roots.append(child)
            del g[root]
        return sorted, g.keys()

    def pick(self):
        # Pick a non-root to convert to a root.
        g = self.graph
        gi = g.items()
        gi.sort(key = lambda (o, (f, t)): (len(f), -len(t)))
        o, (f, t) = gi[0]
        # Work backward through single-reference parents.
        # Invariant: (f, t) = g[o]
        no = o
        seen = [o]
        while 1:
            nf, nt = g[no]
            if len(nf) > 1: break
            o, f, t = no, nf, nt
            no = f.keys()[0]
            if no in seen: break
            seen.append(no)
        for fo in f:
            del g[fo][1][o]
        g[o][0] = {}
        seen.reverse()
        return o, f.keys(), t.keys(), seen

# ---------------------------- main program ---------------------------- #

def usage():
    print USAGE

def cvpat(str):
    if not str: return ('', '')
    sre, mre = '', ''
    for p in str.split(','):
        r = p.replace('.', r'\.').replace('?', '.').replace('*', '.*')
        if '.' in p: mre += '|' + r
        else:        sre += '|' + r
    return (sre and sre[1:], mre and mre[1:])

def forfts(args, defs, refs, proc, verbose):
    # Call proc(m, s) for each (m, s) matching an item in ftlist
    defmods, refmods = defs.fromkeys(), refs.fromkeys()
    while args and not args[0].startswith('-'):
        ft = args.pop(0).split(':')
        if len(ft) != 2: break
        if verbose:
            print '-------- %s:%s --------' % tuple(ft)
        f, t = ft
        fs, fm = cvpat(f)
        ts, tm = cvpat(t)
        if fm:
            fm = re.compile(fm)
            fmods = [m for m in refmods if fm.match(m)]
        else:
            fmods = refmods
        fmods = dict([(m, 1) for m in fmods])
        if tm or not ts:                # neither tm nor ts = all
            tm = re.compile(tm)
            tmods = dict([(m, 1) for m in defmods if tm.match(m)])
        else:
            tmods = None
        # ****** fs IS BOGUS, USES ENTIRE MODULE ******
        if fs:
            fs = re.compile(fs)
            for s in defs.tokeys():
                if fs.match(s):
                    for m in defs.fromkeys(s):
                        fmods[m] = 1
        if ts:
            ts = re.compile(ts)
        for m in fmods:
            if ts:
                for s in refs.tokeys(m):
                    if ts.match(s):
                        proc(m, s)
            if tm:
                for s in refs.tokeys(m):
                    for dm in defs.fromkeys(s):
                        if tm.match(dm):
                            proc(m, s)

def tsort(defs, refs):
    ts = DGraph()
    for m1 in refs.fromkeys():
        for s in refs.tokeys(m1):
            for m2 in defs.fromkeys(s):
                ts.add(m1, m2)
    sorted, loops = ts.toposort()
    passes = 1
    while loops:
        print 'Sorted:', len(sorted), sorted
        print len(ts.graph), 'files remaining'
        o, f, t, seen = ts.pick()
        print 'Picking', o, ': %d references' % len(f), f
        print '    %d children,' % len(t),\
              [(o, len(ts.tokeys(o))) for o in t]
        sorted, loops = ts.toposort()
        passes += 1
    print 'Topo sort completed, %d pass(es).' % passes

def main(argv):
    args = argv[1:]
    if len(args) < 1:
        print 'Use --help for usage information.'
        return
    if args[0] == '--help':
        print USAGE
        print HELP
        return
    if isdir(args[0]):
        cwd = args.pop(0)
    else:
        cwd = ''
    if len(args) < 1: return usage()
    verbose = False
    # Read the ld script and each file's symbol table.
    ldscript = args.pop(0)
    f = file(ldscript)
    ld = f.read()
    f.close()
    tokens = filter(None, ld.replace('\\\n', '').split())
    ofiles, ofdict = [], {}
    del tokens[0]                       # gcc or ld
    while tokens:
        t = tokens.pop(0)
        if t == '-o':
            tokens.pop(0)
            continue
        elif t.startswith('-l') or t.startswith('-L'):
            continue
        elif t.startswith('-'):
            print 'Unrecognized switch in ld script:', t
            return 1
        ofile = OFile(join(cwd, t))
        ofiles.append(ofile)
        ofdict[ofile.basename] = ofile
    print len(ofiles), 'object files'
    # Collect the symbols.
    defs, refs = DGraph(), DGraph()
    for ofile in ofiles:
        fname = basename(ofile.name)
        for sym, (type, loc) in ofile.symbols.items():
            if '.' in sym:              # generated symbol
                continue
            if type == 'U':
                refs.add(fname, sym)
            elif type.islower():        # local symbol
                continue
            elif sym in defs.graph:
                dfiles = defs.graph[sym][0].keys()
                print '****', sym, 'defined in',\
                      [(dfile, ofdict[dfile].symbols[sym]) for dfile in dfiles],\
                      'and', ofile, (type, loc)
            else:
                defs.add(fname, sym)
    print len(refs.graph), 'in refs,', len(defs.graph), 'in defs'
    # Process the rest of the command line.
    while args:
        arg = args.pop(0)
        if arg == '--exclude':
            # Remove excluded relationships.
            def remove1(m, s):
                if verbose:
                    print 'removing', (m, s)
                refs.remove(m, s)
            forfts(args, defs, refs, remove1, verbose)
        elif arg == '--query':
            # Process queries.
            print 64 * '-'
            def query1(m, s):
                dms = defs.fromkeys(s)
                print '>>', m, 'references', s, '(defined in',\
                      ', '.join(dms) + ')'
                ofile, srcs = ofdict[m], []
                for src, syms in ofile.references.items():
                    if s in syms:
                        srcs.append(src)
                if srcs:
                    print '     from', ', '.join(srcs)
            forfts(args, defs, refs, query1, verbose)
        elif arg == '--cycles':
            # Attempt to sort the modules topologically, using the criterion
            # that M1 precedes M2 iff M1 uses a symbol defined in M2.
            print 64 * '-'
            tsort(defs, refs)
        elif arg == '--verbose':
            verbose = True
        else:
            return usage()

if __name__ == '__main__':
    sys.exit(main(sys.argv) or 0)
