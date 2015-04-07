#ifdef _MSC_VER
	#pragma once			// for MSVS
#endif	// #ifdef _MSC_VER

#ifndef _MEDIAFORMATDEFINE_H_	// For Borland compiler
#define _MEDIAFORMATDEFINE_H_

//Media formats
#define MEDIAFORMAT_UNKNOWN				0

#define MEDIAFORMAT_AUDIO				100

#define MEDIAFORMAT_RMAUDIO				150
#define MEDIAFORMAT_WMA					151
#define MEDIAFORMAT_MPEG4AUDIO			152
#define MEDIAFORMAT_MPEG3AUDIO			153
#define MEDIAFORMAT_OGGVORBISAUDIO		154

#define MEDIAFORMAT_VIDEO				200
#define MEDIAFORMAT_VIDEO_UNCOMPRESSED	201
#define MEDIAFORMAT_VIDEO_COMPRESSED	202

#define MEDIAFORMAT_MPEG2				250
#define MEDIAFORMAT_H263				251
#define MEDIAFORMAT_MPEG4				252
#define MEDIAFORMAT_H264				253
#define MEDIAFORMAT_MPEGs				254
#define MEDIAFORMAT_WMV					255
#define MEDIAFORMAT_RMVIDEO				256
#define MEDIAFORMAT_EXTERNAL			257
#define MEDIAFORMAT_H264_EXT			258
#define MEDIAFORMAT_VPX					259

// Intel Media SDK: H.264, MPEG2, VC-1
#define MEDIAFORMAT_INTELMEDIA          260

#define MEDIAFORMAT_DVD_SUBPICTURE		300
#define MEDIAFORMAT_DVD_SUBPICTURE_COMPRESSED 301

#define MEDIAFORMAT_TEXT				350

//BluRay Program association table
#define MEDIAFORMAT_BLURAY_PAT							500
#define MEDIAFORMAT_BLURAY_PROGRAM_MAP					501
#define MEDIAFORMAT_BLURAY_CONDITIONAL_ACCESS_TABLE		502
#define MEDIAFORMAT_BLURAY_TEXT_SUBTITLE				503
#define MEDIAFORMAT_BLURAY_PG							504
#define MEDIAFORMAT_BLURAY_IG							505
#define MEDIAFORMAT_BLURAY_PCR							506

#define	MPEG4_DEFAULT_QUANT				400
#define	MPEG4_MAX_ZONES					64


//Color spaces
#define CSP_PLANAR   (1<< 0) /* 4:2:0 planar (==I420, except for pointers/strides) */
#define CSP_USER	  CSP_PLANAR
#define CSP_I420     (1<< 1) /* 4:2:0 planar */
#define CSP_YV12     (1<< 2) /* 4:2:0 planar */
#define CSP_YUY2     (1<< 3) /* 4:2:2 packed */
#define CSP_UYVY     (1<< 4) /* 4:2:2 packed */
#define CSP_YVYU     (1<< 5) /* 4:2:2 packed */
#define CSP_BGRA     (1<< 6) /* 32-bit bgra packed */
#define CSP_ABGR     (1<< 7) /* 32-bit abgr packed */
#define CSP_RGBA     (1<< 8) /* 32-bit rgba packed */
#define CSP_ARGB     (1<<15) /* 32-bit argb packed */
#define CSP_BGR      (1<< 9) /* 24-bit bgr packed */
#define CSP_RGB555   (1<<10) /* 16-bit rgb555 packed */
#define CSP_RGB565   (1<<11) /* 16-bit rgb565 packed */
#define CSP_SLICE    (1<<12) /* decoder only: 4:2:0 planar, per slice rendering */
#define CSP_INTERNAL (1<<13) /* decoder only: 4:2:0 planar, returns ptrs to internal buffers */
#define CSP_NULL     (1<<14) /* decoder only: dont output anything */
#define CSP_I422		 (1<<16) /* 4:2:2 planar. Add for MPEG2 Video*/
#define CSP_I444		 (1<<17) /* 4:4:4 planar. Add for MPEG2 Video*/
#define CSP_RGB8     (1<<18) /* 8-bit rgb packed */
#define CSP_RGB16     (1<<19) /* 16-bit rgb packed */
#define CSP_VFLIP    (1<<31) /* vertical flip mask */

#define CSP_COLOR_MASK	0x7FFFFFFF

// image formats
#define IMAGEFORMAT_BMP			1
#define IMAGEFORMAT_GIF			2
#define IMAGEFORMAT_JPE			3
#define IMAGEFORMAT_PNG			4
#define IMAGEFORMAT_TIF			5
#define IMAGEFORMAT_WMF			6
#define IMAGEFORMAT_EMF			7
#define IMAGEFORMAT_PCX			8
#define IMAGEFORMAT_TGA			9
#define IMAGEFORMAT_RAS			10


// RM specific
#define	ENCODINGTYPE_CBR						0
#define	ENCODINGTYPE_VBRBITRATE					1
#define	ENCODINGTYPE_VBRQUALITY					2
#define	ENCODINGTYPE_VBRUNCONSTRAINEDQUALITY	3
#define	ENCODINGTYPE_VBRUNCONSTRAINEDBITRATE	4

#define	ENCODINGCOMPLEXITY_LOW		0
#define	ENCODINGCOMPLEXITY_MEDIUM	1
#define	ENCODINGCOMPLEXITY_HIGH		2

#define VIDEOMODESHARP				0
#define VIDEOMODENORMAL				1
#define VIDEOMODESMOOTH				2
#define VIDEOMODESLIDESHOW			3

#define RMAUDIOMUSIC				0
#define RMAUDIOVOICE				1


#endif // #ifndef _MEDIAFORMATDEFINE_H_	// For Borland compiler
