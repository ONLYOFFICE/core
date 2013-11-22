/*! \file	dcrawwrapper.h
 *
 */

#pragma once

#include "ImageRawErrorDescription.h"

#include ".\dcraw\dcrawcommon.h"
#include ".\dcraw\imagerawcore.h"
#include ".\dcraw\dcraw.h"
#include "imagerawthumbnail.h"
#include "ImageRawOptions.h"

#define _FREE(M) do {if (NULL!=(M)) free(M); M=NULL;}while(0);

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
			_IMEGERAW_MESS_(_IMEGERAW_MESS_ERROR,0,OLESTR("[imagerawcore]: out of memory — cannot allocate DCRaw object\n"));
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


		// загружаем всегда в режиме shrink, потом в режиме preview оставляем как есть, иначе разворачиваем
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



	HRESULT _load()
	{
		_HR;

		DWORD loaded_size = 0;

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

		// сохраняем maxsaturation
		_prop_maximum = m_dcraw->maximum;

		// сохраняем отдельно загруженное изображение
		if (memcpy_s(this->m_loaded_image,loaded_size,m_dcraw->image,loaded_size))
		{
			_FREE(m_loaded_image)
			_FREE(m_dcraw->image);
			m_dcraw->__imagesize = m_loaded_image_size = 0;
			_FREE(m_dcraw->meta_data);	
			return E_IMAGERAW(IMAGERAW_MEMORY_ERROR);
		}
	
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
		int row, col, c;
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

		unsigned int filters = m_dcraw->filters;
		
		// не используем интерполяцию в режиме preview
		//m_dcraw->filters = 0;

		if (options->UseCustomWhiteBalance)
		{
			m_dcraw->user_mul[0] = options->CustomWhiteBalance.r;
			m_dcraw->user_mul[1] = options->CustomWhiteBalance.g1;
			m_dcraw->user_mul[2] = options->CustomWhiteBalance.b;
			m_dcraw->user_mul[3] = options->CustomWhiteBalance.g2;
		}

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

		if (options->UseCameraWhiteBalance)
		{
			m_dcraw->use_camera_wb = 1;
		}

		m_dcraw->maximum = _prop_maximum;

		m_dcraw->scale_colors();

		/*ushort (*img)[4] = (ushort (*)[4]) calloc (m_dcraw->iheight*m_dcraw->iwidth, sizeof *img);
		
		if (!img)
			return E_OUTOFMEMORY;*/

	/*	for (row=0; row < m_dcraw->iheight; row++)
		{
			for (col=0; col < m_dcraw->iwidth; col++)
			{
				c = m_dcraw->fc(row,col);
				img[row*m_dcraw->iwidth+col][c] = m_dcraw->image[(row >> 1)*m_dcraw->iwidth+(col >> 1)][c];
			}
		}
	*/

		//m_dcraw->mix_green = m_dcraw->four_color_rgb = 1;

		//if (m_dcraw->colors == 3)
		//	m_dcraw->colors = 4;
		
	/*	for (row = FC(1,0) >> 1; row < m_dcraw->iheight; row+=2)
			for (col = FC(row,1) & 1; col < m_dcraw->iwidth; col+=2)
				img[row*m_dcraw->iwidth+col][1] = img[row*m_dcraw->iwidth+col][3];
	*/
		
		
		/*
		free(m_dcraw->image);
		m_dcraw->image = img;
		*/

		for (/*colors=3,*/ int i=0; i < m_dcraw->iheight*m_dcraw->iwidth; i++)
			m_dcraw->image[i][1] = (m_dcraw->image[i][1] + m_dcraw->image[i][3]) >> 1;

		m_dcraw->width = m_dcraw->iwidth;
		m_dcraw->height = m_dcraw->iheight;
		m_dcraw->convert_to_rgb();
		m_dcraw->width = s_widht;
		m_dcraw->height = s_height;
		
		int soff, rstep, cstep;
		uchar lut[0x10000];
		m_dcraw->gamma_lut_ (lut,m_dcraw->iwidth,m_dcraw->iheight);

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

	unsigned int _prop_maximum;

	int flip_index (int row, int col,int iwidth, int iheight,int flip)
	{
		if (flip & 4) DCRAW_SWAP(row,col);
		if (flip & 2) row = iheight - 1 - row;
		if (flip & 1) col = iwidth  - 1 - col;
		return row * iwidth + col;
	}


};