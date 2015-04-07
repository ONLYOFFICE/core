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
#include "config.h"

#include <float.h>



#define _HR HRESULT hr
#define _FREE(M) do {if (NULL!=(M)) free(M); M=NULL;}while(0);

#define _ERRORRET_(CODE)	do{\
	return E_IMAGERAW(CODE);\
}while(0);

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



#define _IMEGERAW_MESS_ERROR	1
#define _IMEGERAW_MESS_WARNING	2
#define _IMEGERAW_MESS_TRACE	3
#define _IMEGERAW_MESS_INFO		4


#ifdef _DEBUG
#define _IMEGERAW_MESS_(param,mode,...) ATLTRACE2(__VA_ARGS__);
#else
#define _IMEGERAW_MESS_(param,mode,...)
#endif

#define _IMAGERAWFILE_TYPE FILE *
#define _IMAGERAWFILE_OPEN(file,name) _wfopen_s(&(file),name,OLESTR("rb"))


/*!	\brief	Основной класс-обертка над DCRaw
 *	
 */
class c_imageraw_core
{

	/*!	\brief Внутреннее исключение класса \a c_imageraw_core
	 * 
	 */
	class c_imageraw_internal_exception
	{
	protected:
		HRESULT hr;
	public:
		c_imageraw_internal_exception(HRESULT hr_ = E_UNEXPECTED)
		{
			hr = hr_;
		}
		HRESULT gethr()
		{
			return hr;
		}
	};
protected:
	//! Защищенный конструктор копирования объекта
	c_imageraw_core(const c_imageraw_core &){};

	//!	Защищенный оператор присваивания объекта
	c_imageraw_core & operator =(const c_imageraw_core &){};

public:
	
	//!	Конструктор класса
	c_imageraw_core()
	{
		m_isPPM = false;
		m_dcraw = NULL;
		m_loaded_image = NULL;
		m_loaded_image_size = 0;
	}

	//!	Деструктор класса
	~c_imageraw_core()
	{
		_close();
	}

	/*!
	 *	\brief		Открыть RAW-файл
	 *	\details	Функция открывает raw-файл, производит идентификацию
	 *	
	 *	\param[in]	bsFileName	Строка BSTR, с именем файла
	 *	\return		Значение HRESULT
	 */

	HRESULT	_open(BSTR bsFileName)
	{
		_HR;
		errno_t err = 0;

		if (FAILED(hr = _close()))
			return hr;

				
		if (!(m_dcraw = new DCRaw()))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory — cannot allocate DCRaw object\n"));
			return	E_OUTOFMEMORY;
		}

		
		try{
			if (0 != (err = _IMAGERAWFILE_OPEN(m_dcraw->ifp,(const wchar_t *)bsFileName)))
				throw c_imageraw_internal_exception(E_IMAGERAW(IMAGERAW_OPENFILE_ERROR));

			// на всякий случай проверяем действительно ли ненулевой handle
			if (!(m_dcraw->ifp))
				throw c_imageraw_internal_exception(E_IMAGERAW(IMAGERAW_OPENFILE_ERROR));
		}
		catch(c_imageraw_internal_exception ex)
		{
			if (err)
			{
				_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: cannot open file %s, errorno: %d\n"),(const wchar_t *)bsFileName,err);
			}
			else
			{
				_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: cannot open file %s\n"),(const wchar_t *)bsFileName,err);
			}
			_CHECK_HR(_close());
			return ex.gethr();
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

		//	We first check if dcraw recognizes the file
		if (!(m_dcraw->make[0]))
		{
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: unsupported file format\n"));
			_CHECK_HR(_close());
			_ERRORRET_(IMAGERAW_UNSUPPORTERDFORMAT_ERROR);
		}

		//	Next we check if dcraw can decode the file
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
			{
				if (!(m_dcraw->thumb_load_raw)) 
				{
					if (m_dcraw->ifp)
					{
						if (fseek(m_dcraw->ifp, m_dcraw->thumb_offset, SEEK_SET))
							break; // ошибка - неудачный seek, пропускаем загрузку thumbnail'а 
						
						char * thumbnail_data = 0;
						size_t thumbnail_size = 0;
						
						if (m_dcraw->write_thumb)
						{
							hr = (m_dcraw->*m_dcraw->write_thumb)((void **)&thumbnail_data, &thumbnail_size);
							if (SUCCEEDED(hr))
							{
								if (S_PPM_THUMBNAIL == hr)
									m_isPPM = true;
								else
									m_isPPM = false;
								m_thumbnail.CopyData(thumbnail_data,thumbnail_size);
							}
						}
						else
						{
						}
					}
				}
				else 
				{
					//thumb_load_raw
				}
			}else
			{
				// no thumbnail
			}
		}
		while(0);

		if (m_dcraw->ifp)
			fseek(m_dcraw->ifp,currentpos,SEEK_SET);

		m_dcraw->shrink = m_dcraw->filters && (!false && (m_dcraw->threshold || m_dcraw->aber[0] != 1 || m_dcraw->aber[2] != 1));
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



	/*!	\brief		Загрузка raw-изображения
	 *	\details	Производим непосредственную загрузку raw-данных из файла, закрываем файл
	 *				сохраняем raw-данные отдельно от DCRaw в m_loaded_image, сохраняем матрицы
	 *				\a _rgb_cam[3][4], \a _cam_rgb[4][3] (обратная к _rgb_cam), вектора \a _pre_mul[4], \a _cam_mul[4]
	 *				Также заполняем поля структуры options — границы значений, значения по умолчанию
	 *
	 *	\param	options	Опции raw-процессинга, заполняются при загрузке изображения	
	 *	\retrun	Значение HRESULT
	 */

	HRESULT _load(ImageRaw3SimpleModeOptions * options)
	{
		//_HR;
		DWORD loaded_size = 0;

        _ASSERTE(!!options);
		if (!options)
			return E_POINTER;
		
        _ASSERTE(!!m_dcraw);
		if (!m_dcraw)
			return  E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR); // файл не открыт (объект DCRaw не создан)

		_FREE(m_dcraw->raw_image);
		_FREE(m_dcraw->image);
		_FREE(m_loaded_image);
		m_loaded_image_size = 0;
		
		if (m_dcraw->filters || m_dcraw->colors == 1) 
		{
			loaded_size =(m_dcraw->raw_height+7)*m_dcraw->raw_width *2;
			m_dcraw->raw_image = (ushort *) calloc ((m_dcraw->raw_height+7)*m_dcraw->raw_width, 2);
     
			_ASSERTE(!!m_dcraw->raw_image);
		} 
		else 
		{
			loaded_size = m_dcraw->iheight*m_dcraw->iwidth * sizeof (*(m_dcraw->image));
			m_dcraw->image = (ushort (*)[4]) calloc (m_dcraw->iheight*m_dcraw->iwidth, sizeof (*(m_dcraw->image)));

			m_loaded_image = (ushort (*)[4]) calloc (m_dcraw->iheight*m_dcraw->iwidth, sizeof (*(m_dcraw->image)));			

			_ASSERTE(!!m_loaded_image);
			_ASSERTE(!!m_dcraw->image);
		}		

		if ((!m_dcraw->image && !m_dcraw->raw_image))
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
				_FREE(m_dcraw->raw_image);
				_FREE(m_loaded_image)
				_FREE(m_dcraw->image);
				m_dcraw->__imagesize = m_loaded_image_size = 0;
				return E_OUTOFMEMORY; 
			}
		}

		
		if (!m_dcraw->ifp) // ошибочный дискриптор файла
		{
            _ASSERTE(!!m_dcraw->ifp);
			
			_FREE(m_dcraw->raw_image);
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			
			m_dcraw->__imagesize = m_loaded_image_size = 0;
			_FREE(m_dcraw->meta_data);
			return  E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR); 
		}


		if (0 != fseek (m_dcraw->ifp, m_dcraw->data_offset, SEEK_SET))
		{
            _ASSERTE(false && "Failed fseek");
			_FREE(m_dcraw->raw_image);
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;
			_FREE(m_dcraw->meta_data);
			return E_IMAGERAW(IMAGERAW_UNEXPECTED_ERROR);
		}

		if (!(m_dcraw->load_raw)) // функция загрузки load_raw не определена
		{
            _ASSERTE(!!(m_dcraw->load_raw));
			_FREE(m_dcraw->raw_image);
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;
			_FREE(m_dcraw->meta_data);	
			return  E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR);
		}

		try
		{
			// загружаем данные из raw-файла
			(m_dcraw->*m_dcraw->load_raw)();
		}
		catch(...)
		{
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;
			_FREE(m_dcraw->meta_data);	
			return E_IMAGERAW(IMAGERAW_DCRAW_OUTOFMEMORY_ERROR);
		}

		m_dcraw->iheight = (m_dcraw->height + m_dcraw->shrink) >> m_dcraw->shrink;
		m_dcraw->iwidth  = (m_dcraw->width  + m_dcraw->shrink) >> m_dcraw->shrink;
		
		if (m_dcraw->raw_image)
		{
			loaded_size =  m_dcraw->iheight*m_dcraw->iwidth * sizeof (*(m_dcraw->image));
			
			m_dcraw->image = (ushort (*)[4]) calloc (m_dcraw->iheight*m_dcraw->iwidth, sizeof (*(m_dcraw->image)));

			m_loaded_image = (ushort (*)[4]) calloc (m_dcraw->iheight*m_dcraw->iwidth, sizeof (*(m_dcraw->image)));			

			_ASSERTE(!!m_loaded_image);
			_ASSERTE(!!m_dcraw->image);

			m_dcraw->crop_masked_pixels();

			_FREE(m_dcraw->raw_image);
		}
		m_dcraw->__imagesize = m_loaded_image_size =loaded_size;
	
		if ((!m_dcraw->image) || (!m_loaded_image))
		{
			return E_OUTOFMEMORY;
		}

		if (m_dcraw->zero_is_bad)
			m_dcraw->remove_zeroes();
		
		// опционально .. по настройке пользователя ... 
		//if (m_dcraw->dark_frame) m_dcraw->subtract (m_dcraw->dark_frame);
		
		options->InterpolationQuality = 2 + !m_dcraw->fuji_width;
		
		m_dcraw->use_camera_wb = 1;
		
		int i = m_dcraw->cblack[3],c;
		FORC3 if (i > m_dcraw->cblack[c]) i = m_dcraw->cblack[c];
		FORC4 m_dcraw->cblack[c] -= i;
		m_dcraw->black += i;
		FORC4 m_dcraw->cblack[c] += m_dcraw->black;

		if (m_dcraw->is_foveon) 
		{
			if (m_dcraw->load_raw == &DCRaw::foveon_dp_load_raw) 
			{
				for (i=0; i < m_dcraw->height*m_dcraw->width*4; i++)
				  if ((short) m_dcraw->image[0][i] < 0) 
					  m_dcraw->image[0][i] = 0;
			} else
			  m_dcraw->foveon_interpolate();
		} 

		else
			m_dcraw->scale_colors();

//////////////////////////////////////////////////////////		
		fclose(m_dcraw->ifp);
		m_dcraw->ifp = NULL;
////////////////////////////////////////////////////////////////////////////////////////		
		// сохраняем maxsaturation, black level
		_rgbMax		= m_dcraw->maximum;
		_black		= m_dcraw->black;

		// нормализуем множители каналов
		memcpy(_pre_mul,m_dcraw->pre_mul,sizeof _pre_mul);
		__normalize_chan_mul(_pre_mul,m_dcraw->colors);
		if (m_dcraw->colors==3)
			_pre_mul[3] = 0; 

		memcpy(_cam_mul, m_dcraw->cam_mul, sizeof _cam_mul);
		memcpy(_rgb_cam, m_dcraw->rgb_cam, sizeof _rgb_cam);

		double rgb_cam_transpose[4][3];
		for (int i=0; i<4; i++)
			for (int j=0; j<3; j++)
				rgb_cam_transpose[i][j] = m_dcraw->rgb_cam[j][i];
    
		m_dcraw->pseudoinverse (rgb_cam_transpose, _cam_rgb, m_dcraw->colors); 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// сохраняем отдельно загруженное изображение

		if (memcpy_s(m_loaded_image,loaded_size,m_dcraw->image,loaded_size))
		{
            _ASSERTE(false && "Failed memcpy_s");
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			_FREE(m_dcraw->raw_image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;
			_FREE(m_dcraw->meta_data);	
			return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);
		}

		// заполняем поля options

		options->UseDarknessLevel	= false;
		options->DarknessLevel		= m_dcraw->black;
		options->min_DarknessLevel	= 0;
		options->max_DarknessLevel	= m_dcraw->maximum;
		
		options->UseSaturationLevel		= false;
		// \todo	Определить какое максимальное значение Saturation Level считаем допустимым
		options->SaturationLevel		= m_dcraw->maximum;
		options->min_SaturationLevel	= 0;
		options->max_SaturationLevel	= m_dcraw->maximum;

		options->UseAdjustBrightness	= false;
		options->AdjustBrightness		= 1.0;
		options->max_AdjustBrightness	= OPT_MAX_ADJUST_BRIGHTNESS;
		options->min_AdjustBrightness	= OPT_MIN_ADJUST_BRIGHTNESS;

		options->UseCorrectChromaticAberration = false;

		options->CorrectChromaticAberration.min_r = OPT_MIN_CORRECT_CHROMATIC_ABERRATION;
		options->CorrectChromaticAberration.max_r = OPT_MAX_CORRECT_CHROMATIC_ABERRATION;
		options->CorrectChromaticAberration.r = 1.0;
		options->CorrectChromaticAberration.min_b = OPT_MIN_CORRECT_CHROMATIC_ABERRATION;
		options->CorrectChromaticAberration.max_b = OPT_MAX_CORRECT_CHROMATIC_ABERRATION;
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
		options->CustomWhiteBalance.max_r = OPT_MAX_MULTIPLIER;
		options->CustomWhiteBalance.min_r = OPT_MIN_MULTIPLIER;

		options->CustomWhiteBalance.max_g1 = OPT_MAX_MULTIPLIER;
		options->CustomWhiteBalance.min_g1 = OPT_MIN_MULTIPLIER;

		options->CustomWhiteBalance.max_b = OPT_MAX_MULTIPLIER;
		options->CustomWhiteBalance.min_b = OPT_MIN_MULTIPLIER;

		options->CustomWhiteBalance.max_g2 = OPT_MAX_MULTIPLIER;
		options->CustomWhiteBalance.min_g2 = OPT_MIN_MULTIPLIER;

		options->CustomWhiteBalance.r = _pre_mul[0];
		options->CustomWhiteBalance.g1 = _pre_mul[1];
		options->CustomWhiteBalance.b = _pre_mul[2];
		options->CustomWhiteBalance.g2 = _pre_mul[3];

		options->UseTemperature = false;

		__get_temperature(_pre_mul,m_dcraw->colors,options->Temperature.temperature,options->Temperature.green);

		
		//double rgbWB[3];
		//float chanMul[3];
		//memcpy(chanMul,m_dcraw->pre_mul,sizeof chanMul);
		//__normalize_chan_mul(chanMul,3);
		//for (int c=0; c<3; c++)
		//{
		//	rgbWB[c] = 0;
		//	for (int cc=0; cc < m_dcraw->colors; cc++)
		//		rgbWB[c] += _rgb_cam[c][cc] * _pre_mul[cc] /chanMul[cc];
		//}
		//ImageRaw3::RGB_to_Temperature(rgbWB, &(options->Temperature.temperature), &(options->Temperature.green));


		options->Temperature.max_green = OPT_MAX_GREEN;
		options->Temperature.min_green = OPT_MIN_GREEN;
		options->Temperature.max_temperature = OPT_MAX_TEMPERATURE;
		options->Temperature.min_temperature = OPT_MIN_TEMPERATURE;
		

		options->UseInterpolationQuality = false;
		options->InterpolationQuality = 0;

		options->UseDenoisingThreshold = false;
		options->DenoisingThreshold = OPT_MIN_DENOISING;
		options->max_DenoisingThreshold = OPT_MAX_DENOISING;
		
		options->UseHighlightMode = true;
		options->HighlightMode = 0;
		options->min_HighlightMode = 0;
		options->max_HighlightMode = ImageRaw3::highlightmodecount-1;

		options->max_MedianFilterPasses = OPT_MAX_MEDIANFILTER_PASSES;
		options->MedianFilterPasses = 0;

		return S_OK;
	}
	
	/*! \brief	Завершить работу с текущим raw-изображением
	 *
	 */
	HRESULT	_close()
	{
		if (m_dcraw)
		{
			if (m_dcraw->ifp)
				fclose(m_dcraw->ifp);
			m_dcraw->ifp = NULL;
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = 0;
			_FREE(m_dcraw->meta_data);	
			_FREE(m_dcraw->raw_image);
			delete m_dcraw;
		}
		m_dcraw = NULL;

		_FREE(m_loaded_image);
		m_loaded_image_size = 0;
		m_thumbnail.Reset();

		return S_OK;
	}

	/*!	\brief		Получить thumbnail
	 *	\details	Получаем thumbnail в виде участка памяти, содержащего графический файл некоторо формата
	 *
	 *	\param		data		Указатель на полученный участок памяти, освобождается на стороне клиента
	 *	\param		datasize	Размер в байтах
	 */
	HRESULT	_get_thumbnail(void ** data,size_t * datasize)
	{
        _ASSERTE(!!data);
        _ASSERTE(!!datasize);
		if (!data | !datasize)
			return E_POINTER;

		*data = NULL;
		*datasize = 0;
		return m_thumbnail.CopyTo(data,datasize);
	}

	HRESULT _is_ppm_thumbnail(bool * isPPM)
	{
        _ASSERTE(!!isPPM);
		if (!isPPM)
			return E_POINTER;

		*isPPM = m_isPPM;
        return S_OK;
	}


	/*!	\brief		Создать предпросмотр изображения
	 *	\details	Производим raw-процессинг изображения с выбранными настройками, заданными
	 *				параметром \a options. На выходе получаем изображение в виде байтового массива.
	 *				От полноценной raw-обработки отличается тем, что на выходе получаем изображение,
	 *				уменьшенное по каждой из осей в два раза, таким образом отпадает необходимость
	 *				производить байреовскую интерполяцию (см. http://en.wikipedia.org/wiki/Bayer_filter)
	 *				и процесс обработки происходит значительно быстрее.
	 *				После raw-обработки структура options может быть изменена, например выставлены новые значения
	 *				цветовой температуры
	 *
	 *	\param[in,out]	options		Опции raw-обработки
	 *	\param[out]		preview_im	Полученное изображение
	 *	\param[out]		nWidth		Ширина изображения
	 *	\param[out]		nHeight		Высота изображения
	 *	\param[out]		nColors		Число цветовых компонент
	 */
	HRESULT _get_preview(
						ImageRaw3SimpleModeOptions *	options,
						void **							preview_im,
						unsigned int *					nWidth,
						unsigned int *					nHeight,
						unsigned int *					nColors
						)
	{
        _ASSERTE(!!preview_im);
        _ASSERTE(!!options);
        _ASSERTE(!!nWidth);
        _ASSERTE(!!nHeight);
        _ASSERTE(!!nColors);

		int row, col, c,cc;
		if (!preview_im)
			return E_POINTER;
		
		*preview_im = NULL;
		if (!options || !nWidth || !nHeight || !nColors)
			return E_POINTER;

        _ASSERTE(!!m_loaded_image);
		if (!m_loaded_image)
			return E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR);

		//int s_width = m_dcraw->width, s_height = m_dcraw->height,s_shrink = m_dcraw->shrink;
		
		//if (options->UseInterpolationQuality == false)
		//{//half-size
		//	m_dcraw->shrink = 1; 
		//	m_dcraw->iheight = (m_dcraw->height + m_dcraw->shrink) >> m_dcraw->shrink;
		//	m_dcraw->iwidth  = (m_dcraw->width  + m_dcraw->shrink) >> m_dcraw->shrink;
		//}
		m_dcraw->width = m_dcraw->iwidth;
		m_dcraw->height = m_dcraw->iheight;

		_FREE(m_dcraw->raw_image);
		_FREE(m_dcraw->image);
		
		m_dcraw->__imagesize = m_loaded_image_size;

		{
			m_dcraw->image = (ushort (*)[4])malloc(m_loaded_image_size);
			if (memcpy_s(m_dcraw->image,m_dcraw->__imagesize,m_loaded_image,m_loaded_image_size))
			{
				_ASSERTE(false && "Failed memcpy_s");
				return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);
			}		
		}
		
        m_dcraw->use_gamma = 1;
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
		

		// если используем температуру явно, тогда значения перекрывают то что выставлено для CustomWhiteBalance
		//if (options->UseTemperature)
		//{
		//	double rgbWB[3];
		//	ImageRaw3::Temperature_to_RGB(options->Temperature.temperature, rgbWB);
		//	rgbWB[1] = rgbWB[1] / options->Temperature.green;
		//	__set_temperature(rgbWB);
		//	__normailze_user_mul();
		//}

		m_dcraw->black = _black;
        { // TODO ?
            ////unsigned int t = m_dcraw->cblack[3];
            ////for (int c = 0; c < 3; ++c)
            ////    if (t > m_dcraw->cblack[c])
            ////        t = m_dcraw->cblack[c];

            ////for (int c = 0; c < 4; ++c)
            ////    m_dcraw->cblack[c] -= t;
            ////
            ////m_dcraw->black += t;

			int i = m_dcraw->cblack[3];
			FORC3 if (i > m_dcraw->cblack[c]) i = m_dcraw->cblack[c];
			FORC4 m_dcraw->cblack[c] -= i;
			m_dcraw->black += i;
			FORC4 m_dcraw->cblack[c] += m_dcraw->black;

        }
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


		if (options->UseCustomWhiteBalance)
		{
			float __custom_mul[4];
			__custom_mul[0] = options->CustomWhiteBalance.r;
			__custom_mul[1] = options->CustomWhiteBalance.g1;
			__custom_mul[2] = options->CustomWhiteBalance.b;
			__custom_mul[3] = 0;//options->CustomWhiteBalance.g1;
			memcpy(m_dcraw->pre_mul,__custom_mul,sizeof(__custom_mul));
		}
		else if (options->UseTemperature)
		{
			double rgbWB[3];
			float __custom_mul[4];
			ImageRaw3::Temperature_to_RGB(options->Temperature.temperature,rgbWB);
			rgbWB[1] = rgbWB[1] / options->Temperature.green;

			for (c=0; c<m_dcraw->colors; c++)
			{
				double chanMulInv = 0;
				for (cc=0; cc<3; cc++)
					chanMulInv += 1/_pre_mul[c] * _cam_rgb[c][cc]	* rgbWB[cc];
				__custom_mul[c] = 1/chanMulInv;
			}

			__custom_mul[3] = __custom_mul[2];
			__normalize_chan_mul(__custom_mul,m_dcraw->colors);
			__custom_mul[3] = 0;
			memcpy(m_dcraw->pre_mul,__custom_mul,sizeof(__custom_mul));
		}
		else
		{
			memcpy(m_dcraw->pre_mul,_pre_mul,sizeof(_pre_mul));
		}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///****/
//		double rgbWB[3];
//		float chanMul[4];
//		memcpy(chanMul,m_dcraw->pre_mul,sizeof chanMul);
//
//		__normalize_chan_mul(chanMul,3);
//
//		__get_temperature(chanMul,m_dcraw->colors,options->Temperature.temperature,options->Temperature.green);
//
//		
//		for (int c=0; c<3; c++)
//		{
//			rgbWB[c] = 0;
//			for (int cc=0; cc < m_dcraw->colors; cc++)
//				rgbWB[c] += _rgb_cam[c][cc] * _pre_mul[cc] /chanMul[cc];
//		}
//		ImageRaw3::RGB_to_Temperature(rgbWB, &(options->Temperature.temperature), &(options->Temperature.green));
//		
///****/

		//options->CustomWhiteBalance.r = chanMul[0];
		//options->CustomWhiteBalance.g1 = chanMul[1];
		//options->CustomWhiteBalance.b = chanMul[2];
		//options->CustomWhiteBalance.g2 = chanMul[3];

		m_dcraw->width = m_dcraw->iwidth;
		m_dcraw->height = m_dcraw->iheight;
		
// options->UseInterpolationQuality ????
		m_dcraw->pre_interpolate(); 

		if (!options->UseInterpolationQuality)
			options->InterpolationQuality = 2 + !m_dcraw->fuji_width;
				
		if (m_dcraw->filters) 
		{
			if (options->InterpolationQuality == 0)
				m_dcraw->lin_interpolate();
			else if (options->InterpolationQuality == 1 || m_dcraw->colors > 3)
				m_dcraw->vng_interpolate();
			else if (options->InterpolationQuality == 2 && m_dcraw->filters > 1000)
				m_dcraw->ppg_interpolate();
			else if (m_dcraw->filters == 9)
				m_dcraw->xtrans_interpolate (options->InterpolationQuality*2-3);
			else
				m_dcraw->ahd_interpolate();
		}
		if (m_dcraw->mix_green)
		{
			long i=0;
			for (m_dcraw->colors=3, i=0; i < m_dcraw->height*m_dcraw->width; i++)
				m_dcraw->image[i][1] = (m_dcraw->image[i][1] + m_dcraw->image[i][3]) >> 1;
		}
		if (!m_dcraw->is_foveon && m_dcraw->colors == 3) 
			m_dcraw->median_filter();

		if (!m_dcraw->is_foveon && m_dcraw->highlight == 2) 
			m_dcraw->blend_highlights();

		if (!m_dcraw->is_foveon && m_dcraw->highlight > 2) 
			m_dcraw->recover_highlights();
		
		if (/*use_fuji_rotate*/true) m_dcraw->fuji_rotate();

		m_dcraw->convert_to_rgb();
		if (/*use_fuji_rotate*/true) m_dcraw->stretch();		
		
////////////////////////////////////////////////////
		long image_buffer_size = m_dcraw->iheight * m_dcraw->iwidth * 3;
		unsigned char * image_buffer = (unsigned char *)malloc( image_buffer_size);
			
		m_dcraw->output_image(image_buffer,image_buffer_size);

		*preview_im = (void*)image_buffer;

		*nWidth = m_dcraw->iwidth;
		*nHeight = m_dcraw->iheight;


		return S_OK;
	}

	DCRaw * _get_dcraw()
	{
		return m_dcraw;
	}

protected:
	CImageRaw3Thumbnail m_thumbnail;	
	DCRaw * m_dcraw;
	bool m_isPPM;



	ushort		(*m_loaded_image)[4];;
	unsigned int m_loaded_image_size;

	unsigned int _rgbMax;
	unsigned int _black;
	
	 double		_cam_rgb[4][3]; 
	 float		_pre_mul[4], _post_mul[4], _cam_mul[4], _rgb_cam[3][4]; 


	/*!	\brief		Нормализация вектора множителей для цветовых каналов
	 *	\details	Нормализуем вектор, так чтобы минимальное значение в нём стало равным 1.0
	 *	
	 *	\param[in,out]	chan_mul	Вектор цветовых множителей
	 *	\param[in]		colors		Количество используемых компонентов цвета
	 *	\exception		c_imageraw_internal_exception	Внутркнее исключение сигнализирующее о некорректном завершении функции
	 */
	void __normalize_chan_mul(float chan_mul[4],int colors) throw(c_imageraw_internal_exception)
	{
		int c;
		double min = chan_mul[0];
		for (c=1; c<colors; c++)
			if (chan_mul[c] < min)
				min = chan_mul[c];

		if (min==0.0)
        {
            _ASSERTE(0.0 != min);
			throw c_imageraw_internal_exception(E_UNEXPECTED);
        }
			/*
			min = 1.0; // should never happen, just to be safe
			*/
		for (c=0; c<colors; c++) 
			chan_mul[c] /= min;			
	}

		
	/*!	\brief		Вычислить значение цветовой температуры и уровня зеленого
	 *	\details	Вычисляем, используя заданные множители каналов \a chanMul и используем загруженные
	 *				значения множителей \a _pre_mul[4] и матрицу \a _rgb_cam[3][4]
	 *
	 *	\param[in]	chanMul		Заданные множители каналов, для которых вычисляем цветовую температуру
	 *	\param[in]	colors		Количество цветовых компонент
	 *	\param[out]	temperature	Цветовая температура
	 *	\param[out]	green		Уровень зеленого
	 */
	void __get_temperature(float chanMul[4],int colors,float & temperature, float &green)
	{
		double rgbWB[3];
		__normalize_chan_mul(chanMul,colors);
		for (int c=0; c<3; c++)
		{
			rgbWB[c] = 0;
			for (int cc=0; cc < m_dcraw->colors; cc++)
				rgbWB[c] += _rgb_cam[c][cc] * _pre_mul[cc] /chanMul[cc];
		}
		float temperature_,green_;
		ImageRaw3::RGB_to_Temperature(rgbWB, &temperature_, &green_);
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
