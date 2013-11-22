
#include "stdafx.h"
#include "ImageTransform.h"
#include "ImageTransformIPP.h"

float g_fCos[720];
float g_fSin[720];
BOOL  g_bSinCosInit = FALSE;

void InitSinCos()
{
	if (g_bSinCosInit)
		return;
	for (int i=0;i<720;i++)
	{
		g_fCos[i] = cos(M_PI*i/360);
		g_fSin[i] = sin(M_PI*i/360);
	}
}
void IPPClearImage(void* pData, int nSize, DWORD nColor)
{
	if (!pData)
		return;

	int* p = (int*)pData;

	int c = RGBA(GetBValue(nColor), GetGValue(nColor), GetRValue(nColor), 255);
	
	for (int index = 0; index < nSize; index += 4, ++p)
		*p = c;
}

BOOL IPPEffectBlur(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Level)
{
	IppiSize roiSize;
	roiSize.width	= nWidth;
	roiSize.height	= nHeight;

	IppiSize kernelSize;
	kernelSize.width  = 3;
	kernelSize.height  = 3;	
	Ipp32f* pKernel = new Ipp32f[kernelSize.width*kernelSize.height];
	Ipp8u *pDst = new Ipp8u[4*(nWidth + 2)*(nHeight + 2)];

	if (pKernel && pDst)
	{
		double dMatrixKoef = 1.0/((21 - 20.0*Frame*Level/100.0) + 8.0)*Frame;
		pKernel[0] = dMatrixKoef;
		pKernel[1] = dMatrixKoef;
		pKernel[2] = dMatrixKoef;
		pKernel[3] = dMatrixKoef;
		pKernel[4] = dMatrixKoef*(21 - 20.0*Frame*Level/100.0) + (1.0 - Frame);
		pKernel[5] = dMatrixKoef;
		pKernel[6] = dMatrixKoef;
		pKernel[7] = dMatrixKoef;
		pKernel[8] = dMatrixKoef;
	
		IppiPoint anchor;
		anchor.x = 1;
		anchor.y = 1;

		IppiSize roiDstSize;
		roiDstSize.width = nWidth + 2;
		roiDstSize.height = nHeight + 2;
		
		ippiCopyReplicateBorder_8u_AC4R((Ipp8u*)pPixels, 4*nWidth, roiSize, pDst, 4*nWidth + 8, roiDstSize, 1, 1);
	
		ippiFilter32f_8u_AC4R((Ipp8u*)(pDst + 4 + 4*roiDstSize.width), 4*roiDstSize.width, (Ipp8u*)pPixels, 4*nWidth, roiSize, pKernel, kernelSize, anchor);
	}

	if (pKernel)
		delete[] pKernel;
	if (pDst)
		delete[] pDst;
	return TRUE;
}

BOOL IPPEffectSharpen(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Level)
{
	IppiSize roiSize;
	roiSize.width	= nWidth;
	roiSize.height	= nHeight;

	IppiSize kernelSize;
	kernelSize.width  = 3;
	kernelSize.height  = 3;	
	Ipp32f* pKernel = new Ipp32f[kernelSize.width*kernelSize.height];
	Ipp8u* pDst = new Ipp8u[4*(nWidth + 2)*(nHeight + 2)];

	if (pKernel && pDst)
	{
		double dMatrixKoef = 1.0/(1.0 + 23.0*(1.0 - Frame*Level/100.0))*Frame;
		pKernel[0] = -dMatrixKoef;
		pKernel[1] = -dMatrixKoef;
		pKernel[2] = -dMatrixKoef;
		pKernel[3] = -dMatrixKoef;
		pKernel[4] = dMatrixKoef*(9.0 + 23.0*(1.0 - Frame*Level/100.0)) + (1.0 - Frame);
		pKernel[5] = -dMatrixKoef;
		pKernel[6] = -dMatrixKoef;
		pKernel[7] = -dMatrixKoef;
		pKernel[8] = -dMatrixKoef;

		IppiPoint anchor;
		anchor.x = 1;
		anchor.y = 1;

		IppiSize roiDstSize;
		roiDstSize.width = nWidth + 2;
		roiDstSize.height = nHeight + 2;
		
		ippiCopyReplicateBorder_8u_AC4R((Ipp8u*)pPixels, 4*nWidth, roiSize, pDst, 4*nWidth + 8, roiDstSize, 1, 1);
		
		ippiFilter32f_8u_AC4R((Ipp8u*)(pDst + 4 + 4*roiDstSize.width),	4*roiDstSize.width, (Ipp8u*)pPixels, 4*nWidth, roiSize, pKernel, kernelSize, anchor);
	}

	if (pKernel)
		delete[] pKernel;
	if (pDst)
		delete[] pDst;
	return TRUE;
}

BOOL IPPEffectMosaic(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size, BOOL bSimple)
{
	IppiSize roiSize;
	roiSize.width	= nWidth;
	roiSize.height	= nHeight;

	int nTilesX = nWidth/Size;
	int nTilesY = nHeight/Size;

	IppiSize roiCellSize;
	roiCellSize.width	= Size;
	roiCellSize.height	= Size;
	IppiSize roiRightBorderCellSize = roiCellSize;
	roiRightBorderCellSize.width	= nWidth - nTilesX*Size;
	IppiSize roiBottomBorderCellSize = roiCellSize;
	roiBottomBorderCellSize.height	= nHeight - nTilesY*Size;
    
	LPBYTE pPixelsP1 = pPixels;
	LPBYTE pPixelsP2 = pPixels;
	int nStep = 4*nWidth;
	int nStepH = 4*Size;
	int nStepV = nStep*Size;

	if (bSimple)
	{	//значение левого верхнего угла
		Ipp8u min[3];
		if (0 != roiBottomBorderCellSize.height)
		{
			for (int i = 0; i < nTilesX; i++)
			{
				pPixelsP2 = pPixelsP1;
				for (int j = 0; j < nTilesY; j++)
				{
					ippiMin_8u_AC4R(pPixelsP2, nStep, roiCellSize, min);
					ippiSet_8u_AC4R(min, pPixelsP2, nStep, roiCellSize);
					pPixelsP2 += nStepV;
				}

				ippiMin_8u_AC4R(pPixelsP2, nStep, roiBottomBorderCellSize, min);
				ippiSet_8u_AC4R(min, pPixelsP2, nStep, roiBottomBorderCellSize);
				
				pPixelsP1 += nStepH;
			}
		}
		else
		{
			for (int i = 0; i < nTilesX; i++)
			{
				pPixelsP2 = pPixelsP1;
				for (int j = 0; j < nTilesY; j++)
				{
					ippiMin_8u_AC4R(pPixelsP2, nStep, roiCellSize, min);
					ippiSet_8u_AC4R(min, pPixelsP2, nStep, roiCellSize);
					pPixelsP2 += nStepV;
				}
				pPixelsP1 += nStepH;
			}
		}
		if (0 != roiRightBorderCellSize.width)
		{
			pPixelsP2 = pPixelsP1;
			for (int j = 0; j < nTilesY; j++)
			{
				ippiMin_8u_AC4R(pPixelsP2, nStep, roiRightBorderCellSize, min);
				ippiSet_8u_AC4R(min, pPixelsP2, nStep, roiRightBorderCellSize);
				pPixelsP2 += nStepV;
			}

			IppiSize oRightBottomCellSize = {roiRightBorderCellSize.width, roiBottomBorderCellSize.height};

			ippiMin_8u_AC4R(pPixelsP2, nStep, oRightBottomCellSize, min);
			ippiSet_8u_AC4R(min, pPixelsP2, nStep, oRightBottomCellSize);
		}
	}
	else
	{//среднее значение по ячейке
		Ipp64f sum[3];
		Ipp8u value[3];
		int nPixelInCell = roiCellSize.height * roiCellSize.width;

		if( 0 != nPixelInCell )
		{
			if (0 != roiBottomBorderCellSize.height)
			{
				int nPixelInBottomCell = roiBottomBorderCellSize.height * roiBottomBorderCellSize.width;
				for (int i = 0; i < nTilesX; i++)
				{
					pPixelsP2 = pPixelsP1;
					for (int j = 0; j < nTilesY; j++)
					{
						ippiSum_8u_AC4R(pPixelsP2, nStep, roiCellSize, sum);

						value[0] = (Ipp8u)(sum[0]/nPixelInCell);
						value[1] = (Ipp8u)(sum[1]/nPixelInCell);
						value[2] = (Ipp8u)(sum[2]/nPixelInCell);
						
						ippiSet_8u_AC4R(value, pPixelsP2, nStep, roiCellSize);
						pPixelsP2 += nStepV;
					}

					if( 0 != nPixelInBottomCell )
					{
						ippiSum_8u_AC4R(pPixelsP2, nStep, roiBottomBorderCellSize, sum);

						value[0] = sum[0]/nPixelInBottomCell;
						value[1] = sum[1]/nPixelInBottomCell;
						value[2] = sum[2]/nPixelInBottomCell;
						ippiSet_8u_AC4R(value, pPixelsP2, nStep, roiBottomBorderCellSize);
					}
					
					pPixelsP1 += nStepH;
				}
			}
			else
			{
				for (int i = 0; i < nTilesX; i++)
				{
					pPixelsP2 = pPixelsP1;
					for (int j = 0; j < nTilesY; j++)
					{
						ippiSum_8u_AC4R(pPixelsP2, nStep, roiCellSize, sum);
						value[0] = sum[0]/nPixelInCell;
						value[1] = sum[1]/nPixelInCell;
						value[2] = sum[2]/nPixelInCell;
						ippiSet_8u_AC4R(value, pPixelsP2, nStep, roiCellSize);
						pPixelsP2 += nStepV;
					}
					pPixelsP1 += nStepH;
				}
			}
		}
		if (0 != roiRightBorderCellSize.width)
		{
			int nPixelInRightCell = roiRightBorderCellSize.height * roiRightBorderCellSize.width;
			pPixelsP2 = pPixelsP1;
			if( 0 != nPixelInRightCell )
			{
				for (int j = 0; j < nTilesY; j++)
				{
					ippiSum_8u_AC4R(pPixelsP2, nStep, roiRightBorderCellSize, sum);
					value[0] = sum[0]/nPixelInRightCell;
					value[1] = sum[1]/nPixelInRightCell;
					value[2] = sum[2]/nPixelInRightCell;
					ippiSet_8u_AC4R(value, pPixelsP2, nStep, roiRightBorderCellSize);
					pPixelsP2 += nStepV;
				}
			}

			IppiSize oRightBottomCellSize = {roiRightBorderCellSize.width, roiBottomBorderCellSize.height};
			int nPixelInRightBottomCell = roiRightBorderCellSize.width * roiBottomBorderCellSize.height;
			if( 0 != nPixelInRightBottomCell )
			{
				ippiSum_8u_AC4R(pPixelsP2, nStep, oRightBottomCellSize, sum);
				value[0] = sum[0]/nPixelInRightBottomCell;
				value[1] = sum[1]/nPixelInRightBottomCell;
				value[2] = sum[2]/nPixelInRightBottomCell;
				ippiSet_8u_AC4R(value, pPixelsP2, nStep, oRightBottomCellSize);
			}
		}		
	}
	return TRUE;
}

BOOL IPPEffectGaussianBlur(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size)
{
	if(Size < 1)
		return TRUE;

	IppiSize roiSize;
	roiSize.width	= nWidth;
	roiSize.height	= nHeight;
	
	int nCount = 2*Size+1;
	Ipp32s* pKernel = new Ipp32s[nCount];

	IppiSize roiBufferExtVSize;
	roiBufferExtVSize.width = nWidth;
	roiBufferExtVSize.height = nHeight + 2*Size;
	Ipp8u* pBufferExtV = new Ipp8u[4*roiBufferExtVSize.width*roiBufferExtVSize.height];

	IppiSize roiBufferExtHSize;
	roiBufferExtHSize.width	= nWidth + 2*Size;
	roiBufferExtHSize.height = nHeight;
	Ipp8u* pBufferExtH = new Ipp8u[4*roiBufferExtHSize.width*roiBufferExtHSize.height];

	if (pKernel && pBufferExtV && pBufferExtH)
	{
		for (int i = 0; i < nCount; ++i)
			pKernel[i] = 1;

		ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBufferExtV, 4*roiBufferExtVSize.width, roiBufferExtVSize, Size, 0);
		ippiFilterColumn_8u_AC4R((Ipp8u*)(pBufferExtV + Size*(4*roiBufferExtVSize.width)), 4*roiBufferExtVSize.width, pPixels, 4*nWidth, roiSize, pKernel, nCount, Size, nCount);

		ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBufferExtH, 4*roiBufferExtHSize.width, roiBufferExtHSize, 0, Size);
		ippiFilterRow_8u_AC4R((Ipp8u*)(pBufferExtH + 4*Size), 4*roiBufferExtHSize.width, pPixels, 4*nWidth, roiSize, pKernel, nCount, Size, nCount);
		
		ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize,  pBufferExtV, 4*roiBufferExtVSize.width, roiBufferExtVSize, Size, 0);
		ippiFilterColumn_8u_AC4R((Ipp8u*)(pBufferExtV + Size*(4*roiBufferExtVSize.width)), 4*roiBufferExtVSize.width, pPixels, 4*nWidth, roiSize, pKernel, nCount, Size, nCount);
		
		ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBufferExtH, 4*roiBufferExtHSize.width, roiBufferExtHSize, 0, Size);
		ippiFilterRow_8u_AC4R((Ipp8u*)(pBufferExtH + 4*Size), 4*roiBufferExtHSize.width, pPixels, 4*nWidth, roiSize, pKernel, nCount, Size, nCount);
	}
	
	if (pBufferExtH)
		delete[] pBufferExtH;
	if (pBufferExtV)
		delete pBufferExtV;
	if (pKernel)
		delete pKernel;

	/*
	IppiSize kernelSize;
	kernelSize.width  = 2*Size+1;
	kernelSize.height  = kernelSize.width;
	
	int nCount = kernelSize.width*kernelSize.height;
	Ipp32s* pKernel = new Ipp32s[nCount];
		
	pKernel[0] = 1;
	for (int i=1; i<nCount;i++)
		pKernel[i] = 1;

	IppiPoint anchor;
	anchor.x = Size;
	anchor.y = Size;

	IppiSize roiBufferSize;
	roiBufferSize.width = m_nWidth + 2*Size;
	roiBufferSize.height = m_nHeight + 2*Size;
	Ipp8u *pBuffer = new Ipp8u[4*roiBufferSize.width*roiBufferSize.height];
	
	
	ippiCopyReplicateBorder_8u_AC4R((Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, pBuffer, 4*roiBufferSize.width, roiBufferSize, Size, Size);	
	ippiFilter_8u_AC4R((Ipp8u*)(pBuffer + 4*Size*(roiBufferSize.width+1)), 4*roiBufferSize.width, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, pKernel, kernelSize, anchor, nCount);

	ippiCopyReplicateBorder_8u_AC4R((Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, pBuffer, 4*roiBufferSize.width, roiBufferSize, Size, Size);	
	ippiFilter_8u_AC4R((Ipp8u*)(pBuffer + 4*Size*(roiBufferSize.width+1)), 4*roiBufferSize.width, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, pKernel, kernelSize, anchor, nCount);

	delete pBuffer;
	*/
	return TRUE;
}



BOOL IPPEffectNoise(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Level, BOOL bMono)
{
	unsigned int nSeed = GetTickCount();
	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	if (bMono)
	{
		Ipp8u* puChanel = new Ipp8u[nWidth*nHeight];
		Ipp16s* psChanel = new Ipp16s[nWidth*nHeight];
		unsigned int nTempSeed = nSeed;

		if (puChanel && psChanel)
		{
			ippiCopy_8u_C4C1R((Ipp8u*)pPixels, 4*nWidth, puChanel, nWidth, roiSize);
			ippiConvert_8u16s_C1R(puChanel, nWidth, psChanel, sizeof(Ipp16s)*nWidth, roiSize);	
			ippiAddRandUniform_Direct_16s_C1IR(psChanel, sizeof(Ipp16s)*nWidth, roiSize, -Level, Level, &nTempSeed);
			ippiConvert_16s8u_C1R(psChanel, sizeof(Ipp16s)*nWidth, puChanel, nWidth, roiSize);	
			ippiCopy_8u_C1C4R(puChanel, nWidth, (Ipp8u*) pPixels, 4*nWidth, roiSize);
			
			nTempSeed = nSeed;
			ippiCopy_8u_C4C1R((Ipp8u*)(pPixels+1), 4*nWidth, puChanel, nWidth, roiSize);
			ippiConvert_8u16s_C1R(puChanel, nWidth, psChanel, sizeof(Ipp16s)*nWidth, roiSize);	
			ippiAddRandUniform_Direct_16s_C1IR(psChanel, sizeof(Ipp16s)*nWidth, roiSize, -Level, Level, &nTempSeed);
			ippiConvert_16s8u_C1R(psChanel, sizeof(Ipp16s)*nWidth, puChanel, nWidth, roiSize);	
			ippiCopy_8u_C1C4R(puChanel, nWidth, (Ipp8u*) (pPixels+1), 4*nWidth, roiSize);
			
			nTempSeed = nSeed;
			ippiCopy_8u_C4C1R((Ipp8u*)(pPixels + 2), 4*nWidth, puChanel, nWidth, roiSize);
			ippiConvert_8u16s_C1R(puChanel, nWidth, psChanel, sizeof(Ipp16s)*nWidth, roiSize);	
			ippiAddRandUniform_Direct_16s_C1IR(psChanel, sizeof(Ipp16s)*nWidth, roiSize, -Level, Level, &nTempSeed);
			ippiConvert_16s8u_C1R(psChanel, sizeof(Ipp16s)*nWidth, puChanel, nWidth, roiSize);	
			ippiCopy_8u_C1C4R(puChanel, nWidth, (Ipp8u*) (pPixels+2), 4*nWidth, roiSize);
		}
		
		if (puChanel)
			delete[] puChanel;
		if (psChanel)
			delete[] psChanel;
	}
	else
	{
		Ipp16s* pDst = new Ipp16s[4*nWidth*nHeight];
		
		if (pDst)
		{
			ippiConvert_8u16s_AC4R((Ipp8u*) pPixels, 4*nWidth, pDst, 4*sizeof(Ipp16s)*nWidth, roiSize);
			ippiAddRandUniform_Direct_16s_AC4IR(pDst, 4*sizeof(Ipp16s)*nWidth, roiSize, -Level, Level, &nSeed);
			ippiConvert_16s8u_AC4R(pDst, 4*sizeof(Ipp16s)*nWidth, (Ipp8u*) pPixels, 4*nWidth, roiSize);	

			delete[] pDst;
		}
	}
	return TRUE;
}

BOOL IPPEffectDiffuse(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Distance)
{
	unsigned int nSeed = GetTickCount();
	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	IppiRect srcROI;
	srcROI.x = 0;
	srcROI.y = 0;
	srcROI.width = nWidth;
	srcROI.height = nHeight;

	Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];
	Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
	Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];

	if (pBuffer && pyMap && pxMap)
	{
		Ipp32f* pTemp = pyMap;
		for (int j = 0; j < nHeight; j++)
		{
			for (int i = 0; i < nWidth; ++i, ++pTemp)
				*pTemp = j;
		}

		for (int i = 0; i < nWidth; i++)
			pxMap[i] = i;
		
		pTemp = pxMap;
		for (int i = 1; i < nHeight; i++)
		{
			pTemp += nWidth;
			memcpy(pTemp, pxMap, sizeof(Ipp32f)*nWidth);
		}

		ippiAddRandUniform_Direct_32f_C1IR(pxMap, sizeof(Ipp32f)*nWidth, roiSize, -Distance, Distance, &nSeed);
		ippiAddRandUniform_Direct_32f_C1IR(pyMap, sizeof(Ipp32f)*nWidth, roiSize, -Distance, Distance, &nSeed);

		ippiThreshold_LTValGTVal_32f_C1IR(pxMap, sizeof(Ipp32f)*nWidth, roiSize, 0, 0, nWidth - 1, nWidth - 1);
		ippiThreshold_LTValGTVal_32f_C1IR(pyMap, sizeof(Ipp32f)*nWidth, roiSize, 0, 0, nHeight - 1, nHeight - 1);

		ippiRemap_8u_AC4R(pPixels, roiSize, 4*nWidth, srcROI, pxMap, sizeof(Ipp32f)*nWidth, pyMap, sizeof(Ipp32f)*nWidth, pBuffer, 4*nWidth, roiSize, IPPI_INTER_NN);

		Ipp8u alpha = (1.0 - Frame) * 255;
		ippiAlphaCompC_8u_AC4R( pPixels, 4*nWidth, alpha, pBuffer, 4*nWidth, 255, pPixels, 4*nWidth, roiSize, ippAlphaOver);
	}

	if (pxMap)
		delete[] pxMap;
	if (pyMap)
		delete[] pyMap;
	if (pBuffer)
		delete[] pBuffer;
	return TRUE;
}


BOOL IPPEffectMinimal(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size)
{
	IppiSize roiSize;
	roiSize.width	= nWidth;
	roiSize.height	= nHeight;

	IppiPoint anchor;
	anchor.x = Size;
	anchor.y = Size;

	IppiSize maskSize;
	maskSize.width = 2*Size+1;
	maskSize.height = 2*Size+1;

	Ipp8u* pBordered = new Ipp8u[4*(nWidth + 2*Size)*(nHeight + 2*Size)];
	Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];

	if (pBordered && pBuffer)
	{
		IppiSize roiBorderedSize;
		roiBorderedSize.width = nWidth + 2*Size;
		roiBorderedSize.height = nHeight + 2*Size;
		
		Ipp8u value[3]; value[0] = value[1] = value[2] = 255;
		
		// копируем в pDst с бордюром ширины Size со всех сторон, заполненным 0 по всем цветам
		ippiCopyConstBorder_8u_AC4R((Ipp8u*)pPixels, 4*nWidth, roiSize, pBordered, 4*roiBorderedSize.width, roiBorderedSize, Size, Size, value);	

		// в pBuffer складываем минимизированный вариант	
		ippiFilterMin_8u_AC4R((Ipp8u*)(pBordered + 4*Size + 4*Size*roiBorderedSize.width), 4*roiBorderedSize.width, (Ipp8u*)pBuffer, 4*nWidth, roiSize, maskSize, anchor);
		
		// складываем минимизированный вариант из pBuffer с исходной картинкой из pBordered учитывая весовой коэффициент (см. определение ippAlphaOver) и помещаем результат в m_pPixels
		Ipp8u alpha = (1.0-Frame) * 255;
		ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBordered + 4*Size + 4*Size*roiBorderedSize.width), 4*roiBorderedSize.width, alpha, pBuffer, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
	}

	if (pBordered)
		delete[] pBordered;
	if (pBuffer)
		delete[] pBuffer;
	
	return TRUE;
}

BOOL IPPEffectMaximal(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size)
{

	IppiSize roiSize;
	roiSize.width	= nWidth;
	roiSize.height	= nHeight;

	IppiPoint anchor;
	anchor.x = Size;
	anchor.y = Size;

	IppiSize maskSize;
	maskSize.width = 2*Size + 1;
	maskSize.height = 2*Size + 1;

	Ipp8u* pBordered = new Ipp8u[4*(nWidth + 2*Size)*(nHeight + 2*Size)];
	Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];

	if (pBordered && pBuffer)
	{
		IppiSize roiBorderedSize;
		roiBorderedSize.width = nWidth + 2*Size;
		roiBorderedSize.height = nHeight + 2*Size;
		
		Ipp8u value[3]; value[0] = value[1] = value[2] = 0;
		
		// копируем в pDst с бордюром ширины Size со всех сторон, заполненным 0 по всем цветам
		ippiCopyConstBorder_8u_AC4R((Ipp8u*)pPixels, 4*nWidth, roiSize, pBordered, 4*roiBorderedSize.width, roiBorderedSize, Size, Size, value);	

		// в pBuffer складываем максимизированный вариант 
		ippiFilterMax_8u_AC4R((Ipp8u*)(pBordered + 4*Size + 4*Size*roiBorderedSize.width), 4*roiBorderedSize.width, (Ipp8u*)pBuffer, 4*nWidth, roiSize, maskSize, anchor);
		
		// складываем максимизированный вариант из pBuffer с исходной картинкой из pBordered учитывая весовой коэффициент (см. определение ippAlphaOver) и помещаем результат в m_pPixels
		Ipp8u alpha = (1.0 - Frame) * 255;
		ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBordered + 4*Size + 4*Size*roiBorderedSize.width), 4*roiBorderedSize.width, alpha, pBuffer, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
	}

	if (pBordered)
		delete[] pBordered;
	if (pBuffer)
		delete[] pBuffer;
	return TRUE;
}

BOOL IPPEffectDeinterlace(LPBYTE pPixels, int nWidth, int nHeight, double Frame, BOOL bEven, BOOL bInterpolate)
{

	IppiSize roiSize;
	roiSize.width	= nWidth;
	roiSize.height	= nHeight;
	
	if (bInterpolate)
	{
		Ipp8u alpha = (1.0 - Frame) * 255;
		Ipp8u* pBuffer = new Ipp8u[4*nWidth];

		if (pBuffer)
		{
			roiSize.height = 1;
			int nStep = 4*nWidth;
			int nDoubleStep = 2*nStep;
			if (bEven)
			{			
				int nCount = nHeight/2 - 1;
				Ipp8u *pCurr = pPixels;
				Ipp8u *pPrev = pPixels - nStep;
				Ipp8u *pNext = pPixels + nStep;

				// нулевая строка
				ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pNext, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);
				
				pPrev += nDoubleStep;
				pNext += nDoubleStep;
				pCurr+= nDoubleStep;

				for (int i = 0; i < nCount; i++)
				{
					ippiAdd_8u_AC4RSfs(pPrev, nStep, pNext, nStep, pBuffer, nStep, roiSize, 1);
					ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pBuffer, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);

					pPrev += nDoubleStep;
					pNext += nDoubleStep;
					pCurr+= nDoubleStep;
				}

				if (0 != nHeight % 2)
					ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pPrev, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);
			}
			else
			{			
				int nCount = (nHeight - 1)/2;
				Ipp8u *pCurr = pPixels + nStep;
				Ipp8u *pPrev = pPixels;
				Ipp8u *pNext = pPixels + nDoubleStep;

				for (int i = 0; i < nCount; i++)
				{
					ippiAdd_8u_AC4RSfs(pPrev, nStep, pNext, nStep, pBuffer, nStep, roiSize, 1);
					ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pBuffer, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);
					pPrev += nDoubleStep;
					pNext += nDoubleStep;
					pCurr+= nDoubleStep;
				}

				if (0 == nHeight % 2)
					ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pPrev, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);
			}

			delete[] pBuffer;
		}
	}
	else
	{
		Ipp8u alpha = (1.0 - Frame) * 255;
		roiSize.height = 1;
		int nStep = 4*nWidth;
		int nDoubleStep = 2*nStep;

		if (bEven)
		{			
			int nCount = nHeight/2;
			Ipp8u *pCurr = pPixels;
			Ipp8u *pTemp = pPixels + nStep;

			for (int i = 0; i < nCount; i++)
			{
				ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pTemp, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);

				pTemp += nDoubleStep;
				pCurr+= nDoubleStep;
			}

			if (0 != nHeight % 2)
			{
				pTemp -= nDoubleStep;
				ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pTemp, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);
			}
		}
		else
		{			
			Ipp8u *pCurr = pPixels + nStep;
			Ipp8u *pTemp = pPixels + nDoubleStep;	
			int nCount = (nHeight - 1)/2;

			for (int i = 0; i < nCount; i++)
			{
				ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pTemp, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);
				
				pTemp += nDoubleStep;
				pCurr+= nDoubleStep;
			}

			if (0 == nHeight % 2)
			{
				pTemp -= nDoubleStep;
				ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pTemp, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);
			}
		}
	}
	return TRUE;
}

BOOL IPPEffectMedian(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long MedianType)
{
	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	switch (MedianType)
	{
	case c_nMedianHorizontal3:
		{
			IppiSize roiBuffer;
			roiBuffer.width = nWidth + 2;
			roiBuffer.height = nHeight;

			Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

			if (pBuffer)
			{
				ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 0, 1);
				ippiFilterMedianHoriz_8u_AC4R((Ipp8u*)(pBuffer + 4), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize3x1);

				Ipp8u alpha = (1.0 - Frame) * 255;
				ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 4), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
			
				delete[] pBuffer;
			}
		}
		break;
	case c_nMedianHorizontal5:
		{
			IppiSize roiBuffer;
			roiBuffer.width = nWidth + 4;
			roiBuffer.height = nHeight;

			Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

			if (pBuffer)
			{
				ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 0, 2);
				ippiFilterMedianHoriz_8u_AC4R((Ipp8u*)(pBuffer + 8), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize5x1);

				Ipp8u alpha = (1.0 - Frame) * 255;
				ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 8), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
                		
				delete[] pBuffer;
			}
		}
		break;
	case c_nMedianVertical3:
		{
			IppiSize roiBuffer;
			roiBuffer.width = nWidth;
			roiBuffer.height = nHeight + 2;

			Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

			if (pBuffer)
			{
				ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 0);
				ippiFilterMedianVert_8u_AC4R((Ipp8u*)(pBuffer + 4*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize1x3);

				Ipp8u alpha = (1.0 - Frame) * 255;
				ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 4*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
		
				delete[] pBuffer;
			}
		}
		break;
	case c_nMedianVertical5:
		{
			IppiSize roiBuffer;
			roiBuffer.width = nWidth;
			roiBuffer.height = nHeight + 4;

			Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

			if (pBuffer)
			{
				ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 2, 0);
				ippiFilterMedianVert_8u_AC4R((Ipp8u*)(pBuffer + 8*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize1x5);

				Ipp8u alpha = (1.0 - Frame) * 255;
				ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 8*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
		
				delete[] pBuffer;
			}
		}
		break;
	case c_nMedianCross3:
		{
			IppiSize roiBuffer;
			roiBuffer.width = nWidth + 2;
			roiBuffer.height = nHeight + 2;

			Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

			if (pBuffer)
			{
				ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 1);
				ippiFilterMedianCross_8u_AC4R((Ipp8u*)(pBuffer + 4 + 4*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize, ippMskSize3x3);

				Ipp8u alpha = (1.0 - Frame) * 255;
				ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 4 + 4*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
		
				delete[] pBuffer;
			}
		}
		break;
	case c_nMedianCross5:		
		{
			IppiSize roiBuffer;
			roiBuffer.width = nWidth + 4;
			roiBuffer.height = nHeight + 4;

			Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

			if (pBuffer)
			{
				ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 2, 2);		
				ippiFilterMedianCross_8u_AC4R((Ipp8u*)(pBuffer + 8 + 8*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize5x5);

				Ipp8u alpha = (1.0 - Frame) * 255;
				ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 8 + 8*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
		
				delete[] pBuffer;
			}
		}
		break;

	default:

		IppiSize roiBuffer;
		roiBuffer.width = nWidth + 2*MedianType;
		roiBuffer.height = nHeight + 2*MedianType;

		Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

		if (pBuffer)
		{
			ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, MedianType, MedianType);
			
			IppiPoint anchor = {MedianType, MedianType};
			IppiSize  maskSize = {2*MedianType + 1, 2*MedianType + 1};
			ippiFilterMedian_8u_AC4R((Ipp8u*)(pBuffer + 4*MedianType + 4*MedianType*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize, maskSize, anchor);

			Ipp8u alpha = (1.0 - Frame) * 255;
			ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 4*MedianType + 4*MedianType*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
		
			delete[] pBuffer;
		}
	}

	return TRUE;
}



BOOL IPPFilterPrewitt(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type)
{
	if (Frame<c_dFrameMin)
		return TRUE;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiSize roiBuffer;
	roiBuffer.width = nWidth + 2;
	roiBuffer.height = nHeight + 2;

	Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
	if (!pBuffer)
		return FALSE;

	ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 1);

	if (c_nFilterPrewittHoriz == Type)
	{
		ippiFilterPrewittHoriz_8u_AC4R(
			(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
			pPixels, 4*nWidth, roiSize);
	}
	else if (c_nFilterPrewittVertical == Type)
	{
		ippiFilterPrewittVert_8u_AC4R(
			(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
			pPixels, 4*nWidth, roiSize);
	}
	else
	{
		delete []pBuffer;
		return FALSE;
	}

	if (Frame<c_dFrameMax)
	{
		Ipp8u alpha = (1.0-Frame) * 255;	
		ippiAlphaCompC_8u_AC4R( 
			(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, alpha,
			pPixels, 4*nWidth, 255, 
			pPixels, 4*nWidth, roiSize, ippAlphaOver);

	}

	delete [] pBuffer;
	return TRUE;
}

BOOL IPPFilterScharr(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type)
{
	if (Frame<c_dFrameMin)
		return TRUE;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiSize roiBuffer;
	roiBuffer.width = nWidth + 2;
	roiBuffer.height = nHeight + 2;

	Ipp8u* pChanel = new Ipp8u[roiBuffer.width*roiBuffer.height];
	if (!pChanel)
		return FALSE;


	ippiCopy_8u_C4C1R(pPixels, 4*nWidth, 
		(Ipp8u*)(pChanel + roiBuffer.width + 1), roiBuffer.width, roiSize);

	ippiCopyReplicateBorder_8u_C1R(
		(Ipp8u*)(pChanel + roiBuffer.width + 1), roiBuffer.width, roiSize, 
		pChanel, roiBuffer.width, roiBuffer, 1, 1);

	Ipp16s* pExtChanel = new Ipp16s[nWidth*nHeight];
	if (!pExtChanel)
	{
		delete []pChanel;
		return FALSE;
	}
	if (c_nFilterScharrHoriz == Type)
	{
		ippiFilterScharrHoriz_8u16s_C1R(
			(Ipp8u*)(pChanel+roiBuffer.width + 1), roiBuffer.width, 
			pExtChanel, 2*nWidth, roiSize);
	}
	else if (c_nFilterScharrVertical == Type)
	{
		ippiFilterScharrVert_8u16s_C1R(
			(Ipp8u*)(pChanel+roiBuffer.width + 1), roiBuffer.width, 
			pExtChanel, 2*nWidth, roiSize);
	}
	else
	{
		delete []pChanel;
		delete []pExtChanel;
		return FALSE;
	}

	ippiConvert_16s8u_C1R(pExtChanel, 2*nWidth, pChanel, nWidth, roiSize);

	delete []pExtChanel;
	if (Frame>c_dFrameMax)
	{
		ippiCopy_8u_C1C4R(pChanel, nWidth, pPixels, 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pPixels + 1), 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pPixels + 2), 4*nWidth, roiSize);
	}
	else
	{
		Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];
		if (!pBuffer)
		{
			delete []pChanel;
			return FALSE;
		}
		ippiCopy_8u_C1C4R(pChanel, nWidth, pBuffer, 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pBuffer + 1), 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pBuffer + 2), 4*nWidth, roiSize);

		Ipp8u alpha = Frame * 255;	
		ippiAlphaCompC_8u_AC4R( 
			pBuffer, 4*nWidth, alpha,
			pPixels, 4*nWidth, 255, 
			pPixels, 4*nWidth, roiSize, ippAlphaOver);

		delete []pBuffer;
	}

	delete []pChanel;
	return TRUE;
}

BOOL IPPFilterSobel(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type)
{
	if (Frame<c_dFrameMin)
		return TRUE;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiSize roiBuffer;
	IppiMaskSize mask;

	int nOffset;
	switch (Type)
	{
	case c_nFilterSobelHoriz3x3:
	case c_nFilterSobelVertical3x3:
	case c_nFilterSobelCross3x3:
	case c_nFilterSobelSecondHoriz3x3:
	case c_nFilterSobelSecondVertical3x3:
		{
			roiBuffer.width = nWidth + 2;
			roiBuffer.height = nHeight + 2;
			nOffset = roiBuffer.width + 1;
			mask = ippMskSize3x3;
		}
		break;
	case c_nFilterSobelHoriz5x5:
	case c_nFilterSobelVertical5x5:
	case c_nFilterSobelCross5x5:
	case c_nFilterSobelSecondHoriz5x5:
	case c_nFilterSobelSecondVertical5x5:
		{
			roiBuffer.width = nWidth + 4;
			roiBuffer.height = nHeight + 4;
			nOffset = 2*roiBuffer.width + 2;
			mask = ippMskSize5x5;
		}
		break;
	}

	Ipp8u* pChanel = new Ipp8u[roiBuffer.width*roiBuffer.height];
	if (!pChanel)
		return FALSE;

	if (ippMskSize3x3==mask)
	{
		ippiCopy_8u_C4C1R(pPixels, 4*nWidth, 
			(Ipp8u*)(pChanel + nOffset), roiBuffer.width, roiSize);

		ippiCopyReplicateBorder_8u_C1R(
			(Ipp8u*)(pChanel + nOffset), roiBuffer.width, roiSize, 
			pChanel, roiBuffer.width, roiBuffer, 1, 1);
	}
	else
	{
		ippiCopy_8u_C4C1R(pPixels, 4*nWidth, 
			(Ipp8u*)(pChanel + nOffset), roiBuffer.width, roiSize);

		ippiCopyReplicateBorder_8u_C1R(
			(Ipp8u*)(pChanel + nOffset), roiBuffer.width, roiSize, 
			pChanel, roiBuffer.width, roiBuffer, 2, 2);
	}

	Ipp16s* pExtChanel = new Ipp16s[nWidth*nHeight];
	if (!pExtChanel)
	{
		delete []pChanel;
		return FALSE;
	}

	switch (Type)
	{
	case c_nFilterSobelHoriz3x3:
	case c_nFilterSobelHoriz5x5:
		ippiFilterSobelHoriz_8u16s_C1R(
			(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
			pExtChanel, 2*nWidth, roiSize, mask);
		break;
	case c_nFilterSobelVertical3x3:
	case c_nFilterSobelVertical5x5:
		ippiFilterSobelVert_8u16s_C1R(
			(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
			pExtChanel, 2*nWidth, roiSize, mask);
		break;
	case c_nFilterSobelCross3x3:
	case c_nFilterSobelCross5x5:
		ippiFilterSobelCross_8u16s_C1R(
			(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
			pExtChanel, 2*nWidth, roiSize, mask);
		break;
	case c_nFilterSobelSecondHoriz3x3:
	case c_nFilterSobelSecondHoriz5x5:
		ippiFilterSobelHorizSecond_8u16s_C1R(
			(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
			pExtChanel, 2*nWidth, roiSize, mask);
		break;
	case c_nFilterSobelSecondVertical3x3:
	case c_nFilterSobelSecondVertical5x5:
		ippiFilterSobelVertSecond_8u16s_C1R(
			(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
			pExtChanel, 2*nWidth, roiSize, mask);
		break;
	default:
		{
			delete []pChanel;
			delete []pExtChanel;
			return FALSE;
		}
	}

	ippiConvert_16s8u_C1R(pExtChanel, 2*nWidth, pChanel, nWidth, roiSize);

	delete []pExtChanel;
	if (Frame>c_dFrameMax)
	{
		ippiCopy_8u_C1C4R(pChanel, nWidth, pPixels, 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pPixels + 1), 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pPixels + 2), 4*nWidth, roiSize);
	}
	else
	{
		Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];
		if (!pBuffer)
		{
			delete []pChanel;
			return FALSE;
		}
		ippiCopy_8u_C1C4R(pChanel, nWidth, pBuffer, 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pBuffer + 1), 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pBuffer + 2), 4*nWidth, roiSize);

		Ipp8u alpha = Frame * 255;	
		ippiAlphaCompC_8u_AC4R( 
			pBuffer, 4*nWidth, alpha,
			pPixels, 4*nWidth, 255, 
			pPixels, 4*nWidth, roiSize, ippAlphaOver);

		delete []pBuffer;
	}

	delete []pChanel;
	return TRUE;
}


BOOL IPPFilterSobelExt(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type)
{
	if (Frame<c_dFrameMin)
		return TRUE;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiSize roiBuffer;
	roiBuffer.width = nWidth + 2;
	roiBuffer.height = nHeight + 2;

	Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
	if (!pBuffer)
		return FALSE;

	ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 1);

	if (c_nFilterSobelExtHoriz == Type)
	{
		ippiFilterSobelHoriz_8u_AC4R(
			(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
			pPixels, 4*nWidth, roiSize);
	}
	else if (c_nFilterSobelExtVertical == Type)
	{
		ippiFilterSobelVert_8u_AC4R(
			(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
			pPixels, 4*nWidth, roiSize);
	}
	else
	{
		delete []pBuffer;
		return FALSE;
	}

	if (Frame<c_dFrameMax)
	{
		Ipp8u alpha = (1.0-Frame) * 255;	
		ippiAlphaCompC_8u_AC4R( 
			(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, alpha,
			pPixels, 4*nWidth, 255, 
			pPixels, 4*nWidth, roiSize, ippAlphaOver);

	}

	delete [] pBuffer;
	return TRUE;
}


BOOL IPPFilterRoberts(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type)
{
	if (Frame<c_dFrameMin)
		return TRUE;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiSize roiBuffer;
	roiBuffer.width = nWidth + 2;
	roiBuffer.height = nHeight + 2;

	Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
	if (!pBuffer)
		return FALSE;

	ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 1);

	if (c_nFilterRobertsUp == Type)
	{
		ippiFilterRobertsUp_8u_AC4R(
			(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
			pPixels, 4*nWidth, roiSize);
	}
	else if (c_nFilterRobertsDown == Type)
	{
		ippiFilterRobertsDown_8u_AC4R(
			(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
			pPixels, 4*nWidth, roiSize);
	}
	else
	{
		delete []pBuffer;
		return FALSE;
	}

	if (Frame<c_dFrameMax)
	{
		Ipp8u alpha = (1.0-Frame) * 255;	
		ippiAlphaCompC_8u_AC4R( 
			(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, alpha,
			pPixels, 4*nWidth, 255, 
			pPixels, 4*nWidth, roiSize, ippAlphaOver);

	}

	delete [] pBuffer;
	return TRUE;
}


BOOL IPPFilterLaplace(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type)
{
	if (Frame<c_dFrameMin)
		return TRUE;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiSize roiBuffer;
	IppiMaskSize mask;
	int nOffset;
	int nBorder;
	if (c_nFilterLaplace3x3 == Type)
	{
		nBorder = 1;
		roiBuffer.width = nWidth + 2;
		roiBuffer.height = nHeight + 2;
		mask = ippMskSize3x3;
		nOffset = 4*roiBuffer.width + 4;
	}
	else if (c_nFilterLaplace5x5 == Type)
	{	
		nBorder = 2;
		roiBuffer.width = nWidth + 4;
		roiBuffer.height = nHeight + 4;
		mask = ippMskSize5x5;
		nOffset = 8*roiBuffer.width + 8;
	}
	else
		return FALSE;

	Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
	if (!pBuffer)
		return FALSE;

	ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, nBorder, nBorder);

	ippiFilterLaplace_8u_AC4R(
		(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, 
		pPixels, 4*nWidth, roiSize, mask);

	if (Frame<c_dFrameMax)
	{
		Ipp8u alpha = (1.0-Frame) * 255;	
		ippiAlphaCompC_8u_AC4R( 
			(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, alpha,
			pPixels, 4*nWidth, 255, 
			pPixels, 4*nWidth, roiSize, ippAlphaOver);

	}

	delete [] pBuffer;
	return TRUE;
}

BOOL IPPFilterHipass(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type)
{
	if (Frame<c_dFrameMin)
		return TRUE;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiSize roiBuffer;
	IppiMaskSize mask;
	int nOffset;
	int nBorder;
	if (c_nFilterHipass3x3 == Type)
	{
		nBorder = 1;
		roiBuffer.width = nWidth + 2;
		roiBuffer.height = nHeight + 2;
		mask = ippMskSize3x3;
		nOffset = 4*roiBuffer.width + 4;
	}
	else if (c_nFilterHipass5x5 == Type)
	{	
		nBorder = 2;
		roiBuffer.width = nWidth + 4;
		roiBuffer.height = nHeight + 4;
		mask = ippMskSize5x5;
		nOffset = 8*roiBuffer.width + 8;
	}
	else
		return FALSE;

	Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
	if (!pBuffer)
		return FALSE;


	ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, nBorder, nBorder);

	ippiFilterHipass_8u_AC4R(
		(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, 
		pPixels, 4*nWidth, roiSize, mask);

	if (Frame<c_dFrameMax)
	{
		Ipp8u alpha = (1.0-Frame) * 255;	
		ippiAlphaCompC_8u_AC4R( 
			(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, alpha,
			pPixels, 4*nWidth, 255, 
			pPixels, 4*nWidth, roiSize, ippAlphaOver);

	}

	delete [] pBuffer;
	return TRUE;
}

BOOL IPPFilterLowpass(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type)
{
	if (Frame<c_dFrameMin)
		return TRUE;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiSize roiBuffer;
	IppiMaskSize mask;
	int nOffset;
	int nBorder;
	if (c_nFilterLowpass3x3 == Type)
	{
		nBorder = 1;
		roiBuffer.width = nWidth + 2;
		roiBuffer.height = nHeight + 2;
		mask = ippMskSize3x3;
		nOffset = 4*roiBuffer.width + 4;
	}
	else if (c_nFilterLowpass5x5 == Type)
	{	
		nBorder = 2;
		roiBuffer.width = nWidth + 4;
		roiBuffer.height = nHeight + 4;
		mask = ippMskSize5x5;
		nOffset = 8*roiBuffer.width + 8;
	}
	else
		return FALSE;

	Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
	if (!pBuffer)
		return FALSE;

	ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, nBorder, nBorder);

	ippiFilterLowpass_8u_AC4R(
		(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, 
		pPixels, 4*nWidth, roiSize, mask);

	if (Frame<c_dFrameMax)
	{
		Ipp8u alpha = (1.0-Frame) * 255;	
		ippiAlphaCompC_8u_AC4R( 
			(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, alpha,
			pPixels, 4*nWidth, 255, 
			pPixels, 4*nWidth, roiSize, ippAlphaOver);

	}

	delete [] pBuffer;
	return TRUE;
}

BOOL IPPFilterBlur(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size)
{
	if (Frame<c_dFrameMin)
		return TRUE;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;

	IppiSize roiBuffer = {nWidth + 2*Size, nHeight + 2*Size};
	IppiSize maskSize = {2*Size+1,2*Size+1};	
	IppiPoint anchor = {Size,Size};
	int nOffset = (4*roiBuffer.width + 4)*Size;

	Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
	if (!pBuffer)
		return FALSE;

	ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, Size, Size);

	ippiFilterBox_8u_AC4R(
		(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, 
		pPixels, 4*nWidth, roiSize, maskSize, anchor);

	if (Frame<c_dFrameMax)
	{
		Ipp8u alpha = (1.0-Frame) * 255;	
		ippiAlphaCompC_8u_AC4R( 
			(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, alpha,
			pPixels, 4*nWidth, 255, 
			pPixels, 4*nWidth, roiSize, ippAlphaOver);

	}

	delete [] pBuffer;
	return TRUE;
}

BOOL IPPFilterCanny(LPBYTE pPixels, int nWidth, int nHeight, double Frame,  double LowThresh, double HighThresh)
{
	if (Frame<c_dFrameMin)
		return S_OK;

	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	Ipp8u *pGrayScaleBuf = new Ipp8u[nWidth*nHeight];
	Ipp16s *pDXBuf = new Ipp16s[nWidth*nHeight];
	Ipp16s *pDYBuf = new Ipp16s[nWidth*nHeight];
	
	ippiCopy_8u_C4C1R(pPixels, 4*nWidth, 
		pGrayScaleBuf, nWidth, roiSize);


	ippiSobel3x3_Dx_8u16s_C1R(pGrayScaleBuf, nWidth, pDXBuf, 2*nWidth, roiSize);
	ippiSobel3x3_Dy_8u16s_C1R(pGrayScaleBuf, nWidth, pDYBuf, 2*nWidth, IPCV_ORIGIN_TL, roiSize);

	int nBufSize;
	ippiCannyGetSize(roiSize, &nBufSize);
	LPBYTE pBuffer = new BYTE[nBufSize];
	ippiCanny_16s8u_C1R(pDXBuf, 2*nWidth,
						pDYBuf, 2*nWidth,
						pGrayScaleBuf, nWidth, roiSize, 
						LowThresh, HighThresh,
						pBuffer);

	delete [] pBuffer;

	if (c_dFrameMax<Frame)
	{
		ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
							pPixels, 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
							(Ipp8u*)(pPixels+1), 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
							(Ipp8u*)(pPixels+2), 4*nWidth, roiSize);
	}
	else
	{
		Ipp8u *pTemp = new Ipp8u[4*nWidth*nHeight];
		ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
							pTemp, 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
							(Ipp8u*)(pTemp+1), 4*nWidth, roiSize);
		ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
							(Ipp8u*)(pTemp+2), 4*nWidth, roiSize);

		Ipp8u alpha = Frame * 255;	
		ippiAlphaCompC_8u_AC4R( 
			(Ipp8u*)pTemp, 4*nWidth, alpha,
			pPixels, 4*nWidth, 255, 
			(Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);

		delete [] pTemp;
	}
	
	delete [] pGrayScaleBuf;
	delete [] pDXBuf;
	delete [] pDYBuf;
	return TRUE;
}


BOOL IPPUtilitySwapChannels(LPBYTE pPixels, int nWidth, int nHeight, BYTE nChannel0, BYTE nChannel1, BYTE nChannel2)
{
	IppiSize roiSize;
	roiSize.width = nWidth;
	roiSize.height = nHeight;
	//на месте работает только для 8u_С3 поэтому свопим каналы через буффер
	//+ так как можем задать раскопирование одного канала в три
	//маловероятно, что можно работать на месте.
	Ipp8u *pBuffer = new Ipp8u[4*nWidth*nHeight];
	if (!pBuffer)
		return FALSE;

	const int dstOrder[3] = {nChannel0,nChannel1,nChannel2};
	
	ippiSwapChannels_8u_AC4R(pPixels, 4*nWidth, pBuffer, 4*nWidth, roiSize, dstOrder);
	
	memcpy(pPixels,pBuffer,4*nWidth*nHeight);
	delete []pBuffer;
	return TRUE;
}

BOOL IPPTransformTwirl(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Angle, long Degree, DWORD dwBackColor)
{
	Angle *= Frame;

	Ipp8u*  pBuffer = new Ipp8u[nWidth*nHeight*4];
	Ipp32f* pxMap   = new Ipp32f[nWidth*nHeight];
	Ipp32f* pyMap   = new Ipp32f[nWidth*nHeight];

	int nArraySize = max(nWidth, nHeight); // размер массива для синусов/косинусов
	Ipp32fc* pSinCosArray = new Ipp32fc[nArraySize + 1];

	if( pBuffer && pxMap && pyMap && pSinCosArray )
	{
		Angle *= IPP_PI180;

		for( int i = 0; i <= nArraySize; i++ )
		{
			double ang = pow(1 - sqrt(double(i) / nArraySize), Degree) * Angle;
			pSinCosArray[i].re = sin(ang);
			pSinCosArray[i].im = cos(ang);
		}
		
		IppiPoint MinPoint = {
			-nWidth / 2,
			-nHeight / 2,
		};
		IppiPoint MaxPoint = {
			MinPoint.x + nWidth,
			MinPoint.y + nHeight
		};

		Ipp32f* pxMapCur = pxMap;
		Ipp32f* pyMapCur = pyMap;

		double dMaxRadius = 1.0 / (MaxPoint.x * MaxPoint.x + MaxPoint.y * MaxPoint.y);

		for( int nRow = MinPoint.y; nRow < MaxPoint.y; ++nRow)
		{
			for( int nPos = MinPoint.x; nPos < MaxPoint.x; ++nPos, ++pxMapCur, ++pyMapCur )
			{
				double dR = (nRow*nRow + nPos*nPos) * dMaxRadius * nArraySize;
				int index = int(dR);
				dR -= index;
				
				double dSin = dR*(pSinCosArray[index + 1].re - pSinCosArray[index].re) + pSinCosArray[index].re;
				double dCos = dR*(pSinCosArray[index + 1].im - pSinCosArray[index].im) + pSinCosArray[index].im;

				*pxMapCur = float(nPos * dCos - nRow * dSin - MinPoint.x);
				*pyMapCur = float(nPos * dSin + nRow * dCos - MinPoint.y);
			}
		}

		// закрашиваем фоновым цветом
		IppiRect SrcRect = {0, 0, nWidth, nHeight};
		IppiSize SrcSize = {nWidth, nHeight};

		Ipp8u Colors[4] = {
			GetBValue(dwBackColor), 
			GetGValue(dwBackColor), 
			GetRValue(dwBackColor),
			255
		};

		ippiSet_8u_C4R( Colors, pBuffer, nWidth*4, SrcSize );

		// ремепим
		ippiRemap_8u_C4R(pPixels, SrcSize, nWidth*4, SrcRect, 
			pxMap, sizeof(Ipp32f)*nWidth, 
			pyMap, sizeof(Ipp32f)*nWidth, 
			pBuffer, nWidth*4, SrcSize, IPPI_INTER_LINEAR);

		// копируем буфер обратно
		ippiCopy_8u_C4R( pBuffer, nWidth*4, pPixels, nWidth*4, SrcSize );
	}

	if( pSinCosArray )
		delete []pSinCosArray;

	if (pyMap)
		delete []pyMap;

	if (pxMap)
		delete []pxMap;

	if (pBuffer)
		delete []pBuffer;
		
	return TRUE;
}

BOOL IPPTransformSphere(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long CenterX, long CenterY,  long RadiusX, long RadiusY, double Degree, DWORD dwBackColor)
{
	Degree *= Frame;

	double dRadiusX = abs(RadiusX * Frame);
	double dRadiusY = abs(RadiusY * Frame);

	IppiPoint nRadius = {
		int(dRadiusX) + 1,
		int(dRadiusY) + 1
	};

	// сферу без радиуса бестолку отрисовывать!
	if( nRadius.x <= 1 || nRadius.y <= 1 )
		return TRUE;

	// расчитываем прямоугольник, описанный возле сферы
	IppiRect BoundRect = {
		CenterX - nRadius.x,
		CenterY - nRadius.y,
		nRadius.x * 2,
		nRadius.y * 2
	};

	if( BoundRect.x >= nWidth || BoundRect.y >= nHeight )
		return TRUE;

	if( BoundRect.x < 0 )
	{
		if( BoundRect.x + BoundRect.width <= 0 )
			return TRUE;

		BoundRect.width += BoundRect.x;
		BoundRect.x = 0;
	}

	if( BoundRect.y < 0 )
	{
		if( BoundRect.y + BoundRect.height <= 0 )
			return TRUE;

		BoundRect.height += BoundRect.y;
		BoundRect.y = 0;
	}

	if( BoundRect.x + BoundRect.width > nWidth )
		BoundRect.width = nWidth - BoundRect.x;

	if( BoundRect.y + BoundRect.height > nHeight )
		BoundRect.height = nHeight - BoundRect.y;

	// определяем дополнительные переменные
	int nMaxRadius = max( nRadius.x, nRadius.y );

	// создаём необходимые буфера
	Ipp8u*  pBuffer = new Ipp8u[BoundRect.width*BoundRect.height*4];
	Ipp32f* pMapX   = new Ipp32f[BoundRect.width*BoundRect.height];
	Ipp32f* pMapY   = new Ipp32f[BoundRect.width*BoundRect.height];
	Ipp32f* pOffset = new Ipp32f[nMaxRadius + 1];
	
	//dRadiusX *= dRadiusX;
	//dRadiusY *= dRadiusY;
	dRadiusX = 1/(dRadiusX * dRadiusX);
	dRadiusY = 1/(dRadiusY * dRadiusY);

	if( pBuffer && pMapX && pMapY && pOffset )
	{
		Degree /= 10;
		for( int i = 0; i <= nMaxRadius; i++ )
		{
			//pOffset[i] = float(pow( double(i + 1) / (nMaxRadius + 1), Degree )); // чёткий контур
			pOffset[i] = float(pow( sin( double(i + 1) / (nMaxRadius + 1) * IPP_PI2 ), Degree )); // сглаженный контур
		}

		int nMaxRow = BoundRect.y + BoundRect.height;
		int nMaxPos = BoundRect.x + BoundRect.width;

		Ipp32f* pCurMapX = pMapX;
		Ipp32f* pCurMapY = pMapY;

		for( int nRow = BoundRect.y; nRow < nMaxRow; ++nRow )
		{
			int nY = nRow - CenterY;
			double dFactor = nY * nY * dRadiusY;

			for( int nPos = BoundRect.x; nPos < nMaxPos; ++nPos, ++pCurMapX, ++pCurMapY )
			{
				int nX = nPos - CenterX;
				double dR = nX * nX * dRadiusX + dFactor;

				if( dR < 1 )
				{
					// ближайшее значение (без интерполяции)
					//int index = int(dR * nMaxRadius + 0.5);
					//*pCurMapX = float(nX * pOffset[index] + CenterX);
					//*pCurMapY = float(nY * pOffset[index] + CenterY);

					// интерполяция
					dR *= nMaxRadius;
					int index = int(dR);
					dR = (dR - index)*(pOffset[index + 1] - pOffset[index]) + pOffset[index];
					*pCurMapX = float(nX * dR + CenterX);
					*pCurMapY = float(nY * dR + CenterY);
				}
				else
				{
					*pCurMapX = nPos;
					*pCurMapY = nRow;
				}
			}
		}
		
		// закрашиваем фоновым цветом
		IppiSize DstSize = {BoundRect.width, BoundRect.height};

		Ipp8u Colors[4] = {
			GetBValue(dwBackColor), 
			GetGValue(dwBackColor), 
			GetRValue(dwBackColor),
			255
		};

		ippiSet_8u_C4R( Colors, pBuffer, BoundRect.width*4, DstSize );

		// ремэпим
		IppiSize SrcSize = {nWidth, nHeight};
		IppiRect SrcRect = {0, 0, nWidth, nHeight};

		ippiRemap_8u_C4R( pPixels, SrcSize, nWidth*4, SrcRect, 
			pMapX, sizeof(Ipp32f)*BoundRect.width, 
			pMapY, sizeof(Ipp32f)*BoundRect.width, 
			pBuffer, BoundRect.width*4, DstSize, IPPI_INTER_LINEAR);

		// копируем полученную картинку обратно
		Ipp8u* pDstPixels = pPixels + (BoundRect.y * nWidth + BoundRect.x) * 4;
		ippiCopy_8u_C4R( pBuffer, BoundRect.width*4, pDstPixels, nWidth*4, DstSize );
	}

	if (pOffset)
		delete []pOffset;
	if (pMapX)
		delete []pMapX;
	if (pMapY)
		delete []pMapY;
	if (pBuffer)
		delete []pBuffer;
	
	return TRUE;
}

BOOL IPPTransformCylinder(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Center, long Radius, double Degree, long nType, DWORD dwBackColor)
{
	Degree *= Frame;
	
	double dRadius = Radius * Frame;
	int nMaxRadius = int(abs(dRadius)) + 1;
	if( nMaxRadius <= 1 )
		return TRUE;

	IppiRect BoundRect = {0, 0, nWidth, nHeight};

	if( c_nTransformCylinderHoriz == nType )
	{
		BoundRect.y = Center - nMaxRadius;
		BoundRect.height = nMaxRadius * 2;

		if( BoundRect.y >= nHeight )
			return TRUE;

		if( BoundRect.y < 0 )
		{
			if( BoundRect.y + BoundRect.height <= 0 )
				return TRUE;

			BoundRect.height += BoundRect.y;
			BoundRect.y = 0;
		}

		if( BoundRect.y + BoundRect.height > nHeight )
		{
			BoundRect.height = nHeight - BoundRect.y;
		}
	}
	else
	{
		BoundRect.x = Center - nMaxRadius;
		BoundRect.width = nMaxRadius * 2;

		if( BoundRect.x >= nWidth )
			return TRUE;

		if( BoundRect.x < 0 )
		{
			if( BoundRect.x + BoundRect.width <= 0 )
				return TRUE;

			BoundRect.width += BoundRect.x;
			BoundRect.x = 0;
		}

		if( BoundRect.x + BoundRect.width > nWidth )
		{
			BoundRect.width = nWidth - BoundRect.x;
		}
	}

	Ipp8u*  pBuffer = new Ipp8u[BoundRect.width*BoundRect.height*4];
	Ipp32f* pMapX   = new Ipp32f[BoundRect.width*BoundRect.height];
	Ipp32f* pMapY   = new Ipp32f[BoundRect.width*BoundRect.height];

	dRadius = 1.0/(nMaxRadius * nMaxRadius);
	Degree /= 10;

	if( pBuffer && pMapX && pMapY )
	{
		Ipp32f* pCurMapX = pMapX;
		Ipp32f* pCurMapY = pMapY;

		if( c_nTransformCylinderHoriz == nType )
		{
			for( int nRow = BoundRect.y; nRow < BoundRect.y + BoundRect.height; ++nRow )
			{
				int nY = nRow - Center;
				double dR = nY * nY * dRadius;
				
				*pCurMapY = float(nY * pow( dR + 0.0001, Degree ) + Center);
				*pCurMapX = 0;

				for( int nPos = 1; nPos < BoundRect.width; ++nPos, ++pCurMapX, ++pCurMapY )
				{
					*pCurMapY = pCurMapY[-1];
					*pCurMapX = nPos;
				}
			}
		}
		else
		{
			for( int nPos = BoundRect.x; nPos < BoundRect.x + BoundRect.width; ++nPos, ++pCurMapX, ++pCurMapY )
			{
				int nX = nPos - Center;
				double dR = nX * nX * dRadius;

				*pCurMapX = float(nX * pow( dR + 0.0001, Degree ) + Center);
				*pCurMapY = 0;
			}

			for( int nRow = 1; nRow < BoundRect.height; ++nRow )
			{
				for( int nPos = 0; nPos < BoundRect.width; ++nPos, ++pCurMapX, ++pCurMapY )
				{
					*pCurMapX = pCurMapX[-BoundRect.width];
					*pCurMapY = nRow;
				}
			}
		}

		// закрашиваем фоновым цветом
		IppiSize DstSize = {BoundRect.width, BoundRect.height};

		Ipp8u Colors[4] = {
			GetBValue(dwBackColor), 
			GetGValue(dwBackColor), 
			GetRValue(dwBackColor),
			255
		};

		ippiSet_8u_C4R( Colors, pBuffer, BoundRect.width*4, DstSize );

		// ремэпим
		IppiSize SrcSize = {nWidth, nHeight};
		IppiRect SrcRect = {0, 0, nWidth, nHeight};

		ippiRemap_8u_C4R( pPixels, SrcSize, 4*nWidth, SrcRect, 
			pMapX, sizeof(Ipp32f)*BoundRect.width, 
			pMapY, sizeof(Ipp32f)*BoundRect.width, 
			pBuffer, 4*BoundRect.width, DstSize, IPPI_INTER_LINEAR);

		// копируем полученную картинку обратно
		Ipp8u* pDstPixels = pPixels + (BoundRect.y * nWidth + BoundRect.x) * 4;
		ippiCopy_8u_C4R( pBuffer, BoundRect.width*4, pDstPixels, nWidth*4, DstSize );
	}

	if (pMapX)
		delete []pMapX;
	if (pMapY)
		delete []pMapY;
	if (pBuffer)
		delete []pBuffer;
	
	return TRUE;
}

void IPP_BGRA_GaussianBlurHorizontal( Ipp16u* pMapBGR, Ipp8u* pDstBGRA, int nWidth, int nHeight, int nDstStride, int nMapStride, int nBlurSize )
{
	Ipp32u nFactor = nBlurSize * 2 + 1;
	nFactor *= nFactor;

	for( ; nHeight > 0; --nHeight, pDstBGRA += nDstStride, pMapBGR += nMapStride )
	{
		Ipp8u*  pDst = pDstBGRA;
		Ipp16u* pMap = pMapBGR;

		// подсчёт начальной суммы
		Ipp32u nB = Ipp32u(pMap[0] * (nBlurSize + 1));
		Ipp32u nG = Ipp32u(pMap[1] * (nBlurSize + 1));
		Ipp32u nR = Ipp32u(pMap[2] * (nBlurSize + 1));

		int nLength = nBlurSize;
		for( ; nLength > 0; --nLength )
		{
			pMap += 3;
			nB += pMap[0];
			nG += pMap[1];
			nR += pMap[2];
		}

		// расчёт новых пикселов в строке
		pMap = pMapBGR;
		nLength = nWidth;
		
		for( ;; )
		{
			pDst[0] = BYTE(nB / nFactor);
			pDst[1] = BYTE(nG / nFactor);
			pDst[2] = BYTE(nR / nFactor);

			// проверка на выход из цикла
			if( nLength <= 1 )
			{
				break;
			}
			pDst += 4; 

			// вычитаем крайнее левое значение
			int index = 3 * ((nLength > nWidth - nBlurSize) ? nWidth - nLength : nBlurSize);
			nB -= pMap[0 - index];
			nG -= pMap[1 - index];
			nR -= pMap[2 - index];

			// переходим на следующий пиксел
			--nLength;
			pMap += 3;
			
			// прибавляем следующее крайнее правое значение
			index = 3 * (nLength > nBlurSize ? nBlurSize : nLength - 1);
			nB += pMap[0 + index];
			nG += pMap[1 + index];
			nR += pMap[2 + index];
		}
	}
}

void IPP_BGRA_GaussianBlurVertical(Ipp8u* pSrcBGRA, Ipp16u* pMapBGR, int nWidth, int nHeight, int nSrcStride, int nMapStride, int nBlurSize )
{
	for( ; nWidth > 0; --nWidth, pSrcBGRA += 4, pMapBGR += 3 )
	{
		Ipp8u*  pSrc = pSrcBGRA;
		Ipp16u* pMap = pMapBGR;

		// подсчёт начальной суммы
		Ipp16u nB = Ipp16u(pSrc[0] * (nBlurSize + 1));
		Ipp16u nG = Ipp16u(pSrc[1] * (nBlurSize + 1));
		Ipp16u nR = Ipp16u(pSrc[2] * (nBlurSize + 1));

		int nLength = nBlurSize;
		while( nLength-- > 0 )
		{
			pSrc += nSrcStride;
			nB += pSrc[0];
			nG += pSrc[1];
			nR += pSrc[2];
		}

		// расчёт новых пикселов в строке
		pSrc = pSrcBGRA;
		nLength = nHeight;
		
		for( ;; )
		{
			pMap[0] = nB;
			pMap[1] = nG;
			pMap[2] = nR;

			// проверка на выход из цикла
			if( nLength <= 1 )
			{
				break;
			}
			pMap += nMapStride;

			// вычитаем крайнее верхнее значение
			int index = nSrcStride * ((nLength > nHeight - nBlurSize) ? nHeight - nLength : nBlurSize);
			nB -= pSrc[0 - index];
			nG -= pSrc[1 - index];
			nR -= pSrc[2 - index];

			// переходим на следующий пиксел
			--nLength;
			pSrc += nSrcStride;
			
			// прибавляем следующее крайнее нижнее значение
			index = nSrcStride * (nLength > nBlurSize ? nBlurSize : nLength - 1);
			nB += pSrc[0 + index];
			nG += pSrc[1 + index];
			nR += pSrc[2 + index];
		}
	}
}

BOOL IPPEffectGaussianBlur2( BYTE* pPixels, int nWidth, int nHeight, double dFrame, long Size )
{
	// определяем размер блюра
	if( Size < 1 )
		return TRUE;
	
	// создаём вспомогательный массив для ускоренного расчёта блюра
	Ipp16u* pMap = new Ipp16u[nWidth*nHeight*3];
	if( !pMap )
		return TRUE;

	int nImgStride = nWidth * 4;
	int nMapStride = nWidth * 3;

	// первый цикл
	IPP_BGRA_GaussianBlurVertical( pPixels, pMap, nWidth, nHeight, nImgStride, nMapStride, Size );
	IPP_BGRA_GaussianBlurHorizontal( pMap, pPixels, nWidth, nHeight, nImgStride, nMapStride, Size );

	// второй цикл
	IPP_BGRA_GaussianBlurVertical( pPixels, pMap, nWidth, nHeight, nImgStride, nMapStride, Size );
	IPP_BGRA_GaussianBlurHorizontal( pMap, pPixels, nWidth, nHeight, nImgStride, nMapStride, Size );

	delete [] pMap;
	
	return TRUE;
}


