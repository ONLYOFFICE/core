#if !defined(__ximaCFG_h)
#define __ximaCFG_h

/////////////////////////////////////////////////////////////////////////////
// CxImage supported features
#define CXIMAGE_SUPPORT_ALPHA          1
#define CXIMAGE_SUPPORT_SELECTION      1
#define CXIMAGE_SUPPORT_TRANSFORMATION 1
#define CXIMAGE_SUPPORT_DSP            1
#if defined(_WIN32) || defined (_WIN64)
#define CXIMAGE_SUPPORT_LAYERS		   1
#else
#define CXIMAGE_SUPPORT_LAYERS		   0
#endif
#define CXIMAGE_SUPPORT_INTERPOLATION  1

#define CXIMAGE_SUPPORT_DECODE	1
#define CXIMAGE_SUPPORT_ENCODE	1		//<vho><T.Peck>
//#define	CXIMAGE_SUPPORT_WINDOWS 1
#define	CXIMAGE_SUPPORT_EXIF    1

/////////////////////////////////////////////////////////////////////////////
// CxImage supported formats
#define CXIMAGE_SUPPORT_BMP 1
#define CXIMAGE_SUPPORT_GIF 1
#define CXIMAGE_SUPPORT_JPG 1
#define CXIMAGE_SUPPORT_PNG 1
#define CXIMAGE_SUPPORT_ICO 1
#define CXIMAGE_SUPPORT_TIF 1
#define CXIMAGE_SUPPORT_TGA 1
#define CXIMAGE_SUPPORT_PCX 1
#define CXIMAGE_SUPPORT_WBMP 1
#define CXIMAGE_SUPPORT_WMF 0

#define CXIMAGE_SUPPORT_JP2 1
#define CXIMAGE_SUPPORT_JPC 1
#define CXIMAGE_SUPPORT_PGX 1
#define CXIMAGE_SUPPORT_PNM 1
#define CXIMAGE_SUPPORT_RAS 1

#define CXIMAGE_SUPPORT_JBG 0		// GPL'd see ../jbig/copying.txt & ../jbig/patents.htm

#define CXIMAGE_SUPPORT_MNG 1
#define CXIMAGE_SUPPORT_SKA 1
#define CXIMAGE_SUPPORT_RAW 1
#define CXIMAGE_SUPPORT_PSD 1

#ifdef CXIMAGE_DISABLE_SUPPORT_MNG
#undef CXIMAGE_SUPPORT_MNG
#define CXIMAGE_SUPPORT_MNG 0
#endif

#ifdef CXIMAGE_DISABLE_SUPPORT_ADDITIONS
#undef CXIMAGE_SUPPORT_JP2
#undef CXIMAGE_SUPPORT_JPC
#undef CXIMAGE_SUPPORT_PGX
#undef CXIMAGE_SUPPORT_PNM
#undef CXIMAGE_SUPPORT_RAS
#undef CXIMAGE_SUPPORT_SKA
#undef CXIMAGE_SUPPORT_RAW
#undef CXIMAGE_SUPPORT_PSD

#define CXIMAGE_SUPPORT_JP2 0
#define CXIMAGE_SUPPORT_JPC 0
#define CXIMAGE_SUPPORT_PGX 0
#define CXIMAGE_SUPPORT_PNM 0
#define CXIMAGE_SUPPORT_RAS 0
#define CXIMAGE_SUPPORT_SKA 0
#define CXIMAGE_SUPPORT_RAW 0
#define CXIMAGE_SUPPORT_PSD 0
#endif

/////////////////////////////////////////////////////////////////////////////
#define	CXIMAGE_MAX_MEMORY 1073741824 // 1Gb

#define CXIMAGE_DEFAULT_DPI 96

#define CXIMAGE_ERR_NOFILE "null file handler"
#define CXIMAGE_ERR_NOIMAGE "null image!!!"

#if defined(BUILDING_WASM_MODULE)
#define CXIMAGE_SUPPORT_EXCEPTION_HANDLING 0
#else
#define CXIMAGE_SUPPORT_EXCEPTION_HANDLING 1
#endif // BUILDING_WASM_MODULE

/////////////////////////////////////////////////////////////////////////////
//color to grey mapping <H. Muelner> <jurgene>
//#define RGB2GRAY(r,g,b) (((b)*114 + (g)*587 + (r)*299)/1000)
#define RGB2GRAY(r,g,b) (((b)*117 + (g)*601 + (r)*306) >> 10)

#endif
