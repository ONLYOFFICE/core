
#include "stdafx.h"
#include "TextToPath.h"

// constructor
CTextToPath::CTextToPath(const GraphicsPath& guidePath, REAL flatness)
: m_alignmentVertical(c_StringAlignmentNear)
, m_alignmentHorizontal(c_StringAlignmentNear)
, m_kerning(0.F)
, m_addKerning(0.F)
, m_path(guidePath.Clone())
{
	// It all works only if the guide path is flattened
	if (m_path) 
		m_path->Flatten(NULL, flatness);
}

// destructors
CTextToPath::~CTextToPath()
{
	if (m_path)
		delete m_path;
}

// Return: parameter t of (first) intersection of line segment pntA->pntB with circle (center, radius); r2 = radius^2.
// -1: no intersection
REAL CTextToPath::LineSegmentCircle(PointF pntA, PointF pntB, PointF center, REAL r2) const
{
	// Translate to (0, 0) to get rid of center.
	pntA = pntA - center;
	pntB = pntB - center;

	// Calculate squares of distance to (0, 0)
	REAL rA2 = pntA.X * pntA.X + pntA.Y * pntA.Y;
	REAL rB2 = pntB.X * pntB.X + pntB.Y * pntB.Y;

	// If line segment is completely outside circle => no intersection, return -1
	if (rA2 > r2 && rB2 > r2) 
		return -1;

	// If line segment is completely inside circle => no intersection, return -1
	if (rA2 < r2 && rB2 < r2) 
		return -1;

	PointF pntDiff = pntB - pntA;

	// line segment:
	//    x = xA + t * xDiff
	//    y = yA + t * yDiff, t >= 0, t <= 1
	// circle:
	//    x^2 + y^2 = r2
	// results in standard qudratic equation:
	//    a * t^2 + b * t + c = 0
	// with:
	//    a = xDiff^2 + yDiff^2
	//    b = 2 * (xA * xDiff + yA * yDiff)
	//    c = rA2 - r2

	REAL a = pntDiff.X * pntDiff.X + pntDiff.Y * pntDiff.Y;
	REAL b = 2 * (pntA.X * pntDiff.X + pntA.Y * pntDiff.Y);
	REAL c = rA2 - r2;

	// discriminant D = b^2 - 4 * a * c
	REAL D = b * b - 4 * a * c;

	REAL t = -1;    // indicate no intersection (wouldn't be possible)

	// one intersection: t = -b / 2 * a
	if (D == 0) 
		t = - b/(2 * a);
	// two intersections: (t = -b +/- sqrt(D)) / 2 * a
	else if (D > 0)
	{
		REAL sqrtD = (REAL)sqrt(D);
		REAL t1 = (- b + sqrtD) / (2 * a);
		REAL t2 = (- b - sqrtD) / (2 * a);

		if (t1 >= 0 && t1 <= 1)
			t = (t2 >= 0 && t2 < t1)? t2 : t1;
		else if (t2 >= 0 && t2 <= 1) 
			t = t2;
	}

	return t;
}

// Convert position on path to Euclidian PointF
PointF CTextToPath::GetPathPoint(PathPosition position) const
{
	PointF point(0, 0);
	if (position < 0) 
		return point;

	int iSegment = (int) floor(position);
	if (iSegment < m_path->GetPointCount() - 1)
	{
		PathData pd;
		if (m_path->GetPathData(&pd) == Ok)
		{
			point = pd.Points[iSegment];
			PointF diff = pd.Points[iSegment +1] - pd.Points[iSegment];
			REAL t = position - iSegment;
			point.X += t * diff.X;
			point.Y += t * diff.Y;
		}
	}
	return point;
}

// Find the first position to the right where the path intersects with circle
PathPosition CTextToPath::FindRightPosition(PathPosition posCenter, REAL radius) const
{
	if (posCenter < 0) 
		return -1;

	int iStartSegment = (int) floor(posCenter);
	if (iStartSegment >= m_path->GetPointCount() - 1) 
		return -1;

	PathData pd;
	if (m_path->GetPathData(&pd) == Ok)
	{
		PointF pntStart = GetPathPoint(posCenter);

		REAL r2 = radius * radius;

		// First, see if the intersection point is on the first segment
		// I.o.w. see whether the radius is closer than the right point of the first segment
		PointF diffRight = pntStart - pd.Points[iStartSegment + 1];
		REAL r2Right = diffRight.X * diffRight.X + diffRight.Y * diffRight.Y;
		if (r2 < r2Right)
		{
			// Yes, it is, so intersection point is on first segment.
			PointF diffLeft = pntStart - pd.Points[iStartSegment];
			REAL r2Left = diffLeft.X * diffLeft.X + diffLeft.Y * diffLeft.Y;

			REAL rLeft = (REAL)sqrt(r2Left);   // Distance from start point to left point of segment
			REAL rRight =(REAL)sqrt(r2Right);  // Distance from start point to right point of segment

			return iStartSegment + 1.0F - (rRight - radius)/(rRight + rLeft);
		}

		// No, the searched point is not on the first segment
		// So search in next segments
		for (int i = iStartSegment + 1; i < m_path->GetPointCount() - 1; i++)
		{
			REAL t = LineSegmentCircle(pd.Points[i], pd.Points[i + 1], pntStart, r2);
			if (t >= 0) 
				return i + t;	// Found.
		}
	}
	return -1;	// Not found.
}

PathPosition CTextToPath::SetTransform(Graphics* graphics, PathPosition position, REAL wCell) const
{
	// pntOrigin will be updated to the lower right of the character cell.
	if (wCell == 0) 
		return -1;	// Can't work with this.

	// Use this information to find the right position.
	PathPosition posRight = FindRightPosition(position, wCell);
	if (posRight < 0) 
		return posRight;    // Some error, ignore this glyph and return start position

	// Convert lower left and lower right to Euclidian PointF's.
	PointF pntLeft = GetPathPoint(position);
	PointF pntRight = GetPathPoint(posRight);

	// Use these to calculate the cosine and sine of the rotation
	// Note that we don't need the value of the angle itself
	PointF pntDiff = pntRight - pntLeft;
	REAL cosPhi = pntDiff.X / wCell;    // wCell != 0.
	REAL sinPhi = pntDiff.Y / wCell;

	// Fill in a transformation matrix, employing some clever
	Matrix matrix(cosPhi, sinPhi, - sinPhi, cosPhi, pntLeft.X, pntLeft.Y);
	graphics->SetTransform(&matrix);

	return posRight;
}

// Transform indent in logical co-ordinates in position
PathPosition CTextToPath::GetPathPosition(REAL indent) const
{
	PathData pd;
	if (m_path->GetPathData(&pd) == Ok)
	{
		for (int i = 0; i < m_path->GetPointCount() - 1; i++)
		{
			PointF pntDiff = pd.Points[i] - pd.Points[i + 1];
			REAL length = (REAL)sqrt(pntDiff.X * pntDiff.X + pntDiff.Y * pntDiff.Y);
			if (indent < length) 
				return i + indent/length;
			indent -= length;
		}
	}
	return -1;
}

// Draw line path for debug
void CTextToPath::DrawLine(Graphics* Gr)
{
	if (!Gr)
		return; 

	int nCount = m_path->GetPointCount();
	Point* pts = new Point[nCount];
	m_path->GetPathPoints(pts, nCount);
	Pen pen(Color(255, 0 ,0 ,0), 1);
	Gr->DrawLines(&pen, pts, nCount);
	if (pts)
		delete[] pts;
}

void CTextToPath::DrawText(Graphics* Gr, Font* font, Painter::CBrush* brush, Painter::CTextureManager* textures, Painter::CEdgeText* edge, Painter::CShadow* shadow, BSTR Text, int length)
{
	if (!Gr)
		return; 

	// draw line path for debug
//	DrawLine(Gr);

	// get bound rect for string
	REAL size = font->GetSize();
	RectF rectDraw(0.F, 0.F, size*length, size);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	Gr->SetTextRenderingHint(TextRenderingHintAntiAlias);
	RectF rectString;
	Gr->MeasureString(Text, length, font, rectDraw, &format, &rectString);

	// save old smoothing mode and set high quality mode
	SmoothingMode oldSmoothingMode = Gr->GetSmoothingMode();
	Gr->SetSmoothingMode(SmoothingModeHighQuality);

	// draw shadow
	RectF rectCharacter;
	PathPosition pos = GetStartPosition(Gr, font, format, Text, length, rectString);
	REAL shift = m_kerning + m_addKerning;
	if (shift)
		pos = SetTransform(Gr, pos, shift/2.F);
	PathPosition oldPos = pos;
	if (shadow->Visible)
		for(int index = 0; index < length; index++)
		{
			// get rect of current(index) letter
			format.SetLineAlignment(StringAlignmentCenter);
			Painter::GetRectCurrentCharacter(Gr, &format, font, Text, length, index, rectString, rectCharacter);

			// get position letter on curve from begin and transform graphics for letter
			if (index && shift)
				pos = SetTransform(Gr, pos, shift);
			pos = SetTransform(Gr, pos, rectCharacter.Width);

			// draw letter
			if (pos < 0)
				break;

			// set rest for letter
			rectCharacter.X = 0.F;
			rectCharacter.Y = -rectCharacter.Height;
			rectCharacter.Height *= 2;

			// set format
			format.SetLineAlignment((StringAlignment)m_alignmentVertical);

			// draw shadow
			shadow->Draw(Gr, font, &format, Text + index, 1, rectCharacter);

			// reset transforms
			Gr->ResetTransform();
		}

	// draw text
	pos = oldPos;
	for(int index = 0; index < length; index++)
	{
		// get rect of current(index) letter
		format.SetLineAlignment(StringAlignmentCenter);
		Painter::GetRectCurrentCharacter(Gr, &format, font, Text, length, index, rectString, rectCharacter);

		// get position letter on curve from begin and transform graphics for letter
		if (index && shift)
			pos = SetTransform(Gr, pos, shift);
		pos = SetTransform(Gr, pos, rectCharacter.Width);

		// draw letter
		if (pos < 0)
			break;

		// set rest for letter
		rectCharacter.X = 0.F;
		rectCharacter.Y = -rectCharacter.Height;
		rectCharacter.Height *= 2;

		// set format
		format.SetLineAlignment((StringAlignment)m_alignmentVertical);

		// draw edge
		edge->Draw(Gr, font, &format, Text + index, 1, rectCharacter);

		// draw letter
		Brush* pBrush = brush->GetBrush(textures, &rectCharacter, index);
		Gr->DrawString(Text + index, 1, font, rectCharacter, &format, pBrush);
/*
		// draw rect for letter for debug
		Pen blackPen(Color(255, 0, 0, 0), 1);
		Gr->DrawRectangle(&blackPen, rectCharacter);
*/
		// reset transforms
		Gr->ResetTransform();
	}

	// restore old smoothing mode
	Gr->SetSmoothingMode(oldSmoothingMode);
}

// Get start position according to horizontal alignment
PathPosition CTextToPath::GetStartPosition(Graphics* Gr, Font* font, StringFormat& format, BSTR Text, int length, RectF& rectString)
{
	PathPosition pos = GetPathPosition(0);
	m_addKerning = 0.F;

	// case alignment near
	if (c_StringAlignmentNear == m_alignmentHorizontal)
		return 0.F;

	RectF rectCharacter;
	for(int index = 0; index < length; index++)
	{
		// get rect of current(index) letter
		format.SetLineAlignment(StringAlignmentCenter);
		Painter::GetRectCurrentCharacter(Gr, &format, font, Text, length, index, rectString, rectCharacter);

		// get position letter on curve from begin and transform graphics for letter
		pos = SetTransform(Gr, pos, rectCharacter.Width + m_kerning);

		// string is long then curve
		if (pos < 0)
			return 0.F;
	}

	// get rect of 0-th letter
	RectF testRect;
	format.SetLineAlignment(StringAlignmentCenter);
	Painter::GetRectCurrentCharacter(Gr, &format, font, Text, length, 0, rectString, testRect);

	// get count of free cells
	int nCount = -1;
	PathPosition posAdd = pos;
	while (pos >= 0)
	{
		nCount++;
		pos = SetTransform(Gr, pos, testRect.Width + m_kerning);
	}

	// case alignment center
	if (c_StringAlignmentCenter == m_alignmentHorizontal)
	{
		nCount = nCount/2;
		pos = (GetPathPosition(0));
		for(int index = 0; index < nCount; index++)
			pos = SetTransform(Gr, pos, testRect.Width + m_kerning);

		return pos;
	}

	// case alignment far
	if (c_StringAlignmentFar == m_alignmentHorizontal)
	{
		pos = (GetPathPosition(0));
		for(int index = 0; index < nCount; index++)
			pos = SetTransform(Gr, pos, testRect.Width + m_kerning);

		return pos;
	}

	// case alignment stretch
	if (c_StringAlignmentStretch == m_alignmentHorizontal)
	{
		m_addKerning = (testRect.Width + m_kerning)*nCount/length;

		// check result
		pos = GetPathPosition(0);
		for(int index = 0; index < length; index++)
		{
			// get rect of current(index) letter
			format.SetLineAlignment(StringAlignmentCenter);
			Painter::GetRectCurrentCharacter(Gr, &format, font, Text, length, index, rectString, rectCharacter);

			// get position letter on curve from begin and transform graphics for letter
			pos = SetTransform(Gr, pos, rectCharacter.Width + m_kerning + m_addKerning);

			// string is long then curve
			if (pos < 0)
			{
				nCount--;
				break;
			}
		}
		m_addKerning = (testRect.Width + m_kerning)*nCount/length;

		return 0.F;
	}

	return 0.F;
}

// format  functions
void CTextToPath::SetAlignmentVertical(int alignmentVertical)
{
	if (c_StringAlignmentNear <= alignmentVertical && c_StringAlignmentFar >= alignmentVertical)
		m_alignmentVertical = alignmentVertical;
}
void CTextToPath::SetAlignmentHorizontal(int alignmentHorizontal)
{
	if (c_StringAlignmentNear <= alignmentHorizontal && c_StringAlignmentStretch >= alignmentHorizontal)
		m_alignmentHorizontal = alignmentHorizontal;
}
void CTextToPath::SetKerning(REAL kerning)
{
	if (kerning >= 0.F)
		m_kerning = kerning;
}
