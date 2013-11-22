#pragma once
#include "..\..\stdafx.h"


#include "..\ap_AggPlusEnums.h"
#include "..\ap_AggPlusTypes.h"

#include "..\include\agg_pixfmt_rgba.h"
#include "..\include\agg_conv_curve.h"
#include "..\include\agg_conv_stroke.h"
#include "..\include\agg_conv_dash.h"
#include "..\include\agg_span_gradient.h"
#include "..\include\agg_span_interpolator_linear.h"
#include "..\include\agg_conv_transform.h"
#include "..\include\agg_span_image_filter_rgba.h"
#include "..\include\agg_span_pattern_rgba.h"
#include "..\include\agg_image_accessors.h"
#include "..\include\agg_span_allocator.h"
#include "..\include\agg_image_filters.h"

#include "..\include\agg_basics.h"

#include "..\include\agg_color_rgba.h"
#include "..\include\agg_pixfmt_rgba.h"
#include "..\include\agg_rasterizer_scanline_aa.h"
#include "..\include\agg_renderer_scanline.h"
#include "..\include\agg_scanline_u.h"

#include "..\include\agg_pixfmt_amask_adaptor.h"
#include "..\include\agg_scanline_boolean_algebra.h"


#include "../svg/agg_svg_exception.h"
#include "../svg/agg_svg_parser.h"
#include "../svg/agg_svg_indexation_interpreter.h"
#include "../svg/agg_svg_rendering_interpreter.h"
#include "../svg/agg_svg_attributes_map.h"
#include "../svg/agg_svg_rasterizer.h"

#include "Color.h"
#include "Matrix.h"
#include "StringFormat.h"
#include "GraphicsPath.h"
#include "Clip.h"
#include "Brush.h"
#include "Image.h"
#include "..\..\Interfaces\IAVSFontManager.h"

#include "..\..\Objects\AVSGlyphImage.h"

#include "..\..\DocumentViewer\DIB.h"

#include "GdiplusEx.h"
#pragma warning(disable : 4100)


namespace Aggplus
{
typedef agg::rendering_buffer rendering_buffer_type;
typedef agg::pixfmt_bgra32 pixformat_type;

typedef agg::blender_rgba< agg::svg::color_type, agg::svg::component_order >                                blender_type;
typedef agg::pixfmt_alpha_blend_rgba< blender_type, agg::rendering_buffer, agg::svg::pixel_type > pixfmt_type;

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

class CGraphics
{
	friend class CAVSGlyphImage;
	friend class CMatrix;
	friend class CGraphicsPath;
	friend class CColor;
	friend class CBrush;
	friend class CBrushLinearGradient;
	friend class CBrushTexture;
	friend class CImage;

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

	// для отрисовки картинок - используем Gdiplus
	Gdiplus::Graphics*		m_pGraphics;
	Gdiplus::Bitmap*		m_pBitmap;
	NSDocumentViewer::CDIB* m_pDib;
	
	CGdiPlusInit			m_oInitGdiplus;	

public:

	agg::svg::frame_buffer_rgba&   get_frame_buffer() {return m_frame_buffer;}
	agg::svg::rasterizer&          get_rasterizer()   {return m_rasterizer;}

public:

	CGraphics()
	{
		m_pPixels = NULL;
		
		m_dWidthPix			= 0;
		m_dHeightPix		= 0;

		m_dDpiX				= 72.0;
		m_dDpiY				= 72.0;

		m_ePageUnits		= UnitPixel;

		m_pBitmap = NULL;
		m_pGraphics = NULL;

		m_oInitGdiplus.Init();
	}

	CGraphics(int dwWidth, int dwHeight, int stride, BYTE* pBuffer) : m_dwConfigFlags(0)
	{
		m_dWidthPix			= 0;
		m_dHeightPix		= 0;

		m_dDpiX				= 72.0;
		m_dDpiY				= 72.0;

		m_ePageUnits		= UnitPixel;

		m_pPixels	= NULL;

		Create(pBuffer, dwWidth, dwHeight, stride, 0);

		m_pBitmap = NULL;
		m_pGraphics = NULL;
	}

	CGraphics(CImage* pImage) : m_dwConfigFlags(0)
	{
		if (!pImage) 
		{ 
			return;
		}
		
		m_dWidthPix			= 0;
		m_dHeightPix		= 0;

		m_dDpiX				= 72.0;
		m_dDpiY				= 72.0;

		m_ePageUnits		= UnitPixel;

		m_pPixels	= NULL;
		
		Create(pImage->m_pImgData, pImage->GetWidth(), pImage->GetHeight(), pImage->m_nStride, 0);

		m_pBitmap = NULL;
		m_pGraphics = NULL;
	}

	~CGraphics()
	{
		RELEASEOBJECT(m_pGraphics);
		RELEASEOBJECT(m_pBitmap);
	}

	inline BOOL IsDib()
	{
		return (NULL != m_pDib);
	}
public:

	Status Create(BYTE* pPixels, int lWidth, int lHeight, int lStride, LONG lPitch)
	{
		if (lStride < 0)
		{
			//BYTE* pBuffer = pPixels + (lHeight - 1) * lStride;
			//m_frame_buffer.create(lWidth, lHeight, false, lStride, pBuffer);
			m_frame_buffer.create(lWidth, lHeight, false, lStride, pPixels);
		}
		else
		{
			m_frame_buffer.create(lWidth, lHeight, false, lStride, pPixels);
		}

		m_dWidthPix					= (double)lWidth;
		m_dHeightPix				= (double)lHeight;

		m_rasterizer.get_rasterizer().clip_box(0, 0, m_dWidthPix, m_dHeightPix);
		m_rasterizer.get_rasterizer().gamma(agg::gamma_none());

		m_dClipLeft					= 0;
		m_dClipTop					= 0;
		m_dClipWidth				= m_dWidthPix;
		m_dClipHeight				= m_dHeightPix;

		m_oClip.Create(lWidth, lHeight);

		UpdateUnits();
		return Ok;
	}

	Status Create2(BYTE* pPixels, int lWidth, int lHeight, int lStride, LONG lPitch, LONG x, LONG y, LONG w, LONG h, double dW, double dH, NSDocumentViewer::CDIB* pDib = NULL)
	{
		if (lStride < 0)
		{
			//BYTE* pBuffer = pPixels + (lHeight - 1) * lStride;
			//m_frame_buffer.create(lWidth, lHeight, false, lStride, pBuffer);
			m_frame_buffer.create(lWidth, lHeight, false, lStride, pPixels);
		}
		else
		{
			m_frame_buffer.create(lWidth, lHeight, false, lStride, pPixels);
		}

		m_dWidthPix					= (double)dW;
		m_dHeightPix				= (double)dH;

		m_rasterizer.get_rasterizer().clip_box(x, y, w + x, h + y);
		m_rasterizer.get_rasterizer().gamma(agg::gamma_none());

		m_dClipLeft					= x;
		m_dClipTop					= y;
		m_dClipWidth				= w;
		m_dClipHeight				= h;

		m_oClip.Create(lWidth, lHeight);

		UpdateUnits();

		RELEASEOBJECT(m_pGraphics);
		RELEASEOBJECT(m_pBitmap);

		BYTE* pBuffer = pPixels;
		if (0 > lStride)
		{
			// переворачиваем изображение для GDI+
			pBuffer += 4 * lWidth * (lHeight - 1);
		}

		m_pBitmap	= new Gdiplus::Bitmap(lWidth, lHeight, lStride, PixelFormat32bppARGB, pBuffer);
		m_pGraphics = new Gdiplus::Graphics(m_pBitmap);

		m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		m_pGraphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		m_pPixels = pPixels;
		m_pDib = pDib;
	
		return Ok;
	}

	Status CreatePart(LONG lLeft, LONG lTop, LONG lWidth, LONG lHeight, CGraphics** ppPart)
	{
		// здесь минимум кода. Просто сделать дубликат - и выставить ему правильные границы.
		// а потом уже и dpi и все настройки.
		return Ok;
	}

	inline double GetDpiX()
	{
		return m_dDpiX;
	}
	inline double GetDpiY()
	{
		return m_dDpiY;
	}
	inline Status SetDpiX(double dDpiX)
	{
		if (dDpiX != m_dDpiX)
		{
			m_dDpiX = dDpiX;
		}
		return Ok;
	}
	inline Status SetDpiY(double dDpiY)
	{
		if (dDpiY != m_dDpiY)
		{
			m_dDpiY = dDpiY;
		}
		return Ok;
	}
	
	Status SetPageWidth(double lWidth, LONG lUnit)
	{
		// вычисилить dpi и выставить его
		switch (lUnit)
		{
		case UnitPoint:
			{
				lWidth /= c_ag_Inch_to_Point;
				SetDpiX(m_dWidthPix / lWidth);

				break;
			}
		case UnitMillimeter:
			{
				lWidth /= c_ag_Inch_to_MM;
				SetDpiX(m_dWidthPix / lWidth);

				break;
			}
		case UnitInch:
			{
				SetDpiX(m_dWidthPix / lWidth);
				break;
			}
		default:
			break;
		};

		return Ok;
	}
	Status SetPageHeight(double lHeight, LONG lUnit)
	{
		// вычисилить dpi и выставить его
		switch (lUnit)
		{
		case UnitPoint:
			{
				lHeight /= c_ag_Inch_to_Point;
				SetDpiY(m_dHeightPix / lHeight);

				break;
			}
		case UnitMillimeter:
			{
				lHeight /= c_ag_Inch_to_MM;
				SetDpiY(m_dHeightPix / lHeight);

				break;
			}
		case UnitInch:
			{
				SetDpiY(m_dHeightPix / lHeight);
				break;
			}
		default:
			break;
		};

		return Ok;
	}
	
	Unit GetPageUnit()
	{
		return m_ePageUnits;
	}

	Status SetPageUnit(Unit lUnits)
	{
		m_ePageUnits = lUnits;
		UpdateUnits();
		return Ok;
	}
	
	CMatrix* GetTransform()
	{
		return &m_oTransform;
	}
	Status SetTransform(CMatrix* pTransform)
	{
		m_oTransform = *pTransform;
		return Ok;
	}

	CMatrix* GetBaseTransform()
	{
		return &m_oBaseTransform;
	}
	Status SetBaseTransform(CMatrix* pTransform)
	{
		m_oBaseTransform = *pTransform;
		return Ok;
	}
	
	Status TranslateTransform(double x, double y, MatrixOrder order = MatrixOrderPrepend)
	{
		m_oTransform.Translate(x, y, order);
		return Ok;
	}
	Status RotateTransform(double dAngle, MatrixOrder order = MatrixOrderPrepend)
	{
		m_oTransform.Rotate(dAngle, order);
		return Ok;
	}
	Status ScaleTransform(double dScaleX, double dScaleY, MatrixOrder order = MatrixOrderPrepend)
	{
		m_oTransform.Scale(dScaleX, dScaleY, order);
		return Ok;
	}
	Status ShearTransform(double shearX, double shearY, MatrixOrder order = MatrixOrderPrepend)
	{
		m_oTransform.Shear(shearX, shearY, order);
		return Ok;
	}
	Status MultiplyTransform(CMatrix* pMatrix, MatrixOrder order = MatrixOrderPrepend)
	{
		m_oTransform.Multiply(pMatrix, order);
		return Ok;
	}
	
	// функции отсечения
	Status SetClipRect(double dLeft, double dTop, double dWidth, double dHeight)
	{
		double dx1 = dLeft;
		double dy1 = dTop;
		double dx2 = dLeft + dWidth;
		double dy2 = dTop + dHeight;

		m_oFullTransform.TransformPoint(dx1, dy1);
		m_oFullTransform.TransformPoint(dx2, dy2);

		m_dClipLeft		= max(0, dx1);
		m_dClipTop		= max(0, dy1);
		m_dClipWidth	= min(dx2, m_dWidthPix - 1) - m_dClipLeft;
		m_dClipHeight	= min(dy2, m_dHeightPix - 1) - m_dClipTop;

		m_oClip.Reset();
		
		return Ok;
	}
	Status SetClipRect2(double dLeft, double dTop, double dWidth, double dHeight)
	{
		m_dClipLeft		= max(0, dLeft);
		m_dClipTop		= max(0, dTop);
		m_dClipWidth	= min(dWidth, m_dWidthPix - 1 - m_dClipLeft);
		m_dClipHeight	= min(dHeight, m_dHeightPix - 1 - m_dClipTop);

		m_rasterizer.get_rasterizer().clip_box(m_dClipLeft, m_dClipTop, m_dClipWidth + m_dClipLeft, m_dClipHeight + m_dClipTop);
		m_oClip.Reset();
		
		return Ok;
	}

	Status SetClipRect3(double dLeft, double dTop, double dWidth, double dHeight)
	{
		double dRight	= dLeft + dWidth;
		double dBottom	= dTop + dHeight;
		
		m_oFullTransform.TransformPoint(dLeft, dTop);
		m_oFullTransform.TransformPoint(dRight, dBottom);

		dWidth = dRight - dLeft;
		dHeight = dBottom - dTop;		

		m_dClipLeft		= dLeft;
		m_dClipTop		= dTop;
		m_dClipWidth	= dWidth;
		m_dClipHeight	= dHeight;

		m_rasterizer.get_rasterizer().reset_clipping();
		m_rasterizer.get_rasterizer().clip_box(m_dClipLeft, m_dClipTop, m_dClipWidth + m_dClipLeft, m_dClipHeight + m_dClipTop);

		m_frame_buffer.ren_base().clip_box((int)m_dClipLeft, (int)m_dClipTop, (int)(m_dClipWidth + m_dClipLeft), (int)(m_dClipHeight + m_dClipTop));

		m_oClip.Reset();
		
		return Ok;
	}

	Status SetClip(CGraphicsPath* pPath)
	{
		if (NULL == pPath)
			return InvalidParameter;
		
		m_oClip.GenerateClip(pPath, &m_oFullTransform);
		return Ok;
	}

	Status ResetClip()
	{
		m_oClip.Reset();
		return Ok;
	}

	Status ExclugeClip(CGraphicsPath* pPath)
	{
		if (NULL == pPath)
			return InvalidParameter;
		
		CGraphicsPath oMemory;
		oMemory.AddRectangle(0, 0, m_dWidthPix, m_dHeightPix);
		oMemory.AddPath(*pPath);
		
		m_oClip.GenerateClip(&oMemory, &m_oFullTransform);
		return Ok;
	}

	Status CombineClip(CGraphicsPath* pPath, agg::sbool_op_e op)
	{
		if (NULL == pPath)
			return InvalidParameter;
		
		m_oClip.Combine(pPath, &m_oFullTransform, op);
		return Ok;
	}

	// измерение текста
	STDMETHOD(MeasureString)(BSTR bstrText, IAVSFontManager* pManager, double* lWidth, double* lHeight)
	{
		if (NULL == pManager || NULL == lWidth || NULL == lHeight)
			return S_FALSE;

		pManager->LoadString(bstrText, 0, 0);

		float x = 0;
		float y = 0;
		float w = 0;
		float h = 0;
		
		pManager->MeasureString(&x, &y, &w, &h);

		*lWidth		= (double)w;
		*lHeight	= (double)h;

		return S_OK;
	}

	// тут еще надо продумать зачеркивание, подчеркивание. реализовывать нужно, понятно, в методе DrawString
	// отрисовка примитивов (без заливки)

	Status Clear(CColor oColor)
	{
		CBrushSolid oBrush(oColor);
		return FillRectangle(&oBrush, 0, 0, m_dWidthPix, m_dHeightPix);
	}

	Status DrawArc(NSStructures::CPen* pPen, double x, double y, double width, double height, double startAngle, double sweepAngle)
	{
		CGraphicsPath oPath;
		oPath.AddArc(x, y, width, height, startAngle, sweepAngle);

		return DrawPath(pPen, &oPath);
	}

	Status DrawBezier(NSStructures::CPen* pPen, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
	{
		CGraphicsPath oPath;
		oPath.AddBezier(x1, y1, x2, y2, x3, y3, x4, y4);

		return DrawPath(pPen, &oPath);
	}
	Status DrawBeziers(NSStructures::CPen* pPen, double* pPoints, LONG lCount)
	{
		CGraphicsPath oPath;
		oPath.AddBeziers(pPoints, lCount);

		return DrawPath(pPen, &oPath);
	}
	Status DrawCurve(NSStructures::CPen* pPen, double* pPoints, LONG lCount)
	{
		CGraphicsPath oPath;
		oPath.AddBeziers(pPoints, lCount);

		return DrawPath(pPen, &oPath);
	}
	Status DrawLine(NSStructures::CPen* pPen, double x1, double y1, double x2, double y2)
	{
		CGraphicsPath oPath;
		oPath.AddLine(x1, y1, x2, y2);

		return DrawPath(pPen, &oPath);
	}
    Status DrawLines(NSStructures::CPen* pPen, double* pPoints, LONG lCount)
	{
		CGraphicsPath oPath;
		oPath.AddLines(pPoints, lCount);

		return DrawPath(pPen, &oPath);
	}
	Status DrawRectangle(NSStructures::CPen* pPen, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddRectangle(x, y, width, height);

		return DrawPath(pPen, &oPath);
	}
	Status DrawEllipse(NSStructures::CPen* pPen, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddEllipse(x, y, width, height);

		return DrawPath(pPen, &oPath);
	}
	Status DrawPath(NSStructures::CPen* pPen, CGraphicsPath* pPath)
	{
		if (NULL == pPen || NULL == pPath)
			return InvalidParameter;

		m_rasterizer.get_rasterizer().reset();

		agg::line_join_e LineJoin = agg::round_join;
		switch(pPen->LineJoin)
		{
		case LineJoinMiter			: LineJoin = agg::miter_join_revert; break;
		case LineJoinBevel			: LineJoin = agg::bevel_join; break;
		case LineJoinRound			: LineJoin = agg::round_join; break;
		case LineJoinMiterClipped	: LineJoin = agg::miter_join_revert; break;
		default:	break;		
		}
		agg::line_cap_e LineCap = agg::round_cap;
		switch(pPen->LineStartCap)
		{
		case LineCapFlat         : LineCap = agg::butt_cap; break;
		case LineCapRound        : LineCap = agg::round_cap; break;
		case LineCapSquare       : LineCap = agg::square_cap; break;
		default:	break;		
		}

		double dWidth		 = pPen->Size;
		if (0 == dWidth)
		{
			dWidth = 1.0 / sqrt(m_oCoordTransform.m_agg_mtx.determinant());
		}
		
		double dblMiterLimit = pPen->MiterLimit;
		
		agg::path_storage path_copy(pPath->m_agg_ps);
		typedef agg::conv_curve<agg::path_storage> conv_crv_type;
				
		conv_crv_type c_c_path(path_copy);
		c_c_path.approximation_scale(25.0);
		c_c_path.approximation_method(agg::curve_inc);
		DashStyle eStyle = (DashStyle)pPen->DashStyle;
		 
		if (DashStyleSolid == eStyle)
		{
			typedef agg::conv_stroke<conv_crv_type> Path_Conv_StrokeN;
			Path_Conv_StrokeN pgN(c_c_path);

			//pgN.line_join(agg::miter_join_revert);

			pgN.line_cap(LineCap);
			
			pgN.line_join(LineJoin);
			pgN.inner_join(agg::inner_round);
			
			pgN.miter_limit(dblMiterLimit);
			pgN.width(dWidth);

			pgN.approximation_scale(25.0);

			typedef agg::conv_transform<Path_Conv_StrokeN> transStroke;

			transStroke trans(pgN, m_oFullTransform.m_agg_mtx);
			m_rasterizer.get_rasterizer().add_path(trans);
		}
		else
		{
			typedef agg::conv_dash<conv_crv_type> Path_Conv_Dash;
			Path_Conv_Dash poly2_dash(c_c_path);

			typedef agg::conv_stroke<Path_Conv_Dash> Path_Conv_StrokeD;
			Path_Conv_StrokeD pgD(poly2_dash);

			switch (eStyle)
			{
			case DashStyleDash:
				poly2_dash.add_dash(3.00*dWidth, dWidth);
				break;
			case DashStyleDot:
				poly2_dash.add_dash(dWidth, dWidth);
				break;
			case DashStyleDashDot:
				poly2_dash.add_dash(3.00*dWidth, dWidth);
				poly2_dash.add_dash(dWidth, dWidth);
				break;
			case DashStyleDashDotDot:
				poly2_dash.add_dash(3.00*dWidth, dWidth);
				poly2_dash.add_dash(dWidth, dWidth);
				poly2_dash.add_dash(dWidth, dWidth);
				break;
			default:
			case DashStyleCustom:
				{
					double offset	= pPen->DashOffset;
					double* params	= pPen->DashPattern;
					LONG lCount		= pPen->Count;
					LONG lCount2	= lCount / 2;

					double dKoef	= 0.352777778;

					for (LONG i = 0; i < lCount2; ++i)
					{
						if (0 == i)
						{
							poly2_dash.add_dash((params[i * 2]) * dKoef, params[i * 2 + 1] * dKoef); 
						}
						else
						{
							poly2_dash.add_dash(params[i * 2] * dKoef, params[i * 2 + 1] * dKoef); 
						}
					}
					if (1 == (lCount % 2))
					{
						poly2_dash.add_dash(params[lCount - 1] * dKoef, 0);
					}
					poly2_dash.dash_start(offset * dKoef);

					break;
				}
			}
			
			pgD.line_cap(LineCap);
			pgD.line_join(LineJoin);
			pgD.miter_limit(dblMiterLimit);
			pgD.width(dWidth);

			agg::conv_transform<Path_Conv_StrokeD> trans(pgD, m_oFullTransform.m_agg_mtx);
			m_rasterizer.get_rasterizer().add_path(trans);
		}

		CColor oColor((BYTE)pPen->Alpha, pPen->Color);
		CBrushSolid oBrush(oColor);
		
		m_rasterizer.get_rasterizer().filling_rule(agg::fill_non_zero);
		
		DoFillPath(&oBrush);
		return Ok;
	}

	Status DrawPathNoTransform(NSStructures::CPen* pPen, CGraphicsPath* pPath)
	{
		if (NULL == pPen || NULL == pPath)
			return InvalidParameter;

		m_rasterizer.get_rasterizer().reset();
		//butt_cap, square_cap, round_cap
		//pg.line_cap(agg::vcgen_stroke::round_cap);

		agg::line_join_e LineJoin;
		switch(pPen->LineJoin)
		{
			case LineJoinMiter       : LineJoin = agg::miter_join; break;
			case LineJoinBevel       : LineJoin = agg::bevel_join; break;
			default:
			case LineJoinRound       : LineJoin = agg::round_join; break;
			case LineJoinMiterClipped: LineJoin = agg::miter_join_revert; break;
		}

		double dWidth		 = pPen->Size;

		double dblMiterLimit = 0.5;
		
		agg::path_storage path_copy(pPath->m_agg_ps);
		typedef agg::conv_curve<agg::path_storage> conv_crv_type;
				
		conv_crv_type c_c_path(path_copy);
		c_c_path.approximation_scale(25.0);
		c_c_path.approximation_method(agg::curve_inc);
		DashStyle eStyle = (DashStyle)pPen->DashStyle;
		 
		if (DashStyleSolid == eStyle ||	DashStyleCustom == eStyle)
		{
			typedef agg::conv_stroke<conv_crv_type> Path_Conv_StrokeN;
			Path_Conv_StrokeN pgN(c_c_path);
			//typedef agg::conv_stroke<agg::path_storage> Path_Conv_StrokeN;
			//Path_Conv_StrokeN pgN(pPath->m_agg_ps);
			pgN.line_join(agg::round_join);
			pgN.miter_limit(dblMiterLimit);
			//pgN.approximation_scale(25.0);
			pgN.width(dWidth);
			
			typedef agg::conv_transform<Path_Conv_StrokeN> transStroke;

			CMatrix oM;
			transStroke trans(pgN, oM.m_agg_mtx);
			m_rasterizer.get_rasterizer().add_path(trans);
		}
		else
		{
			typedef agg::conv_dash<conv_crv_type> Path_Conv_Dash;
			Path_Conv_Dash poly2_dash(c_c_path);
			

			typedef agg::conv_stroke<Path_Conv_Dash> Path_Conv_StrokeD;
			Path_Conv_StrokeD pgD(poly2_dash);

			switch (eStyle)
			{
			case DashStyleDash:
				poly2_dash.add_dash(3.00*dWidth, dWidth);
				break;
			case DashStyleDot:
				poly2_dash.add_dash(dWidth, dWidth);
				break;
			case DashStyleDashDot:
				poly2_dash.add_dash(3.00*dWidth, dWidth);
				poly2_dash.add_dash(dWidth, dWidth);
				break;
			case DashStyleDashDotDot:
				poly2_dash.add_dash(3.00*dWidth, dWidth);
				poly2_dash.add_dash(dWidth, dWidth);
				poly2_dash.add_dash(dWidth, dWidth);
				break;
			default:
			case DashStyleCustom:break;
			}
			pgD.line_join(LineJoin);
			pgD.miter_limit(dblMiterLimit);
			pgD.width(dWidth);

			agg::conv_transform<Path_Conv_StrokeD> trans(pgD, m_oFullTransform.m_agg_mtx);
			m_rasterizer.get_rasterizer().add_path(trans);
		}

		CColor oColor((BYTE)pPen->Alpha, pPen->Color);
		CBrushSolid oBrush(oColor);
		
		m_rasterizer.get_rasterizer().filling_rule(agg::fill_non_zero);
		
		DoFillPath(&oBrush);
		return Ok;
	}

	// заливка
	Status FillEllipse(CBrush* pBrush, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddEllipse(x, y, width, height);

		return FillPath(pBrush, &oPath);
	}

	Status FillRectangle(CBrush* pBrush, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddRectangle(x, y, width, height);

		return FillPath(pBrush, &oPath);
	}

	Status FillPolygon(CBrush* pBrush, double* pPoints, LONG lCount)
	{
		CGraphicsPath oPath;
		oPath.AddPolygon(pPoints, lCount);

		return FillPath(pBrush, &oPath);
	}
	Status FillPath(CBrush* pBrush, CGraphicsPath* pPath)
	{
		if (NULL == pBrush)
			return InvalidParameter;

		m_rasterizer.get_rasterizer().reset();

		agg::path_storage p2(pPath->m_agg_ps);
		typedef agg::conv_transform<agg::path_storage> trans_type;
		trans_type trans(p2, m_oFullTransform.m_agg_mtx);
		
		typedef agg::conv_curve<trans_type> conv_crv_type;
		conv_crv_type c_c_path(trans);
		 
		m_rasterizer.get_rasterizer().add_path(c_c_path);

		m_rasterizer.get_rasterizer().filling_rule(pPath->m_bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

		if (pBrush->GetType() == Aggplus::BrushTypeTextureFill)
		{
			CBrushTexture *ptxBrush= (CBrushTexture *)pBrush;

			DWORD dwPatternWidth  = ptxBrush->PatternGetWidth();
			DWORD dwPatternHeight = ptxBrush->PatternGetHeight();
			
			if( !dwPatternWidth || !dwPatternHeight )
				return Ok;

			double x = 0;
			double y = 0;
			double r = 0;
			double b = 0;
			pPath->GetBounds(x, y, r, b);

			r += x;
			b += y;

			/*
			m_oFullTransform.TransformPoint(x, y);
			m_oFullTransform.TransformPoint(r, b);

			if (ptxBrush->GetWrapMode() == Aggplus::WrapModeClamp)
			{
				double dScaleX = (r - x) / dwPatternWidth;
				double dScaleY = (b - y) / dwPatternHeight;

				ptxBrush->ScaleTransform(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);
			}
			ptxBrush->TranslateTransform(x, y, Aggplus::MatrixOrderAppend);
			*/
			CMatrix brushMatrix;
			if (ptxBrush->GetWrapMode() == Aggplus::WrapModeClamp)
			{
				double dScaleX = (r - x) / dwPatternWidth;
				double dScaleY = (b - y) / dwPatternHeight;

				brushMatrix.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);
			}
			brushMatrix.Translate(x, y, Aggplus::MatrixOrderAppend);
			brushMatrix.Multiply(&m_oFullTransform, MatrixOrderAppend);
			ptxBrush->SetTransform(&brushMatrix);
		}
		else if( pBrush->GetType() == Aggplus::BrushTypePathGradient )
		{
			
		}

		DoFillPath(pBrush);
		return Ok;
	}


	// отрисовка картинки
	Status DrawImage(CImage* pImage, double x, double y, double width, double height)
	{
		if (!pImage || pImage->GetLastStatus() != Ok) 
			return UnknownImageFormat;

		if(width == 0.00 || height == 0.00) 
			return InvalidParameter;
		
		CGraphicsPath oPath;
		oPath.MoveTo(x, y);
		oPath.LineTo(x+width, y);
		oPath.LineTo(x+width, y+height);
		oPath.LineTo(x, y+height);
		oPath.CloseFigure();

		CBrushTexture oBrush(pImage, Aggplus::WrapModeClamp);
		FillPath(&oBrush, &oPath);
		return Ok;
	}
	Status DrawMeta(BSTR bsFile, double x, double y, double width, double height)
	{
		HENHMETAFILE hMeta = GetEnhMetaFile(bsFile);
		
		HDC hDC = GetDC(NULL);
		HDC memdc = CreateCompatibleDC(hDC);

		ReleaseDC(NULL, hDC);

		m_pGraphics->SetPageUnit(Gdiplus::UnitPixel);

		double x1 = x; 
		double y1 = y;
		m_oFullTransform.TransformPoint(x1, y1);

		double x2 = x + width;
		double y2 = y;
		m_oFullTransform.TransformPoint(x2, y2);

		double x3 = x;
		double y3 = y + height;
		m_oFullTransform.TransformPoint(x3, y3);

		Gdiplus::PointF points[3];
		points[0].X = (float)x1;
		points[0].Y = (float)y1;
		points[1].X = (float)x2;
		points[1].Y = (float)y2;
		points[2].X = (float)x3;
		points[2].Y = (float)y3;

		HGDIOBJ hOld = SelectObject(memdc, m_pDib->m_hBitmap);

		RECT oRect;
		oRect.left = x1;
		oRect.top = y1;
		oRect.right = x2;
		oRect.bottom = y3;
		PlayEnhMetaFile(memdc, hMeta, &oRect);

		DeleteObject(hOld);
		DeleteDC(memdc);
		
		DeleteEnhMetaFile(hMeta);

		return Ok;
	}
	Status DrawGdiplusImage(Gdiplus::Bitmap* pImage, double x, double y, double width, double height)
	{
		if (!pImage || pImage->GetLastStatus() != Gdiplus::Ok) 
			return UnknownImageFormat;

		if(width == 0.00 || height == 0.00) 
			return InvalidParameter;
		
		//float transform[6];
		//m_oFullTransform.GetElements(transform);

		//Gdiplus::Matrix oMatrix(transform[0], transform[1], transform[2], transform[3], transform[4], transform[5]);
		
		m_pGraphics->SetPageUnit(Gdiplus::UnitPixel);
		//m_pGraphics->SetTransform(&oMatrix);

		double x1 = x; 
		double y1 = y;
		m_oFullTransform.TransformPoint(x1, y1);

		double x2 = x + width;
		double y2 = y;
		m_oFullTransform.TransformPoint(x2, y2);

		double x3 = x;
		double y3 = y + height;
		m_oFullTransform.TransformPoint(x3, y3);

		Gdiplus::PointF points[3];
		points[0].X = (float)(x1 - 1.0);
		points[0].Y = (float)(y1 - 1.0);
		points[1].X = (float)(x2 + 1.0);
		points[1].Y = (float)(y2 - 1.0);
		points[2].X = (float)(x3 - 1.0);
		points[2].Y = (float)(y3 + 1.0);
		
		//m_pGraphics->DrawImage(pImage, (float)x, (float)y, (float)width, (float)height);

		m_pGraphics->DrawImage(pImage, points, 3);

		//m_pGraphics->ResetTransform();
		return Ok;
	}
	STDMETHOD(DrawImageUnscaled)(CImage* pImage, double x, double y)
	{
		return S_OK;
	}

	// отрисовка текста
	STDMETHOD(DrawString)(BSTR bstrText, IAVSFontManager* pFont, CBrush* pBrush, double x, double y)
	{		
		if (pBrush->GetType() != BrushTypeSolidColor)
			return S_OK;

		CMatrix oMatrix = m_oBaseTransform;
		oMatrix.Multiply(&m_oTransform, MatrixOrderPrepend);

		double mass[6];
		oMatrix.GetElements(mass);
		//m_oFullTransform.GetElements(mass);

		double _x = x;
		double _y = y;

		//m_oFullTransform.TransformPoint(_x, _y);
		/*m_oCoordTransform.TransformPoint(_x, _y);
		m_oBaseTransform.TransformPoint(_x, _y);*/

		CMatrix oM1 = oMatrix;
		oM1.Invert();
		oM1.Multiply(&m_oFullTransform, MatrixOrderPrepend);
		oM1.TransformPoint(_x, _y);
		
		pFont->SetTextMatrix((float)mass[0], (float)mass[1], (float)mass[2], (float)mass[3], (float)mass[4], (float)mass[5]);

		pFont->LoadString2(bstrText, (float)_x, (float)_y);
		float fX = 0;
		float fY = 0;
		VARIANT_BOOL bRes = VARIANT_FALSE;

		while (TRUE)
		{
			IAVSGlyphImage* pGlyph = NULL;
			float fX = 0, fY = 0;

			HRESULT hr = pFont->GetNextChar2(&pGlyph, &fX, &fY, &bRes);

			if (VARIANT_FALSE == bRes)
				break;

			if (NULL != pGlyph)
			{
				double _fX = fX;
				double _fY = fY;

				FillGlyph2((int)fX, (int)fY, pGlyph, pBrush);

				RELEASEINTERFACE(pGlyph);
			}
		}

		return S_OK;
	}
	STDMETHOD(DrawStringPath)(BSTR bstrText, IAVSFontManager* pFont, CBrush* pBrush, double x, double y)
	{		
		if (pBrush->GetType() != BrushTypeSolidColor)
			return S_OK;

		double mass[6];
		m_oTransform.GetElements(mass);

		double _x = x;
		double _y = y;

		Aggplus::CGraphicsPath oPath;
		oPath.AddString(bstrText, pFont, x, y);

		FillPath(pBrush, &oPath);
		
		return S_OK;
	}
	STDMETHOD(DrawString2)(BSTR bstrText, IAVSFontManager* pFont, IAVSBrush* pBrush, 
														IAVSStringFormat* pFormat, double x, double y, double width, double height)
	{
		return S_OK;
	}

	void CalculateFullTransform()
	{
		m_oFullTransform	= m_oCoordTransform;
		m_oFullTransform.Multiply(&m_oBaseTransform, MatrixOrderAppend);
		m_oFullTransform.Multiply(&m_oTransform, MatrixOrderPrepend);
	}
	bool IsClip()
	{
		return m_oClip.IsClip();
	}

protected:

	template<class Renderer>
	inline void render_scanlines(Renderer& ren)
    {
		if (!m_oClip.IsClip())
		{
			return agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ren);
		}
		else
		{
			if (!m_oClip.IsClip2())
			{
				typedef agg::scanline_p8                               sbool_scanline_type;

				sbool_scanline_type sl_result;
				sbool_scanline_type sl1;
				sbool_scanline_type sl2;

				agg::sbool_combine_shapes_aa(agg::sbool_and, m_rasterizer.get_rasterizer(), m_oClip.m_rasterizer, 
					sl1, sl2, sl_result, ren);
			}
			else
			{
				typedef agg::scanline_p8                               sbool_scanline_type;

				sbool_scanline_type sl_result;
				sbool_scanline_type sl1;
				sbool_scanline_type sl2;

				sbool_scanline_type sl;

				agg::sbool_combine_shapes_aa(agg::sbool_and, m_rasterizer.get_rasterizer(),
					(1 == m_oClip.m_lCurStorage) ? m_oClip.m_storage1 :	m_oClip.m_storage2,	sl1, sl2, sl_result, ren);
			}

			/*typedef agg::scanline_u8_am<agg::alpha_mask_gray8> scanline_type;
			agg::alpha_mask_gray8 alpha_mask(m_oClip.GetRenderingBuffer());
			scanline_type sl(alpha_mask);
			agg::render_scanlines(m_rasterizer.get_rasterizer(), sl, ren);*/

			/*typedef agg::amask_no_clip_gray8 alpha_mask_type;
			typedef agg::pixfmt_amask_adaptor<pixfmt_type, alpha_mask_type> pixfmt_amask_type;
			typedef agg::renderer_base<pixfmt_amask_type>                   amask_ren_type;

			alpha_mask_type m_alpha_mask(m_oClip.GetRenderingBuffer());

			pixfmt_amask_type pixfa(m_frame_buffer.pixfmt(), m_alpha_mask);
			amask_ren_type rbase(pixfa);
			agg::renderer_scanline_aa_solid<amask_ren_type> _ren(rbase);

			agg::scanline_p8              m_sl;

			agg::render_scanlines(m_rasterizer.get_rasterizer(), m_sl, _ren);*/
		}		
    }

	template<class Rasterizer, class Renderer>
	inline void render_scanlines(Rasterizer& ras, Renderer& ren)
    {
		if (!m_oClip.IsClip())
		{
			return agg::render_scanlines(ras, m_rasterizer.get_scanline(), ren);
		}
		else
		{
			if (!m_oClip.IsClip2())
			{
				typedef agg::scanline_p8                               sbool_scanline_type;

				sbool_scanline_type sl_result;
				sbool_scanline_type sl1;
				sbool_scanline_type sl2;

				agg::sbool_combine_shapes_aa(agg::sbool_and, ras, m_oClip.m_rasterizer, 
					sl1, sl2, sl_result, ren);
			}
			else
			{
				typedef agg::scanline_p8                               sbool_scanline_type;

				sbool_scanline_type sl_result;
				sbool_scanline_type sl1;
				sbool_scanline_type sl2;

				sbool_scanline_type sl;

				agg::sbool_combine_shapes_aa(agg::sbool_and, ras,
					(1 == m_oClip.m_lCurStorage) ? m_oClip.m_storage1 :	m_oClip.m_storage2,	sl1, sl2, sl_result, ren);
			}

			/*typedef agg::scanline_u8_am<agg::alpha_mask_gray8> scanline_type;
			agg::alpha_mask_gray8 alpha_mask(m_oClip.GetRenderingBuffer());
			scanline_type sl(alpha_mask);
			agg::render_scanlines(m_rasterizer.get_rasterizer(), sl, ren);*/

			/*typedef agg::amask_no_clip_gray8 alpha_mask_type;
			typedef agg::pixfmt_amask_adaptor<pixfmt_type, alpha_mask_type> pixfmt_amask_type;
			typedef agg::renderer_base<pixfmt_amask_type>                   amask_ren_type;

			alpha_mask_type m_alpha_mask(m_oClip.GetRenderingBuffer());

			pixfmt_amask_type pixfa(m_frame_buffer.pixfmt(), m_alpha_mask);
			amask_ren_type rbase(pixfa);
			agg::renderer_scanline_aa_solid<amask_ren_type> _ren(rbase);

			agg::scanline_p8              m_sl;

			agg::render_scanlines(m_rasterizer.get_rasterizer(), m_sl, _ren);*/
		}		
    }

	void DoFillPathSolid(CColor dwColor)
	{
		typedef agg::renderer_scanline_aa_solid<base_renderer_type> solid_renderer_type;
		solid_renderer_type ren_fine(m_frame_buffer.ren_base());
		ren_fine.color(dwColor.GetAggColor());
		
		render_scanlines(ren_fine);
	}

	void DoFillPathGradient(CBrushLinearGradient *pBrush, const agg::trans_affine* pGlobalTransform = NULL)
	{
		Aggplus::CColor LinearColor[2];
		pBrush->GetLinearColors( LinearColor );

		Aggplus::RectF BrushRect;
		pBrush->GetRectangle( &BrushRect );

		agg::rect_d rect( BrushRect.GetLeft(), BrushRect.GetTop(), BrushRect.GetRight(), BrushRect.GetBottom() );
		if( pBrush->IsRelativeCoords() )
		{
			int x = m_rasterizer.get_rasterizer().min_x();
			int y = m_rasterizer.get_rasterizer().min_y();
			int width  = m_rasterizer.get_rasterizer().max_x() - m_rasterizer.get_rasterizer().min_x();
			int height = m_rasterizer.get_rasterizer().max_y() - m_rasterizer.get_rasterizer().min_y();

			rect.x1 = rect.x1 * width + x;
			rect.x2 = rect.x2 * width + x;
			rect.y1 = rect.y1 * height + y;
			rect.y2 = rect.y2 * height + y;
		}

		typedef agg::my_span_gradient<agg::rgba8> gradient_span_gen;
		gradient_span_gen span_gen;

		if( pBrush->IsRectable() )
			span_gen.SetDirection( rect, pBrush->GetAngle(), pBrush->IsAngleScalable() );
		else
			span_gen.SetDirection( rect );

		agg::rgba8 c1(LinearColor[0].GetB(), LinearColor[0].GetG(), LinearColor[0].GetR(), LinearColor[0].GetA());
		agg::rgba8 c2(LinearColor[1].GetB(), LinearColor[1].GetG(), LinearColor[1].GetR(), LinearColor[1].GetA());
		
		span_gen.SetColors( c1, c2 );
		span_gen.SetWrapMode( pBrush->GetWrapMode() & 1 );

		agg::rgba8* pSubColors = NULL;
		float* pSubBlends = NULL;
		
		int nCountSubColors = pBrush->GetInterpolationColorsCount();
		if( nCountSubColors > 0 )
		{
			pSubColors = new agg::rgba8[nCountSubColors];
			pSubBlends = new float[nCountSubColors];

			if( pSubColors && pSubBlends )
			{
				for( int i = 0; i < nCountSubColors; i++ )
				{
					CColor c;
					pBrush->GetSubColor( i, &c, &pSubBlends[i] );
					pSubColors[i] = agg::rgba8(c.GetB(), c.GetG(), c.GetR(), c.GetA());
				}
				
				span_gen.SetSubColors( pSubColors, pSubBlends, nCountSubColors );
			}
		}
		
		//agg::rgba8 clrs[5] = {c1, agg::rgba8( 0, 255, 0, 255 ), agg::rgba8( 255, 255, 0, 255 ), agg::rgba8( 128, 0, 128, 255 ), c2};
		//float pos[5] = {0.f, 0.25f, 0.5f, 0.75f, 1.f};

		//span_gen.SetSubColors( clrs, pos, 5 );


		typedef agg::span_allocator<gradient_span_gen::color_type> gradient_span_alloc;
		gradient_span_alloc span_alloc;

		typedef agg::renderer_scanline_aa<base_renderer_type, gradient_span_alloc, gradient_span_gen> renderer_gradient_type;
		renderer_gradient_type ren_gradient( m_frame_buffer.ren_base(), span_alloc, span_gen );

		render_scanlines(ren_gradient);

		if( pSubColors ) delete [] pSubColors;
		if( pSubBlends ) delete [] pSubBlends;
	}

	void DoFillPathTextureClampSz(const CMatrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride)
	{
		span_alloc_type span_allocator;                  // Span Allocator

		typedef agg::pixfmt_rgba32     pixfmt;
		typedef agg::image_accessor_clip<pixfmt> img_source_type;
		typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
		typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
		agg::trans_affine mtx_Work(mImgMtx.m_agg_mtx);
		mtx_Work.multiply(m_oFullTransform.m_agg_mtx);
		mtx_Work.invert();
		interpolator_type_linear interpolator(mtx_Work);
		{
			//agg::rendering_buffer PatRendBuff((BYTE *)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);

			agg::rendering_buffer PatRendBuff;

			if (nImgStride < 0)
			{
				BYTE* pBuffer = (BYTE*)pImgBuff + (dwImgHeight - 1) * nImgStride;
				PatRendBuff.attach(pBuffer, dwImgWidth, dwImgHeight, nImgStride);
			}
			else
			{
				PatRendBuff.attach((BYTE*)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);
			}			
			
			pixfmt          img_pixf(PatRendBuff);
			img_source_type img_src(img_pixf, agg::rgba(0, 0, 0, 0));
			span_gen_type sg(img_src, interpolator);
			renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
			//agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
			render_scanlines(ri);
		}
	}

	void DoFillPathTextureClampSz2(const CMatrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride)
	{
		span_alloc_type span_allocator;                  // Span Allocator

		typedef agg::pixfmt_bgra32     pixfmt;
		typedef agg::image_accessor_clip<pixfmt> img_source_type;
		typedef agg::span_image_filter_rgba_nn<img_source_type, interpolator_type_linear> span_gen_type;
		typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
		
		//double dScaleX = 270.0 / dwImgWidth;
		//double dScaleY = 190.0 / dwImgHeight;

		agg::trans_affine mtx_Work(mImgMtx.m_agg_mtx);
		//mtx_Work.scale(dScaleX, dScaleY);

		//mtx_Work.multiply(m_oFullTransform.m_agg_mtx);
		mtx_Work.invert();
		interpolator_type_linear interpolator(mtx_Work);
		{
			//agg::rendering_buffer PatRendBuff((BYTE *)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);

			agg::rendering_buffer PatRendBuff;

			if (nImgStride < 0)
			{
				BYTE* pBuffer = (BYTE*)pImgBuff + (dwImgHeight - 1) * nImgStride;
				PatRendBuff.attach(pBuffer, dwImgWidth, dwImgHeight, nImgStride);
			}
			else
			{
				PatRendBuff.attach((BYTE*)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);
			}

			pixfmt          img_pixf(PatRendBuff);
			img_source_type img_src(img_pixf, agg::rgba(0, 0, 0, 0));
			span_gen_type sg(img_src, interpolator);
			renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
			//agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
			render_scanlines(ri);
		}
	}

	void DoFillPath(const CBrush* Brush)
	{
		if (NULL == Brush) 
			return;

		if (Brush->GetType() == BrushTypeSolidColor)
		{
			CColor clr; 
			((CBrushSolid*)Brush)->GetColor(&clr);
			DoFillPathSolid(clr);
		}
		else if (Brush->GetType() == BrushTypeHatchFill)
		{
			return;
		}
		else if (Brush->GetType() == BrushTypeTextureFill)
		{
			CBrushTexture *ptxBrush = (CBrushTexture *)Brush;
			
			LPVOID pImgBuff = ptxBrush->PatternFinalize();
			if (pImgBuff)
			{
				DWORD dwImgWidth = ptxBrush->PatternGetWidth();
				DWORD dwImgHeight = ptxBrush->PatternGetHeight();
				int nImgStride = ptxBrush->PatternGetStride();
				
				if(pImgBuff && dwImgWidth && dwImgHeight)
				{
					Aggplus::WrapMode wrapmode = ptxBrush->m_wrapMode;
					Aggplus::CMatrix matrix = ptxBrush->m_mtx;

					BYTE* pTmpBuffer = NULL;
					
					if( ptxBrush->m_bUsePattern )
					{
						pTmpBuffer = new BYTE[dwImgWidth*dwImgHeight*4];
						if( pTmpBuffer )
						{
							BYTE clr2[4] = {ptxBrush->m_colors[0].GetR(), ptxBrush->m_colors[0].GetG(), ptxBrush->m_colors[0].GetB(), ptxBrush->m_colors[0].GetA()};
							BYTE clr1[4] = {ptxBrush->m_colors[1].GetR(), ptxBrush->m_colors[1].GetG(), ptxBrush->m_colors[1].GetB(), ptxBrush->m_colors[1].GetA()};
							
							BYTE* src = (BYTE*)pImgBuff;
							BYTE* dst = pTmpBuffer;
							int stride = nImgStride - dwImgWidth * 4;
							
							for( unsigned y = 0; y < dwImgHeight; ++y, src += stride )
							{
								for( unsigned x = 0; x < dwImgWidth; ++x, src += 4, dst += 4 )
								{
									unsigned grey2 = (src[2] * 77 + src[1] * 150 + src[0] * 29 + 128) >> 8;
									unsigned grey1 = 255 - grey2;

									dst[0] = (BYTE)((clr1[0] * grey1 + clr2[0] * grey2 + 128) * 0x8081 >> 23);
									dst[1] = (BYTE)((clr1[1] * grey1 + clr2[1] * grey2 + 128) * 0x8081 >> 23);
									dst[2] = (BYTE)((clr1[2] * grey1 + clr2[2] * grey2 + 128) * 0x8081 >> 23);
									dst[3] = (BYTE)((clr1[3] * (255 - src[3]) + clr2[3] * src[3] + 128) * 0x8081 >> 23);
								}
							}

							pImgBuff = pTmpBuffer;
							nImgStride = dwImgWidth * 4;

							wrapmode = WrapModeTile;
							matrix.Reset();
						}
					}
					
					if(wrapmode == WrapModeClamp)
					{
						DoFillPathTextureClampSz2( matrix, pImgBuff, dwImgWidth, dwImgHeight, nImgStride);
					}
					else
					{
						agg::trans_affine mtx_Work( matrix.m_agg_mtx );
						//mtx_Work.multiply(m_oFullTransform.m_agg_mtx);
						mtx_Work.invert();

						span_alloc_type span_allocator;                  // Span Allocator
						interpolator_type_linear interpolator(mtx_Work);
				  
						//agg::rendering_buffer PatRendBuff((BYTE *)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);
						agg::rendering_buffer PatRendBuff;

						if (nImgStride < 0)
						{
							BYTE* pBuffer = (BYTE*)pImgBuff + (dwImgHeight - 1) * nImgStride;
							PatRendBuff.attach(pBuffer, dwImgWidth, dwImgHeight, nImgStride);
						}
						else
						{
							PatRendBuff.attach((BYTE*)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);
						}

						if(wrapmode == WrapModeTileFlipX)
						{
								typedef agg::pixfmt_bgra32     pixfmt;
								//image_accessor_wrap
								typedef agg::wrap_mode_reflect wrap_x_type;
								typedef agg::wrap_mode_repeat wrap_y_type;
								typedef agg::image_accessor_wrap<pixfmt, wrap_x_type, wrap_y_type> img_source_type;
								typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
								typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;

								pixfmt          img_pixf(PatRendBuff);
								img_source_type img_src(img_pixf);
								span_gen_type sg(img_src, interpolator);
								renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
								
								//agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
								render_scanlines(ri);
						}
						else if(wrapmode == WrapModeTileFlipY)
						{
								typedef agg::pixfmt_bgra32     pixfmt;
								//image_accessor_wrap
								typedef agg::wrap_mode_repeat wrap_x_type;
								typedef agg::wrap_mode_reflect wrap_y_type;
								typedef agg::image_accessor_wrap<pixfmt, wrap_x_type, wrap_y_type> img_source_type;
								typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
								typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;

								pixfmt          img_pixf(PatRendBuff);
								img_source_type img_src(img_pixf);
								span_gen_type sg(img_src, interpolator);
								renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
								
								//agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
								render_scanlines(ri);
						}
						else if(wrapmode == WrapModeTileFlipXY)
						{
								typedef agg::pixfmt_bgra32     pixfmt;
								//image_accessor_wrap
								typedef agg::wrap_mode_reflect wrap_x_type;
								typedef agg::wrap_mode_reflect wrap_y_type;
								typedef agg::image_accessor_wrap<pixfmt, wrap_x_type, wrap_y_type> img_source_type;
								typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
								typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;

								pixfmt          img_pixf(PatRendBuff);
								img_source_type img_src(img_pixf);
								span_gen_type sg(img_src, interpolator);
								renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
								
								//agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
								render_scanlines(ri);
						}
						else //Repeat
						{
								typedef agg::pixfmt_bgra32     pixfmt;
								//image_accessor_wrap
								typedef agg::wrap_mode_repeat wrap_x_type;
								typedef agg::wrap_mode_repeat wrap_y_type;
								typedef agg::image_accessor_wrap<pixfmt, wrap_x_type, wrap_y_type> img_source_type;
								typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
								typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;

								pixfmt          img_pixf(PatRendBuff);
								img_source_type img_src(img_pixf);
								span_gen_type sg(img_src, interpolator);
								renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
								
								//agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
								render_scanlines(ri);
						}
					}

					if( pTmpBuffer )
						delete [] pTmpBuffer;
				}
			}
		}
		else if (Brush->GetType() == BrushTypePathGradient)
		{
			return;
		}
		else if (Brush->GetType() == BrushTypeLinearGradient)
		{
			DoFillPathGradient((CBrushLinearGradient*)Brush);
		}
	}

	// text methods
	int FillGlyph2(int nX, int nY, IAVSGlyphImage* pGlyph, Aggplus::CBrush* pBrush) 
	{
		LONG lWidth = 0;
		pGlyph->get_Width(&lWidth);

		LONG lHeight = 0;
		pGlyph->get_Height(&lHeight);

		BYTE* pData = NULL;
		pGlyph->get_Data(&pData);

		int nFrameW = (int)m_frame_buffer.width();
		int nFrameH = (int)m_frame_buffer.height();

		if (NULL == pData)
			return 0;

		if (NULL == pData || (nX + lWidth < 0) || (nX >= nFrameW) || (nY + lHeight < 0))
			return 0;
		
		if (!m_oClip.IsClip() && (0 <= nX) && (0 <= nY) && ((nX + lWidth) < nFrameW) && ((nY + lHeight) < nFrameH))
		{			
			return FillGlyphSimple(nX, nY, lWidth, lHeight, pData, pBrush);
		}
		
		typedef agg::scanline_u8_am<agg::alpha_mask_gray8> scanline_type;
		
		agg::scanline_storage_aa8 storage;
		//PrepareScanlineStorage(&storage, nX, nY, pGlyph);

		for (int j = 0; j < lHeight; ++j)
		{
			agg::scanline_p8 sl;
			sl.reset(nX, nX + lWidth);
			sl.add_cells(nX, lWidth, pData + j * lWidth);

			sl.finalize(nY + j);

			storage.render(sl);
		}
		
		if (pBrush->GetType() == Aggplus::BrushTypeSolidColor)
		{
			CColor clr; 
			((CBrushSolid*)pBrush)->GetColor(&clr);
			
			typedef agg::renderer_scanline_aa_solid<base_renderer_type> solid_renderer_type;
			solid_renderer_type ren_fine(m_frame_buffer.ren_base());
			ren_fine.color(clr.GetAggColor());

			//agg::render_scanlines(storage, m_rasterizer.get_scanline(), ren_fine);
			render_scanlines(storage, ren_fine);
		}
		
		return 0;
	}
	AGG_INLINE int FillGlyphSimple(int nX, int nY, LONG lWidth, LONG lHeight, BYTE* pData, Aggplus::CBrush* pBrush) 
	{
		CColor clr; 
		((CBrushSolid*)pBrush)->GetColor(&clr);

		typedef agg::renderer_scanline_aa_solid<base_renderer_type> solid_renderer_type;
		solid_renderer_type ren_fine(m_frame_buffer.ren_base());
		ren_fine.color(clr.GetAggColor());
		ren_fine.render(lWidth, lHeight, pData, nX, nY);
		return 0;
	}

	void PrepareScanlineStorage(agg::scanline_storage_aa8* storage, int x, int y, TGlyphBitmap *pGlyph)
	{
		//agg::scanline_p8 sl;
		
		int width = pGlyph->nWidth;
		m_rasterizer.get_scanline().reset(x, x + width);
		for (int j = 0; j < pGlyph->nHeight; ++j)
		{
			m_rasterizer.get_scanline().add_cells(x + j * width, width, pGlyph->pData + j * width);
		}

		m_rasterizer.get_scanline().finalize(y);
		storage->render(m_rasterizer.get_scanline());
	}

	void UpdateUnits()
	{
		// здесь - пересчет координат
		m_oCoordTransform.Reset();

		double dScaleX = 1.0;
		double dScaleY = 1.0;

		switch (m_ePageUnits)
		{
		case UnitPoint:
			{
				dScaleX = m_dDpiX / c_ag_Inch_to_Point;
				dScaleY = m_dDpiY / c_ag_Inch_to_Point;
				break;
			}
		case UnitMillimeter:
			{
				dScaleX = m_dDpiX / c_ag_Inch_to_MM;
				dScaleY = m_dDpiY / c_ag_Inch_to_MM;
				break;
			}
		case UnitInch:
			{
				dScaleX = m_dDpiX;
				dScaleY = m_dDpiY;
				break;
			}
		default:
			break;
		};

		m_oCoordTransform.Scale(dScaleX, dScaleY, MatrixOrderAppend);
		CalculateFullTransform();
	}

};
}