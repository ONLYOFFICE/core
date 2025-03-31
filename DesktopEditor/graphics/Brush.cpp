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
#include "Brush.h"

namespace Aggplus
{
CBrush::CBrush()
{
}

CBrush::CBrush(const BrushType& type)
	: m_bType(type)
{
}

CBrush::~CBrush()
{
}

BrushType CBrush::GetType() const
{
	return m_bType;
}

CBrushSolid::CBrushSolid() : CBrush()
{
}

CBrushSolid::CBrushSolid(CColor color)
	: CBrush(),
	m_dwColor(color)
{
}

CBrushSolid::CBrushSolid(const CBrushSolid& other) : CBrush()
{
	*this = other;
}

CBrushSolid::~CBrushSolid()
{
}

void CBrushSolid::GetColor(CColor* color) const
{
	*color = m_dwColor;
}

void CBrushSolid::SetColor(const CColor &color)
{
	m_dwColor = color;
}

CBrushSolid& CBrushSolid::operator=(const CBrushSolid& other)
{
	if (this == &other)
		return *this;

	m_dwColor = other.m_dwColor;
	return *this;
}

CBrushHatch::CBrushHatch()
	: CBrush(BrushTypeHatchFill)
{
}

CBrushHatch::CBrushHatch(const CBrushHatch& other)
	: CBrush(BrushTypeHatchFill)
{
	*this = other;
}

CBrushHatch::CBrushHatch(CBrushHatch&& other) noexcept
	: CBrush(BrushTypeHatchFill)
{
	*this = std::move(other);
}

CBrushHatch::~CBrushHatch()
{
}

void CBrushHatch::SetName(const std::wstring& name)
{
	m_wsName = name;
}

std::wstring CBrushHatch::GetName() const
{
	return m_wsName;
}

void CBrushHatch::SetColor1(const CColor& color)
{
	m_dwColor1 = color;
}

CColor CBrushHatch::GetColor1() const
{
	return m_dwColor1;
}

void CBrushHatch::SetColor2(const CColor& color)
{
	m_dwColor2 = color;
}

CColor CBrushHatch::GetColor2() const
{
	return m_dwColor2;
}

void CBrushHatch::SetBounds(const CDoubleRect& rect)
{
	m_oBounds = rect;
}

CDoubleRect& CBrushHatch::GetBounds()
{
	return m_oBounds;
}

CBrushHatch& CBrushHatch::operator=(const CBrushHatch& other)
{
	if (this == &other)
		return *this;

	m_wsName = other.m_wsName;
	m_dwColor1 = other.m_dwColor1;
	m_dwColor2 = other.m_dwColor2;
	return *this;
}

CBrushHatch& CBrushHatch::operator=(CBrushHatch&& other) noexcept
{
	if (this == &other)
		return *this;

	m_wsName = std::move(other.m_wsName);
	m_dwColor1 = other.m_dwColor1;
	m_dwColor2 = other.m_dwColor2;
	return *this;
}

CBrushLinearGradient::CBrushLinearGradient()
	: CBrush(BrushTypeLinearGradient)
{
}

CBrushLinearGradient::CBrushLinearGradient(const PointF& p1, const PointF& p2, const CColor& c1, const CColor& c2)
	: CBrush(BrushTypeLinearGradient)
{
	m_arPoints[0] = p1;
	m_arPoints[1] = p2;

	m_arColors[0] = c1;
	m_arColors[1] = c2;
}

CBrushLinearGradient::CBrushLinearGradient(const Point& p1, const Point& p2, const CColor& c1, const CColor& c2)
	: CBrush(BrushTypeLinearGradient)
{
	m_arPoints[0].X = static_cast<float>(p1.X);
	m_arPoints[0].Y = static_cast<float>(p1.Y);
	m_arPoints[1].X = static_cast<float>(p2.X);
	m_arPoints[1].Y = static_cast<float>(p2.Y);

	m_arColors[0] = c1;
	m_arColors[1] = c2;
}

CBrushLinearGradient::CBrushLinearGradient(const RectF& rect, const CColor& c1, const CColor& c2, float angle, bool isAngleScalable)
	: CBrush(BrushTypeLinearGradient)
{
	m_arPoints[0].X = rect.GetLeft();
	m_arPoints[0].Y = rect.GetTop();
	m_arPoints[1].X = rect.GetRight();
	m_arPoints[1].Y = rect.GetBottom();

	m_arColors[0] = c1;
	m_arColors[1] = c2;

	m_fAngle = angle;

	m_bAngleScalable = isAngleScalable;
	m_bRectable = true;
}

CBrushLinearGradient::CBrushLinearGradient(const Rect& rect, const CColor& c1, const CColor& c2, float angle, bool isAngleScalable)
	: CBrush(BrushTypeLinearGradient)
{
	m_arPoints[0].X = static_cast<float>(rect.GetLeft());
	m_arPoints[0].Y = static_cast<float>(rect.GetTop());
	m_arPoints[1].X = static_cast<float>(rect.GetRight());
	m_arPoints[1].Y = static_cast<float>(rect.GetBottom());

	m_arColors[0] = c1;
	m_arColors[1] = c2;

	m_fAngle = angle;

	m_bAngleScalable = isAngleScalable;
	m_bRectable = true;
}

CBrushLinearGradient::CBrushLinearGradient(const RectF& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode)
	: CBrush(BrushTypeLinearGradient)
{
	m_arPoints[0].X = rect.GetLeft();
	m_arPoints[0].Y = rect.GetTop();
	m_arPoints[1].X = rect.GetRight();
	m_arPoints[1].Y = rect.GetBottom();

	switch( mode )
	{
	case LinearGradientModeHorizontal:
		m_fAngle = 0;
		break;
	case LinearGradientModeVertical:
		m_fAngle = 90;
		break;
	case LinearGradientModeForwardDiagonal:
		m_fAngle = 45;
		break;
	default:
		m_fAngle = 315;
	}

	m_arColors[0] = c1;
	m_arColors[1] = c2;

	m_bAngleScalable = true;
	m_bRectable = true;
}

CBrushLinearGradient::CBrushLinearGradient(const Rect& rect, const CColor& c1, const CColor& c2, Aggplus::LinearGradientMode mode)
	: CBrush(BrushTypeLinearGradient)
{
	m_arPoints[0].X = static_cast<float>(rect.GetLeft());
	m_arPoints[0].Y = static_cast<float>(rect.GetTop());
	m_arPoints[1].X = static_cast<float>(rect.GetRight());
	m_arPoints[1].Y = static_cast<float>(rect.GetBottom());

	switch( mode )
	{
	case LinearGradientModeHorizontal:
		m_fAngle = 0;
		break;
	case LinearGradientModeVertical:
		m_fAngle = 90;
		break;
	case LinearGradientModeForwardDiagonal:
		m_fAngle = 45;
		break;
	default:
		m_fAngle = 315;
	}

	m_arColors[0] = c1;
	m_arColors[1] = c2;

	m_bAngleScalable = true;
	m_bRectable = true;
}


CBrushLinearGradient::CBrushLinearGradient(const CBrushLinearGradient& other)
	: CBrush(other.m_bType)
{
	*this = other;
}

CBrushLinearGradient::CBrushLinearGradient(CBrushLinearGradient&& other) noexcept
{
	*this = std::move(other);
}

Status CBrushLinearGradient::GetLinearColors(CColor* colors) const
{
	if(!colors)
		return InvalidParameter;

	colors[0] = m_arColors[0];
	colors[1] = m_arColors[1];

	return Ok;
}

Status CBrushLinearGradient::GetRectangle(Rect *rect) const
{
	if(!rect)
		return InvalidParameter;

	rect->X		 = static_cast<int>((m_arPoints[0].X));
	rect->Y 	 = static_cast<int>((m_arPoints[0].Y));
	rect->Width  = static_cast<int>((m_arPoints[1].X) - rect->X);
	rect->Height = static_cast<int>((m_arPoints[1].Y) - rect->Y);
	return Ok;
}

Status CBrushLinearGradient::GetRectangle(RectF *rect) const
{
	if(!rect)
		return InvalidParameter;

	rect->X 	 = m_arPoints[0].X;
	rect->Y 	 = m_arPoints[0].Y;
	rect->Width  = m_arPoints[1].X - rect->X;
	rect->Height = m_arPoints[1].Y - rect->Y;

	return Ok;
}

Status CBrushLinearGradient::GetTransform(CMatrix* matrix) const
{
	if(!matrix)
		return InvalidParameter;

	*matrix = m_Matrix;

	return Ok;
}

Status CBrushLinearGradient::MultiplyTransform(const CMatrix *matrix, MatrixOrder order)
{
	if( !matrix )
		return InvalidParameter;

	m_Matrix.Multiply(matrix, order);

	return Ok;
}

Status CBrushLinearGradient::ResetTransform()
{
	m_Matrix.Reset();

	return Ok;
}

Status CBrushLinearGradient::RotateTransform(float angle, MatrixOrder order)
{
	m_Matrix.Rotate(angle, order);

	return Ok;
}

Status CBrushLinearGradient::ScaleTransform(float sx, float sy, MatrixOrder order)
{
	m_Matrix.Scale(sx, sy, order);

	return Ok;
}

void CBrushLinearGradient::SetWrapMode(WrapMode mode)
{
	m_eWrap = mode;
}

WrapMode CBrushLinearGradient::GetWrapMode() const
{
	return m_eWrap;
}

Status CBrushLinearGradient::SetInterpolationColors(const CColor *presetColors, const float *blendPositions, int count)
{
	m_arSubColors.clear();

	if(count > 0 && presetColors && blendPositions)
	{
		m_arSubColors.resize(count);

		for(int i = 0; i < count; i++)
		{
			m_arSubColors[i].color = presetColors[i];
			m_arSubColors[i].position = blendPositions[i];
		}
	}

	return Ok;
}

Status CBrushLinearGradient::GetInterpolationColors(CColor *presetColors, float *blendPositions, int count) const
{
	if( count > 0 && count <= (int)m_arSubColors.size() && presetColors && blendPositions )
	{
		for( int i = 0; i < count; i++ )
		{
			presetColors[i] = m_arSubColors[i].color;
			blendPositions[i] = m_arSubColors[i].position;
		}
	}

	return Ok;
}

int CBrushLinearGradient::GetInterpolationColorsCount() const
{
	return static_cast<int>(m_arSubColors.size());
}

void CBrushLinearGradient::GetSubColor(int index, CColor* color, float* position) const
{
	*color = m_arSubColors[index].color;
	*position = m_arSubColors[index].position;
}
	
void CBrushLinearGradient::SetRelativeCoords(bool relative)
{
	m_bRelativeCoords = relative;
}

bool CBrushLinearGradient::IsRelativeCoords() const
{
	return m_bRelativeCoords;
}

bool CBrushLinearGradient::IsAngleScalable() const
{
	return m_bAngleScalable;
}

bool CBrushLinearGradient::IsRectable() const
{
	return m_bRectable;
}

float CBrushLinearGradient::GetAngle() const
{
	return m_fAngle;
}

void CBrushLinearGradient::SetBounds(const CDoubleRect& rect)
{
	m_oBounds = rect;
}

CDoubleRect& CBrushLinearGradient::GetBounds()
{
	return m_oBounds;
}

CBrushLinearGradient& CBrushLinearGradient::operator=(const CBrushLinearGradient& other)
{
	if (this == &other)
		return *this;

	m_arColors[0] = other.m_arColors[0];
	m_arColors[1] = other.m_arColors[1];
	m_arPoints[0] = other.m_arPoints[0];
	m_arPoints[1] = other.m_arPoints[1];

	m_arSubColors = other.m_arSubColors;

	m_Matrix = other.m_Matrix;
	m_fAngle = other.m_fAngle;

	m_eWrap = other.m_eWrap;

	m_bAngleScalable = other.m_bAngleScalable;
	m_bRectable = other.m_bRectable;
	m_bRelativeCoords = other.m_bRelativeCoords;

	return *this;
}

CBrushLinearGradient& CBrushLinearGradient::operator=(CBrushLinearGradient&& other) noexcept
{
	if (this == &other)
		return *this;

	m_arColors[0] = other.m_arColors[0];
	m_arColors[1] = other.m_arColors[1];
	m_arPoints[0] = other.m_arPoints[0];
	m_arPoints[1] = other.m_arPoints[1];

	m_arSubColors = other.m_arSubColors;

	m_Matrix = std::move(other.m_Matrix);

	m_eWrap = other.m_eWrap;

	m_bAngleScalable = other.m_bAngleScalable;
	m_bRectable = other.m_bRectable;
	m_bRelativeCoords = other.m_bRelativeCoords;

	return *this;
}

CBrushTexture::CBrushTexture()
	: CBrush(BrushTypeTextureFill)
{
}

CBrushTexture::CBrushTexture(const std::wstring& name, WrapMode mode)
	: CBrush(BrushTypeTextureFill),
	m_eWrapMode(mode)
{
	m_pImage = new CImage(name);
	m_bReleaseImage = true;
}

CBrushTexture::CBrushTexture(CImage *image, WrapMode mode)
	: CBrush(BrushTypeTextureFill),
	m_eWrapMode(mode)
{
	m_pImage = image;
}

CBrushTexture::CBrushTexture(const CBrushTexture& other)
{
	*this = other;
}

CBrushTexture::CBrushTexture(CBrushTexture&& other)
{
	*this = std::move(other);
}

CBrushTexture::~CBrushTexture()
{
	if (m_bReleaseImage)
		RELEASEOBJECT(m_pImage);
}

void CBrushTexture::TranslateTransform(double x, double y, MatrixOrder order)
{
	m_Matrix.Translate(x, y, order);
}

void CBrushTexture::ScaleTransform(double x, double y, MatrixOrder order)
{
	m_Matrix.Scale(x, y, order);
}

void CBrushTexture::RotateTransform(double angle, MatrixOrder order)
{
	m_Matrix.Rotate(angle, order);
}

void CBrushTexture::GetTransform(CMatrix* matrix) const
{
	*matrix = m_Matrix;
}

void CBrushTexture::SetTransform(const CMatrix* matrix)
{
	m_Matrix = *matrix;
}

void CBrushTexture::SetWrapMode(WrapMode mode)
{
	m_eWrapMode = mode;
}

WrapMode CBrushTexture::GetWrapMode() const
{
	return m_eWrapMode;
}

void CBrushTexture::SetBounds(const CDoubleRect& rect)
{
	m_bUseBounds = true;
	m_oBounds = rect;
}

CDoubleRect& CBrushTexture::GetBounds()
{
	return m_oBounds;
}

void CBrushTexture::SetReleaseImage(bool isReleaseImage)
{
	m_bReleaseImage = isReleaseImage;
}

bool CBrushTexture::IsReleaseImage() const
{
	return m_bReleaseImage;
}

void CBrushTexture::SetAlpha(BYTE alpha)
{
	m_Alpha = alpha;
}

BYTE CBrushTexture::GetAlpha() const
{
	return m_Alpha;
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

CBrushTexture& CBrushTexture::operator=(const CBrushTexture& other)
{
	if (this == &other)
		return *this;

	m_pImage = new CImage(*other.m_pImage);

	m_Matrix = other.m_Matrix;

	m_eWrapMode = other.m_eWrapMode;

	return *this;
}

CBrushTexture& CBrushTexture::operator=(CBrushTexture&& other)
{
	if (this == &other)
		return *this;

	m_pImage = std::move(other.m_pImage);

	m_Matrix = std::move(other.m_Matrix);

	m_eWrapMode = other.m_eWrapMode;

	return *this;
}
}
