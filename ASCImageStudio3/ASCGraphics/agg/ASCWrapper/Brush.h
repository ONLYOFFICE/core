#pragma once

#include "..\ap_AggPlusTypes.h"
#include "..\ap_AggPlusEnums.h"

#include "Color.h"
#include "Matrix.h"
#include "Image.h"

#include "..\..\Objects\Structures.h"

namespace Aggplus
{
	using namespace Gdiplus;

class CBrush  
{
	friend class CGraphics;

protected:

	CBrush(BrushType bType) : m_bType(bType)											{ }
	 
public:
	virtual ~CBrush()																	{ }
	virtual CBrush *Clone() const = 0;
	inline BrushType GetType() const 
	{ 
		return m_bType; 
	}

public:
	BrushType m_bType;
};

class CBrushSolid : public CBrush
{
public:
	CBrushSolid(CColor dwColor) : CBrush(BrushTypeSolidColor), m_dwColor(dwColor)		
	{
	}
	virtual ~CBrushSolid()																
	{
	}
	virtual CBrush *Clone() const 
	{ 
		return new CBrushSolid(m_dwColor);
	}

	void GetColor(CColor* color) const { *color = m_dwColor; }
	void SetColor(const CColor &color) { m_dwColor = color; }
	
protected:
	CColor m_dwColor;
};


class CBrushLinearGradient : public CBrush
{
	friend class CGraphics;

public:
	CBrushLinearGradient( const PointF& p1, const PointF& p2, const CColor& c1, const CColor& c2 ) 
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

	CBrushLinearGradient( const Point& p1, const Point& p2, const CColor& c1, const CColor& c2 ) 
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

	CBrushLinearGradient( const RectF& rect, const CColor& c1, const CColor& c2, float angle, BOOL isAngleScalable ) 
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

	CBrushLinearGradient( const Rect& rect, const CColor& c1, const CColor& c2, float angle, BOOL isAngleScalable ) 
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

	CBrushLinearGradient( const RectF& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode ) 
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

	CBrushLinearGradient( const Rect& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode ) 
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


	CBrushLinearGradient( const CBrushLinearGradient& out )
		: CBrush(out.m_bType)
	{
		m_colors[0] = out.m_colors[0];
		m_colors[1] = out.m_colors[1];
		m_points[0] = out.m_points[0];
		m_points[1] = out.m_points[1];

		m_subcolors.Copy( out.m_subcolors );

		m_matrix = out.m_matrix;
		m_angle = out.m_angle;

		m_wrap = out.m_wrap;
		m_bAngleScalable = out.m_bAngleScalable;
		m_bRectable = out.m_bRectable;
		m_bRelativeCoords = out.m_bRelativeCoords;
	}
	Status GetLinearColors( CColor* colors ) const
	{
		if( !colors )
			return InvalidParameter;

		colors[0] = m_colors[0];
		colors[1] = m_colors[1];

		return Ok;
	}

	Status GetRectangle( Rect *rect ) const
	{
		if( !rect )
			return InvalidParameter;

		rect->X = (int)( m_points[0].X );
		rect->Y = (int)( m_points[0].Y );
		rect->Width = (int)( m_points[1].X ) - rect->X;
		rect->Height = (int)( m_points[1].Y ) - rect->Y;

		return Ok;
	}

	Status GetRectangle( RectF *rect ) const
	{
		if( !rect )
			return InvalidParameter;

		rect->X = m_points[0].X;
		rect->Y = m_points[0].Y;
		rect->Width = m_points[1].X - rect->X;
		rect->Height = m_points[1].Y - rect->Y;

		return Ok;
	}

	Status GetTransform( CMatrix* matrix ) const
	{
		if( !matrix )
			return InvalidParameter;

		*matrix = m_matrix;

		return Ok;
	}

	Status MultiplyTransform( const CMatrix *matrix, MatrixOrder order = MatrixOrderPrepend)
	{
		if( !matrix )
			return InvalidParameter;

		m_matrix.Multiply( matrix, order );

		return Ok;
	}

	Status ResetTransform()
	{
		m_matrix.Reset();

		return Ok;
	}

	Status RotateTransform( float angle, MatrixOrder order = MatrixOrderPrepend )
	{
		m_matrix.Rotate( angle, order );

		return Ok;
	}

	Status ScaleTransform( float sx, float sy, MatrixOrder order = MatrixOrderPrepend )
	{
		m_matrix.Scale( sx, sy, order );

		return Ok;
	}

	void SetWrapMode( WrapMode mode )
	{
		m_wrap = mode;
	}

	WrapMode GetWrapMode() const
	{
		return m_wrap;
	}




	virtual CBrush *Clone() const
	{
		return new CBrushLinearGradient( *this ); 
	}


	Status SetInterpolationColors( const CColor *presetColors, const float *blendPositions, INT count )
	{
		m_subcolors.RemoveAll();

		if( count > 0 && presetColors && blendPositions )
		{
			m_subcolors.SetCount( count, 0 );

			for( int i = 0; i < count; i++ )
			{
				m_subcolors[i].color = presetColors[i];
				m_subcolors[i].position = blendPositions[i];
			}
		}

		return Ok;
	}

	Status GetInterpolationColors( CColor *presetColors, float *blendPositions, INT count ) const
	{
		if( count > 0 && count <= (INT)m_subcolors.GetCount() && presetColors && blendPositions )
		{
			for( int i = 0; i < count; i++ )
			{
				presetColors[i] = m_subcolors[i].color;
				blendPositions[i] = m_subcolors[i].position;
			}
		}

		return Ok;
	}

	INT GetInterpolationColorsCount() const
	{
		return (INT)m_subcolors.GetCount();
	}

	// additional methods
	void GetSubColor( int nIndex, CColor* pColor, float* pPosition ) const
	{
		*pColor = m_subcolors[nIndex].color;
		*pPosition = m_subcolors[nIndex].position;
	}
	
	void SetRelativeCoords( BOOL bRelative )
	{
		m_bRelativeCoords = bRelative;
	}
	BOOL IsRelativeCoords() const
	{
		return m_bRelativeCoords;
	}

	BOOL IsAngleScalable() const
	{
		return m_bAngleScalable;
	}
	BOOL IsRectable() const
	{
		return m_bRectable;
	}
	float GetAngle() const
	{
		return m_angle;
	}


protected:
	//std::vector<CColor> m_colors;
	//double m_rotate_angle_radians;

	CColor m_colors[2];
	PointF m_points[2];

	struct TSubColor
	{
		CColor color;
		float position;
	};
	
	ATL::CAtlArray<TSubColor> m_subcolors;

	CMatrix m_matrix;
	float m_angle; // угол поворота в градусах базовой линии p1 -> p2

	Aggplus::WrapMode m_wrap;

	BOOL m_bAngleScalable; // масштабировать угол поворота относительно заданных точек b = arctan( width / height * tan(angle) );
	BOOL m_bRectable; // в качестве направляющей используется диагональ прямоугольника
	BOOL m_bRelativeCoords; // координаты точек считаются относительно рисуемого примитива

};

class CBrushTexture : public CBrush
{
	friend class CGraphics;
	
protected:
	CBrushTexture() : CBrush(BrushTypeTextureFill)											
	{
		m_pImage = NULL;
		m_bReleaseImage = FALSE;
		m_bUsePattern = FALSE;
	}

public:
	CBrushTexture(const WCHAR *strName, WrapMode wrapMode = WrapModeTile) : CBrush(BrushTypeTextureFill), m_wrapMode(wrapMode)
	{
		m_pImage = new CImage(strName);
		m_bReleaseImage = TRUE;
		m_bUsePattern = FALSE;
	}

	CBrushTexture(CImage *pImage, WrapMode wrapMode = WrapModeTile) : CBrush(BrushTypeTextureFill), m_wrapMode(wrapMode)
	{
		//m_Image.CopyFrom(pImage);
		m_pImage = pImage;
		m_bReleaseImage = FALSE;
		m_bUsePattern = FALSE;
	}
	virtual ~CBrushTexture() 
	{
		if (m_bReleaseImage)
		{
			RELEASEOBJECT(m_pImage);
		}
	}
	virtual CBrush *Clone() const
	{
		CBrushTexture *pRet = new CBrushTexture(m_pImage, m_wrapMode);
		if( pRet )
		{
			pRet->m_mtx = m_mtx;
			pRet->m_bUsePattern = m_bUsePattern;
		}

		return pRet;
	}

	void TranslateTransform(double dX, double dY, MatrixOrder order = MatrixOrderPrepend) 
	{ 
		m_mtx.Translate(dX, dY, order); 
	}
	void ScaleTransform(double dX, double dY, MatrixOrder order = MatrixOrderPrepend) 
	{ 
		m_mtx.Scale(dX, dY, order); 
	}
	void RotateTransform(double angle, MatrixOrder order = MatrixOrderPrepend) 
	{ 
		m_mtx.Rotate(angle, order); 
	}
	void GetTransform(CMatrix* matrix) const 
	{ 
		*matrix = m_mtx; 
	}
	void SetTransform(const CMatrix* matrix) 
	{ 
		m_mtx = *matrix;
	}

	void SetWrapMode(WrapMode wMode) 
	{ 
		m_wrapMode = wMode; 
	}
	WrapMode GetWrapMode() const 
	{ 
		return(m_wrapMode); 
	}
	 
public:

	void *PatternFinalize()				
	{	
		return(m_pImage->m_pImgData);		
	}
	DWORD PatternGetWidth()				
	{	
		return(m_pImage->m_dwWidth);		
	}
	DWORD PatternGetHeight()			
	{	
		return(m_pImage->m_dwHeight);		
	}
	int PatternGetStride()			
	{	
		return(m_pImage->m_nStride);		
	}

		
public:
	CImage* m_pImage;
	BOOL m_bReleaseImage;
	
	WrapMode m_wrapMode;
	CMatrix m_mtx;

	BOOL m_bUsePattern;
	CColor m_colors[2];
};

	static CBrush* CreateBrush(NSStructures::CBrush* pBrush)
	{
		if (NULL == pBrush)
			return NULL;

		LONG Type = pBrush->Type;
		if ((0 == Type) || (c_BrushTypeSolid == Type) || (c_BrushTypeHatch1 <= Type && c_BrushTypeHatch53 >= Type))
		{
			Aggplus::CColor oColor((BYTE)pBrush->Alpha1, pBrush->Color1);
			Aggplus::CBrushSolid* pNew = new Aggplus::CBrushSolid(oColor);

			return pNew;
		}
		else if	   ((c_BrushTypeHorizontal		== Type) ||
					(c_BrushTypeVertical		== Type) ||
					(c_BrushTypeDiagonal1		== Type) ||
					(c_BrushTypeDiagonal2		== Type) ||
					(c_BrushTypeCenter			== Type) ||
					(c_BrushTypePathGradient1	== Type) ||
					(c_BrushTypePathGradient2	== Type) ||
					(c_BrushTypeCylinderHor		== Type) ||
					(c_BrushTypeCylinderVer		== Type))
		{
			Aggplus::CColor o1((BYTE)pBrush->Alpha1, pBrush->Color1);
			Aggplus::CColor o2((BYTE)pBrush->Alpha2, pBrush->Color2);

			Aggplus::CBrushLinearGradient* pNew = new Aggplus::CBrushLinearGradient( Aggplus::RectF(0, 0, 1, 1), o1, o2, pBrush->LinearAngle, TRUE );
			if( pNew )
			{
				pNew->SetRelativeCoords( TRUE );

				int nCountSubColors = pBrush->m_arrSubColors.GetSize();
				if( nCountSubColors > 0 )
				{
					Aggplus::CColor* pColors = new Aggplus::CColor[nCountSubColors];
					float* pBlends = new float[nCountSubColors];

					if( pColors && pBlends )
					{
						for( int i = 0; i < nCountSubColors; i++ )
						{
							pColors[i] = Aggplus::CColor(255, pBrush->m_arrSubColors[i].color);
							pBlends[i] = (float)(pBrush->m_arrSubColors[i].position / 65536.0);
						}

						pNew->SetInterpolationColors( pColors, pBlends, nCountSubColors );
					}

					delete [] pColors;
					delete [] pBlends;
				}
			}

			return pNew;
		}
		else
		{
			Aggplus::CBrushTexture* pNew = new Aggplus::CBrushTexture(pBrush->TexturePath, /*(Aggplus::WrapMode)TextureMode*/Aggplus::WrapModeClamp);
			return pNew;
		}

		//LONG lType = pBrush->Type;
		//if ((0 == lType) || (c_BrushTypeSolid == lType) || (c_BrushTypeHatch1 <= lType && c_BrushTypeHatch53 >= lType))
		//{
		//	CColor oColor((BYTE)pBrush->Alpha1, pBrush->Color1);
		//	CBrushSolid* pNew = new CBrushSolid(oColor);

		//	return pNew;
		//}
		//else if	   ((c_BrushTypeHorizontal		== lType) ||
		//			(c_BrushTypeVertical		== lType) ||
		//			(c_BrushTypeDiagonal1		== lType) ||
		//			(c_BrushTypeDiagonal2		== lType) ||
		//			(c_BrushTypeCenter			== lType) ||
		//			(c_BrushTypePathGradient1	== lType) ||
		//			(c_BrushTypePathGradient2	== lType) ||
		//			(c_BrushTypeCylinderHor		== lType) ||
		//			(c_BrushTypeCylinderVer		== lType))
		//{
		//	CColor c1((BYTE)pBrush->Alpha1, pBrush->Color1);
		//	CColor c2((BYTE)pBrush->Alpha2, pBrush->Color2);

		//	CBrushLinearGradient* pNew = new CBrushLinearGradient(c1, c2, 0.0);

		//	return pNew;
		//}
		//else
		//{
		//	CBrushTexture* pNew = new CBrushTexture(pBrush->TexturePath, (WrapMode)pBrush->TextureMode);
		//	return pNew;
		//}
	}
}