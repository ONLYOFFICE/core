/*!	\file	dcrawcommon.h
 *	\brief	ќбщий заголовок дл€ файлов dcraw
 *
 */

#pragma once

#define M_PI       3.14159265358979323846

#define _(String) (String)
#define N_(String) (String)

#define FORC(cnt) for (c=0; c < cnt; c++)
#define FORC3 FORC(3)
#define FORC4 FORC(4)
#define FORCC FORC(colors)


#define DCRAW_SQR(x) ((x)*(x))
//#ifndef DCRAW_NOMAIN
#define DCRAW_ABS(x) (((int)(x) ^ ((int)(x) >> 31)) - ((int)(x) >> 31))
#define DCRAW_MIN(a,b) ((a) < (b) ? (a) : (b))
#define DCRAW_MAX(a,b) ((a) > (b) ? (a) : (b))
//#endif
#define DCRAW_LIM(x,min,max) DCRAW_MAX(min,DCRAW_MIN(x,max))
#define DCRAW_ULIM(x,y,z) ((y) < (z) ? DCRAW_LIM(x,y,z) : DCRAW_LIM(x,z,y))
#define DCRAW_CLIP(x) DCRAW_LIM(x,0,65535)
#define DCRAW_SWAP(a,b) do { a ^= b; a ^= (b ^= a); } while (0);

//#define SQR(x) ((x)*(x))
//#define LIM(x,min,max) DCRAW_MAX(min,DCRAW_MIN(x,max))
//#define ULIM(x,y,z) ((y) < (z) ? LIM(x,y,z) : LIM(x,z,y))
//#define CLIP(x) LIM(x,0,65535)
//#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }


typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef __int64				INT64;
typedef unsigned __int64	UINT64;



#include "glibfun.h"




#define ctime_r(timestamp,size,buffer)	ctime_s((buffer),(size),(timestamp))




//#define ushort UshORt



/*! \section FILTERDESC	Filters Description
 *	\verbatim
  In order to inline this calculation, I make the risky
  assumption that all filter patterns can be described
  by a repeating pattern of eight rows and two columns
 
  Do not use the FC or BAYER macros with the Leaf CatchLight,
  because its pattern is 16x16, not 2x8.
 
  Return values are either 0/1/2/3 = G/M/C/Y or 0/1/2/3 = R/G1/B/G2 
 
   PowerShot 600   PowerShot A50   PowerShot       Pro70 Pro90 & G1	
 	0xe1e4e1e4:     0x1b4e4b1e:     0x1e4b4e1b:     0xb4b4b4b4:			
 
     0 1 2 3 4 5     0 1 2 3 4 5     0 1 2 3 4 5     0 1 2 3 4 5		
   0 G M G M G M   0 C Y C Y C Y   0 Y C Y C Y C   0 G M G M G M		
   1 C Y C Y C Y   1 M G M G M G   1 M G M G M G   1 Y C Y C Y C		
   2 M G M G M G   2 Y C Y C Y C   2 C Y C Y C Y						
   3 C Y C Y C Y   3 G M G M G M   3 G M G M G M						
                   4 C Y C Y C Y   4 Y C Y C Y C						
   PowerShot A5    5 G M G M G M   5 G M G M G M						
   0x1e4e1e4e:     6 Y C Y C Y C   6 C Y C Y C Y						
                   7 M G M G M G   7 M G M G M G						
     0 1 2 3 4 5														
   0 C Y C Y C Y														
   1 G M G M G M														
   2 C Y C Y C Y														
   3 M G M G M G														
 
  All RGB cameras use one of these Bayer grids:						
 
   0x16161616:     0x61616161:     0x49494949:     0x94949494:			
 																		
     0 1 2 3 4 5     0 1 2 3 4 5     0 1 2 3 4 5     0 1 2 3 4 5		
   0 B G B G B G   0 G R G R G R   0 G B G B G B   0 R G R G R G		
   1 G R G R G R   1 B G B G B G   1 R G R G R G   1 G B G B G B		
   2 B G B G B G   2 G R G R G R   2 G B G B G B   2 R G R G R G		
   3 G R G R G R   3 B G B G B G   3 R G R G R G   3 G B G B G B		
 \endverbatim
 *
 */

/*!
 *	\brief √енерирует маску согласно значению filters
 *	
 *	например,
 *	дл€ значени€ filters = 0x16161616
 *	маска имеет следующий вид (см. дл€ сравнени€ таблицы выше)
 *
 *	2 1 2 1 ... \n
 *	1 0 1 0 ... \n
 *	2 1 2 1 ... \n
 *	1 0 1 0 ... \n
 *	........... \n
 */
#define FC(row,col)		(filters >> ((((row) << 1 & 14) + ((col) & 1)) << 1) & 3)



