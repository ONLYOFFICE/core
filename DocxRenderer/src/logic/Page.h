#pragma once
#include "ElementShape.h"
#include "ElementParagraph.h"
#include "ElementImage.h"

namespace NSDocxRenderer
{
	const double STANDART_STRING_HEIGHT_MM		= 4.2333333333333334;
	const double THE_SAME_STRING_Y_PRECISION_MM = 0.01;

	static _bstr_t g_bstr_sectStart			= L"<w:p><w:pPr><w:sectPr>";
	static _bstr_t g_bstr_lastSect			= L"<w:type w:val=\"continuous\"/>";
	static _bstr_t g_bstr_sectEnd			= L"<w:pgMar w:top=\"0\" w:right=\"0\" w:bottom=\"0\" w:left=\"0\"/></w:sectPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr></w:p>";
	static CString g_string_sectSizeVer		= _T("<w:pgSz w:w=\"%d\" w:h=\"%d\" w:orient=\"portrait\"/>");
	static CString g_string_sectSizeHor		= _T("<w:pgSz w:w=\"%d\" w:h=\"%d\" w:orient=\"landscape\"/>");

	static _bstr_t g_bstr_drawingParStart	= L"<w:p><w:pPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr>";
	static _bstr_t g_bstr_ParEnd			= L"</w:p>";

	static CString g_bstr_lastsection1		= L"<w:sectPr>";
	static CString g_bstr_lastsection2		= L"<w:pgMar w:top=\"0\" w:right=\"0\" w:bottom=\"0\" w:left=\"0\" w:header=\"0\" w:footer=\"0\" w:gutter=\"0\"/></w:sectPr>";

	AVSINLINE bool IsUnicodeSymbol( WCHAR symbol )
	{
		bool result = false;

		if ( ( 0x0009 == symbol ) || ( 0x000A == symbol ) || ( 0x000D == symbol ) ||
		   ( ( 0x0020 <= symbol ) && ( 0xD7FF >= symbol ) ) || ( ( 0xE000 <= symbol ) && ( symbol <= 0xFFFD ) ) ||
		   ( ( 0x10000 <= symbol ) && symbol ) )
		{
			result = true;  
		}

		return result;		  
	}

	class CPage
	{
	public:
		NSStructures::CFont*		m_pFont;
		NSStructures::CPen*			m_pPen;
		NSStructures::CBrush*		m_pBrush;
		NSStructures::CShadow*		m_pShadow;
		NSStructures::CEdgeText*	m_pEdgeText;

		NSDocxRenderer::CMatrix*					m_pTransform;
		AVSGraphics::IASCGraphicSimpleComverter*	m_pSimpleGraphicsConverter;

		CVectorGraphics				m_oVector;

		double m_dWidth;
		double m_dHeight;

		LONG						m_lCurrentCommand;

		CAtlArray<CBaseItem*>	m_arGraphicItems;
		CAtlArray<CParagraph*>	m_arParagraphs;

		CAtlArray<CTextLine*> m_arTextLine;
		CTextLine* m_pCurrentLine;

		CFontManager		m_oManager;
		CFontManagerLight	m_oManagerLight;

		TextAssociationType m_eTextAssociationType;

		NSDocxRenderer::CStringWriter m_oWriterVML;

		bool m_bIsDeleteTextClipPage;

	public:
		CPage() : m_oManager(), m_oManagerLight()
		{
			m_pFont			= NULL;
			m_pBrush		= NULL;
			m_pPen			= NULL;
			m_pShadow		= NULL;
			m_pEdgeText		= NULL;

			m_pTransform	= NULL;
			m_pSimpleGraphicsConverter	= NULL;

			m_dWidth		= 0;
			m_dHeight		= 0;

			m_pCurrentLine	= NULL;
			m_eTextAssociationType = TextAssociationTypeNoFrames;

			m_bIsDeleteTextClipPage = true;
		}

	public:
		AVSINLINE void Init(NSStructures::CFont* pFont, NSStructures::CPen* pPen, NSStructures::CBrush* pBrush,
			NSStructures::CShadow* pShadow, NSStructures::CEdgeText* pEdge, NSDocxRenderer::CMatrix* pMatrix, AVSGraphics::IASCGraphicSimpleComverter* pSimple)
		{
			m_pFont		= pFont;
			m_pPen		= pPen;
			m_pBrush	= pBrush;
			m_pShadow	= pShadow;
			m_pEdgeText	= pEdge;

			m_pTransform				= pMatrix;
			m_pSimpleGraphicsConverter	= pSimple;

			m_oManager.m_pFont			= m_pFont;
			m_oManager.m_pTransform		= m_pTransform;

			m_pCurrentLine = NULL;

			m_oWriterVML.AddSize(1000, 1000);
		}

		AVSINLINE void Clear()
		{
			size_t nCount = 0;

			nCount = m_arTextLine.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CTextLine* pTemp = m_arTextLine[i];
				RELEASEOBJECT(pTemp);
			}
			m_arTextLine.RemoveAll();

			nCount = m_arGraphicItems.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CBaseItem* pTemp = m_arGraphicItems[i];
				RELEASEOBJECT(pTemp);
			}
			m_arGraphicItems.RemoveAll();

			nCount = m_arParagraphs.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CParagraph* pTemp = m_arParagraphs[i];
				RELEASEOBJECT(pTemp);
			}
			m_arParagraphs.RemoveAll();

			m_pCurrentLine = NULL;

			m_oWriterVML.ClearNoAttack();
		}

		~CPage()
		{
			Clear();
		}

		AVSINLINE void SetCurrentLineByBaseline(const double& dBaseLinePos)
		{
			if ((NULL == m_pCurrentLine) || (TextAssociationTypeDefault == m_eTextAssociationType))
			{
				// пуста¤ (в плане текста) страница
				m_pCurrentLine = new CTextLine();

				m_pCurrentLine->m_dBaselinePos = dBaseLinePos;
				m_arTextLine.Add(m_pCurrentLine);
				return;
			}
			if (fabs(m_pCurrentLine->m_dBaselinePos - dBaseLinePos) <= THE_SAME_STRING_Y_PRECISION_MM)
			{
				return;
			}
			size_t nCount = m_arTextLine.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (fabs(m_arTextLine[i]->m_dBaselinePos - dBaseLinePos) <= THE_SAME_STRING_Y_PRECISION_MM)
				{
					m_pCurrentLine = m_arTextLine[i];
					return;
				}
			}

			// лини¤ не нашлась - не беда - создадим новую
			m_pCurrentLine = new CTextLine();
			m_pCurrentLine->m_dBaselinePos = dBaseLinePos;
			m_arTextLine.Add(m_pCurrentLine);
			return;
		}

		// image commands
		AVSINLINE void WriteImage(CImageInfo& oInfo, double& fX, double& fY, double& fWidth, double& fHeight)
		{
			CImage* pImage = new CImage(oInfo, _T(""));

			double dRotation = m_pTransform->z_Rotation();

			if (fabs(dRotation) < 5.0)
			{
				double x1 = fX;
				double y1 = fY;
				double x2 = fX + fWidth;
				double y2 = fY + fHeight;

				m_pTransform->TransformPoint(x1, y1);
				m_pTransform->TransformPoint(x2, y2);

				if (x1 <= x2)
				{
					pImage->m_dLeft		= x1;
					pImage->m_dWidth	= x2 - x1;
				}
				else
				{
					pImage->m_dLeft		= x2;
					pImage->m_dWidth	= x1 - x2;
				}

				if (y1 <= y2)
				{
					pImage->m_dTop		= y1;
					pImage->m_dHeight	= y2 - y1;
				}
				else
				{
					pImage->m_dTop		= y2;
					pImage->m_dHeight	= y1 - y2;
				}

				pImage->m_dRotate = 0.0;
			}
			else
			{
				double x1 = fX;
				double y1 = fY;
				double x2 = fX + fWidth;
				double y2 = fY + fHeight;

				NSDocxRenderer::CMatrix oTemp  = *m_pTransform;
		
				double dCx = (x1 + x2) / 2;
				double dCy = (y1 + y2) / 2;
				m_pTransform->TransformPoint(dCx, dCy);
				oTemp.RotateAt(-dRotation, dCx, dCy, Aggplus::MatrixOrderAppend);

				oTemp.TransformPoint(x1, y1);
				oTemp.TransformPoint(x2, y2);

				if (x1 <= x2)
				{
					pImage->m_dLeft		= x1;
					pImage->m_dWidth	= x2 - x1;
				}
				else
				{
					pImage->m_dLeft		= x2;
					pImage->m_dWidth	= x1 - x2;
				}

				if (y1 <= y2)
				{
					pImage->m_dTop		= y1;
					pImage->m_dHeight	= y2 - y1;
				}
				else
				{
					pImage->m_dTop		= y2;
					pImage->m_dHeight	= y1 - y2;
				}

				pImage->m_dRotate = dRotation;
			}

			m_arGraphicItems.Add(pImage);
		}

		// path commands
		AVSINLINE void MoveTo(double& dX, double& dY)
		{
			m_pTransform->TransformPoint(dX, dY);
			m_oVector.MoveTo(dX, dY);

		}
		AVSINLINE void LineTo(double& dX, double& dY)
		{
			m_pTransform->TransformPoint(dX, dY);
			m_oVector.LineTo(dX, dY);
		}
		AVSINLINE void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			m_pTransform->TransformPoint(x1, y1);
			m_pTransform->TransformPoint(x2, y2);
			m_pTransform->TransformPoint(x3, y3);

			m_oVector.CurveTo(x1, y1, x2, y2, x3, y3);
		}
		AVSINLINE void Start()
		{			
		}
		AVSINLINE void End()
		{
			m_oVector.End();
			m_oWriterVML.ClearNoAttack();
		}
		AVSINLINE void Close()
		{
			m_oVector.Close();
		}
		AVSINLINE void DrawPath(LONG lType, LONG lTxId)
		{
			if ((m_oVector.m_dLeft <= m_oVector.m_dRight) && (m_oVector.m_dTop <= m_oVector.m_dBottom))
			{
				CShape* pShape = new CShape();

				pShape->m_lTxId		= lTxId;

				pShape->m_oPen		= *m_pPen;
				pShape->m_oBrush	= *m_pBrush;

				// нормализуем толщину линии
				double dScaleTransform = (m_pTransform->m_agg_mtx.sx + m_pTransform->m_agg_mtx.sy) / 2.0;
				pShape->m_oPen.Size *= dScaleTransform;

				if ((lType & 0x01) == 0x00)
				{
					if ((fabs(m_oVector.m_dLeft - m_oVector.m_dRight) < 0.3) || (fabs(m_oVector.m_dTop - m_oVector.m_dBottom) < 0.3))
					{
						lType = 0x01;
						pShape->m_bIsStroke  = true;
						pShape->m_oPen.Color = m_pBrush->Color1;
						pShape->m_oPen.Alpha = m_pBrush->Alpha1;
						//pShape->m_oPen.Size	 = max(pShape->m_oPen.Size, 1);	
					}
				}

				pShape->CreateFromVectorData(&m_oVector, m_oWriterVML, 100000, lType);
				m_arGraphicItems.Add(pShape);
			}
		}

		AVSINLINE void WriteText(BSTR bsText, BSTR bsGid, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, bool bIsPDFAnalyzer)
		{
			double dTextX = fX;
			double dTextY = fY;
			double dTextR = fX + fWidth;
			double dTextB = fY + fHeight;

			m_pTransform->TransformPoint(dTextX, dTextY);
			m_pTransform->TransformPoint(dTextR, dTextB);
			
			double dTextW = dTextR - dTextX;
			double dTextH = dTextB - dTextY;

			CString strText = _T("");

			if ((bsText != NULL) && (bsGid != NULL))
			{
				UINT lLen = SysStringLen(bsText);
				for (unsigned int i = 0; i < lLen; ++i)
				{
					if ( IsUnicodeSymbol( bsText[i] ) )
					{
						strText += bsText[i];
					}
					else
					{
						strText += _T(" ");
					}
				}
			}
			else
			{
				strText = (CString)bsText;
			}

			bool bIsPath = ((NULL == bsGid) && !bIsPDFAnalyzer) ? false : true;

			m_oManager.LoadFont(0, !bIsPath);
			
			if (bIsPath)
				m_oManager.GenerateFontName2(strText);

			if ((0 == dTextW) || (dTextW > 5 * m_oManager.m_dSpaceWidthMM))
			{
				double _x = 0;
				double _y = 0;
				double _w = 0;
				double _h = 0;

				if (NULL != bsGid)
				{
					m_oManager.SetStringGid(1);
					m_oManager.MeasureString(bsGid, dTextX, dTextY, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
				}
				else
				{
					// такого быть не должно (только из xps)
					m_oManager.SetStringGid(0);
					m_oManager.MeasureString(bsText, dTextX, dTextY, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
				}

				dTextW = _w;
				//dTextW *= c_dPixToMM;
			}

			double dBaseLinePos = dTextY + fBaseLineOffset;
			dTextH = m_oManager.GetFontHeight();

			SetCurrentLineByBaseline(dBaseLinePos);

			CContText* pLastCont = NULL;
			size_t nCountConts = m_pCurrentLine->m_arConts.GetCount();
			if (nCountConts != 0)
				pLastCont = m_pCurrentLine->m_arConts[nCountConts - 1];

			if (NULL == pLastCont)
			{
				// первое слово в линии
				CContText* pCont = new CContText();

				pCont->m_dX = dTextX;
				pCont->m_dY = dBaseLinePos;

				pCont->m_dWidth		= dTextW;
				pCont->m_dHeight	= dTextH;

				if (_T(" ") == strText)
				{
					pCont->m_dWidthWithoutSpaces	= 0;
					pCont->m_dLeftWithoutSpaces		= dTextX + dTextW;
				}
				else
				{
					pCont->m_dWidthWithoutSpaces	= dTextW;
					pCont->m_dLeftWithoutSpaces		= dTextX;
				}

				pCont->m_strText	= strText;

				pCont->m_oFont		= m_oManager.m_oFont.m_oFont;
				pCont->m_oBrush		= *m_pBrush;

				if (bIsPath)
				{
					pCont->m_strPickFontName	= m_oManager.m_strCurrentPickFont;
					pCont->m_lPickFontStyle		= m_oManager.m_lCurrentPictFontStyle;
				}

				pCont->m_dSpaceWidthMM = m_oManager.m_dSpaceWidthMM;
				
				m_pCurrentLine->AddCont(pCont, m_oManager.m_oFont.m_dBaselineOffset);
				return;
			}
			
			// продолжение линии
			//if (m_lCurrentCommand == c_nTextType && pLastCont->m_oFont.IsEqual(&m_oManager.m_oFontOld) && pLastCont->m_oBrush.IsEqual(m_pBrush))
			//{
			//	// быстрое отметание вс¤ких проверок
			//	pLastCont->m_strText += strText;
			//	pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dX);
			//	return;
			//}
			
			double dRight = pLastCont->m_dX + pLastCont->m_dWidth;

			if (pLastCont->m_oFont.IsEqual(&m_oManager.m_oFont.m_oFont) && pLastCont->m_oBrush.IsEqual(m_pBrush))
			{
				// настройки одинаковые. “еперь смотрим, на расположение
				if (fabs(dRight - dTextX) < 0.5)
				{
					// продолжаем слово
					pLastCont->m_strText += strText;
					pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dX);

					if (_T(" ") != strText)
					{
						if (0 == pLastCont->m_dWidthWithoutSpaces)
							pLastCont->m_dLeftWithoutSpaces = dTextX;
						
						pLastCont->m_dWidthWithoutSpaces = dTextX + dTextW - pLastCont->m_dLeftWithoutSpaces;
					}
					else if (0 == pLastCont->m_dWidthWithoutSpaces)
					{
						pLastCont->m_dLeftWithoutSpaces = dTextX + dTextW;
					}

					return;
				}
				else if ((dRight < dTextX) && ((dTextX - dRight) < m_oManager.m_dSpaceWidthMM))
				{
					// продолжаем слово с пробелом
					pLastCont->m_strText += (_T(" ") + strText);
					pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dX);

					if (_T(" ") != strText)
					{
						if (0 == pLastCont->m_dWidthWithoutSpaces)
							pLastCont->m_dLeftWithoutSpaces = dTextX;

						pLastCont->m_dWidthWithoutSpaces = dTextX + dTextW - pLastCont->m_dLeftWithoutSpaces;
					}
					else if (0 == pLastCont->m_dWidthWithoutSpaces)
					{
						pLastCont->m_dLeftWithoutSpaces = dTextX + dTextW;
					}

					return;
				}
			}
			
			// либо пробел большой между словами, либо новый текст левее, либо настройки не те (шрифт, кисть)
			// либо все вместе... просто добавл¤ем новое слово
			CContText* pCont = new CContText();

			pCont->m_dX = dTextX;
			pCont->m_dY = dBaseLinePos;

			pCont->m_dWidth		= dTextW;
			pCont->m_dHeight	= dTextH;

			if (_T(" ") == strText)
			{
				pCont->m_dWidthWithoutSpaces	= 0;
				pCont->m_dLeftWithoutSpaces		= dTextX + dTextW;
			}
			else
			{
				pCont->m_dWidthWithoutSpaces	= dTextW;
				pCont->m_dLeftWithoutSpaces		= dTextX;
			}

			pCont->m_strText	= strText;

			pCont->m_oFont		= m_oManager.m_oFont.m_oFont;
			pCont->m_oBrush		= *m_pBrush;

			if (bIsPath)
			{
				pCont->m_strPickFontName	= m_oManager.m_strCurrentPickFont;
				pCont->m_lPickFontStyle		= m_oManager.m_lCurrentPictFontStyle;
			}

			pCont->m_dSpaceWidthMM = m_oManager.m_dSpaceWidthMM;
			
			m_pCurrentLine->AddCont(pCont, m_oManager.m_oFont.m_dBaselineOffset);
		}

		void Build()
		{
			if (m_bIsDeleteTextClipPage)
			{
				// удалим все линии, которые выход¤т за границы страницы
				size_t nCount = m_arTextLine.GetCount();
				for (size_t i = 0; i < nCount; ++i)
				{
					CTextLine* pTextLine = m_arTextLine[i];

					double _top = pTextLine->m_dBaselinePos - pTextLine->m_dHeight;
					double _bottom = pTextLine->m_dBaselinePos;

					if (_top >= m_dHeight || _bottom <= 0)
					{
						m_arTextLine.RemoveAt(i);
						--i;
						--nCount;
					}
				}
			}

			switch (m_eTextAssociationType)
			{
			case TextAssociationTypeDefault:
			case TextAssociationTypeLine:
				{
					size_t nCount = m_arTextLine.GetCount();
					for (size_t i = 0; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arTextLine[i];

						CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
						pParagraph->m_pManagerLight = &m_oManagerLight;
						pParagraph->m_bIsTextFrameProperties = true;

						pParagraph->m_dLeft	= pTextLine->m_dX;
						pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset;

						pParagraph->m_arLines.Add(pTextLine);

						m_arParagraphs.Add(pParagraph);
					}

					// удалим все линии
					m_arTextLine.RemoveAll();
					break;
				}
			case TextAssociationTypeBlock:
				{
					size_t nCount = m_arTextLine.GetCount();

					if (0 == nCount)
						break;

					CTextLine* pFirstLine = m_arTextLine[0];

					CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
					pParagraph->m_pManagerLight = &m_oManagerLight;
					pParagraph->m_bIsTextFrameProperties = true;
					
					pParagraph->m_dLeft	= pFirstLine->m_dX;
					pParagraph->m_dTop	= pFirstLine->m_dBaselinePos - pFirstLine->m_dHeight + pFirstLine->m_dBaselineOffset;

					pParagraph->m_arLines.Add(pFirstLine);

					m_arParagraphs.Add(pParagraph);
					
					for (size_t i = 1; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arTextLine[i];

						CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
						pParagraph->m_pManagerLight = &m_oManagerLight;
						pParagraph->m_bIsTextFrameProperties = true;

						if (((fabs(pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pFirstLine->m_dBaselinePos) > STANDART_STRING_HEIGHT_MM) && (pTextLine->m_dX == pFirstLine->m_dX)) ||
							((pTextLine->m_dX != pFirstLine->m_dX) && (pTextLine->m_dBaselinePos != pFirstLine->m_dBaselinePos)))
						{
							pParagraph->m_dLeft	= pTextLine->m_dX;
							pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset;
						}
						else
						{
							pParagraph->m_dLeft	= pFirstLine->m_dX;
							pParagraph->m_dTop	= pFirstLine->m_dBaselinePos - pFirstLine->m_dHeight + pFirstLine->m_dBaselineOffset;
						}

						pFirstLine = pTextLine;

						pParagraph->m_arLines.Add(pTextLine);
						m_arParagraphs.Add(pParagraph);
					}

					// удалим все линии
					m_arTextLine.RemoveAll();
					break;
				}
			case TextAssociationTypeNoFrames:
				{
					SortElements(m_arTextLine);
					Merge(STANDART_STRING_HEIGHT_MM / 3);

					double previousStringOffset = 0;
					size_t nCount = m_arTextLine.GetCount();
					for (size_t i = 0; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arTextLine[i];

						CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
						pParagraph->m_pManagerLight = &m_oManagerLight;
						pParagraph->m_bIsTextFrameProperties = false;

						pParagraph->m_dLeft	= pTextLine->m_dX;
						
						double dBeforeSpacing = (pTextLine->m_dBaselinePos - previousStringOffset - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset);

						pParagraph->m_dSpaceBefore = max(dBeforeSpacing, 0);

						double dHeight = 1;
						if (pTextLine->m_dHeight != 0)
						{
							dHeight = pTextLine->m_dHeight;

							if (dBeforeSpacing < 0)
								dHeight += dBeforeSpacing;
						}

						pParagraph->m_dHeight = dHeight;

						previousStringOffset = pTextLine->m_dBaselinePos + pTextLine->m_dBaselineOffset;

						pParagraph->m_arLines.Add(pTextLine);
						m_arParagraphs.Add(pParagraph);
					}

					m_arTextLine.RemoveAll();
					break;
				}
			}
		}

		void Merge(double dAffinity)
		{
			size_t nCount = m_arTextLine.GetCount();
			if (1 < nCount)
			{
				CTextLine* pPrev = m_arTextLine[0];
				for (size_t i = 1; i < nCount; ++i)
				{
					CTextLine* pNext = m_arTextLine[i];

					if (fabs(pNext->m_dBaselinePos - pPrev->m_dBaselinePos) < dAffinity)
					{
						pPrev->Merge(pNext);

						pNext->m_arConts.RemoveAll();
						RELEASEOBJECT(pNext);

						m_arTextLine.RemoveAt(i);
						--i;
						--nCount;
						continue;
					}
					pPrev = pNext;
				}
			}
		}

		void Write(NSDocxRenderer::CStringWriter& oWriter)
		{
			// drawings
			size_t nCountDrawings = m_arGraphicItems.GetCount();
			if (0 != nCountDrawings)
			{
				oWriter.WriteString(g_bstr_drawingParStart);

				for (size_t i = 0; i < nCountDrawings; ++i)
				{
					m_arGraphicItems[i]->ToXml(oWriter);
				}

				oWriter.WriteString(g_bstr_ParEnd);
			}

			size_t nCountParagraphs = m_arParagraphs.GetCount();
			for (size_t i = 0; i < nCountParagraphs; ++i)
			{
				m_arParagraphs[i]->ToXml(oWriter);
			}
		}

		AVSINLINE void WriteSectionToFile(bool bLastPage, NSDocxRenderer::CStringWriter& oWriter)
		{
			// section
			LONG lWidthDx  = (LONG)(m_dWidth * c_dMMToDx);
			LONG lHeightDx = (LONG)(m_dHeight * c_dMMToDx);

			if (!bLastPage)
				oWriter.WriteString(g_bstr_sectStart);
			else
				oWriter.WriteString(g_bstr_lastsection1);
				
			if (lWidthDx >= lHeightDx)
			{
				CString strSize = _T("");
				strSize.Format(g_string_sectSizeHor, lWidthDx, lHeightDx);
				oWriter.WriteString(strSize);
			}
			else
			{
				CString strSize = _T("");
				strSize.Format(g_string_sectSizeVer, lWidthDx, lHeightDx);
				oWriter.WriteString(strSize);
			}

			if (!bLastPage)
				oWriter.WriteString(g_bstr_sectEnd);
			else
				oWriter.WriteString(g_bstr_lastsection2);
		}
	};
}