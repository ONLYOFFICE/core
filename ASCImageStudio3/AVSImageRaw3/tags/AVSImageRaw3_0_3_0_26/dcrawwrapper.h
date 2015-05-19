/*! \file	dcrawwrapper.h
 *
 */

#pragma once

#include "ImageRawErrorDescription.h"

#include ".\dcraw\dcrawcommon.h"
//#include ".\dcraw\imagerawcore.h"
#include ".\dcraw\dcraw.h"
#include "imagerawthumbnail.h"
#include "ImageRawOptions.h"
#include "ImageRawCommon.h"

#include "float.h"

#define _HR HRESULT hr
#define _FREE(M) do {if (NULL!=(M)) free(M); M=NULL;}while(0);

#define _ERRORRET_(CODE)	do{\
	return E_IMAGERAW(CODE);\
}while(0);

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



#define _IMEGERAW_MESS_ERROR	1
#define _IMEGERAW_MESS_WARNING	2
#define _IMEGERAW_MESS_TRACE	3
#define _IMEGERAW_MESS_INFO		4


#define _IMEGERAW_MESS_(param,mode,...) AtlTrace(__VA_ARGS__);

#define _IMAGERAWFILE_TYPE FILE *
#define _IMAGERAWFILE_OPEN(file,name) _wfopen_s(&(file),name,OLESTR("rb"))


class c_imageraw_core
{
protected:
	c_imageraw_core(const c_imageraw_core &){};
	c_imageraw_core & operator =(const c_imageraw_core &){};

public:
	c_imageraw_core()
	{
		m_dcraw = NULL;
		m_loaded_image = NULL;
		m_loaded_image_size = 0;
	}

	~c_imageraw_core()
	{
		_close();
	}

	HRESULT	_open(BSTR bsFileName)
	{
		_HR;
		errno_t err;

		if (FAILED(hr = _close()))
			return hr;

				
		if (!(m_dcraw = new DCRaw()))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory Ч cannot allocate DCRaw object\n"));
			return	E_OUTOFMEMORY;
		}

		if (0 != (err = _IMAGERAWFILE_OPEN(m_dcraw->ifp,(const wchar_t *)bsFileName)))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,
				OLESTR("[imagerawcore]: cannot open file %s, errorno: %d\n"),(const wchar_t *)bsFileName,err);

			_ERRORRET_(IMAGERAW_OPENFILE_ERROR);	
		}

		if (!(m_dcraw->ifp))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,
				OLESTR("[imagerawcore]: cannot open file %s\n"),(const wchar_t *)bsFileName,err);
			_CHECK_HR(_close());
			_ERRORRET_(IMAGERAW_OPENFILE_ERROR);	
		}		

		try{
			m_dcraw->identify();
		}
		catch(...)
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: dcraw external exception\n"));
			_CHECK_HR(_close());
			_ERRORRET_(IMAGERAW_DCRAWINTERNALEXCEPTION_ERROR);
		}

		/* We first check if dcraw recognizes the file*/
		if (!(m_dcraw->make[0]))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: unsupported file format\n"));
			_CHECK_HR(_close());
			_ERRORRET_(IMAGERAW_UNSUPPORTERDFORMAT_ERROR);
		}

		/* Next we check if dcraw can decode the file */
		if (!m_dcraw->is_raw)
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: cannot decode file\n"));
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
							break; // ошибка - неудачный seek, пропускаем загрузку thumbnail'а 
						
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


		// загружаем всегда в режиме shrink, потом в режиме preview оставл€ем как есть, иначе разворачиваем
		m_dcraw->shrink = (m_dcraw->filters!=0);
		m_dcraw->iheight = (m_dcraw->height + m_dcraw->shrink) >> m_dcraw->shrink;
		m_dcraw->iwidth  = (m_dcraw->width  + m_dcraw->shrink) >> m_dcraw->shrink;

		
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

		return S_OK;

	}



	HRESULT _load(ImageRaw3SimpleModeOptions * options)
	{
		_HR;

		DWORD loaded_size = 0;

		if (!options)
			return E_POINTER;

		if (!m_dcraw)
			return  E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR);

		if (m_dcraw->image)
			free(m_dcraw->image);
		
		m_dcraw->image = NULL;

		loaded_size = m_dcraw->iheight*m_dcraw->iwidth * sizeof (*(m_dcraw->image));

		m_dcraw->image = (ushort (*)[4]) calloc (m_dcraw->iheight*m_dcraw->iwidth, sizeof (*(m_dcraw->image)));
		this->m_loaded_image = (ushort (*)[4]) calloc (m_dcraw->iheight*m_dcraw->iwidth, sizeof (*(m_dcraw->image)));
			
		if (!m_dcraw->image || !this->m_loaded_image)
		{
			return E_OUTOFMEMORY;
		}

		m_dcraw->__imagesize = m_loaded_image_size = loaded_size;

		_FREE(m_dcraw->meta_data);

		if (m_dcraw->meta_length)
		{
			m_dcraw->meta_data = (char *) malloc (m_dcraw->meta_length);
			if (!m_dcraw->meta_data)
			{
				_FREE(m_loaded_image)
				_FREE(m_dcraw->image);
				m_dcraw->__imagesize = m_loaded_image_size = 0;
				return E_OUTOFMEMORY; 
			}
			
		}

		if (!m_dcraw->ifp)
		{
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;

			_FREE(m_dcraw->meta_data);
			return  E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR); 
		}

		if (0 != fseek (m_dcraw->ifp, m_dcraw->data_offset, SEEK_SET))
		{
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;

			_FREE(m_dcraw->meta_data);
			return E_IMAGERAW(IMAGERAW_UNEXPECTED_ERROR);
		}

		if (!(m_dcraw->load_raw))
		{
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;
			_FREE(m_dcraw->meta_data);	
			return  E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR);
		}

		try{	
			(m_dcraw->*m_dcraw->load_raw)();
		}catch(DCRawMemException ex)
		{
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;
			_FREE(m_dcraw->meta_data);	
			return E_IMAGERAW(IMAGERAW_DCRAW_OUTOFMEMORY_ERROR);
		}

		fclose(m_dcraw->ifp);
		m_dcraw->ifp = NULL;



		// сохран€ем maxsaturation
		_rgbMax = m_dcraw->maximum;
		_black = m_dcraw->black;

		// muls
		int i,j;
		double dmin = DBL_MAX; 
		for (i=0; i<m_dcraw->colors; i++)
			if (dmin > m_dcraw->pre_mul[i])
				dmin = m_dcraw->pre_mul[i];
		
		for (i=0; i<m_dcraw->colors; i++)
			_pre_mul[i] = m_dcraw->pre_mul[i]/dmin;

		if (m_dcraw->colors==3)
			_pre_mul[3] = 0; 

		memcpy(_cam_mul, m_dcraw->cam_mul, sizeof _cam_mul);
		memcpy(_rgb_cam, m_dcraw->rgb_cam, sizeof _rgb_cam);


		double rgb_cam_transpose[4][3];
		for (i=0; i<4; i++)
			for (j=0; j<3; j++)
				rgb_cam_transpose[i][j] = m_dcraw->rgb_cam[j][i];
    
		m_dcraw->pseudoinverse (rgb_cam_transpose, _cam_rgb, m_dcraw->colors); 

		// сохран€ем отдельно загруженное изображение
		if (memcpy_s(this->m_loaded_image,loaded_size,m_dcraw->image,loaded_size))
		{
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;
			_FREE(m_dcraw->meta_data);	
			return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);
		}

		// заполн€ем пол€ options

		options->UseDarknessLevel	= false;
		options->DarknessLevel		= m_dcraw->black;
		options->min_DarknessLevel	= 0;
		options->max_DarknessLevel	= m_dcraw->maximum;
		
		options->UseSaturationLevel		= false;
		options->SaturationLevel		= m_dcraw->maximum;
		options->min_SaturationLevel	= 0;
		options->max_SaturationLevel	= m_dcraw->maximum;

		options->UseAdjustBrightness	= false;
		options->AdjustBrightness		= 1.0;
		options->max_AdjustBrightness	= 5.0;
		options->min_AdjustBrightness	= 1.0/5.0;

		options->UseCorrectChromaticAberration = false;

		options->CorrectChromaticAberration.min_r = 0.999-0.0001*5;
		options->CorrectChromaticAberration.max_r = 1.001+0.0001*5;
		options->CorrectChromaticAberration.r = 1.0;
		options->CorrectChromaticAberration.min_b = 0.999-0.0001*5;
		options->CorrectChromaticAberration.max_b = 1.001+0.0001*5;
		options->CorrectChromaticAberration.b = 1.0;


		options->UseAverageGrayBox = false;
	
		options->AverageGrayBox.x	= 0;
		options->AverageGrayBox.y	= 0;
		options->AverageGrayBox.w	= m_dcraw->width;
		options->AverageGrayBox.h	= m_dcraw->height;
		
		options->AverageGrayBox.max_x	=	m_dcraw->width;
		options->AverageGrayBox.max_y	=	m_dcraw->height;

		options->AverageGrayBox.max_w	=	m_dcraw->width;
		options->AverageGrayBox.max_h	=	m_dcraw->height;

		//options->UseAutoWhiteBalance	=	

		options->UseCustomWhiteBalance = false;
		options->CustomWhiteBalance.max_r = 9.0;
		options->CustomWhiteBalance.min_r = 0.0;

		options->CustomWhiteBalance.max_g1 = 9.0;
		options->CustomWhiteBalance.min_g1 = 0.0;

		options->CustomWhiteBalance.max_b = 9.0;
		options->CustomWhiteBalance.min_b = 0.0;

		options->CustomWhiteBalance.max_g2 = 9.0;
		options->CustomWhiteBalance.min_g2 = 0.0;

		options->CustomWhiteBalance.r = m_dcraw->pre_mul[0];
		options->CustomWhiteBalance.g1 = m_dcraw->pre_mul[1];
		options->CustomWhiteBalance.b = m_dcraw->pre_mul[2];
		options->CustomWhiteBalance.g2 = m_dcraw->pre_mul[0]; //todo

		// todo
		options->UseTemperature = false;

		double rgbWB[3];
		float chanMul[3];
		memcpy(chanMul,m_dcraw->pre_mul,sizeof chanMul);
		__normalize_chan_mul(chanMul);
		for (int c=0; c<3; c++)
		{
			rgbWB[c] = 0;
			for (int cc=0; cc < m_dcraw->colors; cc++)
				rgbWB[c] += _rgb_cam[c][cc] * _pre_mul[cc] /chanMul[cc];
		}
		ImageRaw3::RGB_to_Temperature(rgbWB, &(options->Temperature.temperature), &(options->Temperature.green));

		options->Temperature.max_green = 2.5;
		options->Temperature.min_green = 0.2;
		options->Temperature.max_temperature = 15000;
		options->Temperature.min_temperature = 2000;
		

		options->UseInterpolationQuality = false;
		options->InterpolationQuality = 0;

		options->UseDenoisingThreshold = false;
		options->DenoisingThreshold = 0.0;
		options->max_DenoisingThreshold = 1000.0;

		
		options->UseHighlightMode = true;
		options->HighlightMode = 0;
		options->min_HighlightMode = 0;
		options->max_HighlightMode = 9;

		options->max_MedianFilterPasses = 10;
		options->MedianFilterPasses = 0;

		return S_OK;
	}
	
	HRESULT	_close()
	{
		if (m_dcraw)
		{
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = 0;
			_FREE(m_dcraw->meta_data);	
			delete m_dcraw;
		}
		m_dcraw = NULL;

		_FREE(m_loaded_image);

		m_loaded_image_size = 0;
		m_thumbnail.Reset();

		return S_OK;
	}

	HRESULT	_get_thumbnail(void ** data,size_t * datasize)
	{
		if (!data | !datasize)
			return E_POINTER;

		*data = NULL;
		*datasize = 0;
		return m_thumbnail.CopyTo(data,datasize);
	}


	HRESULT _get_preview(ImageRaw3SimpleModeOptions * options, void ** preview_im,
		unsigned int * nWidth, unsigned int * nHeight, unsigned int * nColors)
	{
		int row, col, c,cc;
		if (!preview_im)
			return E_POINTER;
		
		*preview_im = NULL;
		if (!options || !nWidth || !nHeight || !nColors)
			return E_POINTER;

		if (!m_loaded_image)
			return E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR);

		int s_widht = m_dcraw->width, s_height = m_dcraw->height;

//		m_dcraw->width = m_dcraw->iwidth;
//		m_dcraw->height = m_dcraw->iheight;

		_FREE(m_dcraw->image);
		m_dcraw->image = (ushort (*)[4])malloc(m_loaded_image_size);
		m_dcraw->__imagesize = m_loaded_image_size;

		// устанавливаем оригинальную загруженную картинку
		if (memcpy_s(m_dcraw->image,m_dcraw->__imagesize,m_loaded_image,m_loaded_image_size))
			return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);

		// сохран€ем фильтр
		unsigned int filters = m_dcraw->filters;
		
		// не используем интерпол€цию в режиме preview
		//m_dcraw->filters = 0;

		if (options->UseCustomWhiteBalance)
		{
			m_dcraw->user_mul[0] = options->CustomWhiteBalance.r;
			m_dcraw->user_mul[1] = options->CustomWhiteBalance.g1;
			m_dcraw->user_mul[2] = options->CustomWhiteBalance.b;
			m_dcraw->user_mul[3] = /*options->CustomWhiteBalance.g2*/ 0;
			__normailze_user_mul();
			__get_temperature(options->Temperature.temperature,	options->Temperature.green);
		}

		m_dcraw->use_auto_wb = 0;
		if (options->UseAutoWhiteBalance)
		{
			m_dcraw->use_auto_wb = 1;
			if (options->UseAverageGrayBox)
			{
				m_dcraw->greybox[0] = options->AverageGrayBox.x;
				m_dcraw->greybox[1] = options->AverageGrayBox.y;
				m_dcraw->greybox[2] = options->AverageGrayBox.w;
				m_dcraw->greybox[3] = options->AverageGrayBox.h;
			}
		}

		m_dcraw->use_camera_wb = options->UseCameraWhiteBalance;

		
		//	todo	Preset White Balance
		

		// если используем температуру €вно, тогда значени€ перекрывают то что выставлено дл€ CustomWhiteBalance
		//if (options->UseTemperature)
		//{
		//	double rgbWB[3];
		//	ImageRaw3::Temperature_to_RGB(options->Temperature.temperature, rgbWB);
		//	rgbWB[1] = rgbWB[1] / options->Temperature.green;
		//	__set_temperature(rgbWB);
		//	__normailze_user_mul();
		//}

		m_dcraw->black = _black;
		if (options->UseDarknessLevel)
		{
			m_dcraw->black = options->DarknessLevel;
		}

		m_dcraw->maximum = _rgbMax;
		if (options->UseSaturationLevel)
		{
			m_dcraw->maximum = options->SaturationLevel;
		}		

		m_dcraw->threshold = 0.0;
		if (options->UseDenoisingThreshold)
		{
			if (options->DenoisingThreshold<0)
				options->DenoisingThreshold = 0.0;
			m_dcraw->threshold = options->DenoisingThreshold;
		}

		m_dcraw->highlight = 0;
		if (options->UseHighlightMode)
		{
			if (options->HighlightMode<0)
				options->HighlightMode = 0;
			m_dcraw->highlight = options->HighlightMode;
		}

		m_dcraw->bright = 1.0;
		if (options->UseAdjustBrightness)
		{
			if (options->AdjustBrightness != 0.0)
				m_dcraw->bright = options->AdjustBrightness;
		}

		m_dcraw->med_passes = 0;
		if (options->UseMedianFilterPasses)
		{
			if (options->MedianFilterPasses<0)
				options->MedianFilterPasses = 0;
			m_dcraw->med_passes = options->MedianFilterPasses;
		}

		m_dcraw->no_auto_bright = false;
		if (options->NoAutoBrighten)
		{
			m_dcraw->no_auto_bright = true;
		}

		if (options->UseInterpolationQuality)
		{
			//  игнорируем в режиме preview
/*			if (options->InterpolationQuality>0)
			{
			
			}
*/
		}

		m_dcraw->aber[0] = m_dcraw->aber[1] = m_dcraw->aber[2] = m_dcraw->aber[3] = 1.0;
		if (options->UseCorrectChromaticAberration)
		{
			m_dcraw->aber[0] = 1.0/options->CorrectChromaticAberration.r;
			m_dcraw->aber[2] = 1.0/options->CorrectChromaticAberration.b;
		}

		if (options->UseHighlightMode)
			m_dcraw->highlight = options->HighlightMode;
		else
			m_dcraw->highlight = 0;

		memcpy(m_dcraw->pre_mul,_pre_mul,sizeof(_pre_mul));
		
		m_dcraw->scale_colors();

		/****/
		
		double rgbWB[3];
		float chanMul[3];
		memcpy(chanMul,m_dcraw->pre_mul,sizeof chanMul);
		__normalize_chan_mul(chanMul);
		for (int c=0; c<3; c++)
		{
			rgbWB[c] = 0;
			for (int cc=0; cc < m_dcraw->colors; cc++)
				rgbWB[c] += _rgb_cam[c][cc] * _pre_mul[cc] /chanMul[cc];
		}
		ImageRaw3::RGB_to_Temperature(rgbWB, &(options->Temperature.temperature), &(options->Temperature.green));
		
		/****/

		for (int i=0; i < m_dcraw->iheight*m_dcraw->iwidth; i++)
			m_dcraw->image[i][1] = (m_dcraw->image[i][1] + m_dcraw->image[i][3]) >> 1;

		m_dcraw->width = m_dcraw->iwidth;
		m_dcraw->height = m_dcraw->iheight;

		m_dcraw->median_filter();

		if (m_dcraw->highlight == 2)
			m_dcraw->blend_highlights();
		
		if (m_dcraw->highlight > 2)
			m_dcraw->recover_highlights();

		m_dcraw->convert_to_rgb();

		m_dcraw->width = s_widht;
		m_dcraw->height = s_height;
		
		int soff, rstep, cstep;
		uchar lut[0x10000];
		m_dcraw->gamma_lut_(lut,m_dcraw->iwidth,m_dcraw->iheight);

		unsigned int iwidth,iheight,width,height;

		iwidth = width = m_dcraw->iwidth;
		iheight = height = m_dcraw->iheight;
		
		if (m_dcraw->flip & 4)
			DCRAW_SWAP(height,width);

		soff  = flip_index (0, 0,iwidth,iheight,m_dcraw->flip);
		cstep = flip_index (0, 1,iwidth,iheight,m_dcraw->flip) - soff;
		rstep = flip_index (1, 0,iwidth,iheight,m_dcraw->flip) - flip_index (0, width,iwidth,iheight,m_dcraw->flip);

		unsigned char * ppm, *ppm1 = (unsigned char *)malloc(m_dcraw->iheight * m_dcraw->iwidth * 3);

		ppm = ppm1;
		for (row=0; row < height; row++, soff += rstep)
		{
			for (col=0; col < width; col++, soff += cstep)
			{
				for (c = 0; c<3; c++)
					ppm [col*3+2-c] = lut[m_dcraw->image[soff][c]];
			}
			ppm+=width*3;
		}

		*preview_im = (void*)ppm1;

		*nWidth = width;
		*nHeight = height;

		return S_OK;
	}

	DCRaw * _get_dcraw()
	{
		return m_dcraw;
	}

protected:
	CImageRaw3Thumbnail m_thumbnail;	
	DCRaw * m_dcraw;


	ushort		(*m_loaded_image)[4];
	unsigned int m_loaded_image_size;

	unsigned int _rgbMax;
	unsigned int _black;
	
	 double		_cam_rgb[4][3]; 
	 float		_pre_mul[4], _post_mul[4], _cam_mul[4], _rgb_cam[3][4]; 

	int flip_index (int row, int col,int iwidth, int iheight,int flip)
	{
		if (flip & 4) DCRAW_SWAP(row,col);
		if (flip & 2) row = iheight - 1 - row;
		if (flip & 1) col = iwidth  - 1 - col;
		return row * iwidth + col;
	}


	void __normailze_user_mul()
	{
		int c;
		double min = m_dcraw->user_mul[0];
		for (c=1; c<3; c++)
			if (m_dcraw->user_mul[c] < min)
				min = m_dcraw->user_mul[c];

		if (min==0.0)
			min = 1.0; /* should never happen, just to be safe */

		for (c=0; c<3; c++) 
			m_dcraw->user_mul[c] /= min;		
	}

	void __normalize_chan_mul(float chan_mul[3])
	{
		int c;
		double min = chan_mul[0];
		for (c=1; c<3; c++)
			if (chan_mul[c] < min)
				min = chan_mul[c];

		if (min==0.0)
			min = 1.0; /* should never happen, just to be safe */

		for (c=0; c<3; c++) 
			chan_mul[c] /= min;			
	}
		
	void __get_temperature(float & temperature, float &green)
	{
		int c,cc;
		double rgbWB[3]; 	
		for (c=0; c<3; c++)
		{
			rgbWB[c] = 0;
			for (cc=0; cc<3; cc++)
				rgbWB[c] += m_dcraw->rgb_cam[c][cc] * m_dcraw->pre_mul[cc]  / m_dcraw->user_mul[cc]; 
		}
		float temperature_,green_;
		ImageRaw3::RGB_to_Temperature(rgbWB,&temperature_,&green_);
		temperature = temperature_;
		green = green_;
	}

	void __set_temperature(double rgbWB[3])
	{
		int c,cc;
		for (c = 0;c<3;c++)
		{
			double chanMulInv = 0;
			for (cc=0; cc<3; cc++)
				chanMulInv += 1/ m_dcraw->pre_mul[c] * m_dcraw->rgb_cam[c][cc] * rgbWB[cc];
			m_dcraw->user_mul[c] = 1/chanMulInv; 				
		}
		m_dcraw->user_mul[3] = 0.0;
	}

};
