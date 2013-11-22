#pragma once
#include "stdafx.h"
#include "DIB.h"
#include "..\Interfaces\AVSGraphicsRenderer.h"

void NSDocumentViewer::CPainter::SetDpi(double dDpiX, double dDpiY)
{
	m_dKoefPixToMM_X	= c_dInch_to_MM / dDpiX;
	m_dKoefPixToMM_Y	= c_dInch_to_MM / dDpiY;
	
	double dWidthMM		= m_lDisplayWidth  * m_dKoefPixToMM_X;
	double dHeightMM	= m_lDisplayHeight * m_dKoefPixToMM_Y;

	((CAVSGraphicsRenderer*)m_pRenderer)->SetSizes(dWidthMM, dHeightMM);
}