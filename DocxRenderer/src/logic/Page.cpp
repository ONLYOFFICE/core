#include "Page.h"

namespace NSDocxRenderer
{
    CPage::CPage(NSFonts::IApplicationFonts* pFonts) : m_oManager(pFonts), m_oManagerLight(pFonts)
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

        m_dLastTextX = -1;
        m_dLastTextY = -1;
        m_dLastTextX_block = m_dLastTextX;
    }

    void CPage::Init(NSStructures::CFont* pFont, NSStructures::CPen* pPen, NSStructures::CBrush* pBrush,
                     NSStructures::CShadow* pShadow, NSStructures::CEdgeText* pEdge, Aggplus::CMatrix* pMatrix,
                     Aggplus::CGraphicsPathSimpleConverter* pSimple)
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

        m_dLastTextX = -1;
        m_dLastTextY = -1;
        m_dLastTextX_block = m_dLastTextX;
    }

    void CPage::Clear()
    {
        ClearTextLines();
        ClearGraphicItems();
        ClearParagraphs();

        m_pCurrentLine = NULL;

        m_oWriterVML.ClearNoAttack();

        m_dLastTextX = -1;
        m_dLastTextY = -1;
        m_dLastTextX_block = m_dLastTextX;
    }

    void CPage::ClearTextLines()
    {
        size_t nCount = m_arTextLine.size();
        for (size_t i = 0; i < nCount; ++i)
        {
            CTextLine* pTemp = m_arTextLine[i];
            RELEASEOBJECT(pTemp);
        }
        m_arTextLine.clear();
    }

    void CPage::ClearGraphicItems()
    {
        size_t nCount = m_arGraphicItems.size();
        for (size_t i = 0; i < nCount; ++i)
        {
            CBaseItem* pTemp = m_arGraphicItems[i];
            RELEASEOBJECT(pTemp);
        }
        m_arGraphicItems.clear();
    }

    void CPage::ClearParagraphs()
    {
        size_t nCount = m_arParagraphs.size();
        for (size_t i = 0; i < nCount; ++i)
        {
            CParagraph* pTemp = m_arParagraphs[i];
            RELEASEOBJECT(pTemp);
        }
        m_arParagraphs.clear();
    }

    CPage::~CPage()
    {
        Clear();
    }

    void CPage::SetCurrentLineByBaseline(const double& dBaseLinePos)
    {
        if ((NULL == m_pCurrentLine) || (TextAssociationTypeBlockChar == m_eTextAssociationType))
        {
            // пустая (в плане текста) страница

            m_pCurrentLine = new CTextLine();
            m_pCurrentLine->m_dBaselinePos = dBaseLinePos;
            m_arTextLine.push_back(m_pCurrentLine);
            return;
        }
        if (fabs(m_pCurrentLine->m_dBaselinePos - dBaseLinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
        {
            return;
        }
        size_t nCount = m_arTextLine.size();
        for (size_t i = 0; i < nCount; ++i)
        {
            if (fabs(m_arTextLine[i]->m_dBaselinePos - dBaseLinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
            {
                m_pCurrentLine = m_arTextLine[i];
                return;
            }
        }

        // линия не нашлась - не беда - создадим новую
        m_pCurrentLine = new CTextLine();
        m_pCurrentLine->m_dBaselinePos = dBaseLinePos;
        m_arTextLine.push_back(m_pCurrentLine);
        return;
    }

    void CPage::DeleteTextClipPage()
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
    }

    // image commands
    void CPage::WriteImage(CImageInfo& oInfo, double& fX, double& fY, double& fWidth, double& fHeight)
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
    void CPage::MoveTo(double& dX, double& dY)
    {
        m_pTransform->TransformPoint(dX, dY);
        m_oVector.MoveTo(dX, dY);

    }
    void CPage::LineTo(double& dX, double& dY)
    {
        m_pTransform->TransformPoint(dX, dY);
        m_oVector.LineTo(dX, dY);
    }
    void CPage::CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
    {
        m_pTransform->TransformPoint(x1, y1);
        m_pTransform->TransformPoint(x2, y2);
        m_pTransform->TransformPoint(x3, y3);

        m_oVector.CurveTo(x1, y1, x2, y2, x3, y3);
    }
    void CPage::Start()
    {
    }
    void CPage::End()
    {
        m_oVector.End();
        m_oWriterVML.ClearNoAttack();
    }
    void CPage::Close()
    {
        m_oVector.Close();
    }
    void CPage::DrawPath(LONG lType, LONG lTxId)
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

    void CPage::WriteText(unsigned int* pUnicodes, unsigned int* pGids, unsigned int nCount, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, bool bIsPDFAnalyzer)
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

        if (fabs(dTextW) < 0.01 || (dTextW > 10))
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
            pCont->m_dLastX = dTextX;
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

            m_dLastTextX = dTextX;
            m_dLastTextY = dBaseLinePos;
            m_dLastTextX_block = m_dLastTextX;
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
            // настройки одинаковые. теперь смотрим, на расположение
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

                m_dLastTextX = dTextX;
                m_dLastTextY = dBaseLinePos;
                m_dLastTextX_block = m_dLastTextX;
                pLastCont->m_dLastX = dTextX;
                return;
            }
            else if ((dRight < dTextX) && ((dTextX - dRight) < m_oManager.m_dSpaceWidthMM))
            {
                // продолжаем слово с пробелом
                if (m_eTextAssociationType != TextAssociationTypePlainParagraph &&
                    m_eTextAssociationType != TextAssociationTypeShapeLine)
                {
                    pLastCont->m_oText += uint32_t(' ');
                }
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

                m_dLastTextX = dTextX;
                m_dLastTextY = dBaseLinePos;
                m_dLastTextX_block = m_dLastTextX;
                pLastCont->m_dLastX = dTextX;
                return;
            }
            else if (fabs(dBaseLinePos - pLastCont->m_dY) < 0.01 &&
                     fabs(m_dLastTextY - pLastCont->m_dY) < 0.01 &&
                     fabs(m_dLastTextX - pLastCont->m_dLastX) < 0.01)
            {
                // идет текст подряд, но с расстояниями что-то не так. смотрим - если новый текст идет после предыдущего, но
                // просто левее чем предыдущий x + w - то считаем это нормальным. и дописываем слово. корректируя длину
                if (dTextX < dRight && dTextX > pLastCont->m_dLastX)
                {
                    // продолжаем слово
                    pLastCont->m_oText += oText;
                    double dNewW = (dTextX + dTextW - pLastCont->m_dX);
                    if (pLastCont->m_dWidth < dNewW)
                        pLastCont->m_dWidth = dNewW;

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

                    m_dLastTextX = dTextX;
                    m_dLastTextY = dBaseLinePos;
                    m_dLastTextX_block = m_dLastTextX;
                    pLastCont->m_dLastX = dTextX;
                    return;

                }

                // еще одна заглушка на большой пробел - добавляем пробел, потом в линии все разрулится через spacing
                if (dTextX > dRight && (dTextX - dRight) < 5 && fabs(m_dLastTextX_block - m_dLastTextX) < 0.01)
                {
                    // продолжаем слово с пробелом
                    if (m_eTextAssociationType != TextAssociationTypePlainParagraph &&
                        m_eTextAssociationType != TextAssociationTypeShapeLine)
                    {
                        pLastCont->m_oText += uint32_t(' ');
                    }
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

                    m_dLastTextX = dTextX;
                    m_dLastTextY = dBaseLinePos;
                    m_dLastTextX_block = m_dLastTextX;
                    pLastCont->m_dLastX = dTextX;
                    return;
                }
            }
        }

        // либо пробел большой между словами, либо новый текст левее, либо настройки не те (шрифт, кисть)
        // либо все вместе... просто добавл¤ем новое слово
        CContText* pCont = new CContText();

        pCont->m_dX = dTextX;
        pCont->m_dY = dBaseLinePos;
        pCont->m_dLastX = dTextX;

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

        m_dLastTextX = dTextX;
        m_dLastTextY = dBaseLinePos;
        m_dLastTextX_block = m_dLastTextX;
    }

    void CPage::Build()
    {
        DeleteTextClipPage();

        if (0 == m_arTextLine.size())
            return;

        switch (m_eTextAssociationType)
        {
        case TextAssociationTypeBlockChar:
            BuildByTypeBlockChar();
            break;
        case TextAssociationTypeBlockLine:
            BuildByTypeBlockLine();
            break;
        case TextAssociationTypePlainLine:
            BuildByTypePlainLine();
            break;
        case TextAssociationTypeShapeLine:
            BuildByTypeShapeLine();
            break;
        case TextAssociationTypePlainParagraph:
            BuildByTypePlainParagraph();
            break;
        default:
            break;
        }
    }

    void CPage::BuildByTypeBlockChar()
    {
        size_t nCount = m_arTextLine.size();
        for (size_t i = 0; i < nCount; ++i)
        {
            CTextLine* pTextLine = new CTextLine(*m_arTextLine[i]);;

            CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
            pParagraph->m_pManagerLight = &m_oManagerLight;
            pParagraph->m_eTextConversionType = CParagraph::TextToFrame;

            pParagraph->m_dLeft	= pTextLine->m_dX;
            pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pTextLine->m_dBaselineOffset;

            pParagraph->m_arLines.push_back(pTextLine);

            m_arParagraphs.push_back(pParagraph);
        }
    }

    void CPage::BuildByTypeBlockLine()
    {
        CTextLine* pFirstLine = new CTextLine(*m_arTextLine[0]);

        CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
        pParagraph->m_pManagerLight = &m_oManagerLight;
        pParagraph->m_eTextConversionType = CParagraph::TextToFrame;

        pParagraph->m_dLeft	= pFirstLine->m_dX;
        pParagraph->m_dTop	= pFirstLine->m_dBaselinePos - pFirstLine->m_dHeight - pFirstLine->m_dBaselineOffset;
        double dCurrentTop = pParagraph->m_dTop;

        pParagraph->m_arLines.push_back(pFirstLine);

        m_arParagraphs.push_back(pParagraph);
        size_t nCount = m_arTextLine.size();
        for (size_t i = 1; i < nCount; ++i)
        {
            CTextLine* pTextLine = new CTextLine(*m_arTextLine[i]);

            CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
            pParagraph->m_pManagerLight = &m_oManagerLight;
            pParagraph->m_eTextConversionType = CParagraph::TextToFrame;

            if (((fabs(pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pFirstLine->m_dBaselinePos) > c_dSTANDART_STRING_HEIGHT_MM) && (pTextLine->m_dX == pFirstLine->m_dX)) ||
                    ((pTextLine->m_dX != pFirstLine->m_dX) && (pTextLine->m_dBaselinePos != pFirstLine->m_dBaselinePos)))
            {
                pParagraph->m_dLeft	= pTextLine->m_dX;
                pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pTextLine->m_dBaselineOffset;
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
    }

    void CPage::BuildByTypePlainLine()
    {
        SortElements(m_arTextLine);

        if (true) // merge line
        {
            for (std::vector<CTextLine*>::iterator iter = m_arTextLine.begin(); iter != m_arTextLine.end(); ++iter)
            {
                (*iter)->Analyze();
            }
        }
        Merge(c_dSTANDART_STRING_HEIGHT_MM / 3);

        double previousStringOffset = 0;
        for (size_t i = 0; i < m_arTextLine.size(); ++i)
        {
            CTextLine* pTextLine = new CTextLine(*m_arTextLine[i]);;

            CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
            pParagraph->m_pManagerLight = &m_oManagerLight;
            pParagraph->m_eTextConversionType = CParagraph::TextToParagraph;
#if 0
            // у рамок нельзя выключить обтекание. поэтому в этом случае нужно конверировать в шейп
            if (pTextLine->IsForceBlock())
            {
                pParagraph->m_eTextTextConversionType = CParagraph::TextToFrame;
                pParagraph->m_dLeft	= pTextLine->m_dX;
                pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pTextLine->m_dBaselineOffset;

                pParagraph->m_arLines.push_back(pTextLine);
                m_arParagraphs.push_back(pParagraph);
                continue;
            }
#endif

            pParagraph->m_dLeft	= pTextLine->m_dX;
            pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pTextLine->m_dBaselineOffset;
            pParagraph->m_dRight = m_dWidth - (pTextLine->m_dX + pTextLine->m_dWidth);

            double dBeforeSpacing = pTextLine->m_dBaselinePos - previousStringOffset - pTextLine->m_dHeight - pTextLine->m_dBaselineOffset;

            pParagraph->m_dSpaceBefore = std::max(dBeforeSpacing, 0.0);
            pParagraph->m_dBaselinePos = pTextLine->m_dBaselinePos;

            double dHeight = 1;
            if (pTextLine->m_dHeight != 0)
            {
                dHeight = pTextLine->m_dHeight;

                if (dBeforeSpacing < 0)
                    dHeight += dBeforeSpacing;
            }

            pParagraph->m_dHeight = dHeight;

            previousStringOffset = pTextLine->m_dBaselinePos - pTextLine->m_dBaselineOffset;

            pParagraph->m_arLines.push_back(pTextLine);
            pParagraph->m_nNumLines++;

            m_arParagraphs.push_back(pParagraph);
        }
    }

    void CPage::BuildByTypeShapeLine()
    {
        SortElements(m_arTextLine);
        for (std::vector<CTextLine*>::iterator iter = m_arTextLine.begin(); iter != m_arTextLine.end(); ++iter)
        {
            (*iter)->SortConts();
            (*iter)->Analyze();
            (*iter)->CalculateWidth();
        }

        for (size_t i = 0; i < m_arTextLine.size(); ++i)
        {
            CTextLine* pTextLine = new CTextLine(*m_arTextLine[i]);

            CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
            pParagraph->m_pManagerLight = &m_oManagerLight;
            pParagraph->m_eTextConversionType = CParagraph::TextToShape;
            pParagraph->m_arLines.push_back(pTextLine);

            CShape* pShape = new CShape();
            pShape->m_arParagraphs.push_back(pParagraph);

            //todo В итоге Left и Top смещаются на несколько мм. Пока не понял чем это вызвано.
            pShape->m_dLeft	= pTextLine->m_dX - CShape::POSITION_CORRECTION_FOR_X_MM; //подобранная константа
            pShape->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pTextLine->m_dBaselineOffset - CShape::POSITION_CORRECTION_FOR_Y_MM;//подобранная константа
            //todo Width и Height маловаты и текст обрезается. разобраться почему
            pShape->m_dWidth = pTextLine->m_dWidth + CShape::SIZE_CORRECTION_FOR_X_MM; //5мм мало для длинной строки
            pShape->m_dHeight = pTextLine->m_dHeight + CShape::SIZE_CORRECTION_FOR_Y_MM;
            pShape->m_pManagerLight = &m_oManagerLight;

            m_arGraphicItems.push_back(pShape);
        }
    }

    void CPage::CalculateTextAlignmentType(CParagraph& a_rParagraph)
    {
        //todo нужно доработать
        if (a_rParagraph.m_dFirstLine == 0) //еще не был определен или для параграфа с одной строкой внутри
        {
            static const double maxStringWidth = m_dWidth/2; //нужна какая-нибудь константа...
            static const double ERROR_X_MM = 1;
            double delta = 2 * ERROR_X_MM; //координата m_dWidth/2 +- c_dTHE_STRING_X_PRECISION_MM

            if (fabs(m_dWidth/2 - a_rParagraph.m_dLeft - a_rParagraph.m_dWidth/2) <= delta && //если середины линий по x одинаковы
                    a_rParagraph.m_dWidth < maxStringWidth )
            {
                a_rParagraph.m_eTextAlignmentType = CParagraph::TextAlignmentType_ByCenter;
            }
            else if ((a_rParagraph.m_dLeft + a_rParagraph.m_dWidth/2) > (m_dWidth/2 + ERROR_X_MM) && //середина строки правее центра страницы
                     a_rParagraph.m_dWidth < maxStringWidth)
            {
                a_rParagraph.m_eTextAlignmentType = CParagraph::TextAlignmentType_ByRightEdge;
            }
            else if ((a_rParagraph.m_dLeft + a_rParagraph.m_dWidth/2) < (m_dWidth/2 - ERROR_X_MM) && //середина строки левее центра страницы
                     a_rParagraph.m_dWidth < maxStringWidth)
            {
                a_rParagraph.m_eTextAlignmentType = CParagraph::TextAlignmentType_ByLeftEdge;
            }
            else
            {
                a_rParagraph.m_eTextAlignmentType = CParagraph::TextAlignmentType_ByWidth;
            }
        }
        else
        {
            //todo
            a_rParagraph.m_eTextAlignmentType = CParagraph::TextAlignmentType_ByWidth;
        }

    }


    void CPage::BuildByTypePlainParagraph()
    {
        //todo если линия содержит длинный пробел, то выравнивание по правому краю неправильно
        //(больше чем нужно относительно правого края) Поэтому строка переносится

        //todo не отображается перенос в линии
        //todo если в линии есть перенос нужно обеъдинить строки в один параграф

        //todo добавить различные типы текста для распознавания: обычный-сплошной, списки-содержание и тд

        SortElements(m_arTextLine);
        for (std::vector<CTextLine*>::iterator iter = m_arTextLine.begin(); iter != m_arTextLine.end(); ++iter)
        {
            (*iter)->SortConts();
            (*iter)->Analyze();
            (*iter)->CalculateWidth();
        }

        CTextLine *pCurrLine, *pNextLine, *pPrevLine;
        double dCurrRight = 0, dNextRight = 0;
        double dCurrBeforeSpacing = 0, dNextBeforeSpacing = 0, dPrevBeforeSpacing = 0;
        double dBeforeSpacingWithShapes = 0;
        double dPreviousStringOffset = 0;

        for (size_t nIndex = 0; nIndex < m_arTextLine.size(); ++nIndex)
        {
            size_t nNextIndex = nIndex+1;
            CParagraph* pParagraph = nullptr;

            pCurrLine = new CTextLine(*m_arTextLine[nIndex]);
            nNextIndex == m_arTextLine.size() ?
                    pNextLine = nullptr :
                    pNextLine = m_arTextLine[nNextIndex];

            dCurrRight = m_dWidth - (pCurrLine->m_dX + pCurrLine->m_dWidth + pCurrLine->m_arConts.back()->m_dSpaceWidthMM); //добавляем ширину пробела
            if (pNextLine)
                dNextRight = m_dWidth - (pNextLine->m_dX + pNextLine->m_dWidth + pNextLine->m_arConts.back()->m_dSpaceWidthMM);

            dPrevBeforeSpacing = dCurrBeforeSpacing;
            dCurrBeforeSpacing = pCurrLine->m_dBaselinePos - dPreviousStringOffset - pCurrLine->m_dHeight - pCurrLine->m_dBaselineOffset;
            dPreviousStringOffset = pCurrLine->m_dBaselinePos - pCurrLine->m_dBaselineOffset;


            /*if (pNextLine &&
                fabs(pCurrLine->m_dBaselinePos - pNextLine->m_dBaselinePos - pNextLine->m_dHeight) < c_dSTANDART_STRING_HEIGHT_MM &&
                //fabs(pCurrLine->m_dBaselinePos - pNextLine->m_dBaselinePos) < c_dTHE_SAME_STRING_Y_PRECISION_MM && //если примерно на одной строке
                fabs(pCurrLine->m_dX + pCurrLine->m_dWidth - pNextLine->m_dX) > c_dTHE_STRING_X_PRECISION_MM)  //но расстрояние между словами нестандартное в этом месте
            {
                //будет шейпом
                CTextLine* pTextLine = new CTextLine(*m_arTextLine[index]);

                CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
                pParagraph->m_pManagerLight = &m_oManagerLight;
                pParagraph->m_eTextConversionType = CParagraph::TextToShape;
                pParagraph->m_arLines.push_back(pTextLine);

                CShape* pShape = new CShape();
                pShape->m_arParagraphs.push_back(pParagraph);
                pShape->m_dLeft	= pTextLine->m_dX - CShape::POSITION_CORRECTION_FOR_X_MM;
                pShape->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pTextLine->m_dBaselineOffset - CShape::POSITION_CORRECTION_FOR_Y_MM;
                pShape->m_dWidth = pTextLine->m_dWidth + CShape::SIZE_CORRECTION_FOR_X_MM;
                pShape->m_dHeight = pTextLine->m_dHeight + CShape::SIZE_CORRECTION_FOR_Y_MM;
                pShape->m_pManagerLight = &m_oManagerLight;

                m_arGraphicItems.push_back(pShape);
            }
            else*/ if (pNextLine && //это должна быть не последняя строка
                    fabs(pCurrLine->m_dHeight - pNextLine->m_dHeight) <= c_dTHE_SAME_STRING_Y_PRECISION_MM && //высота строк должна быть примерно одинаковой
                    //(pCurrLine->m_eTextAlignmentType == CParagraph::TextAlignmentType_ByLeftEdge || //todo - есть проблема если весь текст выровнен по центру
                     //pCurrLine->m_eTextAlignmentType == CParagraph::TextAlignmentType_ByWidth) &&
                    ((pCurrLine->m_dX > pNextLine->m_dX && //есть отступ
                      (fabs(dCurrRight - dNextRight) <= c_dTHE_STRING_X_PRECISION_MM || dCurrRight < dNextRight)) || //а следующая строка либо короче, либо такая же
                    ((pCurrLine->m_dX == pNextLine->m_dX && //нет отступа
                      (fabs(dCurrRight - dNextRight) <= c_dTHE_STRING_X_PRECISION_MM || dCurrRight < dNextRight))) //а следующая строка либо короче, либо такая же
            ))
            {
                nIndex++;

                //наверное это сплошной текст
                pParagraph = new CParagraph(m_eTextAssociationType);
                pParagraph->m_pManagerLight = &m_oManagerLight;
                pParagraph->m_eTextConversionType = CParagraph::TextToParagraph;

                //делаем абзац в сплошном тексте
                pParagraph->m_bIsNeedFirstLineIndent = pCurrLine->m_dX > pNextLine->m_dX ? true : false;

                pParagraph->m_dFirstLine = pCurrLine->m_dX - pNextLine->m_dX;
                pParagraph->m_dRight = std::min(dCurrRight, dNextRight);
                pParagraph->m_dLeft = std::min(pCurrLine->m_dX, pNextLine->m_dX);
                pParagraph->m_dWidth = std::max(pCurrLine->m_dWidth + pCurrLine->m_arConts.back()->m_dSpaceWidthMM,
                                                pNextLine->m_dWidth + pNextLine->m_arConts.back()->m_dSpaceWidthMM);

                pParagraph->m_dSpaceBefore = dBeforeSpacingWithShapes + std::max(dCurrBeforeSpacing, 0.0);;
                dBeforeSpacingWithShapes = 0;

                pParagraph->m_dBaselinePos = pCurrLine->m_dBaselinePos;
                /*if (fabs(dCurrRight - dNextRight) <= c_dTHE_STRING_X_PRECISION_MM) //предположение
                {
                    pParagraph->m_eTextAlignmentType = CParagraph::TextAlignmentType_ByWidth;
                }*/

                //Объединим 2 строчки в параграф
                pParagraph->m_arLines.push_back(pCurrLine);
                pParagraph->m_nNumLines++;
                pParagraph->m_arLines.back()->AddSpaceToEnd();
                //todo записать все строки из одного параграфа в одну строчку
                //pParagraph->m_arLines.back()->m_arConts.back()->m_oText += pNextLine->m_arConts.back()->m_oText;
                pParagraph->m_arLines.push_back(new CTextLine(*pNextLine));
                pParagraph->m_nNumLines++;

                pPrevLine = pCurrLine;
                pCurrLine = pNextLine;
                ++nNextIndex < m_arTextLine.size()-1?
                    pNextLine = m_arTextLine[nNextIndex] :
                    pNextLine = nullptr;

                dPrevBeforeSpacing = dCurrBeforeSpacing;
                dCurrBeforeSpacing = pCurrLine->m_dBaselinePos - dPreviousStringOffset - pCurrLine->m_dHeight - pCurrLine->m_dBaselineOffset;
                dPreviousStringOffset = pCurrLine->m_dBaselinePos - pCurrLine->m_dBaselineOffset;
                dCurrRight = dNextRight;

                if (pNextLine)
                {
                    dNextBeforeSpacing = pNextLine->m_dBaselinePos - dPreviousStringOffset - pNextLine->m_dHeight - pNextLine->m_dBaselineOffset;
                    dNextRight = m_dWidth - (pNextLine->m_dX + pNextLine->m_dWidth + pNextLine->m_arConts.back()->m_dSpaceWidthMM);
                }

                //междустрочный интервал во всем параграфе
                /*pParagraph->m_dHeight = pCurrLine->m_dHeight;
                if (dCurrBeforeSpacing < 0)
                {
                    pParagraph->m_dHeight += dCurrBeforeSpacing;
                }*/
                pParagraph->m_dHeight = pCurrLine->m_dHeight + std::max(dCurrBeforeSpacing, 0.0);

                //проверим, подходят ли следующие строчки для текущего pParagraph
                while(pNextLine &&
                      fabs(pCurrLine->m_dHeight - pNextLine->m_dHeight) < c_dTHE_SAME_STRING_Y_PRECISION_MM && //высота строк должна быть примерно одинаковой
                      fabs(dCurrBeforeSpacing - dNextBeforeSpacing) < c_dTHE_SAME_STRING_Y_PRECISION_MM && //расстрояние между строк тоже одинаково
                      //(pCurrLine->m_eTextAlignmentType == pNextLine->m_eTextAlignmentType ||
                       //pNextLine->m_eTextAlignmentType == CParagraph::TextAlignmentType_ByLeftEdge) &&
                      ((fabs(pCurrLine->m_dX - pNextLine->m_dX) <= c_dTHE_STRING_X_PRECISION_MM && //у последующих строк нет отступа относительно предыдущей непервой строки
                        (fabs(dCurrRight - dNextRight) <= c_dTHE_STRING_X_PRECISION_MM || dCurrRight < dNextRight))) //а следующая строка либо короче, либо такая же
                      )
                {
                    nIndex++;

                    pPrevLine = pCurrLine;
                    pCurrLine = pNextLine;
                    ++nNextIndex < m_arTextLine.size()-1?
                        pNextLine = m_arTextLine[nNextIndex] :
                        pNextLine = nullptr;

                    dPrevBeforeSpacing = dCurrBeforeSpacing;
                    dCurrBeforeSpacing = pCurrLine->m_dBaselinePos - dPreviousStringOffset - pCurrLine->m_dHeight - pCurrLine->m_dBaselineOffset;
                    dPreviousStringOffset = pCurrLine->m_dBaselinePos - pCurrLine->m_dBaselineOffset;
                    dCurrRight = dNextRight;

                    if (pNextLine)
                    {
                        dNextBeforeSpacing = pNextLine->m_dBaselinePos - dPreviousStringOffset - pNextLine->m_dHeight - pNextLine->m_dBaselineOffset;
                        dNextRight = m_dWidth - (pNextLine->m_dX + pNextLine->m_dWidth + pNextLine->m_arConts.back()->m_dSpaceWidthMM);
                    }

                    //Объединим 2 параграфа-строчки
                    pParagraph->m_arLines.back()->AddSpaceToEnd();
                    //pParagraph->m_arLines.back()->m_arConts.back()->m_oText += pNextLine->m_arConts.back()->m_oText;
                    pParagraph->m_arLines.push_back(new CTextLine(*pCurrLine));
                    pParagraph->m_nNumLines++;

                    pParagraph->m_dRight = std::min(pParagraph->m_dRight, dCurrRight);
                    pParagraph->m_dLeft = std::min(pParagraph->m_dLeft, pCurrLine->m_dX);
                    pParagraph->m_dWidth = std::max(pParagraph->m_dWidth, pCurrLine->m_dWidth + pCurrLine->m_arConts.back()->m_dSpaceWidthMM);
                }

                m_arParagraphs.push_back(pParagraph);
            }
            else
            {
                //будет шейпом для теста
                /*dBeforeSpacingWithShapes += dCurrBeforeSpacing + pCurrLine->m_dHeight + pCurrLine->m_dBaselineOffset;

                CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
                pParagraph->m_pManagerLight = &m_oManagerLight;
                pParagraph->m_eTextConversionType = CParagraph::TextToShape;
                pParagraph->m_arLines.push_back(pCurrLine);

                CShape* pShape = new CShape();
                pShape->m_arParagraphs.push_back(pParagraph);
                pShape->m_dLeft	= pCurrLine->m_dX - CShape::POSITION_CORRECTION_FOR_X_MM;
                pShape->m_dTop	= pCurrLine->m_dBaselinePos - pCurrLine->m_dHeight - pCurrLine->m_dBaselineOffset - CShape::POSITION_CORRECTION_FOR_Y_MM;
                pShape->m_dWidth = pCurrLine->m_dWidth + CShape::SIZE_CORRECTION_FOR_X_MM;
                pShape->m_dHeight = pCurrLine->m_dHeight + CShape::SIZE_CORRECTION_FOR_Y_MM;
                pShape->m_pManagerLight = &m_oManagerLight;

                m_arGraphicItems.push_back(pShape);*/

                //будет отдельной параграфом-строчкой
                pParagraph = new CParagraph(m_eTextAssociationType);
                pParagraph->m_pManagerLight = &m_oManagerLight;
                pParagraph->m_eTextConversionType = CParagraph::TextToParagraph;

                pParagraph->m_dLeft	= pCurrLine->m_dX;
                pParagraph->m_dTop	= pCurrLine->m_dBaselinePos - pCurrLine->m_dHeight - pCurrLine->m_dBaselineOffset;

                pParagraph->m_dFirstLine = 0;
                pParagraph->m_dRight = dCurrRight;
                pParagraph->m_dWidth = pCurrLine->m_dWidth + pCurrLine->m_arConts.back()->m_dSpaceWidthMM;
                pParagraph->m_dHeight = pCurrLine->m_dHeight;
                if (dCurrBeforeSpacing < 0)
                {
                    pParagraph->m_dHeight += dCurrBeforeSpacing;
                }

                pParagraph->m_dSpaceBefore = std::max(dCurrBeforeSpacing, 0.0);
                pParagraph->m_dBaselinePos = pCurrLine->m_dBaselinePos;

                pParagraph->m_arLines.push_back(pCurrLine);
                pParagraph->m_nNumLines++;

                m_arParagraphs.push_back(pParagraph);
            }
        }

        //коррекция всей страницы - возможно не понадобится
        /*for (std::vector<CParagraph*>::iterator iter = m_arParagraphs.begin();
             iter < m_arParagraphs.end(); ++iter)
        {
             if ((*iter)->m_eTextAlignmentType == CParagraph::TextAlignmentType_ByWidth)
             {
                 (*iter)->m_dRight = corrRight;
                 (*iter)->m_dHeight = corrHeight;
             }
        }*/
    }

    void CPage::Merge(double dAffinity)
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

    void CPage::Write(NSStringUtils::CStringBuilder& oWriter)
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

        for (size_t i = 0; i < m_arParagraphs.size(); ++i)
        {
            m_arParagraphs[i]->ToXml(oWriter);
        }


    }

    void CPage::WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter)
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
}
