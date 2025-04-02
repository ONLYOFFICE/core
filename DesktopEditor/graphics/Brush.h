/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
/**
 * @abstract CBrush
 *
 * Base class for brushes (by default a solid brush
 * is created).
 */
class CBrush
{
	friend class CGraphics;

protected:
	CBrush();
	CBrush(const BrushType& type);
	 
public:
	virtual ~CBrush();

	BrushType GetType() const;

public:
	BrushType m_bType = BrushTypeSolidColor;
};

/**
 * @class CBrushSolid
 *
 * A single-color brush used to draw text, graphic paths,
 * and fills with a single color.
 */
class CBrushSolid : public CBrush
{
public:
	CBrushSolid();
	CBrushSolid(CColor color);
	virtual ~CBrushSolid();
	CBrushSolid(const CBrushSolid& other);

	void GetColor(CColor* color) const;
	void SetColor(const CColor &color);
	
	CBrushSolid& operator=(const CBrushSolid& other);
protected:
	CColor m_dwColor{};
};

/**
 * @class CBrushHatch
 *
 * A two-color brush used to fill graphic paths using
 * one of the hatch patterns
 */
class CBrushHatch : public CBrush
{
public:
	CBrushHatch();
	CBrushHatch(const CBrushHatch& other);
	CBrushHatch(CBrushHatch&& other) noexcept;
	virtual ~CBrushHatch();

	void SetName(const std::wstring& name);
	std::wstring GetName() const;

	void SetColor1(const CColor& color);
	CColor GetColor1() const;
	void SetColor2(const CColor& color);
	CColor GetColor2() const;

	void SetBounds(const CDoubleRect& rect);
	CDoubleRect& GetBounds();

	CBrushHatch& operator=(const CBrushHatch& other);
	CBrushHatch& operator=(CBrushHatch&& other) noexcept;
protected:
	/**
	 * @brief m_wsName - hatch brush pattern name
	 *   (as default - cross)
	 *
	 * See all patterns in agg_span_hatch.h, array -
	 * c_resource_hatches_names.
	 */
	std::wstring	m_wsName;
	CColor			m_dwColor1{};
	CColor			m_dwColor2{};

	CDoubleRect		m_oBounds{};
};

/**
 * @class CBrushLinearGradient
 *
 * brush for drawing gradients, stores information about the gradient.
 * According to the pdf documentation, it stores the start and end point
 * and color, as well as the linear interpolation of the colors.
 */
class CBrushLinearGradient : public CBrush
{
	friend class CGraphics;

public:
	CBrushLinearGradient();
	CBrushLinearGradient(const PointF& p1, const PointF& p2, const CColor& c1, const CColor& c2);
	CBrushLinearGradient(const Point& p1, const Point& p2, const CColor& c1, const CColor& c2);
	CBrushLinearGradient(const RectF& rect, const CColor& c1, const CColor& c2, float angle, bool isAngleScalable);
	CBrushLinearGradient(const Rect& rect, const CColor& c1, const CColor& c2, float angle, bool isAngleScalable);
	CBrushLinearGradient(const RectF& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode);
	CBrushLinearGradient(const Rect& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode);
	CBrushLinearGradient(const CBrushLinearGradient& other);
	CBrushLinearGradient(CBrushLinearGradient&& other) noexcept;
	virtual ~CBrushLinearGradient();

	Status GetLinearColors(CColor* colors) const;
	Status GetRectangle(Rect *rect) const;
	Status GetRectangle(RectF *rect) const;

	Status GetTransform(CMatrix* matrix) const;
	Status MultiplyTransform(const CMatrix *matrix, MatrixOrder order = MatrixOrderPrepend);
	Status ResetTransform();
	Status RotateTransform(float angle, MatrixOrder order = MatrixOrderPrepend );
	Status ScaleTransform(float sx, float sy, MatrixOrder order = MatrixOrderPrepend );

	void SetWrapMode(WrapMode mode);
	WrapMode GetWrapMode() const;

	Status SetInterpolationColors(const CColor *presetColors, const float *blendPositions, int count);
	Status GetInterpolationColors(CColor *presetColors, float *blendPositions, int count) const;
	int GetInterpolationColorsCount() const;

	void GetSubColor(int index, CColor* color, float* position) const;
	
	void SetRelativeCoords(bool relative);
	bool IsRelativeCoords() const;
	bool IsAngleScalable() const;
	bool IsRectable() const;
	float GetAngle() const;

	void SetBounds(const CDoubleRect& rect);
	CDoubleRect& GetBounds();

	CBrushLinearGradient& operator=(const CBrushLinearGradient& other);
	CBrushLinearGradient& operator=(CBrushLinearGradient&& other) noexcept;

	NSStructures::GradientInfo m_oGradientInfo{};
protected:
	CColor m_arColors[2];
	PointF m_arPoints[2];

	struct TSubColor
	{
		CColor color{};
		float position = 0;
	};
	
	std::vector<TSubColor> m_arSubColors;

	CMatrix m_Matrix{};

	/**
	 * @brief m_angle - rotation angle of line p1 - p2 (measured in degrees)
	 */
	float m_fAngle = 0;

	CDoubleRect m_oBounds{};

	/**
	 * @brief m_eWrapMode - Used to determine the rotation of the image tiles
	 *   (by default, we do not change the position).
	 */
	WrapMode m_eWrap = Aggplus::WrapModeTile;

	/**
	 * @brief m_bAngleScalable - whether to scale the rotation angle relative
	 *   to the given points
	 *
	 * calculated like this b = arctan(width / height * tan(angle));
	 */
	bool m_bAngleScalable = false;

	/**
	 * @brief m_bRectable - is the diagonal of a rectangle a guide
	 */
	bool m_bRectable = false;

	/**
	 * @brief m_bRelativeCoords - are the coordinates of the points calculated
	 *   relative to the resulting primitive
	 */
	bool m_bRelativeCoords = false;
};

/**
 * @class CBrushTexture
 *
 * A texture brush that is used to draw images on the renderer.
 */
class CBrushTexture : public CBrush
{
	friend class CGraphics;

public:
	CBrushTexture();
	CBrushTexture(const std::wstring& name, WrapMode mode = WrapModeTile);
	CBrushTexture(CImage *image, WrapMode mode = WrapModeTile);
	CBrushTexture(const CBrushTexture& other);
	CBrushTexture(CBrushTexture&& other);
	virtual ~CBrushTexture();

	void TranslateTransform(double x, double y, MatrixOrder order = MatrixOrderPrepend);
	void ScaleTransform(double x, double y, MatrixOrder order = MatrixOrderPrepend);
	void RotateTransform(double angle, MatrixOrder order = MatrixOrderPrepend);
	void GetTransform(CMatrix* matrix) const;
	void SetTransform(const CMatrix* matrix);

	void SetWrapMode(WrapMode mode);
	WrapMode GetWrapMode() const;

	void SetBounds(const CDoubleRect& rect);
	CDoubleRect& GetBounds();

	void SetReleaseImage(bool isReleaseImage);
	bool IsReleaseImage() const;

	void SetAlpha(BYTE alpha);
	BYTE GetAlpha() const;

	void* GetData();
	void* PatternFinalize();
	DWORD PatternGetWidth();
	DWORD PatternGetHeight();
	int PatternGetStride();

	CBrushTexture& operator=(const CBrushTexture& other);
	CBrushTexture& operator=(CBrushTexture&& other);

protected:
	CImage* m_pImage{nullptr};
	bool m_bReleaseImage = false;
	
	/**
	 * @brief m_eWrapMode - Used to determine the rotation of the image tiles
	 *   (by default, we do not change the position).
	 */
	WrapMode m_eWrapMode = WrapModeTile;

	CMatrix m_Matrix{};

	bool m_bUseBounds = false;
	CDoubleRect m_oBounds{};

	BYTE m_Alpha = 255;
};
}

#endif // _BUILD_BRUSH_H_
