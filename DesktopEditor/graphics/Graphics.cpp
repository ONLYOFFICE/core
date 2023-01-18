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
#include "Graphics.h"
#include <algorithm>
#include "../fontengine/FontFile.h"

namespace Aggplus
{
	CGraphics::CGraphics()
	{
		m_pPixels	= NULL;
		m_pDib		= NULL;
		
		m_dWidthPix			= 0;
		m_dHeightPix		= 0;

		m_dDpiX				= 72.0;
		m_dDpiY				= 72.0;

		m_ePageUnits		= UnitPixel;

		m_bIntegerGrid		= false;

#ifdef _WINDOWS_GDIPLUS_USE_
		m_pBitmap = NULL;
		m_pGraphics = NULL;

		m_oInitGdiplus.Init();
#endif

		m_dGlobalAlpha		= 1.0;
		m_bSwapRGB = false;
		m_bIsDarkMode = false;

#if defined (_LINUX) || defined (_QT)
		m_bSwapRGB = true;
#endif

		m_dDpiTile = -1;

		m_nTextRenderMode = FT_RENDER_MODE_NORMAL;
		m_nBlendMode = agg::comp_op_src_over;
	}

	CGraphics::CGraphics(int dwWidth, int dwHeight, int stride, BYTE* pBuffer) : m_dwConfigFlags(0)
	{
		m_dWidthPix			= 0;
		m_dHeightPix		= 0;

		m_dDpiX				= 72.0;
		m_dDpiY				= 72.0;

		m_ePageUnits		= UnitPixel;

		m_pPixels	= NULL;

		m_bIntegerGrid		= false;

		Create(pBuffer, dwWidth, dwHeight, stride, 0);
		m_dGlobalAlpha = 1.0;

#ifdef _WINDOW_GDIPLUS_USE_
		m_pBitmap = NULL;
		m_pGraphics = NULL;
#endif

		m_bSwapRGB = false;

#if defined (_LINUX) || defined (_QT)
		m_bSwapRGB = true;
#endif

		m_dDpiTile = -1;
		
		m_nTextRenderMode = FT_RENDER_MODE_NORMAL;
		m_nBlendMode = agg::comp_op_src_over;
	}

	CGraphics::CGraphics(CImage* pImage) : m_dwConfigFlags(0)
	{
		m_dGlobalAlpha = 1.0;
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

		m_bIntegerGrid		= false;
		
		Create(pImage->m_pImgData, pImage->GetWidth(), pImage->GetHeight(), pImage->m_nStride, 0);

#ifdef _WINDOW_GDIPLUS_USE_
		m_pBitmap = NULL;
		m_pGraphics = NULL;
#endif

		m_bSwapRGB = false;

#if defined (_LINUX) || defined (_QT)
		m_bSwapRGB = true;
#endif

		m_dDpiTile = -1;
		
		m_nTextRenderMode = FT_RENDER_MODE_NORMAL;
		m_nBlendMode = agg::comp_op_src_over;
	}

	CGraphics::~CGraphics()
	{
#ifdef _WINDOW_GDIPLUS_USE_
		RELEASEOBJECT(m_pGraphics);
		RELEASEOBJECT(m_pBitmap);
#endif
	}

	INT CGraphics::IsDib()
	{
		return (NULL != m_pDib);
	}

	Status CGraphics::Create(BYTE* pPixels, int lWidth, int lHeight, int lStride, LONG lPitch)
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

	Status CGraphics::Create2(BYTE* pPixels, int lWidth, int lHeight, int lStride, LONG lPitch, LONG x, LONG y, LONG w, LONG h, double dW, double dH, CDIB* pDib)
	{
		if (lStride < 0)
		{
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

#ifdef _WINDOW_GDIPLUS_USE_
		RELEASEOBJECT(m_pGraphics);
		RELEASEOBJECT(m_pBitmap);
#endif

		BYTE* pBuffer = pPixels;
		if (0 > lStride)
		{
			// переворачиваем изображение для GDI+
			pBuffer += 4 * lWidth * (lHeight - 1);
		}

#ifdef _WINDOW_GDIPLUS_USE_
		m_pBitmap	= new Gdiplus::Bitmap(lWidth, lHeight, lStride, PixelFormat32bppARGB, pBuffer);
		m_pGraphics = new Gdiplus::Graphics(m_pBitmap);

		m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		m_pGraphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
#endif

		m_pPixels = pPixels;
		m_pDib = pDib;

		return Ok;
	}

	Status CGraphics::CreatePart(LONG lLeft, LONG lTop, LONG lWidth, LONG lHeight, CGraphics** ppPart)
	{
		// здесь минимум кода. Просто сделать дубликат - и выставить ему правильные границы.
		// а потом уже и dpi и все настройки.
		return Ok;
	}

	double CGraphics::GetDpiX()
	{
		return m_dDpiX;
	}
	double CGraphics::GetDpiY()
	{
		return m_dDpiY;
	}
	Status CGraphics::SetDpiX(double dDpiX)
	{
		if (dDpiX != m_dDpiX)
		{
			m_dDpiX = dDpiX;
		}
		return Ok;
	}
	Status CGraphics::SetDpiY(double dDpiY)
	{
		if (dDpiY != m_dDpiY)
		{
			m_dDpiY = dDpiY;
		}
		return Ok;
	}
	
	Status CGraphics::SetPageWidth(double lWidth, LONG lUnit)
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
	Status CGraphics::SetPageHeight(double lHeight, LONG lUnit)
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
	
	Unit CGraphics::GetPageUnit()
	{
		return m_ePageUnits;
	}

	Status CGraphics::SetPageUnit(Unit lUnits)
	{
		m_ePageUnits = lUnits;
		UpdateUnits();
		return Ok;
	}
	
	CMatrix* CGraphics::GetTransform()
	{
		return &m_oTransform;
	}
	Status CGraphics::SetTransform(CMatrix* pTransform)
	{
		m_oTransform = *pTransform;
		return Ok;
	}

	CMatrix* CGraphics::GetBaseTransform()
	{
		return &m_oBaseTransform;
	}
	Status CGraphics::SetBaseTransform(CMatrix* pTransform)
	{
		m_oBaseTransform = *pTransform;
		return Ok;
	}
	
	Status CGraphics::TranslateTransform(double x, double y, MatrixOrder order)
	{
		m_oTransform.Translate(x, y, order);
		return Ok;
	}
	Status CGraphics::RotateTransform(double dAngle, MatrixOrder order)
	{
		m_oTransform.Rotate(dAngle, order);
		return Ok;
	}
	Status CGraphics::ScaleTransform(double dScaleX, double dScaleY, MatrixOrder order)
	{
		m_oTransform.Scale(dScaleX, dScaleY, order);
		return Ok;
	}
	Status CGraphics::ShearTransform(double shearX, double shearY, MatrixOrder order)
	{
		m_oTransform.Shear(shearX, shearY, order);
		return Ok;
	}
	Status CGraphics::MultiplyTransform(CMatrix* pMatrix, MatrixOrder order)
	{
		m_oTransform.Multiply(pMatrix, order);
		return Ok;
	}
	
	// функции отсечения
	Status CGraphics::SetClipRect(double dLeft, double dTop, double dWidth, double dHeight)
	{
		double dx1 = dLeft;
		double dy1 = dTop;
		double dx2 = dLeft + dWidth;
		double dy2 = dTop + dHeight;

		m_oFullTransform.TransformPoint(dx1, dy1);
		m_oFullTransform.TransformPoint(dx2, dy2);

		m_dClipLeft		= std::max(0.0, dx1);
		m_dClipTop		= std::max(0.0, dy1);
		m_dClipWidth	= std::min(dx2, m_dWidthPix - 1) - m_dClipLeft;
		m_dClipHeight	= std::min(dy2, m_dHeightPix - 1) - m_dClipTop;

		m_oClip.Reset();
		
		return Ok;
	}
	Status CGraphics::SetClipRect2(double dLeft, double dTop, double dWidth, double dHeight)
	{
		m_dClipLeft		= std::max(0.0, dLeft);
		m_dClipTop		= std::max(0.0, dTop);
		m_dClipWidth	= std::min(dWidth, m_dWidthPix - 1 - m_dClipLeft);
		m_dClipHeight	= std::min(dHeight, m_dHeightPix - 1 - m_dClipTop);

		m_rasterizer.get_rasterizer().clip_box(m_dClipLeft, m_dClipTop, m_dClipWidth + m_dClipLeft, m_dClipHeight + m_dClipTop);
		m_oClip.Reset();
		
		return Ok;
	}

	Status CGraphics::SetClipRect3(double dLeft, double dTop, double dWidth, double dHeight)
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

	Status CGraphics::SetClip(CGraphicsPath* pPath)
	{
		if (NULL == pPath)
			return InvalidParameter;
		
		m_oClip.GenerateClip(pPath, &m_oFullTransform);
		return Ok;
	}

	Status CGraphics::ResetClip()
	{
		m_oClip.Reset();
		m_oClipState.Clear();
		return Ok;
	}

	Status CGraphics::ExclugeClip(CGraphicsPath* pPath)
	{
		if (NULL == pPath)
			return InvalidParameter;
		
		CGraphicsPath oMemory;
		oMemory.AddRectangle(0, 0, m_dWidthPix, m_dHeightPix);
		oMemory.AddPath(*pPath);
		
		m_oClip.GenerateClip(&oMemory, &m_oFullTransform);
		return Ok;
	}

	Status CGraphics::CombineClip(CGraphicsPath* pPath, agg::sbool_op_e op)
	{
		Aggplus::CMatrix m;
		return InternalClip(pPath, (m_bIntegerGrid || pPath->m_internal->m_pTransform != NULL) ? &m : &m_oFullTransform, op);
	}

	Status CGraphics::InternalClip(CGraphicsPath* pPath, CMatrix* pTransform, agg::sbool_op_e op)
	{
		if (NULL == pPath)
			return InvalidParameter;

		m_oClip.Combine(pPath, pTransform, op);

		// write to clips history
		CGraphics_ClipStateRecord* pRecord = new CGraphics_ClipStateRecord();
		pRecord->Path = (NULL != pPath) ? pPath->Clone() : NULL;
		pRecord->Transform = (NULL != pTransform) ? new CMatrix(*pTransform) : new CMatrix();
		pRecord->Operation = op;
		m_oClipState.AddRecord(pRecord);

		return Ok;
	}

	INT CGraphics::MeasureString(const std::wstring& strText, CFontManager* pManager, double* lWidth, double* lHeight)
	{
		if (NULL == pManager || NULL == lWidth || NULL == lHeight)
			return FALSE;

		pManager->LoadString1(strText, 0, 0);

		TBBox oBox = pManager->MeasureString();

		*lWidth		= (double)oBox.fMaxX - oBox.fMinX;
		*lHeight	= (double)oBox.fMaxY - oBox.fMinY;

		return TRUE;
	}

	Status CGraphics::Clear(CColor oColor)
	{
		CBrushSolid oBrush(oColor);
		return FillRectangle(&oBrush, 0, 0, m_dWidthPix, m_dHeightPix);
	}

	Status CGraphics::DrawArc(NSStructures::CPen* pPen, double x, double y, double width, double height, double startAngle, double sweepAngle)
	{
		CGraphicsPath oPath;
		oPath.AddArc(x, y, width, height, startAngle, sweepAngle);

		return DrawPath(pPen, &oPath);
	}

	Status CGraphics::DrawBezier(NSStructures::CPen* pPen, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
	{
		CGraphicsPath oPath;
		oPath.AddBezier(x1, y1, x2, y2, x3, y3, x4, y4);

		return DrawPath(pPen, &oPath);
	}
	Status CGraphics::DrawBeziers(NSStructures::CPen* pPen, double* pPoints, LONG lCount)
	{
		CGraphicsPath oPath;
		oPath.AddBeziers(pPoints, lCount);

		return DrawPath(pPen, &oPath);
	}
	Status CGraphics::DrawCurve(NSStructures::CPen* pPen, double* pPoints, LONG lCount)
	{
		CGraphicsPath oPath;
		oPath.AddBeziers(pPoints, lCount);

		return DrawPath(pPen, &oPath);
	}
	Status CGraphics::DrawLine(NSStructures::CPen* pPen, double x1, double y1, double x2, double y2)
	{
		CGraphicsPath oPath;
		oPath.AddLine(x1, y1, x2, y2);

		return DrawPath(pPen, &oPath);
	}
	Status CGraphics::DrawLines(NSStructures::CPen* pPen, double* pPoints, LONG lCount)
	{
		CGraphicsPath oPath;
		oPath.AddLines(pPoints, lCount);

		return DrawPath(pPen, &oPath);
	}
	Status CGraphics::DrawRectangle(NSStructures::CPen* pPen, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddRectangle(x, y, width, height);

		return DrawPath(pPen, &oPath);
	}
	Status CGraphics::DrawEllipse(NSStructures::CPen* pPen, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddEllipse(x, y, width, height);

		return DrawPath(pPen, &oPath);
	}
	Status CGraphics::DrawPath(NSStructures::CPen* pPen, CGraphicsPath* pPath, const double& gamma)
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
		double dWidthMinSize = 1.0 / sqrt(m_oCoordTransform.m_internal->m_agg_mtx.determinant());

		if ((0 == dWidth && !m_bIntegerGrid) || dWidth < dWidthMinSize)
		{
			//dWidth = dWidthMinSize;
		}
		
		double dblMiterLimit = pPen->MiterLimit;
		
		agg::path_storage path_copy(pPath->m_internal->m_agg_ps);
		bool bIsUseIdentity = m_bIntegerGrid;
		if (!bIsUseIdentity)
		{
			agg::trans_affine* full_trans = &m_oFullTransform.m_internal->m_agg_mtx;
			double dDet = full_trans->determinant();

			if (fabs(dDet) < 0.0001)
			{
				path_copy.transform_all_paths(m_oFullTransform.m_internal->m_agg_mtx);
				dWidth *= sqrt(dDet);

				bIsUseIdentity = true;
			}
		}

		typedef agg::conv_curve<agg::path_storage> conv_crv_type;

		conv_crv_type c_c_path(path_copy);
		c_c_path.approximation_scale(25.0);
		c_c_path.approximation_method(agg::curve_inc);
		DashStyle eStyle = (DashStyle)pPen->DashStyle;

		if (DashStyleCustom == eStyle)
		{
			if (0 == pPen->Count || NULL == pPen->DashPattern)
			{
				eStyle = DashStyleSolid;
			}
			else
			{
				bool bFoundNormal = false;
				for (int i = 0; i < pPen->Count; i++)
				{
					if (fabs(pPen->DashPattern[i]) > 0.0001)
					{
						bFoundNormal = true;
						break;
					}
				}
				if (!bFoundNormal)
					eStyle = DashStyleSolid;
			}
		}

		agg::trans_affine* pAffine = &m_oFullTransform.m_internal->m_agg_mtx;
		if (bIsUseIdentity)
			pAffine = new agg::trans_affine();

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

			transStroke trans(pgN, *pAffine);
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

				double dKoef = 1.0;

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

			if ((0 == dWidth && !m_bIntegerGrid) || dWidth < dWidthMinSize)
				dWidth = dWidthMinSize;

			pgD.line_cap(LineCap);
			pgD.line_join(LineJoin);
			pgD.miter_limit(dblMiterLimit);
			pgD.width(dWidth);

			agg::conv_transform<Path_Conv_StrokeD> trans(pgD, *pAffine);
			m_rasterizer.get_rasterizer().add_path(trans);
		}

		CColor oColor((BYTE)(pPen->Alpha * m_dGlobalAlpha), pPen->Color, m_bSwapRGB);
		CBrushSolid oBrush(oColor);
		
		m_rasterizer.get_rasterizer().filling_rule(agg::fill_non_zero);
		
		if (gamma >= 0)
			m_rasterizer.get_rasterizer().gamma(agg::gamma_threshold(gamma));

		DoFillPath(&oBrush);

		if (gamma >= 0)
			m_rasterizer.gamma(1.0);

		if (bIsUseIdentity)
			RELEASEOBJECT(pAffine);

		return Ok;
	}

	Status CGraphics::DrawPathNoTransform(NSStructures::CPen* pPen, CGraphicsPath* pPath)
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
		
		agg::path_storage path_copy(pPath->m_internal->m_agg_ps);
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
			transStroke trans(pgN, oM.m_internal->m_agg_mtx);
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

			agg::conv_transform<Path_Conv_StrokeD> trans(pgD, m_oFullTransform.m_internal->m_agg_mtx);
			m_rasterizer.get_rasterizer().add_path(trans);
		}

		CColor oColor((BYTE)pPen->Alpha, pPen->Color, m_bSwapRGB);
		CBrushSolid oBrush(oColor);
		
		m_rasterizer.get_rasterizer().filling_rule(agg::fill_non_zero);
		
		DoFillPath(&oBrush);
		return Ok;
	}

	Status CGraphics::FillEllipse(CBrush* pBrush, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddEllipse(x, y, width, height);

		return FillPath(pBrush, &oPath);
	}

	Status CGraphics::FillRectangle(CBrush* pBrush, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddRectangle(x, y, width, height);

		return FillPath(pBrush, &oPath);
	}

	Status CGraphics::FillPolygon(CBrush* pBrush, double* pPoints, LONG lCount)
	{
		CGraphicsPath oPath;
		oPath.AddPolygon(pPoints, lCount);

		return FillPath(pBrush, &oPath);
	}
	Status CGraphics::FillPath(CBrush* pBrush, CGraphicsPath* pPath)
	{
		if (NULL == pBrush)
			return InvalidParameter;

		m_rasterizer.get_rasterizer().reset();

		agg::path_storage p2(pPath->m_internal->m_agg_ps);
		typedef agg::conv_transform<agg::path_storage> trans_type;

		trans_type* ptrans			= NULL;
		agg::trans_affine* paffine	= NULL;
		if (!m_bIntegerGrid)
			ptrans = new trans_type(p2, m_oFullTransform.m_internal->m_agg_mtx);
		else
		{
			paffine = new agg::trans_affine();
			ptrans = new trans_type(p2, *paffine);
		}
		
		typedef agg::conv_curve<trans_type> conv_crv_type;
		conv_crv_type c_c_path(*ptrans);

		m_rasterizer.get_rasterizer().add_path(c_c_path);

		m_rasterizer.get_rasterizer().filling_rule(pPath->m_internal->m_bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

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

			if (!ptxBrush->m_bUseBounds)
			{
				pPath->GetBounds(x, y, r, b);

				r += x;
				b += y;
			}
			else
			{
				x = ptxBrush->m_oBounds.left;
				y = ptxBrush->m_oBounds.top;
				r = ptxBrush->m_oBounds.right;
				b = ptxBrush->m_oBounds.bottom;
			}

			CMatrix brushMatrix;
			if (ptxBrush->GetWrapMode() == Aggplus::WrapModeClamp)
			{
				double dScaleX = (r - x) / dwPatternWidth;
				double dScaleY = (b - y) / dwPatternHeight;

				brushMatrix.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);
			}

			if (ptxBrush->GetWrapMode() != Aggplus::WrapModeClamp && m_dDpiTile > 1)
			{
				double dScaleX = m_dDpiX / m_dDpiTile;
				double dScaleY = m_dDpiY / m_dDpiTile;

				brushMatrix.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);
			}

			brushMatrix.Translate(x, y, Aggplus::MatrixOrderAppend);
			brushMatrix.Multiply(&m_oFullTransform, MatrixOrderAppend);
			ptxBrush->SetTransform(&brushMatrix);
		}

		DoFillPath(pBrush);

		RELEASEOBJECT(ptrans);
		RELEASEOBJECT(paffine);
		return Ok;
	}


	// отрисовка картинки
	Status CGraphics::DrawImage(CImage* pImage, double x, double y, double width, double height)
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
	Status CGraphics::DrawMeta(const std::wstring& strFile, double x, double y, double width, double height)
	{
		return Ok;
	}

#ifdef _WINDOW_GDIPLUS_USE_
	Status CGraphics::DrawGdiplusImage(Gdiplus::Bitmap* pImage, double x, double y, double width, double height)
	{
		if (!pImage || pImage->GetLastStatus() != Gdiplus::Ok)
			return UnknownImageFormat;

		if(width == 0.00 || height == 0.00)
			return InvalidParameter;
		
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
		points[0].X = (float)(x1 - 1.0);
		points[0].Y = (float)(y1 - 1.0);
		points[1].X = (float)(x2 + 1.0);
		points[1].Y = (float)(y2 - 1.0);
		points[2].X = (float)(x3 - 1.0);
		points[2].Y = (float)(y3 + 1.0);
		
		m_pGraphics->DrawImage(pImage, points, 3);
		return Ok;
	}
#endif

	INT CGraphics::DrawImageUnscaled(CImage* pImage, double x, double y)
	{
		return TRUE;
	}

	INT CGraphics::DrawString(const std::wstring& strText, CFontManager* pFont, CBrush* pBrush, double x, double y)
	{
		if (pBrush->GetType() != BrushTypeSolidColor)
			return TRUE;

		CMatrix oMatrix = m_oBaseTransform;
		oMatrix.Multiply(&m_oTransform, MatrixOrderPrepend);

		double mass[6];
		oMatrix.GetElements(mass);
		
		double _x = x;
		double _y = y;

		CMatrix oM1 = oMatrix;
		oM1.Invert();
		oM1.Multiply(&m_oFullTransform, MatrixOrderPrepend);
		oM1.TransformPoint(_x, _y);
		
		pFont->SetTextMatrix((float)mass[0], (float)mass[1], (float)mass[2], (float)mass[3], (float)mass[4], (float)mass[5]);
		m_nTextRenderMode = pFont->m_nRENDER_MODE;

		pFont->LoadString2(strText, (float)_x, (float)_y);
		float fX = 0;
		float fY = 0;
		INT bRes = FALSE;

		while (TRUE)
		{
			TGlyph* pGlyph = NULL;

			bRes = pFont->GetNextChar2(pGlyph, fX, fY);

			if (FALSE == bRes)
				break;

			if (NULL != pGlyph)
			{
				FillGlyph2((int)fX, (int)fY, pGlyph, pBrush);
			}
		}

		return TRUE;
	}
	INT CGraphics::DrawString(const unsigned int* pGids, const unsigned int nGidsCount, CFontManager* pFont, CBrush* pBrush, double x, double y)
	{
		if (pBrush->GetType() != BrushTypeSolidColor)
			return TRUE;

		CMatrix oMatrix = m_oBaseTransform;
		oMatrix.Multiply(&m_oTransform, MatrixOrderPrepend);

		double mass[6];
		oMatrix.GetElements(mass);

		double _x = x;
		double _y = y;

		CMatrix oM1 = oMatrix;
		oM1.Invert();
		oM1.Multiply(&m_oFullTransform, MatrixOrderPrepend);
		oM1.TransformPoint(_x, _y);

		pFont->SetTextMatrix((float)mass[0], (float)mass[1], (float)mass[2], (float)mass[3], (float)mass[4], (float)mass[5]);
		m_nTextRenderMode = pFont->m_nRENDER_MODE;

		pFont->LoadString2(pGids, nGidsCount, (float)_x, (float)_y);
		float fX = 0;
		float fY = 0;
		INT bRes = FALSE;

		while (TRUE)
		{
			TGlyph* pGlyph = NULL;

			bRes = pFont->GetNextChar2(pGlyph, fX, fY);

			if (FALSE == bRes)
				break;

			if (NULL != pGlyph)
			{
				FillGlyph2((int)fX, (int)fY, pGlyph, pBrush);
			}
		}

		return TRUE;
	}

	INT CGraphics::DrawStringC(const LONG& lText, CFontManager* pFont, CBrush* pBrush, double x, double y)
	{
		if (pBrush->GetType() != BrushTypeSolidColor)
			return TRUE;

		CMatrix oMatrix = m_oBaseTransform;
		oMatrix.Multiply(&m_oTransform, MatrixOrderPrepend);

		double mass[6];
		oMatrix.GetElements(mass);
		
		double _x = x;
		double _y = y;

		CMatrix oM1 = oMatrix;
		oM1.Invert();
		oM1.Multiply(&m_oFullTransform, MatrixOrderPrepend);
		oM1.TransformPoint(_x, _y);
		
		pFont->SetTextMatrix((float)mass[0], (float)mass[1], (float)mass[2], (float)mass[3], (float)mass[4], (float)mass[5]);
		m_nTextRenderMode = pFont->m_nRENDER_MODE;

		pFont->LoadString2C(lText, (float)_x, (float)_y);
		float fX = 0;
		float fY = 0;
		INT bRes = FALSE;

		while (TRUE)
		{
			TGlyph* pGlyph = NULL;

			bRes = pFont->GetNextChar2(pGlyph, fX, fY);

			if (FALSE == bRes)
				break;

			if (NULL != pGlyph)
			{
				FillGlyph2((int)fX, (int)fY, pGlyph, pBrush);
			}
		}

		return TRUE;
	}
	INT CGraphics::DrawStringPath(const std::wstring& strText, CFontManager* pFont, CBrush* pBrush, double x, double y)
	{
		if (pBrush->GetType() != BrushTypeSolidColor)
			return TRUE;

		Aggplus::CGraphicsPath oPath;
		oPath.AddString(strText, pFont, x, y);

		FillPath(pBrush, &oPath);
		
		return TRUE;
	}
	INT CGraphics::DrawStringPathC(const LONG& lText, CFontManager* pFont, CBrush* pBrush, double x, double y)
	{
		if (pBrush->GetType() != BrushTypeSolidColor)
			return TRUE;

		Aggplus::CGraphicsPath oPath;
		oPath.AddStringC(lText, pFont, x, y);

		FillPath(pBrush, &oPath);
		
		return TRUE;
	}

	void CGraphics::CalculateFullTransform()
	{
		m_oFullTransform	= m_oCoordTransform;
		m_oFullTransform.Multiply(&m_oBaseTransform, MatrixOrderAppend);
		m_oFullTransform.Multiply(&m_oTransform, MatrixOrderPrepend);
	}
	bool CGraphics::IsClip()
	{
		return m_oClip.IsClip();
	}

	template<class Renderer>
	void CGraphics::render_scanlines(Renderer& ren)
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
		}
	}

	template<class Renderer>
	void CGraphics::render_scanlines_alpha(Renderer& ren, BYTE Alpha)
	{
		double dAlpha = m_dGlobalAlpha * Alpha / 255.0;
		if (fabs(dAlpha - 1.0) < FLT_EPSILON)
		{
			render_scanlines(ren);
		}
		else
		{
			m_rasterizer.gamma_multi(dAlpha);
			render_scanlines(ren);
			m_rasterizer.gamma(1.0);
		}
	}

	template<class Rasterizer, class Renderer>
	void CGraphics::render_scanlines(Rasterizer& ras, Renderer& ren)
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
		}
	}

	void CGraphics::DoFillPathSolid(CColor dwColor)
	{
		if (m_nBlendMode != agg::comp_op_src_over)
		{
			typedef agg::renderer_scanline_aa_solid<comp_renderer_type> solid_comp_renderer_type;
			solid_comp_renderer_type ren_solid;
			comp_renderer_type ren_base;
			pixfmt_type_comp pixfmt;

			pixfmt.attach(m_frame_buffer.ren_buf());
			pixfmt.comp_op(m_nBlendMode);
			ren_base.attach(pixfmt);
			ren_solid.attach(ren_base);

			ren_solid.color(dwColor.GetAggColor());
			render_scanlines(ren_solid);
		}
		else
		{
			typedef agg::renderer_scanline_aa_solid<base_renderer_type> solid_renderer_type;
			solid_renderer_type ren_fine(m_frame_buffer.ren_base());
			ren_fine.color(dwColor.GetAggColor());

			render_scanlines(ren_fine);
		}
	}

	void CGraphics::DoFillPathGradient(CBrushLinearGradient *pBrush)
	{
		CDoubleRect& oBounds = pBrush->GetBounds();

		CMatrix oMatrix;

		agg::rect_d	rect;
		if (oBounds.GetWidth() > FLT_EPSILON || oBounds.GetHeight() > FLT_EPSILON)
		{
			rect.x1 = oBounds.left;
			rect.y1 = oBounds.top;
			rect.x2 = oBounds.right;
			rect.y2 = oBounds.bottom;

			oMatrix = m_oFullTransform;
			oMatrix.Invert();
		}
		else
		{
			int x = m_rasterizer.get_rasterizer().min_x();
			int y = m_rasterizer.get_rasterizer().min_y();
			int r = m_rasterizer.get_rasterizer().max_x();
			int b = m_rasterizer.get_rasterizer().max_y();

			if (r < x || b < y)
				return;

			rect.x1 = x;
			rect.x2 = r;
			rect.y1 = y;
			rect.y2 = b;
		}

		typedef agg::my_span_gradient<agg::rgba8> gradient_span_gen;
		gradient_span_gen span_gen;

		span_gen.SetDirection(rect, (double)pBrush->GetAngle(), oMatrix.m_internal->m_agg_mtx);

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
		
		typedef agg::span_allocator<gradient_span_gen::color_type> gradient_span_alloc;
		gradient_span_alloc span_alloc;

		typedef agg::renderer_scanline_aa<base_renderer_type, gradient_span_alloc, gradient_span_gen> renderer_gradient_type;
		renderer_gradient_type ren_gradient( m_frame_buffer.ren_base(), span_alloc, span_gen );

		if (fabs(m_dGlobalAlpha - 1.0) < FLT_EPSILON)
		{
			render_scanlines(ren_gradient);
		}
		else
		{
			m_rasterizer.gamma_multi(m_dGlobalAlpha);
			render_scanlines(ren_gradient);
			m_rasterizer.gamma(1.0);
		}

		if( pSubColors ) delete [] pSubColors;
		if( pSubBlends ) delete [] pSubBlends;
	}

	void CGraphics::DoFillPathGradient2(CBrushLinearGradient *pBrush)
	{
		CDoubleRect& oBounds = pBrush->GetBounds();

		CMatrix oMatrix;

		agg::rect_d	rect;
		if (oBounds.GetWidth() > FLT_EPSILON || oBounds.GetHeight() > FLT_EPSILON)
		{
			rect.x1 = oBounds.left;
			rect.y1 = oBounds.top;
			rect.x2 = oBounds.right;
			rect.y2 = oBounds.bottom;

			oMatrix = m_oFullTransform;
			oMatrix.Invert();
		}
		else
		{
			int x = m_rasterizer.get_rasterizer().min_x();
			int y = m_rasterizer.get_rasterizer().min_y();
			int r = m_rasterizer.get_rasterizer().max_x();
			int b = m_rasterizer.get_rasterizer().max_y();

			if (r < x || b < y)
				return;

			rect.x1 = x;
			rect.x2 = r;
			rect.y1 = y;
			rect.y2 = b;
		}

		typedef agg::my_span_path_gradient<agg::rgba8> gradient_span_gen;
		gradient_span_gen span_gen;

		span_gen.SetDirection(rect, oMatrix.m_internal->m_agg_mtx);

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
		
		typedef agg::span_allocator<gradient_span_gen::color_type> gradient_span_alloc;
		gradient_span_alloc span_alloc;

		typedef agg::renderer_scanline_aa<base_renderer_type, gradient_span_alloc, gradient_span_gen> renderer_gradient_type;
		renderer_gradient_type ren_gradient( m_frame_buffer.ren_base(), span_alloc, span_gen );

		if (fabs(m_dGlobalAlpha - 1.0) < FLT_EPSILON)
		{
			render_scanlines(ren_gradient);
		}
		else
		{
			m_rasterizer.gamma_multi(m_dGlobalAlpha);
			render_scanlines(ren_gradient);
			m_rasterizer.gamma(1.0);
		}

		if( pSubColors ) delete [] pSubColors;
		if( pSubBlends ) delete [] pSubBlends;
	}

	void CGraphics::DoFillPathHatch(CBrushHatch *pBrush)
	{
#if 0
		CDoubleRect& oBounds = pBrush->GetBounds();

		CMatrix oMatrix;

		agg::rect_d	rect;
		if (oBounds.GetWidth() > FLT_EPSILON || oBounds.GetHeight() > FLT_EPSILON)
		{
			rect.x1 = oBounds.left;
			rect.y1 = oBounds.top;
			rect.x2 = oBounds.right;
			rect.y2 = oBounds.bottom;

			oMatrix = m_oFullTransform;
		}
		else
		{
			int x = m_rasterizer.get_rasterizer().min_x();
			int y = m_rasterizer.get_rasterizer().min_y();
			int width  = m_rasterizer.get_rasterizer().max_x() - m_rasterizer.get_rasterizer().min_x();
			int height = m_rasterizer.get_rasterizer().max_y() - m_rasterizer.get_rasterizer().min_y();

			rect.x1 = x;
			rect.x2 = x + width;
			rect.y1 = y;
			rect.y2 = y + height;
		}

		typedef agg::agg_span_hatch<agg::rgba8> hatch_span_gen;
		hatch_span_gen span_gen;

		agg::rgba8 c1 = agg::rgba8(pBrush->m_dwColor1.GetB(), pBrush->m_dwColor1.GetG(), pBrush->m_dwColor1.GetR(), pBrush->m_dwColor1.GetA());
		agg::rgba8 c2 = agg::rgba8(pBrush->m_dwColor2.GetB(), pBrush->m_dwColor2.GetG(), pBrush->m_dwColor2.GetR(), pBrush->m_dwColor2.GetA());

		span_gen.SetDirection(pBrush->m_name, rect, oMatrix.m_agg_mtx, c1, c2);
		
		typedef agg::span_allocator<hatch_span_gen::color_type> hatch_span_alloc;
		hatch_span_alloc span_alloc;

		typedef agg::renderer_scanline_aa<base_renderer_type, hatch_span_alloc, hatch_span_gen> renderer_hatch_type;
		renderer_hatch_type ren_hatch( m_frame_buffer.ren_base(), span_alloc, span_gen );

		if (fabs(m_dGlobalAlpha - 1.0) < FLT_EPSILON)
		{
			render_scanlines(ren_hatch);
		}
		else
		{
			m_rasterizer.gamma_multi(m_dGlobalAlpha);
			render_scanlines(ren_hatch);
			m_rasterizer.gamma(1.0);
		}
#else
		agg::rgba8 c1 = agg::rgba8(pBrush->m_dwColor1.GetR(), pBrush->m_dwColor1.GetG(), pBrush->m_dwColor1.GetB(), pBrush->m_dwColor1.GetA());
		agg::rgba8 c2 = agg::rgba8(pBrush->m_dwColor2.GetR(), pBrush->m_dwColor2.GetG(), pBrush->m_dwColor2.GetB(), pBrush->m_dwColor2.GetA());

		BYTE* pPattern = new BYTE[HATCH_TX_SIZE * HATCH_TX_SIZE * 4];
		agg::GetHatchPattern(pBrush->m_name, (agg::rgba8*)pPattern, c1, c2);

		agg::trans_affine mtx_Work(m_oTransform.m_internal->m_agg_mtx);
		if (m_dDpiTile > 1)
			mtx_Work.scale(m_dDpiX / m_dDpiTile, m_dDpiY / m_dDpiTile);

		mtx_Work.invert();

		span_alloc_type				span_allocator;
		interpolator_type_linear	interpolator(mtx_Work);

		agg::rendering_buffer PatRendBuff;
		PatRendBuff.attach(pPattern, HATCH_TX_SIZE, HATCH_TX_SIZE, HATCH_TX_SIZE << 2);

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
		
		if (fabs(m_dGlobalAlpha - 1.0) < FLT_EPSILON)
		{
			render_scanlines(ri);
		}
		else
		{
			m_rasterizer.gamma_multi(m_dGlobalAlpha);
			render_scanlines(ri);
			m_rasterizer.gamma(1.0);
		}

		RELEASEARRAYOBJECTS(pPattern);
#endif
	}

	void CGraphics::DoFillPathTextureClampSz(const CMatrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride)
	{
		span_alloc_type span_allocator;                  // Span Allocator

		typedef agg::pixfmt_rgba32     pixfmt;
		typedef agg::image_accessor_clip<pixfmt> img_source_type;
		typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
		typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
		agg::trans_affine mtx_Work(mImgMtx.m_internal->m_agg_mtx);
		mtx_Work.multiply(m_oFullTransform.m_internal->m_agg_mtx);
		mtx_Work.invert();
		interpolator_type_linear interpolator(mtx_Work);
		{
			//agg::rendering_buffer PatRendBuff((BYTE *)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);

			agg::rendering_buffer PatRendBuff;
			PatRendBuff.attach((BYTE*)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);
			
			pixfmt          img_pixf(PatRendBuff);
			img_source_type img_src(img_pixf, agg::rgba(0, 0, 0, 0));
			span_gen_type sg(img_src, interpolator);
			renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
			//agg::render_scanlines(m_rasterizer.get_rasterizer(), m_rasterizer.get_scanline(), ri);
			render_scanlines(ri);
		}
	}

	void CGraphics::DoFillPathTextureClampSz2(const CMatrix &mImgMtx, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride, BYTE Alpha)
	{
		span_alloc_type span_allocator;
		agg::trans_affine mtx_Work(mImgMtx.m_internal->m_agg_mtx);
		mtx_Work.invert();
		interpolator_type_linear interpolator(mtx_Work);

		agg::rendering_buffer PatRendBuff;
		PatRendBuff.attach((BYTE*)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);

		int nCurrentMode = 255;
		if (!m_bSwapRGB)
		{
			typedef agg::pixfmt_bgra32     pixfmt;
			typedef agg::image_accessor_clone<pixfmt> img_source_type;

			pixfmt          img_pixf(PatRendBuff);
			img_source_type img_src(img_pixf);

			switch (nCurrentMode)
			{
			case 0:
			{
				typedef agg::span_image_filter_rgba_nn<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				span_gen_type sg(img_src, interpolator);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 1:
			{
				typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				span_gen_type sg(img_src, interpolator);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 2:
			{
				typedef agg::span_image_filter_rgba_2x2<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				agg::image_filter_lut filter;
				filter.calculate(agg::image_filter_bicubic(), false);
				span_gen_type sg(img_src, interpolator, filter);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 3:
			{
				typedef agg::span_image_filter_rgba_2x2<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				agg::image_filter_lut filter;
				filter.calculate(agg::image_filter_spline16(), false);
				span_gen_type sg(img_src, interpolator, filter);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 4:
			{
				typedef agg::span_image_filter_rgba_2x2<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				agg::image_filter_lut filter;
				filter.calculate(agg::image_filter_blackman256(), false);
				span_gen_type sg(img_src, interpolator, filter);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 255:
			{
				typedef agg::span_image_resample_rgba_affine_for_draw<img_source_type> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				agg::image_filter_lut filter;
				filter.calculate(agg::image_filter_bilinear(), false);
				span_gen_type sg(img_src, interpolator, filter);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			default:
				break;
			}
		}
		else
		{
			typedef agg::pixfmt_rgba32     pixfmt;
			typedef agg::image_accessor_clone<pixfmt> img_source_type;

			pixfmt          img_pixf(PatRendBuff);
			img_source_type img_src(img_pixf);

			switch (nCurrentMode)
			{
			case 0:
			{
				typedef agg::span_image_filter_rgba_nn<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				span_gen_type sg(img_src, interpolator);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 1:
			{
				typedef agg::span_image_filter_rgba_bilinear<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				span_gen_type sg(img_src, interpolator);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 2:
			{
				typedef agg::span_image_filter_rgba_2x2<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				agg::image_filter_lut filter;
				filter.calculate(agg::image_filter_bicubic(), false);
				span_gen_type sg(img_src, interpolator, filter);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 3:
			{
				typedef agg::span_image_filter_rgba_2x2<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				agg::image_filter_lut filter;
				filter.calculate(agg::image_filter_spline16(), false);
				span_gen_type sg(img_src, interpolator, filter);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 4:
			{
				typedef agg::span_image_filter_rgba_2x2<img_source_type, interpolator_type_linear> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				agg::image_filter_lut filter;
				filter.calculate(agg::image_filter_blackman256(), false);
				span_gen_type sg(img_src, interpolator, filter);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			case 255:
			{
				typedef agg::span_image_resample_rgba_affine_for_draw<img_source_type> span_gen_type;
				typedef agg::renderer_scanline_aa<base_renderer_type, span_alloc_type, span_gen_type> renderer_type;
				agg::image_filter_lut filter;
				filter.calculate(agg::image_filter_bilinear(), false);
				span_gen_type sg(img_src, interpolator, filter);
				renderer_type ri(m_frame_buffer.ren_base(), span_allocator, sg);
				render_scanlines_alpha(ri, Alpha);
				break;
			}
			default:
				break;
			}
		}
	}

	template<class ColorSpacePix>
	void CGraphics::DoFillPathTextureClampSz3(const CMatrix &matrix, const void *pImgBuff, DWORD dwImgWidth, DWORD dwImgHeight, int nImgStride, Aggplus::WrapMode wrapmode, BYTE Alpha)
	{
		agg::trans_affine mtx_Work( matrix.m_internal->m_agg_mtx );

		agg::trans_affine coords = m_oCoordTransform.m_internal->m_agg_mtx;
		coords.invert();
		mtx_Work.premultiply(coords);

		//mtx_Work.multiply(m_oFullTransform.m_agg_mtx);
		mtx_Work.invert();

		span_alloc_type span_allocator;                  // Span Allocator
		interpolator_type_linear interpolator(mtx_Work);

		//agg::rendering_buffer PatRendBuff((BYTE *)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);
		agg::rendering_buffer PatRendBuff;
		PatRendBuff.attach((BYTE*)pImgBuff, dwImgWidth, dwImgHeight, nImgStride);

		typedef ColorSpacePix     pixfmt;
		if(wrapmode == WrapModeTileFlipX)
		{
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

			double dAlpha = m_dGlobalAlpha * Alpha / 255.0;
			if (fabs(dAlpha - 1.0) < FLT_EPSILON)
			{
				render_scanlines(ri);
			}
			else
			{
				m_rasterizer.gamma_multi(dAlpha);
				render_scanlines(ri);
				m_rasterizer.gamma(1.0);
			}
		}
		else if(wrapmode == WrapModeTileFlipY)
		{
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

			double dAlpha = m_dGlobalAlpha * Alpha / 255.0;
			if (fabs(dAlpha - 1.0) < FLT_EPSILON)
			{
				render_scanlines(ri);
			}
			else
			{
				m_rasterizer.gamma_multi(dAlpha);
				render_scanlines(ri);
				m_rasterizer.gamma(1.0);
			}
		}
		else if(wrapmode == WrapModeTileFlipXY)
		{
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

			double dAlpha = m_dGlobalAlpha * Alpha / 255.0;
			if (fabs(dAlpha - 1.0) < FLT_EPSILON)
			{
				render_scanlines(ri);
			}
			else
			{
				m_rasterizer.gamma_multi(dAlpha);
				render_scanlines(ri);
				m_rasterizer.gamma(1.0);
			}
		}
		else //Repeat
		{
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

			double dAlpha = m_dGlobalAlpha * Alpha / 255.0;
			if (fabs(dAlpha - 1.0) < FLT_EPSILON)
			{
				render_scanlines(ri);
			}
			else
			{
				m_rasterizer.gamma_multi(dAlpha);
				render_scanlines(ri);
				m_rasterizer.gamma(1.0);
			}
		}
	}

	void CGraphics::DoFillPath(const CBrush* Brush)
	{
		if (NULL == Brush)
			return;

		Aggplus::BrushType eBrushType = Brush->GetType();
		switch (eBrushType)
		{
		case BrushTypeSolidColor:
		{
			CColor clr;
			((CBrushSolid*)Brush)->GetColor(&clr);
			DoFillPathSolid(clr);
			break;
		}
		case BrushTypeHatchFill:
		{
			DoFillPathHatch((Aggplus::CBrushHatch*)Brush);
			break;
		}
		case BrushTypeTextureFill:
		{
			CBrushTexture *ptxBrush = (CBrushTexture *)Brush;

			LPVOID pImgBuff = ptxBrush->GetData();
			if (pImgBuff)
			{
				DWORD dwImgWidth = ptxBrush->PatternGetWidth();
				DWORD dwImgHeight = ptxBrush->PatternGetHeight();
				int nImgStride = ptxBrush->PatternGetStride();

				if(pImgBuff && dwImgWidth && dwImgHeight)
				{
					Aggplus::WrapMode wrapmode = ptxBrush->m_wrapMode;
					Aggplus::CMatrix matrix = ptxBrush->m_mtx;

					if(wrapmode == WrapModeClamp)
					{
						DoFillPathTextureClampSz2( matrix, pImgBuff, dwImgWidth, dwImgHeight, nImgStride, ptxBrush->Alpha);
					}
					else
					{
						if (!m_bSwapRGB)
						{
							DoFillPathTextureClampSz3<agg::pixfmt_bgra32>(matrix, pImgBuff, dwImgWidth, dwImgHeight, nImgStride, wrapmode, ptxBrush->Alpha);
						}
						else
						{
							DoFillPathTextureClampSz3<agg::pixfmt_rgba32>(matrix, pImgBuff, dwImgWidth, dwImgHeight, nImgStride, wrapmode, ptxBrush->Alpha);
						}
					}
				}
			}
			break;
		}
		case BrushTypeLinearGradient:
		{
			DoFillPathGradient((CBrushLinearGradient*)Brush);
			break;
		}
		case BrushTypePathGradient:
		{
			DoFillPathGradient2((CBrushLinearGradient*)Brush);
			break;
		}
		case BrushTypeMyTestGradient:
		case BrushTypeNewLinearGradient:
		case BrushTypeConicalGradient:
		case BrushTypeRadialGradient:
		case BrushTypeDiamondGradient:
		case BrushTypeTriagnleMeshGradient:
		case BrushTypeCurveGradient:
		case BrushTypeTensorCurveGradient:
		{
			DoFillPathGradientType((CBrushLinearGradient*)Brush);
			break;
		}
		default:
			break;
		}
	}
	// text methods
	int CGraphics::FillGlyph2(int nX, int nY, TGlyph* pGlyph, Aggplus::CBrush* pBrush)
	{
		int lWidth = pGlyph->oBitmap.nWidth;
		int lHeight = pGlyph->oBitmap.nHeight;

		BYTE* pData = pGlyph->oBitmap.pData;

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
	int CGraphics::FillGlyphSimple(int nX, int nY, LONG lWidth, LONG lHeight, BYTE* pData, Aggplus::CBrush* pBrush)
	{
		CColor clr;
		((CBrushSolid*)pBrush)->GetColor(&clr);

		typedef agg::renderer_scanline_aa_solid<base_renderer_type> solid_renderer_type;
		solid_renderer_type ren_fine(m_frame_buffer.ren_base());
		ren_fine.color(clr.GetAggColor());

		if (m_nTextRenderMode == FT_RENDER_MODE_LCD)
		{
			ren_fine.render_subpix(lWidth / 3, lHeight, pData, nX, nY);
		}
		else
		{
			ren_fine.render(lWidth, lHeight, pData, nX, nY);
		}
		return 0;
	}

	void CGraphics::PrepareScanlineStorage(agg::scanline_storage_aa8* storage, int x, int y, TGlyphBitmap *pGlyph)
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

	void CGraphics::UpdateUnits()
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


	// Testing
	void CGraphics::DoFillPathGradientType(CBrushLinearGradient *pBrush)
	{
		CDoubleRect& oBounds = pBrush->GetBounds();

		CMatrix oMatrix;

		agg::rect_d	rect;
		if (oBounds.GetWidth() > FLT_EPSILON || oBounds.GetHeight() > FLT_EPSILON)
		{
			rect.x1 = oBounds.left;
			rect.y1 = oBounds.top;
			rect.x2 = oBounds.right;
			rect.y2 = oBounds.bottom;

			oMatrix = m_oFullTransform;
			oMatrix.Invert();
		}
		else
		{
			int x = m_rasterizer.get_rasterizer().min_x();
			int y = m_rasterizer.get_rasterizer().min_y();
			int r = m_rasterizer.get_rasterizer().max_x();
			int b = m_rasterizer.get_rasterizer().max_y();

			if (r < x || b < y)
				return;

			rect.x1 = x;
			rect.x2 = r;
			rect.y1 = y;
			rect.y2 = b;
		}

		ScaleGranientInfo(pBrush->GetType(), pBrush->m_oGradientInfo);

		typedef agg::gradient_base<agg::rgba8> gradient_span_gen;
		gradient_span_gen span_gen;

		span_gen.SetDirection(rect, oMatrix.m_internal->m_agg_mtx, m_bSwapRGB);
		span_gen.SetGradientInfo(pBrush->m_oGradientInfo, pBrush->GetType());
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
		
		typedef agg::span_allocator<gradient_span_gen::color_type> gradient_span_alloc;
		gradient_span_alloc span_alloc;

		typedef agg::renderer_scanline_aa<base_renderer_type, gradient_span_alloc, gradient_span_gen> renderer_gradient_type;
		renderer_gradient_type ren_gradient( m_frame_buffer.ren_base(), span_alloc, span_gen );

		if (fabs(m_dGlobalAlpha - 1.0) < FLT_EPSILON)
		{
			render_scanlines(ren_gradient);
		}
		else
		{
			m_rasterizer.gamma_multi(m_dGlobalAlpha);
			render_scanlines(ren_gradient);
			m_rasterizer.gamma(1.0);
		}

		if( pSubColors ) delete [] pSubColors;
		if( pSubBlends ) delete [] pSubBlends;
	}

	void CGraphics::ScaleGranientInfo(long type, NSStructures::GradientInfo &ginfo)
	{
		if (type == BrushTypeMyTestGradient)
		{
			std::vector<float> new_map(6);
			float M[6];
			std::vector<float> G = ginfo.shading.mapping;
			m_oFullTransform.GetElements(M);

			new_map[0] = M[0] * G[0] + M[2] * G[1];
			new_map[1] = M[1] * G[0] + M[3] * G[1];
			new_map[2] = M[0] * G[2] + M[2] * G[3];
			new_map[3] = M[1] * G[2] + M[3] * G[3];

			new_map[4] = M[0] * G[4] + M[2] * G[5] + M[4];
			new_map[5] = M[1] * G[4] + M[3] * G[5] + M[5];
			ginfo.shading.mapping = new_map;

			float D = new_map[0] * new_map[3] - new_map[1] * new_map[2];

			ginfo.shading.inv_map[0] = new_map[3] / D;
			ginfo.shading.inv_map[1] = -new_map[1] / D;
			ginfo.shading.inv_map[2] = -new_map[2] / D;
			ginfo.shading.inv_map[3] = new_map[0] / D;

			ginfo.shading.inv_map[4] = -new_map[4];
			ginfo.shading.inv_map[5] = -new_map[5];
			return;
		}
		if (type == BrushTypeNewLinearGradient)
		{
			ScaleCoords(ginfo.shading.point1.x, ginfo.shading.point1.y);
			ScaleCoords(ginfo.shading.point2.x, ginfo.shading.point2.y);
			return;
		}
		if (type == BrushTypeRadialGradient)
		{
			ginfo.r0 = ginfo.r0 * sqrt(fabs(m_oFullTransform.Determinant()));
			ginfo.r1 = ginfo.r1 * sqrt(fabs(m_oFullTransform.Determinant()));
			ScaleCoords(ginfo.p0.x, ginfo.p0.y);
			ScaleCoords(ginfo.p1.x, ginfo.p1.y);
			return;
		}
		if (type == BrushTypeTriagnleMeshGradient)
		{
			for (int i = 0; i < 3; i++)
			{
				ScaleCoords(ginfo.shading.triangle[i].x, ginfo.shading.triangle[i].y);
			}
			return;
		}
		if (type == BrushTypeCurveGradient || type == BrushTypeTensorCurveGradient)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					ScaleCoords(ginfo.shading.patch[i][j].x, ginfo.shading.patch[i][j].y);
				}
			}
			return;
		}
	}
	void CGraphics::ScaleCoords(float &x, float &y)
	{
		double newx = x;
		double newy = y;
		m_oFullTransform.TransformPoint(newx, newy);
		x = newx;
		y = newy;
		return;
	}
}
