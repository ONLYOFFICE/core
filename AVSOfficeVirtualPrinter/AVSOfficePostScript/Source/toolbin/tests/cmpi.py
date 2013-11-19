#!/usr/bin/python
#    Copyright (C) 1991-2004 Artifex Software, Inc.  All rights reserved.
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

# $Id: cmpi.py 8409 2007-11-27 20:43:09Z giles $
# python compare image -- interactive graphical image differencing
#
"""
	cmpi	(compare image or compare interactive)

	usage:	cmpi [basline_filename compare_filename]

"""
HelpMsg = '''
       key	mnemonic	function
       ---	------------	-----------------------------
	b	"baseline"	show baseline image
	c	"compare"	show compare image
	d	"differences"	show differences (black is different)
	h	"highlight"	toggle difference highlights (box outline)
	m	"mask"		toggle mask of baseline/compare with diff
	n	"next"		go to next difference
	o	"open"		open new image files
	p	"previous"	go to previous difference
	t	"this"		go to current difference
	q	"quit"
	z	"zoom"		zoom to a value
	+ =			zoom in
	-			zoom out
	?			help message
'''

from Tkinter import *
import Image, ImageTk, ImageDraw
import os
import sys		# for exit

# Set globals
baseline_filename = ""
compare_filename = ""

class ScrolledCanvas(Frame):
    def __init__(self, parent=None, color='#E4E4E4'):	# default bg = lightgray
        Frame.__init__(self, parent)
        self.pack(expand=YES, fill=BOTH)                  

	self.statustext = StringVar()
	self.statustext.set("                                                                                ")
	self.statusbar = Label(self, anchor="w", height=1, textvariable=self.statustext)
	self.statusbar.pack(side=BOTTOM, fill=X)

        self.canv = Canvas(self, bg=color, relief=SUNKEN)
        self.canv.config()

	self.extraX = self.extraY = -1

        self.sbarX = Scrollbar(self, orient=HORIZONTAL)
        self.sbarX.config(command=self.canv.xview)                   
        self.canv.config(xscrollcommand=self.sbarX.set)              
        self.sbarX.pack(side=BOTTOM, fill=X)                     

        self.sbarY = Scrollbar(self, orient=VERTICAL)
        self.sbarY.config(command=self.canv.yview)                   
        self.canv.config(yscrollcommand=self.sbarY.set)              
        self.sbarY.pack(side=RIGHT, fill=Y)                     

	self.canv.config(scrollregion=self.canv.bbox(ALL))
	self.canvas_image = None

	self.line1 = self.canv.create_line(0, 0, 0, 0, arrow=LAST, fill="red")
	self.line2 = self.canv.create_line(0, 0, 0, 0, arrow=LAST, fill="darkgreen")

        self.canv.pack(side=LEFT, expand=YES, fill=BOTH)      
	# self.canv.bind("<Button-1>", self.LeftMouse)

    # not used currently - can't decide what we need the mouse for
    def LeftMouse(self, event):
	print "Left Mouse click at: ", event.x, event.y
	self.canv.coords(self.line1, 0, event.y, event.x, event.y)
	self.canv.coords(self.line2, event.x, 0, event.x, event.y)

def mask_func(val):
    return val*0.9

# Display an image at the current zoom factor
def DoDisplay(SC):
    if SC.canvas_image != None: SC.canv.delete(SC.canvas_image)
    if SC.mask != 0:
	blank_image = Image.new("RGB", SC.image.size, "white")
        mask_image = SC.image_diff.point(mask_func)
	this_image = Image.composite(blank_image, SC.image, mask_image)
    else:
	this_image = SC.image
    SC.image_zoomed = this_image.resize((this_image.size[0]*SC.zoom_factor,
    	this_image.size[1]*SC.zoom_factor))
    SC.photo = ImageTk.PhotoImage(SC.image_zoomed)
    SC.canvas_image = SC.canv.create_image(0, 0, image=SC.photo, anchor=NW)
    SC.canv.tag_lower(SC.canvas_image, 1)	# bottomost element
    # set scrollregion
    SC.canv.config(scrollregion=SC.canv.bbox(ALL))

def b_proc(SC, key):
    # set title to 'baseline image: '
    s = "image: %s" % baseline_filename
    SC.statustext.set(SC.statustext.get()[:66] + s )
    # display baseline
    SC.image = SC.image_baseline
    DoDisplay(SC)

def c_proc(SC, key):
    # set title to 'compare image: '
    s = "image: %s" % compare_filename
    SC.statustext.set(SC.statustext.get()[:66] + s )
    # display baseline
    SC.image = SC.image_compare
    DoDisplay(SC)

def d_proc(SC, key):
    # set title to 'compare image: '
    s = "image: differences"
    SC.statustext.set(SC.statustext.get()[:66] + s )
    SC.image = SC.image_diff
    SC.mask = 0
    DoDisplay(SC)

def DelHighlights():
    if SC.highlight > 0:
	for l in SC.highlight_list:
	    SC.canv.delete(l)
	SC.highlight_list = [ ]

def GoToXY(X, Y):	# (0,0) is upper left

    # window may have been resized - 't' == "this" will re-center
    g = SC.master.geometry()
    gw = int(g.split('x')[0])
    gh = g.split('x')[1]
    gh = int(gh.split('+')[0])

    # compute display size
    dw = gw - SC.extraX
    dh = gh - SC.extraY

    shx = dw / (float(SC.Width) * SC.zoom_factor)		# scrollbar 'handle' width
    xf = (X / float(SC.Width)) - (shx/2)
    shy = dh / (float(SC.Height) * SC.zoom_factor)		# scrollbar 'handle' width
    yf = (Y / float(SC.Height)) - (shy/2)
    SC.canv.xview("moveto", xf)
    SC.canv.yview("moveto", yf)

def HighlightArea(B, Z, Color):
    SC.highlight_list.append(SC.canv.create_line(Z*B[0], Z*B[1], Z*(1+B[2]), Z*B[1], fill=Color))
    SC.highlight_list.append(SC.canv.create_line(Z*B[0], Z*(1+B[3]), Z*(1+B[2]), Z*(1+B[3]), fill=Color))
    SC.highlight_list.append(SC.canv.create_line(Z*B[0], Z*B[1], Z*B[0], Z*(1+B[3]), fill=Color))
    SC.highlight_list.append(SC.canv.create_line(Z*(1+B[2]), Z*B[1], Z*(1+B[2]), Z*(1+B[3]), fill=Color))


def DoHighlights():
    global SC

    if SC.highlight > 0:
	# draw the highlight boxes at the current zoom_factor
	z = SC.zoom_factor
	for i in range(len(SC.areas)):
	    if i == SC.current_area: color = "red"
	    else: color = "green"
	    HighlightArea(SC.area_boxes[i], z, color)

def h_proc(SC, key):
    DelHighlights()	# If they are on, turn them 'OFF'
    SC.highlight = 1 - SC.highlight
    DoHighlights()	# Draw highlights if now 'ON'

def m_proc(SC, key):
    if SC.image == SC.image_diff:
	print 'Cannot mask. First select baseline or compare image.'
    SC.mask = 1 - SC.mask
    DoDisplay(SC)

def npt_proc(SC, key):		# next, previous, this

    if key == 'n': SC.current_area += 1
    if SC.current_area >= 0:		# initially at -1, ignore 't' and 'p'
	DelHighlights()	# If they are on, turn them 'OFF'
	if key == 'p': SC.current_area -= 1
	if SC.current_area >= len(SC.areas): SC.current_area -= 1	# at last
	if SC.current_area < 0: SC.current_area = 0		# at first
	# set statusbar to say which we are at, format "at diff: n of m"
	s = "at diff: %d of %d, Box: (%d,%d) - (%d,%d)" % (SC.current_area + 1, len(SC.areas), \
		SC.area_boxes[SC.current_area][0], SC.area_boxes[SC.current_area][1], \
		SC.area_boxes[SC.current_area][2], SC.area_boxes[SC.current_area][3])
	blanks = "                                                                    "
	SC.statustext.set(s + blanks[0:66-len(s)] + SC.statustext.get()[66:])
	b = SC.area_boxes[SC.current_area]
	X = float(b[0] + b[2]) / 2.0 
	Y = float(b[1] + b[3]) / 2.0 
	GoToXY(X, Y)
    DoHighlights()	# Draw highlights if now 'ON'

def open_dlg_remove():
    global open_dlg
    global baseline_filename, compare_filename

    open_dlg["takefocus"] = 0
    open_dlg.grab_release()
    open_dlg.unbind("<Key>")
    open_dlg.destroy()
    open_dlg == None
    SC["takefocus"] = 1
    SC.master.deiconify()
    SC.bind_all("<Key>", KeyPress)
    open_files()

def open_files():
    # collect differences
    # image types: P4:1-bit binary, P5:8-bit gray, P6:8-bit RGB (24-bit)
    print "Gathering diffs, please wait ..."	##DEBUG	

    DelHighlights()
    SC.zoom_factor = 1
    SC.current_area = -1
    SC.mask = 0
    SC.negate = 0
    SC.highlight = 0
    SC.highlight_list = [ ]

    B = open(baseline_filename, "rb")
    C = open(compare_filename, "rb")
    BType = B.readline()
    CType = C.readline()
    while True:
	BDim = B.readline()
	CDim = C.readline()
	if BDim[0] != '#': break

    SC.Width = int(BDim.split()[0])
    SC.Height = int(BDim.split()[1])

    pixel_size = 1		# default to 1 byte per pixel.
    if BType[0:2] == "P6":
	pixel_size = 3
	BMax = B.readline()
	CMax = C.readline()

    Bstr = B.read()
    Cstr = C.read()

    # areas are lists of (start, end, line#) triples. Note that there may be
    # more than 1 triple with the same line# created when two areas merge on
    # a line after the first (V shape)
    SC.areas = [ ]
    merge = [ ]
    # diffs_prev_line stores triples: (start, end, area#)
    # while collecting diffs_curr_line, area# is set to -1 (unknown)
    diffs_prev_line = [ ]	
    for line in range(SC.Height):
	line_base = line*SC.Width*pixel_size
	diffs_curr_line = [ ]
	start = end = -2
	for i in range(line_base, line_base+(SC.Width*pixel_size), pixel_size):
	    j = i+pixel_size
	    if Bstr[i:j] != Cstr[i:j]:
		# next differing pixel
		if i == end+pixel_size:
		    # set new end point
		    end = i
		else:
		    if end >= 0:
			# store previous run
			diffs_curr_line.append( ((start-line_base)/pixel_size, (end-line_base)/pixel_size, -1) )
		    start = end = i
	# end-of-line, store final diff
	if end >= 0:
	    diffs_curr_line.append( ((start-line_base)/pixel_size, (end-line_base)/pixel_size, -1) )
	if len(diffs_curr_line) > 0:
	    # Now update the areas
	    if len(diffs_prev_line) == 0:
		# add all as new areas
		for i in range(len(diffs_curr_line)):
		    diff = diffs_curr_line[i]
		    SC.areas.append( [ (diff[0], diff[1], line) ] )
		    diffs_curr_line[i] = (diff[0], diff[1], len(SC.areas)-1)	# update area#
	    else:
		# process curr_line, checking for areas adjacent to those in prev_line
		index_in_prev_line = 0
		prev_diff = diffs_prev_line[0] # [index_in_prev_line]
		for i in range(len(diffs_curr_line)):
		    diff = diffs_curr_line[i]
		    while diff[0] > prev_diff[1]+1:
			# advance to next diff in prev_line (if any)
			index_in_prev_line += 1
			prev_diff = advance_diff_in_prev_line(diffs_prev_line, index_in_prev_line)

		    #  curr.E  >= prev.S - 1     & curr.S  <= prev.E + 1
		    if (prev_diff[2] >= 0) and \
			    (diff[1] >= prev_diff[0]-1) and \
			    (diff[0] <= prev_diff[1]+1):
			# append this diff to area of prev diff's area
			SC.areas[prev_diff[2]].append( (diff[0], diff[1], line) )
			diffs_curr_line[i] = (diff[0], diff[1], prev_diff[2])	# update area# from prev line
		    else: 
			SC.areas.append( [ (diff[0], diff[1], line) ] )
			diffs_curr_line[i] = (diff[0], diff[1], len(SC.areas)-1)

	    # Merge areas.
	    for curr in diffs_curr_line:
		for prev in diffs_prev_line:
		    if curr[2] != prev[2]:
			# areas are different
			if (curr[0] <= prev[1]+1) & (curr[1] >= prev[0]):
			    # merge areas curr[2] and prev[2]
			    merge.append( [curr[2], prev[2]] )

	# After updating the areas, save current line as previous
	diffs_prev_line = diffs_curr_line
	
    # finished all lines
    B.close()
    C.close()
    # Process the merge list
    for m in range(len(merge)):
	for n in range(m+1, len(merge)):
	    if merge[m][0] == merge[n][0] or merge[m][1] == merge[n][0]:
		merge[m].append(merge[n][1])
		merge[n][1] = -1
	    if merge[m][0] == merge[n][1] or merge[m][1] == merge[n][1]:
		merge[m].append(merge[n][0])
		merge[n][0] = -1
	dest = merge[m][0]
	if dest >= 0:
	    for src in merge[m][1:]:
		if src >= 0:
		    SC.areas[dest].extend(SC.areas[src])
		    SC.areas[src] = [ ]
	
    # remove any empty (placeholder) elements in areas
    for i in range(SC.areas.count( [ ] )): SC.areas.remove( [ ] )

    # Capture the images for the Canvas
    SC.image_baseline = Image.open(baseline_filename)
    SC.image_compare = Image.open(compare_filename)

    # Finally, construct the difference image (mask)
    # collect the box for each area as we process
    SC.image_diff = Image.new("L", SC.image_baseline.size, 255)
    SC.area_boxes = [ ]
    for a in SC.areas:
	box = [ 999999, 999999, -1, -1 ]
	for run in a:
	    for x in range(run[0], run[1]+1):
		SC.image_diff.putpixel((x,run[2]), 0)
	    if run[2] < box[1]: box[1] = run[2]
	    if run[2] > box[3]: box[3] = run[2]
	    if run[0] < box[0]: box[0] = run[0]
	    if run[1] > box[2]: box[2] = run[1]
	SC.area_boxes.append(box)

    SC.master.title("cmpi %s %s" % (baseline_filename, compare_filename) )

    print "Difference area count: ",len(SC.areas)	##DEBUG	

    # set the canvas to display the entire image if it fits, otherwise use the max
    if SC.Width < 0.95*int(SC.master.maxsize()[0]): w = 1.06*int(SC.Width)
    else: w = 0.95*int(SC.master.maxsize()[0])
    if SC.Height < 0.90*int(SC.master.maxsize()[1]): h = 1.11*int(SC.Height)
    else: h = 0.90*int(SC.master.maxsize()[1])
    g = "%dx%d+50+50" % (w, h)
    SC.master.geometry(g)

    # update the statusbar
    s = "%d differences." % len(SC.areas)
    blanks = "                                          "
    SC.statustext.set(s + blanks[0:66-len(s)] + SC.statustext.get()[66:])

    # Start with the baseline image
    b_proc(SC, 'b')

def FileList_KeyPress(event):
    global open_dlg, open_names, FileListPrompt
    global baseline_filename, compare_filename

    if event.char == 'q' or event.keysym == "Escape":
	open_names = [ ]
	if baseline_filename == "":
	    FileListPrompt["text"] = "\nSelect baseline image file"
	else:
	    open_dlg_remove()
    elif (event.char == "o" or event.keysym == "Return") and \
	    len(open_names) == 2:
	baseline_filename = open_names[0]
	compare_filename = open_names[1]
	open_names = [ ]
	open_dlg_remove()

def GrabFileName():
    global FileList, FileListPrompt, open_names

    cur = FileList.curselection()
    if len(cur) > 0:
	name = FileList.get(cur)
	FileList.after(200, FileList.selection_clear, (cur))
	if len(open_names) == 0:
	    open_names.append(name)
	    FileListPrompt["text"] = "\nSelect compare image file"
	elif len(open_names) == 1:
	    open_names.append(name)
	    FileListPrompt["text"] = "\nPress <Enter> if OK or <Esc> to Cancel"

def FileList_Clicked(event):
    GrabFileName()

def open_dlg_pop():
    global open_dlg
    global FileList, FileListPrompt, open_names

    open_names = [ ]
    open_dlg = Toplevel()
    FileListPrompt = Label(open_dlg, height=3, width=80, anchor=CENTER, text="\nSelect baseline image file")
    FileListPrompt.grid(row=0, sticky=N)
    yScroll = Scrollbar ( open_dlg, orient=VERTICAL )
    yScroll.grid ( row=1, column=1, sticky=N+S )
    xScroll = Scrollbar ( open_dlg, orient=HORIZONTAL )
    xScroll.grid ( row=2, column=0, sticky=E+W )
    FileList = Listbox ( open_dlg, height=20, width=80, xscrollcommand=xScroll.set, yscrollcommand=yScroll.set )
    FileList.grid ( row=1, column=0, sticky=N+S+E+W )
    xScroll["command"] = FileList.xview
    yScroll["command"] = FileList.yview
    SC.master.iconify()
    SC.unbind_all("<Key>")
    open_dlg.bind("<Key>", FileList_KeyPress)
    FileList.bind("<Button-1>",FileList_Clicked)
    open_dlg.grab_set()
    open_dlg.focus_set()
    SC["takefocus"] = 0

    files = os.listdir(".")
    files.sort()
    for f in files:
	FileList.insert(END, f)

def o_proc(SC, key):	# open file using listbox selections
    open_dlg_pop()
 
def quit(SC, key):
    sys.exit(0)

def x_proc(SC, key):
    g = SC.master.geometry()
    gw = int(g.split('x')[0])
    gh = g.split('x')[1]
    gh = int(gh.split('+')[0])
    print "gw x gh = ",gw, " x ", gh	##DEBUG	
    print "extra X,Y: ",SC.extraX,",",SC.extraY
    print "zoom_factor: ", SC.zoom_factor

    xy = input("Enter factors x,y: ")
    xf = xy[0]
    yf = xy[1]
    SC.canv.xview("moveto", xf)
    SC.canv.yview("moveto", yf)

def zoom(SC, key):
    # We are changing zoom factor, so delete highlights and
    # redraw them at new zoom
    DelHighlights()
    if key == '-':
	SC.zoom_factor -= 1
	if SC.zoom_factor < 1: SC.zoom_factor = 1
    else:
	SC.zoom_factor += 1
    DoHighlights()	# if highlights are 'ON', draw them at new zoom_factor
    DoDisplay(SC)

def help_proc(SC, key):
    print HelpMsg

KeyProcs = { 'b' : b_proc, 'c' : c_proc, 'd' : d_proc,
	'h' : h_proc, 'm' : m_proc, 'n' : npt_proc, 'o' : o_proc,
	'p' : npt_proc, 't' : npt_proc, 'q' : quit, 'x' : x_proc,
	'=' : zoom, '+' : zoom, '-' : zoom,
	'?' : help_proc
	}

def KeyPress(event):
    if SC.extraX < 0:
	# compute space outside the Canvas (scroll bars, and bottom statusbar)
	SC.extraX = int(SC.sbarY["width"])
	SC.extraY = 2 * int(SC.sbarX["width"])

    if event.keysym != "Return" and event.keysym != "Escape": 
    	if KeyProcs.has_key(event.char):
	    KeyProcs[event.char](SC, event.char)
	else:
	    print "unknown key function: keysym='%s'" %  event.keysym
	    print HelpMsg

def advance_diff_in_prev_line(diffs_prev_line, index_in_prev_line):
    if index_in_prev_line < len(diffs_prev_line):
	prev_diff = diffs_prev_line[index_in_prev_line]
    else:
	prev_diff = (99999, 99999, -1)	# dummy value starts past max line length
    return prev_diff


# Initialize the Scrolled Canvas 
SC = ScrolledCanvas()
SC.highlight = 0

# get file names, open the images
if len(sys.argv) < 3:
    open_dlg_pop()
else:
    baseline_filename = sys.argv[1]
    compare_filename = sys.argv[2]
    open_files()

SC.bind_all("<Key>", KeyPress)

if __name__ == '__main__': SC.mainloop()
