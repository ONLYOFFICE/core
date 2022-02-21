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
						pShape->m_bIsStroke  = true;
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

			if (!m_pCurrentLine->m_bIsFirstWord)
			{
				if (IsSpaceUtf32(oText))
				{
					m_pCurrentLine->m_bIsFirstWord = true;
				}
				else
				{
					m_pCurrentLine->m_dWidthFirstWord += dTextW;
				}
			}

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

                pCont->m_arWidthText.push_back(dTextW);

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
					pLastCont->m_arWidthText.push_back(pLastCont->m_dWidth);

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
					pLastCont->m_arWidthText.push_back(pLastCont->m_dWidth);

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

			pCont->m_arWidthText.push_back(dTextW);

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

		bool IsRect(const std::wstring& str, const std::wstring& sub)
		{
			if (0 == sub.length())
				return false;

			size_t count{};
			for (size_t offset = str.find(sub); offset != std::string::npos;
				 offset = str.find(sub, offset + sub.length()))
			{
				++count;
			}
			return (count <= 5 && str.find(L"m0") != std::string::npos) ? true : false;
		}

        bool IsWaveUnderline(CShape* pShape)
        {
            std::wstring sub = L",";

            size_t count{};
            for (size_t offset = pShape->m_strPath.find(sub); offset != std::string::npos;
                    offset = pShape->m_strPath.find(sub, offset + sub.length()))
            {
                ++count;
            }

            if (count > 5  && pShape->m_dHeight < 2)
                return true;

            return false;

        }

        bool IsSecondLine(const CShape* pCurShape, const CShape* pSecondShape)
        {
            bool IsEqualLeft = abs(pCurShape->m_dLeft - pSecondShape->m_dLeft) < 0.01;
            bool IsEqualWidth = abs(pCurShape->m_dWidth - pSecondShape->m_dWidth) < 0.01;
            bool bCheckHeight = pCurShape->m_dHeight < 3.0 && pSecondShape->m_dHeight < 3.0;
            double dDiffCoord{};
            if (pSecondShape->m_dTop > pCurShape->m_dTop)
            {
                dDiffCoord = pSecondShape->m_dTop - pCurShape->m_dTop - pCurShape->m_dHeight;
            }
            else
            {
                dDiffCoord = pCurShape->m_dTop - pSecondShape->m_dTop - pSecondShape->m_dHeight;
            }

            bool IsSuitableDiff = abs(dDiffCoord) < 1 ;

            return IsEqualLeft && IsEqualWidth && bCheckHeight && IsSuitableDiff;
        }

        bool IsSearchSecondLine(int lCurIdx, CShape* pCurShape)
        {
            size_t nCount = m_arGraphicItems.size();
            for (size_t i = lCurIdx+1; i < nCount; ++i)
            {
                if (m_arGraphicItems[i]->m_eType == NSDocxRenderer::CBaseItem::etShape)
                {
                    CShape* pShape = dynamic_cast <CShape *> (m_arGraphicItems[i]);
                    if (IsSecondLine(pCurShape, pShape))
                    {
                        m_arGraphicItems.erase(m_arGraphicItems.cbegin() + i);
                        pCurShape->m_TypeLine = NSStructures::LineProperties::DOUBLE;
                        return true;
                    }
                }
            }
            return false;
        }

        bool IsSearchSecondWave(int lCurIdx, CShape* pCurShape)
        {
            size_t nCount = m_arGraphicItems.size();
            for (size_t i = lCurIdx+1; i < nCount; ++i)
            {
                if (m_arGraphicItems[i]->m_eType == NSDocxRenderer::CBaseItem::etShape)
                {
                    CShape* pShape = dynamic_cast <CShape *> (m_arGraphicItems[i]);
                    if (IsWaveUnderline(pShape) && IsSecondLine(pCurShape, pShape))
                    {
                        m_arGraphicItems.erase(m_arGraphicItems.cbegin() + i);
                        return true;
                    }
                }
            }
            return false;

        }

        bool IsDashLine(const CShape* pCurShape, const CShape* pDashShape)
        {
            bool IsEqualWidth = abs(pCurShape->m_dWidth - pDashShape->m_dWidth) < 0.01;
            bool IsEqualTop = abs(pCurShape->m_dTop - pDashShape->m_dTop) < 0.01;
            bool IsEqualHeight = abs(pCurShape->m_dHeight - pDashShape->m_dHeight) < 0.01;
            bool IsMoreLeft = pDashShape->m_dLeft > pCurShape->m_dLeft;
            bool bCheckHeight = pCurShape->m_dHeight < 3.0 && pDashShape->m_dHeight < 3.0;

            return IsEqualHeight && IsEqualTop && IsEqualWidth && IsMoreLeft && bCheckHeight;
        }

        bool IsSearchDash(int lCurIdx, CShape* pCurShape)
        {
            double dCalculatedWidth = pCurShape->m_dWidth;
            size_t nCount = m_arGraphicItems.size();
            for (size_t i = lCurIdx+1; i < nCount; ++i)
            {
                if (m_arGraphicItems[i]->m_eType == NSDocxRenderer::CBaseItem::etShape)
                {
                    CShape* pShape = dynamic_cast <CShape *> (m_arGraphicItems[i]);
                    if (IsDashLine(pCurShape, pShape))
                    {
                        dCalculatedWidth = pShape->m_dLeft + pShape->m_dWidth - pCurShape->m_dLeft;
                        m_arGraphicItems.erase(m_arGraphicItems.cbegin() + i);
                        --nCount;
                        --i;
                    }
                }
            }

            if (dCalculatedWidth > pCurShape->m_dWidth)
            {
                if (pCurShape->m_dWidth < 0.55)
                    pCurShape->m_TypeLine = NSStructures::LineProperties::DOTTED;
                else if (pCurShape->m_dWidth < 1.5)
                    pCurShape->m_TypeLine = NSStructures::LineProperties::DASH;
                else if (pCurShape->m_dWidth < 3)
                    pCurShape->m_TypeLine = NSStructures::LineProperties::DASHLONG;
                //TODO: убрать
                else
                    pCurShape->m_TypeLine = NSStructures::LineProperties::SINGLE;

                pCurShape->m_dWidth = dCalculatedWidth;
                return true;
            }
            return false;
        }

        bool IsDotLine(const CShape* pCurShape, const CShape* pDotShape)
        {
            bool IsEqualTop = abs(pCurShape->m_dTop - pDotShape->m_dTop) < 0.01;
            bool IsSuitableDiff = abs(pDotShape->m_dLeft - pCurShape->m_dLeft - pCurShape->m_dWidth) < 0.5;
            bool IsSuitableWidth = pDotShape->m_dWidth;
            bool bCheckHeight = pCurShape->m_dHeight < 3.0 && pDotShape->m_dHeight < 3.0;

            return IsEqualTop && IsSuitableDiff && IsSuitableWidth && bCheckHeight;
        }

        double SearchWidthDotted(int lCurIdx, const CShape* pCurShape)
        {
            if (pCurShape->m_dWidth > 1.5 || pCurShape->m_dHeight > 3.0)
                return 0;

            size_t nCount = m_arGraphicItems.size();
            for (size_t i = lCurIdx+1; i < nCount; ++i)
            {
                if (m_arGraphicItems[i]->m_eType == NSDocxRenderer::CBaseItem::etShape)
                {
                    CShape* pShape = dynamic_cast <CShape *> (m_arGraphicItems[i]);
                    if (IsDotLine(pCurShape, pShape))
                        return pShape->m_dWidth;
                }
            }
            return 0;
        }

        void SearchDotted(int lCurIdx, CShape* pCurShape, double SecondWidth)
        {
            if (pCurShape->m_dHeight > 3.0)
            {
                pCurShape->m_TypeLine = NSStructures::LineProperties::SINGLE;
                return;
            }
            double dCalculatedWidth = pCurShape->m_dWidth;
            int lLongLine = 1;
            int lShortLine = 0;
            size_t nCount = m_arGraphicItems.size();
            for (size_t i = lCurIdx+1; i < nCount; ++i)
            {
                if (m_arGraphicItems[i]->m_eType == NSDocxRenderer::CBaseItem::etShape)
                {
                    CShape* pShape = dynamic_cast <CShape *> (m_arGraphicItems[i]);
                    if (abs(pShape->m_dTop - pCurShape->m_dTop) < 0.01 && pShape->m_dHeight < 3.0)
                    {
                        if (pShape->m_dWidth < 0.5)
                        {
                            ++lShortLine;
                            dCalculatedWidth = pShape->m_dLeft + pShape->m_dWidth - pCurShape->m_dLeft;
                            m_arGraphicItems.erase(m_arGraphicItems.cbegin() + i);
                            --nCount;
                            --i;
                        }
                        else if (abs(pShape->m_dWidth - pCurShape->m_dWidth) < 0.01)
                        {
                            ++lLongLine;
                            dCalculatedWidth = pShape->m_dLeft + pShape->m_dWidth - pCurShape->m_dLeft;
                            m_arGraphicItems.erase(m_arGraphicItems.cbegin() + i);
                            --nCount;
                            --i;
                        }
                    }
                }
            }

            pCurShape->m_dWidth = dCalculatedWidth;
            if (abs(lLongLine-lShortLine) < 2)
                pCurShape->m_TypeLine = NSStructures::LineProperties::DOTDASH;
            else
                pCurShape->m_TypeLine = NSStructures::LineProperties::DOTDOTDASH;

        }

        void ProcessingLineShape()
        {
            size_t nCount = m_arGraphicItems.size();
            for (int i = 0; i < nCount; ++i)
            {
                if (m_arGraphicItems[i]->m_eType == NSDocxRenderer::CBaseItem::etShape)
                {
                    CShape* pShape = dynamic_cast <CShape *> (m_arGraphicItems[i]);

                    if (pShape->m_dHeight > 3.0){
                        pShape->m_TypeLine = NSStructures::LineProperties::SINGLE;
                        continue;
                    }

                    if (IsWaveUnderline(pShape))
                    {
                        if (IsSearchSecondWave(i, pShape))
                        {
                            pShape->m_TypeLine = NSStructures::LineProperties::WAVYDOUBLE;
                            nCount = m_arGraphicItems.size();
                        }
                        else
                        {
                            pShape->m_TypeLine = NSStructures::LineProperties::WAVE;
                        }
                        continue;
                    }
                    if (IsSearchSecondLine(i, pShape))
                    {
                        nCount = m_arGraphicItems.size();
                        continue;
                    }

                    double dWidthDotted = SearchWidthDotted(i, pShape);
                    if (dWidthDotted > 0)
                    {
                        SearchDotted(i, pShape, dWidthDotted);
                        nCount = m_arGraphicItems.size();
                        continue;
                    }

                    if (IsSearchDash(i, pShape))
                    {
                        nCount = m_arGraphicItems.size();
                        continue;
                    }
                    pShape->m_TypeLine = NSStructures::LineProperties::SINGLE;
                }
            }
        }

        void SetThickUnderlineType(CShape* pShape)
        {
            switch (pShape->m_TypeLine)
            {
                case NSStructures::LineProperties::SINGLE:
                    pShape->m_TypeLine = NSStructures::LineProperties::THICK;
                    break;
                case NSStructures::LineProperties::DOTTED:
                    pShape->m_TypeLine = NSStructures::LineProperties::DOTTEDHEAVY;
                    break;
                case NSStructures::LineProperties::DASH:
                    pShape->m_TypeLine = NSStructures::LineProperties::DASHEDHEAVY;
                    break;
                case NSStructures::LineProperties::DASHLONG:
                    pShape->m_TypeLine = NSStructures::LineProperties::DASHLONGHEAVY;
                    break;
                case NSStructures::LineProperties::DOTDASH:
                    pShape->m_TypeLine = NSStructures::LineProperties::DASHDOTHEAVY;
                    break;
                case NSStructures::LineProperties::DOTDOTDASH:
                    pShape->m_TypeLine = NSStructures::LineProperties::DASHDOTDOTHEAVY;
                    break;
//                case NSStructures::LineProperties::WAVE:
//                    pShape->m_TypeLine = NSStructures::LineProperties::WAVYHEAVY;
//                    break;
                default:
                    break;
            }
        }

        bool IsStrikeout(CShape* pShape, CTextLine* pTextLine)
        {
            double dTopTextLine    = pTextLine->m_dBaselinePos - pTextLine->m_dHeight;
            double dBottomTextLine = pTextLine->m_dBaselinePos;

            bool bCheckTop    = dTopTextLine < pShape->m_dTop;
            bool bCheckHeight = pShape->m_dHeight < 3.0;
            bool bCheckBottom = dBottomTextLine > pShape->m_dHeight + pShape->m_dTop;
            bool bColorEquals = pTextLine->IsMatchColor(pShape->m_oBrush.Color1) ||
                                pTextLine->IsMatchColor(pShape->m_oPen.Color);

            return bCheckTop && bCheckBottom &&
                   bColorEquals && bCheckHeight;
        }

        bool IsUnderline(CShape* pShape, CTextLine* pTextLine)
        {
            double dBottomTextLine = pTextLine->m_dBaselinePos;

            bool bCheckTop    = abs(pShape->m_dTop - dBottomTextLine) < 2;
            bool bCheckHeight = pShape->m_dHeight < 3.0;
            bool bCheckBottom = pShape->m_dTop > dBottomTextLine;
            bool bColorEquals = pTextLine->IsMatchColor(pShape->m_oBrush.Color1) ||
                                pTextLine->IsMatchColor(pShape->m_oPen.Color);

            return bCheckTop    && bCheckHeight &&
                   bCheckBottom && bColorEquals;
        }

        bool IsBackground(CShape* pShape, CTextLine* pTextLine)
        {
            double dBottomTextLine = pTextLine->m_dBaselinePos;
            double dLeftTextLine   = pTextLine->m_dX;
            double dRightTextLine  = pTextLine->m_arConts.back()->m_dX + pTextLine->m_arConts.back()->m_dWidth;

            bool bCheckTop    = dBottomTextLine > pShape->m_dTop;
            bool bCheckBottom = dBottomTextLine < pShape->m_dHeight + pShape->m_dTop;
            bool bCheckLeft   = dLeftTextLine   < pShape->m_dLeft + pShape->m_dWidth;
            bool bCheckRight  = dRightTextLine  > pShape->m_dLeft;
            bool bCheckHeight = pShape->m_dHeight/pTextLine->m_dHeight < 2;

            return bCheckTop  && bCheckBottom &&
                   bCheckLeft && bCheckRight  && bCheckHeight;
        }

        bool IsHighlightColor(const int &key)
        {
            std::map <int, std::wstring> colorHighlight = {
                {0, L"black"}, {16711680, L"blue"}, {65280, L"green"},
                {9109504, L"darkBlue"}, {139, L"darkRed"}, {32896, L"darkYellow"},
                {13882323, L"lightGray"}, {11119017, L"darkGray"}, {25600, L"darkGreen"},
                {16711935, L"magenta"}, {255, L"red"}, {16776960, L"cyan"},
                {9145088, L"darkCyan"}, {8388736, L"darkMagenta"}, {65535, L"yellow"}
            };

            auto it = colorHighlight.find(key);
            if ( it == colorHighlight.end() )
               return false;
            else
               return true;
        }

        bool IsShading(const CShape* pCurShape, int lIdx)
        {
            double dCurShapeLeft   = pCurShape->m_dLeft;
            double dCurShapeRight  = pCurShape->m_dLeft + pCurShape->m_dWidth;
            double dCurShapeTop    = pCurShape->m_dTop;
            double dCurShapeBottom = pCurShape->m_dTop + pCurShape->m_dHeight;

            size_t nCount = m_arGraphicItems.size();
            for (int i = 0; i < nCount; ++i)
            {
                if (lIdx == i)
                    break;

                if (m_arGraphicItems[i]->m_eType == NSDocxRenderer::CBaseItem::etShape)
                {
                    CShape* pShape = dynamic_cast <CShape *> (m_arGraphicItems[i]);
                    double dShapeLeft   = pShape->m_dLeft;
                    double dShapeRight  = pShape->m_dLeft + pShape->m_dWidth;
                    double dShapeTop    = pShape->m_dTop;
                    double dShapeBottom = pShape->m_dTop  + pShape->m_dHeight;

                    if (abs(dShapeLeft - dCurShapeLeft) < 0.1 && abs(dShapeRight - dCurShapeRight) < 0.1 &&
                        (abs(dShapeTop - dCurShapeBottom) < 0.1 || abs(dShapeBottom - dCurShapeTop) < 0.1) )
                    {
                        return true;
                    }
                }
            }
            return false;
        }

		void SetTextOptions()
		{
			ProcessingLineShape();
			std::vector <int> arIdxGraphicItems;
			size_t nCount = m_arGraphicItems.size();
			int counter = -1;
			for (size_t i = 0; i < nCount; ++i)
			{
				++counter;
				if (m_arGraphicItems[i]->m_eType == NSDocxRenderer::CBaseItem::etShape)
				{
					CShape* pShape = dynamic_cast <CShape *> (m_arGraphicItems[i]);
					if (IsRect(pShape->m_strPath, L",") || IsWaveUnderline(pShape))
					{
						size_t nCountTextLine = m_arTextLine.size();
						for (size_t j = 0; j < nCountTextLine; ++j)
						{
							if (pShape->m_dHeight > 3.0)
							{
								if (IsBackground(pShape, m_arTextLine[j]))
								{
									if (IsShading(pShape, i))
									{
									    m_arTextLine[j]->SearchInclusions(pShape, ModeFontOptions::SHADING);
									}
									else if (IsHighlightColor(pShape->m_oBrush.Color1))
									{
									    m_arTextLine[j]->SearchInclusions(pShape, ModeFontOptions::HIGHLIGHT);
									}
									else
									{
									    m_arTextLine[j]->SearchInclusions(pShape, ModeFontOptions::SHADING);
									}

									arIdxGraphicItems.push_back(counter);
									break;
								}
							}
							if ( IsStrikeout(pShape, m_arTextLine[j])  )
							{
								m_arTextLine[j]->SearchInclusions(pShape, ModeFontOptions::STRIKEOUT);
								arIdxGraphicItems.push_back(counter);
								break;
							}

							if ( IsUnderline(pShape, m_arTextLine[j]) )
							{
								if (m_arTextLine[j]->m_dHeight/pShape->m_dHeight < 18 )
									SetThickUnderlineType(pShape);

								m_arTextLine[j]->SearchInclusions(pShape, ModeFontOptions::UNDERLINE);
								arIdxGraphicItems.push_back(counter);
								break;
							}
						}
					}
				}
			}

            auto iter = m_arGraphicItems.cbegin();
            for (size_t i = arIdxGraphicItems.size(); i > 0; --i)
                m_arGraphicItems.erase(iter + arIdxGraphicItems[i-1]);

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
            case TextAssociationTypePlainParagraph:
				{
					SetTextOptions();

					SortElements(m_arTextLine);
					Merge(STANDART_STRING_HEIGHT_MM / 3);

					AddingFirstLine();
					
					double previousStringOffset = m_arTextLine[0]->m_dBaselinePos + m_arTextLine[0]->m_dBaselineOffset;
					double dPrevDiffBaselinePos = 0;
					
					CTextLine* pPrevTextLine = m_arTextLine[0];
					size_t nCount = m_arTextLine.size();
					for (size_t i = 1; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arTextLine[i];

						pTextLine->CalculatingLineWidth();
						double dSpacingRight = this->m_dWidth - pTextLine->m_dX - pTextLine->m_dWidth;

						if ( IsNewParagraph(pTextLine, pPrevTextLine, dPrevDiffBaselinePos) )
						{
							CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
							pParagraph->m_pManagerLight = &m_oManagerLight;
							pParagraph->m_bIsTextFrameProperties = false;

							pParagraph->m_dLeft	= pTextLine->m_dX;

							double dBeforeSpacing = pTextLine->m_dBaselinePos - previousStringOffset - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset;
							dBeforeSpacing = std::max(dBeforeSpacing, 0.0);
							pParagraph->m_dHeight = m_arTextLine[i]->CalculatingLineHeight(dBeforeSpacing);

							dPrevDiffBaselinePos = pTextLine->m_dBaselinePos - pPrevTextLine->m_dBaselinePos;
							pParagraph->m_dSpaceBefore = dPrevDiffBaselinePos;

							pParagraph->m_arLines.push_back(pTextLine);
							pParagraph->m_dSpaceRight = dSpacingRight;
							pParagraph->m_dWidth = pTextLine->m_dWidth;
							m_arParagraphs.push_back(pParagraph);
						}
						else
						{
 							if (m_arParagraphs.back()->m_dSpaceRight > dSpacingRight)
								m_arParagraphs.back()->m_dSpaceRight = dSpacingRight;

							if (pTextLine->m_dWidth > m_arParagraphs.back()->m_dWidth)
								m_arParagraphs.back()->m_dWidth = pTextLine->m_dWidth;

							dPrevDiffBaselinePos = pTextLine->m_dBaselinePos - pPrevTextLine->m_dBaselinePos;
							m_arParagraphs.back()->m_dHeight = dPrevDiffBaselinePos;

							m_arParagraphs.back()->m_arLines.push_back(pTextLine);
						}
						previousStringOffset = pTextLine->m_dBaselinePos + pTextLine->m_dBaselineOffset;
						pPrevTextLine = pTextLine;
					}

					if (m_arParagraphs[0]->m_arLines.size() == 1)
						m_arParagraphs[0]->m_dSpaceRight = 0;

					for (size_t i = 1; i < this->m_arParagraphs.size(); ++i)
					{
						if (m_arParagraphs[i]->m_dSpaceBefore > m_arParagraphs[i]->m_dHeight)
							m_arParagraphs[i]->m_dSpaceBefore -= m_arParagraphs[i]->m_dHeight;
						else
						{
							double dTemp = m_arParagraphs[i]->m_dSpaceBefore;
							m_arParagraphs[i]->m_dSpaceBefore = m_arParagraphs[i]->m_dHeight - m_arParagraphs[i]->m_dSpaceBefore;
							m_arParagraphs[i]->m_dHeight = dTemp;
						}
						
						if (m_arParagraphs[i]->m_arLines.size() == 1)
							m_arParagraphs[i]->m_dSpaceRight = 0;
						else
							AlignmentParagraph(i);
						
					}
					m_arTextLine.clear();
					break;
				}
			}
		}

		void AlignmentParagraph(size_t nNumParagraph)
		{
			size_t i = nNumParagraph;
			size_t nAlignmentLeft = 1;
			size_t nAlignmentRight = 1;

			CTextLine* pPrevTextLine = m_arParagraphs[i]->m_arLines[0];
			size_t nCountLine = m_arParagraphs[i]->m_arLines.size();
			for (size_t k = 1; k < nCountLine; ++k)
			{
				CTextLine* pCurTextLine = m_arParagraphs[i]->m_arLines[k];

				if ( abs(pPrevTextLine->m_dX - pCurTextLine->m_dX) < 0.5 )
					++nAlignmentLeft;

				if ( abs(pPrevTextLine->m_dWidth + pPrevTextLine->m_dX - pCurTextLine->m_dWidth - pCurTextLine->m_dX) < 1.5 )
					++nAlignmentRight;

				pPrevTextLine = pCurTextLine;
			}

			if ((nCountLine > 0) && (nAlignmentLeft >= nCountLine-1))
			{
				m_arParagraphs[i]->m_strAvailable = L"left";
				if (nAlignmentRight >= nCountLine/2)
					m_arParagraphs[i]->m_strAvailable = L"both";
			}
			else
			{
				m_arParagraphs[i]->m_strAvailable = L"center";
				if ((nCountLine > 0) && (nAlignmentRight >= nCountLine-1)) 
					m_arParagraphs[i]->m_strAvailable = L"right";
			}
		}

		void AddingFirstLine()
		{
			m_arTextLine[0]->CalculatingLineWidth();
			CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
			pParagraph->m_pManagerLight = &m_oManagerLight;
			pParagraph->m_bIsTextFrameProperties = false;
			pParagraph->m_dLeft	= m_arTextLine[0]->m_dX;

			double dBeforeSpacing = m_arTextLine[0]->m_dBaselinePos - m_arTextLine[0]->m_dHeight + m_arTextLine[0]->m_dBaselineOffset;
			pParagraph->m_dSpaceBefore = std::max(dBeforeSpacing, 0.0);

			double dHeight = m_arTextLine[0]->CalculatingLineHeight(dBeforeSpacing);
			pParagraph->m_dHeight = dHeight;

			pParagraph->m_arLines.push_back(m_arTextLine[0]);
			pParagraph->m_dSpaceRight = this->m_dWidth - m_arTextLine[0]->m_dX - m_arTextLine[0]->m_dWidth;
			m_arParagraphs.push_back(pParagraph);
		}

        bool IsNewParagraph(CTextLine* pTextLine, CTextLine* pPrevTextLine, double dPrevDiffBaselinePos)
        {

            if (pTextLine->m_dX > pPrevTextLine->m_dX && (pTextLine->m_dX + pTextLine->m_dWidth) > (pPrevTextLine->m_dX + pPrevTextLine->m_dWidth))
                return true;

            if (pTextLine->m_dX < pPrevTextLine->m_dX && (pTextLine->m_dX + pTextLine->m_dWidth) < (pPrevTextLine->m_dX + pPrevTextLine->m_dWidth))
                return true;

            const double dWidthNewlineChar = 1.2;
            double dCurDiffBaselinePos = pTextLine->m_dBaselinePos - pPrevTextLine->m_dBaselinePos;

            double dDiffWidth             = abs(pTextLine->m_dWidth - pPrevTextLine->m_dWidth);
            double dDiffParagraphWidth    = abs(m_arParagraphs.back()->m_dWidth - pPrevTextLine->m_dWidth);
            double dWidthFirstWordCurLine = pTextLine->m_dWidthFirstWord;
            bool IsCurLineLongerPrev = pTextLine->m_dWidth >  pPrevTextLine->m_dWidth;

            bool bFirstWordCheck1 = IsCurLineLongerPrev  &&  dDiffWidth >= dWidthFirstWordCurLine;
            bool bFirstWordCheck2 = IsCurLineLongerPrev  &&  dDiffParagraphWidth >= dWidthFirstWordCurLine;
            bool bFirstWordCheck3 = !IsCurLineLongerPrev &&  dDiffParagraphWidth >= dWidthFirstWordCurLine;

            bool bIsCurLineSpacingEqualPrev  = (dCurDiffBaselinePos/dPrevDiffBaselinePos) > 2.0 ||
                                               (dCurDiffBaselinePos/dPrevDiffBaselinePos) < 0.5 ;
            bool bIsCurLineNewlineCharacter  = pTextLine->m_dWidth < dWidthNewlineChar;
            bool bIsPrevLineNewlineCharacter = pPrevTextLine->m_dWidth < dWidthNewlineChar;

            if ( bIsCurLineNewlineCharacter	|| bIsPrevLineNewlineCharacter ||
                 bIsCurLineSpacingEqualPrev || bFirstWordCheck1            ||
                 bFirstWordCheck2           || bFirstWordCheck3)
            {
                return true;
            }

            double dMaxWidth = pTextLine->m_dWidth;
            double dMinWidth = pTextLine->m_dWidth;
            for (int i = 0; i < m_arParagraphs.back()->m_arLines.size(); ++i)
            {
                if (m_arParagraphs.back()->m_arLines[i]->m_dWidth < dMinWidth)
                    dMinWidth = m_arParagraphs.back()->m_arLines[i]->m_dWidth;

                if (m_arParagraphs.back()->m_arLines[i]->m_dWidth > dMaxWidth)
                    dMaxWidth = m_arParagraphs.back()->m_arLines[i]->m_dWidth;
            }

            dDiffWidth = dMaxWidth - dMinWidth;

            if (IsCurLineLongerPrev && dDiffWidth >= dWidthFirstWordCurLine &&
                m_arParagraphs.back()->m_arLines.size() > 1 )
            {
                return true;
            }
            return false;
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
