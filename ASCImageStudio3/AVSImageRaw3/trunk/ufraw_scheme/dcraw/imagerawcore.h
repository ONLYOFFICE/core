/*!	\file	imagerewcore.h
 *	\brief	ќсновна€ функциональность RAW-обработки Ч оболочка над DCRaw и UFRaw
 *
 *	\author	Nikolay.Pogorskiy@avsmedia.net
 */
#pragma once

#include "atlcom.h"
#include "atlcoll.h"
#include "atlbase.h"
#include "comutil.h"

#include "dcrawcommon.h"
#include "dcraw.h"
#include "ufraw.h"
#include "dcraw_api.h"

#include <float.h>
#include <math.h>



#define _HR HRESULT hr


#define _IMEGERAW_MESS_ERROR	1
#define _IMEGERAW_MESS_WARNING	2
#define _IMEGERAW_MESS_TRACE	3
#define _IMEGERAW_MESS_INFO		4


#define _IMEGERAW_MESS_(param,mode,...) AtlTrace(__VA_ARGS__);


/*!
 *	”станавливаем значение переменной hr, в случае failed выдаем сообщение о возникшей ошибки и выпон€ем return hr
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
 *	”станавливаем значение переменной hr, в случае failed выдаем сообщение о возникшей ошибки и продолжаем выполнение
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



class CImageRawCore
{
protected:

	BSTR m_bsFileName; //!< raw file name


	DCRaw * m_dcraw;			//!< DCRaw object
	dcraw_data * m_dcraw_data;	//!< dcraw_data object



	/* conversion options */




protected:

	//! get image dimensions
	HRESULT _image_dimensions(int *height, int *width)
	{

		if (!m_dcraw_data || !m_dcraw)
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: internal object not initialized\n"));
			return E_POINTER;
		}

		dcraw_data * raw = m_dcraw_data;

		if (raw->fuji_width) {
			*width = (int)((raw->fuji_width - 1) / raw->fuji_step);
			*height = (int)((raw->height - raw->fuji_width + 1) / raw->fuji_step);
		} else {
			if (raw->pixel_aspect < 1)
				*height = (int)(raw->height / raw->pixel_aspect + 0.5);
			else
				*height = raw->height;
			if (raw->pixel_aspect > 1)
				*width = (int)(raw->width * raw->pixel_aspect + 0.5);
			else
				*width = raw->width;
		}

		if (m_dcraw->flip & 4)
		{
			int tmp = *height;
			*height = *width;
			*width = tmp;
		}
		
		return S_OK;
	}


	
	//! load raw image
	HRESULT _load_raw()
	{
		
		if (!m_dcraw || !m_dcraw_data)
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: internal object not initialized\n"));
			return E_POINTER;
		}

		if (!(m_dcraw_data->dcraw))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: internal object not initialized\n"));
			return E_POINTER;
		}

		dcraw_data * _dcraw_data = m_dcraw_data;
		DCRaw * _dcraw = (DCRaw *)_dcraw_data->dcraw;

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

		if (!(_dcraw->load_raw))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: load raw function not initialized\n"));
			if ((_dcraw->image == _dcraw_data->raw.image) && (_dcraw_data->raw.image) )
				free(_dcraw->image);
			_dcraw->image = _dcraw_data->raw.image = NULL;
			return	E_FAIL;
		}
		
		(_dcraw->*_dcraw->load_raw)();

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
	HRESULT	_load_thumb(dcraw_image_data *thumb)
	{
		if (!thumb)
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: NULL pointer argument\n"));
			return E_POINTER;
		}
		
		if (!m_dcraw || !m_dcraw_data)
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: internal object not initialized\n"));
			return E_POINTER;
		}

		if (!(m_dcraw_data->dcraw))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: internal object not initialized\n"));
			return E_POINTER;
		}

		DCRaw *_dcraw = (DCRaw *)m_dcraw_data->dcraw;

		//g_free(_dcraw->messageBuffer);
		//_dcraw->messageBuffer = NULL;
		//_dcraw->lastStatus = DCRAW_SUCCESS;

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
	
	//! create DCRaw objects
	HRESULT _dcraw_create()
	{
		_HR;
		_CHECK_HR_RET(Reset());
		if (!(m_dcraw = new DCRaw()))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч cannot allocate DCRaw object\n"));
			return	E_OUTOFMEMORY;
		}

		if (!(m_dcraw_data = new dcraw_data))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч cannot allocate dcraw_data object\n"));
			return	E_OUTOFMEMORY;
		}

		return S_OK;
	}

	
	//! free DCRaw objects
	HRESULT _dcraw_free()
	{
		if (m_dcraw)
		{
			if (m_dcraw->ifp)
			{
				fclose(m_dcraw->ifp);
				m_dcraw->ifp = NULL;
			}
			delete m_dcraw;
			m_dcraw = NULL;
		}

		if (m_dcraw_data)
		{
			delete m_dcraw_data;
			m_dcraw_data = NULL;
		}
		
		if (m_bsFileName)
			::SysFreeString(m_bsFileName);
		m_bsFileName = NULL;
		
		return S_OK;
	}

	
	//! open file
	HRESULT	_dcraw_open(BSTR bsFileName)
	{
		_HR;
		
		bool bWasException = false;

		if (!bsFileName)
			return E_INVALIDARG;

		_CHECK_HR_RET(_dcraw_create());
		
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
			_CHECK_HR(Reset());
			_ERRORRET_(IMAGERAW_OPENFILE_ERROR);	
		}

		try{
			bWasException = false;
			m_dcraw->identify();
		}
		catch(...)
		{
			bWasException = true;
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore][_dcraw_open]: dcraw external exception\n"));
			_CHECK_HR(Reset());
			_ERRORRET_(IMAGERAW_DCRAWINTERNALEXCEPTION_ERROR);
		}

		/* We first check if dcraw recognizes the file*/
		if (!(m_dcraw->make[0]))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore][_dcraw_open]: unsupported file format\n"));
			_CHECK_HR(Reset());
			_ERRORRET_(IMAGERAW_UNSUPPORTERDFORMAT_ERROR);
		}

		/* Next we check if dcraw can decode the file */
		if (!m_dcraw->is_raw)
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore][_dcraw_open]: cannot decode file\n"));
			_CHECK_HR(Reset());
			_ERRORRET_(IMAGERAW_DECODE_ERROR);
		}

		if (m_dcraw->load_raw == &DCRaw::kodak_ycbcr_load_raw)
		{
			m_dcraw->height += m_dcraw->height & 1;
			m_dcraw->width += m_dcraw->width & 1;
		}

		
		m_dcraw_data->dcraw = m_dcraw;
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

		switch ((m_dcraw->flip+3600) % 360) {
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



	
	HRESULT _process(BSTR bsFileName)
	{
		//int ufraw_convert_image(ufraw_data *uf)
		//{
		//	ufraw_developer_prepare(uf, file_developer);
		//	ufraw_convert_image_init(uf);
		//	ufraw_convert_image_first_phase(uf, TRUE);
		//	if (uf->ConvertShrink>1) {
		//	dcraw_data *raw = uf->raw;
		//	dcraw_image_data final;
		//	final.height = uf->image.height;
		//	final.width = uf->image.width;
		//	final.image = uf->image.image;
		//	/* Scale threshold according to shrink factor, as the effect of
		//	 * neighbouring pixels decays about exponentially with distance. */
		//	float threshold = uf->conf->threshold * exp(-(uf->ConvertShrink/2.0-1));
		//	dcraw_wavelet_denoise_shrinked(&final, raw, threshold);
		//	}
		//	return UFRAW_SUCCESS;
		//}		

	}
public:

	//! class constructor
	CImageRawCore():
					m_dcraw(NULL),
					m_bsFileName(NULL),
					m_dcraw_data(NULL)
	{
		_HR;
	}

	//! class destructor
	~CImageRawCore()
	{
		_HR;
	}

	HRESULT LoadFile(BSTR fileName)
	{
		_HR;
		_CHECK_HR_RET(_dcraw_open(fileName));
		return S_OK;
	}

	HRESULT Reset()
	{
		_HR;
		_CHECK_HR(_dcraw_free());
		return S_OK;
	}

};