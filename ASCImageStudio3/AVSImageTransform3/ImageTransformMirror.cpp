
#include "stdafx.h"
#include "ImageTransform.h"

BOOL ImageTransform3::MirrorVertical(double Frame, int Type, double Level)
{
	// compute mirror level
	int nVMirror = (int)(0.01*Level*Frame*(m_nHeight - 2));

	// mirror image
	if (0 >= nVMirror || nVMirror >= m_nHeight - 1)
		return TRUE;

	// variables
	int nX, nY, nIndex;
	double dSourceX, dSourceY;
	double dR, dG, dB, dCount;

	// allocate memory
	BYTE* pLine = new BYTE[4*m_nWidth];

	// mirror last pixels
	for (nY = nVMirror; nY >= 0; --nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX)
		{
			// compute pixel coordinates
			dSourceX = nX;
			if (Type == c_nMirrorVerticalBottom)
			{
				nIndex = 4*(nX + nY*m_nWidth);
				dSourceY = nVMirror + (m_nHeight - 1 - nVMirror)*(nVMirror - nY)/(double)nVMirror;
			}
			else if (Type == c_nMirrorVerticalTop)
			{
				nIndex = 4*(nX + (m_nHeight - 1 - nY)*m_nWidth);
				dSourceY = m_nHeight - 1 - nVMirror - (m_nHeight - 1 - nVMirror)*(nVMirror - nY)/(double)nVMirror;
			}
			else
				break;

			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
			{
				pLine[4*nX + 0] = m_pBackup[nIndex + 0];
				pLine[4*nX + 1] = m_pBackup[nIndex + 1];
				pLine[4*nX + 2] = m_pBackup[nIndex + 2];
				pLine[4*nX + 3] = m_pBackup[nIndex + 3];
				continue;
			}

			// compute pixel color
			pLine[4*nX + 0] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 0, 4, 0);
			pLine[4*nX + 1] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 1, 4, 0);
			pLine[4*nX + 2] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 2, 4, 0);
			pLine[4*nX + 3] = m_pBackup[nIndex + 3];
		}

		// apply motion blur in the horizontal direction
		for (nX = 0; nX < m_nWidth; ++nX)
		{
			// compute pixel coordinates
			if (Type == c_nMirrorVerticalBottom)
				nIndex = 4*(nX + nY*m_nWidth);
			else if (Type == c_nMirrorVerticalTop)
				nIndex = 4*(nX + (m_nHeight - 1 - nY)*m_nWidth);
			else
				break;

			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
				continue;

			// compute length of the motion blur
			int nBlurLevel = 10*(nVMirror - nY)/nVMirror;
			int nBlurMin = max(0, nX - nBlurLevel);
			int nBlurMax = min(m_nWidth - 1, nX + nBlurLevel);

			// reset values
			dR = 0;
			dG = 0;
			dB = 0;
			dCount = 0;

			// compute average color
			for (int nX1 = nBlurMin; nX1 <= nBlurMax; ++nX1, dCount += 1)
			{
				dR += pLine[4*nX1 + 2];
				dG += pLine[4*nX1 + 1];
				dB += pLine[4*nX1 + 0];
			}

			// compute pixel color
			m_pPixels[nIndex + 0] = (BYTE)(dB/dCount);
			m_pPixels[nIndex + 1] = (BYTE)(dG/dCount);
			m_pPixels[nIndex + 2] = (BYTE)(dR/dCount);
		}
	}

	// clear memory
	if (pLine)
		delete[] pLine;

	// all ok
	return TRUE;
}

BOOL ImageTransform3::MirrorHorizontal(double Frame, int Type, double Level)
{
	// compute mirror level
	int nHMirror = (int)(0.01*Level*Frame*(m_nWidth - 2));

	// mirror image
	if (0 >= nHMirror || nHMirror >= m_nWidth - 1)
		return TRUE;

	// variables
	int nX, nY, nIndex;
	double dSourceX, dSourceY;
	double dR, dG, dB, dCount;

	// allocate memory
	BYTE* pLine = new BYTE[4*m_nHeight];

	// mirror last pixels
	for (nX = nHMirror; nX >= 0; --nX)
	{
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			// compute pixel coordinates
			dSourceY = nY;
			if (Type == c_nMirrorHorizontalLeft)
			{
				nIndex = 4*(nX + nY*m_nWidth);
				dSourceX = nHMirror + (m_nWidth - 1 - nHMirror)*(nHMirror - nX)/(double)nHMirror;
			}
			else if (Type == c_nMirrorHorizontalRight)
			{
				nIndex = 4*(m_nWidth - 1 - nX + nY*m_nWidth);
				dSourceX = m_nWidth - 1 - nHMirror - (m_nWidth - 1 - nHMirror)*(nHMirror - nX)/(double)nHMirror;
			}
			else
				break;			

			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
			{
				pLine[4*nY + 0] = m_pBackup[nIndex + 0];
				pLine[4*nY + 1] = m_pBackup[nIndex + 1];
				pLine[4*nY + 2] = m_pBackup[nIndex + 2];
				pLine[4*nY + 3] = m_pBackup[nIndex + 3];
				continue;
			}

			// compute pixel color
			pLine[4*nY + 0] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 0, 4, 0);
			pLine[4*nY + 1] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 1, 4, 0);
			pLine[4*nY + 2] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 2, 4, 0);
			pLine[4*nY + 3] = m_pBackup[nIndex + 3];
		}

		// apply motion blur in the vertical direction
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			// compute pixel coordinates
			if (Type == c_nMirrorHorizontalLeft)
				nIndex = 4*(nX + nY*m_nWidth);
			else if (Type == c_nMirrorHorizontalRight)
				nIndex = 4*(m_nWidth - 1 - nX + nY*m_nWidth);
			else
				break;			

			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
				continue;

			// compute length of the motion blur
			int nBlurLevel = 10*(nHMirror - nX)/nHMirror;
			int nBlurMin = max(0, nY - nBlurLevel);
			int nBlurMax = min(m_nHeight - 1, nY + nBlurLevel);

			// reset values
			dR = 0;
			dG = 0;
			dB = 0;
			dCount = 0;

			// compute average color
			for (int nY1 = nBlurMin; nY1 <= nBlurMax; ++nY1, dCount += 1)
			{
				dR += pLine[4*nY1 + 2];
				dG += pLine[4*nY1 + 1];
				dB += pLine[4*nY1 + 0];
			}

			// compute pixel color
			m_pPixels[nIndex + 0] = (BYTE)(dB/dCount);
			m_pPixels[nIndex + 1] = (BYTE)(dG/dCount);
			m_pPixels[nIndex + 2] = (BYTE)(dR/dCount);
		}
	}

	// clear memory
	if (pLine)
		delete[] pLine;

	// all ok
	return TRUE;
}


