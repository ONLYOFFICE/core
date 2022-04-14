#pragma once
#include "ElementShape.h"
#include "ElementParagraph.h"
#include "ElementImage.h"

namespace NSDocxRenderer
{
	const double STANDART_STRING_HEIGHT_MM		= 4.2333333333333334;
	const double THE_SAME_STRING_Y_PRECISION_MM = 0.01;

    inline bool IsSpaceUtf32(NSStringUtils::CStringUTF32& oText)
    {
        if (1 != oText.length())
            return false;
        return (' ' == oText[0]) ? true : false;
    }

    inline bool IsUnicodeSymbol( int symbol )
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

        Aggplus::CMatrix*			m_pTransform;
        Aggplus::CGraphicsPathSimpleConverter* m_pSimpleGraphicsConverter;

		CVectorGraphics				m_oVector;

		double m_dWidth;
		double m_dHeight;

		LONG						m_lCurrentCommand;

        std::vector<CBaseItem*>	m_arGraphicItems;
        std::vector<CParagraph*> m_arParagraphs;

        std::vector<CTextLine*> m_arTextLine;
		CTextLine* m_pCurrentLine;

		CFontManager		m_oManager;
		CFontManagerLight	m_oManagerLight;

		TextAssociationType m_eTextAssociationType;

        NSStringUtils::CStringBuilder m_oWriterVML;

		bool m_bIsDeleteTextClipPage;

	public:
        CPage(NSFonts::IApplicationFonts* pFonts) : m_oManager(pFonts), m_oManagerLight(pFonts)
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
            m_eTextAssociationType = TextAssociationTypePlainLine;

			m_bIsDeleteTextClipPage = true;
		}

	public:
        void Init(NSStructures::CFont* pFont, NSStructures::CPen* pPen, NSStructures::CBrush* pBrush,
            NSStructures::CShadow* pShadow, NSStructures::CEdgeText* pEdge, Aggplus::CMatrix* pMatrix, Aggplus::CGraphicsPathSimpleConverter* pSimple)
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

            m_oWriterVML.AddSize(1000);
		}

        void Clear()
		{
			size_t nCount = 0;

            nCount = m_arTextLine.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				CTextLine* pTemp = m_arTextLine[i];
				RELEASEOBJECT(pTemp);
			}
            m_arTextLine.clear();

            nCount = m_arGraphicItems.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				CBaseItem* pTemp = m_arGraphicItems[i];
				RELEASEOBJECT(pTemp);
			}
            m_arGraphicItems.clear();

            nCount = m_arParagraphs.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				CParagraph* pTemp = m_arParagraphs[i];
				RELEASEOBJECT(pTemp);
			}
            m_arParagraphs.clear();

			m_pCurrentLine = NULL;

			m_oWriterVML.ClearNoAttack();
		}

		~CPage()
		{
			Clear();
		}

        void SetCurrentLineByBaseline(const double& dBaseLinePos)
		{
            if ((NULL == m_pCurrentLine) || (TextAssociationTypeBlockChar == m_eTextAssociationType))
			{
                // пустая (в плане текста) страница
				m_pCurrentLine = new CTextLine();

				m_pCurrentLine->m_dBaselinePos = dBaseLinePos;
                m_arTextLine.push_back(m_pCurrentLine);
				return;
			}
			if (fabs(m_pCurrentLine->m_dBaselinePos - dBaseLinePos) <= THE_SAME_STRING_Y_PRECISION_MM)
			{
				return;
			}
            size_t nCount = m_arTextLine.size();
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
            m_arTextLine.push_back(m_pCurrentLine);
			return;
		}

		// image commands
        void WriteImage(CImageInfo& oInfo, double& fX, double& fY, double& fWidth, double& fHeight)
		{
            CImage* pImage = new CImage(oInfo, L"");

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

                Aggplus::CMatrix oTemp  = *m_pTransform;
		
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

			m_arGraphicItems.push_back(pImage);
		}

		// path commands
        void MoveTo(double& dX, double& dY)
		{
			m_pTransform->TransformPoint(dX, dY);
			m_oVector.MoveTo(dX, dY);

		}
        void LineTo(double& dX, double& dY)
		{
			m_pTransform->TransformPoint(dX, dY);
			m_oVector.LineTo(dX, dY);
		}
        void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			m_pTransform->TransformPoint(x1, y1);
			m_pTransform->TransformPoint(x2, y2);
			m_pTransform->TransformPoint(x3, y3);

			m_oVector.CurveTo(x1, y1, x2, y2, x3, y3);
		}
        void Start()
		{			
		}
        void End()
		{
			m_oVector.End();
			m_oWriterVML.ClearNoAttack();
		}
        void Close()
		{
			m_oVector.Close();
		}
        void DrawPath(LONG lType, LONG lTxId)
		{
			if ((m_oVector.m_dLeft <= m_oVector.m_dRight) && (m_oVector.m_dTop <= m_oVector.m_dBottom))
			{
				CShape* pShape = new CShape();

				pShape->m_lTxId		= lTxId;

				pShape->m_oPen		= *m_pPen;
				pShape->m_oBrush	= *m_pBrush;

				// нормализуем толщину линии
                double dScaleTransform = (m_pTransform->sx() + m_pTransform->sy()) / 2.0;
				pShape->m_oPen.Size *= dScaleTransform;

				if ((lType & 0x01) == 0x00)
				{
					if ((fabs(m_oVector.m_dLeft - m_oVector.m_dRight) < 0.3) || (fabs(m_oVector.m_dTop - m_oVector.m_dBottom) < 0.3))
					{
						lType = 0x01;
						pShape->m_oPen.Color = m_pBrush->Color1;
						pShape->m_oPen.Alpha = m_pBrush->Alpha1;
						//pShape->m_oPen.Size	 = max(pShape->m_oPen.Size, 1);	
					}
				}

				pShape->CreateFromVectorData(&m_oVector, m_oWriterVML, 100000, lType);
                m_arGraphicItems.push_back(pShape);
			}
		}

        void WriteText(unsigned int* pUnicodes, unsigned int* pGids, unsigned int nCount, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, bool bIsPDFAnalyzer)
		{
			double dTextX = fX;
			double dTextY = fY;
			double dTextR = fX + fWidth;
			double dTextB = fY + fHeight;

			m_pTransform->TransformPoint(dTextX, dTextY);
			m_pTransform->TransformPoint(dTextR, dTextB);
			
			double dTextW = dTextR - dTextX;
			double dTextH = dTextB - dTextY;

            NSStringUtils::CStringUTF32 oText((uint32_t*)pUnicodes, nCount);

            if ((pUnicodes != NULL) && (pGids != NULL))
			{
                for (unsigned int i = 0; i < nCount; ++i)
				{
                    if ( !IsUnicodeSymbol( pUnicodes[i] ) )
					{
                        oText[i] = ' ';
                    }
				}
			}

            bool bIsPath = ((NULL == pGids) && !bIsPDFAnalyzer) ? false : true;

			m_oManager.LoadFont(0, !bIsPath);
			
			if (bIsPath)
                m_oManager.GenerateFontName2(oText);

			if ((0 == dTextW) || (dTextW > 5 * m_oManager.m_dSpaceWidthMM))
			{
				double _x = 0;
				double _y = 0;
				double _w = 0;
				double _h = 0;

                if (NULL != pGids)
				{
					m_oManager.SetStringGid(1);
                    m_oManager.MeasureStringGids(pGids, nCount, dTextX, dTextY, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
				}
				else
				{
					// такого быть не должно (только из xps)
					m_oManager.SetStringGid(0);
                    m_oManager.MeasureStringGids(pUnicodes, nCount, dTextX, dTextY, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
				}

				dTextW = _w;
				//dTextW *= c_dPixToMM;
			}

			double dBaseLinePos = dTextY + fBaseLineOffset;
			dTextH = m_oManager.GetFontHeight();

			SetCurrentLineByBaseline(dBaseLinePos);

			CContText* pLastCont = NULL;
            size_t nCountConts = m_pCurrentLine->m_arConts.size();
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

                if (IsSpaceUtf32(oText))
				{
					pCont->m_dWidthWithoutSpaces	= 0;
					pCont->m_dLeftWithoutSpaces		= dTextX + dTextW;
				}
				else
				{
					pCont->m_dWidthWithoutSpaces	= dTextW;
					pCont->m_dLeftWithoutSpaces		= dTextX;
				}

                pCont->m_oText = oText;

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
                    pLastCont->m_oText += oText;
					pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dX);

                    if (!IsSpaceUtf32(oText))
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
                    pLastCont->m_oText += uint32_t(' ');
                    pLastCont->m_oText += oText;
					pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dX);

                    if (!IsSpaceUtf32(oText))
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

            if (IsSpaceUtf32(oText))
			{
				pCont->m_dWidthWithoutSpaces	= 0;
				pCont->m_dLeftWithoutSpaces		= dTextX + dTextW;
			}
			else
			{
				pCont->m_dWidthWithoutSpaces	= dTextW;
				pCont->m_dLeftWithoutSpaces		= dTextX;
			}

            pCont->m_oText = oText;

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
                // удалим все линии, которые выходят за границы страницы
                size_t nCount = m_arTextLine.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					CTextLine* pTextLine = m_arTextLine[i];

					double _top = pTextLine->m_dBaselinePos - pTextLine->m_dHeight;
					double _bottom = pTextLine->m_dBaselinePos;

					if (_top >= m_dHeight || _bottom <= 0)
					{
                        m_arTextLine.erase(m_arTextLine.begin() + i);
						--i;
						--nCount;
					}
				}
			}

			switch (m_eTextAssociationType)
			{
                case TextAssociationTypeBlockChar:
				{
                    size_t nCount = m_arTextLine.size();
					for (size_t i = 0; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arTextLine[i];

						CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
						pParagraph->m_pManagerLight = &m_oManagerLight;
						pParagraph->m_bIsTextFrameProperties = true;

						pParagraph->m_dLeft	= pTextLine->m_dX;
						pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset;

                        pParagraph->m_arLines.push_back(pTextLine);

                        m_arParagraphs.push_back(pParagraph);
					}

					// удалим все линии
                    m_arTextLine.clear();
					break;
                }
                case TextAssociationTypeBlockLine:
                {
                    size_t nCount = m_arTextLine.size();

                    if (0 == nCount)
                        break;

                    CTextLine* pFirstLine = m_arTextLine[0];

                    CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
                    pParagraph->m_pManagerLight = &m_oManagerLight;
                    pParagraph->m_bIsTextFrameProperties = true;

                    pParagraph->m_dLeft	= pFirstLine->m_dX;
                    pParagraph->m_dTop	= pFirstLine->m_dBaselinePos - pFirstLine->m_dHeight + pFirstLine->m_dBaselineOffset;
                    double dCurrentTop = pParagraph->m_dTop;

                    pParagraph->m_arLines.push_back(pFirstLine);

                    m_arParagraphs.push_back(pParagraph);

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
                            dCurrentTop = pParagraph->m_dTop;
                        }
                        else
                        {
                            pParagraph->m_dLeft	= pFirstLine->m_dX;
                            pParagraph->m_dTop	= dCurrentTop;
                        }

                        pFirstLine = pTextLine;

                        pParagraph->m_arLines.push_back(pTextLine);
                        m_arParagraphs.push_back(pParagraph);
                    }

                    // удалим все линии
                    m_arTextLine.clear();
                    break;
                }
                case TextAssociationTypePlainLine:
				{
					SortElements(m_arTextLine);
					Merge(STANDART_STRING_HEIGHT_MM / 3);

					double previousStringOffset = 0;
                    size_t nCount = m_arTextLine.size();
					for (size_t i = 0; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arTextLine[i];

						CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
						pParagraph->m_pManagerLight = &m_oManagerLight;
						pParagraph->m_bIsTextFrameProperties = false;

						pParagraph->m_dLeft	= pTextLine->m_dX;
						
						double dBeforeSpacing = (pTextLine->m_dBaselinePos - previousStringOffset - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset);

						pParagraph->m_dSpaceBefore = std::max(dBeforeSpacing, 0.0);

						double dHeight = 1;
						if (pTextLine->m_dHeight != 0)
						{
							dHeight = pTextLine->m_dHeight;

							if (dBeforeSpacing < 0)
								dHeight += dBeforeSpacing;
						}

						pParagraph->m_dHeight = dHeight;

						previousStringOffset = pTextLine->m_dBaselinePos + pTextLine->m_dBaselineOffset;

                        pParagraph->m_arLines.push_back(pTextLine);
                        m_arParagraphs.push_back(pParagraph);
					}

                    m_arTextLine.clear();
					break;
				}
			}
		}

		void Merge(double dAffinity)
		{
            size_t nCount = m_arTextLine.size();
			if (1 < nCount)
			{
				CTextLine* pPrev = m_arTextLine[0];
				for (size_t i = 1; i < nCount; ++i)
				{
					CTextLine* pNext = m_arTextLine[i];

					if (fabs(pNext->m_dBaselinePos - pPrev->m_dBaselinePos) < dAffinity)
					{
						pPrev->Merge(pNext);

                        pNext->m_arConts.clear();
						RELEASEOBJECT(pNext);

                        m_arTextLine.erase(m_arTextLine.begin() + i);
						--i;
						--nCount;
						continue;
					}
					pPrev = pNext;
				}
			}
		}

        void Write(NSStringUtils::CStringBuilder& oWriter)
		{
			// drawings
            size_t nCountDrawings = m_arGraphicItems.size();
			if (0 != nCountDrawings)
			{
                oWriter.WriteString(L"<w:p><w:pPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr>");

				for (size_t i = 0; i < nCountDrawings; ++i)
				{
					m_arGraphicItems[i]->ToXml(oWriter);
				}

                oWriter.WriteString(L"</w:p>");
			}

            size_t nCountParagraphs = m_arParagraphs.size();
			for (size_t i = 0; i < nCountParagraphs; ++i)
			{
				m_arParagraphs[i]->ToXml(oWriter);
			}
		}

        void WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter)
		{
			// section
            int lWidthDx  = (int)(m_dWidth * c_dMMToDx);
            int lHeightDx = (int)(m_dHeight * c_dMMToDx);

			if (!bLastPage)
                oWriter.WriteString(L"<w:p><w:pPr><w:sectPr>");
			else
                oWriter.WriteString(L"<w:sectPr>");

            oWriter.WriteString(L"<w:pgSz w:w=\"");
            oWriter.AddInt((int)(m_dWidth * c_dMMToDx));
            oWriter.WriteString(L"\" w:h=\"");
            oWriter.AddInt((int)(m_dHeight * c_dMMToDx));
            oWriter.WriteString(L"\" w:orient=\"");
            (lWidthDx >= lHeightDx) ? oWriter.WriteString(L"landscape") : oWriter.WriteString(L"portrait");
            oWriter.WriteString(L"\"/>");

			if (!bLastPage)
                oWriter.WriteString(L"<w:pgMar w:top=\"0\" w:right=\"0\" w:bottom=\"0\" w:left=\"0\"/></w:sectPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr></w:p>");
			else
                oWriter.WriteString(L"<w:pgMar w:top=\"0\" w:right=\"0\" w:bottom=\"0\" w:left=\"0\" w:header=\"0\" w:footer=\"0\" w:gutter=\"0\"/></w:sectPr>");
		}
	};
}
