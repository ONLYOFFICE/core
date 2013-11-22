
#include "stdafx.h"
#include "ImageTransform.h"
#include "ImageTransformIPP.h"
#include <ChromoKey.h>
	
BOOL ImageTransform3::CompactImage(SAFEARRAY** Array, BYTE* data, int width, int height)
{
	// check for valid input parameters
	if (!Array || !data || width < 2 || height < 2)
		return FALSE;

	// compute SAFEARRAY bounds
	SAFEARRAYBOUND rgsaBound[3];
		rgsaBound[0].lLbound = 0;
		rgsaBound[0].cElements = 4;
		rgsaBound[1].lLbound = 0;
		rgsaBound[1].cElements = width;
		rgsaBound[2].lLbound = 0;
		rgsaBound[2].cElements = height;

	// compose SAFEARRAY with image data
	*Array = SafeArrayCreate(VT_UI1, 3, rgsaBound);

	// this is the simpliest and mostly common case
	m_oMemoryUtils.memcpy((BYTE*)(*Array)->pvData, data, 4*width*height);

	// all ok
	return TRUE;
}

BOOL ImageTransform3::ExtractImage(SAFEARRAY* Array, BYTE*& data, int& width, int& height)
{
	// check for valid arrey
	if (!Array)
		return FALSE;

	// variables
	VARTYPE type;
	long lBoundC, uBoundC;
	long lBoundW, uBoundW;
	long lBoundH, uBoundH;

	// check for valid pixel array size
	if (SafeArrayGetDim(Array) != 3)
		return FALSE;

	// check for valid array element type
	if (FAILED(SafeArrayGetVartype(Array, &type)) || type != VT_UI1 || SafeArrayGetElemsize(Array) != 1)
		return FALSE;

	// retrieve image attributes
	if (FAILED(SafeArrayGetLBound(Array, 1, &lBoundC)) || FAILED(SafeArrayGetUBound(Array, 1, &uBoundC)) ||
		FAILED(SafeArrayGetLBound(Array, 2, &lBoundW)) || FAILED(SafeArrayGetUBound(Array, 2, &uBoundW)) ||
		FAILED(SafeArrayGetLBound(Array, 3, &lBoundH)) || FAILED(SafeArrayGetUBound(Array, 3, &uBoundH)))
		return FALSE;

	// check for valid number of channels
	if (1 + uBoundC - lBoundC != 4)
		return FALSE;

	// retrieve image data
	width = 1 + uBoundW - lBoundW;
	height = 1 + uBoundH - lBoundH;
	data = (BYTE*)(Array->pvData);

	// check for valid bitmap
	if (!data || width < 2 || height < 2)
		return FALSE;

	// all ok
	return TRUE;
}

BOOL ImageTransform3::ExtractImage2(BYTE* Array, BYTE*& data, int& width, int& height)
{
	// extract image data
	return ExtractImage((SAFEARRAY*)Array, data, width, height);
}

BOOL ImageTransform3::ApplyTable(BYTE* Table)
{
	// check for valid table
	if (!Table)
		return FALSE;

	// variables
	BYTE* pPixels = m_pPixels + 0;
	int nCount = m_nSize/4;

	// apply table
	if (!m_bMask)
	{
		for (int nIndex = 0; nIndex < nCount; ++nIndex)
		{
			*pPixels = Table[*pPixels]; pPixels++;
			*pPixels = Table[*pPixels]; pPixels++;
			*pPixels = Table[*pPixels]; pPixels++;
			pPixels ++;
		}
	}
	else
	{
		for (int nIndex = 0; nIndex < nCount; ++nIndex)
		{
			//if (*(pPixels+3))
			//	continue;

			*pPixels = Table[*pPixels]; pPixels++;
			*pPixels = Table[*pPixels]; pPixels++;
			*pPixels = Table[*pPixels]; pPixels++;
			pPixels ++;
		}
	}	

	// all ok
	return TRUE;
}

BOOL ImageTransform3::ApplyTable(BYTE* TableR, BYTE* TableG, BYTE* TableB)
{
	// check for valid table
	if (!TableR || !TableG || !TableB)
		return FALSE;

	// variables
	BYTE* pPixels = m_pPixels + 0;
	int nCount = m_nSize/4;

	// apply table
	if (!m_bMask)
	{
		for (int nIndex = 0; nIndex < nCount; ++nIndex)
		{
			*pPixels = TableB[*pPixels]; pPixels++;
			*pPixels = TableG[*pPixels]; pPixels++;
			*pPixels = TableR[*pPixels]; pPixels++;
			pPixels ++;
		}
	}
	else
	{
		for (int nIndex = 0; nIndex < nCount; ++nIndex)
		{
			//if (*(pPixels+3))
			//	continue;

			*pPixels = TableB[*pPixels]; pPixels++;
			*pPixels = TableG[*pPixels]; pPixels++;
			*pPixels = TableR[*pPixels]; pPixels++;
			pPixels ++;
		}
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::ApplyFlip(BYTE* pPixels, int nWidth, int nHeight)
{
	// variables
	int nIndex = 0;
	int nLineSize = 4*nWidth;
	int nIndexFlip = nLineSize*(nHeight - 1);
	BYTE* pLine = new BYTE[nLineSize];

	// flip image vertically
	for (int nY = 0; nY < nHeight/2; ++nY, nIndex += nLineSize, nIndexFlip -= nLineSize)
	{
		m_oMemoryUtils.memcpy(pLine, (BYTE*)(pPixels + nIndex), nLineSize);
		m_oMemoryUtils.memcpy((BYTE*)(pPixels + nIndex), (BYTE*)(pPixels + nIndexFlip), nLineSize);
		m_oMemoryUtils.memcpy((BYTE*)(pPixels + nIndexFlip), pLine, nLineSize);
	}

	// clear memory
	delete[] pLine;

	// all ok
	return TRUE;
}

BOOL ImageTransform3::ApplyClear(BYTE* pPixels, int nSize)
{
	// clear 4th channel with zeroes
	for (int nIndex = 0; nIndex < nSize; nIndex += 4)
		pPixels[nIndex + 3] = 0;

	// al ok
	return TRUE;
}

BOOL ImageTransform3::ApplyColor(BYTE* pPixels, int nSize, int nColor, int nAlpha)
{
	// variables
	double dAlpha1 = nAlpha/255.0;
	double dAlpha2 = 1.0 - dAlpha1;
	double dRed = dAlpha1*GetRValue(nColor);
	double dGreen = dAlpha1*GetGValue(nColor);
	double dBlue = dAlpha1*GetBValue(nColor);

	// apply coloring
	for (int nIndex = 0; nIndex < nSize; nIndex += 4)
	{
		pPixels[nIndex + 2] = (BYTE)(dAlpha2*pPixels[nIndex + 2] + dRed);
		pPixels[nIndex + 1] = (BYTE)(dAlpha2*pPixels[nIndex + 1] + dGreen);
		pPixels[nIndex + 0] = (BYTE)(dAlpha2*pPixels[nIndex + 0] + dBlue);
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::ConvertToHls(float fRed, float fGreen, float fBlue, float& fHue, float& fLightness, float& fSaturation)
{
	// variables
	float fMinValue, fMaxValue, fDeltaValue;
	BYTE nMaxComponent = 0; // red

	// compute minimal and maximal components values
	if (fRed > fGreen)
	{
		if (fRed > fBlue)	{ fMaxValue = fRed; nMaxComponent = 0; }
		else				{ fMaxValue = fBlue; nMaxComponent = 2; }

		if (fGreen < fBlue)	{ fMinValue = fGreen; }
		else				{ fMinValue = fBlue; }
	}
	else
	{
		if (fGreen > fBlue) { fMaxValue = fGreen; nMaxComponent = 1; }
		else				{ fMaxValue = fBlue; nMaxComponent = 2; }

		if (fRed < fBlue)	{ fMinValue = fRed; }
		else				{ fMinValue = fBlue; }
	}

	// compute lightness value
	fLightness = 0.5f*(fMaxValue + fMinValue);

	// compute additional useful parameter
	fDeltaValue = (fMaxValue - fMinValue);

	// check for achromatic case
	if (fabs(fDeltaValue) < 0.00001)
	{
		fSaturation = 0.0f;
		fHue = 0.0f;
	}
	else
	{
		// compute saturation value
		if (fLightness < 0.5f)			fSaturation = fDeltaValue / (fMaxValue + fMinValue);
		else							fSaturation = fDeltaValue / (2.0f - fMaxValue - fMinValue);

		// compute hue angle
		if (nMaxComponent == 0)			fHue = (fGreen - fBlue) / fDeltaValue;
		else if (nMaxComponent == 1)	fHue = 2.0f + (fBlue - fRed) / fDeltaValue;
		else							fHue = 4.0f + (fRed - fGreen) / fDeltaValue;

		fHue *= 60.0f;

		// clamp hue angle to 0..360 range
		if (fHue < 0.0f)	fHue += 360.0f;
		if (fHue > 360.0f)	fHue -= 360.0f;
	}

	// all ok
	return TRUE;
}

BOOL ImageTransform3::ConvertToValue(float fValue1, float fValue2, float fHue, float& fValue)
{
	// clamp hue angle to 0..360 range
	if (fHue > 360.0f)		fHue -= 360.0f;
	else if (fHue < 0.0f)	fHue += 360.0f;

	// compute resulting value using hue angle
	if (fHue < 60.0f)			fValue = (fValue1 + (fValue2 - fValue1) * (fHue / 60.0f));
	else if (fHue < 180.0f)		fValue = (fValue2);
	else if (fHue < 240.0f)		fValue = (fValue1 + (fValue2 - fValue1) * (240.0f - fHue) / 60.0f);
	else						fValue = (fValue1);

	// all ok
	return TRUE;
}

BOOL ImageTransform3::ConvertToRgb(float fHue, float fLightness, float fSaturation, float& fRed, float& fGreen, float& fBlue)
{
	// variables
	float fValue1, fValue2;

	// check achromatic case
	if (fabs(fSaturation) < 0.00001)
	{
		fRed	= fLightness;
		fGreen	= fLightness;
		fBlue	= fLightness;
	}
	else
	{
		// compute additional values
		if (fLightness < 0.5f)
			fValue2 = fLightness * (1.0f + fSaturation);
		else
			fValue2 = fLightness + fSaturation - (fLightness * fSaturation);

		fValue1 = (fLightness / 0.5f) - fValue2;

		// compute RGB components
		ConvertToValue(fValue1, fValue2, fHue + 120.0f, fRed);
		ConvertToValue(fValue1, fValue2, fHue, fGreen);
		ConvertToValue(fValue1, fValue2, fHue - 120.0f, fBlue);
	}

	// all ok
	return TRUE;
}

BYTE ImageTransform3::GetPointNearest(double dPointX, double dPointY, BYTE* pData, int nWidth, int nHeight, int nChannel, int nChannels, BYTE nColor)
{
	int nX = (int)floor(dPointX);
	int nY = (int)floor(dPointY);

	if (nX < 0 || nX >= nWidth || nY < 0 || nY >= nHeight)
		return nColor;

	return pData[nChannel + nChannels*(nX + nY*nWidth)];
}

BYTE ImageTransform3::GetPointBillinear(double dPointX, double dPointY, BYTE* pData, int nWidth, int nHeight, int nChannel, int nChannels, BYTE nColor)
{
	int nX = (int)floor(dPointX);
	int nY = (int)floor(dPointY);

	if (nX == nWidth - 1)	{ dPointX -= 1; nX -= 1; }
	if (nY == nHeight - 1)	{ dPointY -= 1; nY -= 1; }

	if (nX < 0 || nX + 1 >= nWidth || nY < 0 || nY + 1 >= nHeight)
		return nColor;

	double dX = dPointX - nX;
	double dY = dPointY - nY;

	int nIndex = nChannel + nChannels*(nX + nY*nWidth);

	BYTE byLT = pData[nIndex];
	BYTE byRT = pData[nIndex + nChannels];
	BYTE byRB = pData[nIndex + nChannels + nChannels*nWidth];
	BYTE byLB = pData[nIndex + nChannels*nWidth];

	return (BYTE)
	(
		(1 - dX)*(1 - dY)*byLT + // Left - Top corner
		(    dX)*(1 - dY)*byRT + // Right - Top corner
		(    dX)*(    dY)*byRB + // Right - Bottom corner
		(1 - dX)*(    dY)*byLB   // Left - Bottom corner
	);
}

void ImageTransform3::GetRadiusAndAngle(double x, double y, double cx, double cy, double& r, double& a)
{
	double dx = x - cx;
	double dy = y - cy;
	r = _hypot(dx, dy);

	double sdy = (dy >= 0 ? 1.0 : -1.0);
	a = (r ? acos(dx/r)*sdy: 0);
}

void ImageTransform3::ClearImage(BYTE* pData, int nSize)
{
	IPPClearImage(pData, nSize, m_nBackColor);

//#ifdef NOT_OPTIM
//
//	int* p = (int*)pData;
//	int c = RGB(GetBValue(m_nBackColor), GetGValue(m_nBackColor), GetRValue(m_nBackColor));
//	for (int i = 0; i < nSize; i += 4, ++p)
//		*p = c;
//
//#else
//
//    Ipp8u backColor[4] = { GetBValue(m_nBackColor), GetGValue(m_nBackColor), GetRValue(m_nBackColor), GetAValue(m_nBackColor) };
//
//	ippiSet_8u_AC4R(backColor, (Ipp8u*)pData, m_nIppiStep, m_IppiRoiSize);
//
//#endif
}
		
STDMETHODIMP ImageTransform3::AdjustBrightness(double Frame, long Level)
{
	// check for no-action effect
	if (!Level)
		return S_OK;

	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp value
	Frame = Clamp(Frame);

	// compute brightness level
	int nLevel = min(255, max(-255, Level));

// non-optimized variant
#ifdef NOT_OPTIM

	// variables
	BYTE Table[256];

	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
		Table[nIndex] = Byte(nIndex + Frame*nLevel);

	// apply table
	if (!ApplyTable(Table))
		return S_FALSE;

#else

	Ipp8u value[3];
	value[0] = (Ipp8u)(Frame*abs(nLevel));
	value[1] = value[0];
	value[2] = value[0];
	

	IppiSize roiSize;
	roiSize.width	= m_nWidth;
	roiSize.height	= m_nHeight;
	if (nLevel>0)
		ippiAddC_8u_AC4IRSfs(value, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);
	else
		ippiSubC_8u_AC4IRSfs(value, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);

#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::AdjustBrightnessEx(double Frame, long LevelR, long LevelG, long LevelB)
{
	// check for no-action effect
	if (!LevelR && !LevelG && !LevelB)
		return S_OK;

	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp value
	Frame = Clamp(Frame);

	// compute brightness levels
	int nLevelR = min(255, max(-255, LevelR));
	int nLevelG = min(255, max(-255, LevelG));
	int nLevelB = min(255, max(-255, LevelB));

// non-optimized variant
// #ifdef NOT_OPTIM

	// variables
	BYTE TableR[256];
	BYTE TableG[256];
	BYTE TableB[256];

	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
	{
		TableR[nIndex] = Byte(nIndex + Frame*nLevelR);
		TableG[nIndex] = Byte(nIndex + Frame*nLevelG);
		TableB[nIndex] = Byte(nIndex + Frame*nLevelB);
	}

	// apply table
	if (!ApplyTable(TableR, TableG, TableB))
		return S_FALSE;

/*#else

	IppiSize roiSize;
	roiSize.width	= m_nWidth;
	roiSize.height	= m_nHeight;

	if ((nLevelR >= 0) && (nLevelG >= 0) && (nLevelB >= 0))
	{
		Ipp8u value[3];
		value[0] = (Ipp8u)(Frame*abs(nLevelB));
		value[1] = (Ipp8u)(Frame*abs(nLevelG));
		value[2] = (Ipp8u)(Frame*abs(nLevelR));
		ippiAddC_8u_AC4IRSfs(value, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);
	}
	else if ((nLevelR <= 0) && (nLevelG <= 0) && (nLevelB <= 0))
	{
		Ipp8u value[3];
		value[0] = (Ipp8u)(Frame*abs(nLevelB));
		value[1] = (Ipp8u)(Frame*abs(nLevelG));
		value[2] = (Ipp8u)(Frame*abs(nLevelR));
		ippiSubC_8u_AC4IRSfs(value, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);
	}
	else
	{
		Ipp8u valueR[3];
		valueR[0] = 0;
		valueR[1] = 0;
		valueR[2] = (Ipp8u)(Frame*abs(nLevelR));
		Ipp8u valueG[3];
		valueG[0] = 0;
		valueG[1] = (Ipp8u)(Frame*abs(nLevelG));
		valueG[2] = 0;
		Ipp8u valueB[3];
		valueB[0] = (Ipp8u)(Frame*abs(nLevelB));
		valueB[1] = 0;
		valueB[2] = 0;
		
		if (nLevelR > 0)
			ippiAddC_8u_AC4IRSfs(valueR, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);
		else if (nLevelR < 0)
			ippiSubC_8u_AC4IRSfs(valueR, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);
		if (nLevelG > 0)
			ippiAddC_8u_AC4IRSfs(valueG, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);
		else if (nLevelG < 0)
			ippiSubC_8u_AC4IRSfs(valueG, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);
		if (nLevelB > 0)
			ippiAddC_8u_AC4IRSfs(valueB, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);
		else if (nLevelB < 0)
			ippiSubC_8u_AC4IRSfs(valueB, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, 0);
		
		//вариант с расширением до 16s еще хуже
		
		//Ipp16s value[3];
		//value[0] = Frame*nLevelB;
		//value[1] = Frame*nLevelG;
		//value[2] = Frame*nLevelR;
		//Ipp16s* pDst = new Ipp16s[m_nSize];
		//ippiConvert_8u16s_AC4R( (Ipp8u*) m_pPixels, 4*m_nWidth, 
		//						pDst, 4*sizeof(Ipp16s)*m_nWidth, roiSize);	
		//ippiAddC_16s_AC4IRSfs(value, pDst, 4*sizeof(Ipp16s)*m_nWidth, roiSize, 0);
		//ippiConvert_16s8u_AC4R(	pDst, 4*sizeof(Ipp16s)*m_nWidth, 
		//						(Ipp8u*) m_pPixels, 4*m_nWidth, roiSize);	
		//delete pDst;
	}

#endif */

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

LRESULT ImageTransform3::GetAutoBrightness(double Completeness, double* Level)
{
	float fRed, fGreen, fBlue;
	float fHue, fLightness, fSaturation;

	int   cnts=0;
	*Level = 0.0;

	// balance each pixel
	for (int nIndex = 0; nIndex < m_nSize; nIndex += 4)
	{
		// check for pixel mask
		if (m_bMask && m_pPixels[nIndex + 3] != 0)
			continue;

		// retrieve pixel color components
		fRed	= m_pPixels[nIndex + 2]/255.0f;
		fGreen	= m_pPixels[nIndex + 1]/255.0f;
		fBlue	= m_pPixels[nIndex + 0]/255.0f;

		// original color -> Hue-Saturation-Lighteness
		ConvertToHls(fRed, fGreen, fBlue, fHue, fLightness, fSaturation);

		*Level += fLightness;
		cnts++;
	}

	if (cnts>0)
		*Level /= cnts;

	if (*Level>0.5)
		*Level *= -100.0;
	else
		*Level *= 100.0;

	return S_OK;

}

STDMETHODIMP ImageTransform3::AdjustContrast(double Frame, double Level)
{
	// check for no-action effect
	if (fabs(Level - 1.0) < 0.001)
		return S_OK;

	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp value
	Frame = Clamp(Frame);

	// variables
	BYTE Table[256];

	// correct contrast level value
	double dLevel = min(100, max(-100, Level));

	// compute contrast level
	if (Level <= 0)
		dLevel = 1.0 + Frame*dLevel/100.0;
	else
		dLevel = pow(255, Frame*dLevel/100.0);
	
	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
		Table[nIndex] = Byte(127.5 + (nIndex - 127.5)*dLevel);

	// apply table
	if (!ApplyTable(Table))
		return S_FALSE;

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::AdjustGamma(double Frame, double Level)
{
	// check for no-action effect
	if (fabs(Level - 1.0) < 0.001)
		return S_OK;

	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp value
	Frame = Clamp(Frame);

	// variables
	BYTE Table[256];

	// correct contrast level value
	double dLevel = min(100, max(-100, Level));

	// compute gamma level
	if (Level <= 0)
	{
		// compute gamma values
		double dPower = 1.0/max(0.0001, 1.0 + Frame*dLevel/100.0);
		double dNumerator = 255.0/pow(255.0, dPower);

		// compose table
		for (int nIndex = 0; nIndex < 256; ++nIndex)
			Table[nIndex] = Byte(dNumerator*pow(nIndex, dPower));
	}
	else
	{
		// compute gamma values
		double dPower = 1.0/pow(255, Frame*dLevel/100.0);
		double dNumerator = 255.0/pow(255.0, dPower);

		// compose table
		for (int nIndex = 0; nIndex < 256; ++nIndex)
			Table[nIndex] = Byte(dNumerator*pow(nIndex, dPower));
	}

	// apply table
	if (!ApplyTable(Table))
		return S_FALSE;

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

LRESULT ImageTransform3::GetAutoGamma(double Completeness, double* Level)
{
	// variables
	BYTE Value;
	BYTE ValMax = 0;
	BYTE ValMin = 255;

	for (int nIndex = 0; nIndex < m_nSize; nIndex += 4)
	{
		Value = Intensity(m_pPixels[nIndex + 2], m_pPixels[nIndex + 1], m_pPixels[nIndex + 0]);
		if (ValMax<Value)
			ValMax = Value;
		if (ValMin>Value)
			ValMin = Value;
	}
	if (ValMax>240)
		ValMax = 240;
	if (ValMin<15)
		ValMin = 15;
	double midVal = (double)(ValMax+ValMin)/2.0;
	//*Level = (128.0-midVal)/128.0*50.0;
	*Level = ((double)(ValMax+ValMin))*10.0/2.0/255.0;
	return S_OK;
}
STDMETHODIMP ImageTransform3::AdjustEqualize(double Frame)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

// non-optimized variant
#ifdef NOT_OPTIM

	// variables
	BYTE Table[256];

	// compute equalize level
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;
	double dNumerator = 1.0/127.5;

	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
		Table[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*Byte(dNumerator*nIndex*nIndex));

	// apply table
	if (!ApplyTable(Table))
		return S_FALSE;

#else

	IppiSize roiSize;
	roiSize.width = m_nWidth;
	roiSize.height = m_nHeight;
	Ipp8u* pBuffer = new Ipp8u[m_nSize];
	Ipp8u* pCopy = new Ipp8u[m_nSize];
	Ipp8u alpha = (Ipp8u)(Frame * 255);

	if (pBuffer && pCopy)
	{
		// int scaleFactor = -7;//dNumerator = 1/128 = 2^(-7)
		ippiSqr_8u_AC4RSfs((Ipp8u*) m_pPixels, 4*m_nWidth, pBuffer, 4*m_nWidth, roiSize, 7);

		// складываем вариант из pBuffer с исходной картинкой 
		// учитывая весовой коэффициент (см. определение ippAlphaOver) 				
		// и помещаем результат в m_pPixels
		
		m_oMemoryUtils.memcpy(pCopy, m_pPixels, m_nSize);
		ippiAlphaCompC_8u_AC4R(pBuffer, 4*m_nWidth, alpha, pCopy, 4*m_nWidth, 255, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, ippAlphaOver);
	}
	
	if (pCopy)
		delete[] pCopy;
	if (pBuffer)
		delete[] pBuffer;

#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::AdjustGrayscale(double Frame, VARIANT_BOOL Desaturate)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp value
	Frame = Clamp(Frame);

// non-optimized variant
#ifdef NOT_OPTIM

	// correct grayscaling values
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// variables
	BYTE Value;

	// apply table
	
	//for (int nIndex = 0; nIndex < m_nSize; nIndex += 4)
	//{
	//	if (m_bMask && m_pPixels[nIndex + 3] != 0)
	//		continue;

	//	// compute new color
	//	if (Desaturate == VARIANT_TRUE)
	//		Value = Lightness(m_pPixels[nIndex + 2], m_pPixels[nIndex + 1], m_pPixels[nIndex + 0]);
	//	else
	//		Value = Intensity(m_pPixels[nIndex + 2], m_pPixels[nIndex + 1], m_pPixels[nIndex + 0]);

	//	// compute new color
	//	m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pPixels[nIndex + 0] + dAlpha1*Value);
	//	m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pPixels[nIndex + 1] + dAlpha1*Value);
	//	m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pPixels[nIndex + 2] + dAlpha1*Value);
	//}

	// variables
	BYTE* pPixels = m_pPixels;
	int nCount = m_nSize/4;

	// compute new color
	if (Desaturate == VARIANT_TRUE)
	{
		// apply table
		for (int nIndex = 0; nIndex < nCount; ++nIndex)
		{
			if (m_bMask && *(pPixels + 3) != 0)
				continue;

			Value = Lightness(*(pPixels + 2), *(pPixels + 1), *pPixels);

			// compute new color
			*pPixels = (BYTE)(dAlpha2*(*pPixels) + dAlpha1*Value); pPixels++;
			*pPixels = (BYTE)(dAlpha2*(*pPixels) + dAlpha1*Value); pPixels++;
			*pPixels = (BYTE)(dAlpha2*(*pPixels) + dAlpha1*Value); pPixels++;
			pPixels++;
		}
	}
	else
	{
		// apply table
		for (int nIndex = 0; nIndex < nCount; nIndex ++)
		{
			if (m_bMask && *(pPixels + 3) != 0)
				continue;

			Value = Intensity(*(pPixels + 2), *(pPixels + 1), *pPixels);

			// compute new color
			*pPixels = (BYTE)(dAlpha2*(*pPixels) + dAlpha1*Value); pPixels++;
			*pPixels = (BYTE)(dAlpha2*(*pPixels) + dAlpha1*Value); pPixels++;
			*pPixels = (BYTE)(dAlpha2*(*pPixels) + dAlpha1*Value); pPixels++;
			pPixels++;
		}
	}

#else

//Lightness = (max(R,G,B) + min(R,G,B))/2
//Intensity = 0.3*R + 0.59*G + 0.11*B

	IppiSize roiSize;
	roiSize.width	= m_nWidth;
	roiSize.height	= m_nHeight;

	if (Desaturate == VARIANT_TRUE)
	{
		// variables
		Ipp8u* pBuffer = new Ipp8u[m_nSize];
		Ipp8u* pLightness = new Ipp8u[m_nWidth*m_nHeight];
		Ipp8u* pSrc[4];
		Ipp8u alpha = (Ipp8u)((1.0 - Frame) * 255);

		if (pBuffer && pLightness)
		{
			ippiBGRToHLS_8u_AC4R((Ipp8u*)m_pPixels, 4*m_nWidth, pBuffer, 4*m_nWidth, roiSize);		
			ippiCopy_8u_C4C1R(pBuffer+1, 4*m_nWidth, pLightness, m_nWidth, roiSize);

			pSrc[0] = pLightness;
			pSrc[1] = pLightness;
			pSrc[2] = pLightness;
			pSrc[3] = pLightness;

			ippiCopy_8u_P4C4R(pSrc, m_nWidth, pBuffer, 4*m_nWidth, roiSize);

			ippiAlphaCompC_8u_AC4R((Ipp8u*)m_pPixels, 4*m_nWidth, alpha, pBuffer, 4*m_nWidth, 255, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, ippAlphaOver);
		}

		if (pBuffer)
			delete[] pBuffer;
		if (pLightness)
			delete[] pLightness;
	}
	else
	{
		// variables
		Ipp8u* pIntensity = new Ipp8u[m_nWidth*m_nHeight];
		Ipp8u* pBuffer = new Ipp8u[m_nSize];
		Ipp32f coeffs[3];
		Ipp8u* pSrc[4];
		Ipp8u alpha = (Ipp8u)((1.0 - Frame) * 255);

		if (pBuffer && pIntensity)
		{
			coeffs[0] = 0.11f;
			coeffs[1] = 0.59f;
			coeffs[2] = 0.3f;

			ippiColorToGray_8u_AC4C1R((Ipp8u*) m_pPixels , 4*m_nWidth, pIntensity, m_nWidth, roiSize, coeffs);

			pSrc[0] = pIntensity;
			pSrc[1] = pIntensity;
			pSrc[2] = pIntensity;
			pSrc[3] = pIntensity;

			ippiCopy_8u_P4C4R(pSrc, m_nWidth, pBuffer, 4*m_nWidth, roiSize);

			ippiAlphaCompC_8u_AC4R((Ipp8u*)m_pPixels, 4*m_nWidth, alpha, pBuffer, 4*m_nWidth, 255, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, ippAlphaOver);
		}

		if (pBuffer)
			delete[] pBuffer;
		if (pIntensity)
			delete[] pIntensity;
	}

#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::AdjustInvert(double Frame)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp value
	Frame = Clamp(Frame);

	// variables
	BYTE Table[256];

	// compute equalize level
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
		Table[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*(255 - nIndex));

	// apply table
	if (!ApplyTable(Table))
		return S_FALSE;

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::AdjustAutocontrast(double Frame)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	//// clamp value
	//Frame = Clamp(Frame);

	//// variables
	//int nIndex;
	//int xHistogram[256]; ZeroMemory(xHistogram, 256*sizeof(int));

	//// compute total histogram
	//for (nIndex = 0; nIndex < m_nSize; nIndex += 4)
	//{
	//	xHistogram[m_pPixels[nIndex + 0]]++;
	//	xHistogram[m_pPixels[nIndex + 1]]++;
	//	xHistogram[m_pPixels[nIndex + 2]]++;
	//}

	//// variables
	//int nContrastRange, nContrastLevel = MulDiv(m_nSize, 5, 1000);
	//int nContrastSum, nContrastMin, nContrastMax;

	//// compute minimal scaling value
	//nContrastMin = 0;
	//nContrastSum = 0;
	//for (nIndex = 0; nIndex < 256; ++nIndex)
	//{
	//	if (nContrastSum >= nContrastLevel)
	//	{
	//		nContrastMin = nIndex;
	//		break;
	//	}


	//	nContrastSum += xHistogram[nIndex];
	//}

	//// compute maximal scaling value
	//nContrastMax = 0;
	//nContrastSum = 0;
	//for (nIndex = 255; nIndex >= 0; --nIndex)
	//{
	//	if (nContrastSum >= nContrastLevel)
	//	{
	//		nContrastMax = nIndex;
	//		break;
	//	}

	//	nContrastSum += xHistogram[nIndex];
	//}

	//// clamp ranges values
	//nContrastMin = min(nContrastMin, 254);
	//nContrastMax = max(nContrastMax, nContrastMin + 1);
	//nContrastRange = nContrastMax - nContrastMin;

	//// variables
	//BYTE Table[256];

	//// compute koefficients
	//double dAlpha1 = Frame;
	//double dAlpha2 = 1.0 - Frame;

	//// compose table
	//for (int nIndex = 0; nIndex < 256; ++nIndex)
	//	Table[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*Byte(255.0*(nIndex - nContrastMin)/(double)nContrastRange));

	//// apply table
	//if (!ApplyTable(Table))
	//	return S_FALSE;

	double AutoContr;
	GetAutoContrast(Frame,&AutoContr);
	AdjustContrast(Frame,AutoContr);

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

LRESULT ImageTransform3::GetAutoContrast(double Frame, double* Level)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp value
	Frame = Clamp(Frame);

	// variables
	int nIndex;
	int xHistogram[256]; ZeroMemory(xHistogram, 256*sizeof(int));

	// compute total histogram
	for (nIndex = 0; nIndex < m_nSize; nIndex += 4)
	{
		xHistogram[m_pPixels[nIndex + 0]]++;
		xHistogram[m_pPixels[nIndex + 1]]++;
		xHistogram[m_pPixels[nIndex + 2]]++;
	}

	// variables
	int nContrastRange, nContrastLevel = MulDiv(m_nSize, 5, 1000);
	int nContrastSum, nContrastMin, nContrastMax;

	// compute minimal scaling value
	nContrastMin = 0;
	nContrastSum = 0;
	for (nIndex = 0; nIndex < 256; ++nIndex)
	{
		if (nContrastSum >= nContrastLevel)
		{
			nContrastMin = nIndex;
			break;
		}


		nContrastSum += xHistogram[nIndex];
	}

	// compute maximal scaling value
	nContrastMax = 0;
	nContrastSum = 0;
	for (nIndex = 255; nIndex >= 0; --nIndex)
	{
		if (nContrastSum >= nContrastLevel)
		{
			nContrastMax = nIndex;
			break;
		}

		nContrastSum += xHistogram[nIndex];
	}

	// clamp ranges values
	nContrastMin = min(nContrastMin, 254);
	nContrastMax = max(nContrastMax, nContrastMin + 1);
	nContrastRange = nContrastMax - nContrastMin;

	if (fabs(Frame)<0.000001)
		Frame = 1.0;

	*Level = ((double)nContrastRange)/(25.50);

	// all ok
	return S_OK;

}

STDMETHODIMP ImageTransform3::AdjustAutolevels(double Frame)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	//// clamp value
	//Frame = Clamp(Frame);

	//// variables
	//int nIndex;
	//int xHistogram[3][256]; ZeroMemory(xHistogram, 3*256*sizeof(int));

	//// compute total histogram
	//for (nIndex = 0; nIndex < m_nSize; nIndex += 4)
	//{
	//	xHistogram[0][m_pPixels[nIndex + 0]]++;
	//	xHistogram[1][m_pPixels[nIndex + 1]]++;
	//	xHistogram[2][m_pPixels[nIndex + 2]]++;
	//}

	//// variables
	//int nContrastLevel = MulDiv(m_nSize/3, 5, 1000);
	//int nContrastSum[3], nContrastMin[3], nContrastMax[3];

	//// compute minimal scaling value
	//nContrastMin[0] = nContrastMin[1] = nContrastMin[2] = 0;
	//nContrastSum[0] = nContrastSum[1] = nContrastSum[2] = 0;
	//for (nIndex = 0; nIndex < 256; ++nIndex)
	//{
	//	if (!nContrastMin[0] && nContrastSum[0] >= nContrastLevel) nContrastMin[0] = nIndex;
	//	if (!nContrastMin[1] && nContrastSum[1] >= nContrastLevel) nContrastMin[1] = nIndex;
	//	if (!nContrastMin[2] && nContrastSum[2] >= nContrastLevel) nContrastMin[2] = nIndex;

	//	nContrastSum[0] += xHistogram[0][nIndex];
	//	nContrastSum[1] += xHistogram[1][nIndex];
	//	nContrastSum[2] += xHistogram[2][nIndex];
	//}

	//// compute maximal scaling value
	//nContrastMax[0] = nContrastMax[1] = nContrastMax[2] = 0;
	//nContrastSum[0] = nContrastSum[1] = nContrastSum[2] = 0;
	//for (nIndex = 255; nIndex >= 0; --nIndex)
	//{
	//	if (!nContrastMax[0] && nContrastSum[0] >= nContrastLevel) nContrastMax[0] = nIndex;
	//	if (!nContrastMax[1] && nContrastSum[1] >= nContrastLevel) nContrastMax[1] = nIndex;
	//	if (!nContrastMax[2] && nContrastSum[2] >= nContrastLevel) nContrastMax[2] = nIndex;

	//	nContrastSum[0] += xHistogram[0][nIndex];
	//	nContrastSum[1] += xHistogram[1][nIndex];
	//	nContrastSum[2] += xHistogram[2][nIndex];
	//}

	//// clamp ranges values
	//nContrastMin[0] = min(nContrastMin[0], 254); nContrastMax[0] = max(nContrastMax[0], nContrastMin[0] + 1);
	//nContrastMin[1] = min(nContrastMin[1], 254); nContrastMax[1] = max(nContrastMax[1], nContrastMin[0] + 1);
	//nContrastMin[2] = min(nContrastMin[2], 254); nContrastMax[2] = max(nContrastMax[2], nContrastMin[0] + 1);
	//
	//// variables
	//BYTE TableR[256];
	//BYTE TableG[256];
	//BYTE TableB[256];

	//// compute koefficients
	//double dAlpha1 = Frame;
	//double dAlpha2 = 1.0 - Frame;

	//// compose table
	//for (int nIndex = 0; nIndex < 256; ++nIndex)
	//{
	//	TableR[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*Byte(255.0*(nIndex - nContrastMin[2])/(double)(nContrastMax[2] - nContrastMin[2])));
	//	TableG[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*Byte(255.0*(nIndex - nContrastMin[1])/(double)(nContrastMax[1] - nContrastMin[1])));
	//	TableB[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*Byte(255.0*(nIndex - nContrastMin[0])/(double)(nContrastMax[0] - nContrastMin[0])));
	//}

	//// apply table
	//if (!ApplyTable(TableR, TableG, TableB))
	//	return S_FALSE;


	double AutoLR,AutoLG,AutoLB;
	GetAutoLevels(Frame,&AutoLR,&AutoLG,&AutoLB);
	AdjustLevels(Frame,AutoLR,AutoLG,AutoLB);

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

LRESULT ImageTransform3::AdjustLevels( double Frame, double LevelR, double LevelG, double LevelB)
{
	if (!IsValid())
		return S_FALSE;

	// clamp value
	Frame = Clamp(Frame);
	// variables
	BYTE TableR[256];
	BYTE TableG[256];
	BYTE TableB[256];

	// compute koefficients
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	int nContrastMin[3], nContrastMax[3];

	nContrastMax[0] = (int)(LevelB*5);
	nContrastMin[0] = LevelB*5000.0-nContrastMax[0]*1000.0;
	nContrastMax[1] = (int)(LevelG*5);
	nContrastMin[1] = LevelG*5000.0-nContrastMax[1]*1000.0;
	nContrastMax[2] = (int)(LevelR*5);
	nContrastMin[2] = LevelR*5000.0-nContrastMax[2]*1000.0;

	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
	{
		TableR[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*Byte(255.0*(nIndex - nContrastMin[2])/(double)(nContrastMax[2] - nContrastMin[2])));
		TableG[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*Byte(255.0*(nIndex - nContrastMin[1])/(double)(nContrastMax[1] - nContrastMin[1])));
		TableB[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*Byte(255.0*(nIndex - nContrastMin[0])/(double)(nContrastMax[0] - nContrastMin[0])));
	}

	// apply table
	if (!ApplyTable(TableR, TableG, TableB))
		return S_FALSE;
	// all ok
	ApplyRegister();

	return S_OK;
}

LRESULT ImageTransform3::GetAutoLevels(double Frame, double* LevelR, double* LevelG, double* LevelB)
{
	// clamp value
	Frame = Clamp(Frame);

	// variables
	int nIndex;
	int xHistogram[3][256]; ZeroMemory(xHistogram, 3*256*sizeof(int));

	// compute total histogram
	for (nIndex = 0; nIndex < m_nSize; nIndex += 4)
	{
		xHistogram[0][m_pPixels[nIndex + 0]]++;
		xHistogram[1][m_pPixels[nIndex + 1]]++;
		xHistogram[2][m_pPixels[nIndex + 2]]++;
	}

	// variables
	int nContrastLevel = MulDiv(m_nSize/3, 5, 1000);
	int nContrastSum[3], nContrastMin[3], nContrastMax[3];

	// compute minimal scaling value
	nContrastMin[0] = nContrastMin[1] = nContrastMin[2] = 0;
	nContrastSum[0] = nContrastSum[1] = nContrastSum[2] = 0;
	for (nIndex = 0; nIndex < 256; ++nIndex)
	{
		if (!nContrastMin[0] && nContrastSum[0] >= nContrastLevel) nContrastMin[0] = nIndex;
		if (!nContrastMin[1] && nContrastSum[1] >= nContrastLevel) nContrastMin[1] = nIndex;
		if (!nContrastMin[2] && nContrastSum[2] >= nContrastLevel) nContrastMin[2] = nIndex;

		nContrastSum[0] += xHistogram[0][nIndex];
		nContrastSum[1] += xHistogram[1][nIndex];
		nContrastSum[2] += xHistogram[2][nIndex];
	}

	// compute maximal scaling value
	nContrastMax[0] = nContrastMax[1] = nContrastMax[2] = 0;
	nContrastSum[0] = nContrastSum[1] = nContrastSum[2] = 0;
	for (nIndex = 255; nIndex >= 0; --nIndex)
	{
		if (!nContrastMax[0] && nContrastSum[0] >= nContrastLevel) nContrastMax[0] = nIndex;
		if (!nContrastMax[1] && nContrastSum[1] >= nContrastLevel) nContrastMax[1] = nIndex;
		if (!nContrastMax[2] && nContrastSum[2] >= nContrastLevel) nContrastMax[2] = nIndex;

		nContrastSum[0] += xHistogram[0][nIndex];
		nContrastSum[1] += xHistogram[1][nIndex];
		nContrastSum[2] += xHistogram[2][nIndex];
	}

	// clamp ranges values
	nContrastMin[0] = min(nContrastMin[0], 254); nContrastMax[0] = max(nContrastMax[0], nContrastMin[0] + 1);
	nContrastMin[1] = min(nContrastMin[1], 254); nContrastMax[1] = max(nContrastMax[1], nContrastMin[0] + 1);
	nContrastMin[2] = min(nContrastMin[2], 254); nContrastMax[2] = max(nContrastMax[2], nContrastMin[0] + 1);

	*LevelR = (nContrastMin[2]+1000*nContrastMax[2])/5000.0;
	*LevelG = (nContrastMin[1]+1000*nContrastMax[1])/5000.0;
	*LevelB = (nContrastMin[0]+1000*nContrastMax[0])/5000.0;
	
	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::AdjustHue(double Frame, double Angle)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Angle *= Clamp(Frame);
	Angle = min(max(-360, Angle), 360);

// non-optimized variant
#ifdef NOT_OPTIM // TODO: disabled optimization due to ignoring completeness value

	// variables
	float fRed, fGreen, fBlue;
	float fHue, fLightness, fSaturation;

	// compute koefficients
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// balance each pixel
	for (int nIndex = 0; nIndex < m_nSize; nIndex += 4)
	{
		// check for pixel mask
		if (m_bMask && m_pPixels[nIndex + 3] != 0)
			continue;

		// retrieve pixel color components
		fRed	= m_pPixels[nIndex + 2]/255.0f;
		fGreen	= m_pPixels[nIndex + 1]/255.0f;
		fBlue	= m_pPixels[nIndex + 0]/255.0f;

		// original color -> Hue-Saturation-Lighteness
		ConvertToHls(fRed, fGreen, fBlue, fHue, fLightness, fSaturation);

		// compute new hue value
		fHue += (float)Angle;

		// correct value
		if (fHue < 0)		fHue += 360.0f;
		if (fHue > 360.0f)	fHue -= 360.0f;

		// clamp value
		fHue = min(max(0.0f, fHue), 360.0f);

		// compute new color components
		ConvertToRgb(fHue, fLightness, fSaturation, fRed, fGreen, fBlue);

		// save pixel color components
		m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pPixels[nIndex + 2] + dAlpha1*255*fRed);
		m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pPixels[nIndex + 1] + dAlpha1*255*fGreen);
		m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pPixels[nIndex + 0] + dAlpha1*255*fBlue);
	}

#else

	// variables
	IppiSize roiSize;
	roiSize.width	= m_nWidth;
	roiSize.height	= m_nHeight;
	Ipp8u* pHLS = new Ipp8u[m_nSize];
	int nLevels[3] = {257, 2, 2};
	Ipp32s* pHValue = new Ipp32s[256];
	Ipp32s* pOValue = new Ipp32s[1];
	Ipp32s* pHLevel = new Ipp32s[257];
	Ipp32s* pOLevel = new Ipp32s[2];

	if (pHValue && pOValue && pHLevel && pOLevel)
	{
		ippiBGRToHLS_8u_AC4R(m_pPixels, 4*m_nWidth, pHLS, 4*m_nWidth, roiSize);
		
		pOValue[0] = 0;
		const Ipp32s* pValues[3] = {pHValue, pOValue, pOValue};
		
		pOLevel[0] = 256;
		pOLevel[1] = 257;
		const Ipp32s* pLevels[3] = {pHLevel, pOLevel, pOLevel};

		Ipp16s nAngle = 0;
		if (Angle>0)
			nAngle = 255*(Angle/360.0);
		else
			nAngle = 255*(1.0 + Angle/360.0);

		for (int i = 0; i < 256; ++i)
		{
			pHLevel[i] = i;
			pHValue[i] = (i + nAngle) & 0xFF;
		}

		pHLevel[256] = 256;

		ippiLUT_8u_AC4R(pHLS, 4*m_nWidth, m_pPixels, 4*m_nWidth, roiSize, pValues, pLevels, nLevels);
		ippiHLSToRGB_8u_AC4R(m_pPixels, 4*m_nWidth, pHLS, 4*m_nWidth, roiSize);

		int dstOrder[3];
		dstOrder[0] = 2;
		dstOrder[1] = 1;
		dstOrder[2] = 0;

		ippiSwapChannels_8u_AC4R(pHLS, 4*m_nWidth, m_pPixels, 4*m_nWidth, roiSize, dstOrder);
	}

	if (pHLS)
		delete[] pHLS;
	if (pHValue)
		delete[] pHValue;
	if (pHLevel)
		delete[] pHLevel;
	if (pOLevel)
		delete[] pOLevel;
	if (pOValue)
		delete[] pOValue;

#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

LRESULT ImageTransform3::GetAutoHue(double Completeness, double* Angle)
{
	float fRed, fGreen, fBlue;
	float fHue, fLightness, fSaturation;

	int   cnts=0;
	*Angle = 0.0;

	// balance each pixel
	for (int nIndex = 0; nIndex < m_nSize; nIndex += 4)
	{
		// check for pixel mask
		if (m_bMask && m_pPixels[nIndex + 3] != 0)
			continue;

		// retrieve pixel color components
		fRed	= m_pPixels[nIndex + 2]/255.0f;
		fGreen	= m_pPixels[nIndex + 1]/255.0f;
		fBlue	= m_pPixels[nIndex + 0]/255.0f;

		// original color -> Hue-Saturation-Lighteness
		ConvertToHls(fRed, fGreen, fBlue, fHue, fLightness, fSaturation);

		// correct value
		if (fHue < 0)		fHue += 360.0f;
		if (fHue > 360.0f)	fHue -= 360.0f;

		// clamp value
		fHue = min(max(0.0f, fHue), 360.0f);

		*Angle += fHue;
		cnts++;
	}

	if (cnts>0)
		*Angle /= cnts;

	return S_OK;
}

STDMETHODIMP ImageTransform3::AdjustSaturation(double Frame, double Level)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Level = min(max(-100, Level), 100)*Frame/100.0;

// non-optimized variant
#ifndef NOT_OPTIM

	// variables
	float fRed, fGreen, fBlue;
	float fHue, fLightness, fSaturation;

	// balance each pixel
	for (int nIndex = 0; nIndex < m_nSize; nIndex += 4)
	{
		// check for pixel mask
		if (m_bMask && m_pPixels[nIndex + 3] != 0)
			continue;

		// retrieve pixel color components
		fRed	= m_pPixels[nIndex + 2]/255.0f;
		fGreen	= m_pPixels[nIndex + 1]/255.0f;
		fBlue	= m_pPixels[nIndex + 0]/255.0f;

		// original color -> Hue-Saturation-Lighteness
		ConvertToHls(fRed, fGreen, fBlue, fHue, fLightness, fSaturation);

		// compute new saturation value
		fSaturation *= (float)(1.0 + Level);

		// clamp value
		fSaturation = min(max(0.0f, fSaturation), 1.0f);

		// compute new color components
		ConvertToRgb(fHue, fLightness, fSaturation, fRed, fGreen, fBlue);

		// save pixel color components
		m_pPixels[nIndex + 2] = (BYTE)(255*fRed);
		m_pPixels[nIndex + 1] = (BYTE)(255*fGreen);
		m_pPixels[nIndex + 0] = (BYTE)(255*fBlue);
	}

#else
	// глючит при отрицательном значении Level
	IppiSize roiSize;
	roiSize.width	= m_nWidth;
	roiSize.height	= m_nHeight;

	Ipp8u* pHLS = new Ipp8u[m_nSize];

	if (pHLS)
	{
		ippiBGRToHLS_8u_AC4R(m_pPixels, 4*m_nWidth, pHLS, 4*m_nWidth, roiSize);
		
		Level = (1.0 + Level);
		if (Level < 1.0/255.0)
		{
			int nLevels[3] = {2, 2, 2};
			Ipp32s* pValue = new Ipp32s[1];
			Ipp32s* pSLevel = new Ipp32s[2];
			Ipp32s* pOLevel = new Ipp32s[2];

			if (pValue && pSLevel && pOLevel)
			{
				pValue[0] = 0;
				const Ipp32s* pValues[3] = {pValue, pValue, pValue};
				
				pSLevel[0] = 0;
				pSLevel[1] = 257;

				//остальные каналы не преобразуем, поэтому ставим заведомо не выполнимые пределы
				pOLevel[0] = 256;
				pOLevel[1] = 257;
				const Ipp32s* pLevels[3] = {pOLevel, pOLevel, pSLevel};

				ippiLUT_8u_AC4R(pHLS, 4*m_nWidth, m_pPixels, 4*m_nWidth, roiSize, pValues, pLevels, nLevels);
			}

			if (pValue)
				delete[] pValue;
			if (pSLevel)
				delete pSLevel;
			if (pOLevel)
				delete pOLevel;
		}
		else if (Level < 1.0)
		{
			int nLevels[3] = {2, 2, 2};
			Ipp32s* pSValue = new Ipp32s[256];
			Ipp32s* pOValue = new Ipp32s[1];
			Ipp32s* pSLevel = new Ipp32s[257];
			Ipp32s* pOLevel = new Ipp32s[2];

			if (pSValue && pOValue && pSLevel && pOLevel)
			{
				pOValue[0] = 0;
				const Ipp32s* pValues[3] = {pOValue, pOValue, pSValue};
				
				//остальные каналы не преобразуем, поэтому ставим заведомо не выполнимые пределы
				pOLevel[0] = 256;
				pOLevel[1] = 257;
				const Ipp32s* pLevels[3] = {pOLevel, pOLevel, pSLevel};

				float fSum = 0.0;
				int nLevelCount=0;
				pSLevel[0] = 0;
				pSValue[0] = 0;
				for (int i = 0; i < 256; ++i)
				{
					fSum += (float)Level;
					if (fSum > 0.5f)
					{
						nLevelCount++;
						pSLevel[nLevelCount] = i;
						pSValue[nLevelCount] = pSValue[nLevelCount] + 1;
						fSum -= 1.0;
					}
				}

				nLevelCount++;
				pSLevel[nLevelCount] = 256;
				nLevels[2] = nLevelCount + 1;

				ippiLUT_8u_AC4R(pHLS, 4*m_nWidth, m_pPixels, 4*m_nWidth, roiSize, pValues, pLevels, nLevels);
			}

			if (pSValue)
				delete[] pSValue;
			if (pSLevel)
				delete[] pSLevel;
			if (pOLevel)
				delete[] pOLevel;
			if (pOValue)
				delete[] pOValue;
		}
		else
		{
			int nLevels[3] = {2,2,257};
			Ipp32s* pSValue = new Ipp32s[256];
			Ipp32s* pOValue = new Ipp32s[1];
			Ipp32s* pSLevel = new Ipp32s[257];
			Ipp32s* pOLevel = new Ipp32s[2];

			if (pSValue && pOValue && pSLevel && pOLevel)
			{
				pOValue[0] = 0;
				const Ipp32s* pValues[3] = {pOValue, pOValue, pSValue};
				
				//остальные каналы не преобразуем, поэтому ставим заведомо не выполнимые пределы
				pOLevel[0] = 256;
				pOLevel[1] = 257;
				const Ipp32s* pLevels[3] = {pOLevel, pOLevel, pSLevel};

				float fSum = 0.0;
				pSLevel[0] = 0;
				pSValue[0] = 0;

				int i=0;
				for (; i < 256; ++i)
				{
					pSLevel[i] = i;
					pSValue[i] = i*Level;
				}
				pSLevel[256] = 256;
				
				ippiLUT_8u_AC4R(pHLS, 4*m_nWidth, m_pPixels, 4*m_nWidth, roiSize, pValues, pLevels, nLevels);
			}

			if (pSValue)
				delete[] pSValue;
			if (pSLevel)
				delete[] pSLevel;
			if (pOLevel)
				delete[] pOLevel;
			if (pOValue)
				delete[] pOValue;
		}

		ippiHLSToRGB_8u_AC4R(m_pPixels, 4*m_nWidth, pHLS, 4*m_nWidth, roiSize);

		int dstOrder[3];
		dstOrder[0] = 2;
		dstOrder[1] = 1;
		dstOrder[2] = 0;

		ippiSwapChannels_8u_AC4R(pHLS, 4*m_nWidth, m_pPixels, 4*m_nWidth, roiSize, dstOrder);

		delete[] pHLS;
	}

#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

LRESULT ImageTransform3::GetAutoSaturation(double Completeness, double* Level)
{
	//GetAutoContrast(Completeness,Level);
	// variables
	float fRed, fGreen, fBlue;
	float fHue, fLightness, fSaturation;

	int   cnts=0;
	*Level = 0.0;

	// balance each pixel
	for (int nIndex = 0; nIndex < m_nSize; nIndex += 4)
	{
		// check for pixel mask
		if (m_bMask && m_pPixels[nIndex + 3] != 0)
			continue;

		// retrieve pixel color components
		fRed	= m_pPixels[nIndex + 2]/255.0f;
		fGreen	= m_pPixels[nIndex + 1]/255.0f;
		fBlue	= m_pPixels[nIndex + 0]/255.0f;

		// original color -> Hue-Saturation-Lighteness
		ConvertToHls(fRed, fGreen, fBlue, fHue, fLightness, fSaturation);

		// clamp value
		fSaturation = min(max(0.0f, fSaturation), 1.0f);

		*Level += fSaturation;
		cnts++;
	}

	if (cnts>0)
		*Level /= cnts;

	return S_OK;

}

STDMETHODIMP ImageTransform3::AdjustPosterize(double Frame, long Levels)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Levels = max(2, min(255, Levels));

	// variables
	BYTE Table[256];
	int nValue;
	double dKoef = (Levels - 1) / 255.0;

	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
	{
		nValue = (int)(0.5 + dKoef*nIndex);

		if (nValue < 0)
			nValue = 0;
		else if (nValue >= Levels)
			nValue = Levels - 1;

		Table[nIndex] = (BYTE)(nIndex*(1.0  - Frame) + Frame*MulDiv(255, nValue, Levels - 1));
	}

	// apply table
	if (!ApplyTable(Table))
		return S_FALSE;

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

LRESULT ImageTransform3::GetAutoPosterize(double Completeness, long* Levels)
{
	double tmpVals[3];
	GetAutoLevels(Completeness,&tmpVals[0],&tmpVals[1],&tmpVals[2]);
	
	*Levels = (tmpVals[0]+tmpVals[1]+tmpVals[2])/3.0;

	return S_OK;
}

STDMETHODIMP ImageTransform3::AdjustTemperature(double Frame, double Level)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Level = min(100, max(-100, Level));

	// variables
	BYTE TableR[256];
	BYTE TableG[256];
	BYTE TableB[256];

    // color adjustment coeffs
    double dDeltaRed = 0.3;
    double dDeltaGreen = 0.05;
    double dDeltaBlue = -0.2;

	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
	{
		TableR[nIndex] = Byte(nIndex + Frame*Level*dDeltaRed);
		TableG[nIndex] = Byte(nIndex + Frame*Level*dDeltaGreen);
		TableB[nIndex] = Byte(nIndex + Frame*Level*dDeltaBlue);
	}

	// apply table
	if (!ApplyTable(TableR, TableG, TableB))
		return S_FALSE;

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}


STDMETHODIMP ImageTransform3::AdjustColorize(double Frame, long Red, long Green, long Blue)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp value
	Frame = Clamp(Frame);

	// variables
	BYTE Value;
	BYTE TableR[256];
	BYTE TableG[256];
	BYTE TableB[256];

	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
	{
		TableR[nIndex] = MulDiv(Red, nIndex, 255);
		TableG[nIndex] = MulDiv(Green, nIndex, 255);
		TableB[nIndex] = MulDiv(Blue, nIndex, 255);
	}

	// correct alpha values
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// apply table
	for (int nIndex = 0; nIndex < m_nSize; nIndex += 4)
	{
		if (m_bMask && m_pPixels[nIndex + 3] != 0)
			continue;

		// compute new color
		Value = Intensity(m_pPixels[nIndex + 2], m_pPixels[nIndex + 1], m_pPixels[nIndex + 0]);

		// compute new color
		m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pPixels[nIndex + 0] + dAlpha1*TableB[Value]);
		m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pPixels[nIndex + 1] + dAlpha1*TableG[Value]);
		m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pPixels[nIndex + 2] + dAlpha1*TableR[Value]);
	}

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::AdjustThreshold(double Frame, long Level, VARIANT_BOOL Grayscale)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// variables
	BYTE Table[256];

	// compute equalize level
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// check for grayscale image
	if (Grayscale == VARIANT_TRUE)
	{
		AdjustGrayscale(Frame, VARIANT_FALSE);
	}
	// compose table
	for (int nIndex = 0; nIndex < 256; ++nIndex)
	{
		Table[nIndex] = (BYTE)(dAlpha2*nIndex + dAlpha1*(nIndex < Level ? 0 : 255));
	}
	// apply table
	if (!ApplyTable(Table))
	{
		return S_FALSE;
	}

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}
		
STDMETHODIMP ImageTransform3::EffectBlur(double Frame, double Level)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Level = min(100, max(0, Level));

// non-optimized variant
#ifdef NOT_OPTIM

	// compose matrix
	double Matrix[9];
	Matrix[0] = Matrix[1] = Matrix[2] = Matrix[3] = Matrix[5] = Matrix[6] = Matrix[7] = Matrix[8] = 1;
	Matrix[4] = (double)(21 - 20*Frame*Level/100.0);

	// matrix parameters
	int nOffsetsX[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
	int nOffsetsY[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	double dMatrixKoef = 1.0/(Matrix[4] + 8.0);

	// variables
	int nX, nY, nIndex, nPoint;
	int nSourceX, nSourceY, nSourceIndex;
	double dRed, dGreen, dBlue;

	// compute koefficients
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// begin effect
	BeginEffect();

	// for each pixel - compute new color value
	for (nY = 0, nIndex = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
				continue;

			// set the default maximal values
			dRed = 0;
			dGreen = 0;
			dBlue = 0;

			// compute new pixel color
			for (nPoint = 0; nPoint < 9; ++nPoint)
			{
				// compute source pixel
				nSourceX = nX + nOffsetsX[nPoint];
				nSourceY = nY + nOffsetsY[nPoint];
				
				// clamp cordinates
				if (nSourceX < 0)					nSourceX = 0;
				else if (nSourceX > m_nWidth - 1)	nSourceX = m_nWidth - 1;
				if (nSourceY < 0)					nSourceY = 0;
				else if (nSourceY > m_nHeight - 1)	nSourceY = m_nHeight - 1;

				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				// update color and denominator
				dRed	+= Matrix[nPoint]*m_pBackup[nSourceIndex + 2];
				dGreen	+= Matrix[nPoint]*m_pBackup[nSourceIndex + 1];
				dBlue	+= Matrix[nPoint]*m_pBackup[nSourceIndex + 0];
			}

			// save resulting color
			m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dAlpha1*Byte(dMatrixKoef*dBlue));
			m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dAlpha1*Byte(dMatrixKoef*dGreen));
			m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dAlpha1*Byte(dMatrixKoef*dRed));
		}
	}

	// finalize effect
	EndEffect();

#else
	IPPEffectBlur(m_pPixels,m_nWidth,m_nHeight,Frame,Level);
#endif
	
	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectSharpen(double Frame, double Level)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Level = min(100, max(0, Level));

// non-optimized variant
#ifdef NOT_OPTIM

	// compose matrix
	double Matrix[9];
	Matrix[0] = Matrix[1] = Matrix[2] = Matrix[3] = Matrix[5] = Matrix[6] = Matrix[7] = Matrix[8] = -1;
	Matrix[4] = (double)(9 + 23*(1.0 - Frame*Level/100.0));

	// matrix parameters
	int nOffsetsX[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
	int nOffsetsY[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	double dMatrixKoef = 1.0/(Matrix[4] - 8.0);

	// variables
	int nX, nY, nIndex, nPoint;
	int nSourceX, nSourceY, nSourceIndex;
	double dRed, dGreen, dBlue;

	// compute koefficients
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// begin effect
	BeginEffect();

	// for each pixel - compute new color value
	for (nY = 0, nIndex = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
				continue;

			// set the default maximal values
			dRed = 0;
			dGreen = 0;
			dBlue = 0;

			// compute new pixel color
			for (nPoint = 0; nPoint < 9; ++nPoint)
			{
				// compute source pixel
				nSourceX = nX + nOffsetsX[nPoint];
				nSourceY = nY + nOffsetsY[nPoint];
				
				// clamp cordinates
				if (nSourceX < 0)					nSourceX = 0;
				else if (nSourceX > m_nWidth - 1)	nSourceX = m_nWidth - 1;
				if (nSourceY < 0)					nSourceY = 0;
				else if (nSourceY > m_nHeight - 1)	nSourceY = m_nHeight - 1;

				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				// update color and denominator
				dRed	+= Matrix[nPoint]*m_pBackup[nSourceIndex + 2];
				dGreen	+= Matrix[nPoint]*m_pBackup[nSourceIndex + 1];
				dBlue	+= Matrix[nPoint]*m_pBackup[nSourceIndex + 0];
			}

			// save resulting color
			m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dAlpha1*Byte(dMatrixKoef*dBlue));
			m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dAlpha1*Byte(dMatrixKoef*dGreen));
			m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dAlpha1*Byte(dMatrixKoef*dRed));
		}
	}

	// finalize effect
	EndEffect();

#else
	IPPEffectSharpen(m_pPixels,m_nWidth,m_nHeight,Frame,Level);
#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectMosaic(double Frame, long Size, VARIANT_BOOL Simple)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Size = (int)(min(255, max(1, Size*Frame)));

// non-optimized variant
#ifdef NOT_OPTIM

	// variables
	int nTilesX = m_nWidth/Size + (m_nWidth % Size > 0 ? 1 : 0);
	int nTilesY = m_nHeight/Size + (m_nHeight % Size > 0 ? 1 : 0);
	int nX, nY, nIndex, nTX, nTY, nTLeft, nTRight, nTTop, nTBottom;
	int nRed, nGreen, nBlue, nDenom;

	// begin effect
	BeginEffect();

	// quadrize image
	for (nTY = 0; nTY < nTilesY; ++nTY)
	{
		// compute tile y - ranges
		nTTop = nTY*Size;
		nTBottom = nTTop + Size;

		// clamp tile y - range
		nTBottom = min(nTBottom, m_nHeight);

		for (nTX = 0; nTX < nTilesX; ++nTX)
		{
			// compute tile x - ranges
			nTLeft = nTX*Size;
			nTRight = nTLeft + Size;

			// clamp tile x - range
			nTRight = min(nTRight, m_nWidth);

			// set tile color
			if (Simple == VARIANT_TRUE)
			{
				// compute pixel index
				nIndex = 4*(nTTop*m_nWidth + nTLeft);

				// retrieve pixel color
				nRed	= m_pBackup[nIndex + 2];
				nGreen	= m_pBackup[nIndex + 1];
				nBlue	= m_pBackup[nIndex + 0];

				// apply computed color
				for (nY = nTTop; nY < nTBottom; ++nY)
				{
					// compute starting index
					nIndex = 4*(nY*m_nWidth + nTLeft);

					for (nX = nTLeft; nX < nTRight; ++nX, nIndex += 4)
					{
						// check for pixel mask
						if (m_bMask && m_pBackup[nIndex + 3] != 0)
							continue;

						// update color
						m_pPixels[nIndex + 2] = nRed;
						m_pPixels[nIndex + 1] = nGreen;
						m_pPixels[nIndex + 0] = nBlue;
					}
				}
			}
			else
			{
				// prepare color values
				nRed	= 0;
				nGreen	= 0;
				nBlue	= 0;
				nDenom	= 0;

				// compute tile color and tile pixel indexes
				for (nY = nTTop; nY < nTBottom; ++nY)
				{
					// compute starting pixel index
					nIndex = 4*(nY*m_nWidth + nTLeft);

					for (nX = nTLeft; nX < nTRight; ++nX, nIndex += 4)
					{
						// update color
						nRed	+= m_pBackup[nIndex + 2];
						nGreen	+= m_pBackup[nIndex + 1];
						nBlue	+= m_pBackup[nIndex + 0];

						// update denominator
						nDenom	+= 1;
					}
				}

				// compute average color value
				nRed	/= nDenom;
				nGreen	/= nDenom;
				nBlue	/= nDenom;

				// apply computed color to each pixel of the tile
				for (nY = nTTop; nY < nTBottom; ++nY)
				{
					// compute starting pixel index
					nIndex = 4*(nY*m_nWidth + nTLeft);

					for (nX = nTLeft; nX < nTRight; ++nX, nIndex += 4)
					{
						// check for pixel mask
						if (m_bMask && m_pBackup[nIndex + 3] != 0)
							continue;

						// update color
						m_pPixels[nIndex + 2] = nRed;
						m_pPixels[nIndex + 1] = nGreen;
						m_pPixels[nIndex + 0] = nBlue;
					}
				}
			}
		}
	}

	// finalize effect
	EndEffect();

#else
	IPPEffectMosaic(m_pPixels, m_nWidth, m_nHeight, Frame, Size, (VARIANT_TRUE == Simple));
#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectGaussianBlur(double Frame, long Size)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Size = int(Size*Frame + 0.5);
	Size = (min(100, max(0, Size)));

// non-optimized variant
#ifdef NOT_OPTIM

	// variables
	int nRed, nGreen, nBlue, nDenom = 2*Size + 1;

	// create additional channel
	BYTE* pCopy = new BYTE[4*m_nWidth*m_nHeight];

	// check for allocated memory
	if (!pCopy)
		return S_OK;

	// variables
	int nX, nY, nIndex, nOffset;
	int nSourceX, nSourceY, nSourceIndex;

	// begin effect
	BeginEffect();

	for (int nStep = 0; nStep < 2; ++nStep)
	{
		// compute starting index
		nIndex = 0;

		// combine several moved images
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
			{
				// prepare color values
				nRed = 0;
				nGreen = 0;
				nBlue = 0;

				// compute moved pixels sum
				for (nOffset = -Size; nOffset <= Size; ++nOffset)
				{
					// compute source y coordinate
					nSourceY = nOffset + nY;

					// clamp it to valid range
					if (nSourceY < 0)				nSourceY = 0;
					else if (nSourceY >= m_nHeight)	nSourceY = m_nHeight - 1;

					// compute source pixel index
					nSourceIndex = 4*(nX + nSourceY*m_nWidth);

					// update color value
					nRed	+= m_pPixels[nSourceIndex + 2];
					nGreen	+= m_pPixels[nSourceIndex + 1];
					nBlue	+= m_pPixels[nSourceIndex + 0];
				}

				// save color
				pCopy[nIndex + 2] = (BYTE)(nRed/nDenom);
				pCopy[nIndex + 1] = (BYTE)(nGreen/nDenom);
				pCopy[nIndex + 0] = (BYTE)(nBlue/nDenom);
			}
		}

		// compute starting index
		nIndex = 0;

		// combine several moved images
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			// compute starting index
			nSourceIndex = 4*(nY*m_nWidth);

			for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
			{
				// prepare color values
				nRed = 0;
				nGreen = 0;
				nBlue = 0;

				// compute moved pixels sum
				for (nOffset = -Size; nOffset <= Size; ++nOffset)
				{
					// compute source y coordinate
					nSourceX = nOffset + nX;

					// clamp it to valid range
					if (nSourceX < 0)				nSourceX = 0;
					else if (nSourceX >= m_nWidth)	nSourceX = m_nWidth - 1;

					// update color value
					nRed	+= pCopy[nSourceIndex + 4*nSourceX + 2];
					nGreen	+= pCopy[nSourceIndex + 4*nSourceX + 1];
					nBlue	+= pCopy[nSourceIndex + 4*nSourceX + 0];
				}

				// save color
				m_pPixels[nIndex + 2] = (BYTE)(nRed/nDenom);
				m_pPixels[nIndex + 1] = (BYTE)(nGreen/nDenom);
				m_pPixels[nIndex + 0] = (BYTE)(nBlue/nDenom);
			}
		}
	}

	// clear memory
	delete[] pCopy;

	// restore unchanged pixels
	for (nY = 0, nIndex = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] == 0)
			{
				// restore pixel color
				m_pPixels[nIndex + 2] = m_pBackup[nIndex + 2];
				m_pPixels[nIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nIndex + 0] = m_pBackup[nIndex + 0];
			}
		}
	}

	// finalize effect
	EndEffect();

#else
	if( Size < 10 )
	{
		IPPEffectGaussianBlur( m_pPixels, m_nWidth, m_nHeight, Frame, Size );
	}
	else
	{
		IPPEffectGaussianBlur2( m_pPixels, m_nWidth, m_nHeight, Frame, Size );
	}
#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectNoise(double Frame, double Level, VARIANT_BOOL Mono)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Level = (int)(min(1000, max(1, Level*Frame)));

// non-optimized variant
#ifdef NOT_OPTIM

	// variables
	int nX, nY, nIndex;
	int nRed, nGreen, nBlue;
	int nNoiseRed, nNoiseGreen, nNoiseBlue;

	// restart randomizer
	srand(GetTickCount());

	// compute starting index
	nIndex = 0;

	// add noise to image
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// check for pixel mask
			if (m_bMask && m_pPixels[nIndex + 3] != 0)
				continue;

			// retrieve pixel color
			nRed	= m_pPixels[nIndex + 2];
			nGreen	= m_pPixels[nIndex + 1];
			nBlue	= m_pPixels[nIndex + 0];

			// add noise
			if (Mono == VARIANT_TRUE)
				nNoiseRed = nNoiseGreen = nNoiseBlue = (int)(Level*Random());
			else
			{
				nNoiseRed	= (int)(Level*Random());
				nNoiseGreen	= (int)(Level*Random());
				nNoiseBlue	= (int)(Level*Random());
			}

			// add noise to color
			nRed	+= nNoiseRed;
			nGreen	+= nNoiseGreen;
			nBlue	+= nNoiseBlue;

			// save color
			m_pPixels[nIndex + 2] = Byte(nRed);
			m_pPixels[nIndex + 1] = Byte(nGreen);
			m_pPixels[nIndex + 0] = Byte(nBlue);
		}
	}

#else
	IPPEffectNoise(m_pPixels,m_nWidth,m_nHeight,Frame,Level,(VARIANT_TRUE==Mono));
#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectDiffuse(double Frame, double Distance)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Distance = min(255, max(1, Distance));

// non-optimized variant
#ifdef NOT_OPTIM

	// restart randomizer
	srand(GetTickCount());

	// variables
	int nX, nY, nIndex;
	int nXSwap, nYSwap, nIndexSwap;
	double dRed, dGreen, dBlue;
	double dRedSwap, dGreenSwap, dBlueSwap;

	// compute penetration values
	double dValue1 = 1.0 - Frame;
	double dValue2 = Frame;

	// diffuse image
	for (nY = 0, nIndex = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// check for pixel mask
			if (m_bMask && m_pPixels[nIndex + 3] != 0)
				continue;

			// compute random swap pixel
			nXSwap = nX + (int)(Distance*Random());
			nYSwap = nY + (int)(Distance*Random());

			// check whether pixel coordinates are valid
			if (nXSwap < 0 || nXSwap >= m_nWidth || nYSwap < 0 || nYSwap >= m_nHeight)
				continue;

			// compute swap pixel index
			nIndexSwap = 4*(nXSwap + nYSwap*m_nWidth);

			// compute swapped colors
			dRed		= dValue1*m_pPixels[nIndex + 2] + dValue2*m_pPixels[nIndexSwap + 2];
			dGreen		= dValue1*m_pPixels[nIndex + 1] + dValue2*m_pPixels[nIndexSwap + 1];
			dBlue		= dValue1*m_pPixels[nIndex + 0] + dValue2*m_pPixels[nIndexSwap + 0];

			dRedSwap	= dValue1*m_pPixels[nIndexSwap + 2] + dValue2*m_pPixels[nIndex + 2];
			dGreenSwap	= dValue1*m_pPixels[nIndexSwap + 1] + dValue2*m_pPixels[nIndex + 1];
			dBlueSwap	= dValue1*m_pPixels[nIndexSwap + 0] + dValue2*m_pPixels[nIndex + 0];

			// swap pixels
			m_pPixels[nIndex + 2] = (BYTE)dRed;
			m_pPixels[nIndex + 1] = (BYTE)dGreen;
			m_pPixels[nIndex + 0] = (BYTE)dBlue;
			
			m_pPixels[nIndexSwap + 2] = (BYTE)dRedSwap;
			m_pPixels[nIndexSwap + 1] = (BYTE)dGreenSwap;
			m_pPixels[nIndexSwap + 0] = (BYTE)dBlueSwap;
		}
	}

#else	
	IPPEffectDiffuse(m_pPixels,m_nWidth,m_nHeight,Frame,Distance);
#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectMotionBlur(double Frame, double Angle, double Distance)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// compute number of pixels to process
	int nAmount = (int)(max(1, Frame*min(max(1, Distance), 255)));

	// compute angle in radians
	double dAngleInRadians = 3.14159265359*(Angle)/180.0;

	// variables
	int nSourceX, nSourceY, nSourceIndex;
	int nX, nY, nIndex, nPoint;
	int nRed, nGreen, nBlue, nDenom = nAmount;

	// allocate memory for offsets
	int* nOffsetsX = new int[nAmount];
	int* nOffsetsY = new int[nAmount];

	// compute offsets
	for (nPoint = 0; nPoint < nAmount; ++nPoint)
	{
		nOffsetsX[nPoint] = (int)(0.5 + nPoint*cos(dAngleInRadians));
		nOffsetsY[nPoint] = (int)(0.5 + nPoint*sin(dAngleInRadians));
	}

	// begin effect
	BeginEffect();

	// initialize pixel index
	nIndex = 0;

	// for each pixel - compute new color value
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
				continue;

			// initialize color values
			nRed = 0;
			nGreen = 0;
			nBlue = 0;

			// compute new pixel color
			for (nPoint = 0; nPoint < nAmount; ++nPoint)
			{
				nSourceX = nX + nOffsetsX[nPoint];
				nSourceY = nY + nOffsetsY[nPoint];
				
				// clamp cordinates
				if (nSourceX < 0)					nSourceX = 0;
				else if (nSourceX > m_nWidth - 1)	nSourceX = m_nWidth - 1;
				if (nSourceY < 0)					nSourceY = 0;
				else if (nSourceY > m_nHeight - 1)	nSourceY = m_nHeight - 1;

				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				// update color and denominator
				nRed	+= m_pBackup[nSourceIndex + 2];
				nGreen	+= m_pBackup[nSourceIndex + 1];
				nBlue	+= m_pBackup[nSourceIndex + 0];
			}

			// save resulting color
			m_pPixels[nIndex + 2] = (BYTE)(nRed/nDenom);
			m_pPixels[nIndex + 1] = (BYTE)(nGreen/nDenom);
			m_pPixels[nIndex + 0] = (BYTE)(nBlue/nDenom);
		}
	}

	// clear memory
	delete[] nOffsetsX;
	delete[] nOffsetsY;

	// finalize effect
	EndEffect();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectEmboss(double Frame, double Angle, double Distance, double Amount)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Amount = min(max(Frame*Amount, 1), 100)/10.0;

	// begin effect
	BeginEffect();

	// variables
	int nX, nY, nIndex;
	int nSourceX[2], nSourceY[2], nSourceIndex[2];
	int nRed[2], nGreen[2], nBlue[2];

	double dRadians = 3.14159265359*Angle/180;
	double dX = 0.5*Frame*Distance*cos(dRadians);
	double dY = 0.5*Frame*Distance*sin(dRadians);

	dX += dX > 0 ? 0.5 : -0.5;
	dY += dY > 0 ? 0.5 : -0.5;

	int nDeltaX = int(dX);
	int nDeltaY = int(dY);

	if( !nDeltaX && !nDeltaY )
	{
		nDeltaX = 1;
	}

	// compute koefficients
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// initialize pixel index
	nIndex = 0;

	// for each pixel - compute new color value
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
				continue;

			// compute source pixels coordinates
			nSourceX[0] = nX + nDeltaX;
			nSourceY[0] = nY + nDeltaY;
			nSourceX[1] = nX - nDeltaX;
			nSourceY[1] = nY - nDeltaY;

			// correct coordinates and compute pixel indexes
			for (int nPoint = 0; nPoint < 2; ++nPoint)
			{
				// check whether source coordinates are valid
				if (nSourceX[nPoint] < 0)			nSourceX[nPoint] = 0;
				if (nSourceX[nPoint] >= m_nWidth)	nSourceX[nPoint] = m_nWidth - 1;
				if (nSourceY[nPoint] < 0)			nSourceY[nPoint] = 0; 
				if (nSourceY[nPoint] >= m_nHeight)	nSourceY[nPoint] = m_nHeight - 1;

				// compute source pixel index
				nSourceIndex[nPoint] = 4*(nSourceX[nPoint] + nSourceY[nPoint]*m_nWidth);

				// retrieve current pixel color
				nRed[nPoint]	= m_pBackup[nSourceIndex[nPoint] + 2];
				nGreen[nPoint]	= m_pBackup[nSourceIndex[nPoint] + 1];
				nBlue[nPoint]	= m_pBackup[nSourceIndex[nPoint] + 0];
			}

			// compute result color
			nRed[0]   = (nRed[0]   + (255 - nRed[1]))  >>1;
			nGreen[0] = (nGreen[0] + (255 - nGreen[1]))>>1;
			nBlue[0]  = (nBlue[0]  + (255 - nBlue[1])) >>1;

			// save pixel color
			m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dAlpha1*Byte(127 + (nRed[0] - 127)*Amount));
			m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dAlpha1*Byte(127 + (nGreen[0] - 127)*Amount));
			m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dAlpha1*Byte(127 + (nBlue[0] - 127)*Amount));
		}
	}

	// finalize effect
	EndEffect();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectMinimal(double Frame, long Size)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Size = min(max(Size, 1), 255);

// non-optimized variant
#ifdef NOT_OPTIM

	// begin effect
	BeginEffect();

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nMinRed, nMinGreen, nMinBlue;
	int nDX, nDY;

	// compute koefficients
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// initialize pixel index
	nIndex = 0;

	// for each pixel - compute new color value
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
				continue;

			// set the default minimal values
			nMinRed = 255;
			nMinGreen = 255;
			nMinBlue = 255;

			// compute new pixel color
			for (nDY = -Size; nDY <= Size; ++nDY)
			{
				for (nDX = -Size; nDX <= Size; ++nDX)
				{
					nSourceX = nX + nDX;
					nSourceY = nY + nDY;
					
					// clamp cordinates
					if (nSourceX < 0 || nSourceX > m_nWidth - 1 || nSourceY < 0 || nSourceY > m_nHeight - 1)
						continue;

					// compute source pixel index
					nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

					// update color
					nMinRed = min(nMinRed, m_pBackup[nSourceIndex + 2]);
					nMinGreen = min(nMinGreen, m_pBackup[nSourceIndex + 1]);
					nMinBlue = min(nMinBlue, m_pBackup[nSourceIndex + 0]);
				}
			}

			// save resulting color
			m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dAlpha1*nMinRed);
			m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dAlpha1*nMinGreen);
			m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dAlpha1*nMinBlue);
		}
	}

	// finalize effect
	EndEffect();

#else
	IPPEffectMinimal(m_pPixels,m_nWidth,m_nHeight,Frame,Size);
#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectMaximal(double Frame, long Size)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Size = min(max(Size, 1), 255);

// non-optimized variant
#ifdef NOT_OPTIM

	// begin effect
	BeginEffect();

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;
	int nMaxRed, nMaxGreen, nMaxBlue;
	int nDX, nDY;

	// compute koefficients
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;

	// initialize pixel index
	nIndex = 0;

	// for each pixel - compute new color value
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
				continue;

			// set the default maximal values
			nMaxRed = 0;
			nMaxGreen = 0;
			nMaxBlue = 0;

			// compute new pixel color
			for (nDY = -Size; nDY <= Size; ++nDY)
			{
				for (nDX = -Size; nDX <= Size; ++nDX)
				{
					nSourceX = nX + nDX;
					nSourceY = nY + nDY;
					
					// clamp cordinates
					if (nSourceX < 0 || nSourceX > m_nWidth - 1 || nSourceY < 0 || nSourceY > m_nHeight - 1)
						continue;

					// compute source pixel index
					nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

					// update color
					nMaxRed = max(nMaxRed, m_pBackup[nSourceIndex + 2]);
					nMaxGreen = max(nMaxGreen, m_pBackup[nSourceIndex + 1]);
					nMaxBlue = max(nMaxBlue, m_pBackup[nSourceIndex + 0]);
				}
			}

			// save resulting color
			m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pBackup[nIndex + 2] + dAlpha1*nMaxRed);
			m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pBackup[nIndex + 1] + dAlpha1*nMaxGreen);
			m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pBackup[nIndex + 0] + dAlpha1*nMaxBlue);
		}
	}

	// finalize effect
	EndEffect();

#else
	IPPEffectMaximal(m_pPixels,m_nWidth,m_nHeight,Frame,Size);
#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectDeinterlace(double Frame, VARIANT_BOOL Even, VARIANT_BOOL Interpolate)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

// non-optimized variant
#ifdef NOT_OPTIM

	// variables
	int nX, nY, nIndex1, nIndex2, nIndex3;
	int nStartLine = (Even == VARIANT_TRUE) ? 0 : 1;

	// compute koefficients
	double dAlpha1 = Frame;
	double dAlpha2 = 1.0 - Frame;
	double dAlpha3 = 0.5*dAlpha1;

	// for each pixel - compute new color value
	for (nY = nStartLine; nY < m_nHeight; nY += 2)
	{
		for (nX = 0; nX < m_nWidth; ++nX)
		{
			// compute index of pixel to process
			nIndex2 = 4*(nX + nY*m_nWidth);

			// check for pixel mask
			if (m_bMask && m_pPixels[nIndex2 + 3] != 0)
				continue;

			// compute index of pixel in previous and next lines
			nIndex1 = nIndex2 - 4*m_nWidth;
			nIndex3 = nIndex2 + 4*m_nWidth;

			// check for interpolation type
			if (nY == 0)
			{
				m_pPixels[nIndex2 + 2] = (BYTE)(dAlpha1*m_pPixels[nIndex3 + 2] + dAlpha2*m_pPixels[nIndex2 + 2]);
				m_pPixels[nIndex2 + 1] = (BYTE)(dAlpha1*m_pPixels[nIndex3 + 1] + dAlpha2*m_pPixels[nIndex2 + 1]);
				m_pPixels[nIndex2 + 0] = (BYTE)(dAlpha1*m_pPixels[nIndex3 + 0] + dAlpha2*m_pPixels[nIndex2 + 0]);
			}
			else if (nY == m_nHeight - 1)
			{
				m_pPixels[nIndex2 + 2] = (BYTE)(dAlpha1*m_pPixels[nIndex1 + 2] + dAlpha2*m_pPixels[nIndex2 + 2]);
				m_pPixels[nIndex2 + 1] = (BYTE)(dAlpha1*m_pPixels[nIndex1 + 1] + dAlpha2*m_pPixels[nIndex2 + 1]);
				m_pPixels[nIndex2 + 0] = (BYTE)(dAlpha1*m_pPixels[nIndex1 + 0] + dAlpha2*m_pPixels[nIndex2 + 0]);
			}
			else
			{
				if (Interpolate == VARIANT_FALSE)
				{
					m_pPixels[nIndex2 + 2] = (BYTE)(dAlpha1*m_pPixels[nIndex3 + 2] + dAlpha2*m_pPixels[nIndex2 + 2]);
					m_pPixels[nIndex2 + 1] = (BYTE)(dAlpha1*m_pPixels[nIndex3 + 1] + dAlpha2*m_pPixels[nIndex2 + 1]);
					m_pPixels[nIndex2 + 0] = (BYTE)(dAlpha1*m_pPixels[nIndex3 + 0] + dAlpha2*m_pPixels[nIndex2 + 0]);
				}
				else
				{
					m_pPixels[nIndex2 + 2] = (BYTE)(dAlpha3*m_pPixels[nIndex1 + 2] + dAlpha3*m_pPixels[nIndex3 + 2] + dAlpha2*m_pPixels[nIndex2 + 2]);
					m_pPixels[nIndex2 + 1] = (BYTE)(dAlpha3*m_pPixels[nIndex1 + 1] + dAlpha3*m_pPixels[nIndex3 + 1] + dAlpha2*m_pPixels[nIndex2 + 1]);
					m_pPixels[nIndex2 + 0] = (BYTE)(dAlpha3*m_pPixels[nIndex1 + 0] + dAlpha3*m_pPixels[nIndex3 + 0] + dAlpha2*m_pPixels[nIndex2 + 0]);
				}
			}
		}
	}

#else
	IPPEffectDeinterlace(m_pPixels, m_nWidth, m_nHeight, Frame, (VARIANT_TRUE == Even), (VARIANT_TRUE == Interpolate));
#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::EffectMedian(double Frame, long MedianType)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	IPPEffectMedian(m_pPixels,m_nWidth,m_nHeight,Frame,MedianType);
	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}
STDMETHODIMP ImageTransform3::EffectChromoKey(double Frame, long Color1, long Color2)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	Color col;
	float r, g, b;
	float fLightness, fSaturation;
	float fHue1;
	col.SetValue(Color1);	
	r = (float)col.GetB();
	r /= 255.0f;
	g = (float)col.GetG();
	g /= 255.0f;
	b = (float)col.GetR();
	b /= 255.0f;

	ConvertToHls(r, g, b, fHue1, fLightness, fSaturation);

	float fHue2;
	col.SetValue(Color2);	
	r = (float)col.GetB();
	r /= 255.0f;
	g = (float)col.GetG();
	g /= 255.0f;
	b = (float)col.GetR();
	b /= 255.0f;

	ConvertToHls(r, g, b, fHue2, fLightness, fSaturation);

	int nHComponent1 = fHue1*255.0f/360.0f;
	int nHComponent2 = fHue2*255.0f/360.0f;

	CChromoKey oChromo;
	oChromo.SetImage(m_pPixels, m_nWidth, m_nHeight);
	oChromo.DoChromoKey(Frame, nHComponent1, m_nAdvancedChromaThreshold1, nHComponent2, m_nAdvancedChromaThreshold2);

	ApplyRegister();

	// all ok	 
	return S_OK;
}
STDMETHODIMP ImageTransform3::EffectChromoKey2(double Frame, long color, long Threshold)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	Color col;
	float r, g, b;
	float fHue, fLightness, fSaturation;
	float koef1 = 0, koef2 = 0;
	col.SetValue(color);	
	r = (float)col.GetB();
	r /= 255.0f;
	g = (float)col.GetG();
	g /= 255.0f;
	b = (float)col.GetR();
	b /= 255.0f;

	ConvertToHls(r, g, b, fHue, fLightness, fSaturation);

	if(Threshold > 100)
		Threshold = 100;
	if(Threshold < 0)
		Threshold = 0;

	int nHComponent = fHue*255.0f/360.0f;
	CChromoKey oChromo;
	oChromo.SetImage(m_pPixels, m_nWidth, m_nHeight);
	oChromo.DoChromoKey(Frame, nHComponent, Threshold);
	
	ApplyRegister();

	return S_OK;
}
		
STDMETHODIMP ImageTransform3::TransformFlip(double Frame, VARIANT_BOOL Vertical)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

#ifndef NOT_OPTIM // TODO: disabled optimization due to access violation

	// variables
	int nX, nY, nIndex;
	int nDestX, nDestY, nDestIndex;

	// begin effect
	BeginEffect();

	// clear image
	ClearImage(m_pPixels, m_nSize);

	// flip image
	for (nY = 0, nIndex = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// check for pixel mask
			if (m_bMask && m_pBackup[nIndex + 3] != 0)
				continue;

			// compute random swap pixel
			if (Vertical == VARIANT_TRUE)
			{
				nDestX = nX;
				nDestY = (int)((1.0 - Frame)*nY + Frame*(m_nHeight - 1 - nY));
			}
			else
			{
				nDestX = (int)((1.0 - Frame)*nX + Frame*(m_nWidth - 1 - nX));
				nDestY = nY;
			}

			// compute swap pixel index
			nDestIndex = 4*(nDestX + nDestY*m_nWidth);

			// swap pixels
			m_pPixels[nDestIndex + 2] = m_pBackup[nIndex + 2];
			m_pPixels[nDestIndex + 1] = m_pBackup[nIndex + 1];
			m_pPixels[nDestIndex + 0] = m_pBackup[nIndex + 0];
		}
	}

#else

	if (fabs(Frame) < 0.001)
		return S_OK;
    
	Ipp8u* pDst = (Ipp8u*)m_pPixels;

	if (fabs(Frame) > 0.999)
	{
		if (Vertical == VARIANT_TRUE)
			ippiMirror_8u_AC4IR(pDst, m_nIppiStep, m_IppiRoiSize, ippAxsHorizontal);
		else
			ippiMirror_8u_AC4IR(pDst, m_nIppiStep, m_IppiRoiSize, ippAxsVertical);
		return S_OK;
	}

	// begin effect
    BeginEffect();

    // clear image
    ClearImage(m_pPixels, m_nSize);

    Ipp8u* pSrc = (Ipp8u*)m_pBackup;
    IppiRect srcRoi = {0, 0, m_IppiRoiSize.width , m_IppiRoiSize.height};
    double dResizeX = 1, dResizeY = 1;
    double dfCenterX = 0.5*m_nWidth;
	double dfCenterY = 0.5*m_nHeight;

	// apply transform
    if (Vertical == VARIANT_TRUE)
    {
        dResizeY = 1 - 2 * ((Frame < 0.5) ? Frame : (1 - Frame));
        ippiResizeCenter_8u_AC4R(pSrc, m_IppiRoiSize, m_nIppiStep , srcRoi, pDst, m_nIppiStep, m_IppiRoiSize, dResizeX, dResizeY, dfCenterX, dfCenterY, IPPI_INTER_NN);
        if (Frame > 0.5)
            ippiMirror_8u_AC4IR(pDst, m_nIppiStep, m_IppiRoiSize, ippAxsHorizontal);
    }
    else
    {
        dResizeX = 1 - 2 * ((Frame < 0.5) ? Frame : (1 - Frame));
        ippiResizeCenter_8u_AC4R(pSrc, m_IppiRoiSize, m_nIppiStep , srcRoi, pDst, m_nIppiStep, m_IppiRoiSize, dResizeX, dResizeY, dfCenterX, dfCenterY, IPPI_INTER_NN);
        if ( Frame > 0.5 )
            ippiMirror_8u_AC4IR(pDst, m_nIppiStep, m_IppiRoiSize, ippAxsVertical);
    }

	// restore masked pixels
	if (m_bMask)
	{
		for ( int nIndex = 0; nIndex < m_nSize; nIndex += 4)
		{
			// check for pixel mask
			if (m_pBackup[nIndex + 3])
			{
				// swap pixels
				m_pPixels[nIndex + 2] = m_pBackup[nIndex + 2];
				m_pPixels[nIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nIndex + 0] = m_pBackup[nIndex + 0];
			}
		}
	}

#endif

	// finalize effect
    EndEffect();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformPerspective(double Frame, VARIANT_BOOL Vertical, double Angle)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Angle = Frame*min(max(-90, Angle), 90);

	// check for simpliest case
	if (fabs(Angle) < 0.001)
		return S_OK;

	// begin effect
	BeginEffect();

	// clear image
	ClearImage(m_pPixels, m_nSize);

#ifdef NOT_OPTIM

	// check for transform center line (vertical or horizontal)
	if (Vertical == VARIANT_TRUE)
	{
		// variables
		int nY1, nY2, nX1, nX2;
		int nRY1, nRY2;

		// compute resulting x coordinates
		nX1 = (int)(0.5*m_nWidth*fabs(Angle)/90);
		nX2 = m_nWidth - 1 - nX1;

		// compute resulting y coordinates
		nY1 = (int)(0.25*m_nHeight*fabs(Angle)/90);
		nY2 = m_nHeight - 1 - nY1;

		// clamp to image ranges
		nX1 = min(max(nX1, 0), m_nWidth - 2);
		nX2 = min(max(nX2, nX1 + 1), m_nWidth - 1);
		nY1 = min(max(nY1, 0), m_nHeight - 2);
		nY2 = min(max(nY2, nY1 + 1), m_nHeight - 1);

		// variables
		int nX, nY, nIndex;
		int nResultX, nResultY, nResultIndex;

		// transform image
		for (nX = 0; nX < m_nWidth; ++nX)
		{
			// compute resulting x coordinate
			nResultX = nX1 + (nX2 - nX1)*nX/(m_nWidth - 1);

			// compute y coordinates ranges
			if (Angle < 0)
			{
				nRY1 = nY1*nX/(m_nWidth - 1);
				nRY2 = m_nHeight - 1 - nRY1;
			}
			else
			{
				nRY1 = nY1*(m_nWidth - 1 - nX)/(m_nWidth - 1);
				nRY2 = m_nHeight - 1 - nRY1;
			}

			for (nY = 0; nY < m_nHeight; ++nY)
			{
				// compute resulting y coordinate
				nResultY = nRY1 + (nRY2 - nRY1)*nY/(m_nHeight - 1);

				// compute source pixel index
				nIndex = 4*(nX + nY*m_nWidth);

				// compute resulting pixel index
				nResultIndex = 4*(nResultX + nResultY*m_nWidth);

				// copy pixel info
				m_pPixels[nResultIndex + 0] = m_pBackup[nIndex + 0];
				m_pPixels[nResultIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nResultIndex + 2] = m_pBackup[nIndex + 2];
			}
		}
	}
	else
	{
		// variables
		int nY1, nY2, nX1, nX2;
		int nRX1, nRX2;

		// compute resulting x coordinates
		nX1 = (int)(0.25*m_nWidth*fabs(Angle)/90);
		nX2 = m_nWidth - 1 - nX1;

		// compute resulting y coordinates
		nY1 = (int)(0.5*m_nHeight*fabs(Angle)/90);
		nY2 = m_nHeight - 1 - nY1;

		// clamp to image ranges
		nX1 = min(max(nX1, 0), m_nWidth - 2);
		nX2 = min(max(nX2, nX1 + 1), m_nWidth - 1);
		nY1 = min(max(nY1, 0), m_nHeight - 2);
		nY2 = min(max(nY2, nY1 + 1), m_nHeight - 1);

		// variables
		int nX, nY, nIndex = 0;
		int nResultX, nResultY, nResultIndex;

		// transform image
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			// compute resulting y coordinate
			nResultY = nY1 + (nY2 - nY1)*nY/(m_nHeight - 1);

			// compute x coordinates ranges
			if (Angle < 0)
			{
				nRX1 = nX1*nY/(m_nHeight - 1);
				nRX2 = m_nWidth - 1 - nRX1;
			}
			else
			{
				nRX1 = nX1*(m_nHeight - 1 - nY)/(m_nHeight - 1);
				nRX2 = m_nWidth - 1 - nRX1;
			}

			for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
			{
				// compute resulting x coordinate
				nResultX = nRX1 + (nRX2 - nRX1)*nX/(m_nWidth - 1);

				// compute resulting pixel index
				nResultIndex = 4*(nResultX + nResultY*m_nWidth);

				// copy pixel info
				m_pPixels[nResultIndex + 0] = m_pBackup[nIndex + 0];
				m_pPixels[nResultIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nResultIndex + 2] = m_pBackup[nIndex + 2];
			}
		}
	}

#else

	// variables
	int nY1, nY2, nX1, nX2;
    Ipp8u* pSrc = (Ipp8u*)m_pBackup;
    Ipp8u* pDst = (Ipp8u*)m_pPixels;
	IppiRect srcRoi = {0, 0, m_IppiRoiSize.width, m_IppiRoiSize.height};
	double quadSrc[4][2], quadDst[4][2];
	quadSrc[0][0] = quadSrc[3][0] = 0;
	quadSrc[1][0] = quadSrc[2][0] = m_nWidth - 1;
	quadSrc[0][1] = quadSrc[1][1] = 0;
	quadSrc[2][1] = quadSrc[3][1] = m_nHeight - 1;

	// check for transform center line (vertical or horizontal)
	if (Vertical == VARIANT_TRUE)
	{
		// compute resulting x coordinates
		nX1 = (int)(0.5*m_nWidth*fabs(Angle)/90);
		nX2 = m_nWidth - 1 - nX1;

		// compute resulting y coordinates
		nY1 = (int)(0.25*m_nHeight*fabs(Angle)/90);
		nY2 = m_nHeight - 1 - nY1;

		quadDst[0][0] = quadDst[3][0] = nX1;
		quadDst[1][0] = quadDst[2][0] = nX2;
		
		if (Angle < 0)
		{
            quadDst[0][1] = 0;
			quadDst[3][1] = m_nHeight - 1;
			quadDst[1][1] = nY1;
			quadDst[2][1] = nY2;
		}
		else
		{
            quadDst[0][1] = nY1;
			quadDst[3][1] = nY2;
			quadDst[1][1] = 0;
			quadDst[2][1] = m_nHeight - 1;
		}
	}
	else
	{
		// compute resulting x coordinates
		nX1 = (int)(0.25*m_nWidth*fabs(Angle)/90);
		nX2 = m_nWidth - 1 - nX1;

		// compute resulting y coordinates
		nY1 = (int)(0.5*m_nHeight*fabs(Angle)/90);
		nY2 = m_nHeight - 1 - nY1;

		quadDst[0][1] = quadDst[1][1] = nY1;
		quadDst[2][1] = quadDst[3][1] = nY2;
		
		if (Angle > 0)
		{
            quadDst[0][0] = nX1;
			quadDst[1][0] = nX2;
			quadDst[2][0] = m_nWidth - 1;
			quadDst[3][0] = 0;
		}
		else
		{
            quadDst[0][0] = 0;
			quadDst[1][0] = m_nWidth - 1;
			quadDst[2][0] = nX2;
			quadDst[3][0] = nX1;
		}
	}

	// apply perspective correction
	ippiWarpPerspectiveQuad_8u_AC4R(pSrc, m_IppiRoiSize, m_nIppiStep, srcRoi, quadSrc, pDst, m_nIppiStep, srcRoi, quadDst, IPPI_INTER_NN);

#endif

	// finalize effect
	EndEffect();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformSkew(double Frame, VARIANT_BOOL Vertical, double Angle)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Angle = Frame*min(max(-90, Angle), 90);

	// check for simpliest case
	if (fabs(Angle) < 0.001)
		return S_OK;

	// begin effect
	BeginEffect();

	// clear image
	ClearImage(m_pPixels, m_nSize);

#ifdef NOT_OPTIM

	// check for transform direction
	if (Vertical == VARIANT_TRUE)
	{
		// variables
		int nY1, nY2, nX1, nX2;
		int nRY1, nRY2;

		// compute resulting x coordinates
		nX1 = (int)(0.5*m_nWidth*fabs(Angle)/90);
		nX2 = m_nWidth - 1 - nX1;

		// compute resulting y coordinates
		nY1 = (int)(0.5*m_nHeight*fabs(Angle)/90);
		nY2 = m_nHeight - 1 - nY1;

		// clamp to image ranges
		nX1 = min(max(nX1, 0), m_nWidth - 2);
		nX2 = min(max(nX2, nX1 + 1), m_nWidth - 1);
		nY1 = min(max(nY1, 0), m_nHeight - 2);
		nY2 = min(max(nY2, nY1 + 1), m_nHeight - 1);

		// variables
		int nX, nY, nIndex;
		int nResultX, nResultY, nResultIndex;

		// transform image
		for (nX = 0; nX < m_nWidth; ++nX)
		{
			// compute resulting x coordinate
			nResultX = nX1 + (nX2 - nX1)*nX/(m_nWidth - 1);

			// compute x coordinates ranges
			if (Angle < 0)
			{
				nRY1 = nY1*nX/(m_nWidth - 1);
				nRY2 = m_nHeight - 1 - nY1*(m_nWidth - 1 - nX)/(m_nWidth - 1);
			}
			else
			{
				nRY1 = nY1*(m_nWidth - 1 - nX)/(m_nWidth - 1);
				nRY2 = m_nHeight - 1 - nY1*nX/(m_nWidth - 1);
			}

			for (nY = 0; nY < m_nHeight; ++nY)
			{
				// compute resulting y coordinate
				nResultY = nRY1 + (nRY2 - nRY1)*nY/(m_nHeight - 1);

				// compute source pixel index
				nIndex = 4*(nX + nY*m_nWidth);

				// compute resulting pixel index
				nResultIndex = 4*(nResultX + nResultY*m_nWidth);

				// copy pixel info
				m_pPixels[nResultIndex + 0] = m_pBackup[nIndex + 0];
				m_pPixels[nResultIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nResultIndex + 2] = m_pBackup[nIndex + 2];
			}
		}
	}
	else
	{
		// variables
		int nY1, nY2, nX1, nX2;
		int nRX1, nRX2;

		// compute resulting x coordinates
		nX1 = (int)(0.5*m_nWidth*fabs(Angle)/90);
		nX2 = m_nWidth - 1 - nX1;

		// compute resulting y coordinates
		nY1 = (int)(0.5*m_nHeight*fabs(Angle)/90);
		nY2 = m_nHeight - 1 - nY1;

		// clamp to image ranges
		nX1 = min(max(nX1, 0), m_nWidth - 2);
		nX2 = min(max(nX2, nX1 + 1), m_nWidth - 1);
		nY1 = min(max(nY1, 0), m_nHeight - 2);
		nY2 = min(max(nY2, nY1 + 1), m_nHeight - 1);

		// variables
		int nX, nY, nIndex = 0;
		int nResultX, nResultY, nResultIndex;

		// transform image
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			// compute resulting y coordinate
			nResultY = nY1 + (nY2 - nY1)*nY/(m_nHeight - 1);

			// compute x coordinates ranges
			if (Angle < 0)
			{
				nRX1 = nX1*nY/(m_nHeight - 1);
				nRX2 = m_nWidth - 1 - nX1*(m_nHeight - 1 - nY)/(m_nHeight - 1);
			}
			else
			{
				nRX1 = nX1*(m_nHeight - 1 - nY)/(m_nHeight - 1);
				nRX2 = m_nWidth - 1 - nX1*nY/(m_nHeight - 1);
			}

			for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
			{
				// compute resulting x coordinate
				nResultX = nRX1 + (nRX2 - nRX1)*nX/(m_nWidth - 1);

				// compute resulting pixel index
				nResultIndex = 4*(nResultX + nResultY*m_nWidth);

				// copy pixel info
				m_pPixels[nResultIndex + 0] = m_pBackup[nIndex + 0];
				m_pPixels[nResultIndex + 1] = m_pBackup[nIndex + 1];
				m_pPixels[nResultIndex + 2] = m_pBackup[nIndex + 2];
			}
		}
	}

#else

	// variables
	int nY1, nY2, nX1, nX2;
    Ipp8u* pSrc = (Ipp8u*)m_pBackup;
    Ipp8u* pDst = (Ipp8u*)m_pPixels;
	IppiRect srcRoi = {0, 0, m_IppiRoiSize.width, m_IppiRoiSize.height};
	double quadSrc[4][2], quadDst[4][2];
	quadSrc[0][0] = quadSrc[3][0] = 0;
	quadSrc[1][0] = quadSrc[2][0] = m_nWidth - 1;
	quadSrc[0][1] = quadSrc[1][1] = 0;
	quadSrc[2][1] = quadSrc[3][1] = m_nHeight - 1;

	// compute resulting x coordinates
	nX1 = (int)(0.5*m_nWidth*fabs(Angle)/90);
	nX2 = m_nWidth - 1 - nX1;

	// compute resulting y coordinates
	nY1 = (int)(0.5*m_nHeight*fabs(Angle)/90);
	nY2 = m_nHeight - 1 - nY1;

	// check for transform direction
	if (Vertical == VARIANT_TRUE)
	{
		quadDst[0][0] = quadDst[3][0] = nX1;
		quadDst[1][0] = quadDst[2][0] = nX2;
		
		if (Angle > 0)
		{
            quadDst[0][1] = nY1;
			quadDst[1][1] = 0;
			quadDst[2][1] = nY2;
			quadDst[3][1] = m_nHeight - 1;
		}
		else
		{
            quadDst[0][1] = 0;
			quadDst[1][1] = nY1;
			quadDst[2][1] = m_nHeight - 1;
			quadDst[3][1] = nY2;
		}
	}
	else
	{
		quadDst[0][1] = quadDst[1][1] = nY1;
		quadDst[2][1] = quadDst[3][1] = nY2;
		
		if (Angle > 0)
		{
            quadDst[0][0] = nX1;
			quadDst[1][0] = m_nWidth - 1;
			quadDst[2][0] = nX2;
			quadDst[3][0] = 0;
		}
		else
		{
            quadDst[0][0] = 0;
			quadDst[1][0] = nX2;
			quadDst[2][0] = m_nWidth - 1;
			quadDst[3][0] = nX1;
		}
	}

	// apply skew correction
	ippiWarpAffineQuad_8u_AC4R(pSrc, m_IppiRoiSize, m_nIppiStep, srcRoi, quadSrc, pDst, m_nIppiStep, srcRoi, quadDst, IPPI_INTER_NN);

#endif

	// finalize effect
	EndEffect();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformShift(double Frame, long X, long Y)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	X = (int)(X*Frame);
	Y = (int)(-Y*Frame);

	// check for simpliest case
	if (X == 0 && Y == 0)
		return S_OK;
	if (X >= m_nWidth || Y >= m_nHeight || X + m_nWidth - 1 < 0 || Y + m_nHeight - 1 < 0)
	{
		ClearImage(m_pPixels, m_nSize);
		return S_OK;
	}

#ifdef NOT_OPTIM

	// compose background color
	BYTE ColorR = GetRValue(m_nBackColor);
	BYTE ColorG = GetGValue(m_nBackColor);
	BYTE ColorB = GetBValue(m_nBackColor);
	BYTE ColorA = GetAValue(m_nBackColor);

	// variables
	int nX, nY, nIndex;
	int nSourceX, nSourceY, nSourceIndex;

	// offset image
	if (X >= 0 && Y >= 0)
	{
		for (nY = m_nHeight - 1; nY >= 0; --nY)
		{
			// compute y coodrinate
			nSourceY = nY - Y;

			for (nX = m_nWidth - 1; nX >= 0; --nX)
			{
				// compute x coordinate
				nSourceX = nX - X;

				// comute current pixel index
				nIndex = 4*(nX + nY*m_nWidth);

				// clear pixel
				m_pPixels[nIndex + 3] = ColorA;
				m_pPixels[nIndex + 2] = ColorR;
				m_pPixels[nIndex + 1] = ColorG;
				m_pPixels[nIndex + 0] = ColorB;

				// check for valid pixel index
				if (nSourceX < 0 || nSourceX >= m_nWidth || nSourceY < 0 || nSourceY >= m_nHeight)
					continue;

				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				// copy pixel
				m_pPixels[nIndex + 2] = m_pPixels[nSourceIndex + 2];
				m_pPixels[nIndex + 1] = m_pPixels[nSourceIndex + 1];
				m_pPixels[nIndex + 0] = m_pPixels[nSourceIndex + 0];
			}
		}
	}
	else if (X < 0 && Y >= 0)
	{
		for (nY = m_nHeight - 1; nY >= 0; --nY)
		{
			// compute y coodrinate
			nSourceY = nY - Y;

			for (nX = 0; nX < m_nWidth; ++nX)
			{
				// compute x coordinate
				nSourceX = nX - X;

				// comute current pixel index
				nIndex = 4*(nX + nY*m_nWidth);

				// clear pixel
				m_pPixels[nIndex + 2] = ColorR;
				m_pPixels[nIndex + 1] = ColorG;
				m_pPixels[nIndex + 0] = ColorB;

				// check for valid pixel index
				if (nSourceX < 0 || nSourceX >= m_nWidth || nSourceY < 0 || nSourceY >= m_nHeight)
					continue;

				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				// copy pixel
				m_pPixels[nIndex + 2] = m_pPixels[nSourceIndex + 2];
				m_pPixels[nIndex + 1] = m_pPixels[nSourceIndex + 1];
				m_pPixels[nIndex + 0] = m_pPixels[nSourceIndex + 0];
			}
		}
	}
	if (X >= 0 && Y < 0)
	{
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			// compute y coodrinate
			nSourceY = nY - Y;

			for (nX = m_nWidth - 1; nX >= 0; --nX)
			{
				// compute x coordinate
				nSourceX = nX - X;

				// comute current pixel index
				nIndex = 4*(nX + nY*m_nWidth);

				// clear pixel
				m_pPixels[nIndex + 2] = ColorR;
				m_pPixels[nIndex + 1] = ColorG;
				m_pPixels[nIndex + 0] = ColorB;

				// check for valid pixel index
				if (nSourceX < 0 || nSourceX >= m_nWidth || nSourceY < 0 || nSourceY >= m_nHeight)
					continue;

				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				// copy pixel
				m_pPixels[nIndex + 2] = m_pPixels[nSourceIndex + 2];
				m_pPixels[nIndex + 1] = m_pPixels[nSourceIndex + 1];
				m_pPixels[nIndex + 0] = m_pPixels[nSourceIndex + 0];
			}
		}
	}
	else if (X < 0 && Y < 0)
	{
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			// compute y coodrinate
			nSourceY = nY - Y;

			for (nX = 0; nX < m_nWidth; ++nX)
			{
				// compute x coordinate
				nSourceX = nX - X;

				// comute current pixel index
				nIndex = 4*(nX + nY*m_nWidth);

				// clear pixel
				m_pPixels[nIndex + 2] = ColorR;
				m_pPixels[nIndex + 1] = ColorG;
				m_pPixels[nIndex + 0] = ColorB;

				// check for valid pixel index
				if (nSourceX < 0 || nSourceX >= m_nWidth || nSourceY < 0 || nSourceY >= m_nHeight)
					continue;

				// compute source pixel index
				nSourceIndex = 4*(nSourceX + nSourceY*m_nWidth);

				// copy pixel
				m_pPixels[nIndex + 2] = m_pPixels[nSourceIndex + 2];
				m_pPixels[nIndex + 1] = m_pPixels[nSourceIndex + 1];
				m_pPixels[nIndex + 0] = m_pPixels[nSourceIndex + 0];
			}
		}
	}

#else

	// begin effect
	BeginEffect();

	// clear image
	ClearImage(m_pPixels, m_nSize);

    Ipp8u* pSrc = (Ipp8u*)m_pBackup;
    Ipp8u* pDst = (Ipp8u*)m_pPixels;
	IppiSize dstRoi = {m_IppiRoiSize.width - abs(X), m_IppiRoiSize.height - abs(Y)};

	if (X >= 0 && Y >= 0)
		ippiCopy_8u_AC4R(pSrc, m_nIppiStep, pDst + m_nIppiStep*Y + X*4, m_nIppiStep, dstRoi);
	else if (X < 0 && Y >= 0)
		ippiCopy_8u_AC4R(pSrc + abs(X)*4, m_nIppiStep, pDst + m_nIppiStep*Y, m_nIppiStep, dstRoi);
	else if (X >= 0 && Y < 0)
		ippiCopy_8u_AC4R(pSrc + m_nIppiStep*abs(Y), m_nIppiStep, pDst + abs(X)*4, m_nIppiStep, dstRoi);
	else if (X < 0 && Y < 0)
		ippiCopy_8u_AC4R(pSrc + m_nIppiStep*abs(Y) + abs(X)*4, m_nIppiStep, pDst, m_nIppiStep, dstRoi);

	// finalize effect
	EndEffect();

#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformRotate(double Frame, double Angle)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Angle = Frame*min(max(-360, Angle), 360);

	// check for simpliest case
	if (fabs(Angle) < 0.001 || Angle > 359.999 || Angle < -359.999)
		return S_OK;

#ifdef NOT_OPTIM

	// begin effect
	BeginEffect();

	// clear image
	ClearImage(m_pPixels, m_nSize);

	// compute rotational angle in radians
	double dNormalAngle = 3.14159265359*Frame*Angle/180.0;

	// variables
	double dCenterX = 0.5*m_nWidth;
	double dCenterY = 0.5*m_nHeight;
	double dOldAngle, dOldRadius;
	double dLTx, dLTy, dRTx, dRTy, dLBx, dLBy, dRBx, dRBy;

	// compute size of the image after rotation
	GetRadiusAndAngle(0, 0, dCenterX, dCenterY, dOldRadius, dOldAngle);
		dLTx = dCenterX + dOldRadius*cos(dNormalAngle + dOldAngle);
		dLTy = dCenterY + dOldRadius*sin(dNormalAngle + dOldAngle);
	GetRadiusAndAngle(m_nWidth - 1, 0, dCenterX, dCenterY, dOldRadius, dOldAngle);
		dRTx = dCenterX + dOldRadius*cos(dNormalAngle + dOldAngle);
		dRTy = dCenterY + dOldRadius*sin(dNormalAngle + dOldAngle);
	GetRadiusAndAngle(m_nWidth - 1, m_nHeight - 1, dCenterX, dCenterY, dOldRadius, dOldAngle);
		dRBx = dCenterX + dOldRadius*cos(dNormalAngle + dOldAngle);
		dRBy = dCenterY + dOldRadius*sin(dNormalAngle + dOldAngle);
	GetRadiusAndAngle(0, m_nHeight - 1, dCenterX, dCenterY, dOldRadius, dOldAngle);
		dLBx = dCenterX + dOldRadius*cos(dNormalAngle + dOldAngle);
		dLBy = dCenterY + dOldRadius*sin(dNormalAngle + dOldAngle);

	double dLeft	= min(min(dLTx, dRTx), min(dLBx, dRBx));
	double dRight	= max(max(dLTx, dRTx), max(dLBx, dRBx));
	double dTop		= min(min(dLTy, dRTy), min(dLBy, dRBy));
	double dBottom	= max(max(dLTy, dRTy), max(dLBy, dRBy));

	int nNewWidth = (int)(dRight - dLeft);
	int nNewHeight = (int)(dBottom - dTop);

	// compute normalization koefficient
	double dNormalize = max((double)nNewWidth/m_nWidth, (double)nNewHeight/m_nHeight);

	// variables
	int nX, nY, nIndex = 0;
	double dSourceX, dSourceY;

	// compose background color
	BYTE ColorR = GetRValue(m_nBackColor);
	BYTE ColorG = GetGValue(m_nBackColor);
	BYTE ColorB = GetBValue(m_nBackColor);
	BYTE ColorA = GetAValue(m_nBackColor);

	// process image
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute pixel angle and radius
			GetRadiusAndAngle(nX, nY, dCenterX, dCenterY, dOldRadius, dOldAngle);

			// compute old pixel angle and radius
			dSourceX = dCenterX + dNormalize*dOldRadius*cos(dNormalAngle + dOldAngle);
			dSourceY = dCenterY + dNormalize*dOldRadius*sin(dNormalAngle + dOldAngle);

			// compute pixel color
			m_pPixels[nIndex + 0] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 0, 4, ColorB);
			m_pPixels[nIndex + 1] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 1, 4, ColorG);
			m_pPixels[nIndex + 2] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 2, 4, ColorR);
			m_pPixels[nIndex + 3] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 3, 4, ColorA);
		}
	}

#else

	Ipp8u* pDst = (Ipp8u*)m_pPixels;
	
	// check whether we flip image on 180 degrees
	if (fabs(fabs(Angle) - 180) < 0.001)
	{
		ippiMirror_8u_AC4IR(pDst, m_nIppiStep, m_IppiRoiSize, ippAxsBoth);
		return S_OK;
	}

	// begin effect
	BeginEffect();

	// clear image
	ClearImage(m_pPixels, m_nSize);

	// variables
    Ipp8u* pSrc = (Ipp8u*)m_pBackup;
    IppiRect srcRoi = {0, 0, m_IppiRoiSize.width , m_IppiRoiSize.height};
	double dfCenterX = 0.5 * m_nWidth;
	double dfCenterY = 0.5 * m_nHeight;
	double quad[4][2];

	ippiGetRotateQuad(srcRoi, quad, Angle, 0, 0);

	double dLeft	= min(min(quad[0][0], quad[1][0]), min(quad[2][0], quad[3][0]));
	double dRight	= max(max(quad[0][0], quad[1][0]), max(quad[2][0], quad[3][0]));
	double dTop		= min(min(quad[0][1], quad[1][1]), min(quad[2][1], quad[3][1]));
	double dBottom	= max(max(quad[0][1], quad[1][1]), max(quad[2][1], quad[3][1]));

	double dResize = 1.0/max((int)(dRight - dLeft)/(double)m_nWidth, (int)(dBottom - dTop)/(double)m_nHeight);
	
	int nStep = 4*m_nWidth;
	Ipp8u* pTmp = new Ipp8u[4*m_nWidth*m_nHeight];

	if (pTmp)
	{
		ClearImage((BYTE*)pTmp, 4*m_nWidth*m_nHeight);

		ippiResizeCenter_8u_AC4R(pSrc, m_IppiRoiSize, m_nIppiStep, srcRoi, pTmp, nStep, m_IppiRoiSize, dResize, dResize, dfCenterX, dfCenterY, IPPI_INTER_LINEAR);
		ippiRotateCenter_8u_AC4R(pTmp, m_IppiRoiSize, nStep, srcRoi, pDst, m_nIppiStep, srcRoi, Angle, dfCenterX, dfCenterY, IPPI_INTER_LINEAR);
		
		delete[] pTmp;
	}

#endif

	// finalize effect
	EndEffect();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformResample(double Frame, long Times)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Times = min(max(1, Times), max(1, min(m_nWidth, m_nHeight)));

	if (fabs(Frame) < 0.001 || Times < 2)
		return S_OK;

	// begin effect
	BeginEffect();

	// compute koeff
	double dKoef = 1.0 + Frame*(Times - 1);

#ifdef NOT_OPTIM

	// variables
	double dCenterX = 0.5*m_nWidth;
	double dCenterY = 0.5*m_nHeight;

	// variables
	int nX, nY, nIndex = 0;
	double dSourceX, dSourceY;

	// process image
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute source pixel coordinates
			dSourceX = dCenterX + (nX - dCenterX)*dKoef;
			dSourceY = dCenterY + (nY - dCenterY)*dKoef;

			// correct coordinates
			if (dSourceX < 0)
			{
				while (dSourceX < 0)
					dSourceX += m_nWidth - 1;
			}
			if (dSourceX > m_nWidth - 1)
			{
				while (dSourceX > m_nWidth - 1)
					dSourceX -= m_nWidth - 1;
			}
			if (dSourceY < 0)
			{
				while (dSourceY < 0)
					dSourceY += m_nHeight - 1;
			}
			if (dSourceY > m_nHeight - 1)
			{
				while (dSourceY > m_nHeight - 1)
					dSourceY -= m_nHeight - 1;
			}

			// compute pixel color
			m_pPixels[nIndex + 0] = GetPointNearest(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 0, 4, 0);
			m_pPixels[nIndex + 1] = GetPointNearest(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 1, 4, 0);
			m_pPixels[nIndex + 2] = GetPointNearest(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 2, 4, 0);
		}
	}

#else

	// variables
	double dResize = 1.0 / dKoef;
    IppiSize ResizedRoi = {(int)(m_IppiRoiSize.width * dResize) , (int)(m_IppiRoiSize.height * dResize)};
	Ipp8u* pSrc = (Ipp8u*)m_pBackup;
    Ipp8u* pDst = (Ipp8u*)m_pPixels;
	IppiRect srcRoi = {0, 0, m_IppiRoiSize.width, m_IppiRoiSize.height};
	int nSrcOffset, nDstOffset;
	int nTileStep = ResizedRoi.width * 4;
	int nFullTilesX = 2 * ((m_nWidth - ResizedRoi.width)/(2 * ResizedRoi.width)) + 1;
	int nFullTilesY = 2 * ((m_nHeight - ResizedRoi.height)/(2 * ResizedRoi.height)) + 1;
	int nXBottom, nXTop, nYBottom, nYTop;

	nXBottom = nXTop = (m_nWidth - nFullTilesX * ResizedRoi.width);
	nYBottom = nYTop = (m_nHeight - nFullTilesY * ResizedRoi.height);
	nXTop /= 2; nYTop /= 2;
	nXBottom -= nXTop; 	nYBottom -= nYTop;

	nDstOffset = nYTop * m_nIppiStep + 4 * nXTop;

	ippiResize_8u_AC4R(pSrc, m_IppiRoiSize, m_nIppiStep , srcRoi, pDst+nDstOffset, m_nIppiStep, ResizedRoi, dResize, dResize, IPPI_INTER_NN);
	nSrcOffset = nDstOffset;

    for (int i = 1; i < nFullTilesX; ++i)
	{
		nDstOffset += nTileStep;
		ippiCopy_8u_AC4R(pDst + nSrcOffset, m_nIppiStep, pDst + nDstOffset, m_nIppiStep, ResizedRoi);
	}

	if (nXBottom > 0)
	{	
		nDstOffset += nTileStep;
		ResizedRoi.width = nXBottom;
		ippiCopy_8u_AC4R(pDst + nSrcOffset, m_nIppiStep, pDst + nDstOffset, m_nIppiStep, ResizedRoi);
	}
	if (nXTop > 0)
	{
		nDstOffset = nSrcOffset - 4 * nXTop;
		nSrcOffset = nSrcOffset + nTileStep - 4 * nXTop;
		ResizedRoi.width = nXTop;
		ippiCopy_8u_AC4R(pDst + nSrcOffset, m_nIppiStep, pDst + nDstOffset, m_nIppiStep, ResizedRoi);
	}
	else
		nDstOffset = nSrcOffset;
	
	nTileStep = ResizedRoi.height * m_nIppiStep;
	nSrcOffset = nDstOffset;
	ResizedRoi.width = m_IppiRoiSize.width;

    for (int i = 1; i < nFullTilesY; ++i)
	{
		nDstOffset += nTileStep;
		ippiCopy_8u_AC4R(pDst + nSrcOffset, m_nIppiStep, pDst + nDstOffset, m_nIppiStep, ResizedRoi);
	}

	if (nYBottom > 0)
	{	
		nDstOffset += nTileStep;
		ResizedRoi.height = nYBottom;
		ippiCopy_8u_AC4R(pDst + nSrcOffset, m_nIppiStep, pDst + nDstOffset, m_nIppiStep, ResizedRoi);
	}
	if (nYTop > 0)
	{
		nDstOffset = 0;
		ResizedRoi.height = nYTop;
		nSrcOffset = nSrcOffset + nTileStep - m_nIppiStep * nYTop;
		ippiCopy_8u_AC4R(pDst + nSrcOffset, m_nIppiStep, pDst + nDstOffset, m_nIppiStep, ResizedRoi);
	}

#endif

	// finalize effect
	EndEffect();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformZoom(double Frame, long Zoom)
{	 
	// check for valid image
	if (!IsValid())
		return S_FALSE;
	
	// clamp values
	Frame = Clamp(Frame);
	Zoom = min(max(-100, Zoom), 100);

	//// compute koeff
	//double dKoef = 1;

	// compute zooming koefficient
	//if (Zoom == 0 || fabs(Frame) < 0.001)
	//	return S_OK;
	//else if (Zoom < 0)
	//	dKoef = 1.0 - min(0.999, fabs(Frame*Zoom/100.0));
	//else
	//	dKoef = 1.0 + 0.5*min(m_nWidth, m_nHeight)*pow(Frame*Zoom/100.0, 2);

	// begin effect
	BeginEffect();

#ifdef NOT_OPTIM

	// variables
	double dCenterX = 0.5*m_nWidth;
	double dCenterY = 0.5*m_nHeight;

	// variables
	int nX, nY, nIndex = 0;
	double dSourceX, dSourceY;

	// compose background color
	BYTE ColorR = GetRValue(m_nBackColor);
	BYTE ColorG = GetGValue(m_nBackColor);
	BYTE ColorB = GetBValue(m_nBackColor);

	// process image
	for (nY = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			// compute source pixel coordinates
			dSourceX = dCenterX + (nX - dCenterX)/dKoef;
			dSourceY = dCenterY + (nY - dCenterY)/dKoef;

			// compute pixel color
			m_pPixels[nIndex + 0] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 0, 4, ColorB);
			m_pPixels[nIndex + 1] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 1, 4, ColorG);
			m_pPixels[nIndex + 2] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 2, 4, ColorR);
		}
	}

#else

    // clear image
    ClearImage(m_pPixels, m_nSize);

	double dScale = Zoom * Frame / 100;
	if( Zoom > 0 )
		dScale *= 10 * Frame; // сделано для "мягкости" эффекта

	dScale += 1;

	if( dScale < 0.00001  )
	{
		// finalize effect
		EndEffect();

		// apply registration
		ApplyRegister();

		return S_OK;
	}

	// !!! не применять ippiResizeCenter(). Она глючит!

	double dOffsetX = m_dAdvancedZoomOffsetX;
	double dOffsetY = m_dAdvancedZoomOffsetY;
	int nWidth  = m_nWidth;
	int nHeight = m_nHeight;

	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];

	if( pxMap && pyMap )
	{
		dOffsetX = nWidth  / 2.0 * (dScale - 1) - dOffsetX;
		dOffsetY = nHeight / 2.0 * (dScale - 1) - dOffsetY;
		dScale = 1 / dScale;

		Ipp32f* pMapCurX = pxMap;
		Ipp32f* pMapCurY = pyMap;

		for( int nRow = 0; nRow < nHeight; ++nRow )
		{
			for( int nPos = 0; nPos < nWidth; ++nPos )
			{
				*pMapCurX++ = Ipp32f((nPos + dOffsetX) * dScale);
				*pMapCurY++ = Ipp32f((nRow + dOffsetY) * dScale);
			}
		}

		IppiSize ImageSize = {nWidth, nHeight};
		IppiRect ImageRect = {0, 0, nWidth, nHeight};

		ippiRemap_8u_AC4R( m_pBackup, ImageSize, 4*nWidth, ImageRect, 
			pxMap, sizeof(Ipp32f)*nWidth, 
			pyMap, sizeof(Ipp32f)*nWidth, 
			m_pPixels, 4*nWidth, ImageSize, IPPI_INTER_LINEAR);
	}
	
	if( pxMap )
		delete [] pxMap;
	
	if( pyMap )
		delete [] pyMap;
	
#endif

	// finalize effect
	EndEffect();

	// apply registration
	ApplyRegister();

	// all ok		
	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformMirror(double Frame, double Level, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Level = min(max(0, Level), 100);

	if (fabs(Frame) < 0.001 || fabs(Level) < 0.001 )
		return S_OK;

#ifdef NOT_OPTIM // TODO: disabled optimization due to access violation

	// begin effect
	BeginEffect();

	// mirror image
	if (Type == c_nMirrorVerticalBottom || Type == c_nMirrorVerticalTop)
		MirrorVertical(Frame, Type, Level);
	else if (Type == c_nMirrorHorizontalLeft || Type == c_nMirrorHorizontalRight)
		MirrorHorizontal(Frame, Type, Level);

	// finalize effect
	EndEffect();

#else

	// variables
    Ipp8u* pDst = (Ipp8u*)m_pPixels;
    IppiRect srcRoi = {0, 0, m_IppiRoiSize.width, m_IppiRoiSize.height};
	IppiSize dstRoiSize = m_IppiRoiSize;
	int nMirror;
	double dResize = 0.01*Level*Frame;
	
	if (Type == c_nMirrorVerticalTop)
	{
		nMirror = (int)(dResize*(m_nHeight - 2));
		srcRoi.height -= nMirror;
		dstRoiSize.height = nMirror;
		dResize = (double)dstRoiSize.height/srcRoi.height;

		ippiResize_8u_AC4R(pDst, m_IppiRoiSize, m_nIppiStep, srcRoi, pDst + srcRoi.height * m_nIppiStep, m_nIppiStep, dstRoiSize, 1, dResize, IPPI_INTER_LINEAR);

		pDst = pDst + srcRoi.height * m_nIppiStep;
        ippiMirror_8u_AC4IR(pDst, m_nIppiStep, dstRoiSize, ippAxsHorizontal);	

		IppiSize MaskSize = {10, 3};
		IppiPoint anchor = {MaskSize.width/2, 1};
        dstRoiSize.width -= (MaskSize.width - 1);
		dstRoiSize.height -= (MaskSize.height - 1);
        ippiFilterBox_8u_AC4IR(pDst + anchor.y*m_nIppiStep + anchor.x*4 , m_nIppiStep, dstRoiSize, MaskSize, anchor);
	}
	else if (Type == c_nMirrorVerticalBottom)
	{
		nMirror = (int)(dResize*(m_nHeight - 2));
		srcRoi.height -= nMirror;
		srcRoi.y = nMirror;
		dstRoiSize.height = nMirror;
		dResize = (double)dstRoiSize.height/srcRoi.height;

		ippiResize_8u_AC4R(pDst, m_IppiRoiSize, m_nIppiStep, srcRoi, pDst, m_nIppiStep, dstRoiSize, 1, dResize, IPPI_INTER_LINEAR);

		ippiMirror_8u_AC4IR( pDst, m_nIppiStep, dstRoiSize, ippAxsHorizontal );		
	
		IppiSize MaskSize = {10, 3};
		IppiPoint anchor = {MaskSize.width/2, 1};
        dstRoiSize.width -= (MaskSize.width - 1);
		dstRoiSize.height -= (MaskSize.height - 1);
        ippiFilterBox_8u_AC4IR(pDst + anchor.y*m_nIppiStep + anchor.x*4 , m_nIppiStep, dstRoiSize, MaskSize, anchor);
	}
	if (Type == c_nMirrorHorizontalRight)
	{
		nMirror = (int)(dResize*(m_nWidth - 2));
		srcRoi.width -= nMirror;
		dstRoiSize.width = nMirror;
		dResize = (double)dstRoiSize.width/srcRoi.width;

		ippiResize_8u_AC4R(pDst, m_IppiRoiSize, m_nIppiStep, srcRoi, pDst + srcRoi.width * 4, m_nIppiStep, dstRoiSize, dResize, 1, IPPI_INTER_LINEAR);
		
		pDst += srcRoi.width * 4;
        ippiMirror_8u_AC4IR(pDst, m_nIppiStep, dstRoiSize, ippAxsVertical);

		IppiSize MaskSize = {3, 10};
		IppiPoint anchor= {1, MaskSize.height / 2};
        dstRoiSize.width -= (MaskSize.width - 1);
		dstRoiSize.height -= (MaskSize.height - 1);
        ippiFilterBox_8u_AC4IR(pDst  + anchor.y*m_nIppiStep + anchor.x*4, m_nIppiStep, dstRoiSize, MaskSize, anchor);
	}
	else if (Type == c_nMirrorHorizontalLeft)
	{
		nMirror = (int)(dResize*(m_nWidth - 2));
		srcRoi.width -= nMirror;
		srcRoi.x = nMirror;
		dstRoiSize.width = nMirror;
		dResize = (double)dstRoiSize.width/srcRoi.width;

        ippiResize_8u_AC4R(pDst, m_IppiRoiSize, m_nIppiStep , srcRoi, pDst, m_nIppiStep, dstRoiSize, dResize, 1, IPPI_INTER_LINEAR);

		ippiMirror_8u_AC4IR(pDst, m_nIppiStep, dstRoiSize, ippAxsVertical);

		IppiSize MaskSize = {3, 10};
		IppiPoint anchor= {1, MaskSize.height/2};
        dstRoiSize.width -= (MaskSize.width - 1);
		dstRoiSize.height -= (MaskSize.height - 1);
		ippiFilterBox_8u_AC4IR(pDst + anchor.y*m_nIppiStep + anchor.x*4, m_nIppiStep, dstRoiSize, MaskSize, anchor);
	}

#endif

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformResize(BYTE* pData, long lWidth, long lHeight,  long Color, long Type)
{
	int NewHeight = lHeight;
	int NewWidth = lWidth;

    if (NewHeight < 2 || NewWidth < 2) 
		return S_FALSE;
	if (m_nWidth == NewWidth && m_nHeight == NewHeight)
	{
		m_oMemoryUtils.memcpy(pData, m_pPixels, 4*NewWidth*NewHeight);
		return S_OK;
	}

#ifdef NOT_OPTIM

	return S_FALSE;

#else

	// variables
	BYTE* pResized = pData;
	double dScaleX = 1.0;
	double dScaleY = 1.0;
	IppiRect srcRect = {0, 0, m_nWidth, m_nHeight};

	if (Type == c_nResizeStretchNearest || Type == c_nResizeStretchBillinear || Type == c_nResizeStretchBicubic)
	{
		IppiSize dstRoi = {NewWidth, NewHeight};

		dScaleX = NewWidth/(double)m_nWidth;
		dScaleY = NewHeight/(double)m_nHeight;
			
		if (Type == c_nResizeStretchNearest)
			ippiResize_8u_AC4R(m_pPixels, m_IppiRoiSize, m_nIppiStep, srcRect, pResized, 4*NewWidth, dstRoi, dScaleX, dScaleY, IPPI_INTER_NN);
		else
			ippiResize_8u_AC4R(m_pPixels, m_IppiRoiSize, m_nIppiStep, srcRect, pResized, 4*NewWidth, dstRoi, dScaleX, dScaleY, IPPI_INTER_LINEAR);

		int x, y;
		double dSourceX, dSourceY;

		for (y = 0; y < NewHeight; ++y)
		{
			for (x = 0; x < NewWidth; ++x, pResized += 4)
			{
				// compute old pixel angle and radius
				dSourceX = 0.5*m_nWidth - (0.5*NewWidth - x)/dScaleX;
				dSourceY = 0.5*m_nHeight - (0.5*NewHeight - y)/dScaleY;

				// compute pixel color
				if (Type == c_nResizeStretchNearest)
				{
					pResized[3] = GetPointNearest(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 3, 4, 255);
				}
				else
				{
					pResized[3] = GetPointBillinear(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 3, 4, 255);
				}
			}
		}
	}
	else if (Type == c_nResizeShrinkNearest || Type == c_nResizeShrinkBillinear || Type == c_nResizeShrinkBicubic)
	{
		IppiSize dstRoi = {NewWidth, NewHeight};

		// clear output image first
		ClearImage((BYTE*)pResized, 4*NewWidth*NewHeight);

		// TODO: variables
		dScaleX = NewWidth/(double)m_nWidth;
		dScaleY = NewHeight/(double)m_nHeight;

		double dZoom = min(1.0, min(dScaleX, dScaleY));

		dScaleX = dZoom;
		dScaleY = dZoom;

		if (m_dAspect != 1)
		{
			double dW = m_dAspect * m_nHeight;
			dScaleX *= (dW / m_nWidth);

			int nNewWidthAsp = dScaleX * m_nWidth;
			if (nNewWidthAsp > NewWidth)
			{
				double dCorrect = (double)NewWidth / nNewWidthAsp;
				dScaleX *= dCorrect;
				dScaleY *= dCorrect;
			}
		}

		Ipp8u *pDst = pResized;
		if (NewHeight > dScaleY*m_nHeight)
			pDst += 4*NewWidth*(int)((NewHeight - dScaleY*m_nHeight)/2);
		if (NewWidth > dScaleX*m_nWidth)
			pDst += 4*(int)((NewWidth - dScaleX*m_nWidth)/2);

		// resize image from center simulating zooming effect
		if (Type == c_nResizeShrinkNearest)
			ippiResize_8u_AC4R((Ipp8u*)m_pPixels, m_IppiRoiSize, m_nIppiStep, srcRect, pDst, 4*NewWidth, dstRoi, dScaleX, dScaleY, IPPI_INTER_NN);
		else 
			ippiResize_8u_AC4R((Ipp8u*)m_pPixels, m_IppiRoiSize, m_nIppiStep, srcRect, pDst, 4*NewWidth, dstRoi, dScaleX, dScaleY, IPPI_INTER_LINEAR);

		int x, y;
		double dSourceX, dSourceY;
		// AlphaChannel
		for (y = 0; y < NewHeight; ++y)
		{
			for (x = 0; x < NewWidth; ++x, pResized += 4)
			{
				// compute old pixel angle and radius
				dSourceX = 0.5*m_nWidth - (0.5*NewWidth - x)/dScaleX;
				dSourceY = 0.5*m_nHeight - (0.5*NewHeight - y)/dScaleY;

				// compute pixel color
				if (Type == c_nResizeShrinkNearest)
				{
					pResized[3] = GetPointNearest(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 3, 4, 255);
				}
				else
				{
					pResized[3] = GetPointBillinear(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 3, 4, 255);
				}
			}
		}
	}
	else if (Type == c_nResizeCropNearest || Type == c_nResizeCropBillinear || Type == c_nResizeCropBicubic)
	{
		// variables
		dScaleX = NewWidth/(double)m_nWidth;
		dScaleY = NewHeight/(double)m_nHeight;

		double dZoom1 = min(dScaleX, dScaleY);
		double dZoom2 = max(dScaleX, dScaleY);
		double dZoom = dZoom1 + (dZoom2 - dZoom1)*m_nAdvancedCropZoom/100.0;

		// compose background color
		BYTE ColorR = GetRValue(m_nBackColor);
		BYTE ColorG = GetGValue(m_nBackColor);
		BYTE ColorB = GetBValue(m_nBackColor);
		BYTE ColorA = GetBValue(m_nBackColor);

		int x, y;
		double dSourceX, dSourceY;

		for (y = 0; y < NewHeight; ++y)
		{
			for (x = 0; x < NewWidth; ++x, pResized += 4)
			{
				// compute old pixel angle and radius
				dSourceX = 0.5*m_nWidth - (0.5*NewWidth - x)/dZoom;
				dSourceY = 0.5*m_nHeight - (0.5*NewHeight - y)/dZoom;

				// compute pixel color
				if (Type == c_nResizeCropNearest)
				{
					pResized[0] = GetPointNearest(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 0, 4, ColorB);
					pResized[1] = GetPointNearest(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 1, 4, ColorG);
					pResized[2] = GetPointNearest(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 2, 4, ColorR);
					pResized[3] = GetPointNearest(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 3, 4, ColorA);
				}
				else
				{
					pResized[0] = GetPointBillinear(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 0, 4, ColorB);
					pResized[1] = GetPointBillinear(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 1, 4, ColorG);
					pResized[2] = GetPointBillinear(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 2, 4, ColorR);
					pResized[3] = GetPointBillinear(dSourceX, dSourceY, m_pPixels, m_nWidth, m_nHeight, 3, 4, ColorA);
				}
			}
		}
	}

#endif
	
	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformResize(SAFEARRAY** Output, long Color, long Type)
{
	if (!Output || !*Output || !m_nWidth || !m_nHeight) 
		return S_FALSE;

	return TransformResize((BYTE*)((*Output)->pvData), (*Output)->rgsabound[1].cElements, (*Output)->rgsabound[0].cElements,  Color, Type);
}
STDMETHODIMP ImageTransform3::TransformResizeMedia(IUnknown** pInterface, long Color, long Type)
{
	if (!pInterface || !*pInterface || !m_nWidth || !m_nHeight) 
		return S_FALSE;

	ImageStudio::Core::MediaData oMediaData;
	
	if (!oMediaData.Create(*pInterface, FALSE) || !oMediaData.IsValidBGRA())
		return S_FALSE;

	// добавляем одну ссылку с тем, чтобы объект реально не "убился", при выходе за текущие скобки ))
	*pInterface = oMediaData.GetMediaData(TRUE);

	return TransformResize(oMediaData.GetBuffer(), oMediaData.GetWidth(), oMediaData.GetHeight(),  Color, Type);
}

STDMETHODIMP ImageTransform3::TransformResizeMedia2(long nNewWidth, long nNewHeight, IUnknown** pMediaData, long Color, long Type)
{
	if (!ImageStudioUtils::ByteArrayToMediaData(NULL, nNewWidth, nNewHeight, pMediaData))
		return S_FALSE;
	IUnknown* pCreatedMediaData = *pMediaData;

	TransformResizeMedia(pMediaData, Color, Type);

	if (*pMediaData != pCreatedMediaData)
		pCreatedMediaData->Release();

	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformRotateMedia(double Frame, double Angle, IUnknown** pMediaData)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Angle = Frame*min(max(-360, Angle), 360);

	// check for simpliest case
	if (fabs(Angle) < 0.001 || Angle > 359.999 || Angle < -359.999)
		return S_OK;

	// check whether we flip image on 180 degrees
	// if (fabs(fabs(Angle) - 180) < 0.001)
	// {
	//	// TODO: special case
	//	ippiMirror_8u_AC4IR(pDst, m_nIppiStep, m_IppiRoiSize, ippAxsBoth);
	//	return S_OK;
	// }

	if (fabs(Angle -  90) < 0.001 || fabs(Angle + 270) < 0.001 || fabs(Angle +  90) < 0.001 || fabs(Angle - 270) < 0.001)
	{
		int nNewWidth = m_nHeight;
		int nNewHeight = m_nWidth;

		if (!ImageStudioUtils::ByteArrayToMediaData(NULL, nNewWidth, nNewHeight, pMediaData))
			return S_FALSE;
		IUnknown* pCreatedMediaData = *pMediaData;
		ImageStudio::Core::MediaData oMediaData;
		if (!oMediaData.Create(*pMediaData, FALSE) || !oMediaData.IsValidBGRA())
			return S_FALSE;
		*pMediaData = oMediaData.GetMediaData(TRUE);
		if (*pMediaData != pCreatedMediaData)
			pCreatedMediaData->Release();

		BYTE* pDst = oMediaData.GetBuffer();

		ClearImage(pDst, 4*nNewWidth*nNewHeight);

		//int x, y;
		int index_src = 0;
		int index_dst = 0;

		BOOL b90  = (fabs(Angle -  90) < 0.001 || fabs(Angle + 270) < 0.001);
		BOOL b270 = (fabs(Angle +  90) < 0.001 || fabs(Angle - 270) < 0.001);

		for (int y = 0; y < nNewHeight; ++y)
		{
			for (int x = 0; x < nNewWidth; ++x, pDst += 4)
			{
				if (b90)
					index_src = 4*(x*nNewHeight + (nNewHeight - 1 - y));
				else if (b270)
					index_src = 4*((nNewWidth - 1 - x)*nNewHeight + y);

				memcpy(pDst, m_pPixels + index_src, 4);
			}
		}

		return S_OK;
	}
	else if (fabs(Angle - 180) < 0.001 || fabs(Angle + 180) < 0.001)
	{
		int nNewWidth = m_nWidth;
		int nNewHeight = m_nHeight;

		if (!ImageStudioUtils::ByteArrayToMediaData(NULL, nNewWidth, nNewHeight, pMediaData))
			return S_FALSE;
		IUnknown* pCreatedMediaData = *pMediaData;
		ImageStudio::Core::MediaData oMediaData;
		if (!oMediaData.Create(*pMediaData, FALSE) || !oMediaData.IsValidBGRA())
			return S_FALSE;
		*pMediaData = oMediaData.GetMediaData(TRUE);
		if (*pMediaData != pCreatedMediaData)
			pCreatedMediaData->Release();

		BYTE* pDst = oMediaData.GetBuffer();

		ClearImage(pDst, 4*nNewWidth*nNewHeight);

		//int x, y;
		int index_src = 0;
		int index_dst = 0;

		for (int y = 0; y < nNewHeight; ++y)
		{
			for (int x = 0; x < nNewWidth; ++x, pDst += 4)
			{
				index_src = 4*((nNewHeight - 1 - y)*nNewWidth + (nNewWidth - 1 - x));

				memcpy(pDst, m_pPixels + index_src, 4);
			}
		}

		return S_OK;
	}
	else
	{
		// variables
		Ipp8u* pSrc = (Ipp8u*)m_pPixels;
		IppiRect srcRoi = {0, 0, m_IppiRoiSize.width , m_IppiRoiSize.height};
		double quad[4][2];

		ippiGetRotateQuad(srcRoi, quad, Angle, 0, 0);

		double dLeft	= min(min(quad[0][0], quad[1][0]), min(quad[2][0], quad[3][0]));
		double dRight	= max(max(quad[0][0], quad[1][0]), max(quad[2][0], quad[3][0]));
		double dTop		= min(min(quad[0][1], quad[1][1]), min(quad[2][1], quad[3][1]));
		double dBottom	= max(max(quad[0][1], quad[1][1]), max(quad[2][1], quad[3][1]));

		int nNewWidth = (int)ceil(dRight - dLeft);
		int nNewHeight = (int)ceil(dBottom - dTop);

		if (!ImageStudioUtils::ByteArrayToMediaData(NULL, nNewWidth, nNewHeight, pMediaData))
			return S_FALSE;
		IUnknown* pCreatedMediaData = *pMediaData;
		ImageStudio::Core::MediaData oMediaData;
		if (!oMediaData.Create(*pMediaData, FALSE) || !oMediaData.IsValidBGRA())
			return S_FALSE;
		*pMediaData = oMediaData.GetMediaData(TRUE);
		if (*pMediaData != pCreatedMediaData)
			pCreatedMediaData->Release();

		Ipp8u* pDst = (Ipp8u*)oMediaData.GetBuffer();
		IppiSize dstSize = {nNewWidth, nNewHeight};
		IppiRect dstRoi = {0, 0, nNewWidth, nNewHeight};

		ClearImage((BYTE*)pDst, 4*nNewWidth*nNewHeight);

		double xShift = 0, yShift = 0;
		ippiGetRotateShift(0.5*m_nWidth, 0.5*m_nHeight, Angle, &xShift, &yShift);
		xShift += 0.5*nNewWidth - 0.5*m_nWidth;
		yShift += 0.5*nNewHeight - 0.5*m_nHeight;

		ippiRotate_8u_AC4R(pSrc, m_IppiRoiSize, 4*m_nWidth, srcRoi, pDst, 4*nNewWidth, dstRoi, Angle, xShift, yShift, IPPI_INTER_LINEAR);
	}

	return S_OK;
}

STDMETHODIMP ImageTransform3::TransformTwirl(double Frame, double Angle, long Degree)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPTransformTwirl(m_pPixels, m_nWidth, m_nHeight, Frame, Angle, Degree, m_nBackColor))
		return S_OK;
	else
		return S_FALSE;
}
STDMETHODIMP ImageTransform3::TransformSphere(double Frame, long CenterX, long CenterY,  long RadiusX, long RadiusY, double Degree)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	if (Degree < -1.0)
		Degree = -1.0;
	else if (Degree > 10.0)
		Degree = 10.0;

	if (IPPTransformSphere(m_pPixels, m_nWidth, m_nHeight, Frame, CenterX, m_nHeight - 1 - CenterY, RadiusX, RadiusY, Degree, m_nBackColor))
		return S_OK;
	else
		return S_FALSE;
}
STDMETHODIMP ImageTransform3::TransformCylinder(double Frame, long Center, long Radius, double Degree, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	if (Degree < -1.0)
		Degree = -1.0;
	else if (Degree > 10.0)
		Degree = 10.0;

	if (IPPTransformCylinder(m_pPixels, m_nWidth, m_nHeight, Frame, Center, Radius, Degree, Type, m_nBackColor))
		return S_OK;
	else
		return S_FALSE;
}
		
STDMETHODIMP ImageTransform3::FilterPrewitt(double Frame, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterPrewitt(m_pPixels,m_nWidth,m_nHeight, Frame, Type))
        return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP ImageTransform3::FilterScharr(double Frame, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterScharr(m_pPixels,m_nWidth,m_nHeight, Frame, Type))
        return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP ImageTransform3::FilterSobel(double Frame, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterSobel(m_pPixels,m_nWidth,m_nHeight,Frame,Type))
        return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP ImageTransform3::FilterSobelExt(double Frame, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterSobelExt(m_pPixels,m_nWidth,m_nHeight,Frame,Type))
        return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP ImageTransform3::FilterRoberts(double Frame, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterRoberts(m_pPixels,m_nWidth,m_nHeight,Frame,Type))
        return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP ImageTransform3::FilterLaplace(double Frame, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterLaplace(m_pPixels,m_nWidth,m_nHeight,Frame, Type))
        return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP ImageTransform3::FilterHipass(double Frame, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterHipass(m_pPixels,m_nWidth,m_nHeight,Frame, Type))
        return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP ImageTransform3::FilterLowpass(double Frame, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterLowpass(m_pPixels,m_nWidth,m_nHeight,Frame, Type))
        return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP ImageTransform3::FilterBlur(double Frame, long Size)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterBlur(m_pPixels,m_nWidth,m_nHeight,Frame, Size))
        return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP ImageTransform3::FilterCanny(double Frame, double LowThresh, double HighThresh)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	if (IPPFilterCanny(m_pPixels,m_nWidth,m_nHeight, Frame, LowThresh, HighThresh))
        return S_OK;
	else
		return S_FALSE;
}


STDMETHODIMP ImageTransform3::UtilitySwapChannels(BYTE nChannel0, BYTE nChannel1, BYTE nChannel2)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	if (IPPUtilitySwapChannels(m_pPixels, m_nWidth, m_nHeight, nChannel0, nChannel1, nChannel2))
        return S_OK;
	else
		return S_FALSE;

}
		    
STDMETHODIMP ImageTransform3::DrawLine(double Frame, long X1, long Y1, long X2, long Y2, long Color, long Alpha, long Size)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// prepare drawing
	BeginDraw();

	// create drawing graphics
	Graphics* graphics = Graphics::FromImage(m_pBitmap);

	// create color
	Gdiplus::Color color((BYTE)(Frame*Alpha), GetRValue(Color), GetGValue(Color), GetBValue(Color));

	// create pen
	Pen pen(color, (REAL)Size);

	// draw line
	graphics->DrawLine(&pen, X1, Y1, X2, Y2);

	// stop painting
	graphics->Flush();

	// delete objects
	delete graphics;

	// finalize drawing
	EndDraw();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::DrawRectangle(double Frame, long X1, long Y1, long X2, long Y2, long Color, long Alpha, VARIANT_BOOL Solid)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// compose alpha valies
	double dAlpha2 = (1.0 - Frame*Alpha/255.0);
	double dAlpha1 = Frame*Alpha/255.0;

	// retrieve color components
	double dRed = dAlpha1*GetRValue(Color);
	double dGreen = dAlpha1*GetGValue(Color);
	double dBlue = dAlpha1*GetBValue(Color);

	// compute sizes
	int nLeft = min(X1, X2);
	int nTop = min(Y1, Y2);
	int nRight = max(X1, X2);
	int nBottom = max(Y1, Y2);

	// variables
	int nX, nY, nIndex;

	// draw image
	for (nY = nTop; nY < nBottom; ++nY)
	{
		// check for valid coordinates
		if (nY < 0 || nY >= m_nHeight)
			continue;

		for (nX = nLeft; nX < nRight; ++nX)
		{
			// check for valid coordinates
			if (nX < 0 || nX >= m_nWidth)
				continue;

			// compute index
			nIndex = 4*(nX + (m_nHeight - 1 - nY)*m_nWidth);

			// draw rectangle
			if ((Solid == VARIANT_TRUE) || ((Solid == VARIANT_FALSE) && (nY == nTop || nY == nBottom - 1 || nX == nLeft || nX == nRight - 1)))
			{
				m_pPixels[nIndex + 0] = (BYTE)(dAlpha2*m_pPixels[nIndex + 0] + dBlue);
				m_pPixels[nIndex + 1] = (BYTE)(dAlpha2*m_pPixels[nIndex + 1] + dGreen);
				m_pPixels[nIndex + 2] = (BYTE)(dAlpha2*m_pPixels[nIndex + 2] + dRed);
			}
		}
	}

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::DrawEllipse(double Frame, long X1, long Y1, long X2, long Y2, long Color, long Alpha, VARIANT_BOOL Solid)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// prepare drawing
	BeginDraw();

	// create drawing graphics
	Graphics* graphics = Graphics::FromImage(m_pBitmap); graphics->SetSmoothingMode(SmoothingModeAntiAlias);

	// create color
	Gdiplus::Color color((BYTE)(Frame*Alpha), GetRValue(Color), GetGValue(Color), GetBValue(Color));

	// create pen and brush
	Pen pen(color, 1.0);
	SolidBrush brush(color);

	// compute rectangle coordinates
	int Left = min(X1, X2);
	int Right = max(X1, X2);
	int Top = min(Y1, Y2);
	int Bottom = max(Y1, Y2);

	// draw solid ellipse
	if (Solid == VARIANT_TRUE)
		graphics->FillEllipse(&brush, Left, Top, Right - Left, Bottom - Top);

	// draw bounding ellipse
	graphics->DrawEllipse(&pen, Left, Top, Right - Left, Bottom - Top);

	// stop painting
	graphics->Flush();

	// delete objects
	delete graphics;

	// finalize drawing
	EndDraw();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::DrawInvRectangle(double Frame, long X1, long Y1, long X2, long Y2, long Color, long Alpha)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// prepare drawing
	BeginDraw();

	// create drawing graphics
	Graphics* graphics = Graphics::FromImage(m_pBitmap);

	// create color
	Gdiplus::Color color((BYTE)(Frame*Alpha), GetRValue(Color), GetGValue(Color), GetBValue(Color));

	// create pen and brush
	SolidBrush brush(color);

	// compute rectangle coordinates
	int Left = min(X1, X2);
	int Right = max(X1, X2);
	int Top = min(Y1, Y2);
	int Bottom = max(Y1, Y2);

	// compose region to fill
	Rect rect1(0, 0, m_nWidth, m_nHeight);
	Rect rect2(Left, Top, Right - Left - 1, Bottom - Top - 1);
	Region region(rect1); region.Xor(rect2);

	// draw region
	graphics->FillRegion(&brush, &region);

	// stop painting
	graphics->Flush();

	// delete objects
	delete graphics;

	// finalize drawing
	EndDraw();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::DrawInvEllipse(double Frame, long X1, long Y1, long X2, long Y2, long Color, long Alpha)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// prepare drawing
	BeginDraw();

	// create drawing graphics
	Graphics* graphics = Graphics::FromImage(m_pBitmap);

	// create color
	Gdiplus::Color color((BYTE)(Frame*Alpha), GetRValue(Color), GetGValue(Color), GetBValue(Color));

	// create pen and brush
	SolidBrush brush(color);

	// compute rectangle coordinates
	int Left = min(X1, X2);
	int Right = max(X1, X2);
	int Top = min(Y1, Y2);
	int Bottom = max(Y1, Y2);

	// compose region to fill
	Rect rect(0, 0, m_nWidth, m_nHeight);
	GraphicsPath path; path.AddEllipse(Left, Top, Right - Left - 1, Bottom - Top - 1);
	Region region(rect); region.Xor(&path);

	// draw region
	graphics->FillRegion(&brush, &region);

	// stop painting
	graphics->Flush();

	// delete objects
	delete graphics;

	// finalize drawing
	EndDraw();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::DrawImagePtr(double Frame, long X1, long Y1, BYTE* Image, long Color, long Alpha)
{
	// variables
	SAFEARRAY* Array = (SAFEARRAY*)Image;

	// draw image
	return DrawImage(Frame, X1, Y1, &Array, Color, Alpha);
}

STDMETHODIMP ImageTransform3::DrawImage(double Frame, long X1, long Y1, SAFEARRAY** Image, long color, long Alpha)
{
	int nWidth = 0;
	int nHeight = 0;
	int nChannels = 0;
	
	BYTE* pData = ImageStudioUtils::ExtractImage(Image, nWidth, nHeight, nChannels);

	if (!pData || nChannels != 4)
		return S_FALSE;

	return DrawImage(Frame, X1, Y1, pData, nWidth, nHeight, color, Alpha);
}

STDMETHODIMP ImageTransform3::DrawImageMedia(double Frame, long X1, long Y1, IUnknown* pInterface, long Color, long Alpha)
{
	ImageStudio::Core::MediaData oMediaData;
	
	if (!oMediaData.Create(pInterface, FALSE) || !oMediaData.IsValidBGRA())
		return S_FALSE;

	return DrawImage(Frame, X1, Y1, oMediaData.GetBuffer(), oMediaData.GetWidth(), oMediaData.GetHeight(), Color, Alpha);
}

STDMETHODIMP ImageTransform3::DrawImageKey(double Frame, long X1, long Y1, SAFEARRAY** Image, long Color1, long Color2)
{	
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	Color col;
	float r, g, b;
	float fLightness, fSaturation;
	float fHue1;
	col.SetValue(Color1);	
	r = (float)col.GetB();
	r /= 255.0f;
	g = (float)col.GetG();
	g /= 255.0f;
	b = (float)col.GetR();
	b /= 255.0f;

	ConvertToHls(r, g, b, fHue1, fLightness, fSaturation);

	float fHue2;
	col.SetValue(Color2);	
	r = (float)col.GetB();
	r /= 255.0f;
	g = (float)col.GetG();
	g /= 255.0f;
	b = (float)col.GetR();
	b /= 255.0f;

	ConvertToHls(r, g, b, fHue2, fLightness, fSaturation);

	long bound;
	// m_pPixels
	BITMAPINFO videoBitmapInfo;
	videoBitmapInfo.bmiHeader.biPlanes = 1;
	videoBitmapInfo.bmiHeader.biBitCount = 32;
	videoBitmapInfo.bmiHeader.biCompression = 0;
	videoBitmapInfo.bmiHeader.biSizeImage = 0;
	videoBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	videoBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	videoBitmapInfo.bmiHeader.biClrUsed = 0;
	videoBitmapInfo.bmiHeader.biClrImportant = 0;
	videoBitmapInfo.bmiHeader.biSize = 40;	
	videoBitmapInfo.bmiHeader.biWidth = m_nWidth;
	videoBitmapInfo.bmiHeader.biHeight = m_nHeight;

	Bitmap videoBitmap(&videoBitmapInfo, m_pPixels);

	// Image
	SafeArrayGetUBound((*Image), 2, &bound);
	int nImageWidth = bound + 1;

	SafeArrayGetUBound((*Image), 3, &bound);
	int nImageHeight = bound + 1;

	int nHComponent1 = fHue1*255.0f/360.0f;
	int nHComponent2 = fHue2*255.0f/360.0f;

	CChromoKey oChromo;
	oChromo.SetImage((LPBYTE)(*Image)->pvData, nImageWidth, nImageHeight);
	oChromo.DoChromoKey(Frame, nHComponent1, m_nAdvancedChromaThreshold1, nHComponent2, m_nAdvancedChromaThreshold2);

	Graphics graphics(&videoBitmap);
	oChromo.DrawToGraphics(&graphics, TRUE, X1, Y1);

	ApplyRegister();

	// all ok	 
	return S_OK;
}
STDMETHODIMP ImageTransform3::DrawImageKey2(double Frame, long X1, long Y1, SAFEARRAY** Image, long color, long Threshold)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	Color col;
	float r, g, b;
	float fHue, fLightness, fSaturation;
	float koef1 = 0, koef2 = 0;
	col.SetValue(color);	
	r = (float)col.GetB();
	r /= 255.0f;
	g = (float)col.GetG();
	g /= 255.0f;
	b = (float)col.GetR();
	b /= 255.0f;

	ConvertToHls(r, g, b, fHue, fLightness, fSaturation);

	if(Threshold > 100)
		Threshold = 100;
	if(Threshold < 0)
		Threshold = 0;

	long bound;
	// m_pPixels
	BITMAPINFO videoBitmapInfo;
	videoBitmapInfo.bmiHeader.biPlanes = 1;
	videoBitmapInfo.bmiHeader.biBitCount = 32;
	videoBitmapInfo.bmiHeader.biCompression = 0;
	videoBitmapInfo.bmiHeader.biSizeImage = 0;
	videoBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	videoBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	videoBitmapInfo.bmiHeader.biClrUsed = 0;
	videoBitmapInfo.bmiHeader.biClrImportant = 0;
	videoBitmapInfo.bmiHeader.biSize = 40;	
	videoBitmapInfo.bmiHeader.biWidth = m_nWidth;
	videoBitmapInfo.bmiHeader.biHeight = m_nHeight;

	Bitmap videoBitmap(&videoBitmapInfo, m_pPixels);

	// Image
	SafeArrayGetUBound((*Image), 2, &bound);
	int nImageWidth = bound + 1;

	SafeArrayGetUBound((*Image), 3, &bound);
	int nImageHeight = bound + 1;

	int nHComponent = fHue*255.0f/360.0f;
	CChromoKey oChromo;
	oChromo.SetImage((LPBYTE)(*Image)->pvData, nImageWidth, nImageHeight);
	oChromo.DoChromoKey(Frame, nHComponent, Threshold);
	
	Graphics graphics(&videoBitmap); graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	oChromo.DrawToGraphics(&graphics, TRUE, X1, Y1);

	ApplyRegister();

	return S_OK;
}
STDMETHODIMP ImageTransform3::DrawText(double Frame, long X1, long Y1, BSTR Text, IUnknown* Font, long Color, long Alpha)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// prepare drawing
	BeginDraw();

	// create drawing graphics
	Graphics* graphics = Graphics::FromImage(m_pBitmap); graphics->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

	// create color
	Gdiplus::Color color((BYTE)(Frame*Alpha), GetRValue(Color), GetGValue(Color), GetBValue(Color));

	// create pen and brush
	PointF point((REAL)X1, (REAL)Y1);
	SolidBrush brush(color);

	// retrieve screen dc
	HDC dc = GetDC(0);

	// variables
	Gdiplus::Font* font = 0;

	// check for valid array
	if (Font)
	{
		// compose font object
		IFont* pFont = 0; Font->QueryInterface(IID_IFont, (void**)&pFont);

		// check for valid font
		if (!pFont)
			return S_FALSE;

		// retrieve font handle
		HFONT hfont = 0; pFont->get_hFont(&hfont);

		// create gdi+ font
		font = new Gdiplus::Font(dc, hfont);
	}
	else
	{
		// create gdi+ font
		font = new Gdiplus::Font(dc);
	}

	// draw text string
	graphics->DrawString(Text, SysStringLen(Text), font, point, &brush);

	// stop painting
	graphics->Flush();

	// release desktop device context
	ReleaseDC(0, dc);

	// delete objects
	delete graphics;
	delete font;

	// finalize drawing
	EndDraw();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::DrawText2(double Frame, long X1, long Y1, long X2, long Y2, BSTR Text, IUnknown* Font, long Color, long Alpha)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// chck for very thin rect
	if (abs(X1 - X2) < 2 || abs(Y1 - Y2) < 2)
	{
		// apply registration
		ApplyRegister();

		// all ok
		return S_OK;
	}

	// clamp values
	Frame = Clamp(Frame);

	// prepare drawing
	BeginDraw();

	// create drawing graphics
	Graphics* graphics = Graphics::FromImage(m_pBitmap); graphics->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

	// create color
	Gdiplus::Color color((BYTE)(Frame*Alpha), GetRValue(Color), GetGValue(Color), GetBValue(Color));

	// create pen and brush
	PointF point((REAL)X1, (REAL)Y1);
	RectF rect((REAL)X1, (REAL)Y1, (REAL)(X2 - X1), (REAL)(Y2 - Y1));
	SolidBrush brush(color);
	StringFormat format; 
	
	// setup painting format
	format.SetFormatFlags(StringFormatFlagsNoWrap);
	format.SetAlignment(StringAlignmentCenter); 
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingNone);

	// retrieve screen dc
	HDC dc = GetDC(0);

	// variables
	Gdiplus::Font* font = 0;

	// check for valid array
	if (Font)
	{
		// compose font object
		IFont* pFont = 0; Font->QueryInterface(IID_IFont, (void**)&pFont);

		// check for valid font
		if (!pFont)
			return S_FALSE;

		// retrieve font handle
		HFONT hfont = 0; pFont->get_hFont(&hfont);

		// create gdi+ font
		font = new Gdiplus::Font(dc, hfont);
	}
	else
	{
		// create gdi+ font
		font = new Gdiplus::Font(dc);
	}

	// retrieve current logfont
	RectF fontRect;

	// measure the string
	graphics->MeasureString(Text, SysStringLen(Text), font, point, &fontRect);

	// update font size
	double scaleX = rect.Width/(double)fontRect.Width;
	double scaleY = rect.Height/(double)fontRect.Height;

	// apply translation to the device context
	graphics->TranslateTransform(point.X, point.Y);
	graphics->ScaleTransform(scaleX, scaleY);

	// draw text at start
	point.X = 0;
	point.Y = 0;
	
	// draw text string
	graphics->DrawString(Text, SysStringLen(Text), font, point, &brush);

	// stop painting
	graphics->Flush();

	// release desktop device context
	ReleaseDC(0, dc);

	// delete objects
	delete graphics;
	delete font;

	// finalize drawing
	EndDraw();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::DrawCredits(double Frame, BSTR Text, IUnknown* Font, long Color, long Alpha, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// prepare drawing
	BeginDraw();

	// create drawing graphics
	Graphics* graphics = Graphics::FromImage(m_pBitmap); graphics->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

	// retrieve screen dc
	HDC dc = GetDC(0);

	// variables
	Gdiplus::Font* font = 0;

	// check for valid array
	if (Font)
	{
		// compose font object
		IFont* pFont = 0; Font->QueryInterface(IID_IFont, (void**)&pFont);

		// check for valid font
		if (!pFont)
			return S_FALSE;

		// retrieve font handle
		HFONT hfont = 0; pFont->get_hFont(&hfont);

		// create gdi+ font
		font = new Gdiplus::Font(dc, hfont);
	}
	else
	{
		// create gdi+ font
		font = new Gdiplus::Font(dc);
	}

	// compute text size
	PointF start(0, 0);
	RectF layout(0, 0, 0, 0);
	SizeF size; graphics->MeasureString(Text, SysStringLen(Text), font, start, &layout);

	// variables
	PointF point;

	// create color
	Gdiplus::Color color((BYTE)Alpha, GetRValue(Color), GetGValue(Color), GetBValue(Color));

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
				color = Gdiplus::Color((BYTE)(Alpha*(1.0 - Frame)/0.25), GetRValue(Color), GetGValue(Color), GetBValue(Color));
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
				color = Gdiplus::Color((BYTE)(Alpha*Frame/0.25), GetRValue(Color), GetGValue(Color), GetBValue(Color));
			Frame = 0.5;
		}
	}

	// create pen and brush
	SolidBrush brush(color);

	// compute points
	if (Type == c_nCreditsHMMBottom || Type == c_nCreditsHMWBottom || Type == c_nCreditsHMHBottom || Type == c_nCreditsHMSBottom || Type == c_nCreditsHHMBottom)
	{
		point.X = (REAL)((m_nWidth + 1)*(1.0 - Frame) + Frame*(-layout.GetRight() - 1));
		point.Y = (REAL)(m_nHeight - 1 - layout.Height);
	}
	if (Type == c_nCreditsHMMCenter || Type == c_nCreditsHMWCenter || Type == c_nCreditsHMHCenter || Type == c_nCreditsHMSCenter || Type == c_nCreditsHHMCenter)
	{
		point.X = (REAL)((m_nWidth + 1)*(1.0 - Frame) + Frame*(-layout.GetRight() - 1));
		point.Y = (REAL)(0.5*m_nHeight - 0.5*layout.Height);
	}
	if (Type == c_nCreditsHMMTop    || Type == c_nCreditsHMWTop    || Type == c_nCreditsHMHTop    || Type == c_nCreditsHMSTop    || Type == c_nCreditsHHMTop)
	{
		point.X = (REAL)((m_nWidth + 1)*(1.0 - Frame) + Frame*(-layout.GetRight() - 1));
		point.Y = (REAL)1;
	}
	if (Type == c_nCreditsVMMLeft   || Type == c_nCreditsVMWLeft   || Type == c_nCreditsVMHLeft   || Type == c_nCreditsVMSLeft   || Type == c_nCreditsVHMLeft)
	{
		point.X = (REAL)(1);
		point.Y = (REAL)((m_nHeight + 1)*(1.0 - Frame) + Frame*(-layout.GetBottom() - 1));
	}
	if (Type == c_nCreditsVMMCenter || Type == c_nCreditsVMWCenter || Type == c_nCreditsVMHCenter || Type == c_nCreditsVMSCenter || Type == c_nCreditsVHMCenter)
	{
		point.X = (REAL)(0.5*(m_nWidth - 1) - 0.5*layout.Width);
		point.Y = (REAL)((m_nHeight + 1)*(1.0 - Frame) + Frame*(-layout.GetBottom() - 1));
	}
	if (Type == c_nCreditsVMMRight  || Type == c_nCreditsVMWRight  || Type == c_nCreditsVMHRight  || Type == c_nCreditsVMSRight  || Type == c_nCreditsVHMRight)
	{
		point.X = (REAL)(m_nWidth - 1 - layout.Width - 1);
		point.Y = (REAL)((m_nHeight + 1)*(1.0 - Frame) + Frame*(-layout.GetBottom() - 1));
	}
	// draw text string
	graphics->DrawString(Text, SysStringLen(Text), font, point, &brush);

	// stop painting
	graphics->Flush();

	// release desktop device context
	ReleaseDC(0, dc);

	// delete objects
	delete graphics;
	delete font;

	// finalize drawing
	EndDraw();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}

STDMETHODIMP ImageTransform3::DrawBorder(double Frame, long Color, long Size, long Type)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);
	Size = (int)(Frame*max(0, min(Size, min(m_nHeight/2 - 1, m_nWidth/2 - 1))));

	// check for simpliest case
	if (Size == 0)
		return S_OK;

	// variables
	int nImageLeft = 0;
	int nImageTop = 0;
	int nImageRight = m_nWidth - 1;
	int nImageBottom = m_nHeight - 1;

	// compute image rectangle
	if (Type == c_nBorderSolidAll || Type == c_nBorderGradientLightAll || Type == c_nBorderGradientDarkAll || Type == c_nBorderTransparentAll)
	{
		nImageLeft = Size;
		nImageTop = Size;
		nImageRight = m_nWidth - 1 - Size;
		nImageBottom = m_nHeight - 1 - Size;
	}
	else if (Type == c_nBorderSolidLeft || Type == c_nBorderGradientLightLeft || Type == c_nBorderGradientDarkLeft || Type == c_nBorderTransparentLeft)
	{
		nImageLeft = Size;
		nImageTop = 0;
		nImageRight = m_nWidth - 1;
		nImageBottom = m_nHeight - 1;
	}
	else if (Type == c_nBorderSolidBottom || Type == c_nBorderGradientLightBottom || Type == c_nBorderGradientDarkBottom || Type == c_nBorderTransparentBottom)
	{
		nImageLeft = 0;
		nImageTop = Size;
		nImageRight = m_nWidth - 1;
		nImageBottom = m_nHeight - 1;
	}
	else if (Type == c_nBorderSolidRight || Type == c_nBorderGradientLightRight || Type == c_nBorderGradientDarkRight || Type == c_nBorderTransparentRight)
	{
		nImageLeft = 0;
		nImageTop = 0;
		nImageRight = m_nWidth - 1 - Size;
		nImageBottom = m_nHeight - 1;
	}
	else if (Type == c_nBorderSolidTop || Type == c_nBorderGradientLightTop || Type == c_nBorderGradientDarkTop || Type == c_nBorderTransparentTop)
	{
		nImageLeft = 0;
		nImageTop = 0;
		nImageRight = m_nWidth - 1;
		nImageBottom = m_nHeight - 1 - Size;
	}
	else if (Type == c_nBorderSolidLeftRight || Type == c_nBorderGradientLightLeftRight || Type == c_nBorderGradientDarkLeftRight || Type == c_nBorderTransparentLeftRight)
	{
		nImageLeft = Size;
		nImageTop = 0;
		nImageRight = m_nWidth - 1 - Size;
		nImageBottom = m_nHeight - 1;
	}
	else if (Type == c_nBorderSolidTopBottom || Type == c_nBorderGradientLightTopBottom || Type == c_nBorderGradientDarkTopBottom || Type == c_nBorderTransparentTopBottom)
	{
		nImageLeft = 0;
		nImageTop = Size;
		nImageRight = m_nWidth - 1;
		nImageBottom = m_nHeight - 1 - Size;
	}

	// variables
	int nX, nY, nIndex = 0;
	double dSourceX, dSourceY, dAlpha;
	BYTE nR1, nG1, nB1, nR2, nG2, nB2, nR3, nG3, nB3;

	// compute border color components
	BYTE nBR = GetRValue(Color);
	BYTE nBG = GetGValue(Color);
	BYTE nBB = GetBValue(Color);
	BYTE nBLR = Byte(nBR + 128);
	BYTE nBLG = Byte(nBG + 128);
	BYTE nBLB = Byte(nBB + 128);
	BYTE nBDR = Byte(nBR - 128);
	BYTE nBDG = Byte(nBG - 128);
	BYTE nBDB = Byte(nBB - 128);

	// begin effect
	BeginEffect();

	// check for simpliest case
	if (Type < c_nBorderTransparentAll || Type > c_nBorderTransparentTopBottom)
	{
		// draw one image inside another
		for (nY = nImageTop; nY <= nImageBottom; ++nY)
		{
			// compute y coordinate
			dSourceY = min(m_nHeight - 0.001, m_nHeight*(nY - nImageTop)/(double)(nImageBottom - nImageTop));

			for (nX = nImageLeft; nX <= nImageRight; ++nX)
			{
				// compute x coordinate
				dSourceX = min(m_nWidth - 0.001, m_nWidth*(nX - nImageLeft)/(double)(nImageRight - nImageLeft));

				// check for valid destination coordinates
				if (nX < 0 || nX >= m_nWidth || nY < 0 || nY >= m_nHeight)
					continue;

				// compute source pixel index
				nIndex = 4*(nX + nY*m_nWidth);

				// copy pixels
				m_pPixels[nIndex + 0] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 0, 4, 0);
				m_pPixels[nIndex + 1] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 1, 4, 0);
				m_pPixels[nIndex + 2] = GetPointBillinear(dSourceX, dSourceY, m_pBackup, m_nWidth, m_nHeight, 2, 4, 0);
			}
		}
	}

	// draw border
	for (nY = 0, nIndex = 0; nY < m_nHeight; ++nY)
	{
		for (nX = 0; nX < m_nWidth; ++nX, nIndex += 4)
		{
			if (nX >= nImageLeft && nY >= nImageTop && nX <= nImageRight && nY <= nImageBottom)
				continue;

			// solid
			if (Type == c_nBorderSolidAll || Type == c_nBorderSolidLeft || Type == c_nBorderSolidTop || Type == c_nBorderSolidRight || Type == c_nBorderSolidBottom || Type == c_nBorderSolidLeftRight || Type == c_nBorderSolidTopBottom)
			{
				m_pPixels[nIndex + 0] = nBB;
				m_pPixels[nIndex + 1] = nBG;
				m_pPixels[nIndex + 2] = nBR;
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

						m_pPixels[nIndex + 2] = (nX < nY) ? nR1 : nR2;
						m_pPixels[nIndex + 1] = (nX < nY) ? nG1 : nG2;
						m_pPixels[nIndex + 0] = (nX < nY) ? nB1 : nB2;
					}
					else if (nY > nImageBottom)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*(m_nHeight - 1 - nY)/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*(m_nHeight - 1 - nY)/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*(m_nHeight - 1 - nY)/(double)Size);

						m_pPixels[nIndex + 2] = (nX < (m_nHeight - 1 - nY)) ? nR1 : nR2;
						m_pPixels[nIndex + 1] = (nX < (m_nHeight - 1 - nY)) ? nG1 : nG2;
						m_pPixels[nIndex + 0] = (nX < (m_nHeight - 1 - nY)) ? nB1 : nB2;
					}
					else
					{
						m_pPixels[nIndex + 2] = nR1;
						m_pPixels[nIndex + 1] = nG1;
						m_pPixels[nIndex + 0] = nB1;
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
					nR1 = Byte(nBR + (nR3 - nBR)*(m_nWidth - 1 - nX)/(double)Size);
					nG1 = Byte(nBG + (nG3 - nBG)*(m_nWidth - 1 - nX)/(double)Size);
					nB1 = Byte(nBB + (nB3 - nBB)*(m_nWidth - 1 - nX)/(double)Size);

					if (nY < nImageTop)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*nY/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*nY/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*nY/(double)Size);

						m_pPixels[nIndex + 2] = ((m_nWidth - 1 - nX) < nY) ? nR1 : nR2;
						m_pPixels[nIndex + 1] = ((m_nWidth - 1 - nX) < nY) ? nG1 : nG2;
						m_pPixels[nIndex + 0] = ((m_nWidth - 1 - nX) < nY) ? nB1 : nB2;
					}
					else if (nY > nImageBottom)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*(m_nHeight - 1 - nY)/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*(m_nHeight - 1 - nY)/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*(m_nHeight - 1 - nY)/(double)Size);

						m_pPixels[nIndex + 2] = ((m_nWidth - 1 - nX) < (m_nHeight - 1 - nY)) ? nR1 : nR2;
						m_pPixels[nIndex + 1] = ((m_nWidth - 1 - nX) < (m_nHeight - 1 - nY)) ? nG1 : nG2;
						m_pPixels[nIndex + 0] = ((m_nWidth - 1 - nX) < (m_nHeight - 1 - nY)) ? nB1 : nB2;
					}
					else
					{
						m_pPixels[nIndex + 2] = nR1;
						m_pPixels[nIndex + 1] = nG1;
						m_pPixels[nIndex + 0] = nB1;
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

						m_pPixels[nIndex + 2] = (nY < nX) ? nR1 : nR2;
						m_pPixels[nIndex + 1] = (nY < nX) ? nG1 : nG2;
						m_pPixels[nIndex + 0] = (nY < nX) ? nB1 : nB2;
					}
					else if (nX > nImageRight)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*(m_nWidth - 1 - nX)/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*(m_nWidth - 1 - nX)/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*(m_nWidth - 1 - nX)/(double)Size);

						m_pPixels[nIndex + 2] = (nY < (m_nWidth - 1 - nX)) ? nR1 : nR2;
						m_pPixels[nIndex + 1] = (nY < (m_nWidth - 1 - nX)) ? nG1 : nG2;
						m_pPixels[nIndex + 0] = (nY < (m_nWidth - 1 - nX)) ? nB1 : nB2;
					}
					else
					{
						m_pPixels[nIndex + 2] = nR1;
						m_pPixels[nIndex + 1] = nG1;
						m_pPixels[nIndex + 0] = nB1;
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
					nR1 = Byte(nBR + (nR3 - nBR)*(m_nHeight - 1 - nY)/(double)Size);
					nG1 = Byte(nBG + (nG3 - nBG)*(m_nHeight - 1 - nY)/(double)Size);
					nB1 = Byte(nBB + (nB3 - nBB)*(m_nHeight - 1 - nY)/(double)Size);

					if (nX < nImageLeft)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*nX/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*nX/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*nX/(double)Size);

						m_pPixels[nIndex + 2] = ((m_nHeight - 1 - nY) < nX) ? nR1 : nR2;
						m_pPixels[nIndex + 1] = ((m_nHeight - 1 - nY) < nX) ? nG1 : nG2;
						m_pPixels[nIndex + 0] = ((m_nHeight - 1 - nY) < nX) ? nB1 : nB2;
					}
					else if (nX > nImageRight)
					{
						nR2 = Byte(nBR + (nR3 - nBR)*(m_nWidth - 1 - nX)/(double)Size);
						nG2 = Byte(nBG + (nG3 - nBG)*(m_nWidth - 1 - nX)/(double)Size);
						nB2 = Byte(nBB + (nB3 - nBB)*(m_nWidth - 1 - nX)/(double)Size);

						m_pPixels[nIndex + 2] = ((m_nHeight - 1 - nY) < (m_nWidth - 1 - nX)) ? nR1 : nR2;
						m_pPixels[nIndex + 1] = ((m_nHeight - 1 - nY) < (m_nWidth - 1 - nX)) ? nG1 : nG2;
						m_pPixels[nIndex + 0] = ((m_nHeight - 1 - nY) < (m_nWidth - 1 - nX)) ? nB1 : nB2;
					}
					else
					{
						m_pPixels[nIndex + 2] = nR1;
						m_pPixels[nIndex + 1] = nG1;
						m_pPixels[nIndex + 0] = nB1;
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
					else if (nY > nImageBottom && nX >= (m_nHeight - 1 - nY))
					{
						if ((m_nHeight - 1 - nY) > 0.5*Size)
							dAlpha = (m_nHeight - 1 - nY - 0.5*Size)/(0.5*Size);
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

					m_pPixels[nIndex + 0] = (BYTE)(dAlpha*m_pBackup[nIndex + 0] + (1.0 - dAlpha)*nBB);
					m_pPixels[nIndex + 1] = (BYTE)(dAlpha*m_pBackup[nIndex + 1] + (1.0 - dAlpha)*nBG);
					m_pPixels[nIndex + 2] = (BYTE)(dAlpha*m_pBackup[nIndex + 2] + (1.0 - dAlpha)*nBR);
				}
			}
			if (Type == c_nBorderTransparentAll || Type == c_nBorderTransparentRight || Type == c_nBorderTransparentLeftRight)
			{
				if (nX > nImageRight)
				{
					if (nY < nImageTop && (m_nWidth - 1 - nX) >= nY)
					{
						if (nY > 0.5*Size)
						{
							dAlpha = (nY - 0.5*Size)/(0.5*Size);
						}
						else
							dAlpha = 0;
					}
					else if (nY > nImageBottom && (m_nWidth - 1 - nX) >= (m_nHeight - 1 - nY))
					{
						if ((m_nHeight - 1 - nY) > 0.5*Size)
							dAlpha = (m_nHeight - 1 - nY - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}
					else
					{
						if ((m_nWidth - 1 - nX) > 0.5*Size)
							dAlpha = ((m_nWidth - 1 - nX) - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}

					m_pPixels[nIndex + 0] = (BYTE)(dAlpha*m_pBackup[nIndex + 0] + (1.0 - dAlpha)*nBB);
					m_pPixels[nIndex + 1] = (BYTE)(dAlpha*m_pBackup[nIndex + 1] + (1.0 - dAlpha)*nBG);
					m_pPixels[nIndex + 2] = (BYTE)(dAlpha*m_pBackup[nIndex + 2] + (1.0 - dAlpha)*nBR);
				}
			}
			if (Type == c_nBorderTransparentAll || Type == c_nBorderTransparentTop || Type == c_nBorderTransparentTopBottom)
			{
				if (nY > nImageBottom)
				{
					if (nX < nImageLeft && (m_nHeight - 1 - nY) >= nX)
					{
						if (nX > 0.5*Size)
						{
							dAlpha = (nX - 0.5*Size)/(0.5*Size);
						}
						else
							dAlpha = 0;
					}
					else if (nX > nImageRight && (m_nHeight - 1 - nY) >= (m_nWidth - 1 - nX))
					{
						if ((m_nWidth - 1 - nX) > 0.5*Size)
							dAlpha = (m_nWidth - 1 - nX - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}
					else
					{
						if ((m_nHeight - 1 - nY) > 0.5*Size)
							dAlpha = ((m_nHeight - 1 - nY) - 0.5*Size)/(0.5*Size);
						else
							dAlpha = 0;
					}

					m_pPixels[nIndex + 0] = (BYTE)(dAlpha*m_pBackup[nIndex + 0] + (1.0 - dAlpha)*nBB);
					m_pPixels[nIndex + 1] = (BYTE)(dAlpha*m_pBackup[nIndex + 1] + (1.0 - dAlpha)*nBG);
					m_pPixels[nIndex + 2] = (BYTE)(dAlpha*m_pBackup[nIndex + 2] + (1.0 - dAlpha)*nBR);
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
					else if (nX > nImageRight && nY >= (m_nWidth - 1 - nX))
					{
						if ((m_nWidth - 1 - nX) > 0.5*Size)
							dAlpha = (m_nWidth - 1 - nX - 0.5*Size)/(0.5*Size);
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

					m_pPixels[nIndex + 0] = (BYTE)(dAlpha*m_pBackup[nIndex + 0] + (1.0 - dAlpha)*nBB);
					m_pPixels[nIndex + 1] = (BYTE)(dAlpha*m_pBackup[nIndex + 1] + (1.0 - dAlpha)*nBG);
					m_pPixels[nIndex + 2] = (BYTE)(dAlpha*m_pBackup[nIndex + 2] + (1.0 - dAlpha)*nBR);
				}
			}
		}
	}

	// finalize effect
	EndEffect();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}
STDMETHODIMP ImageTransform3::DrawMovObject(double Frame, long Type, long Divisions, long Color, long Alpha)
{
	// check for valid image
	if (!IsValid())
		return S_FALSE;

	// clamp values
	Frame = Clamp(Frame);

	// begin effect
	BeginEffect();

	// compose size
	m_nSizeX = m_nWidth/max(1, min(m_nWidth/4, Divisions));
	m_nSizeY = m_nHeight/max(1, min(m_nHeight/4, Divisions));

	// apply effect
	ApplyCompose(Frame, Type, Color, Alpha);

	// finalize effect
	EndEffect();

	// apply registration
	ApplyRegister();

	// all ok
	return S_OK;
}








