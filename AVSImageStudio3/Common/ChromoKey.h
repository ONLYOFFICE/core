#pragma once

class CChromoKey
{
public:

	CChromoKey()
	{
		m_bInitKoef = FALSE;
	}
	
	void SetImage(LPBYTE pPixels, int nWidth, int nHeight)
	{
		m_oImage.m_pPixels = pPixels;
		m_oImage.m_nWidth = nWidth;
		m_oImage.m_nHeight = nHeight;
	}
	void DrawToGraphics(Graphics* pGraphics, BOOL bFlipVertical, double dLeft, double dTop, double dWidth = -1, double dHeight = -1)
	{
		if (!pGraphics || !m_oImage.IsValid())
			return;

		IppiSize roiSize;
		roiSize.width = m_oImage.m_nWidth;
		roiSize.height = m_oImage.m_nHeight;

		// flip
		if (bFlipVertical)
			ippiMirror_8u_C4IR(m_oImage.m_pPixels, 4*m_oImage.m_nWidth, roiSize, ippAxsHorizontal);
	
		Bitmap* pBitmap = new Bitmap(m_oImage.m_nWidth, m_oImage.m_nHeight, 4*m_oImage.m_nWidth, PixelFormat32bppARGB, m_oImage.m_pPixels);

		// draw image with chromo
		if (pBitmap)
		{
			if (dWidth < 0)
				dWidth = m_oImage.m_nWidth;
			if (dHeight < 0)
				dHeight = m_oImage.m_nHeight;

			pGraphics->DrawImage(pBitmap, (REAL)dLeft, (REAL)dTop, (REAL)dWidth, (REAL)dHeight);
			pGraphics->Flush();

			delete pBitmap;
		}
	
		// restore flip
		if (bFlipVertical)
			ippiMirror_8u_C4IR(m_oImage.m_pPixels, 4*m_oImage.m_nWidth, roiSize, ippAxsHorizontal);
	}
	void DoChromoKey(double Frame, int nHComponent, int nDelta, BOOL bComposeAlpha = FALSE)
	{
		if (!m_oImage.IsValid())
			return;

		IppiSize roiSize;
		roiSize.width = m_oImage.m_nWidth;
		roiSize.height = m_oImage.m_nHeight;
		if (0 == nDelta)
		{
			ippiSet_8u_C4CR(255, m_oImage.m_pPixels + 3, 4*m_oImage.m_nWidth, roiSize);
			return;
		}

		LPBYTE pHLS = new BYTE[m_oImage.m_nWidth*m_oImage.m_nHeight*4];
		if (NULL == pHLS)
			return;

		FillKoefArray();

		LPBYTE pSrc = m_oImage.m_pPixels;

		ippiBGRToHLS_8u_AC4R(pSrc, 4*m_oImage.m_nWidth, pHLS, 4*m_oImage.m_nWidth, roiSize);

		LPBYTE pRGBPtr = pSrc + 3;
		LPBYTE pHLSPtr = pHLS;
		int nTemp;
		double dLSKoef;

		for (int i = 0; i < m_oImage.m_nHeight; ++i)
		{
			for (int j = 0; j < m_oImage.m_nWidth; ++j, pRGBPtr += 4, pHLSPtr += 4)
			{	
				nTemp = *pHLSPtr - nHComponent;
				if (nTemp > 128)
					nTemp = nTemp - 256;
				else if (nTemp < -128)
					nTemp = nTemp + 256;
				nTemp = abs(nTemp);
				if (nTemp > nDelta)
					SetPixelAlpha(pRGBPtr, 255*Frame, bComposeAlpha);
				else
				{
					dLSKoef = Frame*m_dLKoef[*(pHLSPtr + 1)]*m_dSKoef[*(pHLSPtr + 2)];

					double dTempDelta = (double)nTemp/nDelta;
					
					SetPixelAlpha(pRGBPtr, 192.0*dLSKoef*dTempDelta*dTempDelta, bComposeAlpha);
				}
			}
		}

		delete []pHLS;
	}
	void DoChromoKey(double Frame, int nHComponentFirst, int nDeltaFirst, int nHComponentSecond, int nDeltaSecond, BOOL bComposeAlpha = FALSE)
	{
		if (!m_oImage.IsValid())
			return;

		IppiSize roiSize;
		roiSize.width = m_oImage.m_nWidth;
		roiSize.height = m_oImage.m_nHeight;
		if ((0 == nDeltaFirst) && (0 == nDeltaSecond))
		{
			ippiSet_8u_C4CR(255, m_oImage.m_pPixels + 3, 4*m_oImage.m_nWidth, roiSize);
			return;
		}
		else if ((0 != nDeltaFirst) && (0 == nDeltaSecond))
		{
			DoChromoKey(Frame, nHComponentFirst, nDeltaFirst, bComposeAlpha);
			return;
		}
		else if ((0 == nDeltaFirst) && (0 != nDeltaSecond))
		{
			DoChromoKey(Frame, nHComponentSecond, nDeltaSecond, bComposeAlpha);
			return;
		}

		LPBYTE pHLS = new BYTE[m_oImage.m_nWidth*m_oImage.m_nHeight*4];
		if (NULL == pHLS)
			return;

		FillKoefArray();

		LPBYTE pSrc = m_oImage.m_pPixels;

		ippiBGRToHLS_8u_AC4R(pSrc, 4*m_oImage.m_nWidth, pHLS, 4*m_oImage.m_nWidth, roiSize);

		LPBYTE pRGBPtr = pSrc + 3;
		LPBYTE pHLSPtr = pHLS;
		int nTemp;
		double dLSKoef;

		for (int i = 0; i < m_oImage.m_nHeight; ++i)
		{
			for (int j = 0; j < m_oImage.m_nWidth; ++j, pRGBPtr += 4, pHLSPtr += 4)
			{	
				nTemp = *pHLSPtr - nHComponentFirst;
				if (nTemp>128)
					nTemp = nTemp - 256;
				else if (nTemp < -128)
					nTemp = nTemp + 256;
				nTemp = abs(nTemp);
				if (nTemp > nDeltaFirst)
					SetPixelAlpha(pRGBPtr, 255, bComposeAlpha);
				else
				{
					dLSKoef = m_dLKoef[*(pHLSPtr + 1)]*m_dSKoef[*(pHLSPtr + 2)];

					double dTempDelta = (double)nTemp/nDeltaFirst;
					
					SetPixelAlpha(pRGBPtr, 192.0*dLSKoef*dTempDelta*dTempDelta, bComposeAlpha);
				}

				nTemp = *pHLSPtr - nHComponentSecond;
				if (nTemp > 128)
					nTemp = nTemp - 256;
				else if (nTemp < -128)
					nTemp = nTemp + 256;
				nTemp = abs(nTemp);
				if (nTemp < nDeltaSecond)
				{
					dLSKoef = m_dLKoef[*(pHLSPtr + 1)]*m_dSKoef[*(pHLSPtr + 2)];

					double dTempDelta = (double)nTemp/nDeltaSecond;
					
					SetPixelAlpha(pRGBPtr, *pRGBPtr + 192.0*dLSKoef*dTempDelta*dTempDelta/2, bComposeAlpha);
				}

				SetPixelAlpha(pRGBPtr, (double)*pRGBPtr * Frame, bComposeAlpha);
			}
		}

		delete []pHLS;
	}
	
private:
	
	struct BitmapFrame
	{
		BitmapFrame()
			: m_pPixels(NULL)
			, m_nWidth(0)
			, m_nHeight(0)
		{
		}
		BOOL IsValid()
		{
			return ((NULL!=m_pPixels)&&(0<m_nWidth)&&(0<m_nHeight));
		}

		BOOL GetBitmap(Bitmap *&pBitmap)
		{
			if (!IsValid())
				return FALSE;

			Rect rcBMP;
			rcBMP.X = 0;
			rcBMP.Y = 0;
			rcBMP.Width = m_nWidth;
			rcBMP.Height = m_nHeight;

			IppiSize roiSize;
			roiSize.width = m_nWidth;
			roiSize.height = m_nHeight;

			ippiMirror_8u_C4IR(m_pPixels, 4*m_nWidth, roiSize, ippAxsHorizontal);
			pBitmap = new Bitmap(m_nWidth,m_nHeight,PixelFormat32bppARGB);
			
			BitmapData bitmapData;
			bitmapData.Width = m_nWidth,
			bitmapData.Height = m_nHeight,
			bitmapData.Stride = 4*bitmapData.Width;
			bitmapData.PixelFormat = PixelFormat32bppARGB; 
			bitmapData.Scan0 = m_pPixels;
			bitmapData.Reserved = NULL;
			
			pBitmap->LockBits(&rcBMP, ImageLockModeWrite|ImageLockModeUserInputBuf, PixelFormat32bppARGB, &bitmapData);
			pBitmap->UnlockBits(&bitmapData);

			ippiMirror_8u_C4IR(m_pPixels, 4*m_nWidth, roiSize, ippAxsHorizontal);

			return TRUE;
		}

		LPBYTE m_pPixels;
		int m_nWidth;
		int m_nHeight;
	};
	
	// working image
	BitmapFrame m_oImage;

	// transformation color maps
	double m_dLKoef[255];
	double m_dSKoef[255];

	BOOL m_bInitKoef;

private:
	
	void SetPixelAlpha(BYTE* pAlpha, double dValue, BOOL bComposeWithExistingValue)
	{
		if (bComposeWithExistingValue)
			dValue = dValue*(*pAlpha)/255.0;

		if (dValue < 0)
			*pAlpha = 0;
		else if (dValue > 255)
			*pAlpha = 255;
		else
			*pAlpha = (BYTE)dValue;
	}

	void FillKoefArray()
	{
		int g_cnLMin = 50;
		int g_cnLMax = 206;
		int g_cnSMin = 30;

		if (m_bInitKoef)
			return;

		for (int i = 0; i < g_cnLMin; ++i)
			m_dLKoef[i] = 1.0;
		
		int nLMiddle = (g_cnLMax + g_cnLMin)/2;
		int nLDelta  = g_cnLMax - nLMiddle;

		for (int i = g_cnLMin; i <= g_cnLMax; ++i)
			m_dLKoef[i] = (double)abs(i - nLMiddle)/nLDelta;

		for (int i = g_cnLMax; i < 255; ++i)
			m_dLKoef[i] = 1.0;

		for (int i = 0; i < g_cnSMin; ++i)
			m_dSKoef[i] = 1.0;
		
		int nSDelta  = 255 - g_cnSMin;
		for (int i = g_cnSMin; i < 255; ++i)
			m_dSKoef[i] = (double)(i - g_cnSMin)/nSDelta;

		m_bInitKoef = TRUE;
	}

};