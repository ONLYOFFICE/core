
#include "stdafx.h"
#include "ImagePaint3.h"
#include <TransformConstants.h>
#include <ChromoKey.h>

// Draw border 
STDMETHODIMP ImagePaint3::DrawBorder(long Size, long Type)
{
	// check for valid image
	if (!Valid() || !m_pixels || m_width < 2 || m_height < 2)
		return S_FALSE;

	// clamp values
	Size = (int)max(0, min(Size, min(m_height/2 - 1, m_width/2 - 1)));

	// check for simpliest case
	if (Size == 0)
		return S_OK;

	// variables
	int nImageLeft = 0;
	int nImageTop = 0;
	int nImageRight = m_width - 1;
	int nImageBottom = m_height - 1;

	// compute image rectangle
	if (Type == c_nBorderSolidAll || Type == c_nBorderGradientLightAll || Type == c_nBorderGradientDarkAll || Type == c_nBorderTransparentAll)
	{
		nImageLeft = Size;
		nImageTop = Size;
		nImageRight = m_width - 1 - Size;
		nImageBottom = m_height - 1 - Size;
	}
	else if (Type == c_nBorderSolidLeft || Type == c_nBorderGradientLightLeft || Type == c_nBorderGradientDarkLeft || Type == c_nBorderTransparentLeft)
	{
		nImageLeft = Size;
		nImageTop = 0;
		nImageRight = m_width - 1;
		nImageBottom = m_height - 1;
	}
	else if (Type == c_nBorderSolidBottom || Type == c_nBorderGradientLightBottom || Type == c_nBorderGradientDarkBottom || Type == c_nBorderTransparentBottom)
	{
		nImageLeft = 0;
		nImageTop = Size;
		nImageRight = m_width - 1;
		nImageBottom = m_height - 1;
	}
	else if (Type == c_nBorderSolidRight || Type == c_nBorderGradientLightRight || Type == c_nBorderGradientDarkRight || Type == c_nBorderTransparentRight)
	{
		nImageLeft = 0;
		nImageTop = 0;
		nImageRight = m_width - 1 - Size;
		nImageBottom = m_height - 1;
	}
	else if (Type == c_nBorderSolidTop || Type == c_nBorderGradientLightTop || Type == c_nBorderGradientDarkTop || Type == c_nBorderTransparentTop)
	{
		nImageLeft = 0;
		nImageTop = 0;
		nImageRight = m_width - 1;
		nImageBottom = m_height - 1 - Size;
	}
	else if (Type == c_nBorderSolidLeftRight || Type == c_nBorderGradientLightLeftRight || Type == c_nBorderGradientDarkLeftRight || Type == c_nBorderTransparentLeftRight)
	{
		nImageLeft = Size;
		nImageTop = 0;
		nImageRight = m_width - 1 - Size;
		nImageBottom = m_height - 1;
	}
	else if (Type == c_nBorderSolidTopBottom || Type == c_nBorderGradientLightTopBottom || Type == c_nBorderGradientDarkTopBottom || Type == c_nBorderTransparentTopBottom)
	{
		nImageLeft = 0;
		nImageTop = Size;
		nImageRight = m_width - 1;
		nImageBottom = m_height - 1 - Size;
	}

	// variables
	int nX, nY, nIndex = 0;
	double dSourceX, dSourceY, dAlpha;
	BYTE nR1, nG1, nB1, nR2, nG2, nB2, nR3, nG3, nB3;

	// compute border color components
	long Color = m_brush.Color1;
	BYTE nBR = GetRValue(Color);
	BYTE nBG = GetGValue(Color);
	BYTE nBB = GetBValue(Color);
	BYTE nBLR = Byte(nBR + 128);
	BYTE nBLG = Byte(nBG + 128);
	BYTE nBLB = Byte(nBB + 128);
	BYTE nBDR = Byte(nBR - 128);
	BYTE nBDG = Byte(nBG - 128);
	BYTE nBDB = Byte(nBB - 128);

	// create temporary memory
	CreateBackup();

	// check for simpliest case
	if (Type < c_nBorderTransparentAll || Type > c_nBorderTransparentTopBottom)
	{
		// draw one image inside another
		for (nY = nImageTop; nY <= nImageBottom; ++nY)
		{
			// compute y coordinate
			dSourceY = min(m_height - 0.001, m_height*(nY - nImageTop)/(double)(nImageBottom - nImageTop));

			for (nX = nImageLeft; nX <= nImageRight; ++nX)
			{
				// compute x coordinate
				dSourceX = min(m_width - 0.001, m_width*(nX - nImageLeft)/(double)(nImageRight - nImageLeft));

				// check for valid destination coordinates
				if (nX < 0 || nX >= m_width || nY < 0 || nY >= m_height)
					continue;

				// compute source pixel index
				nIndex = 4*(nX + nY*m_width);

				// copy pixels
				m_pixels[nIndex + 0] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_width, m_height, 0, 4, 0);
				m_pixels[nIndex + 1] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_width, m_height, 1, 4, 0);
				m_pixels[nIndex + 2] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_width, m_height, 2, 4, 0);
			}
		}
	}

	// draw border
	for (nY = 0, nIndex = 0; nY < m_height; ++nY)
	{
		for (nX = 0; nX < m_width; ++nX, nIndex += 4)
		{
			if (nX >= nImageLeft && nY >= nImageTop && nX <= nImageRight && nY <= nImageBottom)
				continue;

			// solid
			if (Type == c_nBorderSolidAll || Type == c_nBorderSolidLeft || Type == c_nBorderSolidTop || Type == c_nBorderSolidRight || Type == c_nBorderSolidBottom || Type == c_nBorderSolidLeftRight || Type == c_nBorderSolidTopBottom)
			{
				m_pixels[nIndex + 0] = nBB;
				m_pixels[nIndex + 1] = nBG;
				m_pixels[nIndex + 2] = nBR;
			}

			// gradients
			if (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightLeft || Type == c_nBorderGradientLightLeftRight || Type == c_nBorderGradientDarkAll || Type == c_nBorderGradientDarkLeft || Type == c_nBorderGradientDarkLeftRight)
			{
				nR3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightLeft || Type == c_nBorderGradientLightLeftRight) ? nBLR : nBDR;
				nG3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightLeft || Type == c_nBorderGradientLightLeftRight) ? nBLG : nBDG;
				nB3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightLeft || Type == c_nBorderGradientLightLeftRight) ? nBLB : nBDB;

				if (nX < nImageLeft)
				{
					nR1 = Byte(nBR + (nR3 - nBR)*nX/(double)Size);
					nG1 = Byte(nBG + (nG3 - nBG)*nX/(double)Size);
					nB1 = Byte(nBB + (nB3 - nBB)*nX/(double)Size);

					if (nY < nImageTop)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*nY/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*nY/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*nY/(double)Size);

						m_pixels[nIndex + 2] = (nX < nY) ? nR1 : nR2;
						m_pixels[nIndex + 1] = (nX < nY) ? nG1 : nG2;
						m_pixels[nIndex + 0] = (nX < nY) ? nB1 : nB2;
					}
					else if (nY > nImageBottom)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*(m_height - 1 - nY)/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*(m_height - 1 - nY)/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*(m_height - 1 - nY)/(double)Size);

						m_pixels[nIndex + 2] = (nX < (m_height - 1 - nY)) ? nR1 : nR2;
						m_pixels[nIndex + 1] = (nX < (m_height - 1 - nY)) ? nG1 : nG2;
						m_pixels[nIndex + 0] = (nX < (m_height - 1 - nY)) ? nB1 : nB2;
					}
					else
					{
						m_pixels[nIndex + 2] = nR1;
						m_pixels[nIndex + 1] = nG1;
						m_pixels[nIndex + 0] = nB1;
					}
				}
			}
			if (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightRight || Type == c_nBorderGradientLightLeftRight || Type == c_nBorderGradientDarkAll || Type == c_nBorderGradientDarkRight || Type == c_nBorderGradientDarkLeftRight)
			{
				nR3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightRight || Type == c_nBorderGradientLightLeftRight) ? nBLR : nBDR;
				nG3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightRight || Type == c_nBorderGradientLightLeftRight) ? nBLG : nBDG;
				nB3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightRight || Type == c_nBorderGradientLightLeftRight) ? nBLB : nBDB;

				if (nX > nImageRight)
				{
					nR1 = Byte(nBR + (nR3 - nBR)*(m_width - 1 - nX)/(double)Size);
					nG1 = Byte(nBG + (nG3 - nBG)*(m_width - 1 - nX)/(double)Size);
					nB1 = Byte(nBB + (nB3 - nBB)*(m_width - 1 - nX)/(double)Size);

					if (nY < nImageTop)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*nY/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*nY/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*nY/(double)Size);

						m_pixels[nIndex + 2] = ((m_width - 1 - nX) < nY) ? nR1 : nR2;
						m_pixels[nIndex + 1] = ((m_width - 1 - nX) < nY) ? nG1 : nG2;
						m_pixels[nIndex + 0] = ((m_width - 1 - nX) < nY) ? nB1 : nB2;
					}
					else if (nY > nImageBottom)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*(m_height - 1 - nY)/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*(m_height - 1 - nY)/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*(m_height - 1 - nY)/(double)Size);

						m_pixels[nIndex + 2] = ((m_width - 1 - nX) < (m_height - 1 - nY)) ? nR1 : nR2;
						m_pixels[nIndex + 1] = ((m_width - 1 - nX) < (m_height - 1 - nY)) ? nG1 : nG2;
						m_pixels[nIndex + 0] = ((m_width - 1 - nX) < (m_height - 1 - nY)) ? nB1 : nB2;
					}
					else
					{
						m_pixels[nIndex + 2] = nR1;
						m_pixels[nIndex + 1] = nG1;
						m_pixels[nIndex + 0] = nB1;
					}
				}
			}
			if (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightBottom || Type == c_nBorderGradientLightTopBottom || Type == c_nBorderGradientDarkAll || Type == c_nBorderGradientDarkBottom || Type == c_nBorderGradientDarkTopBottom)
			{
				nR3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightBottom || Type == c_nBorderGradientLightTopBottom) ? nBLR : nBDR;
				nG3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightBottom || Type == c_nBorderGradientLightTopBottom) ? nBLG : nBDG;
				nB3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightBottom || Type == c_nBorderGradientLightTopBottom) ? nBLB : nBDB;

				if (nY < nImageTop)
				{
					nR1 = Byte(nBR + (nR3 - nBR)*nY/(double)Size);
					nG1 = Byte(nBG + (nG3 - nBG)*nY/(double)Size);
					nB1 = Byte(nBB + (nB3 - nBB)*nY/(double)Size);

					if (nX < nImageLeft)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*nX/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*nX/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*nX/(double)Size);

						m_pixels[nIndex + 2] = (nY < nX) ? nR1 : nR2;
						m_pixels[nIndex + 1] = (nY < nX) ? nG1 : nG2;
						m_pixels[nIndex + 0] = (nY < nX) ? nB1 : nB2;
					}
					else if (nX > nImageRight)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*(m_width - 1 - nX)/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*(m_width - 1 - nX)/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*(m_width - 1 - nX)/(double)Size);

						m_pixels[nIndex + 2] = (nY < (m_width - 1 - nX)) ? nR1 : nR2;
						m_pixels[nIndex + 1] = (nY < (m_width - 1 - nX)) ? nG1 : nG2;
						m_pixels[nIndex + 0] = (nY < (m_width - 1 - nX)) ? nB1 : nB2;
					}
					else
					{
						m_pixels[nIndex + 2] = nR1;
						m_pixels[nIndex + 1] = nG1;
						m_pixels[nIndex + 0] = nB1;
					}
				}
			}
			if (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightTop || Type == c_nBorderGradientLightTopBottom || Type == c_nBorderGradientDarkAll || Type == c_nBorderGradientDarkTop || Type == c_nBorderGradientDarkTopBottom)
			{
				nR3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightTop || Type == c_nBorderGradientLightTopBottom) ? nBLR : nBDR;
				nG3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightTop || Type == c_nBorderGradientLightTopBottom) ? nBLG : nBDG;
				nB3 = (Type == c_nBorderGradientLightAll || Type == c_nBorderGradientLightTop || Type == c_nBorderGradientLightTopBottom) ? nBLB : nBDB;

				if (nY > nImageBottom)
				{
					nR1 = Byte(nBR + (nR3 - nBR)*(m_height - 1 - nY)/(double)Size);
					nG1 = Byte(nBG + (nG3 - nBG)*(m_height - 1 - nY)/(double)Size);
					nB1 = Byte(nBB + (nB3 - nBB)*(m_height - 1 - nY)/(double)Size);

					if (nX < nImageLeft)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*nX/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*nX/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*nX/(double)Size);

						m_pixels[nIndex + 2] = ((m_height - 1 - nY) < nX) ? nR1 : nR2;
						m_pixels[nIndex + 1] = ((m_height - 1 - nY) < nX) ? nG1 : nG2;
						m_pixels[nIndex + 0] = ((m_height - 1 - nY) < nX) ? nB1 : nB2;
					}
					else if (nX > nImageRight)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*(m_width - 1 - nX)/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*(m_width - 1 - nX)/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*(m_width - 1 - nX)/(double)Size);

						m_pixels[nIndex + 2] = ((m_height - 1 - nY) < (m_width - 1 - nX)) ? nR1 : nR2;
						m_pixels[nIndex + 1] = ((m_height - 1 - nY) < (m_width - 1 - nX)) ? nG1 : nG2;
						m_pixels[nIndex + 0] = ((m_height - 1 - nY) < (m_width - 1 - nX)) ? nB1 : nB2;
					}
					else
					{
						m_pixels[nIndex + 2] = nR1;
						m_pixels[nIndex + 1] = nG1;
						m_pixels[nIndex + 0] = nB1;
					}
				}
			}

			// transparency
			if (Type == c_nBorderTransparentAll || Type == c_nBorderTransparentLeft || Type == c_nBorderTransparentLeftRight)
			{
				if (nX < nImageLeft)
				{
					if (nY < nImageTop && nX >= nY)
					{
						if (nY > 0.5*Size)
						{
							dAlpha = (nY - 0.5*Size)/(0.5*Size);
						}
						else
							dAlpha = 0;
					}
					else if (nY > nImageBottom && nX >= (m_height - 1 - nY))
					{
						if ((m_height - 1 - nY) > 0.5*Size)
							dAlpha = (m_height - 1 - nY - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}
					else
					{
						if (nX > 0.5*Size)
							dAlpha = (nX - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}

					m_pixels[nIndex + 0] = (BYTE)(dAlpha*m_pBackup[nIndex + 0] + (1.0 - dAlpha)*nBB);
					m_pixels[nIndex + 1] = (BYTE)(dAlpha*m_pBackup[nIndex + 1] + (1.0 - dAlpha)*nBG);
					m_pixels[nIndex + 2] = (BYTE)(dAlpha*m_pBackup[nIndex + 2] + (1.0 - dAlpha)*nBR);
				}
			}
			if (Type == c_nBorderTransparentAll || Type == c_nBorderTransparentRight || Type == c_nBorderTransparentLeftRight)
			{
				if (nX > nImageRight)
				{
					if (nY < nImageTop && (m_width - 1 - nX) >= nY)
					{
						if (nY > 0.5*Size)
						{
							dAlpha = (nY - 0.5*Size)/(0.5*Size);
						}
						else
							dAlpha = 0;
					}
					else if (nY > nImageBottom && (m_width - 1 - nX) >= (m_height - 1 - nY))
					{
						if ((m_height - 1 - nY) > 0.5*Size)
							dAlpha = (m_height - 1 - nY - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}
					else
					{
						if ((m_width - 1 - nX) > 0.5*Size)
							dAlpha = ((m_width - 1 - nX) - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}

					m_pixels[nIndex + 0] = (BYTE)(dAlpha*m_pBackup[nIndex + 0] + (1.0 - dAlpha)*nBB);
					m_pixels[nIndex + 1] = (BYTE)(dAlpha*m_pBackup[nIndex + 1] + (1.0 - dAlpha)*nBG);
					m_pixels[nIndex + 2] = (BYTE)(dAlpha*m_pBackup[nIndex + 2] + (1.0 - dAlpha)*nBR);
				}
			}
			if (Type == c_nBorderTransparentAll || Type == c_nBorderTransparentTop || Type == c_nBorderTransparentTopBottom)
			{
				if (nY > nImageBottom)
				{
					if (nX < nImageLeft && (m_height - 1 - nY) >= nX)
					{
						if (nX > 0.5*Size)
						{
							dAlpha = (nX - 0.5*Size)/(0.5*Size);
						}
						else
							dAlpha = 0;
					}
					else if (nX > nImageRight && (m_height - 1 - nY) >= (m_width - 1 - nX))
					{
						if ((m_width - 1 - nX) > 0.5*Size)
							dAlpha = (m_width - 1 - nX - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}
					else
					{
						if ((m_height - 1 - nY) > 0.5*Size)
							dAlpha = ((m_height - 1 - nY) - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}

					m_pixels[nIndex + 0] = (BYTE)(dAlpha*m_pBackup[nIndex + 0] + (1.0 - dAlpha)*nBB);
					m_pixels[nIndex + 1] = (BYTE)(dAlpha*m_pBackup[nIndex + 1] + (1.0 - dAlpha)*nBG);
					m_pixels[nIndex + 2] = (BYTE)(dAlpha*m_pBackup[nIndex + 2] + (1.0 - dAlpha)*nBR);
				}
			}
			if (Type == c_nBorderTransparentAll || Type == c_nBorderTransparentBottom || Type == c_nBorderTransparentTopBottom)
			{
				if (nY < nImageTop)
				{
					if (nX < nImageLeft && nY >= nX)
					{
						if (nX > 0.5*Size)
						{
							dAlpha = (nX - 0.5*Size)/(0.5*Size);
						}
						else
							dAlpha = 0;
					}
					else if (nX > nImageRight && nY >= (m_width - 1 - nX))
					{
						if ((m_width - 1 - nX) > 0.5*Size)
							dAlpha = (m_width - 1 - nX - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}
					else
					{
						if (nY > 0.5*Size)
							dAlpha = (nY - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}

					m_pixels[nIndex + 0] = (BYTE)(dAlpha*m_pBackup[nIndex + 0] + (1.0 - dAlpha)*nBB);
					m_pixels[nIndex + 1] = (BYTE)(dAlpha*m_pBackup[nIndex + 1] + (1.0 - dAlpha)*nBG);
					m_pixels[nIndex + 2] = (BYTE)(dAlpha*m_pBackup[nIndex + 2] + (1.0 - dAlpha)*nBR);
				}
			}
		}
	}

	// delete temporary memory
	ClearBackup();

	// all ok
	return S_OK;
}
	
// Draw mov object
STDMETHODIMP ImagePaint3::DrawMovObject(double Frame, long Type, long Divisions, long Color, long Alpha)
{
	// check for valid image
	if (!Valid() || !m_pixels || m_width < 2 || m_height < 2)
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// create temporary memory
	CreateBackup();

	// compose size
	m_nSizeX = m_width/max(1, min(m_width/4, Divisions));
	m_nSizeY = m_height/max(1, min(m_height/4, Divisions));

	// apply effect
	ApplyCompose(Frame, Type, Color, Alpha);

	// delete temporary memory
	ClearBackup();

	// all ok
	return S_OK;
}
		
// Draw credits
STDMETHODIMP ImagePaint3::DrawCredits(double Frame, BSTR Text, long Type)
{
	// check for valid image
	if (!Valid() || !m_pixels || m_width < 2 || m_height < 2)
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// set TextRenderingHintAntiAliasGridFit for graphics
	TextRenderingHint oldTextRenderingHint = m_graphics->GetTextRenderingHint();
	m_graphics->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

	// variables
	Gdiplus::Font* font = m_font.GetFont();

	// compute text size
	PointF start(0, 0);
	RectF layout(0, 0, 0, 0);
	SizeF size; m_graphics->MeasureString(Text, SysStringLen(Text), font, start, &layout);

	// variables
	PointF point;

	// create color
	Gdiplus::Color color((BYTE)m_brush.Alpha1, GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));

	// compute advanced colors and positions
	if (Type == c_nCreditsHMWBottom || Type == c_nCreditsHMWCenter || Type == c_nCreditsHMWTop ||
		Type == c_nCreditsVMWLeft || Type == c_nCreditsVMWCenter || Type == c_nCreditsVMWRight)
	{
		if (Frame < 0.25)
			Frame = 2*Frame;
		else if (Frame > 0.75)
			Frame = 0.5 + 2*(Frame - 0.75);
		else
			Frame = 0.5;
	}
	else if (Type == c_nCreditsHMHBottom || Type == c_nCreditsHMHCenter || Type == c_nCreditsHMHTop ||
			Type == c_nCreditsVMHLeft || Type == c_nCreditsVMHCenter || Type == c_nCreditsVMHRight)
	{
		if (Frame > 0.5)
		{
			if (Frame > 0.75)
				color = Gdiplus::Color((BYTE)(m_brush.Alpha1*(1.0 - Frame)/0.25), GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
			Frame = 0.5;
		}
	}
	else if (Type == c_nCreditsHMSBottom || Type == c_nCreditsHMSCenter || Type == c_nCreditsHMSTop ||
			Type == c_nCreditsVMSLeft || Type == c_nCreditsVMSCenter || Type == c_nCreditsVMSRight)
	{
		Frame *= 0.5;
	}
	else if (Type == c_nCreditsHHMBottom || Type == c_nCreditsHHMCenter || Type == c_nCreditsHHMTop ||
			Type == c_nCreditsVHMLeft || Type == c_nCreditsVHMCenter || Type == c_nCreditsVHMRight)
	{
		if (Frame < 0.5)
		{
			if (Frame < 0.25)
				color = Gdiplus::Color((BYTE)(m_brush.Alpha1*Frame/0.25), GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
			Frame = 0.5;
		}
	}

	// create pen and brush
	SolidBrush brush(color);

	// compute points
	if (Type == c_nCreditsHMMBottom || Type == c_nCreditsHMWBottom || Type == c_nCreditsHMHBottom || Type == c_nCreditsHMSBottom || Type == c_nCreditsHHMBottom)
	{
		point.X = (REAL)((m_width + 1)*(1.0 - Frame) + Frame*(-layout.GetRight() - 1));
		point.Y = (REAL)(m_height - 1 - layout.Height);
	}
	if (Type == c_nCreditsHMMCenter || Type == c_nCreditsHMWCenter || Type == c_nCreditsHMHCenter || Type == c_nCreditsHMSCenter || Type == c_nCreditsHHMCenter)
	{
		point.X = (REAL)((m_width + 1)*(1.0 - Frame) + Frame*(-layout.GetRight() - 1));
		point.Y = (REAL)(0.5*m_height - 0.5*layout.Height);
	}
	if (Type == c_nCreditsHMMTop    || Type == c_nCreditsHMWTop    || Type == c_nCreditsHMHTop    || Type == c_nCreditsHMSTop    || Type == c_nCreditsHHMTop)
	{
		point.X = (REAL)((m_width + 1)*(1.0 - Frame) + Frame*(-layout.GetRight() - 1));
		point.Y = (REAL)1;
	}
	if (Type == c_nCreditsVMMLeft   || Type == c_nCreditsVMWLeft   || Type == c_nCreditsVMHLeft   || Type == c_nCreditsVMSLeft   || Type == c_nCreditsVHMLeft)
	{
		point.X = (REAL)(1);
		point.Y = (REAL)((m_height + 1)*(1.0 - Frame) + Frame*(-layout.GetBottom() - 1));
	}
	if (Type == c_nCreditsVMMCenter || Type == c_nCreditsVMWCenter || Type == c_nCreditsVMHCenter || Type == c_nCreditsVMSCenter || Type == c_nCreditsVHMCenter)
	{
		point.X = (REAL)(0.5*(m_width - 1) - 0.5*layout.Width);
		point.Y = (REAL)((m_height + 1)*(1.0 - Frame) + Frame*(-layout.GetBottom() - 1));
	}
	if (Type == c_nCreditsVMMRight  || Type == c_nCreditsVMWRight  || Type == c_nCreditsVMHRight  || Type == c_nCreditsVMSRight  || Type == c_nCreditsVHMRight)
	{
		point.X = (REAL)(m_width - 1 - layout.Width - 1);
		point.Y = (REAL)((m_height + 1)*(1.0 - Frame) + Frame*(-layout.GetBottom() - 1));
	}
	// draw text string
	m_graphics->DrawString(Text, SysStringLen(Text), font, point, &brush);

	// stop painting
	m_graphics->Flush();

	// restore TextRenderingHint for graphics
	m_graphics->SetTextRenderingHint(oldTextRenderingHint);

	// all ok
	return S_OK;
}
