/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 * 
 * The Original Code is the Open Exif Toolkit,
 * released November 13, 2000.
 *
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 2001 Eastman Kodak Company. All 
 * Rights Reserved. 
 *
 * Portions of the Original Code are
 * Copyright (c) 1988-1996 Sam Leffler
 * Copyright (c) 1991-1996 Silicon Graphics, Inc.
 *
 * Creation Date: 07/14/2001
 *
 * Original Author: 
 * George Sotak george.sotak@kodak.com 
 *
 * Contributor(s): 
 * Ricardo Rosario ricardo.rosario@kodak.com
 * Sam Fryer       samuel.fryer@kodak.com
 */ 


#ifndef _EXIF_TYPE_DEFS_H
#define _EXIF_TYPE_DEFS_H

/*! \file */

#include "ExifDefs.h"
namespace Exif 
{
////////////////////////
//	WARNING DISABLING
////////////////////////
#ifdef _MSC_VER
#pragma warning (disable:4503)
#pragma warning (disable:4786)
#pragma warning (disable:4251)
#endif // _MSC_VER

/////////////////////////
// WINDDOW DLL STUFF
/////////////////////////

#ifdef WIN32

#if defined( EXIF_BUILD_DLL)
#define EXIF_DECL __declspec(dllexport)
#define EXIF_EXPLICIT 'e'
#elif  (defined( EXIF_DLL))
#define EXIF_DECL __declspec(dllimport)
#define EXIF_EXPLICIT 'i'
#define EXIF_TMPL_EXT extern
#endif // defined( EXIF_BUILD_DLL)

#pragma warning (disable:4251)
#pragma warning (disable:4661)
#pragma warning (disable:4786)

#endif // WIN32

#ifndef EXIF_DECL
#define EXIF_DECL
#endif

#ifndef EXIF_TMPL_EXT
#define EXIF_TMPL_EXT
#endif

///////////////////////////
// NAMESPACE 
///////////////////////////
#ifdef __mips
#define EXIF_USING_STD
#endif

#ifdef __SUNPRO_CC
#define EXIF_USING_STD
#endif

#ifdef __GNUC__
#ifdef EXIF_USE_NAMESPACE
#define EXIF_USING_STD namespace std { }; using namespace std;
#else
#define EXIF_USING_STD
#endif
#endif

#ifdef _WIN32_WCE
#define EXIF_USING_STD
#endif

#ifdef _MSC_VER
#if (_MSC_VER < 1100)
#define EXIF_USING_STD
#else
#define EXIF_USING_STD //namespace std { }; using namespace std;
#endif
#endif

#ifndef EXIF_USING_STD
#define EXIF_USING_STD namespace std { }; using namespace std;
#endif  // default

EXIF_USING_STD


/////////////////////////
// TYPES
/////////////////////////
#ifdef WIN32
#ifdef INTERNET_REQUIRED
typedef void* HINTERNET;
#endif // INTERNET_REQUIRED
#endif // INTERNET_REQUIRED

/*
 * Intrinsic data types required by the file format:
 *
 * 8-bit quantities	int8/uint8
 * 16-bit quantities	int16/uint16
 * 32-bit quantities	exif_int32/exif_uint32
 * strings		unsigned char*
 */
typedef	char int8;
typedef	unsigned char uint8;
typedef	short int16;
typedef	unsigned short uint16;	/* sizeof (uint16) must == 2 */
typedef	int exif_int32;
typedef	unsigned int exif_uint32;	/* sizeof (exif_uint32) must == 4 */


/*
 * The following typedefs define the intrinsic size of
 * data types used in the *exported* interfaces.  These
 * definitions depend on the proper definition of types
 * in tiff.h.  Note also that the varargs interface used
 * pass tag types and values uses the types defined in
 * tiff.h directly.
 *
 * NB: exiftag_t is unsigned int and not unsigned short because
 *     ANSI C requires that the type before the ellipsis be a
 *     promoted type (i.e. one of int, unsigned int, pointer,
 *     or double) and because we defined pseudo-tags that are
 *     outside the range of legal Aldus-assigned tags.
 * NB: tsize_t is exif_int32 and not exif_uint32 because some functions
 *     return -1.
 * NB: exifoff_t is not off_t for many reasons; EXIFs max out at
 *     32-bit file offsets being the most important
 */
typedef	exif_uint32 exiftag_t;	/* directory tag */
typedef	uint16 ttype_t;		    /* directory type */
typedef	uint16 tdir_t;		    /* directory index */
typedef	uint16 tsample_t;	    /* sample number */
typedef exif_uint32 exiftile_t;	/* tile number */
typedef	exif_int32 tsize_t;		/* i/o size in bytes */


//!i: IFD path is specified as a pair of values. The
//!i: first value is the directory's TIFF Tag, the
//!i: second value is the index of the directory
//!i: in the case where multiple instances of the
//!i: same directory tag exist at the same level. The
//!i: relationship is implied through the index of the
//!i: vector, i.e., ExifIFDPath[0] contains ExifIFDPath[1] contains
//!i: ExifIFDPath[2], etc.
// IFD0 = <0,0>
// IFD1 = <0,1>
//#include <pair.h>
}
#include <vector>
namespace Exif{
//typedef pair<exiftag_t, uint16> ExifIFDPair;
class ExifIFDPair
{
    public:
        ExifIFDPair(exiftag_t f, uint16 s):first(f),second(s){}
        ~ExifIFDPair(){}
        exiftag_t first;
        uint16 second;
};

typedef std::vector<ExifIFDPair> ExifIFDPath;
typedef ExifIFDPath::iterator ExifIFDPathIter;
typedef ExifIFDPath::value_type ExifIFDPathValType;

typedef uint16 ExifAppSegIdType ;

typedef	void* tdata_t;		/* image data ref */
typedef	exif_int32 exifoff_t;		/* file offset */


//! Data type enumeration for Exif Tags
typedef	enum {
    EXIF_NOTYPE	= 0,	    //!< placeholder
    EXIF_BYTE = 1,	    //!< 8-bit unsigned integer
    EXIF_ASCII = 2,	    //!< 8-bit bytes w/ last byte null
    EXIF_SHORT	= 3,	    //!< 16-bit unsigned integer
    EXIF_LONG = 4,	    //!< 32-bit unsigned integer
    EXIF_RATIONAL = 5,	    //!< 64-bit unsigned fraction
    EXIF_SBYTE = 6,	    //!< 8-bit signed integer
    EXIF_UNDEFINED = 7,     //!< 8-bit untyped data
    EXIF_SSHORT = 8,	    //!< 16-bit signed integer */
    EXIF_SLONG = 9,	    //!< 32-bit signed integer */
    EXIF_SRATIONAL = 10,     //!< 64-bit signed fraction */
    EXIF_FLOAT = 11,	    //!< 32-bit IEEE floating point */
    EXIF_DOUBLE	= 12,	    //!< 64-bit IEEE floating point */
    EXIF_MAXTYPE = 13	    // MAX +1 Valid Data Type Value */
} ExifDataType;

// Tag Location enum
typedef enum {
    EXIF_APP1_IFD0,
    EXIF_APP1_EXIFIFD,
    EXIF_APP1_IFD0_CAMERAINFO_IFD,
	EXIF_APP1_EXIFINTEROP_IFD,
	EXIF_APP1_IFD0_GPSINFO_IFD,
    EXIF_APP1_IFD1,
    EXIF_APP3_IFD0,
    EXIF_APP3_IFD0_SPECIALEFFECTS_IFD,
    EXIF_APP3_IFD0_BORDER_IFD,
    EXIF_APP3_DELTAIMAGE_ERIMM_IFD,
    EXIF_APP3_DELTAIMAGE_ROMM8_IFD,
    EXIF_APP3_DELTAIMAGE_ROMM12_IFD,
    EXIF_APP4_IFD0,
    EXIF_APP4_DELTAIMAGE_ERIMM_IFD,
    EXIF_APP4_DELTAIMAGE_ROMM8_IFD,
    EXIF_APP4_DELTAIMAGE_ROMM12_IFD,
	EXIF_APP5_KPRO1_IFD0,
	EXIF_APP5_KPRO2_IFD0,
    EXIF_GENERATE,
    UNDEFINED_LOCATION
} ExifTagLocation;

#if (defined _MSC_VER) && (defined WIN32) 
#pragma pack( push,2 )
#endif

//! Represents the contents of the Exif Tag Entry
typedef	struct
{
    uint16  tag;       /*!< The tag number */
    uint16  type;      /*!< Enumerated type for the data - see ExifDataType */
    exif_uint32  count;     /*!< Number of data bytes */
    exif_uint32  offset;    /*!< Data if count < 5, else offset to data segment where data is stored */
} ExifDirEntry;

//! Tiff header that is present at the beginning of Tiff-based application segments
typedef	struct 
{
	uint16	magic;	    /*!< magic number (defines byte order) */
	uint16	version;    /*!< Tiff version number */
	exif_uint32	ifdoff;	    /*!< byte offset to first directory */
} ExifTiffHeader;

#if (defined _MSC_VER) && (defined WIN32) 
#pragma pack( pop )
#endif


typedef struct {
	exif_uint32 width;
	exif_uint32 height;
} ExifJpegScaleDimension;	

typedef enum {
	JPEG_DEFAULT_TABLE = 0,
	JPEG_CUSTOM_TABLE_1 
} ExifJpegTableSelection;

typedef enum {
	JPEG_ZIG_ZAG = 0,
	JPEG_NATURAL
} ExifJpegTableOrder;

typedef enum {
	NIF_JPEG_SUBSAMPLE_422,
	NIF_JPEG_SUBSAMPLE_420
} ExifJpegSubSampleType;


typedef enum
{
    EXIF_DCT_SLOW_INT    = 0,
    EXIF_DCT_FAST_INT    = 1,
    EXIF_DCT_FLOAT       = 2
} ExifJpegDCTMethod;

//
//      A JPEG quantizer table
//      XXX Is it bigendian or little endian? JPEG byte order or host machine?
//      Probably should be in JPEG format.
//      Rem: unsigned short quantizer is large enough for non-baseline JPEG.
//      (16bit lossless)
//
typedef struct
{ 
        bool           is_in_JPEG_byte_order;  /* if FALSE, assumed to be in host byte order. */ 
        uint16         quantizer[64];      /* one per DCT coef.  */
} ExifJpegQuantTable;


//! Provides the values needed to choose between straight JPEG and full Exif
/*!
  OpenExif supports the creation, reading, and writing of both straight JPEG
  (i.e., without the metadata application segments) and full Exif. This
  enumeration provides the values necessary to communicate the format
  of the file being operated on.
*/
typedef enum
{
    NIF_FILE_JPEG,  /*!< plain JPEG file */
    NIF_FILE_EXIF   /*!< Exif file */
} ExifFileType;


//! Used to communicate Color Map information
typedef struct 
{
	uint16 numColors;   /*!< Number of colors */
	uint16 *colorMap0;  /*!< Zeroth channel of the color map */	
	uint16 *colorMap1;  /*!< First channel of the color map */	
	uint16 *colorMap2;  /*!< Second channel of the color map */	
} ExifJpegColorMap;

//! Communicates the color space of the image through ExifImageInfo
// NOTE: Only 3 Channel valid for Exif files
typedef enum {
    EXIF_YCbCr, /*!< YCbCr colorspace */
    EXIF_RGB,   /*!< RGB colorspace */
    EXIF_YCC,   /*!< YCC colorspace */
    EXIF_MONO,   /*!< Monochrome colorspace */
	JPEG_BGR,
    JPEG_RGBA,
	JPEG_RGBA_LEGACY,
	JPEG_YCbCrA,
	JPEG_YCbCrA_LEGACY,
	JPEG_YCCA,
	JPEG_YCCK,
	JPEG_CMYK,
	JPEG_GRAYSCALE,
	JPEG_UNKNOWN
} ExifColorSpace;

#define EXIF_MAX_COMPONENTS 4

class ExifComponentInfo
{
    public:
        uint8 hvSampling;
        uint8 quantizationDesignation;
        inline uint8 getHSampling()
            { return (hvSampling & 0xF0)>>4 ; };
        inline uint8 getVSampling()
            { return (hvSampling & 0x0F); };
        ExifComponentInfo() : hvSampling(0), quantizationDesignation(0){};
};

/*! Carries the basic container information for the image

    This has a subset of the information contained in ExifImageDesc. The
    purpose of this structure is to allow one to get / set the basic
    image information without having to get / set the image pixels.
*/
class ExifImageInfo
{
    public:
        exif_uint32 width;      /*!< width of image in pixels */
        exif_uint32 height;     /*!< height of image in pixels */
        uint16 numChannels; /*!< number of color channels (default 3) */ 
        uint16 precision;   /*!< bit depth of the pixels (default 8) */ 

        ExifColorSpace colorSpace; /*!< Colorspace of image (default EXIF_RGB)*/

        ExifComponentInfo components[EXIF_MAX_COMPONENTS];
        
        ExifImageInfo(void) 
            : width(0), height(0), numChannels(3),
              precision(8), colorSpace(EXIF_RGB) {}
};

//! Enumeration to indicate the color space component
typedef enum
{
    YCRCB_Y,   //!< Y component of YCrCb
    YCRCB_CB,  //!< Cb component of YCrCb
    YCRCB_CR,  //!< Cr component of YCrCb
    
    RGB_R,     //!< Red component of RGB
    RGB_G,     //!< Green component of RGB
    RGB_B,     //!< Blue component of RGB
 
    UNSPECIFIED_COLOR,
 
    ALPHA
} ExifComponentColor;

//! Enumeration to indicate the color components data type
typedef enum
{
    UNSIGNED_BYTE,
    SIGNED_BYTE,
    UNSIGNED_SHORT,
    SIGNED_SHORT,
    FLOAT_TYPE,
    DOUBLE_TYPE
} ExifComponentDataType;

//! Define the color/datatype of an image's component.
struct ExifComponentColorType
{
    ExifComponentColor myColor;      //  Y, C1, C2, R, B, G, or ALPHA
    ExifComponentDataType    myDataType;   //  unsigned byte, signed short, etc
    ExifComponentColorType(void)
        : myColor(UNSPECIFIED_COLOR),
          myDataType(UNSIGNED_BYTE)
      {}
};


//! Component of ExifImageDesc that holds the actual pixels.
struct ExifImageComponentDesc
{
    ExifComponentColorType myColorType;      //  color and datatype
    exif_uint32        horzSubSampFactor;//  horizontal subsampling
    exif_uint32        vertSubSampFactor;//  vertical subsampling
    exif_int32         columnStride;     //  items to next column on this row.
    exif_int32         lineStride;       //  items to next line in this column.
    uint8*        theData;          //  maybe void * XXX?
    ExifImageComponentDesc   (void)
        : horzSubSampFactor(1),
          vertSubSampFactor(1),
          columnStride(0),
          lineStride(0),
          theData(NULL)
        {}
};

//! Main handle to the image pixels
struct ExifImageDesc
{
    uint16   numberOfComponents;  //!< Number of (color) components
    exif_uint32   numberOfColumns;     //!< Number of columns (width of image)
    exif_uint32   numberOfRows;        //!< Number of rows (height of image)
    bool     buffersReadOnly;     //!< Currently not used
    ExifImageComponentDesc components[EXIF_MAX_COMPONENTS]; //!< The image's components (Four is the maximum allowed
    ExifImageDesc(void)
        : numberOfComponents(0),
          numberOfColumns(0),
          numberOfRows(0),
          buffersReadOnly(0)
        {}
};


// Additional Types for ExifIJGWrapper
typedef enum {
	JPEG_OK = 0,
	JPEG_ERROR,
	JPEG_COMPRESS_NOT_STARTED,
	JPEG_READHEADER_ERROR,
	JPEG_SOURCE_NOT_SET
} JpegStatus;

typedef enum {
	JPEG_INIT,
	JPEG_STARTED,
	JPEG_STOPPED
} JpegState;

typedef struct {
	unsigned short    quantizer[64];     /* one per DCT coef. */
} ExifJpegQTable;

typedef enum {
	JPEG_SCALE_NONE = 0,
	JPEG_SCALE_HALF,
	JPEG_SCALE_QUARTER,
	JPEG_SCALE_EIGHTH
} ExifJpegScaleFactor;

}
#endif // _EXIF_TYPE_DEFS_H
