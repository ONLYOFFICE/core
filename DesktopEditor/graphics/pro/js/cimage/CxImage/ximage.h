#ifndef __ximage_h
#define __ximage_h

#include "xmemfile.h"
#include "ximadef.h"

enum ENUM_CXIMAGE_FORMATS{
CXIMAGE_FORMAT_UNKNOWN = 0,
#if CXIMAGE_SUPPORT_BMP
CXIMAGE_FORMAT_BMP = 1,
#endif
#if CXIMAGE_SUPPORT_GIF
CXIMAGE_FORMAT_GIF = 2,
#endif
#if CXIMAGE_SUPPORT_JPG
CXIMAGE_FORMAT_JPG = 3,
#endif
#if CXIMAGE_SUPPORT_PNG
CXIMAGE_FORMAT_PNG = 4,
#endif
#if CXIMAGE_SUPPORT_ICO
CXIMAGE_FORMAT_ICO = 5,
#endif
#if CXIMAGE_SUPPORT_TIF
CXIMAGE_FORMAT_TIF = 6,
#endif
#if CXIMAGE_SUPPORT_TGA
CXIMAGE_FORMAT_TGA = 7,
#endif
#if CXIMAGE_SUPPORT_PCX
CXIMAGE_FORMAT_PCX = 8,
#endif
#if CXIMAGE_SUPPORT_WBMP
CXIMAGE_FORMAT_WBMP = 9,
#endif
#if CXIMAGE_SUPPORT_WMF
CXIMAGE_FORMAT_WMF = 10,
#endif
#if CXIMAGE_SUPPORT_JP2
CXIMAGE_FORMAT_JP2 = 11,
#endif
#if CXIMAGE_SUPPORT_JPC
CXIMAGE_FORMAT_JPC = 12,
#endif
#if CXIMAGE_SUPPORT_PGX
CXIMAGE_FORMAT_PGX = 13,
#endif
#if CXIMAGE_SUPPORT_PNM
CXIMAGE_FORMAT_PNM = 14,
#endif
#if CXIMAGE_SUPPORT_RAS
CXIMAGE_FORMAT_RAS = 15,
#endif
#if CXIMAGE_SUPPORT_JBG
CXIMAGE_FORMAT_JBG = 16,
#endif
#if CXIMAGE_SUPPORT_MNG
CXIMAGE_FORMAT_MNG = 17,
#endif
#if CXIMAGE_SUPPORT_SKA
CXIMAGE_FORMAT_SKA = 18,
#endif
#if CXIMAGE_SUPPORT_RAW
CXIMAGE_FORMAT_RAW = 19,
#endif
#if CXIMAGE_SUPPORT_PSD
CXIMAGE_FORMAT_PSD = 20,
#endif
CMAX_IMAGE_FORMATS = CXIMAGE_SUPPORT_BMP + CXIMAGE_SUPPORT_GIF + CXIMAGE_SUPPORT_JPG +
                     CXIMAGE_SUPPORT_PNG + CXIMAGE_SUPPORT_MNG + CXIMAGE_SUPPORT_ICO +
                     CXIMAGE_SUPPORT_TIF + CXIMAGE_SUPPORT_TGA + CXIMAGE_SUPPORT_PCX +
                     CXIMAGE_SUPPORT_WBMP+ CXIMAGE_SUPPORT_WMF +
                     CXIMAGE_SUPPORT_JBG + CXIMAGE_SUPPORT_JP2 + CXIMAGE_SUPPORT_JPC +
                     CXIMAGE_SUPPORT_PGX + CXIMAGE_SUPPORT_PNM + CXIMAGE_SUPPORT_RAS +
                     CXIMAGE_SUPPORT_SKA + CXIMAGE_SUPPORT_RAW + CXIMAGE_SUPPORT_PSD + 1
};

#if CXIMAGE_SUPPORT_EXIF

#define MAX_COMMENT 255
#define MAX_SECTIONS 20

typedef struct tag_ExifInfo
{
    char  Version      [5];
    char  CameraMake   [32];
    char  CameraModel  [40];
    char  DateTime     [20];
    int32_t   Height, Width;
    int32_t   Orientation;
    int32_t   IsColor;
    int32_t   Process;
    int32_t   FlashUsed;
    float FocalLength;
    float ExposureTime;
    float ApertureFNumber;
    float Distance;
    float CCDWidth;
    float ExposureBias;
    int32_t   Whitebalance;
    int32_t   MeteringMode;
    int32_t   ExposureProgram;
    int32_t   ISOequivalent;
    int32_t   CompressionLevel;
    float FocalplaneXRes;
    float FocalplaneYRes;
    float FocalplaneUnits;
    float Xresolution;
    float Yresolution;
    float ResolutionUnit;
    float Brightness;
    char  Comments[MAX_COMMENT+1];

    uint8_t * ThumbnailPointer;  /* Pointer at the thumbnail */
    unsigned ThumbnailSize;     /* Size of thumbnail. */

    bool  IsExif;
} EXIFINFO;

#endif //CXIMAGE_SUPPORT_EXIF

class CxImage
{
    //extensible information collector
    typedef struct tagCxImageInfo
    {
        uint32_t	dwEffWidth;			///< uint32_t aligned scan line width
        uint8_t*	pImage;				///< THE IMAGE BITS
        CxImage* pGhost;			///< if this is a ghost, pGhost points to the body
        CxImage* pParent;			///< if this is a layer, pParent points to the body
        uint32_t	dwType;				///< original image format
        char	szLastError[256];	///< debugging
        int32_t	nProgress;			///< monitor
        int32_t	nEscape;			///< escape
        int32_t	nBkgndIndex;		///< used for GIF, PNG, MNG
        RGBQUAD nBkgndColor;		///< used for RGB transparency
        float	fQuality;			///< used for JPEG, JPEG2000 (0.0f ... 100.0f)
        uint8_t	nJpegScale;			///< used for JPEG [ignacio]
        int32_t	nFrame;				///< used for TIF, GIF, MNG : actual frame
        int32_t	nNumFrames;			///< used for TIF, GIF, MNG : total number of frames
        uint32_t	dwFrameDelay;		///< used for GIF, MNG
        int32_t	xDPI;				///< horizontal resolution
        int32_t	yDPI;				///< vertical resolution
        RECT	rSelectionBox;		///< bounding rectangle
        uint8_t	nAlphaMax;			///< max opacity (fade)
        bool	bAlphaPaletteEnabled; ///< true if alpha values in the palette are enabled.
        bool	bEnabled;			///< enables the painting functions
        int32_t	xOffset;
        int32_t	yOffset;
        uint32_t	dwCodecOpt[CMAX_IMAGE_FORMATS];	///< for GIF, TIF : 0=def.1=unc,2=fax3,3=fax4,4=pack,5=jpg
        RGBQUAD last_c;				///< for GetNearestIndex optimization
        uint8_t	last_c_index;
        bool	last_c_isvalid;
        int32_t	nNumLayers;
        uint32_t	dwFlags;			///< 0x??00000 = reserved, 0x00??0000 = blend mode, 0x0000???? = layer id - user flags
        uint8_t	dispmeth;
        bool	bGetAllFrames;
        bool	bLittleEndianHost;

    #if CXIMAGE_SUPPORT_EXIF
        EXIFINFO ExifInfo;
    #endif

    } CXIMAGEINFO;
public:
	CxImage(uint32_t imagetype = 0);
	virtual ~CxImage() { DestroyFrames(); Destroy(); };

	bool	Destroy();
	bool	DestroyFrames();
    bool	Transfer(CxImage &from, bool bTransferFrames = true);

    const char*	GetLastError();

    void	SetXDPI(int32_t dpi);
    void	SetYDPI(int32_t dpi);

#if CXIMAGE_SUPPORT_DECODE
    bool Decode(CxFile * hFile, uint32_t imagetype);
    bool Decode(uint8_t * buffer, uint32_t size, uint32_t imagetype);
#endif //CXIMAGE_SUPPORT_DECODE

protected:
    void Startup(uint32_t imagetype = 0);
    void CopyInfo(const CxImage &src);

	void*				pDib; //contains the header, the palette, the pixels
	void*				pDibLimit;

    BITMAPINFOHEADER    head; //standard header
	CXIMAGEINFO			info; //extended information
	uint8_t*			pSelection;	//selected region
	uint8_t*			pAlpha; //alpha channel
	CxImage**			ppLayers; //generic layers
	CxImage**			ppFrames;
};

#endif // __ximage_h
