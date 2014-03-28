#ifndef _BUILD_GRAPHICS_H_
#define _BUILD_GRAPHICS_H_

//#define _WINDOWS_GDIPLUS_USE_
#include "AggPlusEnums.h"

#include "../agg-2.4/include/agg_pixfmt_rgba.h"
#include "../agg-2.4/include/agg_conv_curve.h"
#include "../agg-2.4/include/agg_conv_stroke.h"
#include "../agg-2.4/include/agg_conv_dash.h"
#include "../agg-2.4/include/agg_span_gradient.h"
#include "../agg-2.4/include/agg_span_hatch.h"
#include "../agg-2.4/include/agg_span_interpolator_linear.h"
#include "../agg-2.4/include/agg_conv_transform.h"
#include "../agg-2.4/include/agg_span_image_filter_rgba.h"
#include "../agg-2.4/include/agg_span_pattern_rgba.h"
#include "../agg-2.4/include/agg_image_accessors.h"
#include "../agg-2.4/include/agg_span_allocator.h"
#include "../agg-2.4/include/agg_image_filters.h"

#include "../agg-2.4/include/agg_basics.h"

#include "../agg-2.4/include/agg_color_rgba.h"
#include "../agg-2.4/include/agg_pixfmt_rgba.h"
#include "../agg-2.4/include/agg_rasterizer_scanline_aa.h"
#include "../agg-2.4/include/agg_renderer_scanline.h"
#include "../agg-2.4/include/agg_scanline_u.h"

#include "../agg-2.4/include/agg_pixfmt_amask_adaptor.h"
#include "../agg-2.4/include/agg_scanline_boolean_algebra.h"

#include "../agg-2.4/svg/agg_svg_rasterizer.h"

#include "Color.h"
#include "Matrix.h"
#include "GraphicsPath.h"
#include "Clip.h"
#include "Brush.h"
#include "Image.h"

#ifdef WIN32
namespace NSStringFormat
{
	// printf-like formatting using variable arguments parameter
	static std::wstring FormatV(wchar_t* lpszFormat, va_list argList) 
	{
		std::wstring s;

		int sizeBuff = 512 * 2;
		wchar_t* szFormat = NULL;
		int write = -1;
		do 
		{
			szFormat = new wchar_t[sizeBuff];
			write = _vsnwprintf(szFormat, sizeBuff - 1, lpszFormat, argList);
			if (write >= 0) 
			{
				s = szFormat;
			}
			delete [] szFormat;
		} while (write < 0);

		return s;
	}
	static std::wstring Format(wchar_t* lpszFormat, ...) 
	{
		va_list arglist;
		va_start(arglist, lpszFormat);
		std::wstring s = FormatV(lpszFormat, arglist);
		va_end(arglist);
		return s;
	}
}
#endif

const double c_ag_Inch_to_MM					= 25.4;
const double c_ag_Inch_to_Point					= 72;

const long c_ag_CombineModeReplace				= 0;
const long c_ag_CombineModeIntersect			= 1;
const long c_ag_CombineModeUnion				= 2;
const long c_ag_CombineModeXor					= 3;
const long c_ag_CombineModeExclude				= 4;
const long c_ag_CombineModeComplement			= 5;

const long c_ag_PageUnitPixel					= 0;
const long c_ag_PageUnitPoint					= 1;
const long c_ag_PageUnitInch					= 2;
const long c_ag_PageUnitMillimeter				= 3;

namespace Aggplus
{
typedef agg::rendering_buffer rendering_buffer_type;
typedef agg::pixfmt_bgra32 pixformat_type;

typedef agg::blender_rgba< agg::svg::color_type, agg::svg::component_order >						blender_type;
typedef agg::pixfmt_alpha_blend_rgba< blender_type, agg::rendering_buffer, agg::svg::pixel_type >	pixfmt_type;

typedef agg::renderer_base<pixfmt_type> base_renderer_type;
typedef agg::scanline_u8 scanline_type;

typedef agg::span_interpolator_linear<> interpolator_type_linear;
typedef agg::span_allocator<agg::rgba8> span_alloc_type;

#define def_flag_G_ExtBuffer 0x0001
#define def_flag_G_FromHDC   0x0002


// Расстояние от центра до контрольных точек Безье для аппроксимации окружности = (4 * (sqrt(2) - 1) / 3) * r
static const double c_dKappa   = ((double)0.55228475);
static const double c_dKappa_2 = ((double)(0.5 * 0.55228475));

// Делим 16-битное значение [0, 255*255] на 255, возвращаем 8-битное значение.
static inline unsigned char Div255(int nValue)
{
	return (unsigned char)((nValue + (nValue >> 8) + 0x80) >> 8);
}

//-------------------------------------------------------------------------------------------------------------------------------

class CDIB : public IGrObject
{
public:
	BYTE* m_pBits;
	LONG m_lWidth;
	LONG m_lHeight;

public:
	CDIB() : IGrObject()
	{
		m_pBits = NULL;
		m_lWidth = 0;
		m_lHeight = 0;
	}
	virtual ~CDIB()
	{
		// delete all in system wrapper
	}

	virtual BOOL Create(LONG lWidth, LONG lHeight, double dDPIX, double dDPIY) = 0;
};


class CGraphics
{
	friend class CGlyphImage;
	friend class CMatrix;
	friend class CGraphicsPath;
	friend class CColor;
	friend class CBrush;
	friend class CBrushLinearGradient;
	friend class CBrushTexture;
	friend class CImage;
	friend class CGraphicsRenderer;

protected:
	BYTE* m_pPixels;
		
	DWORD	m_dwConfigFlags;
	
	double	m_dWidthPix;
	double	m_dHeightPix;

	double	m_dDpiX;
	double	m_dDpiY;

	Unit	m_ePageUnits;

	CMatrix m_oCoordTransform;
	CMatrix m_oBaseTransform;
	CMatrix m_oTransform;	

	CMatrix	m_oFullTransform;

	double m_dClipLeft;
	double m_dClipTop;
	double m_dClipWidth;
	double m_dClipHeight;

	CClipMulti  m_oClip;

	agg::svg::frame_buffer_rgba       m_frame_buffer;
	agg::svg::rasterizer              m_rasterizer;
	
#ifdef _WINDOW_GRAPHIS_USE_
	// для отрисовки картинок - используем Gdiplus
	Gdiplus::Graphics*		m_pGraphics;
	Gdiplus::Bitmap*		m_pBitmap;

	CGdiPlusInit			m_oInitGdiplus;	
#endif

	CDIB*					m_pDib;

public:
	agg::svg::frame_buffer_rgba&   get_frame_buffer();
	agg::svg::rasterizer&          get_rasterizer();

	bool	m_bIntegerGrid;
	double	m_dGlobalAlpha;

public:

	CGraphics();
	CGraphics(int dwWidth, int dwHeight, int stride, BYTE* pBuffer);
	CGraphics(CImage* pImage);
	~CGraphics();

	BOOL IsDib();

	Status Create(BYTE* pPixels, int lWidth, int lHeight, int lStride, LONG lPitch);
	Status Create2(BYTE* pPixels, int lWidth, int lHeight, int lStride, LONG lPitch, LONG x, LONG y, LONG w, LONG h, double dW, double dH, CDIB* pDib = NULL);
	Status CreatePart(LONG lLeft, LONG lTop, LONG lWidth, LONG lHeight, CGraphics** ppPart);

	double GetDpiX();
	double GetDpiY();
	Status SetDpiX(double dDpiX);
	Status SetDpiY(double dDpiY);
	
	Status SetPageWidth(double lWidth, LONG lUnit);
	Status SetPageHeight(double lHeight, LONG lUnit);
	
	Unit GetPageUnit();
	Status SetPageUnit(Unit lUnits);
	
	CMatrix* GetTransform();
	Status SetTransform(CMatrix* pTransform);
	CMatrix* GetBaseTransform();
	Status SetBaseTransform(CMatrix* pTransform);	
	Status TranslateTransform(double x, double y, MatrixOrder order = MatrixOrderPrepend);
	Status RotateTransform(double dAngle, MatrixOrder order = MatrixOrderPrepend);
	Status ScaleTransform(double dScaleX, double dScaleY, MatrixOrder order = MatrixOrderPrepend);
	Status ShearTransform(double shearX, double shearY, MatrixOrder order = MatrixOrderPrepend);
	Status MultiplyTransform(CMatrix* pMatrix, MatrixOrder order = MatrixOrderPrepend);
	
	// функции отсечения
	Status SetClipRect(double dLeft, double dTop, double dWidth, double dHeight);
	Status SetClipRect2(double dLeft, double dTop, double dWidth, double dHeight);
	Status SetClipRect3(double dLeft, double dTop, double dWidth, double dHeight);

	Status SetClip(CGraphicsPath* pPath);
	Status ResetClip();
	Status ExclugeClip(CGraphicsPath* pPath);
	Status CombineClip(CGraphicsPath* pPath, agg::sbool_op_e op);

	// измерение текста
	BOOL MeasureString(const std::wstring& strText, CFontManager* pManager, double* lWidth, double* lHeight);
	Status Clear(CColor oColor);

	Status DrawArc(NSStructures::CPen* pPen, double x, double y, double width, double height, double startAngle, double sweepAngle);
	Status DrawBezier(NSStructures::CPen* pPen, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	Status DrawBeziers(NSStructures::CPen* pPen, double* pPoints, LONG lCount);
	Status DrawCurve(NSStructures::CPen* pPen, double* pPoints, LONG lCount);
	Status DrawLine(NSStructures::CPen* pPen, double x1, double y1, double x2, double y2);
    Status DrawLines(NSStructures::CPen* pPen, double* pPoints, LONG lCount);
	Status DrawRectangle(NSStructures::CPen* pPen, double x, double y, double width, double height);
	Status DrawEllipse(NSStructures::CPen* pPen, double x, double y, double width, double height);
	Status DrawPath(NSStructures::CPen* pPen, CGraphicsPath* pPath);
	Status DrawPathNoTransform(NSStructures::CPen* pPen, CGraphicsPath* pPath);

	// заливка
	Status FillEllipse(CBrush* pBrush, double x, double y, double width, double height);
	Status FillRectangle(CBrush* pBrush, double x, double y, double width, double height);
	Status FillPolygon(CBrush* pBrush, double* pPoints, LONG lCount);
	Status FillPath(CBrush* pBrush, CGraphicsPath* pPath);

	// отрисовка картинки
	Status DrawImage(CImage* pImage, double x, double y, double width, double height);
	Status DrawMeta(const std::wstring& strFile, double x, double y, double width, double height);

#ifdef _WINDOWS_GDIPLUS_USE_
	Status DrawGdiplusImage(Gdiplus::Bitmap* pImage, double x, double y, double width, double height);
#endif

	BOOL DrawImageUnscaled(CImage* pImage, double x, double y);

	// отрисовка текста
	BOOL DrawString(const std::wstring& strText, CFontManager* pFont, CBrush* pBrush, double x, double y);
	BOOL DrawStringC(const LONG& lText, CFontManager* pFont, CBrush* pBrush, double x, double y);
	BOOL DrawStringPath(const std::wstring& strText, CFontManager* pFont, CBrush* pBrush, double x, double y);
	BOOL DrawStringPathC(const LONG& lText, CFontManager* pFont, CBrush* pBrush, double x, double y);

	void CalculateFullTransform();
	bool IsClip();

	inline Aggplus::CMatrix* GetFullTransform() { return &m_oFullTransform; }
	inline Aggplus::CMatrix* GetCoordTransform() { return &m_oCoordTransform; }

	inline double GetPixW() { return m_dWidthPix; }
	inline double GetPixH() { return m_dHeightPix; }

protected:

	template<class Renderer>
	void render_scanlines(Renderer& ren);
	template<class Rasterizer, class Renderer>
	void render_scanlines(Rasterizer& ras, Renderer& ren);

	void DoFillPathSolid(CColor dwColor);
	void DoFillPathGradient(CBrushLinearGradient *pBrush);
	void DoFillPathGradient2(CBrushLinearGradient *pBrush);
	void DoFillPathHatch(CBrushHatch *pBrush);
	void DoFillPathTextureClampSz(const CMatrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride);
	void DoFillPathTextureClampSz2(const CMatrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride);
	void DoFillPath(const CBrush* Brush);

	// text methods
	int FillGlyph2(int nX, int nY, TGlyph* pGlyph, Aggplus::CBrush* pBrush);
	int FillGlyphSimple(int nX, int nY, LONG lWidth, LONG lHeight, BYTE* pData, Aggplus::CBrush* pBrush);

	void PrepareScanlineStorage(agg::scanline_storage_aa8* storage, int x, int y, TGlyphBitmap *pGlyph);

	void UpdateUnits();
};
}

#endif // _BUILD_GRAPHICS_H_