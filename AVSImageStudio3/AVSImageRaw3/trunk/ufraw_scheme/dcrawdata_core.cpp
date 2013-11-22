/*!	\file	dcrawdata_core.cpp
 *	\brief	–абота с dcraw и dcraw_data
 *
 *	\details	
 */
#include "stdafx.h"
#include "dcrawdata_core.h"
//#include "thumbnail.h"



extern const double xyz_rgb[3][3];
extern const float d65_white[3];


#define FC(row,col) \
	(int)(filters >> ((((row) << 1 & 14) + ((col) & 1)) << 1) & 3)

#define BAYER(row,col) \
	image[((row) >> shrink)*iwidth + ((col) >> shrink)][FC(row,col)]


int fc_INDI (const unsigned filters, const int row, const int col)
{
	static const char filter[16][16] =
	{ { 2,1,1,3,2,3,2,0,3,2,3,0,1,2,1,0 },
	{ 0,3,0,2,0,1,3,1,0,1,1,2,0,3,3,2 },
	{ 2,3,3,2,3,1,1,3,3,1,2,1,2,0,0,3 },
	{ 0,1,0,1,0,2,0,2,2,0,3,0,1,3,2,1 },
	{ 3,1,1,2,0,1,0,2,1,3,1,3,0,1,3,0 },
	{ 2,0,0,3,3,2,3,1,2,0,2,0,3,2,2,1 },
	{ 2,3,3,1,2,1,2,1,2,1,1,2,3,0,0,1 },
	{ 1,0,0,2,3,0,0,3,0,3,0,3,2,1,2,3 },
	{ 2,3,3,1,1,2,1,0,3,2,3,0,2,3,1,3 },
	{ 1,0,2,0,3,0,3,2,0,1,1,2,0,1,0,2 },
	{ 0,1,1,3,3,2,2,1,1,3,3,0,2,1,3,2 },
	{ 2,3,2,0,0,1,3,0,2,0,1,2,3,0,1,0 },
	{ 1,3,1,2,3,2,3,2,0,2,0,1,1,0,3,0 },
	{ 0,2,0,3,1,0,0,1,1,3,3,2,3,2,2,1 },
	{ 2,1,3,2,3,1,2,1,0,3,0,2,0,2,0,2 },
	{ 0,3,1,0,0,2,0,3,2,1,3,1,1,3,1,3 } };

	if (filters != 1)
		return FC(row,col);

	/* Assume that we are handling the Leaf CatchLight with
	* top_margin = 8; left_margin = 18; 
	*/
	//  return filter[(row+top_margin) & 15][(col+left_margin) & 15];
	return filter[(row+8) & 15][(col+18) & 15];
}

/*static void CLASS merror (void *ptr, char *where_)
{
//	if (ptr) return;
	_ASSERT(ptr!=NULL);
//	g_error("Out of memory in %s\n", where);
	//dcraw_message (DCRAW_ERROR,_("Out of memory in %s\n"), where_);
}*/

static void hat_transform (float *temp, float *base, int st, int size, int sc)
{
	int i;
	for (i=0; i < sc; i++)
		temp[i] = 2*base[st*i] + base[st*(sc-i)] + base[st*(i+sc)];
	for (; i+sc < size; i++)
		temp[i] = 2*base[st*i] + base[st*(i-sc)] + base[st*(i+sc)];
	for (; i < size; i++)
		temp[i] = 2*base[st*i] + base[st*(i-sc)] + base[st*(2*size-2-(i+sc))];
}

HRESULT wavelet_denoise_INDI(ushort (*image)[4], const int black,
								const int iheight, const int iwidth, const int height, const int width,
								const int colors, const int shrink, float pre_mul[4],
								const float threshold, const unsigned filters)
{
	_RAW_TRACE(OLESTR("[wavelet_denoise_INDI]\n"));
	if (!image)
		return E_POINTER;

	float *fimg, *temp, thold, mul[2], avg, diff;
	int /*scale=1,*/ size, lev, hpass, lpass, row, col, nc, c, i, wlast;
	ushort *window[4];
	static const float noise[] = { 0.8002,0.2735,0.1202,0.0585,0.0291,0.0152,0.0080,0.0044 };

	//(void)dcraw;

	//  dcraw_message (dcraw, DCRAW_VERBOSE,_("Wavelet denoising...\n")); /*UF*/

	/* Scaling is done somewhere else - NKBJ*/
#if 0
	while (maximum << scale < 0x10000) scale++;
	maximum <<= --scale;
	black <<= scale;
#endif

	size = iheight*iwidth;
	fimg = (float *) malloc ((size*3 + iheight + iwidth) * sizeof *fimg);
	
	if (!fimg)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч cannot allocate mem for fim\n"));
		return	E_OUTOFMEMORY;		
	}
	

	temp = fimg + size*3;
	if ((nc = colors) == 3 && filters) nc++;
	for (c=0; c < nc; c++)
	{      /* denoise R,G1,B,G3 individually */
		for (i=0; i < size; i++)
			fimg[i] = sqrt((float)((unsigned) (image[i][c] << (/*scale+*/16))));
		for (hpass=lev=0; lev < 5; lev++)
		{
			lpass = size*((lev & 1)+1);
			for (row=0; row < iheight; row++)
			{
				hat_transform (temp, fimg+hpass+row*iwidth, 1, iwidth, 1 << lev);
				for (col=0; col < iwidth; col++)
					fimg[lpass + row*iwidth + col] = temp[col] * 0.25;
			}
			for (col=0; col < iwidth; col++)
			{
				hat_transform (temp, fimg+lpass+col, iwidth, iheight, 1 << lev);
				for (row=0; row < iheight; row++)
					fimg[lpass + row*iwidth + col] = temp[row] * 0.25;
			}
			thold = threshold * noise[lev];
			for (i=0; i < size; i++)
			{
				fimg[hpass+i] -= fimg[lpass+i];
				if (fimg[hpass+i] < -thold)
					fimg[hpass+i] += thold;
				else if (fimg[hpass+i] >  thold)
					fimg[hpass+i] -= thold;
				else
					fimg[hpass+i] = 0;

				if (hpass)
					fimg[i] += fimg[hpass+i];
			}
			hpass = lpass;
		}

		for (i=0; i < size; i++)
			image[i][c] = DCRAW_CLIP((ushort)(DCRAW_SQR(fimg[i]+fimg[lpass+i])/0x10000));
	}

	if (filters && colors == 3) 
	{  /* pull G1 and G3 closer together */
		for (row=0; row < 2; row++)
			mul[row] = 0.125 * pre_mul[FC(row+1,0) | 1] / pre_mul[FC(row,0) | 1];

		for (i=0; i < 4; i++)
			window[i] = (ushort *) fimg + width*i;

		for (wlast=-1, row=1; row < height-1; row++)
		{
			while (wlast < row+1)
			{
				for (wlast++, i=0; i < 4; i++)
					window[(i+3) & 3] = window[i];

				for (col = FC(wlast,1) & 1; col < width; col+=2)
					window[2][col] = BAYER(wlast,col);
			}
			thold = threshold/512;
			for (col = (FC(row,0) & 1)+1; col < width-1; col+=2)
			{
				avg = ( window[0][col-1] + window[0][col+1] + window[2][col-1] + window[2][col+1] - black*4 ) * mul[row & 1] 
				+ (window[1][col] - black) * 0.5 + black;

				avg = avg < 0 ? 0 : sqrt(avg);
				diff = sqrt((float)(BAYER(row,col))) - avg;
				if (diff < -thold)
					diff += thold;
				else if (diff >  thold)
					diff -= thold;
				else
					diff = 0;

				BAYER(row,col) = DCRAW_CLIP((ushort)(DCRAW_SQR(avg+diff) + 0.5));
			}
		}
	}
	free (fimg);
	return S_OK;
}

HRESULT		scale_colors_INDI(ushort (*image)[4], int maximum, const int black,
							 const int use_auto_wb, const int use_camera_wb, const float cam_mul[4],
							 const int iheight, const int iwidth, const int colors, float pre_mul[4],
							 const unsigned filters, /*const*/ ushort white[8][8],
							 const char *ifname_display)
{
	_RAW_TRACE(OLESTR("[scale_colors_INDI]\n"));
	bool NoCamWB = false;
	if (!image)
		return E_POINTER;

	int dblack, row, col, x, y, c, val, sum[8];
	double dsum[8], dmin, dmax;
	float scale_mul[4];

	if (use_auto_wb || (use_camera_wb && cam_mul[0] == -1))
	{
		memset (dsum, 0, sizeof dsum);
		for (row=0; row < iheight-7; row += 8)
			for (col=0; col < iwidth-7; col += 8)
			{
				memset (sum, 0, sizeof sum);
				for (y=row; y < row+8; y++)
					for (x=col; x < col+8; x++)
						FORC4 
					{
						val = image[y*iwidth+x][c];
						if (!val)
							continue;
						if (val > maximum-25)
							goto skip_block;
						val -= black;
						if (val < 0)
							val = 0;
						sum[c] += val;
						sum[c+4]++;
					}
					for (c=0; c < 8; c++) dsum[c] += sum[c];
skip_block:
					continue;
			}

			FORC4
				if (dsum[c])
					pre_mul[c] = dsum[c+4] / dsum[c];
	}

	if (use_camera_wb && cam_mul[0] != -1)
	{
		memset (sum, 0, sizeof sum);

		for (row=0; row < 8; row++)
		{
			for (col=0; col < 8; col++)
			{
				c = FC(row,col);
				if ((val = white[row][col] - black) > 0)
					sum[c] += val;
				sum[c+4]++;
			}
		}

		if (sum[0] && sum[1] && sum[2] && sum[3])
		{
			//FORC4 
			for (c = 0;c<4;c++)
				pre_mul[c] = (float) sum[c+4] / sum[c];
		}
		else if (cam_mul[0] && cam_mul[2])
		{
			/* 'sizeof pre_mul' does not work because pre_mul is an argument (UF)*/
			memcpy (pre_mul, cam_mul, 4*sizeof(float));
		}
		else
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_WARNING,0,OLESTR("[imagerawcore]: cannot use camera white balance\n"));
			NoCamWB = true;

		}
	}
	//  if (user_mul[0])
	//    memcpy (pre_mul, user_mul, sizeof pre_mul);
	if (pre_mul[3] == 0)
		pre_mul[3] = colors < 4 ? pre_mul[1] : 1;

	dblack = black;
	//  if (threshold) wavelet_denoise();

	maximum -= black; // todo убрать?
	for (dmin=DBL_MAX, dmax=c=0; c < 4; c++)
	{
		if (dmin > pre_mul[c])
			dmin = pre_mul[c];
		if (dmax < pre_mul[c])
			dmax = pre_mul[c];
	}
	//  if (!highlight) dmax = dmin;
	//FORC4
	for (c = 0;c<4;c++)
		scale_mul[c] = (pre_mul[c] /= dmax) * 65535.0 / maximum;

	_RAW_TRACE(OLESTR("Scaling with black %d, multipliers:\npre_mul:\t"),dblack);
	FORC4
		_RAW_TRACE(OLESTR(" %.4f"),pre_mul[c]);
	_RAW_TRACE(OLESTR("\nscale_mul:\t"));
	FORC4
		_RAW_TRACE(OLESTR(" %.4f"),scale_mul[c]);
	_RAW_TRACE(OLESTR("\n"));


	/* The rest of the scaling is done somewhere else UF*/
#if 1
	for (row=0; row < iheight; row++)
	{
		for (col=0; col < iwidth; col++)
		{
			for (c = 0;c<4;c++)
			{
				val = image[row*iwidth+col][c];
				if (!val)
					continue;
				val -= black;
				val = (int)(val*scale_mul[c]);
				image[row*iwidth+col][c] = DCRAW_CLIP(val);
			}
		}
	}
#endif
	if (NoCamWB)
		return E_IMAGERAW(IMAGERAW_NOCAMWB_ERROR);
	return S_OK;
}

/*! \brief	Border interpolate
 */
HRESULT	border_interpolate_INDI (const int height, const int width,
									ushort (*image)[4], const unsigned filters, int colors,
									int border)
{
	_RAW_TRACE(OLESTR("[border_interpolate_INDI]\n"));
	if (!image)
		return E_POINTER;

	int row, col, y, x, f, c, sum[8];

	for (row=0; row < height; row++)
	{
		for (col=0; col < width; col++)
		{
			if (col==border && row >= border && row < height-border)
				col = width-border;
			memset (sum, 0, sizeof sum);
			for (y=row-1; y != row+2; y++)
				for (x=col-1; x != col+2; x++)
					if (y >= 0 && y < height && x >= 0 && x < width)
					{
						f = fc_INDI(filters, y, x);
						sum[f] += image[y*width+x][f];
						sum[f+4]++;
					}
					f = fc_INDI(filters,row,col);
					FORCC if (c != f && sum[c+4])
						image[row*width+col][c] = sum[c] / sum[c+4];
		}
	}
	return S_OK;
}

/*!
 *	\brief	Linear Interpolation
 */
HRESULT lin_interpolate_INDI(ushort (*image)[4], const unsigned filters,
								const int width, const int height, const int colors) /*UF*/
{
	_RAW_TRACE(OLESTR("[lin_interpolate_INDI]\n"));
	if (!image)
		return E_POINTER;
	_HR;
	int code[16][16][32], *ip, sum[4];
	int c, i, x, y, row, col, shift, color;
	ushort *pix;

//	dcraw_message (dcraw, DCRAW_VERBOSE,_("Bilinear interpolation...\n")); /*UF*/

	_CHECK_RET_(border_interpolate_INDI(height, width, image, filters, colors, 1))

	for (row=0; row < 16; row++)
	{
		for (col=0; col < 16; col++)
		{
			ip = code[row][col];
			memset (sum, 0, sizeof sum);
			for (y=-1; y <= 1; y++)
			{
				for (x=-1; x <= 1; x++)
				{
					shift = (y==0) + (x==0);
					if (shift == 2)
						continue;
					color = fc_INDI(filters,row+y,col+x);
					*ip++ = (width*y + x)*4 + color;
					*ip++ = shift;
					*ip++ = color;
					sum[color] += 1 << shift;
				}
			}

			FORCC
				if (c != fc_INDI(filters,row,col))
				{
					*ip++ = c;
					*ip++ = 256 / sum[c];
				}
		}
	}

	for (row=1; row < height-1; row++)
	{
		for (col=1; col < width-1; col++)
		{
			pix = image[row*width+col];
			ip = code[row & 15][col & 15];
			memset (sum, 0, sizeof sum);
			for (i=8; i--; ip+=3)
				sum[ip[2]] += pix[ip[0]] << ip[1];
			for (i=colors; --i; ip+=2)
				pix[ip[0]] = sum[ip[0]] * ip[1] >> 8;
		}
	}
	
	return S_OK;
}

/*!
 *	\brief		Interpolation using a Threshold-based variable number of gradients
 *
 *	\details	This algorithm is officially called:
 *				"Interpolation using a Threshold-based variable number of gradients"
 *				described in http://www-ise.stanford.edu/~tingchen/algodep/vargra.html
 *				I've extended the basic idea to work with non-Bayer filter arrays.
 *				Gradients are numbered clockwise from NW=0 to W=7.
 */
HRESULT vng_interpolate_INDI(ushort (*image)[4], const unsigned filters,
								   const int width, const int height, const int colors) /*UF*/
{
	_RAW_TRACE(OLESTR("[vng_interpolate_INDI]\n"));
	static const signed char *cp, terms[] = {
		-2,-2,+0,-1,0,0x01, -2,-2,+0,+0,1,0x01, -2,-1,-1,+0,0,0x01,
		-2,-1,+0,-1,0,0x02, -2,-1,+0,+0,0,0x03, -2,-1,+0,+1,1,0x01,
		-2,+0,+0,-1,0,0x06, -2,+0,+0,+0,1,0x02, -2,+0,+0,+1,0,0x03,
		-2,+1,-1,+0,0,0x04, -2,+1,+0,-1,1,0x04, -2,+1,+0,+0,0,0x06,
		-2,+1,+0,+1,0,0x02, -2,+2,+0,+0,1,0x04, -2,+2,+0,+1,0,0x04,
		-1,-2,-1,+0,0,0x80, -1,-2,+0,-1,0,0x01, -1,-2,+1,-1,0,0x01,
		-1,-2,+1,+0,1,0x01, -1,-1,-1,+1,0,0x88, -1,-1,+1,-2,0,0x40,
		-1,-1,+1,-1,0,0x22, -1,-1,+1,+0,0,0x33, -1,-1,+1,+1,1,0x11,
		-1,+0,-1,+2,0,0x08, -1,+0,+0,-1,0,0x44, -1,+0,+0,+1,0,0x11,
		-1,+0,+1,-2,1,0x40, -1,+0,+1,-1,0,0x66, -1,+0,+1,+0,1,0x22,
		-1,+0,+1,+1,0,0x33, -1,+0,+1,+2,1,0x10, -1,+1,+1,-1,1,0x44,
		-1,+1,+1,+0,0,0x66, -1,+1,+1,+1,0,0x22, -1,+1,+1,+2,0,0x10,
		-1,+2,+0,+1,0,0x04, -1,+2,+1,+0,1,0x04, -1,+2,+1,+1,0,0x04,
		+0,-2,+0,+0,1,0x80, +0,-1,+0,+1,1,0x88, +0,-1,+1,-2,0,0x40,
		+0,-1,+1,+0,0,0x11, +0,-1,+2,-2,0,0x40, +0,-1,+2,-1,0,0x20,
		+0,-1,+2,+0,0,0x30, +0,-1,+2,+1,1,0x10, +0,+0,+0,+2,1,0x08,
		+0,+0,+2,-2,1,0x40, +0,+0,+2,-1,0,0x60, +0,+0,+2,+0,1,0x20,
		+0,+0,+2,+1,0,0x30, +0,+0,+2,+2,1,0x10, +0,+1,+1,+0,0,0x44,
		+0,+1,+1,+2,0,0x10, +0,+1,+2,-1,1,0x40, +0,+1,+2,+0,0,0x60,
		+0,+1,+2,+1,0,0x20, +0,+1,+2,+2,0,0x10, +1,-2,+1,+0,0,0x80,
		+1,-1,+1,+1,0,0x88, +1,+0,+1,+2,0,0x08, +1,+0,+2,-1,0,0x40,
		+1,+0,+2,+1,0,0x10
	}, chood[] = { -1,-1, -1,0, -1,+1, 0,+1, +1,+1, +1,0, +1,-1, 0,-1 };

	if (!image)
		return E_POINTER;

	_HR;
	ushort (*brow[5])[4], *pix;
	int prow=7, pcol=1, *ip, *code[16][16], gval[8], gmin, gmax, sum[4];
	int row, col, x, y, x1, x2, y1, y2, t, weight, grads, color, diag;
	int g, diff, thold, num, c;

	_CHECK_RET_(lin_interpolate_INDI(image, filters, width, height, colors));

	//	dcraw_message (dcraw, DCRAW_VERBOSE,_("VNG interpolation...\n")); /*UF*/

	if (filters == 1) prow = pcol = 15;

	int *ipalloc = ip = (int *) calloc ((prow+1)*(pcol+1), 1280);

	if (!ip)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч ip (vng_interpolate_INDI)\n"));
		return	E_OUTOFMEMORY;		
	}

	for (row=0; row <= prow; row++)/* Precalculate for VNG */
	{
		for (col=0; col <= pcol; col++) {
			code[row][col] = ip;
			for (cp=terms, t=0; t < 64; t++) {
				y1 = *cp++;  x1 = *cp++;
				y2 = *cp++;  x2 = *cp++;
				weight = *cp++;
				grads = *cp++;
				color = fc_INDI(filters,row+y1,col+x1);
				if (fc_INDI(filters,row+y2,col+x2) != color)
					continue;
				diag = (fc_INDI(filters,row,col+1) == color && fc_INDI(filters,row+1,col) == color) ? 2:1;
				if (abs(y1-y2) == diag && abs(x1-x2) == diag)
					continue;
				*ip++ = (y1*width + x1)*4 + color;
				*ip++ = (y2*width + x2)*4 + color;
				*ip++ = weight;
				for (g=0; g < 8; g++)
					if (grads & 1<<g) *ip++ = g;
				*ip++ = -1;
			}
			*ip++ = INT_MAX;
			for (cp=chood, g=0; g < 8; g++)
			{
				y = *cp++;  x = *cp++;
				*ip++ = (y*width + x) * 4;
				color = fc_INDI(filters,row,col);
				if (fc_INDI(filters,row+y,col+x) != color && fc_INDI(filters,row+y*2,col+x*2) == color)
					*ip++ = (y*width + x) * 8 + color;
				else
					*ip++ = 0;
			}
		}
	}
	
	brow[4] = (ushort (*)[4]) calloc (width*3, sizeof **brow);

	if (!(brow[4]))
	{
		free(ipalloc);
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч brow[4] (vng_interpolate_INDI)\n"));
		return	E_OUTOFMEMORY;		
	}

	for (row=0; row < 3; row++)
		brow[row] = brow[4] + row*width;

	/*! Do VNG interpolation */
	for (row=2; row < height-2; row++)
	{		
		for (col=2; col < width-2; col++)
		{
			pix = image[row*width+col];
			ip = code[row & prow][col & pcol];
			memset (gval, 0, sizeof gval);
			/*! Calculate gradients */
			while ((g = ip[0]) != INT_MAX)
			{		
				diff = DCRAW_ABS(pix[g] - pix[ip[1]]) << ip[2];
				gval[ip[3]] += diff;
				ip += 5;
				if ((g = ip[-1]) == -1) continue;
				gval[g] += diff;
				while ((g = *ip++) != -1)
					gval[g] += diff;
			}
			ip++;
			/*! Choose a threshold */
			gmin = gmax = gval[0];			
			for (g=1; g < 8; g++)
			{
				if (gmin > gval[g]) gmin = gval[g];
				if (gmax < gval[g]) gmax = gval[g];
			}
			
			if (gmax == 0)
			{
				memcpy (brow[2][col], pix, sizeof *image);
				continue;
			}

			thold = gmin + (gmax >> 1);
			memset (sum, 0, sizeof sum);
			color = fc_INDI(filters,row,col);
			/*! Average the neighbors */
			for (num=g=0; g < 8; g++,ip+=2)
			{		
				if (gval[g] <= thold)
				{
					FORCC
						if (c == color && ip[1])
							sum[c] += (pix[c] + pix[ip[1]]) >> 1;
						else
							sum[c] += pix[ip[0] + c];
					num++;
				}
			}
			
			/*! Save to buffer */
			FORCC {					
				t = pix[color];
				if (c != color)
					t += (sum[c] - sum[color]) / num;
				brow[2][col][c] = DCRAW_CLIP(t);
			}

		}
		/*! Write buffer to image */
		
		if (row > 3)				
			memcpy (image[(row-2)*width+2], brow[0]+2, (width-4)*sizeof *image);

		for (g=0; g < 4; g++)
			brow[(g-1) & 3] = brow[g];
	}

	memcpy (image[(row-2)*width+2], brow[0]+2, (width-4)*sizeof *image);
	memcpy (image[(row-1)*width+2], brow[1]+2, (width-4)*sizeof *image);
	free (brow[4]);
	free(ipalloc);
	
	return S_OK;
}


#define UT(c1, c2, c3, g1, g3) \
	DCRAW_CLIP((long)(((g1 +g3) >> 1) +((c2-c1 +c2-c3) >> 3)))

#define UT1(v1, v2, v3, c1, c3) \
	DCRAW_CLIP((long)(v2 +((c1 +c3 -v1 -v3) >> 1)))

/*!
 *	\brief	Patterned Pixel Grouping Interpolation by Alain Desbiolles
 */
HRESULT ppg_interpolate_INDI(ushort (*image)[4], const unsigned filters,
							 const int width, const int height, const int colors)
{
	_RAW_TRACE(OLESTR("[ppg_interpolate_INDI]\n"));
	ushort (*pix)[4];            // Pixel matrix
	ushort g2, c1, c2, cc1, cc2; // Simulated green and color
	int    row, col, diff[2], guess[2], c, d, i;
	int    dir[5]  = { 1, width, -1, -width, 1 };
	int    g[2][4] = {{ -1 -2*width, -1 +2*width,  1 -2*width, 1 +2*width },
	{ -2 -width,    2 -width,   -2 +width,   2 +width   }};

	_HR;

	if (!image)
		return E_POINTER;
	
	_CHECK_RET_(border_interpolate_INDI (height, width, image, filters, colors, 4));

	//	dcraw_message (dcraw, DCRAW_VERBOSE,_("PPG interpolation...\n")); /*UF*/

	//! Fill in the green layer with gradients from RGB color pattern simulation
	for (row=3; row < height-4; row++)
	{
		for (col=3+(FC(row,3) & 1), c=FC(row,col); col < width-4; col+=2)
		{
			pix = image + row*width+col;

			//! Horizontaly and verticaly
			for (i=0; d=dir[i], i < 2; i++) {

				//! Simulate RGB color pattern
				guess[i] = UT (pix[-2*d][c], pix[0][c], pix[2*d][c],
					pix[-d][1], pix[d][1]);
				g2       = UT (pix[0][c], pix[2*d][c], pix[4*d][c],
					pix[d][1], pix[3*d][1]);
				c1       = UT1(pix[-2*d][1], pix[-d][1], guess[i],
					pix[-2*d][c], pix[0][c]);
				c2       = UT1(guess[i], pix[d][1], g2,
					pix[0][c], pix[2*d][c]);
				cc1      = UT (pix[g[i][0]][1], pix[-d][1], pix[g[i][1]][1],
					pix[-1-width][2-c], pix[1-width][2-c]);
				cc2      = UT (pix[g[i][2]][1],  pix[d][1], pix[g[i][3]][1],
					pix[-1+width][2-c], pix[1+width][2-c]);

				//! Calculate gradient with RGB simulated color
				diff[i]  = ((DCRAW_ABS(pix[-d][1] -pix[-3*d][1]) +
					DCRAW_ABS(pix[0][c]  -pix[-2*d][c]) +
					DCRAW_ABS(cc1        -cc2)          +
					DCRAW_ABS(pix[0][c]  -pix[2*d][c])  +
					DCRAW_ABS(pix[d][1]  -pix[3*d][1])) * 2 / 3) +
					DCRAW_ABS(guess[i]   -pix[-d][1])   +
					DCRAW_ABS(pix[0][c]  -c1)           +
					DCRAW_ABS(pix[0][c]  -c2)           +
					DCRAW_ABS(guess[i]   -pix[d][1]);
			}

			// Then, select the best gradient
			d = dir[diff[0] > diff[1]];
			pix[0][1] = DCRAW_ULIM(guess[diff[0] > diff[1]], pix[-d][1], pix[d][1]);
		}
	}

	//! Calculate red and blue for each green pixel
	for (row=1; row < height-1; row++)
	{
		for (col=1+(FC(row,2) & 1), c=FC(row,col+1); col < width-1; col+=2)
		{
			pix = image + row*width+col;
			for (i=0; (d=dir[i]) > 0; c=2-c, i++)
				pix[0][c] = UT1(pix[-d][1], pix[0][1], pix[d][1],pix[-d][c], pix[d][c]);
		}
	}

	//! Calculate blue for red pixels and vice versa
	for (row=1; row < height-1; row++)
	{
		for (col=1+(FC(row,1) & 1), c=2-FC(row,col); col < width-1; col+=2) {
			pix = image + row*width+col;
			for (i=0; (d=dir[i]+dir[i+1]) > 0; i++) {
				diff[i]  = DCRAW_ABS(pix[-d][c] - pix[d][c]) +
					DCRAW_ABS(pix[-d][1] - pix[d][1]);
				guess[i] = UT1(pix[-d][1], pix[0][1], pix[d][1],
					pix[-d][c], pix[d][c]);
			}
			pix[0][c] = DCRAW_CLIP(guess[diff[0] > diff[1]]);
		}
	}
	return S_OK;
}


/*!
 *	Tile Size
 */
#define TS 256		

/*!
 *	\brief	Adaptive Homogeneity-Directed interpolation is based on
 *			the work of Keigo Hirakawa, Thomas Parks, and Paul Lee.
 */
HRESULT ahd_interpolate_INDI(ushort (*image)[4], const unsigned filters,
							 const int width, const int height, const int colors,
							 const float rgb_cam[3][4])
{
	_RAW_TRACE(OLESTR("[ahd_interpolate_INDI]\n"));
	int i, j, k, top, left, row, col, tr, tc, c, d, val, hm[2];
	ushort (*pix)[4], (*rix)[3];
	static const int dir[4] = { -1, 1, -TS, TS };
	unsigned ldiff[2][4], abdiff[2][4], leps, abeps;
	float r, cbrt[0x10000], xyz[3], xyz_cam[3][4];
	ushort (*rgb)[TS][TS][3];
	short (*lab)[TS][TS][3], (*lix)[3];
	char (*homo)[TS][TS], *buffer;
	_HR;

	if (!image)
		return E_POINTER;

	for (i=0; i < 0x10000; i++)
	{
		r = i / 65535.0;
		cbrt[i] = r > 0.008856 ? pow((float)r,(float)(1./3.0)) : 7.787*r + 16/116.0;
	}

	for (i=0; i < 3; i++)
		for (j=0; j < colors; j++)
			for (xyz_cam[i][j] = k=0; k < 3; k++)
				xyz_cam[i][j] += xyz_rgb[i][k] * rgb_cam[k][j] / d65_white[i];

	_CHECK_RET_(border_interpolate_INDI (height, width, image, filters, colors, 5));

	buffer = (char *) malloc (26*TS*TS);		/* 1664 kB */
	if (!buffer)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч buffer (ahd_interpolate_INDI)\n"));
		return	E_OUTOFMEMORY;	
	}

	rgb  = (ushort(*)[TS][TS][3]) buffer;
	lab  = (short (*)[TS][TS][3])(buffer + 12*TS*TS);
	homo = (char  (*)[TS][TS])   (buffer + 24*TS*TS);

	for (top=2; top < height-5; top += TS-6)
	{
		for (left=2; left < width-5; left += TS-6)
		{

			/*!  Interpolate green horizontally and vertically:              */
			for (row = top; row < top+TS && row < height-2; row++)
			{
				col = left + (FC(row,left) & 1);
				for (c = FC(row,col); col < left+TS && col < width-2; col+=2) {
					pix = image + row*width+col;
					val = ((pix[-1][1] + pix[0][c] + pix[1][1]) * 2
						- pix[-2][c] - pix[2][c]) >> 2;
					rgb[0][row-top][col-left][1] = DCRAW_ULIM(val,pix[-1][1],pix[1][1]);
					val = ((pix[-width][1] + pix[0][c] + pix[width][1]) * 2
						- pix[-2*width][c] - pix[2*width][c]) >> 2;
					rgb[1][row-top][col-left][1] = DCRAW_ULIM(val,pix[-width][1],pix[width][1]);
				}
			}
			/*!  Interpolate red and blue, and convert to CIELab:		*/
			for (d=0; d < 2; d++){
				for (row=top+1; row < top+TS-1 && row < height-3; row++)
				{
					for (col=left+1; col < left+TS-1 && col < width-3; col++)
					{
						pix = image + row*width+col;
						rix = &rgb[d][row-top][col-left];
						lix = &lab[d][row-top][col-left];
						if ((c = 2 - FC(row,col)) == 1)
						{
							c = FC(row+1,col);
							val = pix[0][1] + (( pix[-1][2-c] + pix[1][2-c]
							- rix[-1][1] - rix[1][1] ) >> 1);
							rix[0][2-c] = DCRAW_CLIP(val);
							val = pix[0][1] + (( pix[-width][c] + pix[width][c]
							- rix[-TS][1] - rix[TS][1] ) >> 1);
						} else
							val = rix[0][1] + (( pix[-width-1][c] + pix[-width+1][c]
						+ pix[+width-1][c] + pix[+width+1][c]
						- rix[-TS-1][1] - rix[-TS+1][1]
						- rix[+TS-1][1] - rix[+TS+1][1] + 1) >> 2);
						rix[0][c] = DCRAW_CLIP(val);
						c = FC(row,col);
						rix[0][c] = pix[0][c];
						xyz[0] = xyz[1] = xyz[2] = 0.5;
						FORCC {
							xyz[0] += xyz_cam[0][c] * rix[0][c];
							xyz[1] += xyz_cam[1][c] * rix[0][c];
							xyz[2] += xyz_cam[2][c] * rix[0][c];
						}
						xyz[0] = cbrt[DCRAW_CLIP((int) xyz[0])];
						xyz[1] = cbrt[DCRAW_CLIP((int) xyz[1])];
						xyz[2] = cbrt[DCRAW_CLIP((int) xyz[2])];
						lix[0][0] = (short)(64 * (116 * xyz[1] - 16));
						lix[0][1] = (short)(64 * 500 * (xyz[0] - xyz[1]));
						lix[0][2] = (short)(64 * 200 * (xyz[1] - xyz[2]));
					}
				}
			}

			/*! Build homogeneity maps from the CIELab images */
			memset (homo, 0, 2*TS*TS);
			for (row=top+2; row < top+TS-2 && row < height-4; row++)
			{
				tr = row-top;
				for (col=left+2; col < left+TS-2 && col < width-4; col++)
				{
					tc = col-left;
					for (d=0; d < 2; d++)
					{
						lix = &lab[d][tr][tc];
						for (i=0; i < 4; i++)
						{
							ldiff[d][i] = DCRAW_ABS(lix[0][0]-lix[dir[i]][0]);
							abdiff[d][i] = DCRAW_SQR(lix[0][1]-lix[dir[i]][1])
								+ DCRAW_SQR(lix[0][2]-lix[dir[i]][2]);
						}
					}
					leps = DCRAW_MIN(DCRAW_MAX(ldiff[0][0],ldiff[0][1]),DCRAW_MAX(ldiff[1][2],ldiff[1][3]));
					abeps = DCRAW_MIN(DCRAW_MAX(abdiff[0][0],abdiff[0][1]),DCRAW_MAX(abdiff[1][2],abdiff[1][3]));
					for (d=0; d < 2; d++)
						for (i=0; i < 4; i++)
							if (ldiff[d][i] <= leps && abdiff[d][i] <= abeps)
								homo[d][tr][tc]++;
				}
			}
			/*  Combine the most homogenous pixels for the final result:	*/
			for (row=top+3; row < top+TS-3 && row < height-5; row++)
			{
				tr = row-top;
				for (col=left+3; col < left+TS-3 && col < width-5; col++)
				{
					tc = col-left;
					for (d=0; d < 2; d++)
						for (hm[d]=0, i=tr-1; i <= tr+1; i++)
							for (j=tc-1; j <= tc+1; j++)
								hm[d] += homo[d][i][j];

					if (hm[0] != hm[1])
						FORC3 image[row*width+col][c] = rgb[hm[1] > hm[0]][tr][tc][c];

					else
						FORC3 image[row*width+col][c] =
						(rgb[0][tr][tc][c] + rgb[1][tr][tc][c]) >> 1;
				}
			}
		}
	}
	free (buffer);
	return S_OK;
}
#undef TS


#define DTOP(x) ((x>65535) ? (unsigned short)65535 : (x<0) ? (unsigned short)0 : (unsigned short) x)

/*
* MG - This comment applies to the 3x3 optimized median function
*
* The following routines have been built from knowledge gathered
* around the Web. I am not aware of any copyright problem with
* them, so use it as you want.
* N. Devillard - 1998
*/

#define PIX_SORT(a,b) { if ((a)>(b)) PIX_SWAP((a),(b)); }
#define PIX_SWAP(a,b) { int temp=(a);(a)=(b);(b)=temp; }

static inline int median9(int *p)
{
	PIX_SORT(p[1], p[2]) ; PIX_SORT(p[4], p[5]) ; PIX_SORT(p[7], p[8]) ;
	PIX_SORT(p[0], p[1]) ; PIX_SORT(p[3], p[4]) ; PIX_SORT(p[6], p[7]) ;
	PIX_SORT(p[1], p[2]) ; PIX_SORT(p[4], p[5]) ; PIX_SORT(p[7], p[8]) ;
	PIX_SORT(p[0], p[3]) ; PIX_SORT(p[5], p[8]) ; PIX_SORT(p[4], p[7]) ;
	PIX_SORT(p[3], p[6]) ; PIX_SORT(p[1], p[4]) ; PIX_SORT(p[2], p[5]) ;
	PIX_SORT(p[4], p[7]) ; PIX_SORT(p[4], p[2]) ; PIX_SORT(p[6], p[4]) ;
	PIX_SORT(p[4], p[2]) ; return(p[4]) ;
}

#undef PIX_SWAP
#undef PIX_SORT

// Just making this function inline speeds up ahd_interpolate_INDI() up to 15%
static inline ushort eahd_median(int row, int col, int color,ushort (*image)[4], const int width)
{
	//declare the pixel array
	int pArray[9];
	int result;

	//perform the median filter (this only works for red or blue)
	//  result = median(R-G)+G or median(B-G)+G
	//
	// to perform the filter on green, it needs to be the average
	//  results = (median(G-R)+median(G-B)+R+B)/2

	//no checks are done here to speed up the inlining
	pArray[0] = image[width*(row  )+col+1][color] - image[width*(row  )+col+1][1];
	pArray[1] = image[width*(row-1)+col+1][color] - image[width*(row-1)+col+1][1];
	pArray[2] = image[width*(row-1)+col  ][color] - image[width*(row-1)+col  ][1];
	pArray[3] = image[width*(row-1)+col-1][color] - image[width*(row-1)+col-1][1];
	pArray[4] = image[width*(row  )+col-1][color] - image[width*(row  )+col-1][1];
	pArray[5] = image[width*(row+1)+col-1][color] - image[width*(row+1)+col-1][1];
	pArray[6] = image[width*(row+1)+col  ][color] - image[width*(row+1)+col  ][1];
	pArray[7] = image[width*(row+1)+col+1][color] - image[width*(row+1)+col+1][1];
	pArray[8] = image[width*(row  )+col  ][color] - image[width*(row  )+col  ][1];

	median9(pArray);
	result = pArray[4]+image[width*(row  )+col  ][1];
	return DTOP(result);

}


/*!	\brief	Add the color smoothing from Kimmel as suggested in the AHD paper
 *			Algorithm updated by Michael Goertz
 */

HRESULT color_smooth(ushort (*image)[4], const int width, const int height,const int passes)
{
	_RAW_TRACE(OLESTR("[color_smooth]\n"));
	int row, col;
	int row_start = 2;
	int row_stop  = height-2;
	int col_start = 2;
	int col_stop  = width-2;
	//interate through all the colors
	int count;

	if (!image)
		return E_POINTER;

	ushort *mpix;

	for (count=0; count<passes; count++)
	{
		//perform 3 iterations - seems to be a commonly settled upon number of iterations
		for (row=row_start; row < row_stop; row++)
		{
			for (col=col_start; col < col_stop; col++)
			{
				//calculate the median only over the red and blue
				//calculating over green seems to offer very little additional quality
				mpix = image[row*width+col];
				mpix[0] = eahd_median(row, col, 0, image, width);
				mpix[2] = eahd_median(row, col, 2, image, width);
			}
		}
	}
	return S_OK;
}

HRESULT fuji_rotate_INDI(ushort (**image_p)[4], int *height_p,
						 int *width_p, int *fuji_width_p, const int colors, const double step)
{
	_RAW_TRACE(OLESTR("[fuji_rotate_INDI]\n"));
	if (!image_p || !height_p || !width_p)
		return E_POINTER;

	int height = *height_p, width = *width_p, fuji_width = *fuji_width_p; /*UF*/
	ushort (*image)[4] = *image_p; /*UF*/
	int i, row, col;
	//  double step;
	float r, c, fr, fc;
	int ur, uc;
	ushort wide, high, (*img)[4], (*pix)[4];

	if (!fuji_width)
		return S_FALSE;

	//	dcraw_message (dcraw, DCRAW_VERBOSE,_("Rotating image 45 degrees...\n"));
	fuji_width = (fuji_width - 1/* + shrink*/)/* >> shrink*/;
	//  step = sqrt(0.5);
	wide = (int)(fuji_width / step);
	high = (int)((height - fuji_width) / step);

	img = (ushort (*)[4]) calloc (wide*high, sizeof *img);
	if (!img)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч img (fuji_rotate_INDI)\n"));
		return	E_OUTOFMEMORY;	
	}

	for (row=0; row < high; row++)
	{
		for (col=0; col < wide; col++)
		{
			ur = (int)(r = fuji_width + (row-col)*step);
			uc = (int)(c = (row+col)*step);
			if (ur > height-2 || uc > width-2)
				continue;
			fr = r - ur;
			fc = c - uc;
			pix = image + ur*width + uc;
			for (i=0; i < colors; i++)
				img[row*wide+col][i] = (ushort)(
				(pix[    0][i]*(1-fc) + pix[      1][i]*fc) * (1-fr) +
				(pix[width][i]*(1-fc) + pix[width+1][i]*fc) * fr);
		}
	}
	free (image);
	width  = wide;
	height = high;
	image  = img;
	fuji_width = 0;
	*height_p = height; /* INDI - UF*/
	*width_p = width;
	*fuji_width_p = fuji_width;
	*image_p = image;
	return S_OK;
}

HRESULT flip_image_INDI(ushort (*image)[4], int *height_p, int *width_p,
						   /*const*/ int flip) /*UF*/
{
	_RAW_TRACE(OLESTR("[flip_image_INDI]\n"));
	if (!height_p || !width_p || !image)
		return E_POINTER;

	unsigned *flag;
	int size, base, dest, next, row, col;
	INT64 *img, hold;
	int height = *height_p, width = *width_p;/* INDI - UF*/

	//  Message is suppressed because error handling is not enabled here.
	//  dcraw_message (dcraw, DCRAW_VERBOSE,_("Flipping image %c:%c:%c...\n"),
	//	flip & 1 ? 'H':'0', flip & 2 ? 'V':'0', flip & 4 ? 'T':'0'); /*UF*/

	img = (INT64 *) image;
	size = height * width;
	flag = (unsigned int *)calloc ((size+31) >> 5, sizeof *flag);
	if (!flag)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч flag (flip_image_INDI)\n"));
		return	E_OUTOFMEMORY;	
	}
	
	
	for (base = 0; base < size; base++)
	{
		if (flag[base >> 5] & (1 << (base & 31)))
			continue;
		dest = base;
		hold = img[base];
		while (1)
		{
			if (flip & 4)
			{
				row = dest % height;
				col = dest / height;
			}
			else
			{
				row = dest / width;
				col = dest % width;
			}

			if (flip & 2)
				row = height - 1 - row;

			if (flip & 1)
				col = width - 1 - col;

			next = row * width + col;
			if (next == base)
				break;

			flag[next >> 5] |= 1 << (next & 31);

			img[dest] = img[next];
			dest = next;
		}
		img[dest] = hold;
	}
	free (flag);
	
	if (flip & 4)
		DCRAW_SWAP (height, width);

	*height_p = height; 
	*width_p = width;

	return S_OK;
}



HRESULT dcraw_image_resize(dcraw_image_data *image, int size)
{
	_RAW_TRACE(OLESTR("[dcraw_image_resize]\n"));
	int h, w, wid, r, ri, rii, c, ci, cii, cl, norm;
	guint64 riw, riiw, ciw, ciiw;
	guint64 (*iBuf)[4];
	int mul=size, div=DCRAW_MAX(image->height, image->width);

	if (mul > div)
		return E_INVALIDARG;

	/* I'm skiping the last row/column if it is not a full row/column */
	h = image->height * mul / div;
	w = image->width * mul / div;
	wid = image->width;
	iBuf = (guint64(*)[4])g_new0(guint64, h * w * 4);
	norm = div * div;

	for(r=0; r<image->height; r++)
	{
		/* r should be divided between ri and rii */
		ri = r * mul / div;
		rii = (r+1) * mul / div;
		/* with weights riw and riiw (riw+riiw==mul) */
		riw = rii * div - r * mul;
		riiw = (r+1) * mul - rii * div;
		if (rii>=h) {rii=h-1; riiw=0;}
		if (ri>=h) {ri=h-1; riw=0;}
		for(c=0; c<image->width; c++)
		{
			ci = c * mul / div;
			cii = (c+1) * mul / div;
			ciw = cii * div - c * mul;
			ciiw = (c+1) * mul - cii * div;
			if (cii>=w) {cii=w-1; ciiw=0;}
			if (ci>=w) {ci=w-1; ciw=0;}
			for (cl=0; cl<image->colors; cl++)
			{
				iBuf[ri *w+ci ][cl] += image->image[r*wid+c][cl]*riw *ciw ;
				iBuf[ri *w+cii][cl] += image->image[r*wid+c][cl]*riw *ciiw;
				iBuf[rii*w+ci ][cl] += image->image[r*wid+c][cl]*riiw*ciw ;
				iBuf[rii*w+cii][cl] += image->image[r*wid+c][cl]*riiw*ciiw;
			}
		}
	}
	for (c=0; c<h*w; c++) for (cl=0; cl<image->colors; cl++)
		image->image[c][cl] = iBuf[c][cl]/norm;
	g_free(iBuf);
	image->height = h;
	image->width = w;
	return S_OK;
}

/* Adapted from dcraw.c stretch() - NKBJ */
HRESULT dcraw_image_stretch(dcraw_image_data *image, double pixel_aspect)
{
	_RAW_TRACE(OLESTR("[dcraw_image_stretch]\n"));
	_HR;
	int newdim, row, col, c, colors = image->colors;
	double rc, frac;
	ushort *pix0, *pix1;
	dcraw_image_type *iBuf;

	if (pixel_aspect==1)
		return S_OK;
	if (pixel_aspect < 1)
	{
		newdim = (int)(image->height / pixel_aspect + 0.5);
		
		iBuf = g_new(dcraw_image_type, image->width * newdim);
		
		if (!iBuf)
			return E_OUTOFMEMORY;

		for (rc=row=0; row < newdim; row++, rc+=pixel_aspect)
		{
			frac = rc - (c = (int)rc);
			pix0 = pix1 = image->image[c*image->width];
			if (c+1 < image->height) pix1 += image->width*4;
			for (col=0; col < image->width; col++, pix0+=4, pix1+=4)
				FORCC iBuf[row*image->width+col][c] =
				(guint16)(pix0[c]*(1-frac) + pix1[c]*frac + 0.5);
		}
		image->height = newdim;
	} else
	{
		newdim = (int)(image->width * pixel_aspect + 0.5);
		iBuf = g_new(dcraw_image_type, image->height * newdim);
		for (rc=col=0; col < newdim; col++, rc+=1/pixel_aspect)
		{
			frac = rc - (c = (int)rc);
			pix0 = pix1 = image->image[c];
			if (c+1 < image->width) pix1 += 4;
			for (row=0; row < image->height;
				row++, pix0+=image->width*4, pix1+=image->width*4)
				FORCC iBuf[row*newdim+col][c] =
				(guint16)(pix0[c]*(1-frac) + pix1[c]*frac + 0.5);
		}
		image->width = newdim;
	}
	g_free(image->image);
	image->image = iBuf;
	return S_OK;
}

HRESULT dcraw_flip_image(dcraw_image_data *image, int flip)
{
	_HR;
	if (flip)
	{
		_CHECK_RET_(flip_image_INDI(image->image, &image->height, &image->width, flip));
	}
	return S_OK;
}

	

/*********************************************************************************************/

c_dcraw_data::c_dcraw_data()
{

}

c_dcraw_data::~c_dcraw_data()
{
	_close();
}


HRESULT c_dcraw_data::_close()
{
	_m_dcraw_data.reset();
	return S_OK;
}

HRESULT	c_dcraw_data::_open(BSTR bsFileName)
{
	_RAW_TRACE(OLESTR("[c_dcraw_data::_open]\n"));

	_HR;

	dcraw_data * m_dcraw_data = &_m_dcraw_data;

	if (!bsFileName)
		return E_INVALIDARG;

	DCRaw * m_dcraw = NULL;
	if (!(m_dcraw = new DCRaw()))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч cannot allocate DCRaw object\n"));
		return	E_OUTOFMEMORY;
	}

	m_bsFileName = _bstr_t(bsFileName).copy();

	errno_t err;
	if (0 != (err = _IMAGERAWFILE_OPEN(m_dcraw->ifp,(const wchar_t *)bsFileName)))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,
			OLESTR("[imagerawcore][_dcraw_open]: cannot open file %s, errorno: %d\n"),(const wchar_t *)bsFileName,err);

		_ERRORRET_(IMAGERAW_OPENFILE_ERROR);	
	}

	if (!(m_dcraw->ifp))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,
			OLESTR("[imagerawcore][_dcraw_open]: cannot open file %s\n"),(const wchar_t *)bsFileName,err);
		_CHECK_HR(_close());
		_ERRORRET_(IMAGERAW_OPENFILE_ERROR);	
	}

	try{
		m_dcraw->identify();
	}
	catch(...)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore][_dcraw_open]: dcraw external exception\n"));
		_CHECK_HR(_close());
		_ERRORRET_(IMAGERAW_DCRAWINTERNALEXCEPTION_ERROR);
	}

	/* We first check if dcraw recognizes the file*/
	if (!(m_dcraw->make[0]))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore][_dcraw_open]: unsupported file format\n"));
		_CHECK_HR(_close());
		_ERRORRET_(IMAGERAW_UNSUPPORTERDFORMAT_ERROR);
	}

	/* Next we check if dcraw can decode the file */
	if (!m_dcraw->is_raw)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore][_dcraw_open]: cannot decode file\n"));
		_CHECK_HR(_close());
		_ERRORRET_(IMAGERAW_DECODE_ERROR);
	}

	if (m_dcraw->load_raw == &DCRaw::kodak_ycbcr_load_raw)
	{
		m_dcraw->height += m_dcraw->height & 1;
		m_dcraw->width += m_dcraw->width & 1;
	}

	// load thumbnail
	
	long currentpos = 0;
	if (m_dcraw->ifp)
		currentpos = ftell(m_dcraw->ifp);

	m_thumbnail.Reset();
	do{
		if (m_dcraw->thumb_offset)
			if (!(m_dcraw->thumb_load_raw)) // todo другой тип thumbnail'а
				if (m_dcraw->ifp)
				{
					if (fseek(m_dcraw->ifp, m_dcraw->thumb_offset, SEEK_SET))
						break; // неудачный seek, пропускаем загрузку thumbnail'а 
					
					char * thumbnail_data = 0;
					size_t thumbnail_size = 0;
					
					if (m_dcraw->write_thumb_)
						if SUCCEEDED((m_dcraw->*m_dcraw->write_thumb_)((void **)&thumbnail_data, &thumbnail_size))
							m_thumbnail.CopyData(thumbnail_data,thumbnail_size);
				}
	}
	while(0);

	if (m_dcraw->ifp)
		fseek(m_dcraw->ifp,currentpos,SEEK_SET);


	m_dcraw_data->SetDCRaw(m_dcraw);
	m_dcraw_data->ifp = m_dcraw->ifp;
	m_dcraw_data->height = m_dcraw->height;
	m_dcraw_data->width = m_dcraw->width;
	m_dcraw_data->fuji_width = m_dcraw->fuji_width;
	m_dcraw_data->fuji_step = sqrt((double)0.5);
	m_dcraw_data->colors = m_dcraw->colors;
	m_dcraw_data->filters = m_dcraw->filters;
	m_dcraw_data->raw_color = m_dcraw->raw_color;
	m_dcraw_data->shrink = m_dcraw->shrink = (m_dcraw_data->filters!=0);
	m_dcraw_data->pixel_aspect = m_dcraw->pixel_aspect;

	switch ((m_dcraw->flip+3600) % 360)
	{
	case 270:
		m_dcraw->flip = 5;
		break;
	case 180:
		m_dcraw->flip = 3;
		break;
	case  90:
		m_dcraw->flip = 6;
	}

	m_dcraw_data->flip = m_dcraw->flip;
	m_dcraw_data->toneCurveSize = m_dcraw->tone_curve_size;
	m_dcraw_data->toneCurveOffset = m_dcraw->tone_curve_offset;
	m_dcraw_data->toneModeOffset = m_dcraw->tone_mode_offset;
	m_dcraw_data->toneModeSize = m_dcraw->tone_mode_size;
	strcpy_s(m_dcraw_data->make, 80, m_dcraw->make);
	strcpy_s(m_dcraw_data->model, 80, m_dcraw->model);
	m_dcraw_data->iso_speed = m_dcraw->iso_speed;
	m_dcraw_data->shutter = m_dcraw->shutter;
	m_dcraw_data->aperture = m_dcraw->aperture;
	m_dcraw_data->focal_len = m_dcraw->focal_len;
	m_dcraw_data->timestamp = m_dcraw->timestamp;
	m_dcraw_data->raw.image = NULL;
	m_dcraw_data->thumbType = unknown_thumb_type;
	//m_dcraw_data->message = m_dcraw->messageBuffer;
	return S_OK;
}

HRESULT c_dcraw_data::_image_dimensions(int flip, int *height, int *width)
{
	_RAW_TRACE(OLESTR("[c_dcraw_data::_image_dimensions]\n"));
	dcraw_data * raw = &_m_dcraw_data;

	*width = NULL;
	*height = NULL;

	if (raw->fuji_width) {
		if (raw->fuji_step)
		{
			*width = (int)((raw->fuji_width - 1) / raw->fuji_step);
			*height = (int)((raw->height - raw->fuji_width + 1) / raw->fuji_step);
		}
	} else {
		if (raw->pixel_aspect < 1)
		{	
			if (raw->pixel_aspect)
				*height = (int)(raw->height / raw->pixel_aspect + 0.5);
		}
		else
			*height = raw->height;

		if (raw->pixel_aspect > 1)
			*width = (int)(raw->width * raw->pixel_aspect + 0.5);
		else
			*width = raw->width;
	}

	if (flip & 4)
	{
		int tmp = *height;
		*height = *width;
		*width = tmp;
	}

	return S_OK;
}

HRESULT c_dcraw_data::_load_raw()
{
	_RAW_TRACE(OLESTR("[c_dcraw_data::_load_raw]\n"));
	dcraw_data * m_dcraw_data = &_m_dcraw_data;

	if (!(m_dcraw_data->GetDCRaw()))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: internal object not initialized\n"));
		return E_POINTER;
	}

	dcraw_data * _dcraw_data = m_dcraw_data;
	DCRaw * _dcraw = _dcraw_data->GetDCRaw();

	int i, j;
	double dmin;

	_dcraw_data->raw.height = _dcraw->iheight = (_dcraw_data->height+_dcraw_data->shrink) >> _dcraw_data->shrink;
	_dcraw_data->raw.width = _dcraw->iwidth = (_dcraw_data->width+_dcraw_data->shrink) >> _dcraw_data->shrink;

	/* free */
	if ((_dcraw->image == _dcraw_data->raw.image) && (_dcraw_data->raw.image) )
		free(_dcraw->image);

	_dcraw->image = _dcraw_data->raw.image = NULL;

	_dcraw_data->raw.image = _dcraw->image 
		= g_new0(dcraw_image_type, _dcraw->iheight * _dcraw->iwidth + _dcraw->meta_length);

	if (!(_dcraw_data->raw.image))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч cannot allocate mem for image\n"));
		return	E_OUTOFMEMORY;
	}

	_dcraw->meta_data = (char *) (_dcraw->image + _dcraw->iheight*_dcraw->iwidth);

	if (_dcraw->filters && _dcraw->colors == 3) {
		for (i=0; i < 32; i+=4)
		{
			if ((_dcraw->filters >> i & 15) == 9)
				_dcraw->filters |= 2 << i;
			if ((_dcraw->filters >> i & 15) == 6)
				_dcraw->filters |= 8 << i;
		}
		_dcraw->colors++;
	}

	_dcraw_data->raw.colors = _dcraw->colors;
	_dcraw_data->fourColorFilters = _dcraw->filters;


	if (!(_dcraw->ifp))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: invalid file handle\n"));
		if ((_dcraw->image == _dcraw_data->raw.image) && (_dcraw_data->raw.image) )
			free(_dcraw->image);
		_dcraw->image = _dcraw_data->raw.image = NULL;
		return	E_FAIL;
	}

	if (0 != fseek (_dcraw->ifp, _dcraw->data_offset, SEEK_SET))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: file seek error\n"));
		if ((_dcraw->image == _dcraw_data->raw.image) && (_dcraw_data->raw.image) )
			free(_dcraw->image);
		_dcraw->image = _dcraw_data->raw.image = NULL;
		return	E_FAIL;
	}

	//!	\todo	ѕроверить условие	
	if (!(_dcraw->load_raw))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: load raw function not initialized\n"));
		if ((_dcraw->image == _dcraw_data->raw.image) && (_dcraw_data->raw.image) )
			free(_dcraw->image);
		_dcraw->image = _dcraw_data->raw.image = NULL;
		return	E_FAIL;
	}
 
	//  функци€ merror в DCRaw при ошибке выделени€ пам€ти возвращает исключение DCRawMemException
	try{	
		(_dcraw->*_dcraw->load_raw)();
	}catch(DCRawMemException ex)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory\n"));		
		if ((_dcraw->image == _dcraw_data->raw.image) && (_dcraw_data->raw.image) )
			free(_dcraw->image);
		_dcraw->image = _dcraw_data->raw.image = NULL;
		return E_IMAGERAW(IMAGERAW_DCRAW_OUTOFMEMORY_ERROR);
	}

	if (_dcraw->zero_is_bad) _dcraw->remove_zeroes();

	//! \todo убрал пока обработку bad_pixels, потом надо перепроверить и вернуть
	// _dcraw->bad_pixels(NULL);

	if (_dcraw->is_foveon)
	{
		_dcraw->foveon_interpolate();
		_dcraw_data->raw.width = _dcraw_data->width = _dcraw->width;
		_dcraw_data->raw.height = _dcraw_data->height = _dcraw->height;
	}

	fclose(_dcraw->ifp);
	_dcraw->ifp = _dcraw_data->ifp = NULL;

	_dcraw_data->ifp = NULL;
	_dcraw_data->rgbMax = _dcraw->maximum;
	_dcraw_data->black = _dcraw->black;
	dmin = DBL_MAX;

	for (i=0; i<_dcraw_data->colors; i++)
		if (dmin > _dcraw->pre_mul[i])
			dmin = _dcraw->pre_mul[i];

	if (!dmin)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: attempt to divizion by zero...\n"));
		if ((_dcraw->image == _dcraw_data->raw.image) && (_dcraw_data->raw.image) )
			free(_dcraw->image);
		_dcraw->image = _dcraw_data->raw.image = NULL;
		return E_FAIL;
	}

	for (i=0; i<_dcraw_data->colors; i++)
		_dcraw_data->pre_mul[i] = _dcraw->pre_mul[i]/dmin;

	if (_dcraw_data->colors==3)
		_dcraw_data->pre_mul[3] = 0;

	memcpy(_dcraw_data->cam_mul, _dcraw->cam_mul, sizeof(_dcraw->cam_mul));
	memcpy(_dcraw_data->rgb_cam, _dcraw->rgb_cam, sizeof(_dcraw->rgb_cam));

	double rgb_cam_transpose[4][3];
	for (i=0; i<4; i++) for (j=0; j<3; j++)
		rgb_cam_transpose[i][j] = _dcraw->rgb_cam[j][i];

	_dcraw->pseudoinverse (rgb_cam_transpose, _dcraw_data->cam_rgb, _dcraw->colors);

	//_dcraw_data->message = _dcraw->messageBuffer;
	//return _dcraw->lastStatus;
	return S_OK;
}

//! load thumbnail
HRESULT	c_dcraw_data::_load_thumb(dcraw_image_data *thumb)
{
	_RAW_TRACE(OLESTR("[c_dcraw_data::_load_thumb]\n"));
	if (!thumb)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: NULL pointer argument\n"));
		return E_POINTER;
	}

	dcraw_data * m_dcraw_data = & _m_dcraw_data;
	DCRaw *_dcraw = m_dcraw_data->GetDCRaw();

	if (!(_dcraw))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: internal object not initialized\n"));
		return E_POINTER;
	}

	thumb->height = _dcraw->thumb_height;
	thumb->width = _dcraw->thumb_width;
	m_dcraw_data->thumbOffset = _dcraw->thumb_offset;
	m_dcraw_data->thumbBufferLength = _dcraw->thumb_length;

	if (_dcraw->thumb_offset==0)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_WARNING,0,OLESTR("[imagerawcore]: image has no thumbnail\n"));
		_ERRORRET_(IMAGERAW_NOTHUMBNAIL_ERROR);
		//dcraw_message(d, DCRAW_ERROR,_("%s has no thumbnail."),	_dcraw->ifname_display);
	} 
	else if (_dcraw->thumb_load_raw!=NULL)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_WARNING,0,OLESTR("[imagerawcore]: unsupported thumb format\n"));
		_ERRORRET_(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR);
		//dcraw_message(d, DCRAW_ERROR,_("Unsupported thumb format (load_raw) for %s"),_dcraw->ifname_display);
	} 
	else if (_dcraw->write_thumb==&DCRaw::jpeg_thumb)
	{
		m_dcraw_data->thumbType = jpeg_thumb_type;
	}
	else if (_dcraw->write_thumb==&DCRaw::ppm_thumb)
	{
		m_dcraw_data->thumbType = ppm_thumb_type;
		m_dcraw_data->thumbBufferLength = thumb->width*thumb->height*3;
	}
	else
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_WARNING,0,OLESTR("[imagerawcore]: unsupported thumb format\n"));			
		_ERRORRET_(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR);
		//dcraw_message(d, DCRAW_ERROR,_("Unsupported thumb format for %s"), _dcraw->ifname_display);
	}
	return S_OK;
}



int c_dcraw_data::_finalize_shrink(dcraw_image_data *f, dcraw_data *dark, int scale)
{
	_RAW_TRACE(OLESTR("[c_dcraw_data::_finalize_shrink]\n"));
	dcraw_data * m_dcraw_data = &(this->_m_dcraw_data);
	DCRaw *_dcraw = _m_dcraw_data.GetDCRaw();
	_HR;

	if (!(_dcraw))
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: internal object not initialized\n"));
		return E_POINTER;
	}

	if (!f)
	{
		_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: null pointer (dcraw_image_data)\n"));
		return E_POINTER;		
	}


	int h, w, fujiWidth, r, c, ri, ci, cl, norm, s, recombine, pixels;
	int f4, sum[4], count[4];

	pixels = m_dcraw_data->raw.width * m_dcraw_data->raw.height;

	recombine = ( m_dcraw_data->colors==3 && m_dcraw_data->raw.colors==4 );
	f->colors = m_dcraw_data->colors;

	int black = dark ? DCRAW_MAX(m_dcraw_data->black - dark->black, 0) : m_dcraw_data->black;

	/* m_dcraw_data->raw.image is shrunk in half if there are filters.
	* If scale is odd we need to "unshrink" it using the info in
	* m_dcraw_data->fourColorFilters before scaling it. */
	if (m_dcraw_data->filters!=0 && scale%2==1)
	{
		/* I'm skiping the last row/column if it is not a full row/column */
		f->height = h = m_dcraw_data->height / scale;
		f->width = w = m_dcraw_data->width / scale;
		fujiWidth = m_dcraw_data->fuji_width / scale;
		f->image = (dcraw_image_type *)	g_realloc(f->image, h * w * sizeof(dcraw_image_type));
		f4 = m_dcraw_data->fourColorFilters;
		for(r=0; r<h; r++)
		{
			for(c=0; c<w; c++)
			{
				for (cl=0; cl<m_dcraw_data->raw.colors; cl++)
					sum[cl] = count[cl] = 0;

				for (ri=0; ri<scale; ri++)
				{
					for (ci=0; ci<scale; ci++)
					{
						cl = fc_INDI(f4, r*scale+ri, c*scale+ci);
						sum[cl] += get_pixel(m_dcraw_data, dark, r*scale+ri, c*scale+ci, cl, pixels);
						count[cl]++;
					}

				}

				for (cl=0; cl<m_dcraw_data->raw.colors; cl++)
					f->image[r*w+c][cl] = DCRAW_MAX(sum[cl]/count[cl] - black,0);

				if (recombine)
					f->image[r*w+c][1] = (f->image[r*w+c][1] + f->image[r*w+c][3])>>1;
			}
		}
	} 
	else 
	{
		if (m_dcraw_data->filters!=0)
			scale /= 2;
		/* I'm skiping the last row/column if it is not a full row/column */
		f->height = h = m_dcraw_data->raw.height / scale;
		f->width = w = m_dcraw_data->raw.width / scale;
		fujiWidth = ( (m_dcraw_data->fuji_width+m_dcraw_data->shrink) >> m_dcraw_data->shrink ) / scale;

		f->image = (dcraw_image_type *)g_realloc( f->image, h * w * sizeof(dcraw_image_type));

		norm = scale * scale;
		for(r=0; r<h; r++)
		{
			for(c=0; c<w; c++)
			{
				for (cl=0; cl<m_dcraw_data->raw.colors; cl++)
				{
					for (ri=0, s=0; ri<scale; ri++)
					{
						for (ci=0; ci<scale; ci++)
							s += get_pixel(m_dcraw_data, dark, r*scale+ri, c*scale+ci, cl, pixels);
					}
					f->image[r*w+c][cl] = DCRAW_MAX(s/norm - black,0);
				}
				if (recombine)
					f->image[r*w+c][1] =	(f->image[r*w+c][1] + f->image[r*w+c][3])>>1;
			}
		}
	}

	_CHECK_RET_(fuji_rotate_INDI(&f->image, &f->height,
		&f->width, &fujiWidth, f->colors, m_dcraw_data->fuji_step));

	return S_OK;
}


HRESULT c_dcraw_data::_set_color_scale(int useAutoWB, int useCameraWB)
{
	_RAW_TRACE(OLESTR("[c_dcraw_data::_set_color_scale]\n"));
	_HR;
	dcraw_data * m_dcraw_data = &_m_dcraw_data;
	DCRaw *m_dcraw = m_dcraw_data->GetDCRaw();
	memcpy(m_dcraw_data->post_mul, m_dcraw_data->pre_mul, sizeof m_dcraw_data->post_mul);
	if (!m_dcraw->is_foveon) /* foveon_interpolate() do this. */
		/* BUG white should not be global */
	{
		_CHECK_RET_(scale_colors_INDI(m_dcraw_data->raw.image,
			m_dcraw_data->rgbMax-m_dcraw_data->black, m_dcraw_data->black, useAutoWB, useCameraWB,
			m_dcraw_data->cam_mul, m_dcraw_data->raw.height, m_dcraw_data->raw.width, m_dcraw_data->raw.colors,
			m_dcraw_data->post_mul, m_dcraw_data->filters, m_dcraw->white, m_dcraw->ifname_display));
	}
	return S_OK;
}

HRESULT c_dcraw_data::_wavelet_denoise(float threshold)
{
	_RAW_TRACE(OLESTR("[c_dcraw_data::_wavelet_denoise]\n"));
	_HR;
	dcraw_data * m_dcraw_data = &_m_dcraw_data;
	DCRaw *m_dcraw = (DCRaw *)m_dcraw_data->GetDCRaw();
	memcpy(m_dcraw_data->post_mul, m_dcraw_data->pre_mul, sizeof m_dcraw_data->post_mul);
	if (threshold)
	{
		_CHECK_RET_(wavelet_denoise_INDI(m_dcraw_data->raw.image, m_dcraw_data->black, m_dcraw_data->raw.height,
			m_dcraw_data->raw.width, m_dcraw_data->height, m_dcraw_data->width, m_dcraw_data->raw.colors, m_dcraw_data->shrink,
			m_dcraw_data->post_mul, threshold, m_dcraw_data->filters));
	}
	return S_OK;
}

HRESULT c_dcraw_data::_wavelet_denoise_shrinked(dcraw_image_data *f,float threshold)
{
	_RAW_TRACE(OLESTR("[c_dcraw_data::_wavelet_denoise_shrinked]\n"));
	_HR;
	DCRaw *m_dcraw = _m_dcraw_data.GetDCRaw();
	if (threshold)
	{
		_CHECK_RET_(wavelet_denoise_INDI(f->image, 0, f->height, f->width, 0, 0, 4, 0,	NULL, threshold, 0));
	}
	return S_OK;
}

HRESULT c_dcraw_data::_finalize_interpolate(dcraw_image_data *f,dcraw_data *dark, int interpolation, int smoothing, int rgbWB[4])
{
	_RAW_TRACE(OLESTR("[c_dcraw_data::_finalize_interpolate]\n"));
	_HR;
	dcraw_data * m_dcraw_data = &_m_dcraw_data;
	DCRaw *m_dcraw = m_dcraw_data->GetDCRaw();
	int fujiWidth, i, r, c, cl, pixels;
	unsigned ff, f4;

	pixels = m_dcraw_data->raw.width * m_dcraw_data->raw.height;

	f->width	= m_dcraw_data->width;
	f->height	= m_dcraw_data->height;
	fujiWidth	= m_dcraw_data->fuji_width;
	f->colors	= m_dcraw_data->colors;
	f->image	= (dcraw_image_type *)	g_realloc(f->image, f->height * f->width * sizeof(dcraw_image_type));

	memset(f->image, 0, f->height * f->width * sizeof(dcraw_image_type));

	if (m_dcraw_data->filters==0)
		return E_FAIL;

	cl = m_dcraw_data->colors;
	if (interpolation == dcraw_four_color_interpolation || m_dcraw_data->colors == 4)
	{
		ff = m_dcraw_data->fourColorFilters;
		cl = 4;
		interpolation = dcraw_vng_interpolation;
	}
	else 
	{
		ff = m_dcraw_data->filters &= ~((m_dcraw_data->filters & 0x55555555) << 1);
	}

	int black = (dark ? DCRAW_MAX(m_dcraw_data->black - dark->black, 0) : m_dcraw_data->black);

	/* It might be better to report an error here: */
	/* (dcraw also forbids AHD for Fuji rotated images) */
	if ( interpolation==dcraw_ahd_interpolation && m_dcraw_data->colors > 3 )
		interpolation = dcraw_vng_interpolation;

	if ( interpolation==dcraw_ppg_interpolation && m_dcraw_data->colors > 3 )
		interpolation = dcraw_vng_interpolation;

	f4 = m_dcraw_data->fourColorFilters;

	if (m_dcraw_data->colors==3)
		rgbWB[3] = rgbWB[1];

	for(r=0; r<m_dcraw_data->height; r++)
	{
		for(c=0; c<m_dcraw_data->width; c++)
		{
			int cc = fc_INDI(f4,r,c);
			f->image[r*f->width+c][fc_INDI(ff,r,c)] = DCRAW_MIN( DCRAW_MAX( (gint64)
				(get_pixel(m_dcraw_data, dark, r/2, c/2, cc, pixels) - black) *
				rgbWB[cc]/0x10000, 0), 0xFFFF);
		}
	}
	int smoothPasses = 1;
	if (interpolation == dcraw_bilinear_interpolation)
	{
		_CHECK_RET_(lin_interpolate_INDI(f->image, ff, f->width, f->height, cl));
	}
#ifdef ENABLE_INTERP_NONE
	else if (interpolation==dcraw_none_interpolation)
		smoothing = 0;
#endif
	else if (interpolation==dcraw_vng_interpolation)
	{
		_CHECK_RET_(vng_interpolate_INDI(f->image, ff, f->width, f->height, cl /*,0xFFFF*/ ));
	}
	else if (interpolation==dcraw_ahd_interpolation)
	{
		_CHECK_RET_(ahd_interpolate_INDI(f->image, ff, f->width, f->height, cl,	m_dcraw_data->rgb_cam));
		smoothPasses = 3;
	} else if (interpolation==dcraw_ppg_interpolation)
	{
		_CHECK_RET_(ppg_interpolate_INDI(f->image, ff, f->width, f->height, cl));
	}


	if ( smoothing )
	{
		_CHECK_RET_(color_smooth(f->image, f->width, f->height, smoothPasses));
	}

	if (cl==4 && m_dcraw_data->colors == 3)
	{
		for (i=0; i<f->height*f->width; i++)
			f->image[i][1] = (f->image[i][1]+f->image[i][3])/2;
	}

	_CHECK_RET_(fuji_rotate_INDI(&f->image, &f->height, &f->width, &fujiWidth,
		f->colors, m_dcraw_data->fuji_step));

	return S_OK;
}

HRESULT c_dcraw_data::_get_thumbnail(void ** data,size_t * datasize)
{
	if (!data | !datasize)
		return E_POINTER;

	*data = NULL;
	*datasize = 0;
	return m_thumbnail.CopyTo(data,datasize);
}


