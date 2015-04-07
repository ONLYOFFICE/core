/////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Appendix functions

struct ImageFormat
{
	int picture_type;			// picture type
	int picture_subtype;		// clipboard image type

	// common variables
	long colors;
	long sizeX;
	long sizeY;
	long format;				// format details (Win-OS/2 for Bmp)
	int out_color_space;		// colorspace for output (1=gray, >1=color)
	int out_bit_depth;			// colordepth for output (<=8, paletted)
	unsigned int scale_num;		// always=1
	unsigned int scale_denom;	// fraction by which to scale image
	BOOL bUseFSdither;			// for paletted image sabings

	// internal common variables
	UCHAR* image_ptr;
	BOOL PropDefaults;
	BOOL PreviewAllowed;
	long DesiredSizeX;
	long DesiredSizeY;
	int OEMcompression;			// Tiff compression type
	DWORD CustData;				// temporary data

	// unused or unnecessary
  	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;

	// JPEG, PNG variables
	int quality;				// JPEG(0..100), PNG(0..9=>0..100)
	BOOL simple_progressive;

	// PNG variables
	int filter;					// 0 - default, 1..5

	// GIF variables
	int framenum;		// in multiimage GIFs: 0=count of images
	int frameidx;		// 0=all images at once (screen size), >0: index of image
	
	// BMP, TIFF variables
  	int compression;	// 0-based index of compression type

	// TIFF variables
	int byteorder;		// (0=IBM-PC(default), 1-Mac, 2-host)
	int planarconfig;	// contiguous(default)/separate
	BOOL tiled;			// stripped(default)/tiled image
	int stripsize;		// size of strip buffer (index of control)
	int tilesize;		// size of tile (0-based index in combo)
};

typedef struct ImageFormat* LPIMAGEFORMAT;

HRESULT GetJp2Format(LPCTSTR fileName, LPIMAGEFORMAT pFmt);
HRESULT LoadJp2(LPCTSTR fileName, LPSAFEARRAY *saImage, LPIMAGEFORMAT pFmt = NULL);
HRESULT SaveJp2(LPCTSTR fileName, LPSAFEARRAY *saImage, LPIMAGEFORMAT pFmt = NULL);

/////////////////////////////////////////////////////////////////////////////
