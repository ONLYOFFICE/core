/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "SvmPlayer.h"
#include "../Common/MetaFile.h"

#if defined(DrawText)
#undef DrawText
#endif

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
		 m_currentLanguage		= 0;
		 m_currentActionType	= 0;
		 m_bMainStream			= true;
	 };
	 CSvmFile(BYTE *Data, int DataSize): m_oPlayer(this)
	 {
		 m_oStream.SetStream(Data, DataSize);
		 
		 m_pDC					= m_oPlayer.GetDC();
		 m_currentActionVersion = 0;
		 m_currentCharset		= 0;
		 m_currentLanguage		= 0;
		 m_currentActionType	= 0;
		 m_bMainStream			= true;
	 }

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
		m_currentLanguage		= 0;
	}
	TRect*   GetBounds()
	{
		return &m_oBoundingBox;
	}	
	TRect* GetDCBounds()
	{
		//if (m_oHeader.mapMode.isSimple)
		if (m_pDC->m_oMapMode.isSimple)
		{
			m_oDCRect  = m_oBoundingBox;
			return &m_oDCRect;
		}
		else
		{
			return &m_oHeader.boundRect;
		}
	}
	double GetPixelHeight()
	{
		return m_pDC->m_dPixelHeight;
	}
	double GetPixelWidth()
	{
		return m_pDC->m_dPixelWidth;
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
	int GetTextBgColor()
	{
		TSvmColor& oColor = m_pDC->GetTextBgColor();
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b);
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
	TXForm*      GetTransform(int iGraphicsMode = GM_ADVANCED)
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
	bool         IsWindowFlippedY()
	{
		return false;
	}
	bool         IsWindowFlippedX()
	{
		return false;
	}

	unsigned int GetMapMode()
	{
		return MM_ANISOTROPIC;
	}

	double GetDpi()
	{
		return 96.;
	}

	IRegion* GetRegion()
	{
		return NULL;
	}

	unsigned int GetArcDirection()
	{
		return AD_CLOCKWISE;
	}

 private:
	TSvmPoint			m_oCurrnetOffset;
	BYTE*				m_pBufferData;
	CSvmDC*				m_pDC;
	CSvmPlayer          m_oPlayer;
	SvmHeader			m_oHeader;
	bool				m_bMainStream;

	unsigned short		m_currentActionVersion;
	unsigned short		m_currentCharset;
	unsigned short		m_currentLanguage;
	unsigned short		m_currentActionType;

	unsigned int		m_unRecordSize;
	unsigned int		m_unRecordPos;
	
	bool				m_bFirstPoint;
	TRect				m_oBoundingBox;
	//TRect				m_oRect;
	TRect				m_oDCRect;

	friend class CSvmPlayer;

	void Read_META_LINE();
	void Read_META_RECTANGLE();
	void Read_SVM_HEADER();
	void Read_META_POLYGON();
	void Read_META_POLYLINE();
	void Read_META_POLYPOLYGON();
	void Read_META_TEXT();
	void Read_META_ARRAYTEXT();
	void Read_META_TEXTALIGN();
	void Read_META_TEXTRECT();
	void Read_META_SETMAPMODE();
	void Read_META_SETTEXTFILLCOLOR();
	void Read_META_SETTEXTCOLOR();
	void Read_META_SETFILLCOLOR();
	void Read_META_SETLINECOLOR();
	void Read_META_FONT();
	void Read_META_BMPSCALE();
	void Read_META_BMPEXSCALE();
        void Read_META_BMP();
        void Read_META_BMP(TSvmBitmap &bitmap_info, BYTE** ppDstBuffer, unsigned int* pulWidth, unsigned int* pulHeight);
        void Read_META_BMPEX();
        void Read_META_RASTEROP();
	void Read_META_PUSH();
	void Read_META_POP();
	void Read_META_GRADIENT();
	void Read_META_GRADIENTEX();
	void Read_META_TRANSPARENT();
	void Read_META_FLOATTRANSPARENT();
	void Read_META_LAYOUTMODE();

	void Read_META_SECTRECTCLIPREGION();
	void Read_META_SECTREGIONCLIPREGION();

	void Read_META_STRETCHTEXT();

	void Read_META_TEXTLANGUAGE();

	void Read_META_POLYPOLYGON(std::vector<TSvmPolygon> & polygons, std::vector<TSvmPolygon> & complexPolygons);

	
//-------------------------------------------------------------------------------------------------------
	void TranslatePoint(TSvmPoint& oPoint, double& dX, double& dY)
	{
		TranslatePoint(oPoint.x, oPoint.y, dX, dY);
	}
	void TranslatePoint(int nX, int nY, double& dX, double &dY)
	{
		dX = (double)nX;
		dY = (double)nY;
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
			double dX, dY, dR, dB;
			
			TranslatePoint(nX, nY, dX, dY);
			TranslatePoint(nX + nW, nY + nH, dR, dB);

			//Если каждый 4-ый байт оставить 0, то получим черную картинку
			for (int nIndex = 3, nSize = 4 * unImageW * unImageH; nIndex < nSize; nIndex += 4)
				pImageBuffer[nIndex] = 255;

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

				m_pOutput->DrawBitmap(dX, dY, fabs(dX1 - dX), fabs(dY1 - dY), pBgra, unWidth, unHeight);
			}

			if (pBgra)
				delete[] pBgra;

			//int nRemainingBytes = GetRecordRemainingBytesCount();
			//if (nRemainingBytes < 0)
			//	return SetError();
		}
		else
		{
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
	void DrawText(std::wstring& wsString, unsigned int unCharsCount, TSvmRect& rect)
	{
		int nX = rect.l;
		int nY = rect.t;

		int nX1 = rect.r;
		int nY1 = rect.b;
	
		if (m_pDC->GetTextAlign() & TA_UPDATECP)
		{
			nX = m_pDC->GetCurPos().x;
			nY = m_pDC->GetCurPos().y;
		}

		if (m_pOutput)
		{
			double dX = nX, dY = nY, dX1 = nX1, dY1 = nY1;
			TranslatePoint(nX, nY, dX, dY);
			TranslatePoint(nX1, nY1, dX1, dY1);

			double* pdDx =  new double[unCharsCount];
			if (pdDx)
			{
				for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount; unCharIndex++)
				{
					pdDx[unCharIndex] = (dX1 - dX)/unCharsCount;
				}
			}
			TSvmMapMode aMapMode_new, aMapMode_old;
			//
			aMapMode_old = m_pDC->m_oMapMode;	
			aMapMode_new.unit = MAP_RELATIVE;

			m_pDC->SetMapMode(aMapMode_new);

			m_pOutput->DrawString(wsString, unCharsCount, dX, dY, pdDx);

			m_pDC->SetMapMode(aMapMode_old);
			if (pdDx)
				delete[] pdDx;
		}
	}

        void DrawText(std::wstring& wsString, unsigned int unCharsCount, int _nX, int _nY, int* pnDx = NULL, unsigned int nDxSize = 0)
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
			double dX, dY;
			TranslatePoint(nX, nY, dX, dY);

			double* pdDx = NULL;
			if (pnDx)
			{
				pdDx = new double[unCharsCount];
				if (pdDx)
				{
					int nCurX		= nX;
					double dCurX	= dX;

					int nCurXFirst		= nX;
					double dCurXFirst	= dX;

					for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount; unCharIndex++)
					{
						int nX1 = nCurXFirst + (unCharIndex < nDxSize ? pnDx[unCharIndex] : 0);
						double dX1, dY1;
						
						TranslatePoint(nX1, nY, dX1, dY1);
						
						pdDx[unCharIndex] = dX1 - dCurX;
						nCurX = nX1;
						dCurX = dX1;
					}
				}
			}

			TSvmMapMode aMapMode_new, aMapMode_old;
			//
			aMapMode_old = m_pDC->m_oMapMode;	
			aMapMode_new.unit = MAP_RELATIVE;

			m_pDC->SetMapMode(aMapMode_new);

			m_pOutput->DrawString(wsString, unCharsCount, dX, dY, pdDx);

			m_pDC->SetMapMode(aMapMode_old);
			if (pdDx)
				delete[] pdDx;
		}
	}
};


};

