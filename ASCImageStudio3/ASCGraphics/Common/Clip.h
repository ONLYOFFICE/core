#pragma once
#include "..\stdafx.h"

#include "..\VectorGraphics\SClip.h"
#include "GraphicsPath.h"


class CPixels; 

class CClip : public SClip
{
public:
	CClip(double dMinX = 0, double dMinY = 0, double dMaxX = 0, double dMaxY = 0, BOOL bAntialias = TRUE) : SClip(dMinX, dMinY, dMaxX, dMaxY, bAntialias)
	{
		m_dLeft		= dMinX;
		m_dTop		= dMinY;
		m_dRight	= dMaxX;
		m_dBottom	= dMaxY;
	}
	~CClip()
	{
	}

	void Reset(double dMinX, double dMinY, double dMaxX, double dMaxY, BOOL bAntialias);
	void Reset();

public:

	BOOL Intersect(CGraphicsPath* pPath, double* pMatrix, double dFlatness, BOOL bEO)
	{
		return (0 == SClip::ÑlipToPath(pPath, pMatrix, dFlatness, bEO));
	}
	BOOL Exclude(CGraphicsPath* pPath, double* pMatrix, double dFlatness, BOOL bEO)
	{
		CGraphicsPath oPath;
		oPath.AddRectangle(m_dLeft, m_dTop, m_dRight - m_dLeft, m_dBottom - m_dTop);

		oPath.AddPath(*pPath);

		return Intersect(&oPath, pMatrix, dFlatness, bEO);
	}

	void ClipAALine(CPixels* pAABuffer, int *pnX0, int *pnX1, int nY);

private:

	double m_dLeft;
	double m_dTop;
	double m_dRight;
	double m_dBottom;	
};
