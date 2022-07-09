#include "Page.h"
#include "../resources/ColorTable.h"
#include "../resources/Constants.h"
#include "../resources/SingletonTemplate.h"
#include "../resources/SortElements.h"
#include "../resources/utils.h"

namespace NSDocxRenderer
{
    CPage::CPage(NSFonts::IApplicationFonts* pFonts) : m_oManager(pFonts), m_oManagerLight(pFonts)
    {
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

        m_pCurrentLine = nullptr;

        m_dLastTextX = -1;
        m_dLastTextY = -1;
        m_dLastTextX_block = m_dLastTextX;
    }

    void CPage::Clear()
    {
        ClearImages();
        ClearTextData();
        ClearTextLines();
        ClearShapes();
        ClearParagraphs();

        m_pCurrentLine = nullptr;

        m_dLastTextX = -1;
        m_dLastTextY = -1;
        m_dLastTextX_block = m_dLastTextX;
    }

    void CPage::ClearImages()
    {
        for (auto pImage: m_arImages)
        {
            pImage->Clear();
        }
        m_arImages.clear();
    }

    void CPage::ClearTextData()
    {
        for (auto pCont : m_arTextData)
        {
            RELEASEOBJECT(pCont);
        }
        m_arTextData.clear();
    }

    void CPage::ClearTextLines()
    {
        for (auto pLine : m_arTextLine)
        {
            pLine->Clear();
        }
        m_arTextLine.clear();
    }

    void CPage::ClearShapes()
    {
        for (auto pShape : m_arShapes)
        {
            pShape->Clear();
        }
        m_arShapes.clear();
    }

    void CPage::ClearParagraphs()
    {
        for (auto pParagraph : m_arParagraphs)
        {
            pParagraph->Clear();
        }
        m_arParagraphs.clear();
    }

    CPage::~CPage()
    {
        Clear();
    }

    void CPage::SetCurrentLineByBaseline(const double& dBaseLinePos)
    {
        if ((nullptr == m_pCurrentLine) || (tatBlockChar == m_eTextAssociationType))
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
        for (size_t i = 0; i < m_arTextLine.size(); ++i)
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

        m_arImages.push_back(pImage);
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
    }

    void CPage::Close()
    {
        m_oVector.Close();
    }

    void CPage::DrawPath(LONG lType, LONG lTxId)
    {
        if ((m_oVector.m_dLeft <= m_oVector.m_dRight) && (m_oVector.m_dTop <= m_oVector.m_dBottom))
        {
            //todo зачем это добавляется?
            if (m_pBrush->Color1 == c_iWhiteColor &&
                m_oVector.m_dLeft < c_dGRAPHICS_ERROR_MM &&
                m_oVector.m_dTop < c_dGRAPHICS_ERROR_MM &&
                fabs(m_oVector.m_dRight - m_dWidth) < c_dGRAPHICS_ERROR_MM &&
                fabs(m_oVector.m_dBottom - m_dHeight) < c_dGRAPHICS_ERROR_MM)
            {
                return;
            }

            CShape* pShape = new CShape();

            pShape->m_lTxId		= lTxId;

            pShape->m_oPen		= *m_pPen;
            pShape->m_oBrush	= *m_pBrush;

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

            pShape->GetDataFromVector(m_oVector, lType, 100000);

            m_arShapes.push_back(pShape);
        }
    }

    void CPage::CollectTextData(const PUINT pUnicodes, const PUINT pGids, const UINT& nCount,
                                const double& fX, const double& fY, const double& fWidth, const double& fHeight,
                                const double& fBaseLineOffset, const bool& bIsPDFAnalyzer)
    {
        if (pUnicodes != nullptr && nCount == 1 && IsSpaceUtf32(*pUnicodes))
        {
            //note пробелы не нужны, добавляются при анализе
            return;
        }

        double dTextX = fX;
        double dTextY = fY;
        double dTextR = fX + fWidth;
        double dTextB = fY + fHeight;

        m_pTransform->TransformPoint(dTextX, dTextY);
        m_pTransform->TransformPoint(dTextR, dTextB);

        double dTextW = dTextR - dTextX;
        double dTextH = dTextB - dTextY;

        NSStringUtils::CStringUTF32 oText((uint32_t*)pUnicodes, nCount);

        if ((pUnicodes != nullptr) && (pGids != nullptr))
        {
            for (unsigned int i = 0; i < nCount; ++i)
            {
                if ( !IsUnicodeSymbol( pUnicodes[i] ) )
                {
                    oText[i] = ' ';
                }
            }
        }

        bool bIsPath = ((nullptr == pGids) && !bIsPDFAnalyzer) ? false : true;

        m_oManager.LoadFont(0, !bIsPath);

        if (bIsPath)
            m_oManager.GenerateFontName2(oText);

        if (fabs(dTextW) < 0.01 || (dTextW > 10))
        {
            double _x = 0;
            double _y = 0;
            double _w = 0;
            double _h = 0;

            if (nullptr != pGids)
            {
                m_oManager.SetStringGid(1);
                m_oManager.MeasureStringGids(pGids, nCount, dTextX, dTextY, _x, _y, _w, _h, CFontManager::mtPosition);
            }
            else
            {
                // такого быть не должно (только из xps)
                m_oManager.SetStringGid(0);
                m_oManager.MeasureStringGids(pUnicodes, nCount, dTextX, dTextY, _x, _y, _w, _h, CFontManager::mtPosition);
            }

            dTextW = _w;
        }

        double dBaseLinePos = dTextY + fBaseLineOffset;
        dTextH = m_oManager.GetFontHeight();

        CContText* pCont = new CContText(m_oManagerLight);

        pCont->m_dLeft = dTextX;
        pCont->m_dBaselinePos = dBaseLinePos;
        pCont->m_dBaselineOffset = m_oManager.m_oFont.m_dBaselineOffset;
        pCont->m_dLastX = dTextX;

        pCont->m_dTop       = dBaseLinePos - dTextH - m_oManager.m_oFont.m_dBaselineOffset;
        pCont->m_dWidth		= dTextW;
        pCont->m_dHeight	= dTextH;

        pCont->m_oText = oText;

        pCont->m_oFont		= m_oManager.m_oFont.m_oFont;
        pCont->m_oBrush		= *m_pBrush;

        if (bIsPath)
        {
            pCont->m_strPickFontName	= m_oManager.m_strCurrentPickFont;
            pCont->m_lPickFontStyle		= m_oManager.m_lCurrentPictFontStyle;
        }

        pCont->m_dSpaceWidthMM = m_oManager.m_dSpaceWidthMM;

        m_arTextData.push_back(pCont);
    }

    void CPage::AnalyzeCollectedShapes()
    {
        //todo Объединить контур и заливку одного рисунка в шейпе если m_strPath одинаковые
        //todo Объединить выделения соседних строк

        DetermineContWithMaxSizeFont();
        DetermineLinesType();

        //Добавить различные условия объединений
        /*bool bIf1 = false;

        if (bIf1)
        {
            CShape* pNewShape = new CShape(*pCurrShape, *pNextShape);
        pCurrShape->m_bIsNotNecessaryToUse = true;
        pNextShape->m_bIsNotNecessaryToUse = true;
        m_arShapes.push_back(pNewShape);
        nShapesCount++;
        }*/
    }

    void CPage::DetermineContWithMaxSizeFont()
    {
        for (size_t i = 0; i < m_arShapes.size(); ++i)
        {
            if (m_arShapes[i]->m_bIsNotNecessaryToUse)
            {
                continue;
            }

            //Нужно собрать всю графику, которая находится на одной линии
            std::vector<CShape*> arCurrShapes;
            arCurrShapes.push_back(m_arShapes[i]);

            for (size_t j = i+1; j < m_arShapes.size(); ++j)
            {
                if (!m_arShapes[j]->m_bIsNotNecessaryToUse &&
                    m_arShapes[i]->IsCorrelated(m_arShapes[j]) &&
                    std::abs(m_arShapes[i]->m_dHeight - m_arShapes[j]->m_dHeight) < c_GRAPHICS_ERROR_IN_LINES_MM && //линия должна быть одного размера по высоте
                    std::abs(m_arShapes[i]->m_dTop - m_arShapes[j]->m_dTop) < c_GRAPHICS_ERROR_IN_LINES_MM * 5) //все должно быть на одной линии
                {
                    arCurrShapes.push_back(m_arShapes[j]);
                }
            }


        }
    }

    void CPage::DetermineLinesType()
    {
        for (size_t i = 0; i < m_arShapes.size(); ++i)
        {
            if (m_arShapes[i]->m_bIsNotNecessaryToUse)
            {
                continue;
            }

            //Нужно собрать всю графику, которая находится на одной линии
            std::vector<CShape*> arCurrShapes;
            arCurrShapes.push_back(m_arShapes[i]);

            for (size_t j = i+1; j < m_arShapes.size(); ++j)
            {
                if (!m_arShapes[j]->m_bIsNotNecessaryToUse &&
                    m_arShapes[i]->IsCorrelated(m_arShapes[j]) &&
                    std::abs(m_arShapes[i]->m_dHeight - m_arShapes[j]->m_dHeight) < c_GRAPHICS_ERROR_IN_LINES_MM && //линия должна быть одного размера по высоте
                    std::abs(m_arShapes[i]->m_dTop - m_arShapes[j]->m_dTop) < c_GRAPHICS_ERROR_IN_LINES_MM * 5) //все должно быть на одной линии
                {
                    arCurrShapes.push_back(m_arShapes[j]);
                }
            }

            CShape* pCurrShape;
            //Отсортируем собранный массив по x
            size_t nCurrShapesCount = arCurrShapes.size();
            if (nCurrShapesCount > 1)
            {
                SortElements(arCurrShapes);
                pCurrShape = arCurrShapes[0];

                //сравнение
                for (size_t k = 1; k < nCurrShapesCount; ++k)
                {
                    CShape* pNextShape = arCurrShapes[k];

                    //note логика работатет только если arCurrShapes отсортирован по m_dLeft
                    pCurrShape->DetermineLineType(pNextShape);

                    if (pCurrShape->m_bIsNotNecessaryToUse)
                    {
                        pCurrShape = pNextShape;
                        k++;
                    }
                }
            }
            else if (nCurrShapesCount == 1)
            {
                arCurrShapes[0]->DetermineLineType();
            }

            arCurrShapes.clear();
        }
    }

    void CPage::AnalyzeCollectedData()
    {
        for (size_t i = 0; i < m_arShapes.size(); ++i)
        {
            CShape* pShape = m_arShapes[i];

            if (pShape->m_bIsNotNecessaryToUse ||
                (pShape->m_eGraphicsType != eGraphicsType::gtRectangle &&
                pShape->m_eGraphicsType != eGraphicsType::gtCurve))
            {
                continue;
            }

            for (size_t j = 0; j < m_arTextData.size(); ++j)
            {
                CContText* pCont = m_arTextData[j];

                if (IsLineCrossingText(pShape, pCont) ||
                    IsLineBelowText(pShape, pCont) ||
                    IsItHighlightingBackground(pShape, pCont))
                {
                    pShape->m_bIsNotNecessaryToUse = true;
                }
            }
        }
    }

    bool CPage::IsLineCrossingText(const CShape* pShape, CContText* pContText)
    {
        double dTopShape = pShape->m_dTop;
        double dTopContText = pContText->m_dTop;
        double dBottomShape = pShape->m_dTop + pShape->m_dHeight;
        double dBottomContText = pContText->m_dBaselinePos;

        double dRightShape = pShape->m_dLeft + pShape->m_dWidth;
        double dRightContText = pContText->m_dLeft + pContText->m_dWidth;

        //Условие пересечения по вертикали
        bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle;
        //Условие пересечения по вертикали
        bool bIf2 = dTopShape > dTopContText && dBottomShape < dBottomContText;
        //Условие пересечения по горизонтали
        bool bIf3 = fabs(pShape->m_dLeft - pContText->m_dLeft) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                fabs(dRightShape - dRightContText) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                (pShape->m_dLeft < pContText->m_dLeft && dRightShape > dRightContText);
        //Условие для размеров по высоте
        bool bIf4 = pShape->m_dHeight < pContText->m_dHeight &&
                pContText->m_dHeight - pShape->m_dHeight > c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM;

        if (bIf1 && bIf2 && bIf3 && bIf4)
        {
            pContText->m_oFont.Strikeout = TRUE;
            if (pShape->m_eLineType == eLineType::ltDouble)
            {
                pContText->m_bIsDoubleStrikeout = true;
            }
            return true;
        }

        return false;
    }

    bool CPage::IsLineBelowText(const CShape* pShape, CContText* pContText)
    {
        double dTopShape = pShape->m_dTop;
        double dBottomContText = pContText->m_dBaselinePos;

        double dRightShape = pShape->m_dLeft + pShape->m_dWidth;
        double dRightContText = pContText->m_dLeft + pContText->m_dWidth;

        bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle || pShape->m_eGraphicsType == eGraphicsType::gtCurve;
        //Условие по вертикали
        bool bIf2 = fabs(dTopShape - dBottomContText) < pContText->m_dHeight * 0.2;
        //Условие пересечения по горизонтали
        bool bIf3 = fabs(pShape->m_dLeft - pContText->m_dLeft) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                fabs(dRightShape - dRightContText) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                (pShape->m_dLeft < pContText->m_dLeft && dRightShape > dRightContText);
        //Условие для размеров по высоте
        bool bIf4 = pShape->m_dHeight < pContText->m_dHeight &&
                pContText->m_dHeight - pShape->m_dHeight > c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM;

        if (bIf1 && bIf2 && bIf3 && bIf4)
        {
            pContText->m_oFont.Underline = TRUE;
            pContText->m_eUnderlineType  = pShape->m_eLineType;
            if (pShape->m_oPen.Color != c_iBlackColor)
            {
                pContText->m_lUnderlineColor = pShape->m_oPen.Color;
            }
            else
            {
                pContText->m_lUnderlineColor = pShape->m_oBrush.Color1;
            }
            return true;
        }

        return false;
    }

    bool CPage::IsItHighlightingBackground(const CShape* pShape, CContText* pContText)
    {
        double dTopShape = pShape->m_dTop;
        double dBottomShape = pShape->m_dTop + pShape->m_dHeight;
        double dBottomContText = pContText->m_dBaselinePos - pContText->m_dBaselineOffset;
        double dSomeBaseLine1 = dBottomContText - pContText->m_dHeight * 0.75;
        double dSomeBaseLine2 = dBottomContText - pContText->m_dHeight * 0.5;
        double dSomeBaseLine3 = dBottomContText - pContText->m_dHeight * 0.25;

        double dRightShape = pShape->m_dLeft + pShape->m_dWidth;
        double dRightContText = pContText->m_dLeft + pContText->m_dWidth;

        bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle;
        //Условие пересечения по вертикали
        bool bIf2 = (dSomeBaseLine1 > dTopShape && dSomeBaseLine1 < dBottomShape &&
                     dSomeBaseLine2 > dTopShape && dSomeBaseLine2 < dBottomShape &&
                     dSomeBaseLine3 > dTopShape && dSomeBaseLine3 < dBottomShape);
        //Условие пересечения по горизонтали
        bool bIf3 = fabs(pShape->m_dLeft - pContText->m_dLeft) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                fabs(dRightShape - dRightContText) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                (pShape->m_dLeft < pContText->m_dLeft && dRightShape > dRightContText);
        //Цвета должны быть разными
        bool bIf4 = pContText->m_oBrush.Color1 != pShape->m_oBrush.Color1 /*&&
                pContText->m_oBrush.Color1 != pShape->m_oPen.Color*/;

        if (bIf1 && bIf2 && bIf3 && bIf4)
        {
            //Удовлетворяет расположением и размером - привязываем указатель на картинку
            pContText->m_pShape = pShape;
            pContText->m_bIsHighlightPresent = true;
            pContText->m_lHighlightColor = pShape->m_oBrush.Color1;
            return true;
        }

        return false;
    }

    void CPage::BuildLines()
    {
        for (size_t i = 0; i < m_arTextData.size(); i++)
        {
            //note Элементы в m_arTextData в случайных местах страницы
            BuildLines(m_arTextData[i]);
        }

        if (m_eTextAssociationType == tatPlainParagraph ||
            m_eTextAssociationType == tatShapeLine ||
            m_eTextAssociationType == tatPlainLine)
        {
            DetermineDominantGraphics();
        }
    }

    void CPage::DetermineDominantGraphics()
    {
        const CShape* pDominantShape = nullptr;

        for (size_t i = 0; i < m_arTextLine.size(); i++)
        {
            for (size_t j = 0; j < m_arTextLine[i]->m_arConts.size(); j++)
            {
                if (m_arTextLine[i]->m_arConts[j]->m_pShape &&
                    m_arTextLine[i]->m_arConts[j]->m_pShape != pDominantShape)
                {
                    double dLeftShape = m_arTextLine[i]->m_arConts[j]->m_pShape->m_dLeft;
                    double dLeftCont = m_arTextLine[i]->m_arConts[j]->m_dLeft;
                    double dRightShape = dLeftShape + m_arTextLine[i]->m_arConts[j]->m_pShape->m_dWidth;
                    double dRightCont = dLeftCont + m_arTextLine[i]->m_arConts[j]->m_dWidth;

                    if (dLeftShape < dLeftCont && dRightShape > dRightCont)
                    {
                        if (!pDominantShape)
                        {
                            pDominantShape = m_arTextLine[i]->m_arConts[j]->m_pShape;
                        }
                        else
                        {
                            double dLeftDomShape = pDominantShape->m_dLeft;
                            double dRightDomShape = dLeftDomShape + pDominantShape->m_dWidth;
                            if (dLeftShape < dLeftDomShape && dRightShape > dRightDomShape)
                            {
                                pDominantShape = m_arTextLine[i]->m_arConts[j]->m_pShape;
                            }
                        }
                    }
                }
            }

            m_arTextLine[i]->m_pDominantShape = pDominantShape;
            pDominantShape = nullptr;
        }
    }

    void CPage::BuildLines(const CContText* pContText)
    {
        double dTextX       = pContText->m_dLeft;
        double dBaseLinePos = pContText->m_dBaselinePos;
        double dTextW       = pContText->m_dWidth;
        NSStringUtils::CStringUTF32 oText(pContText->m_oText);

        SetCurrentLineByBaseline(dBaseLinePos);

        CContText* pLastCont = nullptr;
        size_t nCountConts = m_pCurrentLine->m_arConts.size();
        if (nCountConts != 0)
            pLastCont = m_pCurrentLine->m_arConts.back();

        if (nullptr == pLastCont)
        {
            // первое слово в линии
            CContText* pCont = new CContText(*pContText);

            pCont->m_dLastX = dTextX;

            m_pCurrentLine->AddCont(pCont);
            m_dLastTextX = dTextX;
            m_dLastTextY = dBaseLinePos;
            m_dLastTextX_block = m_dLastTextX;
            return;
        }

        double dRight = pLastCont->m_dLeft + pLastCont->m_dWidth;

        if (pLastCont->IsEqual(pContText))
        {
            bool bIsConditionPassed = false;

            // настройки одинаковые. теперь смотрим, на расположение
            if (fabs(dRight - dTextX) < c_dTHE_STRING_X_PRECISION_MM)
            {
                // продолжаем слово
                pLastCont->m_oText += oText;
                pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dLeft);
                bIsConditionPassed = true;
            }
            else if ((dRight < dTextX) && ((dTextX - dRight) < pContText->m_dSpaceWidthMM))
            {
                // продолжаем слово с пробелом
                pLastCont->m_oText += uint32_t(' ');
                pLastCont->m_oText += oText;
                pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dLeft);
                bIsConditionPassed = true;
            }
            else if (fabs(dBaseLinePos - pLastCont->m_dBaselinePos) < 0.01 &&
                     fabs(m_dLastTextY - pLastCont->m_dBaselinePos) < 0.01 &&
                     fabs(m_dLastTextX - pLastCont->m_dLastX) < 0.01)
            {
                // идет текст подряд, но с расстояниями что-то не так. смотрим - если новый текст идет после предыдущего, но
                // просто левее чем предыдущий x + w - то считаем это нормальным. и дописываем слово. корректируя длину
                if (dTextX < dRight && dTextX > pLastCont->m_dLastX)
                {
                    // продолжаем слово
                    pLastCont->m_oText += oText;
                    double dNewW = (dTextX + dTextW - pLastCont->m_dLeft);
                    if (pLastCont->m_dWidth < dNewW)
                        pLastCont->m_dWidth = dNewW;
                    bIsConditionPassed = true;
                }
                // еще одна заглушка на большой пробел - добавляем пробел, потом в линии все разрулится через spacing
                else if (dTextX > dRight && (dTextX - dRight) < 5 && fabs(m_dLastTextX_block - m_dLastTextX) < 0.01)
                {
                    // продолжаем слово с пробелом
                    pLastCont->m_oText += uint32_t(' ');
                    pLastCont->m_oText += oText;
                    pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dLeft);
                    bIsConditionPassed = true;
                }
            }

            if (bIsConditionPassed)
            {
                m_dLastTextX = dTextX;
                m_dLastTextY = dBaseLinePos;
                m_dLastTextX_block = m_dLastTextX;
                pLastCont->m_dLastX = dTextX;
                return;
            }
        }

        // либо пробел большой между словами, либо новый текст левее, либо настройки не те (шрифт, кисть)
        // либо все вместе... просто добавл¤ем новое слово
        m_pCurrentLine->AddCont(new CContText(*pContText));
        m_dLastTextX = dTextX;
        m_dLastTextY = dBaseLinePos;
        m_dLastTextX_block = m_dLastTextX;
    }

    void CPage::BuildByType()
    {
        if (0 == m_arTextLine.size())
            return;

        switch (m_eTextAssociationType)
        {
        case tatBlockChar:
            BuildByTypeBlockChar();
            break;
        case tatBlockLine:
            BuildByTypeBlockLine();
            break;
        case tatPlainLine:
            BuildByTypePlainLine();
            break;
        case tatShapeLine:
            BuildByTypeShapeLine();
            break;
        case tatPlainParagraph:
            BuildByTypePlainParagraph();
            break;
        default:
            break;
        }
    }

    void CPage::BuildByTypeBlockChar()
    {
        for (auto pLine : m_arTextLine)
        {
            CTextLine* pTextLine = new CTextLine(*pLine);;

            CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
            pParagraph->m_eTextConversionType = CParagraph::tctTextToFrame;

            pParagraph->m_dLeft	= pTextLine->m_dLeft;
            pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pTextLine->m_dBaselineOffset;

            pParagraph->m_arLines.push_back(pTextLine);

            m_arParagraphs.push_back(pParagraph);
        }
    }

    void CPage::BuildByTypeBlockLine()
    {
        CTextLine* pFirstLine = new CTextLine(*m_arTextLine[0]);

        CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
        pParagraph->m_eTextConversionType = CParagraph::tctTextToFrame;

        pParagraph->m_dLeft	= pFirstLine->m_dLeft;
        pParagraph->m_dTop	= pFirstLine->m_dBaselinePos - pFirstLine->m_dHeight - pFirstLine->m_dBaselineOffset;
        double dCurrentTop = pParagraph->m_dTop;

        pParagraph->m_arLines.push_back(pFirstLine);

        m_arParagraphs.push_back(pParagraph);
        size_t nCount = m_arTextLine.size();
        for (size_t i = 1; i < nCount; ++i)
        {
            CTextLine* pTextLine = new CTextLine(*m_arTextLine[i]);

            CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
            pParagraph->m_eTextConversionType = CParagraph::tctTextToFrame;

            if (((fabs(pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pFirstLine->m_dBaselinePos) > c_dSTANDART_STRING_HEIGHT_MM) && (pTextLine->m_dLeft == pFirstLine->m_dLeft)) ||
                    ((pTextLine->m_dLeft != pFirstLine->m_dLeft) && (pTextLine->m_dBaselinePos != pFirstLine->m_dBaselinePos)))
            {
                pParagraph->m_dLeft	= pTextLine->m_dLeft;
                pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pTextLine->m_dBaselineOffset;
                dCurrentTop = pParagraph->m_dTop;
            }
            else
            {
                pParagraph->m_dLeft	= pFirstLine->m_dLeft;
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
                (*iter)->CalculateWidth();
            }
        }
        Merge(c_dSTANDART_STRING_HEIGHT_MM / 3);

        double previousStringOffset = 0;
        for (size_t i = 0; i < m_arTextLine.size(); ++i)
        {
            CTextLine* pTextLine = new CTextLine(*m_arTextLine[i]);

            double dBeforeSpacing = pTextLine->CalculateBeforeSpacing(&previousStringOffset);
            previousStringOffset = pTextLine->CalculateStringOffset();
            double dRight = pTextLine->CalculateRightBorder(m_dWidth);

            CreateSingleLineParagraph(pTextLine, &dRight, &dBeforeSpacing);
        }
    }

    void CPage::BuildByTypeShapeLine()
    {
        SortElements(m_arTextLine);
        for (std::vector<CTextLine*>::iterator iter = m_arTextLine.begin(); iter != m_arTextLine.end(); ++iter)
        {
            (*iter)->SortConts();
            (*iter)->CalculateWidth();
        }

        for (size_t i = 0; i < m_arTextLine.size(); ++i)
        {
            CTextLine* pTextLine = new CTextLine(*m_arTextLine[i]);
            CreateSingleLineShape(pTextLine);
        }
    }

    void CPage::BuildByTypePlainParagraph()
    {
        //todo смещение текста по высоте при использовании шейпов
        //todo не отображается перенос в линии
        //todo если в линии есть перенос нужно обеъдинить строки в один параграф
        //todo сноски не работают - формат сохраняется на всю строку - переделать в шейпы или в сноски )
        //todo шрифты явно получаются шире чем в оригинале(видно по подчеркиваниям и правым границам оригинала) - за счет этого увеличивается ширина строки - временное решение RightBorderCorrection();
        //todo RightBorderCorrection() - ввести для каждого размера шрифта свою поправку. Брать максимальную из возможных. Зависимость нелинейная.
        //todo выставить шейпы перед основным текстом, чтобы были доступны дря редактирования?

        //todo в зависимости от очередности загрузки файлов проявляется проблема со шрифтами - текст в некоторых конвертированных файлах становится жирным, зачеркнутым или курсив. С одним файлом проблем не наблюдалось

        //todo добавить различные типы текста для распознавания: обычный-сплошной, списки-содержание и тд

        SortElements(m_arTextLine);
        for (auto iter = m_arTextLine.begin(); iter != m_arTextLine.end(); ++iter)
        {
            (*iter)->SortConts();
            (*iter)->CalculateWidth();
            (*iter)->DetermineAssumedTextAlignmentType(m_dWidth);
        }

        CTextLine *pCurrLine, *pNextLine, *pPrevLine;
        double dCurrRight = 0, dNextRight = 0;
        double dCurrBeforeSpacing = 0, dNextBeforeSpacing = 0, dPrevBeforeSpacing = 0;
        double dBeforeSpacingWithShapes = 0;
        double dPreviousStringOffset = 0;
        double dCorrectionBeforeSpacing = 0;

        for (size_t nIndex = 0; nIndex < m_arTextLine.size(); ++nIndex)
        {
            size_t nNextIndex = nIndex+1;
            CParagraph* pParagraph = nullptr;

            pCurrLine = new CTextLine(*m_arTextLine[nIndex]);
            nNextIndex == m_arTextLine.size() ?
                    pNextLine = nullptr :
                    pNextLine = m_arTextLine[nNextIndex];

            nIndex == 0 ?
                    pPrevLine = nullptr :
                    pPrevLine = m_arTextLine[nIndex-1];

            dCurrRight = pCurrLine->CalculateRightBorder(m_dWidth);
            if (pNextLine)
                dNextRight = pNextLine->CalculateRightBorder(m_dWidth);

            dPrevBeforeSpacing = dCurrBeforeSpacing;
            dCurrBeforeSpacing = pCurrLine->CalculateBeforeSpacing(&dPreviousStringOffset);
            dPreviousStringOffset = pCurrLine->CalculateStringOffset();

            /*if (pNextLine && pCurrLine->AreLinesCrossing(pNextLine))
            {
                //Пусть обе строки пока будут шейпами
                CreateSingleLineShape(pCurrLine);
                pNextLine = new CTextLine(*pNextLine);
                CreateSingleLineShape(pNextLine);

                double dCurrentAdditive = pCurrLine->m_dHeight + std::max(dCurrBeforeSpacing, 0.0);

                dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(&dPreviousStringOffset);

                dCurrentAdditive -= pNextLine->m_dHeight + dNextBeforeSpacing;
                dBeforeSpacingWithShapes += dCurrentAdditive;

                //СЃРјРµС‰Р°РµРј СЂР°Р±РѕС‡СѓСЋ С‚РѕС‡РєСѓ РЅР° СЃР»РµРґСѓСЋС‰СѓСЋ Р»РёРЅРёСЋ
                nIndex++;
                dPrevBeforeSpacing = dCurrBeforeSpacing;
                dCurrBeforeSpacing = dNextBeforeSpacing;
                //dPreviousStringOffset = pNextLine->CalculateStringOffset();*/


                //Или определим какая из строк будет шейпом
                /*if (pCurrLine->m_dWidth >= pNextLine->m_dWidth)
                {
                    pNextLine = new CTextLine(*pNextLine);

                    CreateSingleLineParagraph(pCurrLine, &dCurrRight, &dCurrBeforeSpacing);

                    dPrevBeforeSpacing = dCurrBeforeSpacing;
                    dCurrBeforeSpacing = pNextLine->m_dBaselinePos - dPreviousStringOffset - pNextLine->m_dHeight - pNextLine->m_dBaselineOffset;
                    dPreviousStringOffset = pNextLine->m_dBaselinePos - pNextLine->m_dBaselineOffset;

                    CreateSingleLineShape(pNextLine);
                    dBeforeSpacingWithShapes += pNextLine->m_dHeight + std::max(dCurrBeforeSpacing, 0.0);

                    nIndex++;
                }
                else
                {
                    CreateSingleLineShape(pCurrLine);
                    dBeforeSpacingWithShapes += pCurrLine->m_dHeight + std::max(dCurrBeforeSpacing, 0.0);

                    //dCurrBeforeSpacing = pNextLine->m_dBaselinePos - dPreviousStringOffset - pNextLine->m_dHeight - pNextLine->m_dBaselineOffset;
                    //dCurrRight = m_dWidth - (pNextLine->m_dX + pNextLine->m_dWidth + pNextLine->m_arConts.back()->m_dSpaceWidthMM);

                    //CreateSingleLineParagraph(pNextLine, &dCurrRight, &dCurrBeforeSpacing);
                }*/
            /*}
            else */if (pNextLine && //это должна быть не последняя строка
                    fabs(pCurrLine->m_dHeight - pNextLine->m_dHeight) <= c_dTHE_SAME_STRING_Y_PRECISION_MM && //высота строк должна быть примерно одинаковой
                    pCurrLine->AreAlignmentsAppropriate(pNextLine) &&
                    ((pCurrLine->m_dLeft > pNextLine->m_dLeft && //есть отступ
                      (fabs(dCurrRight - dNextRight) <= c_dERROR_OF_RIGHT_BORDERS_MM || dCurrRight < dNextRight)) || //а следующая строка либо короче, либо такая же
                    ((pCurrLine->m_dLeft == pNextLine->m_dLeft && //нет отступа
                      (fabs(dCurrRight - dNextRight) <= c_dERROR_OF_RIGHT_BORDERS_MM || dCurrRight < dNextRight))) //а следующая строка либо короче, либо такая же
            ))
            {
                nIndex++;

                //наверное это сплошной текст
                pParagraph = new CParagraph(m_eTextAssociationType);
                pParagraph->m_eTextConversionType = CParagraph::tctTextToParagraph;

                //делаем абзац в сплошном тексте
                pParagraph->m_bIsNeedFirstLineIndent = pCurrLine->m_dLeft > pNextLine->m_dLeft ? true : false;

                pParagraph->m_dFirstLine = pCurrLine->m_dLeft - pNextLine->m_dLeft;
                pParagraph->m_dRight = std::min(dCurrRight, dNextRight);
                pParagraph->m_dLeft = std::min(pCurrLine->m_dLeft, pNextLine->m_dLeft);
                pParagraph->m_dWidth = std::max(pCurrLine->m_dWidth + pCurrLine->m_arConts.back()->m_dSpaceWidthMM,
                                                pNextLine->m_dWidth + pNextLine->m_arConts.back()->m_dSpaceWidthMM);
                pParagraph->m_dTop	= pCurrLine->m_dBaselinePos - pCurrLine->m_dHeight - pCurrLine->m_dBaselineOffset;
                pParagraph->m_dBaselinePos = pCurrLine->m_dBaselinePos;

                if (fabs(dCurrRight - dNextRight) <= c_dERROR_OF_RIGHT_BORDERS_MM) //предположение
                {
                    pParagraph->m_eTextAlignmentType = CParagraph::tatByWidth;
                }

                //размер строк во всем параграфе
                pParagraph->m_dHeight = pCurrLine->m_dHeight;
                pParagraph->m_dSpaceBefore = std::max(dCurrBeforeSpacing, 0.0);

                //Объединим 2 строчки в параграф
                pParagraph->m_arLines.push_back(pCurrLine);
                pParagraph->m_nNumLines++;
                pParagraph->m_arLines.back()->AddSpaceToEnd();
                //todo записать все строки из одного параграфа в одну строчку
                //pParagraph->m_arLines.back()->m_arConts.back()->m_oText += pNextLine->m_arConts.back()->m_oText;
                pParagraph->m_arLines.push_back(new CTextLine(*pNextLine));
                pParagraph->m_nNumLines++;

                if (IsShadingPresent(pCurrLine, pNextLine))
                {
                    pParagraph->m_bIsShadingPresent = true;
                    pParagraph->m_lColorOfShadingFill = pCurrLine->m_pDominantShape->m_oBrush.Color1;
                }

                //сдвигаем рабочую точку
                pPrevLine = pCurrLine;
                pCurrLine = pNextLine;
                ++nNextIndex < m_arTextLine.size()?
                    pNextLine = m_arTextLine[nNextIndex] :
                    pNextLine = nullptr;

                dPrevBeforeSpacing = dCurrBeforeSpacing;
                dCurrBeforeSpacing = pCurrLine->CalculateBeforeSpacing(&dPreviousStringOffset);
                dPreviousStringOffset = pCurrLine->CalculateStringOffset();
                dCorrectionBeforeSpacing = dCurrBeforeSpacing;
                dCurrRight = dNextRight;

                if (pNextLine)
                {
                    dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(&dPreviousStringOffset);
                    dNextRight = pNextLine->CalculateRightBorder(m_dWidth);
                }

                //проверим, подходят ли следующие строчки для текущего pParagraph
                while(pNextLine &&
                      fabs(pCurrLine->m_dHeight - pNextLine->m_dHeight) < c_dTHE_SAME_STRING_Y_PRECISION_MM && //высота строк должна быть примерно одинаковой
                      fabs(dCurrBeforeSpacing - dNextBeforeSpacing) < c_dLINE_DISTANCE_ERROR_MM && //расстрояние между строк тоже одинаково
                      pCurrLine->AreAlignmentsAppropriate(pNextLine) &&
                      ((fabs(pCurrLine->m_dLeft - pNextLine->m_dLeft) <= c_dERROR_OF_RIGHT_BORDERS_MM && //у последующих строк нет отступа относительно предыдущей непервой строки
                        (fabs(dCurrRight - dNextRight) <= c_dERROR_OF_RIGHT_BORDERS_MM || dCurrRight < dNextRight))) //а следующая строка либо короче, либо такая же
                      )
                {
                    nIndex++;

                    if (!IsShadingPresent(pCurrLine, pNextLine))
                    {
                        pParagraph->m_bIsShadingPresent = false;
                        pParagraph->m_lColorOfShadingFill = c_iWhiteColor;
                    }

                    pPrevLine = pCurrLine;
                    pCurrLine = pNextLine;
                    ++nNextIndex < m_arTextLine.size()?
                        pNextLine = m_arTextLine[nNextIndex] :
                        pNextLine = nullptr;

                    dPrevBeforeSpacing = dCurrBeforeSpacing;
                    dCurrBeforeSpacing = pCurrLine->CalculateBeforeSpacing(&dPreviousStringOffset);
                    dPreviousStringOffset = pCurrLine->CalculateStringOffset();
                    dCorrectionBeforeSpacing = (dCorrectionBeforeSpacing + dCurrBeforeSpacing)/2; //наверное лучше так... текст может быть уже, чем в оригинале
                    dCurrRight = dNextRight;

                    if (pNextLine)
                    {
                        dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(&dPreviousStringOffset);
                        dNextRight = pNextLine->CalculateRightBorder(m_dWidth);
                    }

                    //Объединим 2 параграфа-строчки
                    pParagraph->m_arLines.back()->AddSpaceToEnd();
                    //pParagraph->m_arLines.back()->m_arConts.back()->m_oText += pNextLine->m_arConts.back()->m_oText;
                    pParagraph->m_arLines.push_back(new CTextLine(*pCurrLine));
                    pParagraph->m_nNumLines++;

                    pParagraph->m_dRight = std::min(pParagraph->m_dRight, dCurrRight);
                    pParagraph->m_dLeft = std::min(pParagraph->m_dLeft, pCurrLine->m_dLeft);
                    pParagraph->m_dWidth = std::max(pParagraph->m_dWidth, pCurrLine->m_dWidth + pCurrLine->m_arConts.back()->m_dSpaceWidthMM);
                }

                //коррекция
                pParagraph->m_dHeight += dCorrectionBeforeSpacing;
                pParagraph->m_dRight -= RightBorderCorrection(pCurrLine);
                pParagraph->m_dSpaceBefore = std::abs(pParagraph->m_dSpaceBefore - dCorrectionBeforeSpacing);

                pParagraph->m_dSpaceBefore += std::max(dBeforeSpacingWithShapes, 0.0);
                dBeforeSpacingWithShapes = 0;

                pParagraph->RemoveHighlightColor();

                m_arParagraphs.push_back(pParagraph);
            }
            else
            {
                //будет отдельной параграфом-строчкой
                CreateSingleLineParagraph(pCurrLine, &dCurrRight, &dCurrBeforeSpacing);

                //или будет шейпом для теста - вроде неплохо работает без учета проблем в todo
                /*CreateSingleLineShape(pCurrLine);

                double dCurrentAdditive = pCurrLine->m_dHeight + std::max(dCurrBeforeSpacing, 0.0);
                if (pNextLine && pCurrLine->AreLinesCrossing(pNextLine))
                {
                    dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(&dPreviousStringOffset);
                    dCurrentAdditive -= pNextLine->m_dHeight + dNextBeforeSpacing;//std::max(dNextBeforeSpacing, 0.0);
                }
                dBeforeSpacingWithShapes += dCurrentAdditive;*/
            }
        }
    }

    double CPage::RightBorderCorrection(const CTextLine *pLine)
    {
        size_t nIndex = 0;
        double dHeight = 0;
        for (size_t i = 0; i < pLine->m_arConts.size(); ++i)
        {
            if (dHeight < pLine->m_arConts[i]->m_dHeight)
            {
                dHeight = pLine->m_arConts[i]->m_dHeight;
                nIndex = i;
            }
        }

        UINT lSize = static_cast<UINT>(2 * pLine->m_arConts[nIndex]->m_oFont.Size);
        UINT nType = pLine->m_arConts[nIndex]->m_oFont.GetTextFontStyle();
        if (nType > 3)
        {
            //Error!
            return 0.0;
        }

        //note нужно корректировать каждый размер отдельно
        //note для больших форматов возможно нужно добавить промежуточные значения
        if      (lSize <= 12) return c_dRightBorderCorrectionSize[0][nType]; //6pt
        else if (lSize <= 14) return c_dRightBorderCorrectionSize[1][nType]; //7pt
        else if (lSize <= 16) return c_dRightBorderCorrectionSize[2][nType]; //8pt
        else if (lSize <= 18) return c_dRightBorderCorrectionSize[3][nType]; //9pt
        else if (lSize <= 20) return c_dRightBorderCorrectionSize[4][nType]; //10pt
        else if (lSize <= 22) return c_dRightBorderCorrectionSize[5][nType]; //11pt
        else if (lSize <= 24) return c_dRightBorderCorrectionSize[6][nType]; //12pt
        else if (lSize <= 28) return c_dRightBorderCorrectionSize[7][nType]; //14pt
        else if (lSize <= 32) return c_dRightBorderCorrectionSize[8][nType]; //16pt
        else if (lSize <= 36) return c_dRightBorderCorrectionSize[9][nType]; //18pt
        else if (lSize <= 40) return c_dRightBorderCorrectionSize[10][nType]; //20pt
        else if (lSize <= 44) return c_dRightBorderCorrectionSize[11][nType]; //22pt
        else if (lSize <= 48) return c_dRightBorderCorrectionSize[12][nType]; //24pt
        else if (lSize <= 52) return c_dRightBorderCorrectionSize[13][nType]; //26pt
        else if (lSize <= 56) return c_dRightBorderCorrectionSize[14][nType]; //28pt
        else if (lSize <= 72) return c_dRightBorderCorrectionSize[15][nType]; //36pt
        else if (lSize <= 96) return c_dRightBorderCorrectionSize[16][nType]; //48pt
        else if (lSize <= 144)return c_dRightBorderCorrectionSize[17][nType]; //72pt
        else                  return c_dRightBorderCorrectionSize[18][nType];
    }

    bool CPage::IsShadingPresent(const CTextLine *pLine1, const CTextLine *pLine2)
    {
        if (pLine1->m_pDominantShape && pLine2->m_pDominantShape &&
            pLine1->m_pDominantShape->m_oBrush.Color1 == pLine2->m_pDominantShape->m_oBrush.Color1)
        {
            return true;
        }

        return false;
    }

    void CPage::CreateSingleLineParagraph(CTextLine *pLine, const double *pRight, const double *pBeforeSpacing)
    {
        CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
        pParagraph->m_eTextConversionType = CParagraph::tctTextToParagraph;
        pParagraph->m_arLines.push_back(pLine);
        pParagraph->m_nNumLines++;

        pParagraph->m_dLeft	= pLine->m_dLeft;
        pParagraph->m_dTop	= pLine->m_dBaselinePos - pLine->m_dHeight - pLine->m_dBaselineOffset;

        pParagraph->m_dFirstLine = 0;
        pParagraph->m_dRight = *pRight - RightBorderCorrection(pLine);
        pParagraph->m_dWidth = pLine->m_dWidth;
        pParagraph->m_dHeight = pLine->m_dHeight;
        if (*pBeforeSpacing < 0)
        {
            pParagraph->m_dHeight += *pBeforeSpacing;
        }

        pParagraph->m_dSpaceBefore = std::max(*pBeforeSpacing, 0.0);
        pParagraph->m_dBaselinePos = pLine->m_dBaselinePos;

        if (pLine->m_pDominantShape)
        {
            pParagraph->m_bIsShadingPresent = true;
            pParagraph->m_lColorOfShadingFill = pLine->m_pDominantShape->m_oBrush.Color1;
            pParagraph->RemoveHighlightColor();
        }

        m_arParagraphs.push_back(pParagraph);
    }

    void CPage::CreateSingleLineOldShape(CTextLine *pLine)
    {
        CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
        pParagraph->m_eTextConversionType = CParagraph::tctTextToShape;
        pParagraph->m_arLines.push_back(pLine);
        pParagraph->m_nNumLines++;

        if (pLine->m_pDominantShape)
        {
            pParagraph->m_bIsShadingPresent = true;
            pParagraph->m_lColorOfShadingFill = pLine->m_pDominantShape->m_oBrush.Color1;
            pParagraph->RemoveHighlightColor();
        }

        COldShape* pShape = new COldShape();
        pShape->m_arParagraphs.push_back(pParagraph);

        //todo В итоге Left и Top смещаются на несколько мм. Пока не понял чем это вызвано.
        pShape->m_dLeft	= pLine->m_dLeft - COldShape::POSITION_CORRECTION_FOR_X_MM; //подобранная константа
        pShape->m_dTop	= pLine->m_dBaselinePos - pLine->m_dHeight - pLine->m_dBaselineOffset - COldShape::POSITION_CORRECTION_FOR_Y_MM;//подобранная константа
        //todo Width и Height маловаты и текст обрезается. разобраться почему
        pShape->m_dWidth = pLine->m_dWidth + COldShape::SIZE_CORRECTION_FOR_X_MM; //5мм мало для длинной строки
        pShape->m_dHeight = pLine->m_dHeight + COldShape::SIZE_CORRECTION_FOR_Y_MM;

        //m_arGraphicItems.push_back(pShape);
    }

    void CPage::CreateSingleLineShape(CTextLine *pLine)
    {
        CParagraph* pParagraph = new CParagraph(m_eTextAssociationType);
        pParagraph->m_eTextConversionType = CParagraph::tctTextToShape;
        pParagraph->m_arLines.push_back(pLine);
        pParagraph->m_nNumLines++;

        if (pLine->m_pDominantShape)
        {
            pParagraph->m_bIsShadingPresent = true;
            pParagraph->m_lColorOfShadingFill = pLine->m_pDominantShape->m_oBrush.Color1;
            pParagraph->RemoveHighlightColor();
        }

        CShape* pShape = new CShape();
        pShape->m_arParagraphs.push_back(pParagraph);

        pShape->m_dLeft	= pLine->m_dLeft;
        pShape->m_dTop	= pLine->m_dBaselinePos - pLine->m_dHeight - pLine->m_dBaselineOffset;
        pShape->m_dWidth = pLine->m_dWidth + RightBorderCorrection(pLine);
        pShape->m_dHeight = pLine->m_dHeight;

        pShape->m_bIsNoFill = true;
        pShape->m_bIsNoStroke = true;

        m_arShapes.push_back(pShape);
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

    void CPage::ToXml(NSStringUtils::CStringBuilder& oWriter)
    {
        if (!m_arImages.empty())
        {
            oWriter.WriteString(L"<w:p>");
            //note при удалении строки откуда-то добавляется <w:p/> в начале страницы (если есть графика и текст), что добавляет дополнительную строку и сдвигает текст
            oWriter.WriteString(L"<w:pPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr>");

            for (size_t i = 0; i < m_arImages.size(); ++i)
            {
                if (!m_arImages[i]->m_bIsNotNecessaryToUse)
                {
                    m_arImages[i]->ToXml(oWriter);
                }
            }

            oWriter.WriteString(L"</w:p>");
        }

        // drawings
        if (!m_arShapes.empty())
        {
            oWriter.WriteString(L"<w:p>");
            //note при удалении строки откуда-то добавляется <w:p/> в начале страницы (если есть графика и текст), что добавляет дополнительную строку и сдвигает текст
            oWriter.WriteString(L"<w:pPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr>");

            for (size_t i = 0; i < m_arShapes.size(); ++i)
            {
                if (!m_arShapes[i]->m_bIsNotNecessaryToUse)
                {
                    m_arShapes[i]->ToXml(oWriter);
                }
            }

            oWriter.WriteString(L"</w:p>");
        }

        for (size_t i = 0; i < m_arParagraphs.size(); ++i)
        {
            if (!m_arParagraphs[i]->m_bIsNotNecessaryToUse)
            {
                m_arParagraphs[i]->ToXml(oWriter);
            }
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
