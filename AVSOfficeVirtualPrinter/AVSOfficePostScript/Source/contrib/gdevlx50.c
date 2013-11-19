/*

   Copyright (C) 1999,2000  Peter B. West <pbwest@netscape.net>
   Portions Copyright (C) 1999 Aladdin Enterprises.  All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 U.S.A.

   This program may also be distributed as part of Aladdin Ghostscript,
   under the terms of the Aladdin Free Public License (the "License").

   Every copy of Aladdin Ghostscript must include a copy of the
   License, normally in a plain ASCII text file named PUBLIC.  The
   License grants you the right to copy, modify and redistribute
   Aladdin Ghostscript, but only under certain conditions described in
   the License.  Among other things, the License requires that the
   copyright notice and this notice be preserved on all copies.

  The author, Peter B. West, may be contacted via e-mail as
  pbwest@netscape.net
  or by post at
  1/32 Bent Street
  Toowong, Brisbane,
  QLD, AUSTRALIA. 4066

  http://www.powerup.com.au/~pbwest

  Based on the driver for the Lexmark 5700, pioneered by
    Stephen Taylor  setaylor@ma.ultranet.com  staylor@cs.wpi.edu
  and on other pioneering work on the Lexmark 7000 by 
    Henryk Paluch <paluch@bimbo.fjfi.cvut.cz>
 
  My heartfelt thanks to these blokes (i.e. guys, fellows).

  I should also like to thank Marija Svilans, through whom I discovered the
  work of Stephen and Henryk.

  My deepest praise and thanksgiving is reserved for the Author of all that
  is Good, Beautiful and True; my Lord and my God, Jesus Christ.
  "...all things were made through Him, and without Him was not anything
   made that was made."  John 1:3
 
  Peter B. West pbwest@netscape.net
  (First release: Friday, 8th October, 1999.)
  
*/

/*$Id: gdevlx50.c,v 1.4 2002/07/30 18:53:21 easysw Exp $*/
/*
 * Lexmark 5000 ink-jet printer driver for Ghostscript
 *
 * Black and colour cartridges supported - NO PHOTO CARTRIDGE support yet.
 *
 * defines the lx5000 device for printing in black-and-white and colour
 * at 600x600 (default), 1200x600 & 300x600 dpi, unidirectional.
 *
 * I use the command
 * gs -sOutputFile=/dev/lp0 -sDEVICE=lx5000 file.ps
 *
 * For black-only printing, use
 * gs -sOutputFile=/dev/lp0 -sDEVICE=lx5000 -dBitsPerPixel=1 file.ps
 * For compatibility with earlier versions of the code, the following
 *  equivalent command is still supported.
 * gs -sOutputFile=/dev/lp0 -sDEVICE=lx5000 -dCMYK=false file.ps
 *
 * For 1200x600 printing, add the argument
 * -r1200x600
 * For 300x600 printing, add the argument
 * -r300x600
 *
 * A number of integer valued options are available.
 * These options are accessed as arguments of the form
 *	-doptionname=optionvalue
 * 	Optionname		Optionvalue range
 *	BitsPerPixel		1 or 4
 *	HeadSeparation		1 - 30
 *	AlignA			0 - 30
 *	AlignB			0 - 15
 *	DryingTime		0 - 60
 *
 * BitsPerPixel currently only defines Black-only (1) or CMYK (4).
 *  -dBitsPerPixel=4 is the default, and is eauivalent to the older form
 *  -dCMYK=true
 * [ HeadSeparation varies from print-cartridge to print-cartridge and
 *  16 (the default) usually works fine. Stephen Taylor]
 *
 * AlignA is the horizontal alignment adjustment between the colour and
 *  the black pens.  This distance will vary with each pair of cartridges.
 *
 * AlignB is the vertical alignment distance between the colour and black
 *  pens, and will also vary with each pair of cartridges.
 *
 * These values can be determined by running the alignment test; i.e,
 * by writing the printer data file blckalgn.out to the device.
 *
 *  cat blckalgn.out >/dev/lp0
 *
 * This file is now part of the distribution for the driver.
 *
 * DryingTime is used to include a delay after printing a page, to allow the
 * page to dry to some extent before a subsequent page is printed and fed on
 * top of it.  The range of values is 0-60.
 * 
 * Ancilliary files:
 *
 * blckalgn.out		As mentioned above,this is a file of printer commands
 *			which will print the allignment page for the 5000.
 *   Usage:  cat blckalgn.out >/dev/lp0 (or appropriate printer device)
 *
 * blckhcln.out		This is a file of printer commands to print the head
 *			cleaning sequences.
 *   Usage:  cat blckhcln.out >/dev/lp0
 *
 * showcarts.out	This is a file with the printer command to move the
 *			printer cartridges into position for changing a
 *			cartridge.
 *   Usage:  cat showcarts.out >/dev/lp0
 *
 * parkcarts.out	This is a file with the printer command to move the
 *			printer cartridges back to a parked position.
 *   Usage:  cat parkcarts.out >/dev/lp0
 *
 ==================   K N O W N   P R O B L E M S   =======================
 
  PROBLEMS WITH 1200dpi horizontally

  This release was delayed by a bug in the production of black output
  (including black pixels in colour output) at 1200 Xdpi which
  manifested as the truncation of black swipes.  In the RedHat test
  page, for example, the black background to the hat in the RedHat
  logo would abruptly halt, and no more of the black swipe would be
  printed.  This was difficult to track down, because of the sheer
  size of the otuput file produced, and I ran out of time to find and
  fix it, although I was beginning to suspect a problem with the
  parallel driver.  (It is always tempting to blame someone else.)

  When I recently returned to this problem, and tried the test print
  again on the unchanged binary, the bug had disappeared.  In the
  meantime, I had added more physical memory to the system, and
  upgraded my 6.0 system to the 6.1 kernel (2.2.12), the 6.1 glibc
  version (2.1.2-11) and miscellaneous other 6.1 rpms.

  All was not lost, however.  When I tested black-only printing at
  1200dpi, the problem re-appeared.  I have come to the conclusion
  that the 5000 simply cannot print at 1200dpi.  I have tested this by
  printing solid bars of black at 600 and 1200 dpi.  The 600dpi bar
  (all nozzles on for most of the width of the paper) produces the
  expected bar.  Doing the same thing at 1200dpi produces movement of
  the printhead, but only the beginnings of a bar.  Reducing the
  demand by printing from only every second nozzle get an intermediate
  result; the printing of the bar starts, but the nozzle lines trail
  off and dry up at various points in the traversal of the
  page. leaving an effect somthing like a tattered flag.

  How does the Windows driver do it?  When printing a high density
  black-only page, the driver prints repeated overlays offset by half
  the height of the print swathe.  Although I have not analysed these
  swathes in detail, I suspect that the function of this "halving" is
  simply to halve the printed pixel density, and thereby eliminate the
  problem discussed above.

  I have taken the "half-height" printing method for granted ever
  since I read a passing reference to it in the H-P Jounal articles on
  PPA, but I misunderstood it.  I had thought that it was a way of
  performing a pseudo-doubling of the VERTICAL print density by
  overprinting adjacent pairs of lines.  I was originally unaware that
  the 5000 supported native 1200x600 resolution.  The problem, as far
  as I can determine, is that the 5000 does NOT support such printing.
  And if the printer is not going to support 1200dpi horizontally,
  neither am I.

  PROBLEMS WITH PRINTER DATA "JAMMING"

  I am experiencing an annoying problem with my 5000 on my AST Bravo
  P100 (yes, really) system under RedHat linux (6.0 with kernel
  2.2.12-20 and glibc 2.1.2-11).  The printer frequently fails to
  respond between print jobs, and sometimes between pages of the same
  print job.  The only way to wake it up is to power cycle the
  printer.  I have no idea why this is, of whether it is a general
  problem.  I would appreciate all feedback on this problem.

  I have received feedback on this from Bob Clark
  <rlc@c317689-a.scllg1.pa.home.com>.  He realised that the printer
  was queueing status data to be read from the same device.  When this
  queue (which appears to be 64 bytes long) fills up, the printer
  stalls until it is reset (by a power cycle or by some as yet
  undiscovered escape sequence), or until the status data is read.
  That's the problem, but I do not have a clean solution yet.

 *=================================================
   Other requirements for compiling this driver:-
  =================================================
   The Ghostscript 6.0 source distribution, available from
      http://www.cs.wisc.edu/~ghost/aladdin/get600.html

   This includes reasonable html documentation on compiling GS.

   For users of RPM-based systems like Red Hat, I have a source RPM
   available which automates most of the process of compiling and
   installing Ghostscript.  A discussion of the details of compiling
   and installing with this source RPM is available on my web page at
   <http://localhost:80/~pbw/ghostscript/compileGS.html>.

   The RPM includes a number of patch files which are applied to the
   pristine Ghostscript 6.0 sources.  Users of non-RPM systems can
   extract these patches from the cpio archive which is also noted in
   the instructions.

   I have compiled and tested on a RedHat 6.0 i386 system, using
   egcs-2.91.66 19990314/Linux (egcs-1.1.2 release), so the relevant
   system-specific .mak file for me was unix-gcc.mak.  The .mak file
   of general application is contrib.mak.

 */

/************************************************************************
 *			I N C L U D E   F I L E S			*
 ************************************************************************/
#include "std.h"
#include <unistd.h>
#include "gdevprn.h"
#include "gsparam.h"
#include "gsmalloc.h"

/************************************************************************
 *		     R C S   I D E N T   S T R I N G S			*
 ************************************************************************/
const char Id[]		=
	"$Id: gdevlx50.c,v 1.4 2002/07/30 18:53:21 easysw Exp $";
const char RCSFile[]	= "$RCSFile$";
const char Revision[]	= "$Revision: 1.4 $";
const char Author[]	=
	"Peter B. West   pbwest@netscape.net\n"
	"$Author: easysw $\n";
/*===========================================================================
 In gxdevice.h, the default width and height are expressed in 10ths of an
 inch.  The conversion to mm will therefore, not be exact.

 These values are immediately converted to (x,y) pixels for storage in the
 width and height fields defined in the gx_device_common macro which is
 defined in gxdevcli.h.  These in turn are converted to points and stored
 in the float MediaSize[2] array in gx_device_common by the
 std_device_part_2_ macro in gxdevice.h.

 The same macro stores the (x,y)dpi values in the float arrays
 HWResolution[2] and MarginsHWResolution[2].

 The macro prn_device_std_margins_body in gdevprn.h provides for X and Y
 offsets, as well as left, bottom, right and top margin values.  X & Y are
 expressed in inches, as are all the other margin values.

 The X & Y offsets are known as Margins in gx_device_common.	They provide
 the offset from the physical page corner (presumably top left, but may
 depend on the position of 0,0) to 0,0 in the device co-ordinate system.
 These values are converted by prn_device_std_margins_body into _negated_
 (x,y)dpi and stored in the float Margins[2] array.

 The left, bottom, right and top margin values are converted to points,
 and stored in the float HWMargins[4] array.  Note that these values are
 defined relative to the physical paper, not the co-ordinate system.  The
 Margins array is used to transform from physical page offsets to the
 device co-ordinate system.

 The implications of this are that the X & Y offset from page corner to
 0,0 should be defined as accurately as possible.  The other four margins
 should then be defined for a comprehensive coverage of the physical page.
 The definitions of paper size given in 10ths of an inch in gxdevice.h are
 a constraint.

 I presume that the intention of the transformation from 0,0 to
 the physical page is to leave the defined width and height mapped
 onto the physical page, and for the left and top margins to then restore
 those two dimensions of the accessible page.  The result would be, if the
 values of X & Y offset precisely reversed the values of left and top
 HWMargins, that the top left hand printable point would be at device
 co-ordinates 0,0.  The bottom and right HWMargins would then provide the
 necessary clipping of the co-ordinate system to keep output within the
 printable area.

 In fact, this is the way the x, Y offsets and the left and top margin
 values are set up in the default prn_device_std_body macro in gdevprn.h.
 In that macro, the X, Y offset values (expressed in inches) are simply
 copied from the left and top margins.

 What I have found with my 5000, is that it can print on virtually the
 whole area of the paper.  This shows some remarkable paper handling cap-
 ability.  This capability almost demands that 0,0 in the device co-ordin-
 ate system be _outside_ the paper area.  It is.  On my printer, the X
 offset from the physical page top left corner is 48 600ths of an inch -
 a little less than 6 points.

 The physical paper is a little narrower than 8.3 inches (210.82 mm), so
 the attempt to print right to the 8.3" margin falls short by about 16
 600ths, or about 2 points.  When 0,0 falls _within_ the page, the combin-
 ed margin clipping ensures that the driver will never issue printer com-
 mands which exceed the defined width and height of the page.  What
 happens when 0,0 is outside the page?  This implies, especially when the
 hardware is capable of printing to the edges of the paper, that the
 driver may issue print commands whose offsets exceed the defined width of
 the page, in order to print to the right edge of the paper.	I don't know
 whether Ghostscript will allow this.

                   Print width
              |<------------ 8.3" ----------->|
              |                               |
              |                               |
              +-------------- ~ --------------+
                                              |<-->| 2 pts
              |      |<----- Paper width --------->|
        6 pts |<---->|                             |
                     +-------------- ~ ------------+

 In this case, the X offset is going to be negative, and the device co-
 ordinate value of X for the left edge of the paper is going to almost 50
 600ths.  It is a good idea to apply some restrictions on printing to the
 edge of the paper, so a HWMargin should be applied.

 The problem with setting the margins is this: Ghostscript knows where the
 top and left sides of the page are relative to 0,0, because it is spec-
 ifically informed of these values.  It has no way of knowing where the
 bottom and right of the page are, except from the height and width values.
 These are only provided to an accuracy of a tenth of an inch, so the
 assumed right and bottom margins may fall short of or overrun the edge of
 the paper.

 In these circumstances, it seems a good idea to provide HWMargin values
 designed to enforce a uniform margin around all of the edges.  The left &
 top margins can be given directly; the others must be worked out so that
 when applied with reference to the height and width parameters, they
 result in the same margin.

 For example, on my 5000, 0,0 is approximately 48/600" (about 6 points) to
 the left of the physical page.  When I print at an offset from 0 of
 8.3", the default A4 width, the print is approximately 32/600" (about 4
 points) inside the right edge of the page.  That is, 8.3" is (48-32)/600"
 too wide for the physical page.

 4 points (4/72") is 0.05555", and 32/600" is 0.053333".  If I set a left
 HWMargin of 0.053", and Ghostscript obliges by mapping a new printable
 origin 4 points inside the left edge of the paper, with 8.3" of print-
 able line to the right of it, my left margin will need to account for the
 extra 16/600" that GS thinks exists on the right, and for the actual
 margin of 0.053" that I want to establish on the right.  I think.

 The interest in the top margin is that it seems to be keyed to the colour
 pens.  These form three blocks of nozzles vertically aligned in the
 colour print head.

 ---    -- +-----+                 v Alignment B
  ^     ^  |     | v              --- +-----+ ---
  |     64 |  C  | -- --- +-----+ --- |     |  ^
  |  v  v  |     | 40  ^  |     |  ^  |     |  |
  |  -- -- +-----+ --  |  |     |     |     |  |
  |  24            ^   |  |     |     |     |  |
  |  -- -- +-----+     |  |     |     |     |  |
  |  ^  ^  |     |     |  |     |     |     |
 240    64 |  M  |    192 |  K  |     |  K  | 208
  |  v  v  |     |     |  |     |     |     |
  |  -- -- +-----+     |  |     |     |     |  |
  |  24            v   |  |     |     |     |  |
  |  -- -- +-----+ --  |  |     |     |     |  |
  |  ^  ^  |     | 40  v  |     |     |     |  |
  |     64 |  Y  | -- --- +-----+     |     |  v
  v     v  |     | ^                  +-----+ ---
 ---    -- +-----+

 The colour pens have 64 nozzles each, and the black has 208.  Black can
 also be driven in colour compatibility mode, when it uses 192 nozzles
 only, the same as the total number of colour nozzles.  In order to align
 the black nozzles with the colour, an alignment value is specified in the
 initialization sequence for the page.  This value is in turn determined
 by printing an alignment page under control of the printer driver (See
 ancilliary files above.)

 Note that when an odd number is specified as Alignment B, an odd numbered
 nozzle will be mapped as nozzle 0 of the 192 nozzle black pen, so the
 even/odd association of nozzle numbers will be reversed; i.e., nozzle 0
 will become and ODD nozzle.

 The paper handler seems to assume that the paper load position is with
 the page lined up immediately before the lowest nozzle of the YELLOW pen.
 (Note that the paper is being fed in from the bottom of the printheads.)
 When I issue a paper feed instruction of 480 (240x2) 1200ths, and print a
 three-colour bar, the top of the CYAN pen is just on the physical page.
 Efectively, the Y component of 0,0 is right at the top of the page, with
 respect to the YELLOW pen.  Y=0 with respect to the 192 nozzle black pen
 is always constant, and with respect to the 208 pen, can be determined
 from Alignment B.


 *===========================================================================*/

/************************************************************************
 *			B A S I C   M A C R O S				*
 ************************************************************************/

#define A5_11_5000	1
#define A5_11_5700	2


#define ALIGN_A_DEF	15
#define ALIGN_A_OFFSET	5	/* in pageinit escape sequence */
#define ALIGN_B_DEF	8
#define ALIGN_B_OFFSET	6	/* in pageinit escape sequence */

#define HEADSEP_DEF	16
				/* number of pixels between even columns in */
				/* output and odd ones */
#define DRY_TIME_DEF	0	/* Default pause for page to dry */


/************************************************************************
 *	     P R I N T E R   T Y P E   D E F I N I T I O N S	 	*
 ************************************************************************/

#ifndef LX_BI
#   define LX_UNI
				/* or define LX_BI */
#endif
#define MIN_LX5000_X	300
#define MAX_LX5000_X	1200
#define DEF_LX5000_X	600
#define MIN_LX5000_Y	600
#define MAX_LX5000_Y	1200
#define DEF_LX5000_Y	MIN_LX5000_Y

#ifndef LX5000_XDPI
#   define LX5000_XDPI	DEF_LX5000_X
#endif
#ifndef LX5000_YDPI
#   define LX5000_YDPI	DEF_LX5000_Y
#endif

				/* Multiply lines by FEED_FACTOR when feeding
				   paper.  The 5000 feeds in 1200ths. */
#define FEED_FACTOR	( 1200/LX5000_YDPI )

#define LX5000_XOFFSET_TO_0_0_XDPI	(-48.0)
#define LX5000_YOFFSET_TO_0_0_YDPI	(0.0)

#define LX5000_XOFFSET_TO_0_0	( LX5000_XOFFSET_TO_0_0_XDPI/LX5000_XDPI )
#define LX5000_YOFFSET_TO_0_0	( LX5000_YOFFSET_TO_0_0_YDPI/LX5000_YDPI )

/* The theory */
#define LX5000_LEFT_HWMARGIN_INS	0.053
#define LX5000_BOTTOM_HWMARGIN_INS	0.067
#define LX5000_RIGHT_HWMARGIN_INS	0.08
#define LX5000_TOP_HWMARGIN_INS		0.053
/**/
/* The practice
??
*/

#define LINE_PAD_BYTES	8
				/* number of bytes of padding on each end of */
				/* scan line to account for head separation */
				/* N.B. Keep this value 16-bit aligned.  */

#define RIGHTWARD	0
#define LEFTWARD	1

#define SWIPE_WORD_BITS	16
				/* Bits per directory word */
#define BLACK_NOZZLES	208
				/* height of printhead in pixels */
#define _1COLOUR_NOZZLES 64
				/* Height of one colour pen in pixels */

				/* Height of colour compatible black swipe */
#define BLK_COLOUR_NOZZLES	(_1COLOUR_NOZZLES * 3)

				/* number of shorts described by each */
				/* full black column directory */
#define BLK_SWIPE_WORDS		( BLACK_NOZZLES / SWIPE_WORD_BITS )
#define COLOUR_SWIPE_WORDS	( BLK_COLOUR_NOZZLES / SWIPE_WORD_BITS )
				/* and for a colour swipe command */
#define _1COLOUR_WORDS		( _1COLOUR_NOZZLES / SWIPE_WORD_BITS )
				/* and for a single colour pen */

#define BLK_DIRECTORY_MASK	(( ~0 << BLK_SWIPE_WORDS ) ^ ~0 )
#define COLOUR_DIRECTORY_MASK	(( ~0 << COLOUR_SWIPE_WORDS ) ^ ~0 )
#define DIRECTORY_TYPE_BIT	0x2000
				/* Directory type: Set = normal;
					   unset = repeat compression */

#define COLOUR_PEN_GAP	24	/* Defined in equivalent nozzles.  This will */
				/* also be the offset from the top of the */
				/* first colour pen to the top of the colour */
				/* compatible black pen. */
#define COLOUR_PEN_DIFF	( _1COLOUR_NOZZLES + COLOUR_PEN_GAP )
				/* The difference between corresponding */
				/* nozzle positions (e.g. bottom nozzle) on */
				/* contiguous colour pens. */

				/* The intial pen scanline positions are */
				/* defined with reference to the first */
				/* printable line (line 0) on the page.  The */
				/* initial colour cartridge position is */
				/* immediately above line 0. */
#define INITIAL_YELLOW_BOTTOM_LINE	( -1 )

				/*  The pen buffer size must be a power of */
				/*  2, so that the index into the buffer */
				/*  will either wrap within that size on */
				/*  increment, or be able to be masked to */
				/*  force a wrap of the index into the */
				/*  circular pen buffers.  See discussion */
				/*  of pen buffers in lx5000_print_page(). */
#define COLOUR_BUF_MASK		0xff
				/* 2^8 - 1 */
#define COLOUR_BUF_LINES	( COLOUR_BUF_MASK + 1 )

/************************************************************************
 *		S W I P E   C O M M A N D   M A C R O S			*
 ************************************************************************/

#define SWIPE_LEADER	0x1b, '*', 4
				/* Lead-in bytes for swipe command */

#define CMDLEN_X	3
				/* Index in swipe command of 4 byte */

#define DIRECTION_X	7
				/* Index in swipe cmd of direction flag */

#define UNIDIRECTIONAL	0
#define BIDIRECTIONAL	1

#define HEADSPEED_X	8
				/* Index in swipe cmd of head speed */
				/* Head speeds for various densities */
#define _300X		1
#define _600X		2
#define _1200X		5

#define PEN_X		9
				/* Index of 2 byte pen selectors */
#define BLACK0		1
#define BLACK1		1
#define COLOUR0		2
#define COLOUR1		0

#define NOZZLE_COUNT_X	11
				/* Index of nozzle count selector */
#define _192NOZZLES	0x18
#define _208NOZZLES	0x1a

#define UNKNOWN1_X	12
				/* Index of an unknown byte */
#define UNKNOWN1VAL	0
#define NUM_COLUMNS_X	13
				/* Index in swipe command of 2 byte */
				/* column count				*/
#define _1ST_COLUMN_X	15
				/* Index in swipe command of 2 byte */
				/* first column horizontal offset */
#define LAST_COLUMN_X	17
				/* Index in swipe command of 2 byte */
				/* last column horizontal offset	*/

#define SWIPE_HDR_END_X	19
#define SWIPE_HDR_END_LEN	7
#define SWIPE_HDR_END	0, 0, '+', 'p', 'b', 'w', 1
				/* Final byte sequence of swipe cmd header */

#define SWIPE_HDR_LEN	( SWIPE_HDR_END_X + SWIPE_HDR_END_LEN )

/************************************************************************
 *		C O L O U R   H A N D L I N G   M A C R O S		*
 ************************************************************************/
/* Macros for the relative position on the colour components in */
/* gx_color_index, the driver defined representation of individual */
/* colours. */

#define BLACK_X		0
#define YELLOW_X	1
#define MAGENTA_X	2
#define CYAN_X		3

#define LO_PEN		0
#define HI_PEN		1

				/* Colour defines for lx5000 black printer */

#   define NUM_COMPONENTS_BLK	1
#   define BITS_PER_PIXEL_BLK	1
#   define MAX_GREY_BLK		1
#   define MAX_RGB_BLK		0
#   define DITHER_GREYS_BLK	2
#   define DITHER_COLOURS_BLK	0

				/* Colour defines for lx5000 colour printer */
#   define NUM_COMPONENTS_CMY	4
#   define BITS_PER_PIXEL_CMY	4
#   define MAX_GREY_CMY		1
#   define MAX_RGB_CMY		1
#   define DITHER_GREYS_CMY	2
#   define DITHER_COLOURS_CMY	2

#   define MIN_COLOUR		BLACK_X
#   define MAX_COLOUR		CYAN_X
#   define BLACK_PEN		BLACK_X
#   define _1ST_CMY_COLOUR	YELLOW_X
#   define LAST_CMY_COLOUR	CYAN_X

#define BPP			BITS_PER_PIXEL_CMY
				/* N.B. This is only required for
				   processCMYKline */
#   define PIXEL_MASK		(( 1 << BPP ) - 1 )
#   define INITIAL_PIXEL_SHIFT	( ( sizeof( uchar ) * 8 ) - BPP )

#define NUM_COLOURS		( MAX_COLOUR - MIN_COLOUR + 1 )
#define PENS_PER_COLOUR	2
#define NUM_PENS		( NUM_COLOURS * PENS_PER_COLOUR )

#define BITS_PER_COLOUR		(BITS_PER_PIXEL_CMY / NUM_COMPONENTS_CMY)

#define DEF_NUM_COLOURS		NUM_COLOURS
#define DEF_PENS_PER_COLOUR	1
#define DEF_LINE_INCREMENT	1

/* !!!!!!!!!!!!!!!!!!!!! WARNING, WARNING, WILL ROBINSON !!!!!!!!!!!!!!!!!!!!*/
				/* If BPP ever exceeds 8, this will break
				   badly, as will the whole of processCMYKline.
				   PIXELS_PER_BYTE is used when skipping over
				   empty bytes in processCMYKline().  Each
				   empty scanline byte skips over this many
				   bits in the colourBufs.	*/
#define PIXELS_PER_BYTE		(8 / BPP)

#define COLOUR_MASK	(( 1 << ( BITS_PER_COLOUR ) ) - 1 )

/************************************************************************
 *			Memory allocation macros			*
 *	    ( For use with getColourBufs & releaseAllocations )		*
 ************************************************************************/
#define ALLOCATE	true
#define DEALLOCATE	false

/************************************************************************
 *		" F U N C T I O N A L "   M A C R O S			*
 ************************************************************************/
/************************************************************************
 *			Macro to fill the swipe header			*
 ************************************************************************/

#define FILL_SWIPE_HEADER( swipeHdr, len, dir, speed, pen0, pen1, nozzles, \
	unknown, numcols, firstcol, lastcol ) \
	    swipeHdr[ CMDLEN_X ]	= ( ( len >> 24 ) & 0xff ); \
	    swipeHdr[ CMDLEN_X + 1 ]	= ( ( len >> 16 ) & 0xff ); \
	    swipeHdr[ CMDLEN_X + 2 ]	= ( ( len >> 8 ) & 0xff ); \
	    swipeHdr[ CMDLEN_X + 3 ]	= ( len & 0xff ); \
	    swipeHdr[ DIRECTION_X ]	= dir; \
	    swipeHdr[ HEADSPEED_X ]	= speed; \
	    swipeHdr[ PEN_X ]		= pen0; \
	    swipeHdr[ PEN_X + 1 ]	= pen1; \
	    swipeHdr[ NOZZLE_COUNT_X ]	= nozzles; \
	    swipeHdr[ UNKNOWN1_X ]	= unknown; \
	    swipeHdr[ NUM_COLUMNS_X ]	= ( numcols >> 8 ); \
	    swipeHdr[ NUM_COLUMNS_X + 1 ] = ( numcols & 0xff ); \
	    swipeHdr[ _1ST_COLUMN_X ]      = ( firstcol >> 8 ); \
	    swipeHdr[ _1ST_COLUMN_X + 1 ]  = ( firstcol & 0xff ); \
	    swipeHdr[ LAST_COLUMN_X ]      = ( lastcol >> 8 ); \
	    swipeHdr[ LAST_COLUMN_X + 1 ]  = ( lastcol & 0xff )


/************************************************************************
 *   Macros to manipulate bit pointers in bitBuf & scanPixels structs	*
 ************************************************************************/
#define BIT_TO_MASK( n ) ( 0x80 >> ( n ) )
#define INC_BIT( bitPtr ) \
do { \
    if ( ! ((bitPtr).xBit >>= 1 )) \
	    { (bitPtr).xByte++ ; (bitPtr).xBit = 0x80; } \
} while (0)

#define DEC_BIT( bitPtr ) \
do { \
    if ( ! ( (bitPtr).xBit = (( (bitPtr).xBit << 1 ) & 0xff ))) \
	{ (bitPtr).xByte-- ; (bitPtr).xBit = 1; } \
} while (0)

/************************************************************************
 *		Macro for defining gx_device_procs structure		*
 ************************************************************************/

#define lx5000_proctab(get_params, put_params, map_color_rgb, map_cmyk_color)\
{	gdev_prn_open,\
	gx_default_get_initial_matrix,\
	NULL,	/* sync_output */\
	gdev_prn_output_page,\
	gdev_prn_close,\
	NULL,	/* map_rgb_color */\
	map_color_rgb,\
	NULL,	/* fill_rectangle */\
	NULL,	/* tile_rectangle */\
	NULL,	/* copy_mono */\
	NULL,	/* copy_color */\
	NULL,	/* draw_line */\
	NULL,	/* get_bits */\
	get_params,\
	put_params,\
	map_cmyk_color,\
	NULL,	/* get_xfont_procs */\
	NULL,	/* get_xfont_device */\
	NULL,	/* map_rgb_alpha_color */\
	gx_page_device_get_page_device	/* get_page_device */\
}

/************************************************************************
 *   T Y P E D E F S   E N U M S   &   E X T E R N A L   S T O R A G E	*
 ************************************************************************/
				/* The procedure descriptors */
				/* declare functions */
static dev_proc_print_page(lx5000_print_page);
static dev_proc_get_params(lx5000_get_params);
static dev_proc_put_params(lx5000_put_params);

static dev_proc_map_cmyk_color(lx5000_map_cmyk_color);
static dev_proc_map_color_rgb(lx5000_map_color_rgb);

static const gx_device_procs lx5000_procs = 
    lx5000_proctab(
                     lx5000_get_params,
		     lx5000_put_params,
		     lx5000_map_color_rgb,
		     lx5000_map_cmyk_color
		     );

				/* The device descriptors */
				/* define a subclass containing useful state */
				/* a sub-class of gx_device_printer */
typedef struct lx5000_device_s {
    gx_device_common;
    gx_prn_device_common;
    int		alignA;
    int		alignB;
    int		headSeparation;
    int		dryTime;	/* Seconds delay at end of page for drying */
    int		pensPerColour;	/* 1 for 600Y; 2 for 1200Y */
    int		lineIncrement;	/* 1 for 600Y; 2 for 1200Y */
    int		scanLineBytes;	/* Returned by GS */
    int		penLineBytes;	/* When scanline colour elements are each
				   reduced to a single bit, this is the result-
				   ing line length in bytes.	*/
    int		penLineLen;	/* penLineBytes + BOL + EOL padding */
    int		penBufSize;	/* penLineLen * no. of lines */
    int		swipeBufSize;	/* calculated size of a swipe command buffer */
    bool	isCMYK;
} lx5000_device;

				/* Define a structure for a pointer to an */
				/* individual bit in a scanline	 */
typedef struct bufBit_s {
    byte 	*xByte;		/* Pointer to byte in buffer */
    uchar	xBit;		/* Mask for invidual bit in byte */
} bufBit;


typedef struct penData_s {
    int		topLine;	/* Top printable line of this pen */
    int		bottomLine;	/* Bottom printable line of this pen */
    int		nextPrintLine;	/* Next line to be printed, this pen */

    int		initialBottomLine;
    int		bottomToBottomYellow;
    int		topToBottomYellow;
    int		finalLine;
} penData;

				/* Structure for extracting pixels from the
				   initial scan line, which must be int
				   aligned.			*/
typedef struct scanPixels_s {
    uchar	*scanByte;	/* Pointer to a byte in the scan buffer */
    int		pixShift;	/* Shift required to get next pixel to */
} scanPixels;			/* the LSBits of the word. */

				/* Standard lx5000 device */
lx5000_device far_data gs_lx5000_device = {
    prn_device_margins_body
    	( lx5000_device,
	  lx5000_procs,
	  "lx5000",
	  DEFAULT_WIDTH_10THS,
	  DEFAULT_HEIGHT_10THS,
	  LX5000_XDPI,		/* x dpi */
	  LX5000_YDPI,		/* y dpi */
				/* Offset inches from page left to 0,0 */
	  LX5000_XOFFSET_TO_0_0,
				/* Offset inches from page top to 0,0 */
	  LX5000_YOFFSET_TO_0_0,
	  LX5000_LEFT_HWMARGIN_INS,	/* margins */
	  LX5000_BOTTOM_HWMARGIN_INS,
	  LX5000_RIGHT_HWMARGIN_INS,
	  LX5000_TOP_HWMARGIN_INS,
	  NUM_COMPONENTS_CMY,	/* colour info  */
	  BITS_PER_PIXEL_CMY,	/*	"	*/
	  MAX_GREY_CMY,		/*	"	*/
	  MAX_RGB_CMY,		/*	"	*/
	  DITHER_GREYS_CMY,	/*	"	*/
	  DITHER_COLOURS_CMY,	/*	"	*/
	  lx5000_print_page
	  ),
    ALIGN_A_DEF,		/* default(!) horizontal pen alignment	*/
    ALIGN_B_DEF,		/* default(!) vertical pen alignment	*/
    HEADSEP_DEF,		/* default headSeparation value */
    DRY_TIME_DEF,		/* Default page drying time */
    DEF_PENS_PER_COLOUR,
    DEF_LINE_INCREMENT,
    0,				/* scanLineBytes */
    0,				/* penLineBytes */
    0,				/* penLineLen */
    0,				/* penBufSize */
    0,				/* swipeBufSize */
    true			/* isCMYK - defaults to using colour	*/
};

static const gx_device_color_info color_info_cmy =
     dci_values(NUM_COMPONENTS_CMY,
		BITS_PER_PIXEL_CMY,
		MAX_GREY_CMY,
		MAX_RGB_CMY,
		DITHER_GREYS_CMY,
		DITHER_COLOURS_CMY);

static const gx_device_color_info color_info_blk =
     dci_values(NUM_COMPONENTS_BLK,
		BITS_PER_PIXEL_BLK,
		MAX_GREY_BLK,
		MAX_RGB_BLK,
		DITHER_GREYS_BLK,
		DITHER_COLOURS_BLK);

/************************************************************************
 ************************************************************************
 *		     D R I V E R   P R O C E D U R E S			*
 ************************************************************************
 ************************************************************************/

/************************************************************************
 *		    U T I L I T Y   P R O C E D U R E S			*
 ************************************************************************/

/*----------------------------------------------------------------------*
 *	i n i t P e n C o n s t a n t s ( )				*
 *----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
static void
initPenConstants( lx5000_device *lx5000dev,
		  penData pens[NUM_COLOURS][PENS_PER_COLOUR] )
{
				/* Indexed by pensPerColour.  */
    static const int	nozzleCount[ NUM_COLOURS ][ PENS_PER_COLOUR + 1 ] =
    {
	{ -1, BLACK_NOZZLES, (BLACK_NOZZLES / 2) },
	{ -1, _1COLOUR_NOZZLES, (_1COLOUR_NOZZLES / 2) },
	{ -1, _1COLOUR_NOZZLES, (_1COLOUR_NOZZLES / 2) },
	{ -1, _1COLOUR_NOZZLES, (_1COLOUR_NOZZLES / 2) }
    };

    int		colour, pen;
    int		pensPerColour	= lx5000dev->pensPerColour;

    pens[YELLOW_X][LO_PEN].initialBottomLine	= -1;
    pens[MAGENTA_X][LO_PEN].initialBottomLine =
				pens[YELLOW_X][LO_PEN].initialBottomLine
							- COLOUR_PEN_DIFF;
    pens[CYAN_X][LO_PEN].initialBottomLine =
				pens[MAGENTA_X][LO_PEN].initialBottomLine
							- COLOUR_PEN_DIFF;
    pens[BLACK_X][LO_PEN].initialBottomLine =
				pens[YELLOW_X][LO_PEN].initialBottomLine
						- COLOUR_PEN_GAP
				+ ( SWIPE_WORD_BITS - lx5000dev->alignB );

    for ( colour = 0; colour < NUM_COLOURS; colour++ )
    {
	pens[colour][LO_PEN].bottomToBottomYellow
				= pens[YELLOW_X][LO_PEN].initialBottomLine
	    			- pens[colour][LO_PEN].initialBottomLine;
    }

    if ( pensPerColour == 1 )
    {
	pens[BLACK_X][LO_PEN].topToBottomYellow =
					BLK_COLOUR_NOZZLES + COLOUR_PEN_GAP
						+ lx5000dev->alignB - 1;
	pens[YELLOW_X][LO_PEN].topToBottomYellow =  _1COLOUR_NOZZLES - 1;
	pens[MAGENTA_X][LO_PEN].topToBottomYellow =
				pens[YELLOW_X][LO_PEN].topToBottomYellow
						+ COLOUR_PEN_DIFF;
	pens[CYAN_X][LO_PEN].topToBottomYellow =
				pens[MAGENTA_X][LO_PEN].topToBottomYellow
						+ COLOUR_PEN_DIFF;

	for ( colour = 0; colour < NUM_COLOURS; colour++ )
	{
	    pens[colour][HI_PEN].topToBottomYellow
				= pens[colour][LO_PEN].topToBottomYellow;
	    pens[colour][HI_PEN].initialBottomLine
				= pens[colour][LO_PEN].initialBottomLine;
	    pens[colour][HI_PEN].bottomToBottomYellow
				= pens[colour][LO_PEN].bottomToBottomYellow;
	}
    }
    else			/* pensPerColour > 1 */
    {
	pens[BLACK_X][HI_PEN].topToBottomYellow =
					BLK_COLOUR_NOZZLES + COLOUR_PEN_GAP
						+ lx5000dev->alignB - 1;
	pens[BLACK_X][LO_PEN].topToBottomYellow =
					pens[BLACK_X][HI_PEN].topToBottomYellow
					- nozzleCount[BLACK_X][pensPerColour];
	pens[YELLOW_X][HI_PEN].topToBottomYellow =  _1COLOUR_NOZZLES - 1;
	pens[YELLOW_X][LO_PEN].topToBottomYellow =
				pens[YELLOW_X][HI_PEN].topToBottomYellow
					- nozzleCount[YELLOW_X][pensPerColour];
	pens[MAGENTA_X][HI_PEN].topToBottomYellow =
				pens[YELLOW_X][HI_PEN].topToBottomYellow
						+ COLOUR_PEN_DIFF;
	pens[MAGENTA_X][LO_PEN].topToBottomYellow =
				pens[MAGENTA_X][HI_PEN].topToBottomYellow
					-nozzleCount[MAGENTA_X][pensPerColour];
	pens[CYAN_X][HI_PEN].topToBottomYellow =
				pens[MAGENTA_X][HI_PEN].topToBottomYellow
						+ COLOUR_PEN_DIFF;
	pens[CYAN_X][LO_PEN].topToBottomYellow =
				pens[CYAN_X][HI_PEN].topToBottomYellow
					- nozzleCount[CYAN_X][pensPerColour];

	for ( colour = 0; colour < NUM_COLOURS; colour++ )
	{
	    pens[colour][HI_PEN].bottomToBottomYellow =
				    pens[colour][LO_PEN].bottomToBottomYellow
				    + nozzleCount[colour][pensPerColour];
	    pens[colour][HI_PEN].initialBottomLine =
				    pens[colour][LO_PEN].initialBottomLine
				    - nozzleCount[colour][pensPerColour];
	}
    }

    for ( colour = 0; colour < NUM_COLOURS; colour++ )
    {
	for ( pen = 0; pen < PENS_PER_COLOUR; pen++ )
	{
	    pens[colour][pen].finalLine =
		( lx5000dev->height ) + pens[colour][pen].topToBottomYellow;
	}
    }
}

/*----------------------------------------------------------------------*
 *	p a g e I n i t ( )						*
 *----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
static void
pageInit( unsigned int alignA, unsigned int alignB, FILE *prn_stream )
{
    static char page_init[] = {
        0x1b, '*', 'm', 0, 0x40, ALIGN_A_DEF, ALIGN_B_DEF, 0xf, 0xf
    };

    page_init[ALIGN_A_OFFSET] = (uchar)alignA;
    page_init[ALIGN_B_OFFSET] = (uchar)alignB;
    
    fwrite( page_init, 1, sizeof( page_init ), prn_stream );
}

/*----------------------------------------------------------------------*
 *	p a g e E n d ( )						*
 *----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
static void
pageEnd( FILE *prn_stream )
{
    static const char page_end[] = {
	0x1b,'*', 7, 0x65
    };

    fwrite( page_end, 1, sizeof( page_end ), prn_stream );
				/* Do it twice - that's what the Windows */
				/* driver for the 5000 does	*/
    fwrite( page_end, 1, sizeof( page_end ), prn_stream );
}

/*----------------------------------------------------------------------*
 *	f e e d P a p e r ( )						*
 *----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
static void
feedPaper( lx5000_device *lx5000dev, int newLine, int *currentLine,
	   penData pens[NUM_COLOURS][PENS_PER_COLOUR],
	   FILE *prn_stream )
{
    static const char feed_paper[] = {
	0x1b, '*', 3
    };

    ushort	_1200ths;
    int		colour, pen;

    if ( newLine >= *currentLine )
    {
	_1200ths = (ushort)(( newLine - *currentLine ) * FEED_FACTOR );
	fwrite( feed_paper, 1, sizeof( feed_paper ), prn_stream );
	putc( _1200ths >> 8 , prn_stream);
	putc( _1200ths & 0xff, prn_stream );

	*currentLine			= newLine;

	for ( colour = 0;
	      colour < lx5000dev->color_info.num_components;
	      colour++ )
	    for ( pen = 0; pen < lx5000dev->pensPerColour; pen++ )
	    {
		pens[colour][pen].topLine =
		    newLine - pens[colour][pen].topToBottomYellow;
		pens[colour][pen].bottomLine =
		    newLine - pens[colour][pen].bottomToBottomYellow;
	    }

    }
}


/*----------------------------------------------------------------------*
 *	g e t C o l o u r B u f s ( )					*
 *----------------------------------------------------------------------*
  Return pointers to the allocated buffers in those pointers whose addresses
  have been passed as parameters, or release the allocations.

  Allocation is indicated by a true value of the argument `allocate'.

  The buffers themselves are allocated and the pointers to them are kept
  in static storage.  On the first call, the buffers are allocated, and the
  pointers to them are maintained in static storage.  On all subsequent
  calls, the pointer contents are simply returned.

  Deallocation is indicated by a false value of the argument `allocate'.

  If the pointers are non-null, the allocated memory is released, and
  the pointer values are set to NULL.

  N.B.  The arrays are defined in terms of the maximum values for the array
  dimensions, even though the actual usage of each dimension is dynamically
  determined.  E.g. colourBufs is dimensioned [NUM_COLOURS],
  even though the number of colour dimensions actually used is determined
  by the variable numColours, which will be 1 if the driver is being used
  in black-only mode.
 *----------------------------------------------------------------------*/
int
getColourBufs( lx5000_device *lx5000dev,
	       byte **lineBufferPtr, byte *colourBufPtrs[],
	       byte **swipeBufPtr, bool allocate )
{
    static byte  	*colourBufs[ NUM_COLOURS ];
				/* Only an array of pointers; OK if too big */
    static byte 	*lineBuffer	= NULL;
    static byte 	*swipeBuf	= NULL;

    int	colour;
    int	colourBufNull	= 0;
    int	numColours	= lx5000dev->color_info.num_components;

    if ( allocate )
    {
	if ( lineBuffer == NULL )
	{
				/*  Initialise the pen buffers	*/
	    for ( colour = 0; colour < numColours; colour++ )
		colourBufs[ colour ] = NULL;

	    /*---------------------------------------------------------------*
	      Derive the size of a colour buffer line:  If the number of
	      components is one and the number of bits per pixel is one, then
	      there is only one bit per pixel in the scan line, so the colour
	      buffer line is the same size.

	      Otherwise, reduce each gx_color_index element in the scan line to
	      one bit in the penprint buffer.
	      *--------------------------------------------------------------*/

	    if (lx5000dev->color_info.num_components > GX_DEVICE_COLOR_MAX_COMPONENTS)
		return_error(gs_error_limitcheck);

	    lx5000dev->scanLineBytes = gdev_mem_bytes_per_scan_line(
						(gx_device *)lx5000dev );
	    lx5000dev->penLineBytes	=
		( lx5000dev->color_info.num_components == 1
			    && lx5000dev->color_info.depth == 1 )
		? lx5000dev->scanLineBytes
		: ( lx5000dev->scanLineBytes / lx5000dev->color_info.num_components);
	    
	    lx5000dev->penLineLen =
			lx5000dev->penLineBytes + ( LINE_PAD_BYTES * 2);
    
	    lx5000dev->penBufSize
				= lx5000dev->penLineLen * COLOUR_BUF_LINES;

	    /* swipeBuf size:
	     * No. of columns = No. of bits in the linebuf
	     * Bits per column = maximum swipe height
	     * Bytes per column = Bits per column / 8 + 2 byte directory
	     * Total bytes = Bytes/column * no. of columns + header bytes
	     */
	    lx5000dev->swipeBufSize	=
		((lx5000dev->penLineLen * 8)
		 * ((BLACK_NOZZLES / 8) + 2) + SWIPE_HDR_LEN);

				/* Allocate a buffer for a single scan line */
	    lineBuffer	= (byte *)gs_alloc_byte_array
		( gs_lib_ctx_get_non_gc_memory_t(), lx5000dev->scanLineBytes, 1,
		  "lx5000_print_page(lineBuffer)" );

	    swipeBuf	= (byte *)gs_alloc_byte_array
		( gs_lib_ctx_get_non_gc_memory_t(), lx5000dev->swipeBufSize, 1,
		  "lx5000_print_page(swipeBuf)" );

	    for ( colour = 0 ; colour < numColours; colour++ )
	    {
		if ( ( colourBufs[colour] =
		       (byte *)gs_alloc_byte_array
		       ( gs_lib_ctx_get_non_gc_memory_t(), lx5000dev->penBufSize, 1,
			 "lx5000_print_page(colourBufs)"
			 )
		       ) == NULL )
		{
		    colourBufNull = 1;
		    colour = numColours;
		}
	    }
				/* Check allocations */
	    if ( lineBuffer == NULL || colourBufNull || swipeBuf == NULL ) {
		getColourBufs( lx5000dev, lineBufferPtr, colourBufPtrs,
				    swipeBufPtr, DEALLOCATE );
		return_error( gs_error_VMerror );
	    }
	}
	/* Clear the black buffer, iff ! isCMYK.  If CMYK, scan lines are
	   processed by processCMYKlines(), and buffer lines are cleared
	   individually, before a new scan line is processed.  As part of this
	   clearing, the line EOL and BOL pads are also cleared.
	   If ! CMYK, the black-only scan line is read or copied directly
	   into the black buffer, without clearing the buffer.  Therefore, the
	   line pad regions must either be cleared individually for each line
	   processed, or cleared once when the buffer is allocated.
	*/
	if ( ! lx5000dev->isCMYK )
	    memset( colourBufs[BLACK_X], 0, lx5000dev->penBufSize );
	   
				/* Return the values */
	*lineBufferPtr	= lineBuffer;
	*swipeBufPtr	= swipeBuf;
	for ( colour = 0; colour < numColours; colour++ )
	    colourBufPtrs[colour] = colourBufs[colour];

	return 0;
    }
    else			/* Deallocate the buffers */
    {
	for ( colour = 0; colour < numColours; colour++ )
	{
	    if ( colourBufs[colour] != NULL )
		gs_free_object( gs_lib_ctx_get_non_gc_memory_t(),
				(char *)colourBufs[colour],
				"lx5000_print_page(colourBufs)" );
	    colourBufs[ colour ]	= NULL;
	    colourBufPtrs[ colour ]	= NULL;
	}
	if ( swipeBuf != NULL )
	    gs_free_object( gs_lib_ctx_get_non_gc_memory_t(),
			    (char *)swipeBuf, "lx5000_print_page(swipeBuf)" );
	swipeBuf	= NULL;
	*swipeBufPtr	= NULL;
	if ( lineBuffer != NULL )
	    gs_free_object( gs_lib_ctx_get_non_gc_memory_t(),
		    (char *)lineBuffer, "lx5000_print_page(lineBuffer)" );
	lineBuffer	= NULL;
	*lineBufferPtr	= NULL;
	return 0;
    }
}


/*----------------------------------------------------------------------*
 *	p r o c e s s C M Y K l i n e ( )				*
 *----------------------------------------------------------------------*
  Given a scan line number, a pointer to a scan line, an array of pointers to
  individual colour buffers, and an array of the individual pen scanBit
  arrays, distribute the scan line elements into the colour buffers.

  N.B. The empty indicator for each line for each pen buffer must also be
  set by this procedure.
  
  Things like dithering may well end up in here.
 *----------------------------------------------------------------------*/
static void
processCMYKline( lx5000_device *lx5000dev, int linenum,
		 byte *lineBuffer, byte *scanLine,
		 byte *colourBufs[NUM_COLOURS],
		 bufBit
		 colourLines[NUM_COLOURS][PENS_PER_COLOUR][COLOUR_BUF_LINES],
		 penData pens[NUM_COLOURS][PENS_PER_COLOUR],
		 bool lineEmpty[NUM_COLOURS][ COLOUR_BUF_LINES ] )
{
    int		colour;
    int		numColours = lx5000dev->color_info.num_components;

    uchar	colourBits[ 8 ];
    uchar	lineIndex;
    uchar	scanPixel;
    scanPixels	nextPixel;
    bufBit	nextBit[ 8 ];
				/* The end of the (scan)line. */
    uchar	*scanEnd	= scanLine + lx5000dev->scanLineBytes;

    nextPixel.scanByte	= scanLine;  /* Set up the moving pixel pointer. */
    nextPixel.pixShift	= INITIAL_PIXEL_SHIFT;
    lineIndex		= (uchar)( linenum & COLOUR_BUF_MASK );

    for ( colour = 0; colour < numColours; colour++ )
    {
				/* Set up the moving output bit pointer for
				   each of the colours.	*/
	nextBit[ colour ].xByte	= colourBufs[ colour ] + LINE_PAD_BYTES
				+ ( lineIndex * lx5000dev->penLineLen );
	nextBit[ colour ].xBit	= BIT_TO_MASK( 0 );
				/* Clear the colourBuf line if necessary */
	if ( ! lineEmpty[ colour ][ lineIndex ] )
	{
	    memset( nextBit[ colour ].xByte - LINE_PAD_BYTES,
		    0, lx5000dev->penLineLen );
	    lineEmpty[ colour ][ lineIndex ] = true;
	}
    }
				/* Is the line empty? */
    if (( ! *scanLine ) &&
	! memcmp( scanLine, scanLine + 1, lx5000dev->scanLineBytes - 1 ))
	return;			/* N.B. empty flag is pre-set to true */

    while ( nextPixel.scanByte < scanEnd )
    {
	int	colour;
	int	cmy;		/* Used to detect if C+M+Y set */
	int	skipPixels, skipBytes, skipBits;
				/* Number of consecutive empty pixels, equiv-
				   alent bytes and remainder bits just skipped
				   over in the CMYK line being processed. */

				/* Get next pixel and increment the pointer */
	scanPixel =
	    ( ( *(nextPixel.scanByte) >> nextPixel.pixShift ) & PIXEL_MASK );

		/* pixShift is the number of bits to right-shift the scanByte
		   before applying PIXEL_MASK.  When we attempt to decrement
		   it past 0, it's time to look at the next scanByte.
		   N.B.  We assume that the subtraction of BPP will end with
		   zero, i.e., that there are an integral number of BPPs in
		   INITIAL_PIXEL_SHIFT.  We test for <= 0 just
		   in case this condition gets violated.
		*/
	skipPixels = 0;
	if (  nextPixel.pixShift <= 0 )
	{
	    nextPixel.scanByte++;
	    nextPixel.pixShift = INITIAL_PIXEL_SHIFT;
				/* If the next byte is empty, skip it */
	    while (( ! *nextPixel.scanByte ) && nextPixel.scanByte < scanEnd )
	    {
		nextPixel.scanByte++;
		skipPixels += PIXELS_PER_BYTE;
	    }
	}
	else
	    nextPixel.pixShift -= BPP;
				/* Get the black bit(s) */
	colourBits[ BLACK_X ]	= scanPixel & COLOUR_MASK;
	scanPixel		>>= BITS_PER_COLOUR;
				/* Set to catch all ANDed bits from colours */
	cmy			= ~0;
	for ( colour = YELLOW_X; colour <= CYAN_X; colour++ )
	{
	    colourBits[ colour ] = scanPixel & COLOUR_MASK;
	    cmy			&= scanPixel & COLOUR_MASK;
	    scanPixel		>>= BITS_PER_COLOUR;
	}
	if ( ( cmy ^ COLOUR_MASK ) == 0 ) /* C, M & Y all set == COLOUR_MASK */
	{
	    colourBits[ YELLOW_X ] =
		colourBits[ MAGENTA_X ] = colourBits[ CYAN_X ] = 0;
	    colourBits[ BLACK_X ] = COLOUR_MASK;
	}
				/* Now set the colourBuf bits */
	skipBytes = skipPixels >> 3;
	skipBits  = skipPixels & 7;
	for ( colour = 0; colour < numColours; colour++ )
	{
	    if ( colourBits[ colour ] )
	    {
		*( nextBit[ colour ].xByte )	|= nextBit[ colour ].xBit;
		lineEmpty[ colour ][ lineIndex ] = false;
	    }
	    INC_BIT( nextBit[ colour ] );
	    if ( skipPixels )
	    {
		int	bitSkip = skipBits;

		nextBit[ colour ].xByte += skipBytes;
		while ( bitSkip-- )
		{
		    INC_BIT( nextBit[ colour ] );
		}
	    }
	}
    }
}


/*----------------------------------------------------------------------*
 *	r e f r e s h B u f f e r ( )					*
 *----------------------------------------------------------------------*
 Given pointers to the values of the next line to retrieve from GS, and
 the next printable line, the GS line buffer, the per colour bit buffers,
 the per colour arrays containing the per line `empty'
 indicators, and the per pen penData structs, containing the next print
 line for each pen, refresh the buffer, and perform initial processing on
 the lines read from GS.

 When the scan line is for a black-only device, the line bits can be read
 directly into the appropriate colour buffer at the appropriate place.

 When the scan lines hold colour info, each line must undergo CMYK pro-
 cessing to derive the individual arrays of single-colour bits.

 Refreshing the buffers involves reading lines into the circular colour
 buffers until COLOUR_BUF_LINES have been read, or until the available
 lines have been exhausted.

 On return, nextLineToPrint must be set to the said line number, or beyond
 the end of the page, if no more remain to be printed.
 *----------------------------------------------------------------------*/
static void
refreshBuffer( lx5000_device *lx5000dev,
	       int *nextLineToGet, int *nextLineToPrint,
	       byte *lineBuffer, byte *colourBufs[],
	       bufBit
	       colourLines[NUM_COLOURS][PENS_PER_COLOUR][COLOUR_BUF_LINES],
	       penData pens[NUM_COLOURS][PENS_PER_COLOUR],
	       bool lineEmpty[NUM_COLOURS][COLOUR_BUF_LINES] )
{
    byte	*lineBuf;
    uchar	nextToGet;	/* Circular buffer pointer, 0 to ff */
    int		bufferOffset;
    int		colour, pen;
    int		_1stPrintable;	/* Across all pens */
    int		numColours = lx5000dev->color_info.num_components;

				/* Establish the next line to print, if it
				   is already in the buffer.		*/
    _1stPrintable		= pens[YELLOW_X][LO_PEN].finalLine;
    for ( colour = 0; colour < numColours; colour++ )
    {
	for ( pen = 0; pen < lx5000dev->pensPerColour; pen++ )
	    if ( pens[ colour ][ pen ].nextPrintLine < _1stPrintable)
		_1stPrintable = pens[ colour ][ pen ].nextPrintLine;
    }
    *nextLineToPrint = _1stPrintable;

    nextToGet = (uchar)(*nextLineToGet & COLOUR_BUF_MASK);
				/* nextLineToPrint may be set high (i.e. not
				   known), or may be a known print line. */
    while (  ( *nextLineToGet < *nextLineToPrint
	       || ( *nextLineToGet - *nextLineToPrint ) < COLOUR_BUF_LINES )
	     && *nextLineToGet < lx5000dev->height )
    {
	bufferOffset = LINE_PAD_BYTES + ( lx5000dev->penLineLen * nextToGet );

	if ( ! lx5000dev->isCMYK )
	{
				/* For black-only, read the bits directly into
				   the colour buffer. */
	    gdev_prn_get_bits( (gx_device_printer *)lx5000dev, *nextLineToGet,
			       colourBufs[BLACK_X] + bufferOffset, &lineBuf );
				/* If necessary, copy the bits into the actual
				   buffer.		*/
	    if ( lineBuf != colourBufs[BLACK_X] + bufferOffset )
		memcpy(colourBufs[BLACK_X] + bufferOffset,
		       lineBuf, lx5000dev->penLineBytes);
				/* Check for a printing line */
	    if ( *(colourBufs[BLACK_X] + bufferOffset) != 0
		 || memcmp( colourBufs[BLACK_X] + bufferOffset,
			    colourBufs[BLACK_X] + bufferOffset + 1,
			    lx5000dev->scanLineBytes - 1 ))
		lineEmpty[ BLACK_X ][  nextToGet ] = false;
	    else
		lineEmpty[ BLACK_X ][  nextToGet ] = true;
	}
	else			/* CMYK printing */
	{
				/* Process a CMYK line from GS */
	    gdev_prn_get_bits( (gx_device_printer *)lx5000dev,
			       *nextLineToGet, lineBuffer, &lineBuf );
	    processCMYKline( lx5000dev, *nextLineToGet, lineBuffer, lineBuf,
			     colourBufs, colourLines, pens, lineEmpty );
	}
	for (colour = 0; colour < numColours; colour++ )
	{
				/* Check for printing line -
				   N.B. For two-pen colour, this becomes more
				   complicated.  The relationship between the
				   first print line of the lower and of the
				   upper pen is determined by odd/even line
				   numbers, although this is not an essential
				   relationship.  Blank lines at the top of
				   the lower pen could be skipped, and the
				   first available line could be printed by
				   the lower pen. */
	    if ( ! lineEmpty[colour][nextToGet] )
	    {
		if ( pens[colour][LO_PEN].nextPrintLine > *nextLineToGet )
		    pens[colour][LO_PEN].nextPrintLine = *nextLineToGet;
		if ( *nextLineToPrint > *nextLineToGet )
		    *nextLineToPrint = *nextLineToGet;
	    }
	}
	++*nextLineToGet;
	nextToGet = ( *nextLineToGet & COLOUR_BUF_MASK ); /* N.B. This is safer
				   than just doing the increment, in case the
				   buffer length changes.		*/
    }
				/* Check that no buffer padding is necessary */
    if ( *nextLineToPrint < lx5000dev->height )
    {
	while ( ( *nextLineToGet - *nextLineToPrint ) < COLOUR_BUF_LINES )
	{			/* Last refresh fell short of COLOUR_BUF_LINES
				   because last line of page was reached.
				   Pad the buffer to COLOUR_BUF_LINES with 0
					   (no print) bytes.  */
	    for ( colour = 0; colour < numColours; colour++ )
	    {			/* Fill the colour line buffer, incl pad */
		memset( colourBufs[colour]
			+ ( lx5000dev->penLineLen * nextToGet ),
			0, lx5000dev->penLineLen );
				/* Set empty indicator - note that firstBit
				   is NOT set here. */
		lineEmpty[colour][nextToGet] = true;
	    }
	    ++*nextLineToGet;
	    nextToGet = ( *nextLineToGet & COLOUR_BUF_MASK );
	}
    }
}

/*----------------------------------------------------------------------*
 *	c a r t r i d g e M o v e T o ( )				*
 *----------------------------------------------------------------------*
  Given the array of penBits structures for each pen, containing the
  nextPrintLine value for the pen, return the line position to
  which the colour cartridge must be moved to print that line with the
  TOP nozzle of the required pen.
  If the print line for any pen is within the current range of that pen,
  a line less than the current colour head position will be returned.
  If colour is not defined, return a value off the end of the page.
 *----------------------------------------------------------------------*/
static int
cartridgeMoveTo( lx5000_device *lx5000dev,
		 penData pens[NUM_COLOURS][PENS_PER_COLOUR],
		 int _1stColour, int lastColour )
{
    if ( _1stColour != BLACK_X && ! lx5000dev->isCMYK )
				/* Send back an off-the-page value */
	return pens[CYAN_X][LO_PEN].finalLine;
    else
    {
	int	colour, pen, minLine;

	minLine	= pens[CYAN_X][LO_PEN].finalLine; /* Initialise high  */
				/* For each pen, calculate a moveto value
				   which will print the next line with the
				   first nozzle of that pen.
				   Return the minimum of these values. */
	for ( colour = _1stColour; colour <= lastColour; colour++ )
	    for ( pen = 0; pen < lx5000dev->pensPerColour; pen++ )
		if ( ( pens[colour][pen].nextPrintLine
		       + pens[colour][pen].topToBottomYellow )
		     < minLine )
		    minLine =
			pens[colour][pen].nextPrintLine
			+ pens[colour][pen].topToBottomYellow;

	return minLine;
    }
}
/*----------------------------------------------------------------------*
 *	s e t C o l o u r C o l u m n E x t e n t ( )			*
 *----------------------------------------------------------------------*
  Given a colour buffer, a single array of colourBits structures,
  1st and last lines, 1st nozzle, and the addresses of
  _1stColumn, lastColumn and columnExtent variables, calculate and set
  these column variables after stripping leading and trailing white space.
 *----------------------------------------------------------------------*/
static void
setColourColumnExtent( lx5000_device *lx5000dev, byte *colourBuf,
		       bool lineEmpty[COLOUR_BUF_LINES],
		       int _1stLine, int _1stNozzle, int lastLine,
		       int *_1stColumn, int *lastColumn, int *columnExtent )
{
    uchar	_1stIndex;
    int		firstPrintByte, lastPrintByte;
    int		line;

    line		= _1stLine;
    firstPrintByte	= lx5000dev->penLineBytes;
    lastPrintByte	= 0;

    for ( ; line <= lastLine; line += lx5000dev->lineIncrement )
    {				/* For each active line in swipe */
	int	columnByte;

	_1stIndex	= line & COLOUR_BUF_MASK;
	if ( lineEmpty[ _1stIndex ] )
	    continue;		/* If the line is empty, skip it. */
				/* Otherwise, scan from the beginning to any
				   previously found non-zero byte for the
				   beginning of the active column extent. */
	for ( columnByte = 0; columnByte < firstPrintByte; columnByte++ )
	    if ( colourBuf[ _1stIndex * lx5000dev->penLineLen
			  + LINE_PAD_BYTES + columnByte ] )
	    {
		firstPrintByte = columnByte;
		break;
	    }
				/* Scan from the end of the line to any pre-
				   viously found non-zero byte for the end of
				   the active column extent.		*/
	for ( columnByte = lx5000dev->penLineBytes;
	      columnByte > lastPrintByte; columnByte-- )
	    if ( colourBuf[ _1stIndex * lx5000dev->penLineLen
			  + LINE_PAD_BYTES + columnByte ] )
	    {
		lastPrintByte = columnByte;
		break;
	    }
    }
				/* If we have no extent, there are no
				   printable lines for this pen.	*/
    if ( firstPrintByte > lastPrintByte )
    {
	*_1stColumn	= firstPrintByte * 8;
	*lastColumn	= lastPrintByte * 8;
	*columnExtent	= 0;
	return;
    }

    *_1stColumn		= firstPrintByte * 8;
    *lastColumn		= lastPrintByte * 8 + 7 + lx5000dev->headSeparation;
    *columnExtent	= ( *lastColumn - *_1stColumn ) + 1;
    
}

/*----------------------------------------------------------------------*
 *	s e t C o l o u r L i n e s ( )					*
 *----------------------------------------------------------------------*
  Given a single colour/pen buffer, a single array of bufBit structures,
  1st and last lines, 1st nozzle, print direction, and
  _1stColumn, lastColumn and columnExtent variables, set up the pointers
  to the first actual column of each line, taking account of even and odd
  nozzle numbers.
 *----------------------------------------------------------------------*/
static void
setColourLines( lx5000_device *lx5000dev,
		byte *colourBuf, bufBit colourLines[COLOUR_BUF_LINES],
		int _1stLine, int _1stNozzle, int lastLine, int direction,
		int _1stColumn, int lastColumn )
{
    uchar	_1stIndex;
    int		line, nozzle, bit, headSep, headSepBytes, headSepBits;
    int		firstPrintByte, lastPrintByte, lastPrintBit;
    bufBit	startPtr;
    
    /* Now initialize the penBits bufBit structures to point to the
       beginning and end of the extent for each of the active lines in
       this swipe
       
       Adjust the beginning for all EVEN nozzles by moving it to the
       left by HeadSeparation bits.
    */

    headSep		= lx5000dev->headSeparation;
    if ( lx5000dev->x_pixels_per_inch == 300.0 )
	headSep		= headSep >> 1;	/* Halve the value at 300 dpi */
    if ( lx5000dev->x_pixels_per_inch == 1200.0 )
	headSep		= headSep << 1; /* Double the value at 1200dpi */
    headSepBytes	= headSep / 8;
    headSepBits		= headSep % 8;

    firstPrintByte	= _1stColumn / 8;
    lastPrintByte	= lastColumn / 8;
    lastPrintBit	= lastColumn % 8;
        
    if ( direction == RIGHTWARD )
    {
				/* Set up the ODD columns first */
	startPtr.xByte	= colourBuf + LINE_PAD_BYTES + firstPrintByte;
	startPtr.xBit	= BIT_TO_MASK( 0 );
	line		= _1stLine;
	nozzle		= _1stNozzle;	/* Is 1st nozzle effectively odd? */
	if ( ! ( nozzle & 1 ) )	/* Nozzle is even, so increment */
	    line += lx5000dev->lineIncrement;
	for ( ; line <= lastLine; line += ( lx5000dev->lineIncrement << 1 ) )
	{
	    _1stIndex	= (uchar)( line & COLOUR_BUF_MASK );
	    colourLines[_1stIndex].xByte	=
		startPtr.xByte + _1stIndex * lx5000dev->penLineLen;
	    colourLines[_1stIndex].xBit	= startPtr.xBit;
	}
				/* Set up EVEN columns */
	startPtr.xByte -= headSepBytes;
	for ( bit = 0; bit < headSepBits; bit++ )
	{
	    DEC_BIT( startPtr );
	}
	line	= _1stLine;
	nozzle	= _1stNozzle;	/* Is 1st nozzle effectively even? */
	if ( nozzle & 1 )	/* Nozzle is odd, so increment */
	    line += lx5000dev->lineIncrement;
	for ( ; line <= lastLine; line += ( lx5000dev->lineIncrement << 1 ) )
	{
	    _1stIndex	= (uchar)( line & COLOUR_BUF_MASK );
	    colourLines[_1stIndex].xByte	=
		startPtr.xByte + _1stIndex * lx5000dev->penLineLen;
	    colourLines[_1stIndex].xBit	= startPtr.xBit;
	}
    }
    else			/* direction == LEFTWARD */
    {
				/* Set up the ODD columns first */
	startPtr.xByte	= colourBuf + LINE_PAD_BYTES + lastPrintByte;
	startPtr.xBit	= BIT_TO_MASK( lastPrintBit );
	line		= _1stLine;
	nozzle		= _1stNozzle;	/* Is 1st nozzle effectively odd? */
	if ( ! ( nozzle & 1 ) )	/* Nozzle is even, so increment */
	    line += lx5000dev->lineIncrement;
	for ( ; line <= lastLine; line += ( lx5000dev->lineIncrement << 1 ) )
	{
	    _1stIndex	= (uchar)( line & COLOUR_BUF_MASK );
	    colourLines[_1stIndex].xByte	=
		startPtr.xByte + _1stIndex * lx5000dev->penLineLen;
	    colourLines[_1stIndex].xBit	= startPtr.xBit;
	}
				/* Set up EVEN columns */
	startPtr.xByte -= headSepBytes;
	for ( bit = 0; bit < headSepBits; bit++ )
	{
	    DEC_BIT( startPtr );
	}
	line	= _1stLine;
	nozzle	= _1stNozzle;	/* Is 1st nozzle effectively even? */
	if ( nozzle & 1 ) /* Nozzle is odd, so increment */
	    line += lx5000dev->lineIncrement;
	for ( ; line <= lastLine; line += ( lx5000dev->lineIncrement << 1 ) )
	{
	    _1stIndex	= (uchar)( line & COLOUR_BUF_MASK );
	    colourLines[_1stIndex].xByte	=
		startPtr.xByte + _1stIndex * lx5000dev->penLineLen;
	    colourLines[_1stIndex].xBit	= startPtr.xBit;
	}
    }
}

/*----------------------------------------------------------------------*
 *	p r i n t S w i p e ( )						*
 *----------------------------------------------------------------------*
 *----------------------------------------------------------------------*/
static void
printSwipe( lx5000_device *lx5000dev, byte *colourBufs[NUM_COLOURS],
	     bufBit
	     colourLines[NUM_COLOURS][PENS_PER_COLOUR][COLOUR_BUF_LINES],
	     penData pens[NUM_COLOURS][PENS_PER_COLOUR],
	     bool lineEmpty[NUM_COLOURS][COLOUR_BUF_LINES], byte *swipeBuf,
	     int *nextLineToPrint, int *nextLineToGet, int direction,
	     int _1stColour, int lastColour, FILE *prn_stream )
{
    /*--------------------------------------------------------------------*
      For unidirectional colour swathes, the effective print direction on
      a 5000 is reversed.  The implication of this is that, once the extent
      of the printing columns has been determined, the data for the colour
      pens must be fed to the head bit-reversed; i.e., set the bit pointer
      to the last column to be printed, and decrement the bit pointer until
      all of the columns have been printed.
     *--------------------------------------------------------------------*/

    static const char swipeHeader[SWIPE_HDR_LEN] = {
	SWIPE_LEADER,
	0, 0, 0, 0,			/* command length */
	0,				/* direction */
	0,				/* head speed */
	0, 0,			/* pen selector */
	0,				/* nozzle count */
	0,				/* don't know */
	0, 0,			/* number of columns */
	0, 0,			/* 1st column */
	0, 0,			/* last column */
	SWIPE_HDR_END
    };

    static const int	wordsPerPen[NUM_COLOURS][PENS_PER_COLOUR + 1] =
    {
	{ -1,
	  BLACK_NOZZLES / SWIPE_WORD_BITS,
	  BLACK_NOZZLES / SWIPE_WORD_BITS / 2
	},
	{ -1,
	  _1COLOUR_NOZZLES / SWIPE_WORD_BITS,
	  _1COLOUR_NOZZLES / SWIPE_WORD_BITS / 2
	},
	{ -1,
	  _1COLOUR_NOZZLES / SWIPE_WORD_BITS,
	  _1COLOUR_NOZZLES / SWIPE_WORD_BITS / 2
	},
	{ -1,
	  _1COLOUR_NOZZLES / SWIPE_WORD_BITS,
	  _1COLOUR_NOZZLES / SWIPE_WORD_BITS / 2
	}
    }; 

    int		_1stNozzle[NUM_COLOURS][PENS_PER_COLOUR];
				/* 1st active nozzle for pen */
    int		lastNozzle[NUM_COLOURS][PENS_PER_COLOUR];
				/* Last nozzle of this pen */
    int		_1stLine[NUM_COLOURS][PENS_PER_COLOUR];
				/* Line no. of first print line for pen */
    int		_1stPenColumn[NUM_COLOURS][PENS_PER_COLOUR];
    int		lastPenColumn[NUM_COLOURS][PENS_PER_COLOUR];
				/* Per-pen values... */
    int		penExtent[NUM_COLOURS][PENS_PER_COLOUR];
    int		_1stColumn;	/* ...and overall values of... */
    int		lastColumn;
    int		columnExtent;	/* Column range after */
				/* stripping leading and trailing white space*/
    int		column;		/* Loop variable - current column */
    int		line;		/* Loop variables - line corresponding to
				   current nozzle & temp line variable.  */
    uchar	lineIndex;	/* Circular index into pen buffer,
				   corresponding to current line. */
    int		swipeCmdLen;	/* Length of the swipe command */
    int		colour, pen;
    byte 	*outp;
    int		x_dpi		= (int)( lx5000dev->x_pixels_per_inch + 0.1 );

				/* Initialize worst case values for column
				   extremities.		*/
    _1stColumn	= lx5000dev->penLineBytes * 8
					+ 7 + lx5000dev->headSeparation;
    lastColumn	= 0;
				/* Set up extents on a per-colour basis. */
    for ( colour = _1stColour; colour <= lastColour; colour++ )
	for ( pen = 0; pen < lx5000dev->pensPerColour; pen++ )
	{
	    _1stLine[colour][pen]	= pens[colour][pen].nextPrintLine;
	    _1stNozzle[colour][pen]	= _1stLine[colour][pen]
						- pens[colour][pen].topLine;
	    lastNozzle[colour][pen]	= pens[colour][pen].bottomLine
	 					- pens[colour][pen].topLine;

	    setColourColumnExtent( lx5000dev, colourBufs[colour],
				   lineEmpty[colour], _1stLine[colour][pen],
				   _1stNozzle[colour][pen],
				   pens[colour][pen].bottomLine,
				   &_1stPenColumn[colour][pen],
				   &lastPenColumn[colour][pen],
				   &penExtent[colour][pen] );
				/* Get the overall extents for the swipe */
	    if ( _1stPenColumn[colour][pen] < _1stColumn )
		_1stColumn = _1stPenColumn[colour][pen];
	    if ( lastPenColumn[colour][pen] > lastColumn )
		lastColumn = lastPenColumn[colour][pen];
	}
				/* Set overall extent */
    columnExtent	= ( lastColumn - _1stColumn ) + 1;

				/* Set the column range for each of the colour
				   pens now that the full extent is known. */
    for ( colour = _1stColour; colour <= lastColour; colour++ )
	for ( pen = 0; pen < lx5000dev->pensPerColour; pen++ )
	{
	    setColourLines( lx5000dev, colourBufs[colour],
			    colourLines[colour][pen],
			    _1stLine[colour][pen], _1stNozzle[colour][pen],
			    pens[colour][pen].bottomLine, direction,
			    _1stColumn, lastColumn );
	}
    
    memcpy( swipeBuf, swipeHeader, SWIPE_HDR_LEN );
    outp	= swipeBuf + SWIPE_HDR_LEN;
				/* For each column of the output, build two
				   stripes.  One uses the standard directory
				   scheme, and the other uses the repeat
				   compression scheme.  Output the more
				   compact result. */
    for ( column = _1stColumn; column <= lastColumn; column++ )
    {
	ushort	dataword;	/* Loop transient - contents of swipe data
				   word currently under construction. */
	ushort	lastWord;	/* Last data word constructed for repeat
				   compression directory method.	*/
	int	nozzle;		/* Loop variable - current nozzle */
				/* The arrays of the two sets of data words
				   generated by each of the swipe construction
				   methods for a single swipe.  Does not
				   include the 16 bit directory word. */
	int	wordCount1;	/* The count of words in the column arrays, */
	int	wordCount2;	/* below.			*/
	ushort	column1[BLK_SWIPE_WORDS];
	ushort	column2[BLK_SWIPE_WORDS];
	int	columnWord;	/* Current word within column. */
	bufBit *lineBit;	/* Loop transient - current scan line bit */
	ushort	wordBit;	/* Current bit within swipe word - incremented
				   by right shifts. */
	ushort	directoryBit;	/* Current bit within directory word.  It
				   indicates the currently active set of 16
				   nozzles.  Incremented by left shifts. */
	ushort	directory1;	/* The directories for the two methods. Each */
	ushort	directory2;	/* bit controls one following 16bit word. */

	if ( _1stColour == BLACK_X )
	{
	    directory1	= (ushort)(BLK_DIRECTORY_MASK); /* empty */
	    directory2	= (ushort)(BLK_DIRECTORY_MASK); /* empty */
	}
	else
	{
	    directory1	= (ushort)(COLOUR_DIRECTORY_MASK); /* empty */
	    directory2	= (ushort)(COLOUR_DIRECTORY_MASK); /* empty */
	}
	directory1	|= DIRECTORY_TYPE_BIT; /* Normal directory */
	directory2	&= ~DIRECTORY_TYPE_BIT; /* Repeat compression
						   directory */
				/* LSBit of directory refers to 1st following
				   word */
	directoryBit	= 1;
	lastWord	= 0;
	wordCount1	= 0;
	wordCount2	= 0;
				/* Note that the order of the pens in the
				   datawords (top nozzle to bottom) is
				   CMY, the reverse of the numbering */
				/*------------------------------------------
				  N.B. What are the implications of this for
				  pen processing within a colour?
				  -----------------------------------------*/
	for ( colour = lastColour; colour >= _1stColour; colour-- )
	    for ( pen = 0; pen < lx5000dev->pensPerColour; pen++ )
	    {
		dataword	= 0;
		nozzle	= 0;
		line	= _1stLine[colour][pen];
				/* If there are any non-printing nozzles at
				   the top of the printhead, set up the entries
				   for them here.		*/
		while ( ( (  _1stNozzle[colour][pen] - nozzle )
			  >= SWIPE_WORD_BITS )
			&& ( nozzle <= lastNozzle[colour][pen] ) )
		{
		    if ( lastWord != 0 )
		    {
			directory2		&= ~directoryBit;
			column2[ wordCount2++ ]	=  dataword;
			lastWord		=  dataword;
		    }
		    nozzle		+= SWIPE_WORD_BITS;
		    directoryBit	<<= 1;
		}
				/* Shift over any remaining empty bits in
				   the current column word.  */
		wordBit = ( 1 << ( SWIPE_WORD_BITS - 1 ));
		wordBit >>= ( _1stNozzle[colour][pen] - nozzle );

		for ( columnWord = _1stNozzle[colour][pen] / SWIPE_WORD_BITS;
		      columnWord <
			  wordsPerPen[colour][lx5000dev->pensPerColour];
		      columnWord++ )
		{
		    while ( wordBit )
		    {
			lineIndex = (uchar)( line & COLOUR_BUF_MASK );
				/* Get the bit from the next line */
			lineBit   = &colourLines[colour][pen][lineIndex];
			if ( *(lineBit->xByte) & lineBit->xBit )
			    dataword |= wordBit;
			wordBit >>= 1;
			if ( direction == LEFTWARD )
			{
			    DEC_BIT( *lineBit );
			}
			else
			{
			    INC_BIT( *lineBit );
			}
			line += lx5000dev->lineIncrement;
		    }
				/* Do we have a non-null dataword? */
		    if ( dataword )/* For normal compression, every nonempty */
		    {		/* dataword is noted and sent to printer */
			directory1		&= ~directoryBit;
			column1[ wordCount1++ ]	=  dataword;
		    }
		    if ( dataword != lastWord ) /* For repeat compression, */
		    {		/* only datawords which change are recorded */
			directory2		&= ~directoryBit;
			column2[ wordCount2++ ]	=  dataword;
			lastWord		=  dataword;
		    }
				/* Look at next set of nozzles */
		    directoryBit <<= 1;
		    dataword	= 0;
				/* Reset dataword bit mask to 1st bit */
		    wordBit	= ( 1 << ( SWIPE_WORD_BITS - 1 ));
		}
	    }
				/* Output the smaller array of column words -
				   is it normal or repeat compressed? */
	if ( wordCount1 < wordCount2 )
	{			/* A normal directory + data */
	    int		i;

	    *outp++	= directory1 >> 8;
	    *outp++	= directory1 & 0xff;
	    for ( i = 0; i < wordCount1; i++ )
	    {
		*outp++	= column1[ i ] >> 8;
		*outp++	= column1[ i ] & 0xff;
	    }
	}
	else
	{			/* A repeat compression directory + data */
	    int		i;

	    *outp++	= directory2 >> 8;
	    *outp++	= directory2 & 0xff;
	    for ( i = 0; i < wordCount2; i++ )
	    {
		*outp++	= column2[ i ] >> 8;
		*outp++	= column2[ i ] & 0xff;
	    }
	}
    }
				/* Set up the header and output the swipe */
    swipeCmdLen	= outp - swipeBuf;
    {
	int	ps1, ps2, direction, density, nozzles;

	direction	= UNIDIRECTIONAL;
	if ( x_dpi == 300 )
	{
	    density	= _300X;
	}
	else
	{
	    if ( x_dpi == 1200 )
		density	= _1200X;
	    else
		density	= _600X;
	}

	if ( _1stColour == BLACK_X )
	{
	    ps1		= BLACK0;
	    ps2		= BLACK1;
	    nozzles	= _208NOZZLES;
	}
	else
	{
	    ps1		= COLOUR0;
	    ps2		= COLOUR1;
	    nozzles	= _192NOZZLES;
	}
	FILL_SWIPE_HEADER( swipeBuf, swipeCmdLen, direction, density,
			   ps1, ps2, nozzles, UNKNOWN1VAL,
			   columnExtent, _1stColumn, lastColumn );
    }
    fwrite( swipeBuf,1, outp - swipeBuf, prn_stream );

    for ( colour = _1stColour; colour <= lastColour; colour++ )
	for ( pen = 0; pen < lx5000dev->pensPerColour; pen++ )
	{
				/* Scan the buffer for the next print line,
				   if present.  If not, set sentinel value. */
	    if ( pens[colour][pen].nextPrintLine
		 <= pens[colour][pen].bottomLine
		 || pens[colour][pen].nextPrintLine >= *nextLineToGet )
	    {
		pens[colour][pen].nextPrintLine =
		    			pens[YELLOW_X][LO_PEN].finalLine;
		line = pens[colour][pen].bottomLine + 1;
		while ( line < *nextLineToGet )
		{
		    lineIndex	= (uchar)( line & COLOUR_BUF_MASK );
		    if ( ! lineEmpty[colour][ lineIndex ] )
		    {
			pens[colour][pen].nextPrintLine = line;
			break;
		    }
		    line += lx5000dev->lineIncrement;
		}
	    }
	}
}

/*----------------------------------------------------------------------*
 *	l x 5 0 0 0 _ p r i n t _ p a g e ( )				*
 *----------------------------------------------------------------------*
  Send the page to the printer.
 *----------------------------------------------------------------------*/
static int
lx5000_print_page( gx_device_printer *pdev, FILE *prn_stream )
{
    /*
     Data structures for the buffer:
     
     ------------------------------------------------------------------*
     			C O L O U R   B U F F E R S
     ------------------------------------------------------------------*
     The colour buffers contain the individual colour bits that will
     actually be passed to the print heads; therefore, there is one bit
     per printable element.  So far, the correspondence between the
     gx_color_index elements in the scan line and the individual print
     bits is 1-to-1.
     
     For the colour driver, the colour buffers will be:
     		Cbuffer, Mbuffer, Ybuffer and Kbuffer
     while for the black driver, only Kbuffer is used.
     (See BUFFER HANDLING MACROS.)
     
     ------------------------------------------------------------------*
     		D E T E R M I N I N G   B U F F E R   S I Z E
     ------------------------------------------------------------------*
     The colour buffers are circular buffers: in order to implement this
     characteristic, the size of the buffer must be 2^n.  The buffers may
     then be indexed by the scan line number, masked by 2^n - 1.
     
     The buffer must be able to accommodate the maximum range of scan
     lines that might be affected by a single swipe command.  For colour
     pens this is 240 ( 3 x 64 pens + 2 x 24 inter-pen gaps).  For the
     black pen, this is 208.  Therefore, the minimum buffer size is 256.
     In fact, this is probably also the optimal buffer size, as there seems
     to be no advantage in buffering twice the number of scan line required
     to hold any individual swipe.
     
     ------------------------------------------------------------------*
     	   S T R U C T U R E   O F   T H E   C O L O U R   B U F F E R S
     ------------------------------------------------------------------*
     The colour buffers hold the data for individual colour printheads,
     extracted from the original data provided as scan lines.  When the
     data for each colour is extracted from the scan line, it is placed
     into a line of the colour buffer corresponding to the scan line.
     
     Within each line of the colour buffer, the data is structured like so:
     +-------+------------------- ~ ---------------------+-------+
     |  Pad  |    Scan line data bits for one colour     |  Pad  |
     | bytes |                                           | bytes |
     +-------+-------------------------------------------+-------+
     
     When the scan line data is first processed, the colour data is copied
     into the colour buffer in the position indicated above.  This line of
     data is padded on both ends.  The size of the pad is sufficient to
     account for the number of bits of head separation between the EVEN
     and ODD rows of nozzles in each print head; by default 16.
     
     The individual lines are accessed through an array of colourLines struct-
     ures.  This is defined in the TYPEDEFS & EXTERNAL STORAGE secion above.
     
     The colourLines structure contains a byte pointer which points to one of
     the bytes of an individual colour buffer line, and an unsigned char
     which is a single bit mask isolating one bit of the byte pointed to by
     the other element.
     		typedef struct bufBit_s {
     			byte *	xByte;
     			uchar	xBit;
     		} bufBit;
     
     The colourLines structure holds one instance of a bufBit; it points to
     the first bit of the scan line.  In general, the bit pointers for the
     first bit of the EVEN columns will point to the beginning of the
     extracted scan line data.  For the ODD columns, the first bit
     will point into the leading PAD bytes by 16 bits before the start of
     the actual scan line data.
     
     In this way, all of the complications of managing the separation
     between the EVEN and ODD columns on the heads is contained in the
     set-up procedures for the pointers in the scanLines array.  If it is
     required to reverse the adjustments of EVEN and ODD columns (for bi-
     directional printing, for example) this is achieved by adjusting the
     firstBit entries in this array.
     
     That, at any rate, is the theory.  It seemed like a good idea at the
     time.
      
     */

    static const unsigned char init_string[] = {
	0xa5, 0, 6, 0x40, 3, 3, 0xc0, 0x0f, 0x0f,
	0xa5, 0, 3, 0x40, 4, 5,
	0xa5, 0, 3, 0x40, 4, 6,
	0xa5, 0, 3, 0x40, 4, 7,
	0xa5, 0, 3, 0x40, 4, 8,
	0xa5, 0, 4, 0x40, 0xe0, 0xb, 3,
	0xa5, 0, 11, 0x40, 0xe0, 0x41, 0, 0, 0, 0, 0, 0, 0, A5_11_5000,
	0xa5, 0, 6, 0x40, 5, 0, 0, 0x80, 0,
	0x1b, '*', 7, 0x73, 0x30,
	0x1b, '*', 'm', 0, 0x14, 3, 0x84, 2, 0, 1, 0xf4,
	0x1b, '*', 7, 0x63,
	0x1b, '*', 'm', 0, 0x42, 0, 0,
	0xa5, 0, 5, 0x40, 0xe0, 0x80, 8, 7
    };

    byte 	*lineBuffer;
    byte 	*swipeBuffer;
    byte 	*colourBufs[ NUM_COLOURS ];
    bufBit	colourLines[NUM_COLOURS][PENS_PER_COLOUR][COLOUR_BUF_LINES];
    bool	lineEmpty[ NUM_COLOURS ][ COLOUR_BUF_LINES ];
    penData	pens[ NUM_COLOURS ][ PENS_PER_COLOUR ];
				/* nextLineToPrint is the top unprinted line
				   in the buffers.  It is adjusted as lines
				   are processed and printed.  Its sentinel
				   value is pens[YELLOW_X][LO_PEN].finalLine.
				   When any line
				   which is not blank is read from GS &
				   processed, nextLineToPrint will be set to
				   this line if it is less than the current
				   value.

				   nextLineToGet is the next scanline to read
				   from GS.				*/
    int		nextLineToPrint;
    int		nextLineToGet	= 0;
				/* The current bottom line position of
					  the colour cartridge.		*/
    int		bottomYellowLine;
    int		retval;
     
    lx5000_device *lx5000dev	= (lx5000_device *)pdev;

    nextLineToPrint	= pens[YELLOW_X][LO_PEN].finalLine;
    bottomYellowLine	= INITIAL_YELLOW_BOTTOM_LINE;
				/* Allocate the buffer storage */
    if ( ( retval = getColourBufs( lx5000dev, &lineBuffer, colourBufs,
				   &swipeBuffer, ALLOCATE ) )) 
	return retval;

    initPenConstants( lx5000dev, pens );
			/* Initialize the lineEmpty[] and pens[] arrays -
			   should happen during parameter setup process */
    {
	int	colour, pen, line;

	for ( colour = 0;
	      colour < lx5000dev->color_info.num_components;
	      colour++ )
	{
	    for ( pen = 0; pen < lx5000dev->pensPerColour; pen++ )
	    {
		pens[colour][pen].nextPrintLine =
		    			pens[YELLOW_X][LO_PEN].finalLine;

		pens[colour][pen].bottomLine	=
					pens[colour][pen].initialBottomLine;
		pens[colour][pen].topLine	=
			bottomYellowLine
			- pens[colour][pen].topToBottomYellow;
	    }
	    for ( line = 0; line < COLOUR_BUF_LINES; line++ )
		/* Force the zeroing of the line first time through */
		lineEmpty[colour][line]	= false;
	}
    }

				/* Initialize the printer and reset margins. */
    pageInit( (uchar)lx5000dev->alignA, (uchar)lx5000dev->alignB, prn_stream );
    fwrite( init_string, 1, sizeof( init_string ), prn_stream );

	/*--------------------------------------------------------------*
	 Assume that COLOUR_BUF_LINES of data are available, starting with
	 nextLineToPrint.  Work out which cartridge will be doing the
	 printing, and how far it will have to move.

	 Something is available to be printed.  This may (should?) involve
	 initial paper movement to line up the appropriate pen.  Deciding
	 on when to move the paper and by how much is tricky.  One aim is
	 to minimize paper movements.

	 Check for black vs colour printing first.  If the required
	 movement for any of the colour pens is less than the required
	 movement for the black pen, print a colour swathe, adjust all of
	 the colour buffer values, and the nextLineToPrint value if
	 necessary.

	 Then refreshBuffer() and take another look.  Sooner or later,
	 the next required movement of the colour cartridge will exceed
	 the movement required of the black cartridge.  Print a black
	 swathe.  Whether to move the black cartridge for this print is
	 determined by checking on the subsequent colour cartridge
	 movement.

	 If the difference between the required movement for the black
	 and the required movement for the colour is less than or equal
	 to one colour pen depth, do not move the pen before printing the
	 black swathe.

	 Adjust black buffer values, including nextLineToPrint, which will
	 (almost?) certainly change after a black print.  refreshBuffer()
	 and try again.

	 A note on 192 vs 208 black swathes.  I can see the reasons for
	 using the 192 swathes.  When printing solid blocks with some of
	 each colour and black on each scan line, use of 192 nozzles
	 obviates the need for any black-related head movement.  A black
	 swathe can be printed on every third movement of the heads, and,
	 in the circumstances sketched above, will use all 192 nozzles.

	 Obviously, the same simple algorithm can be used when the colours
	 are less densely packed.  The Alignment B value, coded into the
	 header of every swipe command, automatically adjusts the first
	 pen of the set of 192 within a range of 0-15, so that the 192
	 nozzles are symmetrically arranged with respect to the colour
	 pens.  The complication is that the EVEN/ODD status of the
	 nozzles in the 208 set is unaffected, even though the numbering
	 may change.

	 It originally seemed to me that it was completely unnecessary
	 to use the 192 nozzle set.  The alignment value was needed to
	 get the positioning right, but for all other purposes, the
	 208 set could be used, and adjustments made on the fly.
	 However, I when considering the printing of pseudo 1200Y
	 pages, which involves the use of half-height pseudo-pens, it
	 became clear that the calculation of the half-height of a
	 pens would be greatly simplified by the use of 192 nozzles.
	 This would leave the height of a pseudo-pen at an integral
	 number of 16-bit datawords.

	 As a result, I will use 192 nozzles only when printing at
	 Nx1200Ydpi.

	 *--------------------------------------------------------------*/
    refreshBuffer( lx5000dev, &nextLineToGet, &nextLineToPrint,
		   lineBuffer, colourBufs, colourLines, pens, lineEmpty );
    while ( nextLineToPrint < pdev->height )
    {
	int	blackDirection	= RIGHTWARD;
	int	colourDirection	= LEFTWARD;
				/* Target of next move of colour cartridge */
	int	nextColourLine;
	int	nextBlack208Line = cartridgeMoveTo( lx5000dev, pens,
							  BLACK_X,
							  BLACK_X );

				/* In Black-only mode, cartridgeMoveTo
				   will return an off-page value for the
				   colour pens.			*/
	nextColourLine = cartridgeMoveTo( lx5000dev, pens,
						   _1ST_CMY_COLOUR,
						   LAST_CMY_COLOUR );
	if (  lx5000dev->isCMYK  && nextColourLine <= nextBlack208Line )
	{			/* Move the head & print a 3-colour swathe */
	    feedPaper( lx5000dev, nextColourLine, &bottomYellowLine, pens,
		       prn_stream );
	    printSwipe( lx5000dev, colourBufs, colourLines, pens,
			 lineEmpty, swipeBuffer,
			 &nextLineToPrint, &nextLineToGet, colourDirection,
			 _1ST_CMY_COLOUR, LAST_CMY_COLOUR,
			 prn_stream );
	}
	else
	{
	    feedPaper( lx5000dev, nextBlack208Line, &bottomYellowLine,
			   pens, prn_stream );
	    printSwipe( lx5000dev, colourBufs, colourLines, pens,
			lineEmpty, swipeBuffer,
			&nextLineToPrint, &nextLineToGet, blackDirection,
			BLACK_X, BLACK_X,
			prn_stream );
	}
	refreshBuffer( lx5000dev, &nextLineToGet, &nextLineToPrint,
		       lineBuffer, colourBufs, colourLines, pens, lineEmpty );
    } /* ends the loop for swipes of the print head.*/

				/* Eject the page, reinitialize the printer */
    pageEnd( prn_stream );
    fflush( prn_stream );
    getColourBufs( lx5000dev, &lineBuffer, colourBufs, &swipeBuffer,
		   DEALLOCATE );
    if ( lx5000dev->dryTime )
	sleep( lx5000dev->dryTime );

    return 0;
}

/* 
 * There are a number of parameters which can differ between ink cartridges. 
 * The Windows driver asks you to recalibrate every time you load a new
 * cartridge.
 *
 * [The Lexmark 5700 black
 * cartridge has two columns of dots, separated by about 16 pixels.
 * This `head separation' distance can vary between cartridges, so
 * we provide a parameter to set it.  In my small experience I've not
 * set the corresponding parameter in windows to anything greater than 17
 * or smaller than 15, but it would seem that it can vary from 1 to 32,
 * based on the calibration choices offered.
 *		Stephen Taylor's comment on the 5700 ]
 *
 * N.B.  The above discussion relates to the 5700.  On the 5000, in the
 *  Windows driver at any rate, there is no adjustment for the pixel
 *  distance between the rows of jets on the one print head.
 *
 * There are, however, two values which are always encoded into the initial
 *  escape sequence that the Windows driver sends to the printer for each
 *  new page: alignA the horizontal alignment between the colour and the
 *  black pens, and alignB, the vertical alignment between the same pens.
 *
 * AlignC is a black-only alignment, which seems to have something to do
 *  with bi-directional alignment.
 *
 * AlignD is a colour-only alignment, corresponding to AlignC.
 *
 * [As I understand the rules laid out in gsparams.h,
 * lx5000_get_params is supposed to return the current values of parameters
 * and lx5000_put_params is supposed to set up values in the lx5000_device
 * structure which can be used by the lx5000_print_page routine.
 * I've copied my routines from gdevcdj.c
 *		Stephen Taylor]
 *
 * DryingTime is a delay to allow page drying before a new page is fed
 * through.
 *
 * CMYK is a boolean parameter which specifies colour capability.  Set
 *  -dCMYK=false for black-only printing.
 *
 * See lx5000_put_params() for the legal ranges of values for these parameters.
 *
 */

/*----------------------------------------------------------------------*
 *	l x 5 0 0 0 b _ g e t _ p a r a m s ( )				*
 *----------------------------------------------------------------------*/
static int
lx5000_get_params( gx_device *pdev, gs_param_list *plist )
/*----------------------------------------------------------------------*/
{       
    lx5000_device *lx5000dev	= (lx5000_device *)pdev;
    int code			= gdev_prn_get_params( pdev, plist );

    if ( code < 0 ||
	 ( code =
	  param_write_int( plist, "HeadSeparation",
			  &lx5000dev->headSeparation )) < 0 ||
	 ( code = param_write_int(plist, "AlignA", &lx5000dev->alignA)) < 0 ||
	 ( code = param_write_int(plist, "AlignB", &lx5000dev->alignB)) < 0 ||
	 ( code = param_write_bool( plist, "CMYK", &lx5000dev->isCMYK )) < 0 ||
	 ( code = param_write_bool( plist,
				    "DryingTime", &lx5000dev->dryTime )) < 0
	 )
	return code;
           
    return code;
}

/*----------------------------------------------------------------------*
 *	l x 5 0 0 0 b _ p u t _ p a r a m _ i n t ( )			*
 *----------------------------------------------------------------------*/
static int
lx5000_put_param_int(
		      gs_param_list *plist, gs_param_name pname,
		      int *pvalue,  int minval, int maxval, int ecode
		      )
/*----------------------------------------------------------------------*/
{
    int code, value;
    
    switch ( code = param_read_int( plist, pname, &value ) )
    {
    default:
	return code;
    case 0:
	if ( value < minval || value > maxval )
	{
	    code =
		param_signal_error( plist, pname, gs_error_rangecheck );
	    ecode = ( code < 0 ? code : ecode );
	}
	else
	    *pvalue = value;
    case 1:
	return ecode;
    }
}

/*----------------------------------------------------------------------*
 *	l x 5 0 0 0 b _ p u t _ p a r a m _ b o o l ( )			*
 *----------------------------------------------------------------------*/
static int
lx5000_put_param_bool(
		      gs_param_list *plist, gs_param_name pname,
		      bool *pvalue,  int ecode
		      )
/*----------------------------------------------------------------------*/
{
    int 	code;

    switch ( code = param_read_bool( plist, pname, pvalue ) )
    {
    default:
	ecode = code;
	param_signal_error(plist, pname, ecode);
	return code;
    case 1:
    case 0:
	return ecode;
    }
}

/*----------------------------------------------------------------------*
 *	l x 5 0 0 0 b _ p u t _ p a r a m s ( )				*
 *----------------------------------------------------------------------*/
static int
lx5000_put_params( gx_device *pdev, gs_param_list *plist )
/*----------------------------------------------------------------------*/
/* put_params is supposed to check all the parameters before setting any. */
/*----------------------------------------------------------------------*/
{
    lx5000_device	*lx5000dev = (lx5000_device *)pdev;

    int 	ecode, code	= 0;
    int 	headSeparation	= lx5000dev->headSeparation;
    int 	alignA		= lx5000dev->alignA;
    int 	alignB		= lx5000dev->alignB;
    int		dryTime		= lx5000dev->dryTime;
    int		bitsPerPixel	= lx5000dev->isCMYK ? 4 : 1;
    int		oldBPP		= bitsPerPixel;
    bool	isCMYK		= lx5000dev->isCMYK;
    bool	isCMYK_old	= isCMYK;

    code = lx5000_put_param_int( plist, "HeadSeparation", &headSeparation,
				 8, 24, code ); /* 8 - 24 columns */

    code = lx5000_put_param_int( plist, "AlignA", &alignA, 0, 30, code );
				/* 0 - 30 columns */

    code = lx5000_put_param_int( plist, "AlignB", &alignB, 0, 15, code );
				/* 0 -15 nozzles */

    code = lx5000_put_param_int( plist, "DryingTime", &dryTime, 0, 60, code );
				/* 0 -60 seconds */

    code = lx5000_put_param_int
	( plist, "BitsPerPixel", &bitsPerPixel, 1, 4, code );
				/* Black cartridge only */

    code = lx5000_put_param_bool( plist, "CMYK", &isCMYK, code );
				/* Black cartridge only */

    /* Take precautions against input errors */
    if ( bitsPerPixel != 1 ) { bitsPerPixel = 4; }

    if ( isCMYK != isCMYK_old || bitsPerPixel != oldBPP )
    {
	bool	isCMYK_new;
	/* What has changed?  Assume that initial values were consistent,
	   and set isCMYK to a value consistent with the changed args
	   If both have changed, give precedence to isCMYK
	*/
	if ( bitsPerPixel != oldBPP )
	{
	    if ( bitsPerPixel == 1 )
	    { isCMYK_new = false; }
	    else
	    { isCMYK_new = true; };
	}
	if ( isCMYK != isCMYK_old )
	{ isCMYK_new = isCMYK; }

	isCMYK = isCMYK_new;
	bitsPerPixel = isCMYK ? 4 : 1;

	if ( isCMYK )
	{
	    lx5000dev->color_info = color_info_cmy;
	    dev_proc(pdev, map_cmyk_color) = lx5000_map_cmyk_color;
	    dev_proc(pdev, map_rgb_color) = NULL;
	    dev_proc(pdev, map_color_rgb) = lx5000_map_color_rgb;
	}
	else
	{
	    lx5000dev->color_info = color_info_blk;
	    dev_proc(pdev, map_cmyk_color) = NULL;
	    dev_proc(pdev, map_rgb_color) = gdev_prn_map_rgb_color;
	    dev_proc(pdev, map_color_rgb) = gdev_prn_map_color_rgb;
	}
	if ( pdev->is_open )
	    gs_closedevice(pdev);
    }

				/* call super class put_params */
    ecode = gdev_prn_put_params( pdev, plist );

    if ( ecode < 0 || code < 0 )
    {
	if ( isCMYK != isCMYK_old )
	{
	    if ( ! isCMYK )
	    {
		lx5000dev->color_info = color_info_cmy;
		dev_proc(pdev, map_cmyk_color) = lx5000_map_cmyk_color;
		dev_proc(pdev, map_rgb_color) = NULL;
		dev_proc(pdev, map_color_rgb) = lx5000_map_color_rgb;
	    }
	    else
	    {
		lx5000dev->color_info = color_info_blk;
		dev_proc(pdev, map_cmyk_color) = NULL;
		dev_proc(pdev, map_rgb_color) = gdev_prn_map_rgb_color;
		dev_proc(pdev, map_color_rgb) = gdev_prn_map_color_rgb;
	    }
	    if ( pdev->is_open )
		gs_closedevice(pdev);
	}
	return ecode < 0 ? ecode : code;
    }

				/* looks like everything okay; */
				/* go ahead and set parameters */
    lx5000dev->headSeparation = headSeparation;
    lx5000dev->alignA = alignA;
    lx5000dev->alignB = alignB;
    lx5000dev->dryTime = dryTime;
    lx5000dev->isCMYK = isCMYK;
				/* N.B. I am only setting these values here -
				   should they also be set as part of the "two
				   phase commit" of parameter changes?  My
				   code is the only place these things are
				   accessed.	*/
    if ( lx5000dev->y_pixels_per_inch == MAX_LX5000_Y )
    {
	lx5000dev->pensPerColour	= 2;
	lx5000dev->lineIncrement	= 2;
    }
    else
    {
	lx5000dev->pensPerColour	= 1;
	lx5000dev->lineIncrement	= 1;
    }
    if ( code == 1 ) return ecode; 
    return 0;
}


/*----------------------------------------------------------------------*
 * The following colour handling procedures are lifted from gdevbit.c
 *----------------------------------------------------------------------*/
/* Map color to RGB.  This has 3 separate cases, but since it is rarely */
/* used, we do a case test rather than providing 3 separate routines. */
static int
lx5000_map_color_rgb(gx_device * dev, gx_color_index color,
		     gx_color_value rgb[3])
{
    int depth = dev->color_info.depth;
    int ncomp = dev->color_info.num_components;
    int bpc = depth / ncomp;
    uint mask = (1 << bpc) - 1;

#define cvalue(c) ((gx_color_value)((ulong)(c) * gx_max_color_value / mask))

    /* Map CMYK back to RGB. */

    gx_color_index cshift = color;
    uint c, m, y, k;

    k = cshift & mask;
    cshift >>= bpc;
    y = cshift & mask;
    cshift >>= bpc;
    m = cshift & mask;
    c = cshift >> bpc;
    /* We use our improved conversion rule.... */
    rgb[0] = cvalue((mask - c) * (mask - k) / mask);
    rgb[1] = cvalue((mask - m) * (mask - k) / mask);
    rgb[2] = cvalue((mask - y) * (mask - k) / mask);

    return 0;
#undef cvalue
}

/* Map CMYK to color. */
static gx_color_index
lx5000_map_cmyk_color(gx_device * dev, const gx_color_value cv[])
{
    int bpc = dev->color_info.depth / 4;
    int drop = sizeof(gx_color_value) * 8 - bpc;
    gx_color_value cyan = cv[0], magenta = cv[1], yellow = cv[2], black = cv[3];
    gx_color_index color =
    ((((((cyan >> drop) << bpc) +
	(magenta >> drop)) << bpc) +
      (yellow >> drop)) << bpc) +
    (black >> drop);

    return (color == gx_no_color_index ? color ^ 1 : color);
}

/*=============== Clean up my #define's =================*/
#undef A5_11_5000
#undef A5_11_5700
#undef ALIGN_A_DEF
#undef ALIGN_A_OFFSET
#undef ALIGN_B_DEF
#undef ALIGN_B_OFFSET
#undef HEADSEP_DEF
#undef DRY_TIME_DEF
#undef LX_UNI
#undef MIN_LX5000_X
#undef MAX_LX5000_X
#undef DEF_LX5000_X
#undef MIN_LX5000_Y
#undef MAX_LX5000_Y
#undef DEF_LX5000_Y
#undef LX5000_XDPI
#undef LX5000_YDPI
#undef FEED_FACTOR
#undef LX5000_XOFFSET_TO_0_0_XDPI
#undef LX5000_YOFFSET_TO_0_0_YDPI
#undef LX5000_XOFFSET_TO_0_0
#undef LX5000_YOFFSET_TO_0_0
#undef LX5000_LEFT_HWMARGIN_INS
#undef LX5000_BOTTOM_HWMARGIN_INS
#undef LX5000_RIGHT_HWMARGIN_INS
#undef LX5000_TOP_HWMARGIN_INS
#undef LINE_PAD_BYTES
#undef RIGHTWARD
#undef LEFTWARD
#undef SWIPE_WORD_BITS
#undef BLACK_NOZZLES
#undef _1COLOUR_NOZZLES
#undef BLK_COLOUR_NOZZLES
#undef BLK_SWIPE_WORDS
#undef COLOUR_SWIPE_WORDS
#undef _1COLOUR_WORDS
#undef BLK_DIRECTORY_MASK
#undef COLOUR_DIRECTORY_MASK
#undef DIRECTORY_TYPE_BIT
#undef COLOUR_PEN_GAP
#undef COLOUR_PEN_DIFF
#undef INITIAL_YELLOW_BOTTOM_LINE
#undef COLOUR_BUF_MASK
#undef COLOUR_BUF_LINES
#undef SWIPE_LEADER
#undef CMDLEN_X
#undef DIRECTION_X
#undef UNIDIRECTIONAL
#undef BIDIRECTIONAL
#undef HEADSPEED_X
#undef _300X
#undef _600X
#undef _1200Y
#undef PEN_X
#undef BLACK0
#undef BLACK1
#undef COLOUR0
#undef COLOUR1
#undef NOZZLE_COUNT_X
#undef _192NOZZLES
#undef _208NOZZLES
#undef UNKNOWN1_X
#undef UNKNOWN1VAL
#undef NUM_COLUMNS_X
#undef _1ST_COLUMN_X
#undef LAST_COLUMN_X
#undef SWIPE_HDR_END_X
#undef SWIPE_HDR_END_LEN
#undef SWIPE_HDR_END
#undef SWIPE_HDR_LEN
#undef BLACK_X
#undef YELLOW_X
#undef MAGENTA_X
#undef CYAN_X
#undef LO_PEN
#undef HI_PEN
#undef NUM_COMPONENTS_BLK
#undef BITS_PER_PIXEL_BLK
#undef MAX_GREY_BLK
#undef MAX_RGB_BLK
#undef DITHER_GREYS_BLK
#undef DITHER_COLOURS_BLK
#undef NUM_COMPONENTS_CMY
#undef BITS_PER_PIXEL_CMY
#undef MAX_GREY_CMY
#undef MAX_RGB_CMY
#undef DITHER_GREYS_CMY
#undef DITHER_COLOURS_CMY
#undef MIN_COLOUR
#undef MAX_COLOUR
#undef BLACK_PEN
#undef _1ST_CMY_COLOUR
#undef LAST_CMY_COLOUR
#undef BPP
#undef PIXEL_MASK
#undef INITIAL_PIXEL_SHIFT
#undef NUM_COLOURS
#undef PENS_PER_COLOUR
#undef NUM_PENS
#undef BITS_PER_COLOUR
#undef DEF_NUM_COLOURS
#undef DEF_PENS_PER_COLOUR
#undef DEF_LINE_INCREMENT
#undef PIXELS_PER_BYTE
#undef COLOUR_MASK
#undef ALLOCATE
#undef DEALLOCATE
#undef FILL_SWIPE_HEADER
#undef BIT_TO_MASK
#undef INC_BIT
#undef DEC_BIT
#undef lx5000_proctab
