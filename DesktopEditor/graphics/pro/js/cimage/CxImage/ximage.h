#ifndef __ximage_h
#define __ximage_h

#include "xmemfile.h"
#include "ximadef.h"

enum ENUM_CXIMAGE_FORMATS
{
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
    char Version     [5];
    char CameraMake  [32];
    char CameraModel [40];
    char DateTime    [20];
    int32_t Height, Width;
    int32_t Orientation;
    int32_t IsColor;
    int32_t Process;
    int32_t FlashUsed;
    float FocalLength;
    float ExposureTime;
    float ApertureFNumber;
    float Distance;
    float CCDWidth;
    float ExposureBias;
    int32_t Whitebalance;
    int32_t MeteringMode;
    int32_t ExposureProgram;
    int32_t ISOequivalent;
    int32_t CompressionLevel;
    float FocalplaneXRes;
    float FocalplaneYRes;
    float FocalplaneUnits;
    float Xresolution;
    float Yresolution;
    float ResolutionUnit;
    float Brightness;
    char  Comments[MAX_COMMENT + 1];

    uint8_t* ThumbnailPointer;
    unsigned ThumbnailSize;

    bool IsExif;
} EXIFINFO;

#endif //CXIMAGE_SUPPORT_EXIF

class CxImage
{
    typedef struct tagCxImageInfo
    {
        uint32_t dwEffWidth;
        uint8_t* pImage;
        CxImage* pGhost;
        CxImage* pParent;
        uint32_t dwType;
        char     szLastError[256];
        int32_t  nProgress;
        int32_t  nEscape;
        int32_t  nBkgndIndex;
        RGBQUAD  nBkgndColor;
        float    fQuality;
        uint8_t  nJpegScale;
        int32_t  nFrame;
        int32_t  nNumFrames;
        uint32_t dwFrameDelay;
        int32_t  xDPI;
        int32_t  yDPI;
        RECT     rSelectionBox;
        uint8_t  nAlphaMax;
        bool     bAlphaPaletteEnabled;
        bool     bEnabled;
        int32_t  xOffset;
        int32_t  yOffset;
        uint32_t dwCodecOpt[CMAX_IMAGE_FORMATS];
        RGBQUAD  last_c;
        uint8_t  last_c_index;
        bool     last_c_isvalid;
        int32_t  nNumLayers;
        uint32_t dwFlags;
        uint8_t  dispmeth;
        bool     bGetAllFrames;
        bool     bLittleEndianHost;
    #if CXIMAGE_SUPPORT_EXIF
        EXIFINFO ExifInfo;
    #endif
    } CXIMAGEINFO;
public:
    CxImage(uint32_t imagetype = 0);
    virtual ~CxImage() { DestroyFrames(); Destroy(); };

    void* Create(uint32_t dwWidth, uint32_t dwHeight, uint32_t wBpp, uint32_t imagetype = 0);
    bool Destroy();
    bool DestroyFrames();
    bool Transfer(CxImage& from, bool bTransferFrames = true);

    uint32_t    GetSize();
    uint8_t*    GetBits(uint32_t row = 0);
    uint32_t    GetHeight() const;
    uint32_t    GetWidth() const;
    uint32_t    GetEffWidth() const;
    uint32_t    GetType() const;
    const char* GetLastError();

    uint8_t	GetJpegQuality() const;

    uint8_t	GetJpegScale() const;

    int32_t	GetXDPI() const;
    int32_t	GetYDPI() const;
    void SetXDPI(int32_t dpi);
    void SetYDPI(int32_t dpi);

    uint32_t GetCodecOption(uint32_t imagetype = 0);

    static uint32_t GetTypeIndexFromId(const uint32_t id);

    uint32_t GetPaletteSize();
    RGBQUAD* GetPalette() const;
    void     SetGrayPalette();
    void     SetPalette(uint32_t n, uint8_t* r, uint8_t* g, uint8_t* b);

#if CXIMAGE_SUPPORT_DECODE
    bool Decode(CxFile*  hFile,  uint32_t imagetype);
    bool Decode(uint8_t* buffer, uint32_t size, uint32_t imagetype);
#endif //CXIMAGE_SUPPORT_DECODE

#if CXIMAGE_SUPPORT_SELECTION
    bool SelectionDelete();
#endif //CXIMAGE_SUPPORT_SELECTION

#if CXIMAGE_SUPPORT_ALPHA
    void AlphaDelete();
#endif //CXIMAGE_SUPPORT_ALPHA

protected:
    void Startup(uint32_t imagetype = 0);
    void CopyInfo(const CxImage &src);
    void RGBtoBGR(uint8_t* buffer, int32_t length);

    void* pDib;
    void* pDibLimit;

    BITMAPINFOHEADER head;
    CXIMAGEINFO      info;
    uint8_t*         pSelection;
    uint8_t*         pAlpha;
    CxImage**        ppLayers;
    CxImage**        ppFrames;
};

#endif // __ximage_h
