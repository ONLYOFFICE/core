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
#include "Brush.h"

namespace Aggplus
{
	////////////////////////////////////////////////////////////////////////////////

	CBrush::CBrush(BrushType bType) : m_bType(bType)
	{
	}
	 
	CBrush::~CBrush()
	{
	}

	BrushType CBrush::GetType() const 
	{ 
		return m_bType; 
	}

	////////////////////////////////////////////////////////////////////////////////

	CBrushSolid::CBrushSolid(CColor dwColor) : CBrush(BrushTypeSolidColor), m_dwColor(dwColor)		
	{
	}
	CBrushSolid::~CBrushSolid()																
	{
	}
	CBrush* CBrushSolid::Clone() const 
	{ 
		return new CBrushSolid(m_dwColor);
	}

	void CBrushSolid::GetColor(CColor* color) const { *color = m_dwColor; }
	void CBrushSolid::SetColor(const CColor &color) { m_dwColor = color; }

	////////////////////////////////////////////////////////////////////////////////

	CBrushHatch::CBrushHatch() : CBrush(BrushTypeHatchFill)
	{
	}
	CBrushHatch::~CBrushHatch()																
	{
	}
	CBrush* CBrushHatch::Clone() const 
	{ 
		CBrushHatch* clone = new CBrushHatch();
		clone->m_name		= m_name;
		clone->m_dwColor1	= m_dwColor1;
		clone->m_dwColor2	= m_dwColor2;
		return clone;
	}
	
	////////////////////////////////////////////////////////////////////////////////

	CBrushLinearGradient::CBrushLinearGradient( const PointF& p1, const PointF& p2, const CColor& c1, const CColor& c2 ) 
		: CBrush(BrushTypeLinearGradient)
	{
		m_points[0] = p1;
		m_points[1] = p2;

		m_colors[0] = c1;
		m_colors[1] = c2;

		m_angle = 0;
		m_wrap = Aggplus::WrapModeTile;

		m_bAngleScalable = FALSE;
		m_bRectable = FALSE;
		m_bRelativeCoords = FALSE;
	}

	CBrushLinearGradient::CBrushLinearGradient( const Point& p1, const Point& p2, const CColor& c1, const CColor& c2 ) 
		: CBrush(BrushTypeLinearGradient)
	{
		m_points[0].X = (float)p1.X;
		m_points[0].Y = (float)p1.Y;
		m_points[1].X = (float)p2.X;
		m_points[1].Y = (float)p2.Y;

		m_colors[0] = c1;
		m_colors[1] = c2;

		m_angle = 0;
		
		m_wrap = Aggplus::WrapModeTile;

		m_bAngleScalable = FALSE;
		m_bRectable = FALSE;
		m_bRelativeCoords = FALSE;
	}

	CBrushLinearGradient::CBrushLinearGradient( const RectF& rect, const CColor& c1, const CColor& c2, float angle, INT isAngleScalable ) 
		: CBrush(BrushTypeLinearGradient)
	{
		m_points[0].X = rect.GetLeft();
		m_points[0].Y = rect.GetTop();
		m_points[1].X = rect.GetRight();
		m_points[1].Y = rect.GetBottom();

		m_colors[0] = c1;
		m_colors[1] = c2;

		m_angle = angle;
		
		m_wrap = Aggplus::WrapModeTile;

		m_bAngleScalable = isAngleScalable;
		m_bRectable = TRUE;
		m_bRelativeCoords = FALSE;
	}

	CBrushLinearGradient::CBrushLinearGradient( const Rect& rect, const CColor& c1, const CColor& c2, float angle, INT isAngleScalable ) 
		: CBrush(BrushTypeLinearGradient)
	{
		m_points[0].X = (float)rect.GetLeft();
		m_points[0].Y = (float)rect.GetTop();
		m_points[1].X = (float)rect.GetRight();
		m_points[1].Y = (float)rect.GetBottom();

		m_colors[0] = c1;
		m_colors[1] = c2;

		m_angle = angle;
		
		m_wrap = Aggplus::WrapModeTile;

		m_bAngleScalable = isAngleScalable;
		m_bRectable = TRUE;
		m_bRelativeCoords = FALSE;
	}

	CBrushLinearGradient::CBrushLinearGradient( const RectF& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode ) 
		: CBrush(BrushTypeLinearGradient)
	{
		m_points[0].X = rect.GetLeft();
		m_points[0].Y = rect.GetTop();
		m_points[1].X = rect.GetRight();
		m_points[1].Y = rect.GetBottom();

		switch( mode )
		{
		case LinearGradientModeHorizontal:
			m_angle = 0;
			break;

		case LinearGradientModeVertical:
			m_angle = 90;
			break;

		case LinearGradientModeForwardDiagonal:
			m_angle = 45;
			break;

		default:
			m_angle = 315;
		}

		m_colors[0] = c1;
		m_colors[1] = c2;

		m_wrap = Aggplus::WrapModeTile;

		m_bAngleScalable = TRUE;
		m_bRectable = TRUE;
		m_bRelativeCoords = FALSE;
	}

	CBrushLinearGradient::CBrushLinearGradient( const Rect& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode ) 
		: CBrush(BrushTypeLinearGradient)
	{
		m_points[0].X = (REAL)rect.GetLeft();
		m_points[0].Y = (REAL)rect.GetTop();
		m_points[1].X = (REAL)rect.GetRight();
		m_points[1].Y = (REAL)rect.GetBottom();

		switch( mode )
		{
		case LinearGradientModeHorizontal:
			m_angle = 0;
			break;

		case LinearGradientModeVertical:
			m_angle = 90;
			break;

		case LinearGradientModeForwardDiagonal:
			m_angle = 45;
			break;

		default:
			m_angle = 315;
		}

		m_colors[0] = c1;
		m_colors[1] = c2;

		m_wrap = Aggplus::WrapModeTile;

		m_bAngleScalable = TRUE;
		m_bRectable = TRUE;
		m_bRelativeCoords = FALSE;
	}


	CBrushLinearGradient::CBrushLinearGradient( const CBrushLinearGradient& out )
		: CBrush(out.m_bType)
	{
		m_colors[0] = out.m_colors[0];
		m_colors[1] = out.m_colors[1];
		m_points[0] = out.m_points[0];
		m_points[1] = out.m_points[1];

		m_subcolors = out.m_subcolors;

		m_matrix = out.m_matrix;
		m_angle = out.m_angle;

		m_wrap = out.m_wrap;
		m_bAngleScalable = out.m_bAngleScalable;
		m_bRectable = out.m_bRectable;
		m_bRelativeCoords = out.m_bRelativeCoords;
	}

	Status CBrushLinearGradient::GetLinearColors( CColor* colors ) const
	{
		if( !colors )
			return InvalidParameter;

		colors[0] = m_colors[0];
		colors[1] = m_colors[1];

		return Ok;
	}

	Status CBrushLinearGradient::GetRectangle( Rect *rect ) const
	{
		if( !rect )
			return InvalidParameter;

		rect->X = (int)( m_points[0].X );
		rect->Y = (int)( m_points[0].Y );
		rect->Width = (int)( m_points[1].X ) - rect->X;
		rect->Height = (int)( m_points[1].Y ) - rect->Y;

		return Ok;
	}

	Status CBrushLinearGradient::GetRectangle( RectF *rect ) const
	{
		if( !rect )
			return InvalidParameter;

		rect->X = m_points[0].X;
		rect->Y = m_points[0].Y;
		rect->Width = m_points[1].X - rect->X;
		rect->Height = m_points[1].Y - rect->Y;

		return Ok;
	}

	Status CBrushLinearGradient::GetTransform( CMatrix* matrix ) const
	{
		if( !matrix )
			return InvalidParameter;

		*matrix = m_matrix;

		return Ok;
	}

	Status CBrushLinearGradient::MultiplyTransform( const CMatrix *matrix, MatrixOrder order)
	{
		if( !matrix )
			return InvalidParameter;

		m_matrix.Multiply( matrix, order );

		return Ok;
	}

	Status CBrushLinearGradient::ResetTransform()
	{
		m_matrix.Reset();

		return Ok;
	}

	Status CBrushLinearGradient::RotateTransform( REAL angle, MatrixOrder order )
	{
		m_matrix.Rotate( angle, order );

		return Ok;
	}

	Status CBrushLinearGradient::ScaleTransform( REAL sx, REAL sy, MatrixOrder order )
	{
		m_matrix.Scale( sx, sy, order );

		return Ok;
	}

	void CBrushLinearGradient::SetWrapMode( WrapMode mode )
	{
		m_wrap = mode;
	}

	WrapMode CBrushLinearGradient::GetWrapMode() const
	{
		return m_wrap;
	}

	CBrush* CBrushLinearGradient::Clone() const
	{
		return new CBrushLinearGradient( *this ); 
	}

	Status CBrushLinearGradient::SetInterpolationColors( const CColor *presetColors, const REAL *blendPositions, INT count )
	{
        m_subcolors.clear();

		if( count > 0 && presetColors && blendPositions )
		{
            m_subcolors.resize( count );

			for( int i = 0; i < count; i++ )
			{
				m_subcolors[i].color = presetColors[i];
				m_subcolors[i].position = blendPositions[i];
			}
		}

		return Ok;
	}

	Status CBrushLinearGradient::GetInterpolationColors( CColor *presetColors, REAL *blendPositions, INT count ) const
	{
        if( count > 0 && count <= (INT)m_subcolors.size() && presetColors && blendPositions )
		{
			for( int i = 0; i < count; i++ )
			{
				presetColors[i] = m_subcolors[i].color;
				blendPositions[i] = m_subcolors[i].position;
			}
		}

		return Ok;
	}

	INT CBrushLinearGradient::GetInterpolationColorsCount() const
	{
        return (INT)m_subcolors.size();
	}

	// additional methods
	void CBrushLinearGradient::GetSubColor( int nIndex, CColor* pColor, float* pPosition ) const
	{
		*pColor = m_subcolors[nIndex].color;
		*pPosition = m_subcolors[nIndex].position;
	}
	
	void CBrushLinearGradient::SetRelativeCoords( INT bRelative )
	{
		m_bRelativeCoords = bRelative;
	}
	INT CBrushLinearGradient::IsRelativeCoords() const
	{
		return m_bRelativeCoords;
	}

	INT CBrushLinearGradient::IsAngleScalable() const
	{
		return m_bAngleScalable;
	}
	INT CBrushLinearGradient::IsRectable() const
	{
		return m_bRectable;
	}
	float CBrushLinearGradient::GetAngle() const
	{
		return m_angle;
	}

	////////////////////////////////////////////////////////////////////////////////

	CBrushTexture::CBrushTexture() : CBrush(BrushTypeTextureFill)											
	{
		m_pImage = NULL;
		m_bReleaseImage = FALSE;
		Alpha = 255;
        m_bUseBounds = false;
	}

	CBrushTexture::CBrushTexture(const std::wstring& strName, WrapMode wrapMode) : CBrush(BrushTypeTextureFill), m_wrapMode(wrapMode)
	{
		m_pImage = new CImage(strName);
		m_bReleaseImage = TRUE;
		Alpha = 255;
        m_bUseBounds = false;
	}

	CBrushTexture::CBrushTexture(CImage *pImage, WrapMode wrapMode) : CBrush(BrushTypeTextureFill), m_wrapMode(wrapMode)
	{
		m_pImage = pImage;
		m_bReleaseImage = FALSE;
		Alpha = 255;
        m_bUseBounds = false;
	}
	
	CBrushTexture::~CBrushTexture() 
	{
		if (m_bReleaseImage)
		{
			RELEASEOBJECT(m_pImage);
		}
	}
	
	CBrush* CBrushTexture::Clone() const
	{
		CBrushTexture *pRet = new CBrushTexture(m_pImage, m_wrapMode);
		if( pRet )
		{
			pRet->m_mtx = m_mtx;
		}

		return pRet;
	}

	void CBrushTexture::TranslateTransform(double dX, double dY, MatrixOrder order) 
	{ 
		m_mtx.Translate(dX, dY, order); 
	}
	void CBrushTexture::ScaleTransform(double dX, double dY, MatrixOrder order) 
	{ 
		m_mtx.Scale(dX, dY, order); 
	}
	void CBrushTexture::RotateTransform(double angle, MatrixOrder order) 
	{ 
		m_mtx.Rotate(angle, order); 
	}
	void CBrushTexture::GetTransform(CMatrix* matrix) const 
	{ 
		*matrix = m_mtx; 
	}
	void CBrushTexture::SetTransform(const CMatrix* matrix) 
	{ 
		m_mtx = *matrix;
	}

	void CBrushTexture::SetWrapMode(WrapMode wMode) 
	{ 
		m_wrapMode = wMode; 
	}
	WrapMode CBrushTexture::GetWrapMode() const 
	{ 
		return(m_wrapMode); 
	}

	void* CBrushTexture::GetData()
	{
		return m_pImage->m_pImgData;
	}
	void* CBrushTexture::PatternFinalize()
	{	
		if (m_pImage->m_nStride < 0)
			return m_pImage->m_pImgData - ((m_pImage->m_dwHeight - 1) * m_pImage->m_nStride);
		return m_pImage->m_pImgData;
	}
	DWORD CBrushTexture::PatternGetWidth()
	{	
		return m_pImage->m_dwWidth;
	}
	DWORD CBrushTexture::PatternGetHeight()
	{	
		return m_pImage->m_dwHeight;
	}
	int CBrushTexture::PatternGetStride()
	{	
		return m_pImage->m_nStride;
	}
}
