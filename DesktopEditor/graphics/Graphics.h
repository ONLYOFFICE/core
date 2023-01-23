/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
#include "../fontengine/FontManager.h"

#include <vector>

#if defined(_WIN32) || defined (_WIN64)
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
typedef agg::comp_op_adaptor_rgba< agg::svg::color_type, agg::svg::component_order >				blender_type_comp;
typedef agg::pixfmt_alpha_blend_rgba< blender_type, agg::rendering_buffer, agg::svg::pixel_type >	pixfmt_type;
typedef agg::pixfmt_custom_blend_rgba< blender_type_comp, agg::rendering_buffer>					pixfmt_type_comp;

typedef agg::renderer_base<pixfmt_type> base_renderer_type;
typedef agg::renderer_base<pixfmt_type_comp> comp_renderer_type;
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

class CDIB;

class CGraphics_ClipStateRecord
{
public:
    CGraphicsPath*      Path;
    Aggplus::CMatrix*   Transform;
    agg::sbool_op_e     Operation;

public:
    CGraphics_ClipStateRecord()
    {
        Path        = NULL;
        Transform   = NULL;
        Operation   = agg::sbool_and;
    }
    ~CGraphics_ClipStateRecord()
    {
        RELEASEOBJECT(Path);
        RELEASEOBJECT(Transform);
    }

    CGraphics_ClipStateRecord* Clone() const
    {
        CGraphics_ClipStateRecord* pRet = new CGraphics_ClipStateRecord();
        if (Path)
            pRet->Path = Path->Clone();
        if (Transform)
            pRet->Transform = new CMatrix(*Transform);
        pRet->Operation = Operation;

        return pRet;
    }
};

class CGraphics_ClipState
{
public:
    std::vector<CGraphics_ClipStateRecord*> Records;

public:
    CGraphics_ClipState()
    {
    }
    CGraphics_ClipState(const CGraphics_ClipState& oSrc)
    {
        for (std::vector<CGraphics_ClipStateRecord*>::const_iterator i = oSrc.Records.begin(); i != oSrc.Records.end(); i++)
        {
            Records.push_back((*i)->Clone());
        }
    }
    ~CGraphics_ClipState()
    {
        Clear();
    }

    void AddRecord(CGraphics_ClipStateRecord* pRecord)
    {
        Records.push_back(pRecord);
    }
    void Clear()
    {
        for (std::vector<CGraphics_ClipStateRecord*>::iterator i = Records.begin(); i != Records.end(); i++)
        {
            CGraphics_ClipStateRecord* pRec = *i;
            RELEASEOBJECT(pRec);
        }
        Records.clear();
    }

    CGraphics_ClipState* Clone() const
    {
        CGraphics_ClipState* pRet = new CGraphics_ClipState();

        for (std::vector<CGraphics_ClipStateRecord*>::const_iterator i = Records.begin(); i != Records.end(); i++)
        {
            pRet->Records.push_back((*i)->Clone());
        }

        return pRet;
    }
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

	Aggplus::CMatrix m_oCoordTransform;
	Aggplus::CMatrix m_oBaseTransform;
	Aggplus::CMatrix m_oTransform;	

	Aggplus::CMatrix	m_oFullTransform;


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

    // for RGBA painting
    bool    m_bSwapRGB;
    bool    m_bIsDarkMode;

    double m_dDpiTile;

    CGraphics_ClipState m_oClipState;
	
	int m_nTextRenderMode;
	unsigned int m_nBlendMode;

public:

	CGraphics();
	CGraphics(int dwWidth, int dwHeight, int stride, BYTE* pBuffer);
	CGraphics(CImage* pImage);
	~CGraphics();

	INT IsDib();

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
	
	Aggplus::CMatrix* GetTransform();
	Status SetTransform(CMatrix* pTransform);
	Aggplus::CMatrix* GetBaseTransform();
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
    Status InternalClip(CGraphicsPath* pPath, CMatrix* pTransform, agg::sbool_op_e op);

	// измерение текста
	INT MeasureString(const std::wstring& strText, CFontManager* pManager, double* lWidth, double* lHeight);
	Status Clear(CColor oColor);

	Status DrawArc(NSStructures::CPen* pPen, double x, double y, double width, double height, double startAngle, double sweepAngle);
	Status DrawBezier(NSStructures::CPen* pPen, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	Status DrawBeziers(NSStructures::CPen* pPen, double* pPoints, LONG lCount);
	Status DrawCurve(NSStructures::CPen* pPen, double* pPoints, LONG lCount);
	Status DrawLine(NSStructures::CPen* pPen, double x1, double y1, double x2, double y2);
    Status DrawLines(NSStructures::CPen* pPen, double* pPoints, LONG lCount);
	Status DrawRectangle(NSStructures::CPen* pPen, double x, double y, double width, double height);
	Status DrawEllipse(NSStructures::CPen* pPen, double x, double y, double width, double height);
    Status DrawPath(NSStructures::CPen* pPen, CGraphicsPath* pPath, const double& gamma = -1);
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

	INT DrawImageUnscaled(CImage* pImage, double x, double y);

	// отрисовка текста
	INT DrawString(const std::wstring& strText, CFontManager* pFont, CBrush* pBrush, double x, double y);
    INT DrawString(const unsigned int* pGids, const unsigned int nGidsCount, CFontManager* pFont, CBrush* pBrush, double x, double y);
	INT DrawStringC(const LONG& lText, CFontManager* pFont, CBrush* pBrush, double x, double y);
	INT DrawStringPath(const std::wstring& strText, CFontManager* pFont, CBrush* pBrush, double x, double y);
	INT DrawStringPathC(const LONG& lText, CFontManager* pFont, CBrush* pBrush, double x, double y);

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
    template<class Renderer>
    void render_scanlines_alpha(Renderer& ren, BYTE Alpha);

	void DoFillPathSolid(CColor dwColor);
	void DoFillPathGradient(CBrushLinearGradient *pBrush);
	void DoFillPathGradient2(CBrushLinearGradient *pBrush);
    //--Test
    void DoFillPathGradientType(CBrushLinearGradient *pBrush);
    //--test
	void DoFillPathHatch(CBrushHatch *pBrush);
	void DoFillPathTextureClampSz(const CMatrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride);
	void DoFillPathTextureClampSz2(const CMatrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride, BYTE Alpha = 255);
    
    template<class ColorSpacePix>
    void DoFillPathTextureClampSz3(const CMatrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride, Aggplus::WrapMode wrapmode, BYTE Alpha = 255);
    
	void DoFillPath(const CBrush* Brush);

	// text methods
	int FillGlyph2(int nX, int nY, TGlyph* pGlyph, Aggplus::CBrush* pBrush);
	int FillGlyphSimple(int nX, int nY, LONG lWidth, LONG lHeight, BYTE* pData, Aggplus::CBrush* pBrush);

	void PrepareScanlineStorage(agg::scanline_storage_aa8* storage, int x, int y, TGlyphBitmap *pGlyph);

	void UpdateUnits();

    //test 
    
	void ScaleGranientInfo(long Type, NSStructures::GradientInfo &ginfo);
	void ScaleCoords(float &x, float &y);
};
}

#endif // _BUILD_GRAPHICS_H_
