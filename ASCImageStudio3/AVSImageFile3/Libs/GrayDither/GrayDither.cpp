// GrayDither.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "..\GrayDither.h"

#define gray_quant_fsdither1 BMPgray_quant_fsdither1

#define grayHistAlloc		BMPgrayHistAlloc
#define grayHistDealloc		BMPgrayHistDealloc
#define grayDitherUnprepare	BMPgrayDitherUnprepare
#define grayDitherPrepare	BMPgrayDitherPrepare
#define gray_quant_fsdither1 BMPgray_quant_fsdither1

extern ULONG** grayHistAlloc(UCHAR* imgMap);
extern void grayHistDealloc(ULONG** p);
extern int grayDitherPrepare(ULONG* raster, ULONG width, ULONG height, ULONG tilelen = 0, int bps = 0, BOOL dither = FALSE, BOOL paletted = FALSE, int numcolors = 0);
extern void grayDitherUnprepare();
extern int gray_quant_fsdither1(ULONG* inbuf, ULONG step, ULONG w, ULONG h, ULONG x, LONG fromskew, LONG toskew, UCHAR* outbuf);

/*
BOOL Dither32Image(BYTE* data, int width, int height)
{
	// compute compressed image size
	int compressedSize = (width + 7)/8;

	// allocate memory for compressed image
	BYTE* compressed = new BYTE[compressedSize];

	// variables
	int x, y;
	ULONG* pixels = (ULONG*)data;

	// dither image by line
    for (y = 0; y < height; ++y)
	{
		// quantize data
		if (gray_quant_fsdither1(pixels + width*y, 1, width, 1, 0, 0, 0, compressed) == 0)
			return FALSE;

		// decompress bit-image to 32bpp image
	//	for (x = 0; x < width; ++x)
	//		*(pixels + x) = (compressed[x/8] & 1 << (x % 8))? 0xFFFFFF : 0;
	}
 
	// quantize by whole image
	// gray_quant_fsdither1((ULONG*)(bufin+4*cx*i), 1, cx, cy, 0, 0, 0, bufout);

	// all ok
	return TRUE;
}
*/

BOOL Dither32Image(BYTE* data, int width, int height)
{
	BYTE * bufin, * bufout;
	int cx, cy, i, j;

	bufin = data;
	cx = width;
	cy = height;

	int m_out_bit_depth = 1;	// for B/W (1 bit / pixel)
	int wout = (m_out_bit_depth * cx + 7)/8;

	//	BOOL m_bGrayscale = TRUE;
	BOOL m_bUseFSdither = TRUE;

	int* histogram = NULL;
	ULONG** histp;
	long step = 1;	// параметр разрежения для Icons (1,2,3...)

	static RGBQUAD palette[256];
	ZeroMemory(palette, sizeof(palette));
	int ncols = 0;
	// подготовка палитры (R,G,B массивы)
	if (m_out_bit_depth <= 8) {	// building palette
//		if (m_bGrayscale) {
			ncols = 1<<m_out_bit_depth;
			histp = grayHistAlloc(NULL);
			if (histp == NULL) throw (-1);
			ncols = grayDitherPrepare((ULONG*)bufin, cx, cy,
				0, m_out_bit_depth, m_bUseFSdither, FALSE, ncols);
			if (ncols == 0) throw (-1);
			USHORT *rm;
			rm = (USHORT*)histp[0];
			histogram = (int*)histp[1];
		// здесь индексы 8битовые, хотя и записаны в 16 бит
			for (int icol = 0; icol < ncols; icol++)
				palette[icol].rgbRed = palette[icol].rgbGreen =
				palette[icol].rgbBlue = (UCHAR)rm[icol];
//			}

	}

//	if (m_bGrayscale) {	? для серых 16,24,32 бит
//		ColorToGrayscale(bpMem, cx, cy);
//		}


// вариант - записи по одной строке
bufout = (BYTE*)malloc(wout);	//рабочий упакованный

	for (i=0; i < cy; i++) {
		if (gray_quant_fsdither1(
				(ULONG*)bufin+cx*i, 1,
				cx, 1, 0, 0, 0, bufout) == 0) throw (-1);
	//		fwrite(bufout, wout, 1, file);
		// распаковка в исходный

		for (j=0; j<cx; j++)
			*((ULONG*)bufin+(cx*i+j)) = (bufout[j/8] & 1<<(j%8))? 0xFFFFFF : 0;
		}
free(bufout);

// другой вариант - всего массива сразу
/*
	if (gray_quant_fsdither1(
			(ULONG*)(bufin+4*cx*i), 1,
			cx, cy, 0, 0, 0, bufout) == 0) throw (-1);
	fwrite(bufout, wout*cy, 1, file);
*/




	if (m_out_bit_depth <= 8)
	{
		grayDitherUnprepare();
		grayHistDealloc(histp);
	}

	return TRUE;
}
