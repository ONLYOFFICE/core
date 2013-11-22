#pragma once
#include <PaintStruct.h>

typedef REAL PathPosition;

// constants for format text
const int c_StringAlignmentNear =		0;
const int c_StringAlignmentCenter =		1;
const int c_StringAlignmentFar =		2;
const int c_StringAlignmentStretch =	3;

// class CTextToPath
class CTextToPath
{
public:
	// constructors-destructors
	CTextToPath(const GraphicsPath& guidePath, REAL flatness = FlatnessDefault);
	~CTextToPath();

	// draw functions
	void DrawText(Graphics* grahics, Font* font, Painter::CBrush* brush, Painter::CTextureManager* textures, Painter::CEdgeText* edge, Painter::CShadow* shadow, BSTR Text, int length);
	void DrawLine(Graphics* Gr);

	// format  functions
	void SetAlignmentVertical(int alignmentVertical = c_StringAlignmentNear);
	void SetAlignmentHorizontal(int alignmentHorizontal = c_StringAlignmentNear);
	void SetKerning(REAL kerning);

protected:
	GraphicsPath*	m_path;
	int				m_alignmentVertical;
	int				m_alignmentHorizontal;
	REAL			m_kerning;
	REAL			m_addKerning;

	// methods
	PathPosition	GetStartPosition(Graphics* grahics, Font* font, StringFormat& format, BSTR Text, int length, RectF& rectString);
	PathPosition	SetTransform(Graphics* graphics, PathPosition position, REAL wCell) const;
	PointF			GetPathPoint(PathPosition position) const;
	PathPosition	GetPathPosition(REAL indent) const;
	PathPosition	FindRightPosition(PathPosition posCenter, REAL radius) const;
	REAL			LineSegmentCircle(PointF pntA, PointF pntB, PointF center, REAL r2) const;
};