/*! \file	dcrawdata_core.h
 *	\brief	Функции-обертки для работы с dcraw и dcraw_data
 *
 *	\details	Структуры и функции из dcraw_api обернуты в классы, сделана обработка ошибок через HRESULT
 */

#pragma once

#include "atlcom.h"
#include "atlcoll.h"
#include "atlbase.h"
#include "comutil.h"

#include ".\dcraw\dcrawcommon.h"
#include ".\dcraw\dcraw.h"

#include "ImageRawErrorDescription.h"

#include <float.h>
#include <math.h>

#include "imagerawthumbnail.h"

#define _HR HRESULT hr


extern const double xyz_rgb[3][3];
extern const float d65_white[3];

#define _CHECK_RET_(HR) 	if (FAILED(hr = (HR))) { return hr;}

/*
	In order to inline this calculation, I make the risky
	assumption that all filter patterns can be described
	by a repeating pattern of eight rows and two columns

	Return values are either 0/1/2/3 = G/M/C/Y or 0/1/2/3 = R/G1/B/G2
*/


int fc_INDI (const unsigned filters, const int row, const int col);
static void hat_transform (float *temp, float *base, int st, int size, int sc);

HRESULT wavelet_denoise_INDI(ushort (*image)[4], const int black,
							 const int iheight, const int iwidth, const int height, const int width,
							 const int colors, const int shrink, float pre_mul[4],
							 const float threshold, const unsigned filters);

HRESULT	scale_colors_INDI(ushort (*image)[4], int maximum, const int black,
						  const int use_auto_wb, const int use_camera_wb, const float cam_mul[4],
						  const int iheight, const int iwidth, const int colors, float pre_mul[4],
						  const unsigned filters, /*const*/ ushort white[8][8],
						  const char *ifname_display);

HRESULT	border_interpolate_INDI(const int height, const int width,
								 ushort (*image)[4], const unsigned filters, int colors,
								 int border);

HRESULT  lin_interpolate_INDI(ushort (*image)[4], const unsigned filters,
							  const int width, const int height, const int colors);

HRESULT	vng_interpolate_INDI(ushort (*image)[4], const unsigned filters,
							  const int width, const int height, const int colors);

HRESULT	ppg_interpolate_INDI(ushort (*image)[4], const unsigned filters,
							 const int width, const int height, const int colors);

HRESULT ahd_interpolate_INDI(ushort (*image)[4], const unsigned filters,
							 const int width, const int height, const int colors,
							 const float rgb_cam[3][4]);

static inline int median9(int *p);
static inline ushort eahd_median(int row, int col, int color,ushort (*image)[4], const int width);
HRESULT color_smooth(ushort (*image)[4], const int width, const int height,const int passes);

HRESULT fuji_rotate_INDI(ushort (**image_p)[4], int *height_p,
						 int *width_p, int *fuji_width_p, const int colors, const double step);

HRESULT flip_image_INDI(ushort (*image)[4], int *height_p, int *width_p,
						/*const*/ int flip);

#define _IMEGERAW_MESS_ERROR	1
#define _IMEGERAW_MESS_WARNING	2
#define _IMEGERAW_MESS_TRACE	3
#define _IMEGERAW_MESS_INFO		4


#define _IMEGERAW_MESS_(param,mode,...) AtlTrace(__VA_ARGS__);


/*!
*	Устанавливаем значение переменной hr, в случае failed выдаем сообщение о возникшей ошибки и выпоняем return hr
*/
#define _CHECK_HR_RET(HR)	do {\
	if (FAILED(hr = (HR)))\
{\
	_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,\
	OLESTR("[imageraw]: failed hresult 0x%08lX at file %S, line %d\n"),hr,__FILE__,__LINE__);\
	return hr;\
}\
} while(0);

/*!
*	Устанавливаем значение переменной hr, в случае failed выдаем сообщение о возникшей ошибки и продолжаем выполнение
*/
#define _CHECK_HR(HR)	do {\
	if (FAILED(hr = (HR)))\
{\
	_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,\
	OLESTR("[imageraw]: failed hresult 0x%08lX at file %S, line %d\n"),hr,__FILE__,__LINE__);\
}\
} while(0);

#define _ERRORRET_(CODE)	do{\
	return E_IMAGERAW(CODE);\
}while(0);



#define _IMAGERAWFILE_TYPE FILE *
#define _IMAGERAWFILE_OPEN(file,name) _wfopen_s(&(file),name,OLESTR("rb"))

typedef guint16 dcraw_image_type[4];

struct __dcraw_image_data{
	dcraw_image_type *image;
	int width, height, colors;
};

typedef	__dcraw_image_data dcraw_image_data;

class __dcraw_data{

protected:
	DCRaw *dcraw;

public:
	FILE *ifp;
	int width, height, colors, fourColorFilters, filters, raw_color;
	int flip, shrink;
	double pixel_aspect;
	dcraw_image_data raw;
	dcraw_image_type thresholds;
	float pre_mul[4], post_mul[4], cam_mul[4], rgb_cam[3][4];
	double cam_rgb[4][3];
	int rgbMax, black, fuji_width;
	double fuji_step;
	int toneCurveSize, toneCurveOffset;
	int toneModeSize, toneModeOffset;

protected:
	char *message;

public:
	float iso_speed, shutter, aperture, focal_len;
	time_t timestamp;
	char make[80], model[80];
	int thumbType, thumbOffset, thumbBufferLength;

public:

	void SetDCRaw(DCRaw * _dcraw)
	{
		reset();
		dcraw = _dcraw;
	}

	DCRaw * GetDCRaw()
	{
		return dcraw;
	}

	__dcraw_data()
	{
		memset(this,0,sizeof(*this));
	}

	~__dcraw_data()
	{
		reset();
	}

	void reset()
	{
		if (dcraw)
			delete (dcraw);
		dcraw = NULL;

		if (ifp)
		{
			fclose(ifp);
			ifp = NULL;
		}

		if (raw.image)
		{
			free(raw.image);
			raw.image = NULL;
		}

		if (message)
			free(message);

		message = NULL;

		memset(this,0,sizeof(*this));
	}

};
typedef __dcraw_data	dcraw_data;

enum {
	dcraw_ahd_interpolation,
	dcraw_vng_interpolation,
	dcraw_four_color_interpolation,
	dcraw_ppg_interpolation,
	dcraw_bilinear_interpolation,
	dcraw_none_interpolation
};

enum {
	unknown_thumb_type,
	jpeg_thumb_type,
	ppm_thumb_type
};


HRESULT dcraw_image_resize(dcraw_image_data *image, int size);

HRESULT dcraw_image_stretch(dcraw_image_data *image, double pixel_aspect);

HRESULT dcraw_flip_image(dcraw_image_data *image, int flip);


/*!
*	Safed wrapper under dcraw_data structure
*/
class c_dcraw_data
{
protected:	
	BSTR m_bsFileName;
	dcraw_data _m_dcraw_data;
	CImageRaw3Thumbnail m_thumbnail;

public:
	c_dcraw_data();
	~c_dcraw_data();
	HRESULT _close();
	HRESULT	_open(BSTR bsFileName);
	HRESULT _image_dimensions(int flip, int *height, int *width);
	HRESULT _load_raw();
	//! load thumbnail
	HRESULT	_load_thumb(dcraw_image_data *thumb);
	static int get_dark_pixel(const dcraw_data *h, const dcraw_data *dark,int i, int cl)
	{
		return DCRAW_MAX(h->raw.image[i][cl] - dark->raw.image[i][cl], 0);
	}

	static int get_pixel(const dcraw_data *h, const dcraw_data *dark,int r, int c, int cl, int pixels)
	{
		int i = r * h->raw.width + c;
		int pixel = h->raw.image[i][cl];
		if (dark != 0) {
			int w = h->raw.width;
			pixel = (dark->raw.image[i][cl] <= dark->thresholds[cl])
				? DCRAW_MAX(pixel - dark->raw.image[i][cl], 0)
				: (get_dark_pixel(h, dark, i + ((i >= 1) ? -1 : 1), cl) +
				get_dark_pixel(h, dark, i + ((i < pixels-1) ? 1 : -1), cl) +
				get_dark_pixel(h, dark, i + ((i >= w) ? -w : w), cl) +
				get_dark_pixel(h, dark, i + ((i < pixels-w) ? w : -w), cl))
				/ 4;
		}
		return pixel;
	}

	int _finalize_shrink(dcraw_image_data *f, dcraw_data *dark, int scale);
	HRESULT _set_color_scale(int useAutoWB, int useCameraWB);
	HRESULT _wavelet_denoise(float threshold);
	HRESULT _wavelet_denoise_shrinked(dcraw_image_data *f,float threshold);
	HRESULT _finalize_interpolate(dcraw_image_data *f,	dcraw_data *dark, int interpolation, int smoothing, int rgbWB[4]);

	dcraw_data * GetDCRawData()
	{
		return &_m_dcraw_data;
	}

	HRESULT _get_thumbnail(void ** data,size_t * datasize);
};
