/*!
 *	\file	ImageRawFileDesc.h
 *	\brief	—труктуры дл€ описани€ параметров изображени€, метаданных и т.д.
 *
 *	\author	Nikolay.Pogorskiy@avsmedia.net
 */

#pragma once

#include <time.h>

/*!
 *	\brief	—троковый тип дл€ описани€ изготовител€, модели камеры и владельца
 */
#define INFO_STRING1	char *

/*!
 *	\brief	“ип дл€ описани€ временной метки изображени€
 */
#define INFO_TIMESTAMP	time_t


/*!
 *	\struct	CImageRawFileDesc
 *	\brief	ќписание изображени€, метаданные
 */
struct CImageRawFileDesc
{
	INFO_TIMESTAMP	TimeStamp;			//!< Time stamp					
	INFO_STRING1	Make;				//!< Manufacturer				
	INFO_STRING1	Model;				//!< Model						
	INFO_STRING1	Owner;				//!< Camera owner				
	unsigned int	DNGVersion;			//!< DNG version				
	float			ISOSpeed;			//!< ISO speed					
	int				ShutterFormat;		//!< Shutter speed value format	
	float			Shutter;			//!< Shutter speed value
	float			ExposureTime;		//!< Exposure time
	float			Aperture;			//!< Aperture value
	float			FocalLength;		//!< Focal length
	double			PixelAspect;		//!< Pixel aspect ratio

	unsigned short	ThumbnailWidth;		//!< Thumbnail width
	unsigned short	ThumbnailHeight;	//!< Thumbnail height

	unsigned short	RawWidth;			//!< Raw width
	unsigned short	RawHeight;			//!< Raw height

	float			MaxApertureValue;	//!< MaxApertureValue
	unsigned char	Orientation;		//!< Orientation

	float			XResolution;		//!< XResolution
	float			YResolution;		//!< YResolution

	unsigned int	EXIFVersion;		//!< EXIF Version

	unsigned short	ImageWidth;			//!< Pixels per row
	unsigned short	ImageLength;		//!< The number of rows

/*	 TIFF Rev. 6.0 Attribute Information */
		
};