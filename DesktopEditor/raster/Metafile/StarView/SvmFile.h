#pragma once

#include "SvmPlayer.h"

#include "../Common/MetaFile.h"

namespace MetaFile
{


class CSvmFile : virtual public IMetaFileBase
{
 public:
	 CSvmFile() : m_oPlayer(this)
	 {
		 m_pDC					= m_oPlayer.GetDC();
		 m_currentActionVersion = 0;
		 m_currentCharset		= 0;
		 m_currentActionType	= 0;
	 };

	 ~CSvmFile()
	 {
		 ClearFile();
	 };


	void PlayMetaFile();
	void ClearFile()
	{
		m_oPlayer.Clear();
		m_pDC = m_oPlayer.GetDC();

		m_currentActionVersion	= 0;
		m_currentCharset		= 0;
	}
	TRect*   GetBounds()
	{
		return &m_oHeader.boundRect;//&m_oBoundingBox;//
	}	
	TRect* GetDCBounds()
	{
		return  &m_oHeader.boundRect;
	}
	double GetPixelHeight()
	{
		return m_pDC->GetPixelHeight();
	}
	double GetPixelWidth()
	{
		return m_pDC->GetPixelWidth();
	}
	int GetTextColor()
	{
		TSvmColor& oColor = m_pDC->GetTextColor();
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b);
	}
	IFont*       GetFont()
	{
		CSvmFont* pFont = m_pDC->GetFont();
		if (!pFont)
			return NULL;

		return (IFont*)pFont;
	}
	IBrush*      GetBrush()
	{
		CSvmBrush* pBrush = m_pDC->GetBrush();
		if (!pBrush)
			return NULL;

		return (IBrush*)pBrush;
	}
	IPen*        GetPen()
	{
		CSvmPen* pPen = m_pDC->GetPen();
		if (!pPen)
			return NULL;

		return (IPen*)pPen;
	}
	unsigned int GetTextAlign()
	{
		return m_pDC->GetTextAlign();
	}
	unsigned int GetTextBgMode()
	{
		return m_pDC->GetBgMode();
	}
	int          GetTextBgColor()
	{
		TSvmColor& oColor = m_pDC->GetBgColor();
		return oColor.color; //METAFILE_RGBA(oColor.r, oColor.g, oColor.b);
	}
	unsigned int GetFillMode()
	{
		return m_pDC->GetFillMode();
	}
	TPointD      GetCurPos()
	{
		TSvmPoint oPoint = m_pDC->GetCurPos();
		TPointD oRes( oPoint.x,  oPoint.y);
		TranslatePoint(oPoint.x, oPoint.y, oRes.x, oRes.y);
		return oRes;
	}
	TXForm*      GetInverseTransform()
	{
		return m_pDC->GetInverseTransform();
	}
	TXForm*      GetTransform()
	{
		return m_pDC->GetTransform();
	}
	unsigned int GetMiterLimit()
	{
		return m_pDC->GetMiterLimit();
	}
	unsigned int GetRop2Mode()
	{
		return m_pDC->GetRop2Mode();
	}
	IClip*       GetClip()
	{
		CSvmClip* pClip = m_pDC->GetClip();			
		if (!pClip)
			return NULL;

		return (IClip*)pClip;
	}
	int          GetCharSpace()
	{
		return 0;
	}

 private:
	BYTE*				m_pBufferData;
	CSvmDC*				m_pDC;
	CSvmPlayer          m_oPlayer;
	SvmHeader			m_oHeader;

	unsigned short		m_currentActionVersion;
	unsigned short		m_currentCharset;
	unsigned short		m_OurrentActionType;

	unsigned int		m_unRecordSize;
	unsigned int		m_unRecordPos;
	
	bool				m_bFirstPoint;
	TRect				m_oBoundingBox;

	friend class CSvmPlayer;

	void Read_META_RECTANGLE();
	void Read_SVM_HEADER();
	void Read_META_POLYGON();
	void Read_META_POLYLINE();
	void Read_META_POLYPOLYGON();
	void Read_META_TEXT();
	void Read_META_ARRAYTEXT();
	void Read_META_SETMAPMODE();
	void Read_META_SETTEXTCOLOR();
	void Read_META_SETFILLCOLOR();
	void Read_META_SETLINECOLOR();
	void Read_META_FONT();
	void Read_META_BMPSCALE();
	void Read_META_RASTEROP();
	void Read_META_PUSH();
	void Read_META_POP();
	void Read_META_GRADIENT();
	void Read_META_GRADIENTEX();
	void Read_META_TRANSPARENT();

	void Read_META_POLYPOLYGON(std::vector<TSvmPolygon> & polygons);

	
//-------------------------------------------------------------------------------------------------------
	void TranslatePoint(TSvmPoint& oPoint, double& dX, double& dY)
	{
		TranslatePoint(oPoint.x, oPoint.y, dX, dY);
	}
	void TranslatePoint(int nX, int nY, double& dX, double &dY)
	{
		TSvmWindow* pWindow		= m_pDC->GetWindow();
		TSvmWindow* pViewport	= m_pDC->GetViewport();

		dX = (double)((double)(nX - pWindow->lX) * m_pDC->GetPixelWidth()) + pViewport->lX;
		dY = (double)((double)(nY - pWindow->lY) * m_pDC->GetPixelHeight()) + pViewport->lY;

		// Координаты приходят уже с примененной матрицей. Поэтому сначала мы умножаем на матрицу преобразования, 
		// вычитаем начальные координаты и умножаем на обратную матрицу преобразования.
		TRect* pBounds = GetDCBounds();
		double dT = pBounds->nTop;
		double dL = pBounds->nLeft;

		TXForm* pInverse	= GetInverseTransform();
		TXForm* pTransform	= GetTransform();
		pTransform->Apply(dX, dY);
		dX -= dL;
		dY -= dT;
		pInverse->Apply(dX, dY);
	}

	bool ReadImage(unsigned short ushColorUsage, BYTE** ppBgraBuffer, unsigned int* pulWidth, unsigned int* pulHeight);
	bool ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE** ppBgraBuffer, unsigned int* pulWidth, unsigned int* pulHeight)
	{
		int lHeaderOffset = offBmi - ulSkip;
		unsigned int ulHeaderSize = cbBmi;
		int lBitsOffset = offBits - offBmi - cbBmi;
		unsigned int ulBitsSize = cbBits;
		if (ulHeaderSize <= 0 || ulBitsSize <= 0 || lHeaderOffset < 0 || lBitsOffset < 0)
		{
			// TODO: Если попали сюда, значит надо смотреть BitBltRasterOperation
			if (lHeaderOffset > 0)
				m_oStream.Skip(lHeaderOffset);

			m_oStream.Skip(ulHeaderSize);

			if (lBitsOffset > 0)
				m_oStream.Skip(lBitsOffset);

			m_oStream.Skip(ulBitsSize);

			return false;
		}

		m_oStream.Skip(lHeaderOffset);

		BYTE* pHeaderBuffer = m_oStream.GetCurPtr();
		m_oStream.Skip(ulHeaderSize + lBitsOffset);
		BYTE* pBitsBuffer = m_oStream.GetCurPtr();
		m_oStream.Skip(ulBitsSize);

		MetaFile::ReadImage(pHeaderBuffer, ulHeaderSize, pBitsBuffer, ulBitsSize, ppBgraBuffer, pulWidth, pulHeight);

		return true;
	}
	void DrawImage(int nX, int nY, int nW, int nH, BYTE* pImageBuffer, unsigned int unImageW, unsigned int unImageH)
	{
		if (m_pOutput)
		{
			double dX=nX, dY=nY, dR=nX+nW, dB=nY+nH;
			
			TranslatePoint(nX, nY, dX, dY);
			TranslatePoint(nX + nW, nY + nH, dR, dB);
			
			m_pOutput->DrawBitmap(dX, dY, dR - dX, dB - dY, pImageBuffer, unImageW, unImageH);
		}
	}
	void DrawImage(int nX, int nY, int nW, int nH, unsigned int unColorUsage)
	{
		if (m_pOutput)
		{
			BYTE* pBgra = NULL;
			unsigned int unWidth, unHeight;
			if (ReadImage(unColorUsage, &pBgra, &unWidth, &unHeight))
			{
				//ProcessRasterOperation(unRasterOperation, &pBgra, unWidth, unHeight);

				double dX=nX, dY=nY, dX1=nX+nW, dY1=nY+nH;
				TranslatePoint(nX, nY, dX, dY);
				TranslatePoint(nX + nW, nY + nH, dX1, dY1);

				m_pOutput->DrawBitmap(dX, dY, abs(dX1 - dX), abs(dY1 - dY), pBgra, unWidth, unHeight);
			}

			if (pBgra)
				delete[] pBgra;

			//int nRemainingBytes = GetRecordRemainingBytesCount();
			//if (nRemainingBytes < 0)
			//	return SetError();
		}
		else
		{
			RegisterPoint(nX, nY);
			RegisterPoint(nX + nW, nY + nH);
		}
	}
	void MoveTo(TSvmPoint& oPoint)
	{
		MoveTo(oPoint.x, oPoint.y);
	}

	void MoveTo(int nX, int nY)
	{
		double dX = nX, dY = nY;
		TranslatePoint(nX, nY, dX, dY);

		//if (m_pPath)
		//{
		//	if (!m_pPath->MoveTo(dX, dY))
		//		return SetError();
		//}
		//else 
		if (m_pOutput)
		{
			m_pOutput->MoveTo(dX, dY);
		}

		m_pDC->SetCurPos(nX, nY);
	}
	void LineTo(int nX, int nY)
	{
		double dX = nX, dY = nY;
		TranslatePoint(nX, nY, dX, dY);
		//RegisterPoint(nX, nY);

		//if (m_pPath)
		//{
		//	if (!m_pPath->LineTo(dX, dY))
		//		return SetError();
		//}
		//else 
		if (m_pOutput)
		{
			m_pOutput->LineTo(dX, dY);
		}

		m_pDC->SetCurPos(nX, nY);
	}
	void LineTo(TSvmPoint& oPoint)
	{
		LineTo(oPoint.x, oPoint.y);
	}
	void CurveTo(int nX1, int nY1, int nX2, int nY2, int nXe, int nYe)
	{
		double dX1, dY1, dX2, dY2, dXe, dYe;
		TranslatePoint(nX1, nY1, dX1, dY1);
		TranslatePoint(nX2, nY2, dX2, dY2);
		TranslatePoint(nXe, nYe, dXe, dYe);

		//if (m_pPath)
		//{
		//	if (!m_pPath->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe))
		//		return SetError();
		//}
		//else 
		if (m_pOutput)
		{
			m_pOutput->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
		}

		m_pDC->SetCurPos(nXe, nYe);
	}
	void CurveTo(TSvmPoint& oPoint1, TSvmPoint& oPoint2, TSvmPoint& oPointE)
	{
		CurveTo(oPoint1.x, oPoint1.y, oPoint2.x, oPoint2.y, oPointE.x, oPointE.y);
	}
	void ClosePath()
	{
		//if (m_pPath)
		//{
		//	if (!m_pPath->Close())
		//		return SetError();
		//}
		//else 
		if (m_pOutput)
			m_pOutput->ClosePath();
	}
	void ArcTo(int nL, int nT, int nR, int nB, double dStart, double dSweep)
	{
		double dL, dT, dR, dB;
		TranslatePoint(nL, nT, dL, dT);
		TranslatePoint(nR, nB, dR, dB);

		//if (m_pPath)
		//{
		//	if (!m_pPath->ArcTo(dL, dT, dR, dB, dStart, dSweep))
		//		return SetError();
		//}
		//else 
		if (m_pOutput)
		{
			m_pOutput->ArcTo(dL, dT, dR, dB, dStart, dSweep);
		}

		// Пересчет текущей позиции делается в каждой функции отдельно после вызова данной
	}
	void DrawPath(bool bStroke, bool bFill)
	{
		//if (m_pPath && m_pOutput)
		//{
		//}
		//else 
		if (m_pOutput)
		{
			int lType = (bStroke ? 1 : 0) + (bFill ? 2 : 0);
			m_pOutput->DrawPath(lType);
			m_pOutput->EndPath();
		}
	}
	void UpdateOutputDC()
	{
		if (m_pOutput)
			m_pOutput->UpdateDC();
	}
	void DrawText(std::wstring& wsString, unsigned int unCharsCount, int _nX, int _nY, int* pnDx = NULL)
	{
		int nX = _nX;
		int nY = _nY;

		if (m_pDC->GetTextAlign() & TA_UPDATECP)
		{
			nX = m_pDC->GetCurPos().x;
			nY = m_pDC->GetCurPos().y;
		}

		if (m_pOutput)
		{
			double dX = nX, dY = nY;
			TranslatePoint(nX, nY, dX, dY);

			double* pdDx = NULL;
			if (pnDx)
			{
				pdDx = new double[unCharsCount];
				if (pdDx)
				{
					int nCurX = nX;
					double dCurX = dX;

					for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount; unCharIndex++)
					{
						int nX1 = nCurX + pnDx[unCharIndex];
						double dX1 = nX1, dY1;
						TranslatePoint(nX1, nY, dX1, dY1);
						pdDx[unCharIndex] = dX1 - dCurX;
						nCurX = nX1;
						dCurX = dX1;
					}
				}
			}

			m_pOutput->DrawString(wsString, unCharsCount, dX, dY, pdDx);

			if (pdDx)
				delete[] pdDx;
		}
	}
   TRect GetBoundingBox()
	{
		TRect oBB = m_oBoundingBox;

		if (abs(oBB.nRight - oBB.nLeft) <= 1)
			oBB.nRight = oBB.nLeft + 1024;

		if (abs(oBB.nBottom - oBB.nTop) <= 1)
			oBB.nBottom = m_oBoundingBox.nTop + 1024;

		return oBB;
	}
	void RegisterPoint(short shX, short shY)
	{
		if (m_bFirstPoint)
		{
			m_oBoundingBox.nLeft   = shX;
			m_oBoundingBox.nRight  = shX;
			m_oBoundingBox.nTop    = shY;
			m_oBoundingBox.nBottom = shY;
			m_bFirstPoint = false;
		}
		else
		{
			if (shX < m_oBoundingBox.nLeft)
				m_oBoundingBox.nLeft = shX;
			else if (shX > m_oBoundingBox.nRight)
				m_oBoundingBox.nRight = shX;

			if (shY < m_oBoundingBox.nTop)
				m_oBoundingBox.nTop = shY;
			else if (shY > m_oBoundingBox.nBottom)
				m_oBoundingBox.nBottom = shY;
		}
	}
};


};

