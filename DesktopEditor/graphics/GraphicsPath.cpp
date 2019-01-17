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
#include "GraphicsPath_private.h"
#include <algorithm>

namespace Aggplus
{
	// GraphicsPath
	CGraphicsPath::CGraphicsPath() : ISimpleGraphicsPath()
	{
        m_internal = new CGraphicsPath_private();
	}

	CGraphicsPath::~CGraphicsPath()
	{
        RELEASEOBJECT(m_internal);
	}

	CGraphicsPath* CGraphicsPath::Clone()
	{
		CGraphicsPath* pNew = new CGraphicsPath();
        pNew->m_internal->m_agg_ps		= m_internal->m_agg_ps;
        pNew->m_internal->m_bEvenOdd	= m_internal->m_bEvenOdd;
        pNew->m_internal->m_bIsMoveTo	= m_internal->m_bIsMoveTo;
		return pNew;
	}

	Status CGraphicsPath::Reset()
	{
        m_internal->m_agg_ps.remove_all();
        m_internal->m_bIsMoveTo = false;
		return Ok;
	}

	void CGraphicsPath::SetRuler(bool bEvenOdd)
	{
        m_internal->m_bEvenOdd = bEvenOdd;
	}

	Status CGraphicsPath::StartFigure() 
	{ 
        m_internal->m_agg_ps.start_new_path();
		return Ok; 
	}

	Status CGraphicsPath::CloseFigure() 
	{ 
        m_internal->m_agg_ps.close_polygon();
		return Ok; 
	}

	bool CGraphicsPath::Is_poly_closed()
	{
        if (!m_internal->m_agg_ps.total_vertices())
			return true;

		double x, y;
        unsigned int nTip = m_internal->m_agg_ps.last_vertex(&x, &y);
		
		if (nTip & agg::path_flags_close) 
			return true;

		return false;
	}

	Status CGraphicsPath::MoveTo(double x, double y)
	{
        m_internal->m_bIsMoveTo = true;
        m_internal->m_agg_ps.move_to(x, y);
		return Ok;
	}
	Status CGraphicsPath::LineTo(double x, double y)
	{
        m_internal->m_agg_ps.line_to(x, y);
		return Ok;
	}
	Status CGraphicsPath::CurveTo(double x1, double y1, double x2, double y2, double x3, double y3)
	{
        m_internal->m_agg_ps.curve4(x1, y1, x2, y2, x3, y3);
		return Ok;
	}

	Status CGraphicsPath::AddLine(double x1, double y1, double x2, double y2)
	{
		 if (Is_poly_closed()) 
		 {
             m_internal->m_agg_ps.move_to(x1, y1);
		 }
		 else
		 {
             m_internal->m_agg_ps.line_to(x1, y1);
		 }

         m_internal->m_agg_ps.line_to(x2, y2);
		 return Ok;
	}

	Status CGraphicsPath::AddLines(double* pPoints, int nCount)
	{
		if (4 > nCount)
		{
			return InvalidParameter;
		}
		int nRet = 0;

        if (!m_internal->m_bIsMoveTo)
		{
			MoveTo(pPoints[0], pPoints[1]);
		}

		/*if (Is_poly_closed()) 
		{
			m_agg_ps.move_to((double)pPoints[0], (double)pPoints[1]);
		}
		else
		{
			m_agg_ps.line_to((double)pPoints[0], (double)pPoints[1]);
		}*/

		int n = (nCount / 2) - 1;

		for (int i = 1; i <= n; ++i)
		{
			const double* points = &pPoints[i * 2];
            m_internal->m_agg_ps.line_to(points[0], points[1]);
		}
		return Ok;
	}
	
	Status CGraphicsPath::AddBezier(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
	{
		if (Is_poly_closed()) 
            m_internal->m_agg_ps.move_to(x1, y1);
		else   
            m_internal->m_agg_ps.line_to(x1, y1);

        m_internal->m_agg_ps.curve4(x2, y2, x3, y3, x4, y4);
		return Ok;
	}
	Status CGraphicsPath::AddBeziers(double* pPoints, int nCount)
	{
		if (8 > nCount)
			return InvalidParameter;

        if (!m_internal->m_bIsMoveTo)
		{
			MoveTo(pPoints[0], pPoints[1]);
		}

		const double* points = pPoints;
		
		agg::curve4 curve;
		curve.approximation_method(agg::curve_inc);
		curve.approximation_scale(25.0);
		curve.init(points[0], points[1], points[2], points[3], points[4], points[5], points[6], points[7]);

		if (Is_poly_closed()) 
		{
            m_internal->m_agg_ps.concat_path(curve, 0);
		}
		else
		{
            m_internal->m_agg_ps.join_path(curve, 0);
		}

		int nCountTo = (nCount - 8) / 6;
		for (int i = 0; i < nCountTo; ++i)
		{
			points = pPoints + 8 + 6 * i;
			CurveTo(points[0], points[1], points[2], points[3], points[4], points[5]);
		}

		return Ok;
	}
	
	Status CGraphicsPath::AddCurve(double* pPoints, int nCount)
	{
		// этим мы не пользуемся. Понадобится - реализую.
		return AddBeziers(pPoints, nCount);
	}

	Status CGraphicsPath::AddEllipse(double x, double y, double width, double height)
	{
		agg::bezier_arc arc(x+width/2.0, y+height/2.0, width/2.0, height/2.0, 0.0, agg::pi2);
		//2.3 m_agg_ps.add_path(arc, 0, true);
        m_internal->m_agg_ps.join_path(arc, 0);
		return Ok;
	}
	Status CGraphicsPath::AddRectangle(double x, double y, double width, double height)
	{
        m_internal->m_agg_ps.move_to(x, y);
        m_internal->m_agg_ps.line_to(x + width, y);
        m_internal->m_agg_ps.line_to(x + width, y + height);
        m_internal->m_agg_ps.line_to(x, y + height);
        m_internal->m_agg_ps.close_polygon();
		return Ok;
	}
	Status CGraphicsPath::AddPolygon(double* pPoints, int nCount)
	{
		if (2 > nCount)
		{
			return InvalidParameter;
		}
		int nRet = 0;

		if (Is_poly_closed()) 
		{
            m_internal->m_agg_ps.move_to(pPoints[0], pPoints[1]);
		}
		else
		{
            m_internal->m_agg_ps.line_to(pPoints[0], pPoints[1]);
		}

		int n = (nCount / 2) - 1;

		for (int i = 1; i < n; ++i)
		{
			double* points = &pPoints[i * 2];
            m_internal->m_agg_ps.line_to(points[0], points[1]);
		}

        m_internal->m_agg_ps.close_polygon();
		return Ok;
	}
	Status CGraphicsPath::AddPath(const CGraphicsPath& oPath)
	{
		typedef agg::conv_curve<agg::path_storage> conv_crv_type;

        agg::path_storage p_copy(oPath.m_internal->m_agg_ps);
		conv_crv_type p3(p_copy);

        m_internal->m_agg_ps.join_path(p3, 0);
		return Ok;
	}
	Status CGraphicsPath::AddArc(double x, double y, double width, double height, double startAngle, double sweepAngle)
	{
		if(sweepAngle >= 360.0)
		{
			sweepAngle = 360;
		}
		
		agg::bezier_arc arc(x+width/2.00, y+height/2.00, width/2.00, height/2.00, agg::deg2rad(startAngle), agg::deg2rad(sweepAngle));
		
		//2.3 m_agg_ps.add_path(arc, 0,  !z_is_poly_closed());

		if (Is_poly_closed()) 
		{
            m_internal->m_agg_ps.concat_path(arc, 0);
		}
		else
		{
            m_internal->m_agg_ps.join_path(arc, 0);
		}

		return Ok;
	}

	ULONG CGraphicsPath::GetPointCount() const
	{
		ULONG nPointCount=0;
        ULONG nTotal = m_internal->m_agg_ps.total_vertices();
		
		double x, y;
		
		for(ULONG i = 0; i < nTotal; ++i)
		{
            ULONG nTip = m_internal->m_agg_ps.vertex(i, &x, &y);
			if(nTip)
			{
				if (!(nTip & agg::path_flags_close))
				{
					++nPointCount;
				}
			}
		}
		return nPointCount;
	}

	Status CGraphicsPath::GetPathPoints(PointF* points, int count) const
	{
        int nTotal = m_internal->m_agg_ps.total_vertices();
		double x, y;
		int i = 0, k = 0;
		
		while (k < count && i < nTotal)
		{
            unsigned int nTip = m_internal->m_agg_ps.vertex(i, &x, &y);
			if (nTip)
			{
				if(!(nTip & agg::path_flags_close))
				{
					points[k].X = REAL(x);
					points[k].Y = REAL(y);
					++k;
				}
			}
			++i;
		}

		return Ok;
	}

	Status CGraphicsPath::GetLastPoint(double& x, double& y)
	{
        m_internal->m_agg_ps.last_vertex(&x, &y);
		return Ok;
	}

	Status CGraphicsPath::GetPathPoints(double* points, int count) const
	{
        int nTotal = m_internal->m_agg_ps.total_vertices();
		double x, y;
		int i = 0, k = 0;
		
		while (k < count && i < nTotal)
		{
            unsigned int nTip = m_internal->m_agg_ps.vertex(i, &x, &y);
			if (nTip)
			{
				if(!(nTip & agg::path_flags_close))
				{
					points[2 * k]		= x;
					points[2 * k + 1]   = y;
					++k;
				}
			}
			++i;
		}

		return Ok;
	}

	void CGraphicsPath::GetBounds(double& left, double& top, double& width, double& height)
	{
        unsigned int nTotal = m_internal->m_agg_ps.total_vertices();
		if (nTotal)
		{
			agg::rect_d bounds(1e100, 1e100, -1e100, -1e100);
			double x, y;
			for(unsigned int i = 0; i < nTotal; i++)
			{
                unsigned int nTip = m_internal->m_agg_ps.vertex(i, &x, &y);
				if(agg::is_vertex(nTip))
				{
					if(x < bounds.x1) bounds.x1 = x;
					if(y < bounds.y1) bounds.y1 = y;
					if(x > bounds.x2) bounds.x2 = x;
					if(y > bounds.y2) bounds.y2 = y;
				}
			}
			
			left	= bounds.x1;
			top		= bounds.y1;
			width	= (bounds.x2 - bounds.x1);
			height	= (bounds.y2 - bounds.y1);
		}
		else 
		{
			left	= 0;
			top		= 0;
			width	= 0;
			height	= 0;
		}
	}

	Status CGraphicsPath::Transform(const CMatrix* matrix)
	{
		if (NULL != matrix)
		{
            agg::path_storage p2(m_internal->m_agg_ps);
            agg::conv_transform<agg::path_storage> trans(p2, matrix->m_internal->m_agg_mtx);
            m_internal->m_agg_ps.remove_all();
			//2.3 m_agg_ps.add_path(trans, 0, false);
            m_internal->m_agg_ps.concat_path(trans, 0);
		}
		return Ok;
	}

	bool CGraphicsPath::_MoveTo(double x, double y)
	{
        if (NULL != m_internal->m_pTransform)
		{
            m_internal->m_pTransform->TransformPoint(x, y);
		}
		return (Ok == MoveTo(x, y));
	}
	bool CGraphicsPath::_LineTo(double x, double y)
	{
        if (NULL != m_internal->m_pTransform)
		{
            m_internal->m_pTransform->TransformPoint(x, y);
		}
		return (Ok == LineTo(x, y));
	}
	bool CGraphicsPath::_CurveTo(double x1, double y1, double x2, double y2, double x3, double y3)
	{
        if (NULL != m_internal->m_pTransform)
		{
            m_internal->m_pTransform->TransformPoint(x1, y1);
            m_internal->m_pTransform->TransformPoint(x2, y2);
            m_internal->m_pTransform->TransformPoint(x3, y3);
		}
		return (Ok == CurveTo(x1, y1, x2, y2, x3, y3));
	}
	bool CGraphicsPath::_Close()
	{
		return (Ok == CloseFigure());
	}

    Status CGraphicsPath::AddString(const std::wstring& strText, NSFonts::IFontManager* pFont, double x, double y)
	{
		if (NULL == pFont)
			return InvalidParameter;
		
		pFont->SetTextMatrix(1, 0, 0, 1, 0, 0);
		pFont->LoadString1(strText, (float)x, (float)y);
		return (TRUE == pFont->GetStringPath(this)) ? Ok : InvalidParameter;
	}
    Status CGraphicsPath::AddString(const unsigned int* pGids, const unsigned int nGidsCount, NSFonts::IFontManager* pFont, double x, double y)
    {
        if (NULL == pFont)
            return InvalidParameter;

		pFont->SetTextMatrix(1, 0, 0, 1, 0, 0);
        pFont->LoadString1(pGids, nGidsCount, (float)x, (float)y);
        return (TRUE == pFont->GetStringPath(this)) ? Ok : InvalidParameter;
    }

    Status CGraphicsPath::AddStringC(const LONG& lText, NSFonts::IFontManager* pFont, double x, double y)
	{
		if (NULL == pFont)
			return InvalidParameter;

		unsigned int _c = (int)lText;
		pFont->SetTextMatrix(1, 0, 0, 1, 0, 0);
		pFont->LoadString1(&_c, 1, (float)x, (float)y);
		return (TRUE == pFont->GetStringPath(this)) ? Ok : InvalidParameter;
	}

    void CGraphicsPath::z_Stroke(const double& size)
	{
		typedef agg::conv_stroke<agg::path_storage> Path_Conv_Stroke;
        Path_Conv_Stroke pg(m_internal->m_agg_ps);
		pg.line_join(agg::round_join);
		pg.line_cap(agg::round_cap);
		pg.approximation_scale(25.00);
		//pg.miter_limit(0.50);

        pg.width(size);
		//pg.auto_detect_orientation(true);

		agg::path_storage psNew;
		//2.3 psNew.add_path(pg, 0, false);
		psNew.concat_path(pg, 0);

        m_internal->m_agg_ps = psNew;
	}

    void CGraphicsPath::Widen(const double& size, const Aggplus::LineJoin& join, const CMatrix* matrix, float flatness)
	{
        if (NULL == matrix || 0.0f == flatness)
			return;

		typedef agg::conv_curve<agg::path_storage> conv_crv_type;

		typedef agg::conv_contour<conv_crv_type> Path_Conv_Contour;

        conv_crv_type crv(m_internal->m_agg_ps);
		Path_Conv_Contour pg(crv);

		pg.miter_limit(0.50);
		//pg.miter_limit_theta(0.05);
		//pg.approximation_scale(2.00);

        pg.width(size);

		agg::line_join_e LineJoin;
        switch (join)
		{
		case LineJoinMiter       : LineJoin=agg::miter_join; break;
		case LineJoinBevel       : LineJoin=agg::bevel_join; break;
		default:
		case LineJoinRound       : LineJoin=agg::round_join; break;
		case LineJoinMiterClipped: LineJoin=agg::miter_join_revert; break;
		}
		pg.line_join(LineJoin);

		pg.auto_detect_orientation(false);

		agg::path_storage psNew;
		//2.3 psNew.add_path(pg, 0, false);
		//m_agg_ps.concat_path(pg, 0);
        m_internal->m_agg_ps.concat_path(pg, 0);
        m_internal->m_agg_ps = psNew;
	}

	int CGraphicsPath::EllipseArc(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection)
	{
		int nRet = 0;
		
		while ( fAngle1 < 0 )
			fAngle1 += 360;

		while ( fAngle1 > 360 )
			fAngle1 -= 360;

		while ( fAngle2 < 0 )
			fAngle2 += 360;

		while ( fAngle2 >= 360 )
			fAngle2 -= 360;

		if ( !bClockDirection )
		{
			if ( fAngle1 <= fAngle2 )
				nRet = EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, fAngle2, FALSE );
			else
			{
				nRet += EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, 360, FALSE );
				nRet += EllipseArc2( fX, fY, fXRad, fYRad, 0, fAngle2, FALSE );
			}
		}
		else
		{
			if ( fAngle1 >= fAngle2 )
				nRet = EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, fAngle2, TRUE );
			else
			{
				nRet += EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, 0, TRUE );
				nRet += EllipseArc2( fX, fY, fXRad, fYRad, 360, fAngle2, TRUE );
			}
		}
		return nRet;
	}

	double CGraphicsPath::AngToEllPrm(double fAngle, double fXRad, double fYRad)
	{
		// Функция для перевода реального угла в параметрическое задание эллписа
		// т.е. x= a cos(t) y = b sin(t) - параметрическое задание эллписа.
		// x = r cos(p), y = r sin(p) => t = atan2( sin(p) / b, cos(p) / a );
		return atan2( sin( fAngle ) / fYRad,  cos( fAngle ) / fXRad );
	}

	int CGraphicsPath::EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection)
	{
		// переведем углы в радианы
		int nRet = 0;

		double dAngle1 = fAngle1 * 3.141592 / 180;
		double dAngle2 = fAngle2 * 3.141592 / 180;

		// Выясним в каких четвертях находятся начальная и конечная точки
		unsigned int nFirstPointQuard  = int(fAngle1) / 90 + 1; 
		unsigned int nSecondPointQuard = int(fAngle2) / 90 + 1;
        nSecondPointQuard = std::min( 4, std::max( 1, (int)nSecondPointQuard ) );
        nFirstPointQuard  = std::min( 4, std::max( 1, (int)nFirstPointQuard ) );
		// Проведем линию в начальную точку дуги
		double fStartX = 0.0, fStartY = 0.0, fEndX = 0.0, fEndY = 0.0;

		fStartX = fX + fXRad * cos( AngToEllPrm( dAngle1, fXRad, fYRad ) );
		fStartY = fY + fYRad * sin( AngToEllPrm( dAngle1, fXRad, fYRad ) );

		LineTo(fStartX, fStartY);

		// Дальше рисуем по четверям

		double fCurX = fStartX, fCurY = fStartY;
		double dStartAngle = dAngle1;
		double dEndAngle = 0;

		if ( !bClockDirection )
		{
			for( unsigned int nIndex = nFirstPointQuard; nIndex <= nSecondPointQuard; nIndex++ ) 
			{
				if ( nIndex == nSecondPointQuard )
					dEndAngle = dAngle2;
				else
					dEndAngle = (90 * (nIndex ) ) * 3.141592f / 180;
				if ( !( nIndex == nFirstPointQuard ) )
					dStartAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;

				EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
			}
		}
		else
		{
			for( unsigned int nIndex = nFirstPointQuard; nIndex >= nSecondPointQuard; nIndex-- ) 
			{
				if ( nIndex == nFirstPointQuard )
					dStartAngle = dAngle1;
				else
					dStartAngle = (90 * (nIndex ) ) * 3.141592f / 180;
				if ( !( nIndex == nSecondPointQuard ) )
					dEndAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;
				else
					dEndAngle = dAngle2;

				EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
			}
		}

		return nRet;
	}

	int CGraphicsPath::EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double *pfXCur, double *pfYCur, INT bClockDirection)
	{
		// Рассчитаем начальную, конечную и контрольные точки
		double fX1  = 0.0, fX2  = 0.0, fY1  = 0.0, fY2  = 0.0;
		double fCX1 = 0.0, fCX2 = 0.0, fCY1 = 0.0, fCY2 = 0.0;

		double fAlpha = sin( dAngle2 - dAngle1 ) * ( sqrt( 4.0 + 3.0 * tan( (dAngle2 - dAngle1) / 2.0 ) * tan( (dAngle2 - dAngle1) / 2.0 ) ) - 1.0 ) / 3.0;

		double fKoef = 1;

		fX1 = fX + fXRad * cos( dAngle1 );
		fY1 = fY + fYRad * sin( dAngle1 );

		fX2 = fX + fXRad * cos( dAngle2 );
		fY2 = fY + fYRad * sin( dAngle2 );

		fCX1 = fX1 - fAlpha * fXRad * sin ( dAngle1 );
		fCY1 = fY1 + fAlpha * fYRad * cos ( dAngle1 );

		fCX2 = fX2 + fAlpha * fXRad * sin ( dAngle2 );
		fCY2 = fY2 - fAlpha * fYRad * cos ( dAngle2 );

		if ( !bClockDirection )
		{
			CurveTo(fCX1, fCY1, fCX2, fCY2, fX2, fY2);

			*pfXCur = fX2; 
			*pfYCur = fY2;
		}
		else
		{
			CurveTo(fCX2, fCY2, fCX1, fCY1, fX1, fY1);

			*pfXCur = fX1; 
			*pfYCur = fY1;
		}
		return 0;
	}

	int CGraphicsPath::Ellipse(double fX, double fY, double fXRad, double fYRad)
	{
		MoveTo(fX - fXRad, fY);

		double c_fKappa = 0.552;
		CurveTo(fX - fXRad, fY + fYRad * c_fKappa, fX - fXRad * c_fKappa, fY + fYRad, fX, fY + fYRad);
		CurveTo(fX + fXRad * c_fKappa, fY + fYRad, fX + fXRad, fY + fYRad * c_fKappa, fX + fXRad, fY);
		CurveTo(fX + fXRad, fY - fYRad * c_fKappa, fX + fXRad * c_fKappa, fY - fYRad, fX, fY - fYRad);
		CurveTo(fX - fXRad * c_fKappa, fY - fYRad, fX - fXRad, fY - fYRad * c_fKappa, fX - fXRad, fY);

		return 0;
	}

	Status CGraphicsPath::AddArc2(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		if (0 >= fWidth || 0 >= fHeight)
			return InvalidParameter;
		
		if ( Is_poly_closed() )
		{
			double dStartAngle = fStartAngle * agg::pi / 180;
			double fStartX = fX + fWidth / 2.0 + fWidth / 2 * cos( AngToEllPrm( dStartAngle, fWidth / 2, fHeight / 2 ) );
			double fStartY = fY + fHeight / 2.0 - fHeight / 2 * sin( AngToEllPrm ( dStartAngle, fWidth / 2, fHeight / 2 ) );

			// В случае, когда эллипс рисуется целиком используется команда AppendEllipse, в которой команда MoveTo уже есть
			if ( fSweepAngle < 360 )
				if ( Ok != MoveTo( fStartX, fStartY ) )
					return GenericError;
		}

		INT bClockDirection = FALSE;
		double fEndAngle = 360 - ( fSweepAngle + fStartAngle );
		double fSrtAngle = 360 - fStartAngle;
		if( fSweepAngle > 0 )
			bClockDirection = TRUE;

		if( fabs(fSweepAngle) >= 360 ) // Целый эллипс
		{
			return (0 == Ellipse(fX + fWidth / 2, fY + fHeight / 2, fWidth / 2, fHeight / 2)) ? Ok : GenericError;
		}
		else // Дуга эллипса
		{
			return (0 == EllipseArc(fX + fWidth / 2, fY + fHeight / 2, fWidth / 2, fHeight / 2, fSrtAngle, fEndAngle, bClockDirection)) ? Ok : GenericError;
		}
 
		return Ok;
	}

    bool CGraphicsPath::IsPointInPath(const double& x, const double& y)
    {
        agg::rasterizer_scanline_aa<> rasterizer;
        agg::conv_curve<agg::path_storage> c_c_path(m_internal->m_agg_ps);
        rasterizer.add_path(c_c_path);

        return rasterizer.hit_test((int)x, (int)y);
    }

}

namespace Aggplus
{
	// Converter
	CGraphicsPathSimpleConverter::CGraphicsPathSimpleConverter()
	{
		m_pRenderer = NULL;
        m_internal = new CGraphicsPathSimpleConverter_private();
	}

	CGraphicsPathSimpleConverter::~CGraphicsPathSimpleConverter()
	{
		RELEASEINTERFACE(m_pRenderer);
        RELEASEOBJECT(m_internal);
	}

	void CGraphicsPathSimpleConverter::SetRenderer(IRenderer* pRenderer)
	{
		RELEASEINTERFACE(m_pRenderer);
		m_pRenderer = pRenderer;
		ADDREFINTERFACE(m_pRenderer);
	}
	IRenderer* CGraphicsPathSimpleConverter::GetRenderer(INT bIsAddref)
	{
		if (bIsAddref)
		{
			ADDREFINTERFACE(m_pRenderer);
		}

		return m_pRenderer;
	}

	bool CGraphicsPathSimpleConverter::PathCommandMoveTo(double fX, double fY)
	{
		return _MoveTo(fX, fY);
	}
	bool CGraphicsPathSimpleConverter::PathCommandLineTo(double fX, double fY)
	{
		return _LineTo(fX, fY);
	}
	bool CGraphicsPathSimpleConverter::PathCommandLinesTo(double* pPoints, LONG lCount)
	{
		if (NULL == pPoints)
			return false;

		double* pData = pPoints;
		
		if (2 == lCount)
		{
			return _LineTo(pData[0], pData[1]);
		}
				
		if (4 > lCount)
		{
			return false;
		}

		int nRet = 0;

        if (!m_internal->m_bIsMoveTo)
		{
			_MoveTo(pData[0], pData[1]);
		}

		int n = (lCount / 2) - 1;

		for (int i = 1; i <= n; ++i)
		{
			double* points = &pData[i * 2];
			_LineTo(points[0], points[1]);
		}
		
		return true;
	}

	bool CGraphicsPathSimpleConverter::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		return _CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	bool CGraphicsPathSimpleConverter::PathCommandCurvesTo(double* pData, LONG lCount)
	{
		if (NULL == pData)
			return false;

		if (8 > lCount)
			return false;

        if (!m_internal->m_bIsMoveTo)
		{
			_MoveTo(pData[0], pData[1]);

			pData += 2;
			lCount -= 2;
		}
		else 
		{
			_LineTo(pData[0], pData[1]);

			pData += 2;
			lCount -= 2;
		}

		double* points = pData;
		
		int nCountTo = (lCount) / 6;
		for (int i = 0; i < nCountTo; ++i)
		{
			points = pData + 6 * i;
			_CurveTo(points[0], points[1], points[2], points[3], points[4], points[5]);
		}

		return true;
	}
	bool CGraphicsPathSimpleConverter::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		return AddArc(fX, fY, fWidth, fHeight, -fStartAngle, -fSweepAngle);
	}
	bool CGraphicsPathSimpleConverter::PathCommandClose()
	{
		return _Close();
	}
	bool CGraphicsPathSimpleConverter::PathCommandEnd()
	{
		return _Reset();
	}

	bool CGraphicsPathSimpleConverter::PathCommandStart()
	{
		return _Start();
	}
	bool CGraphicsPathSimpleConverter::PathCommandGetCurrentPoint(double* fX, double* fY)
	{
        m_internal->m_agg_ps.last_vertex(fX, fY);
		return true;
	}
    bool CGraphicsPathSimpleConverter::PathCommandText(const std::wstring& bsText, NSFonts::IFontManager* pManager, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		return AddString(bsText, pManager, fX, fY + fBaseLineOffset);
	}
    bool CGraphicsPathSimpleConverter::PathCommandTextEx(std::wstring& bsText, std::wstring& bsGidText, NSFonts::IFontManager* pManager, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		if (!bsGidText.empty())
		{
			return PathCommandText(bsGidText, pManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
		}
		
		return PathCommandText(bsText, pManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}

    bool CGraphicsPathSimpleConverter::PathCommandText2(const int* pUnicodes, const int* pGids, const int& nCount, NSFonts::IFontManager* pManager,
                          const double& x, const double& y, const double& w, const double& h)
    {
        if (NULL == pGids)
        {
            pManager->SetStringGID(FALSE);
            pManager->LoadString1((const unsigned int*)pUnicodes, (unsigned int)nCount, (float)x, (float)y);
            return (TRUE == pManager->GetStringPath(this)) ? true : false;
        }
        else
        {
            pManager->SetStringGID(TRUE);
            pManager->LoadString1((const unsigned int*)pGids, (unsigned int)nCount, (float)x, (float)y);
            return (TRUE == pManager->GetStringPath(this)) ? true : false;
        }
    }
    bool CGraphicsPathSimpleConverter::PathCommandText2(const std::wstring& sUnicodes, const int* pGids, const int& nCount, NSFonts::IFontManager* pManager,
                          const double& x, const double& y, const double& w, const double& h)
    {
        if (NULL == pGids)
        {
            pManager->SetStringGID(FALSE);
            pManager->LoadString1(sUnicodes, (float)x, (float)y);
            return (TRUE == pManager->GetStringPath(this)) ? true : false;
        }
        else
        {
            pManager->SetStringGID(TRUE);
            pManager->LoadString1((const unsigned int*)pGids, (unsigned int)nCount, (float)x, (float)y);
            return (TRUE == pManager->GetStringPath(this)) ? true : false;
        }
    }

	bool CGraphicsPathSimpleConverter::PathCommandGetBounds(double& left, double& top, double& width, double &height)
	{
        unsigned int nTotal = m_internal->m_agg_ps.total_vertices();
		if (nTotal)
		{
			agg::rect_d bounds(1e100, 1e100, -1e100, -1e100);
			double x, y;
			for(unsigned int i = 0; i < nTotal; i++)
			{
                unsigned int nTip = m_internal->m_agg_ps.vertex(i, &x, &y);
				if(agg::is_vertex(nTip))
				{
					if(x < bounds.x1) bounds.x1 = x;
					if(y < bounds.y1) bounds.y1 = y;
					if(x > bounds.x2) bounds.x2 = x;
					if(y > bounds.y2) bounds.y2 = y;
				}
			}
			
			left	= bounds.x1;
			top		= bounds.y1;
			width	= (bounds.x2 - bounds.x1);
			height	= (bounds.y2 - bounds.y1);
		}
		else 
		{
			left	= 0;
			top		= 0;
			width	= 0;
			height	= 0;
		}
		return true;
	}
	
	bool CGraphicsPathSimpleConverter::_MoveTo(double x, double y)
	{
        m_internal->m_bIsMoveTo = true;
        m_internal->m_agg_ps.move_to(x, y);

		if (NULL != m_pRenderer)
		{
			m_pRenderer->BeginCommand(c_nSimpleGraphicType);
			m_pRenderer->PathCommandMoveTo(x, y);
			m_pRenderer->EndCommand(c_nSimpleGraphicType);
		}
		return true;
	}
	bool CGraphicsPathSimpleConverter::_LineTo(double x, double y)
	{
        if (!m_internal->m_bIsMoveTo)
		{
			_MoveTo(x, y);
		}		
		
        m_internal->m_agg_ps.line_to(x, y);
		
		if (NULL != m_pRenderer)
		{
			m_pRenderer->BeginCommand(c_nSimpleGraphicType);
			m_pRenderer->PathCommandLineTo(x, y);
			m_pRenderer->EndCommand(c_nSimpleGraphicType);
		}

		return true;
	}
	bool CGraphicsPathSimpleConverter::_CurveTo(double x1, double y1, double x2, double y2, double x3, double y3)
	{
        if (!m_internal->m_bIsMoveTo)
		{
			_MoveTo(x1, y1);
		}
		
        m_internal->m_agg_ps.curve4(x1, y1, x2, y2, x3, y3);

		if (NULL != m_pRenderer)
		{
			m_pRenderer->BeginCommand(c_nSimpleGraphicType);
			m_pRenderer->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
			m_pRenderer->EndCommand(c_nSimpleGraphicType);
		}

		return true;
	}
	bool CGraphicsPathSimpleConverter::_Close()
	{
        m_internal->m_bIsClosed = true;
        m_internal->m_agg_ps.close_polygon();
		
		if (NULL != m_pRenderer)
		{
			m_pRenderer->BeginCommand(c_nSimpleGraphicType);
			m_pRenderer->PathCommandClose();
			m_pRenderer->EndCommand(c_nSimpleGraphicType);
		}
		return true;
	}
	bool CGraphicsPathSimpleConverter::_Reset()
	{
        m_internal->m_bEvenOdd	= false;
        m_internal->m_bIsMoveTo	= false;
        m_internal->m_bIsClosed	= false;

        m_internal->m_agg_ps.remove_all();

		if (NULL != m_pRenderer)
		{
			m_pRenderer->BeginCommand(c_nSimpleGraphicType);
			m_pRenderer->PathCommandEnd();
			m_pRenderer->EndCommand(c_nSimpleGraphicType);
		}

		return true;
	}
	bool CGraphicsPathSimpleConverter::_Start()
	{
        m_internal->m_agg_ps.start_new_path();

		if (NULL != m_pRenderer)
		{
			m_pRenderer->BeginCommand(c_nSimpleGraphicType);
			m_pRenderer->PathCommandStart();
			m_pRenderer->EndCommand(c_nSimpleGraphicType);
		}

		return true;
	}

    bool CGraphicsPathSimpleConverter::AddString(const std::wstring& bstrText, NSFonts::IFontManager* pFont, double x, double y)
	{
		if (NULL == pFont)
			return false;
		
		pFont->LoadString1(bstrText, (float)x, (float)y);
		return (TRUE == pFont->GetStringPath(this)) ? true : false;
	}

	int CGraphicsPathSimpleConverter::EllipseArc(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection)
	{
		int nRet = 0;
		
		while ( fAngle1 < 0 )
			fAngle1 += 360;

		while ( fAngle1 > 360 )
			fAngle1 -= 360;

		while ( fAngle2 < 0 )
			fAngle2 += 360;

		while ( fAngle2 >= 360 )
			fAngle2 -= 360;

		if ( !bClockDirection )
		{
			if ( fAngle1 <= fAngle2 )
				nRet = EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, fAngle2, FALSE );
			else
			{
				nRet += EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, 360, FALSE );
				nRet += EllipseArc2( fX, fY, fXRad, fYRad, 0, fAngle2, FALSE );
			}
		}
		else
		{
			if ( fAngle1 >= fAngle2 )
				nRet = EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, fAngle2, TRUE );
			else
			{
				nRet += EllipseArc2( fX, fY, fXRad, fYRad, fAngle1, 0, TRUE );
				nRet += EllipseArc2( fX, fY, fXRad, fYRad, 360, fAngle2, TRUE );
			}
		}
		return nRet;
	}

	double CGraphicsPathSimpleConverter::AngToEllPrm(double fAngle, double fXRad, double fYRad)
	{
		// Функция для перевода реального угла в параметрическое задание эллписа
		// т.е. x= a cos(t) y = b sin(t) - параметрическое задание эллписа.
		// x = r cos(p), y = r sin(p) => t = atan2( sin(p) / b, cos(p) / a );
		return atan2( sin( fAngle ) / fYRad,  cos( fAngle ) / fXRad );
	}

	int CGraphicsPathSimpleConverter::EllipseArc2(double fX, double fY, double fXRad, double fYRad, double fAngle1, double fAngle2, INT bClockDirection)
	{
		// переведем углы в радианы
		int nRet = 0;

		double dAngle1 = fAngle1 * 3.141592 / 180;
		double dAngle2 = fAngle2 * 3.141592 / 180;

		// Выясним в каких четвертях находятся начальная и конечная точки
		unsigned int nFirstPointQuard  = int(fAngle1) / 90 + 1; 
		unsigned int nSecondPointQuard = int(fAngle2) / 90 + 1;
        nSecondPointQuard = std::min( 4, std::max( 1, (int)nSecondPointQuard ) );
        nFirstPointQuard  = std::min( 4, std::max( 1, (int)nFirstPointQuard ) );
		// Проведем линию в начальную точку дуги
		double fStartX = 0.0, fStartY = 0.0, fEndX = 0.0, fEndY = 0.0;

		fStartX = fX + fXRad * cos( AngToEllPrm( dAngle1, fXRad, fYRad ) );
		fStartY = fY + fYRad * sin( AngToEllPrm( dAngle1, fXRad, fYRad ) );

		_LineTo(fStartX, fStartY);

		// Дальше рисуем по четверям

		double fCurX = fStartX, fCurY = fStartY;
		double dStartAngle = dAngle1;
		double dEndAngle = 0;

		if ( !bClockDirection )
		{
			for( unsigned int nIndex = nFirstPointQuard; nIndex <= nSecondPointQuard; nIndex++ ) 
			{
				if ( nIndex == nSecondPointQuard )
					dEndAngle = dAngle2;
				else
					dEndAngle = (90 * (nIndex ) ) * 3.141592f / 180;
				if ( !( nIndex == nFirstPointQuard ) )
					dStartAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;

				EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
			}
		}
		else
		{
			for( unsigned int nIndex = nFirstPointQuard; nIndex >= nSecondPointQuard; nIndex-- ) 
			{
				if ( nIndex == nFirstPointQuard )
					dStartAngle = dAngle1;
				else
					dStartAngle = (90 * (nIndex ) ) * 3.141592f / 180;
				if ( !( nIndex == nSecondPointQuard ) )
					dEndAngle = (90 * (nIndex - 1 ) ) * 3.141592f / 180;
				else
					dEndAngle = dAngle2;

				EllipseArc3(fX, fY, fXRad, fYRad, AngToEllPrm( dStartAngle, fXRad, fYRad ), AngToEllPrm( dEndAngle, fXRad, fYRad ), &fEndX, &fEndY, FALSE);
			}
		}

		return nRet;
	}

	int CGraphicsPathSimpleConverter::EllipseArc3(double fX, double fY, double fXRad, double fYRad, double dAngle1, double dAngle2, double *pfXCur, double *pfYCur, INT bClockDirection)
	{
		// Рассчитаем начальную, конечную и контрольные точки
		double fX1  = 0.0, fX2  = 0.0, fY1  = 0.0, fY2  = 0.0;
		double fCX1 = 0.0, fCX2 = 0.0, fCY1 = 0.0, fCY2 = 0.0;

		double fAlpha = sin( dAngle2 - dAngle1 ) * ( sqrt( 4.0 + 3.0 * tan( (dAngle2 - dAngle1) / 2.0 ) * tan( (dAngle2 - dAngle1) / 2.0 ) ) - 1.0 ) / 3.0;

		double fKoef = 1;

		fX1 = fX + fXRad * cos( dAngle1 );
		fY1 = fY + fYRad * sin( dAngle1 );

		fX2 = fX + fXRad * cos( dAngle2 );
		fY2 = fY + fYRad * sin( dAngle2 );

		fCX1 = fX1 - fAlpha * fXRad * sin ( dAngle1 );
		fCY1 = fY1 + fAlpha * fYRad * cos ( dAngle1 );

		fCX2 = fX2 + fAlpha * fXRad * sin ( dAngle2 );
		fCY2 = fY2 - fAlpha * fYRad * cos ( dAngle2 );

		if ( !bClockDirection )
		{
			_CurveTo(fCX1, fCY1, fCX2, fCY2, fX2, fY2);

			*pfXCur = fX2; 
			*pfYCur = fY2;
		}
		else
		{
			_CurveTo(fCX2, fCY2, fCX1, fCY1, fX1, fY1);

			*pfXCur = fX1; 
			*pfYCur = fY1;
		}
		return 0;
	}

	int CGraphicsPathSimpleConverter::Ellipse(double fX, double fY, double fXRad, double fYRad)
	{
		_MoveTo(fX - fXRad, fY);

		double c_fKappa = 0.552;
		_CurveTo(fX - fXRad, fY + fYRad * c_fKappa, fX - fXRad * c_fKappa, fY + fYRad, fX, fY + fYRad);
		_CurveTo(fX + fXRad * c_fKappa, fY + fYRad, fX + fXRad, fY + fYRad * c_fKappa, fX + fXRad, fY);
		_CurveTo(fX + fXRad, fY - fYRad * c_fKappa, fX + fXRad * c_fKappa, fY - fYRad, fX, fY - fYRad);
		_CurveTo(fX - fXRad * c_fKappa, fY - fYRad, fX - fXRad, fY - fYRad * c_fKappa, fX - fXRad, fY);

		return 0;
	}

	bool CGraphicsPathSimpleConverter::AddArc(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		if (0 >= fWidth || 0 >= fHeight)
			return false;
		
		if ( Is_poly_closed() )
		{
			double dStartAngle = fStartAngle * agg::pi / 180;
			double fStartX = fX + fWidth / 2.0 + fWidth / 2 * cos( AngToEllPrm( dStartAngle, fWidth / 2, fHeight / 2 ) );
			double fStartY = fY + fHeight / 2.0 - fHeight / 2 * sin( AngToEllPrm ( dStartAngle, fWidth / 2, fHeight / 2 ) );

			// В случае, когда эллипс рисуется целиком используется команда AppendEllipse, в которой команда MoveTo уже есть
			if ( fSweepAngle < 360 )
				if ( false == _MoveTo( fStartX, fStartY ) )
					return false;
		}

		INT bClockDirection = FALSE;
		double fEndAngle = 360 - ( fSweepAngle + fStartAngle );
		double fSrtAngle = 360 - fStartAngle;
		if( fSweepAngle > 0 )
			bClockDirection = TRUE;

		if( fabs(fSweepAngle) >= 360 ) // Целый эллипс
		{
			return (0 == Ellipse(fX + fWidth / 2, fY + fHeight / 2, fWidth / 2, fHeight / 2)) ? true : false;
		}
		else // Дуга эллипса
		{
			return (0 == EllipseArc(fX + fWidth / 2, fY + fHeight / 2, fWidth / 2, fHeight / 2, fSrtAngle, fEndAngle, bClockDirection)) ? true : false;
		}
 
		return Ok;
	}

	bool CGraphicsPathSimpleConverter::Is_poly_closed()
	{
        if (!m_internal->m_agg_ps.total_vertices())
			return true;

		double x, y;
        unsigned int nTip = m_internal->m_agg_ps.last_vertex(&x, &y);
		
		if (nTip & agg::path_flags_close) 
			return true;

		return false;
	}
}
