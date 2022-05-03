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
#ifndef _BUILD_BRUSH_H_
#define _BUILD_BRUSH_H_

#include "AggPlusEnums.h"

#include "Color.h"
#include "Matrix.h"
#include "Image.h"

#include "structures.h"
#include "../common/Array.h"

namespace Aggplus
{
class CBrush
{
	friend class CGraphics;

protected:

	CBrush(BrushType bType);
	 
public:
	virtual ~CBrush();

	virtual CBrush* Clone() const = 0;
	BrushType GetType() const;

public:
	BrushType m_bType;
};

class CBrushSolid : public CBrush
{
public:
	CBrushSolid(CColor dwColor);
	virtual ~CBrushSolid();
	virtual CBrush *Clone() const;

	void GetColor(CColor* color) const;
	void SetColor(const CColor &color);
	
protected:
	CColor m_dwColor;
};

class CBrushHatch : public CBrush
{
public:
	CBrushHatch();
	virtual ~CBrushHatch();
	virtual CBrush *Clone() const;

	inline CDoubleRect& GetBounds() { return Bounds; }

		
public:
	std::wstring	m_name;
	CColor			m_dwColor1;
	CColor			m_dwColor2;

	CDoubleRect		Bounds;
};


class CBrushLinearGradient : public CBrush
{
	friend class CGraphics;

public:
	CBrushLinearGradient( const PointF& p1, const PointF& p2, const CColor& c1, const CColor& c2 );
	CBrushLinearGradient( const Point& p1, const Point& p2, const CColor& c1, const CColor& c2 );
	CBrushLinearGradient( const RectF& rect, const CColor& c1, const CColor& c2, float angle, INT isAngleScalable );
	CBrushLinearGradient( const Rect& rect, const CColor& c1, const CColor& c2, float angle, INT isAngleScalable );
	CBrushLinearGradient( const RectF& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode );
	CBrushLinearGradient( const Rect& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode );
	CBrushLinearGradient( const CBrushLinearGradient& out );

	Status GetLinearColors( CColor* colors ) const;
	Status GetRectangle( Rect *rect ) const;
	Status GetRectangle( RectF *rect ) const;

	Status GetTransform( CMatrix* matrix ) const;
	Status MultiplyTransform( const CMatrix *matrix, MatrixOrder order = MatrixOrderPrepend);
	Status ResetTransform();
	Status RotateTransform( REAL angle, MatrixOrder order = MatrixOrderPrepend );
	Status ScaleTransform( REAL sx, REAL sy, MatrixOrder order = MatrixOrderPrepend );

	void SetWrapMode( WrapMode mode );
	WrapMode GetWrapMode() const;

	virtual CBrush *Clone() const;

	Status SetInterpolationColors( const CColor *presetColors, const REAL *blendPositions, INT count );
	Status GetInterpolationColors( CColor *presetColors, REAL *blendPositions, INT count ) const;
	INT GetInterpolationColorsCount() const;

	// additional methods
	void GetSubColor( int nIndex, CColor* pColor, float* pPosition ) const;
	
	void SetRelativeCoords( INT bRelative );
	INT IsRelativeCoords() const;
	INT IsAngleScalable() const;
	INT IsRectable() const;
	float GetAngle() const;
	inline void SetBounds(const CDoubleRect& oRect) { Bounds = oRect; }
	inline CDoubleRect& GetBounds() { return Bounds; }

    // info about gradient
    NSStructures::GradientInfo m_oGradientInfo; // used in 60xx grad types
protected:
	CColor m_colors[2];
	PointF m_points[2];

	struct TSubColor
	{
		CColor color;
		float position;
	};
	
    std::vector<TSubColor> m_subcolors;

	CMatrix m_matrix;
	float m_angle; // угол поворота в градусах базовой линии p1 -> p2

	CDoubleRect Bounds;

	Aggplus::WrapMode m_wrap;

	INT m_bAngleScalable; // масштабировать угол поворота относительно заданных точек b = arctan( width / height * tan(angle) );
	INT m_bRectable; // в качестве направляющей используется диагональ прямоугольника
	INT m_bRelativeCoords; // координаты точек считаются относительно рисуемого примитива
};

class CBrushTexture : public CBrush
{
	friend class CGraphics;
	
protected:
	CBrushTexture();

public:
	CBrushTexture(const std::wstring& strName, WrapMode wrapMode = WrapModeTile);
	CBrushTexture(CImage *pImage, WrapMode wrapMode = WrapModeTile);
	virtual ~CBrushTexture();
	virtual CBrush* Clone() const;

	void TranslateTransform(double dX, double dY, MatrixOrder order = MatrixOrderPrepend);
	void ScaleTransform(double dX, double dY, MatrixOrder order = MatrixOrderPrepend);
	void RotateTransform(double angle, MatrixOrder order = MatrixOrderPrepend);
	void GetTransform(CMatrix* matrix) const;
	void SetTransform(const CMatrix* matrix);

	void SetWrapMode(WrapMode wMode);
	WrapMode GetWrapMode() const;
	 
public:

	void* GetData();
	void* PatternFinalize();
	DWORD PatternGetWidth();
	DWORD PatternGetHeight();
	int PatternGetStride();
		
public:
	CImage* m_pImage;
	INT m_bReleaseImage;
	
	WrapMode m_wrapMode;
	CMatrix m_mtx;

	CColor m_colors[2];

	bool m_bUseBounds;
	CDoubleRect m_oBounds;

	BYTE Alpha;
};
}

#endif // _BUILD_BRUSH_H_
