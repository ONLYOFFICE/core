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
        for (auto pCont : m_arSymbol)
        {
            RELEASEOBJECT(pCont);
        }
        m_arSymbol.clear();
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

    void CPage::SetCurrentLineByBaseline(const CContText* pCont)
    {
        if ((nullptr == m_pCurrentLine) || (tatBlockChar == m_eTextAssociationType))
        {
            // пустая (в плане текста) страница
            m_pCurrentLine = new CTextLine();
            m_pCurrentLine->m_dBaselinePos = pCont->m_dBaselinePos;
            m_arTextLine.push_back(m_pCurrentLine);
            return;
        }
        if (fabs(m_pCurrentLine->m_dBaselinePos - pCont->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
        {
            if (m_pCurrentLine->m_eVertAlignType == eVertAlignType::vatUnknown &&
                pCont->m_eVertAlignType != eVertAlignType::vatUnknown)
            {
                //note считаем, что линия может иметь только один тип
                m_pCurrentLine->m_eVertAlignType = pCont->m_eVertAlignType;
            }
            return;
        }
        for (size_t i = 0; i < m_arTextLine.size(); ++i)
        {
            if (fabs(m_arTextLine[i]->m_dBaselinePos - pCont->m_dBaselinePos) <= c_dTHE_SAME_STRING_Y_PRECISION_MM)
            {
                m_pCurrentLine = m_arTextLine[i];
                if (m_pCurrentLine->m_eVertAlignType == eVertAlignType::vatUnknown &&
                    pCont->m_eVertAlignType != eVertAlignType::vatUnknown)
                {
                    //note считаем, что линия может иметь только один тип
                    m_pCurrentLine->m_eVertAlignType = pCont->m_eVertAlignType;
                }
                return;
            }
        }

        // линия не нашлась - не беда - создадим новую
        m_pCurrentLine = new CTextLine();
        m_pCurrentLine->m_dBaselinePos = pCont->m_dBaselinePos;
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

        m_arSymbol.push_back(pCont);
    }

    void CPage::AnalyzeCollectedShapes()
    {
        //todo Объединить контур и заливку одного рисунка в шейпе если m_strPath одинаковые
        //todo Объединить выделения соседних строк

        CorrelateContWithShape();
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

    void CPage::CorrelateContWithShape()
    {
        //
        for (auto pShape : m_arShapes)
        {
            if (pShape->m_bIsNotNecessaryToUse ||
                (pShape->m_eGraphicsType != eGraphicsType::gtRectangle &&
                 pShape->m_eGraphicsType != eGraphicsType::gtCurve))
            {
                continue;
            }

            for (auto pCont : m_arSymbol)
            {
                double dTopShape = pShape->m_dTop;
                double dTopCont = pCont->m_dTop;
                double dBottomShape = pShape->m_dTop + pShape->m_dHeight;
                double dBottomCont = pCont->m_dBaselinePos - pCont->m_dBaselineOffset;
                double dRightShape = pShape->m_dLeft + pShape->m_dWidth;
                double dRightCont = pCont->m_dLeft + pCont->m_dWidth;

                double dSomeBaseLine1 = dBottomCont - pCont->m_dHeight * 0.75;
                double dSomeBaseLine2 = dBottomCont - pCont->m_dHeight * 0.5;
                double dSomeBaseLine3 = dBottomCont - pCont->m_dHeight * 0.25;

                //Условие пересечения по вертикали для подчеркиваний/зачеркиваний
                bool bIf1 = ((fabs(dTopShape - dTopCont) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                              dTopShape > dTopCont) &&
                             (fabs(dBottomShape - dBottomCont) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                              dBottomShape < dBottomCont)) ||
                             (fabs(dTopShape - dBottomCont) < pCont->m_dHeight * 0.15);
                //Условие пересечения по вертикали для выделения текста
                bool bIf2 = (dSomeBaseLine1 > dTopShape && dSomeBaseLine1 < dBottomShape &&
                             dSomeBaseLine2 > dTopShape && dSomeBaseLine2 < dBottomShape &&
                             dSomeBaseLine3 > dTopShape && dSomeBaseLine3 < dBottomShape);
                //Условие пересечения по горизонтали
                bool bIf3 = fabs(pShape->m_dLeft - pCont->m_dLeft) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                        fabs(dRightShape - dRightCont) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                        (pShape->m_dLeft < pCont->m_dLeft && dRightShape > dRightCont);
                //Условие для размеров по высоте (нужно если только это не выделение)
                bool bIf4 = bIf2 ||
                        fabs(pCont->m_dHeight - pShape->m_dHeight) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                        pShape->m_dHeight < pCont->m_dHeight;

                if ((bIf1 || bIf2) && bIf3 && bIf4)
                {
                    //note Выбираем Cont c максимальным размером шрифта (возможно понадобится для определения толщины линий)
                    if (!pShape->m_pCont || pShape->m_pCont->m_oFont.Size < pCont->m_oFont.Size)
                    {
                        pShape->m_pCont = pCont;
                    }
                }
            }
        }
    }

    void CPage::DetermineLinesType()
    {
        for (size_t i = 0; i < m_arShapes.size(); ++i)
        {
            if (m_arShapes[i]->m_bIsNotNecessaryToUse ||
                !m_arShapes[i]->m_pCont || //note определяем тип только для зачеркиваний/подчеркиваний/выделений
                m_arShapes[i]->m_dHeight > c_dMAX_LINE_MM || //рассматриваем только тонкие объекты
                (m_arShapes[i]->m_eGraphicsType != eGraphicsType::gtRectangle &&
                m_arShapes[i]->m_eGraphicsType != eGraphicsType::gtCurve))
            {
                continue;
            }

            //Нужно собрать всю графику, которая находится на одной линии
            std::vector<CShape*> arCurrShapes;
            arCurrShapes.push_back(m_arShapes[i]);

            for (size_t j = i+1; j < m_arShapes.size(); ++j)
            {
                bool bIf1 = !m_arShapes[j]->m_bIsNotNecessaryToUse;
                bool bIf2 = m_arShapes[i]->IsCorrelated(m_arShapes[j]);
                //note довольно странное поведение - в зависимости от толщины линии информация о графике записывается в разные структуры
                bool bIf3 = m_arShapes[i]->m_oBrush.IsEqual(&m_arShapes[j]->m_oBrush);
                bool bIf4 = m_arShapes[i]->m_oPen.IsEqual(&m_arShapes[j]->m_oPen);
                //линия должна быть одного размера по высоте
                bool bIf5 = fabs(m_arShapes[i]->m_dHeight - m_arShapes[j]->m_dHeight) < c_dGRAPHICS_ERROR_IN_LINES_MM;
                //все должно быть на одной линии
                bool bIf6 = fabs(m_arShapes[i]->m_dTop - m_arShapes[j]->m_dTop) < c_dGRAPHICS_ERROR_IN_LINES_MM * 5;

                if (bIf1 && bIf2 && (bIf3 || bIf4) && bIf5 && bIf6) //все должно быть на одной линии
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
                    pCurrShape->DetermineLineType(pNextShape, k == nCurrShapesCount - 1);

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

    void CPage::AnalyzeCollectedSymbols()
    {
        for (auto pShape : m_arShapes)
        {
            if (//pShape->m_bIsNotNecessaryToUse ||
                !pShape->m_pCont || //note если нет указателя, то текст далеко от графики
                (pShape->m_eGraphicsType != eGraphicsType::gtRectangle &&
                pShape->m_eGraphicsType != eGraphicsType::gtCurve &&
                pShape->m_eLineType != eLineType::ltUnknown))
            {
                continue;
            }

            for (auto pCont : m_arSymbol)
            {
                if (IsLineCrossingText(pShape, pCont) ||
                    IsLineBelowText(pShape, pCont) ||
                    IsItHighlightingBackground(pShape, pCont))
                {
                    pShape->m_bIsNotNecessaryToUse = true;
                }
            }
        }

        DetermineVertAlignTypeBetweenConts();
    }

    bool CPage::IsLineCrossingText(const CShape* pShape, CContText* pCont)
    {
        double dTopShape = pShape->m_dTop;
        double dTopCont = pCont->m_dTop + pCont->m_dHeight/2; //note Height - это максимально возможный размер символа. Больше реального размера.
        double dBottomShape = pShape->m_dTop + pShape->m_dHeight;
        double dBottomContText = pCont->m_dBaselinePos;

        double dRightShape = pShape->m_dLeft + pShape->m_dWidth;
        double dRightContText = pCont->m_dLeft + pCont->m_dWidth;

        bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle &&
                pShape->m_eLineType != eLineType::ltUnknown;
        //Условие пересечения по вертикали
        bool bIf2 = dTopShape > dTopCont && dBottomShape < dBottomContText;
        //Условие пересечения по горизонтали
        bool bIf3 = fabs(pShape->m_dLeft - pCont->m_dLeft) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                fabs(dRightShape - dRightContText) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                (pShape->m_dLeft < pCont->m_dLeft && dRightShape > dRightContText);
        //Условие для размеров по высоте
        bool bIf4 = pShape->m_dHeight < pCont->m_dHeight &&
                pCont->m_dHeight - pShape->m_dHeight > c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM;

        if (bIf1 && bIf2 && bIf3 && bIf4)
        {
            pCont->m_oFont.Strikeout = TRUE;
            if (pShape->m_eLineType == eLineType::ltDouble)
            {
                pCont->m_bIsDoubleStrikeout = true;
            }
            return true;
        }

        return false;
    }

    bool CPage::IsLineBelowText(const CShape* pShape, CContText* pCont)
    {
        double dTopShape = pShape->m_dTop;
        double dBottomContText = pCont->m_dBaselinePos;

        double dRightShape = pShape->m_dLeft + pShape->m_dWidth;
        double dRightContText = pCont->m_dLeft + pCont->m_dWidth;

        bool bIf1 = (pShape->m_eGraphicsType == eGraphicsType::gtRectangle || pShape->m_eGraphicsType == eGraphicsType::gtCurve) &&
                pShape->m_eLineType != eLineType::ltUnknown;
        //Условие по вертикали
        bool bIf2 = fabs(dTopShape - dBottomContText) < pCont->m_dHeight * 0.15;
        //Условие пересечения по горизонтали
        bool bIf3 = fabs(pShape->m_dLeft - pCont->m_dLeft) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                fabs(dRightShape - dRightContText) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                (pShape->m_dLeft < pCont->m_dLeft && dRightShape > dRightContText);
        //Условие для размеров по высоте
        bool bIf4 = pShape->m_dHeight < pCont->m_dHeight &&
                pCont->m_dHeight - pShape->m_dHeight > c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM;

        if (bIf1 && bIf2 && bIf3 && bIf4)
        {
            pCont->m_oFont.Underline = TRUE;
            pCont->m_eUnderlineType  = pShape->m_eLineType;
            pCont->m_lUnderlineColor = pShape->m_dHeight > 0.3 ? pShape->m_oBrush.Color1 : pShape->m_oPen.Color;
            return true;
        }

        return false;
    }

    bool CPage::IsItHighlightingBackground(const CShape* pShape, CContText* pCont)
    {
        double dTopShape = pShape->m_dTop;
        double dTopCont = pCont->m_dTop;
        double dBottomShape = pShape->m_dTop + pShape->m_dHeight;
        double dBottomContText = pCont->m_dBaselinePos - pCont->m_dBaselineOffset;
        double dSomeBaseLine1 = dBottomContText - pCont->m_dHeight * 0.75;
        double dSomeBaseLine2 = dBottomContText - pCont->m_dHeight * 0.5;
        double dSomeBaseLine3 = dBottomContText - pCont->m_dHeight * 0.25;

        double dRightShape = pShape->m_dLeft + pShape->m_dWidth;
        double dRightContText = pCont->m_dLeft + pCont->m_dWidth;

        bool bIf1 = pShape->m_eGraphicsType == eGraphicsType::gtRectangle;
        //Условие пересечения по вертикали
        bool bIf2 = (dSomeBaseLine1 > dTopShape && dSomeBaseLine1 < dBottomShape &&
                     dSomeBaseLine2 > dTopShape && dSomeBaseLine2 < dBottomShape &&
                     dSomeBaseLine3 > dTopShape && dSomeBaseLine3 < dBottomShape);
        //Условие пересечения по горизонтали
        bool bIf3 = fabs(pShape->m_dLeft - pCont->m_dLeft) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                fabs(dRightShape - dRightContText) < c_dERROR_FOR_TEXT_WITH_GRAPHICS_MM ||
                (pShape->m_dLeft < pCont->m_dLeft && dRightShape > dRightContText);
        //Цвета должны быть разными
        bool bIf4 = pCont->m_oBrush.Color1 != pShape->m_oBrush.Color1 ||
                pCont->m_oBrush.Color1 != pShape->m_oPen.Color;

        if (bIf1 && bIf2 && bIf3 && bIf4)
        {
            //Удовлетворяет расположением и размером - привязываем указатель на картинку
            pCont->m_pShape = pShape;
            pCont->m_bIsHighlightPresent = true;
            pCont->m_lHighlightColor = pShape->m_oBrush.Color1;
            return true;
        }

        return false;
    }

    void CPage::DetermineVertAlignTypeBetweenConts()
    {
        for (size_t i = 0; i < m_arSymbol.size(); i++)
        {
            auto pCont = m_arSymbol[i];

            if (pCont->m_bIsNotNecessaryToUse ||
                    i > m_arSymbol.size() - 2)
            {
                continue;
            }

            auto pNext = m_arSymbol[i+1];

            double dRightCont = pCont->m_dLeft + pCont->m_dWidth;
            double dRightNext = pNext->m_dLeft + pNext->m_dWidth;

            //Условие пересечения по вертикали
            bool bIf1 = (pCont->m_dTop < pNext->m_dTop && pCont->m_dBaselinePos < pNext->m_dBaselinePos); //текущая линия ниже
            bool bIf2 = (pCont->m_dTop > pNext->m_dTop && pCont->m_dBaselinePos > pNext->m_dBaselinePos); //текущая линия выше
            //Условие пересечения по горизонтали
            bool bIf3 = fabs(dRightCont - pNext->m_dLeft) < c_dTHE_STRING_X_PRECISION_MM * 3; //текущая линия левее
            bool bIf4 = fabs(pCont->m_dLeft - dRightNext) < c_dTHE_STRING_X_PRECISION_MM * 3; //текущая линия правее
            //Размеры шрифта должны бать разными
            bool bIf5 = pCont->m_oFont.Size > pNext->m_oFont.Size;
            bool bIf6 = pCont->m_oFont.Size < pNext->m_oFont.Size;
            bool bIf7 = pNext->m_oFont.Size > pCont->m_oFont.Size;
            bool bIf8 = pNext->m_oFont.Size < pCont->m_oFont.Size;

            if ((bIf1 || bIf2) && (bIf3 || bIf4) &&
                    ((bIf3 && (bIf5 || bIf6)) || (bIf4 && (bIf7 || bIf8))))
            {
                if (bIf1 && bIf5)
                {
                    pNext->m_eVertAlignType = eVertAlignType::vatSubscript;
                    pNext->m_pCont = pCont;
                    pCont->m_eVertAlignType = eVertAlignType::vatBase;
                    pCont->m_pCont = pNext;
                }
                else if (bIf2 && bIf8)
                {
                    pNext->m_eVertAlignType = eVertAlignType::vatSuperscript;
                    pNext->m_pCont = pCont;
                    pCont->m_eVertAlignType = eVertAlignType::vatBase;
                    pCont->m_pCont = pNext;
                }
                else if (bIf1 && bIf6)
                {
                    pCont->m_eVertAlignType = eVertAlignType::vatSuperscript;
                    pCont->m_pCont = pNext;
                    pNext->m_eVertAlignType = eVertAlignType::vatBase;
                    pNext->m_pCont = pCont;
                }
                else if (bIf2 && bIf7)
                {
                    pCont->m_eVertAlignType = eVertAlignType::vatSubscript;
                    pCont->m_pCont = pNext;
                    pNext->m_eVertAlignType = eVertAlignType::vatBase;
                    pNext->m_pCont = pCont;
                }
            }
        }
    }

    void CPage::BuildLines()
    {
        for (auto pCont : m_arSymbol)
        {
            //note Элементы в m_arSymbol в случайных местах страницы
            BuildLines(pCont);
        }

        MergeLinesByVertAlignType();

        for (auto pLine : m_arTextLine)
        {
            pLine->SortConts();
            pLine->CalculateWidth();
            if (m_eTextAssociationType == tatPlainLine)
            {
                pLine->Analyze();
            }
            pLine->DetermineAssumedTextAlignmentType(m_dWidth);
        }

        if (m_eTextAssociationType == tatPlainParagraph ||
            m_eTextAssociationType == tatShapeLine ||
            m_eTextAssociationType == tatPlainLine)
        {
            DetermineDominantGraphics();
        }
    }

    void CPage::BuildLines(const CContText* pCont)
    {
        double dTextX       = pCont->m_dLeft;
        double dTextW       = pCont->m_dWidth;
        NSStringUtils::CStringUTF32 oText(pCont->m_oText);

        SetCurrentLineByBaseline(pCont);

        CContText* pLastCont = nullptr;
        size_t nCountConts = m_pCurrentLine->m_arConts.size();
        if (nCountConts != 0)
            pLastCont = m_pCurrentLine->m_arConts.back();

        if (nullptr == pLastCont)
        {
            // первое слово в линии
            CContText* pCurrCont = new CContText(*pCont);

            pCurrCont->m_dLastX = dTextX;

            m_pCurrentLine->AddCont(pCurrCont);
            m_dLastTextX = dTextX;
            m_dLastTextY = pCurrCont->m_dBaselinePos;
            m_dLastTextX_block = m_dLastTextX;
            return;
        }

        if (pLastCont->IsEqual(pCont))
        {
            bool bIsConditionPassed = false;
            double dRight = pLastCont->m_dLeft + pLastCont->m_dWidth;

            // настройки одинаковые. теперь смотрим, на расположение
            if (fabs(dRight - dTextX) < c_dTHE_STRING_X_PRECISION_MM)
            {
                // продолжаем слово
                pLastCont->m_oText += oText;
                pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dLeft);
                bIsConditionPassed = true;
            }
            else if ((dRight < dTextX) && ((dTextX - dRight) < pCont->m_dSpaceWidthMM))
            {
                // продолжаем слово с пробелом
                pLastCont->m_oText += uint32_t(' ');
                pLastCont->m_oText += oText;
                pLastCont->m_dWidth	= (dTextX + dTextW - pLastCont->m_dLeft);
                bIsConditionPassed = true;
            }
            else if (fabs(pCont->m_dBaselinePos - pLastCont->m_dBaselinePos) < 0.01 &&
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
                m_dLastTextY = pCont->m_dBaselinePos;
                m_dLastTextX_block = m_dLastTextX;
                pLastCont->m_dLastX = dTextX;
                if (!pLastCont->m_pCont)
                {
                    pLastCont->m_pCont = pCont->m_pCont;
                    pLastCont->m_eVertAlignType = pCont->m_eVertAlignType;
                }
                return;
            }
        }

        // либо пробел большой между словами, либо новый текст левее, либо настройки не те (шрифт, кисть)
        // либо все вместе... просто добавл¤ем новое слово
        m_pCurrentLine->AddCont(new CContText(*pCont));
        m_dLastTextX = dTextX;
        m_dLastTextY = pCont->m_dBaselinePos;
        m_dLastTextX_block = m_dLastTextX;
    }

    void CPage::MergeLinesByVertAlignType()
    {
        SortElements(m_arTextLine);

        for (size_t i = 0; i < m_arTextLine.size(); i++)
        {
            auto pLine = m_arTextLine[i];

            if (pLine->m_bIsNotNecessaryToUse)
            {
                continue;
            }

            if (pLine->m_eVertAlignType != eVertAlignType::vatUnknown)
            {
                if (i > m_arTextLine.size() - 2)
                {
                    continue;
                }

                auto pLineNext = m_arTextLine[i+1];

                if (pLine->m_eVertAlignType == eVertAlignType::vatSuperscript &&
                    pLineNext->m_eVertAlignType == eVertAlignType::vatBase)
                {
                    pLine->m_bIsNotNecessaryToUse = true;
                    for (auto pCont : pLine->m_arConts)
                    {
                        pCont->m_eVertAlignType = eVertAlignType::vatSuperscript;

                        if (pCont->m_pCont)
                        {
                            pCont->m_oFont.Size = pCont->m_pCont->m_oFont.Size;
                        }

                        if (pLineNext->m_dLeft > pCont->m_dLeft)
                        {
                            pLineNext->m_dLeft = pCont->m_dLeft;
                        }

                        pLineNext->m_arConts.push_back(new CContText(*pCont));
                    }
                }
                else if (pLine->m_eVertAlignType == eVertAlignType::vatBase &&
                         pLineNext->m_eVertAlignType == eVertAlignType::vatSubscript)
                {
                    pLineNext->m_bIsNotNecessaryToUse = true;
                    for (auto pCont : pLineNext->m_arConts)
                    {
                        pCont->m_eVertAlignType = eVertAlignType::vatSubscript;

                        if (pCont->m_pCont)
                        {
                            pCont->m_oFont.Size = pCont->m_pCont->m_oFont.Size;
                        }

                        if (pLine->m_dLeft > pCont->m_dLeft)
                        {
                            pLine->m_dLeft = pCont->m_dLeft;
                        }

                        pLine->m_arConts.push_back(new CContText(*pCont));
                    }
                }
            }
        }
    }

    void CPage::DetermineDominantGraphics()
    {
        const CShape* pDominantShape = nullptr;

        for (auto pLine : m_arTextLine)
        {
            for (auto pCont : pLine->m_arConts)
            {
                if (pCont->m_pShape &&
                    pCont->m_pShape != pDominantShape)
                {
                    double dLeftShape = pCont->m_pShape->m_dLeft;
                    double dLeftCont = pCont->m_dLeft;
                    double dRightShape = dLeftShape + pCont->m_pShape->m_dWidth;
                    double dRightCont = dLeftCont + pCont->m_dWidth;

                    if (dLeftShape < dLeftCont && dRightShape > dRightCont)
                    {
                        if (!pDominantShape)
                        {
                            pDominantShape = pCont->m_pShape;
                        }
                        else
                        {
                            double dLeftDomShape = pDominantShape->m_dLeft;
                            double dRightDomShape = dLeftDomShape + pDominantShape->m_dWidth;
                            if (dLeftShape < dLeftDomShape && dRightShape > dRightDomShape)
                            {
                                pDominantShape = pCont->m_pShape;
                            }
                        }
                    }
                }
            }

            pLine->m_pDominantShape = pDominantShape;
            pDominantShape = nullptr;
        }
    }

    void CPage::BuildByType()
    {
        if (m_arTextLine.empty())
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

            pParagraph->m_dLeft = pTextLine->m_dLeft;
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
        Merge(c_dSTANDART_STRING_HEIGHT_MM / 3);

        double previousStringOffset = 0;
        for (auto pLine : m_arTextLine)
        {
            CTextLine* pTextLine = new CTextLine(*pLine);

            double dBeforeSpacing = pTextLine->CalculateBeforeSpacing(&previousStringOffset);
            previousStringOffset = pTextLine->CalculateStringOffset();
            double dRight = pTextLine->CalculateRightBorder(m_dWidth);

            CreateSingleLineParagraph(pTextLine, &dRight, &dBeforeSpacing);
        }
    }

    void CPage::BuildByTypeShapeLine()
    {
        for (auto pLine : m_arTextLine)
        {
            CTextLine* pTextLine = new CTextLine(*pLine);
            CreateSingleLineShape(pTextLine);
        }
    }

    void CPage::BuildByTypePlainParagraph()
    {
        //todo смещение текста по высоте при использовании шейпов
        //todo не отображается перенос в линии
        //todo если в линии есть перенос нужно обеъдинить строки в один параграф

        //todo в зависимости от очередности загрузки файлов проявляется проблема со шрифтами - текст в некоторых конвертированных файлах становится жирным, зачеркнутым или курсив. С одним файлом проблем не наблюдалось

        //todo добавить различные типы текста для распознавания: обычный-сплошной, списки-содержание и тд

        CTextLine *pCurrLine, *pNextLine, *pPrevLine;
        double dCurrRight = 0, dNextRight = 0, dPrevRight = 0;
        double dCurrBeforeSpacing = 0, dNextBeforeSpacing = 0, dPrevBeforeSpacing = 0;
        double dBeforeSpacingWithShapes = 0;
        double dPreviousStringOffset = 0;
        double dCorrectionBeforeSpacing = 0;

        bool bIf1, bIf2, bIf3, bIf4, bIf5, bIf6, bIf7;

        for (size_t nIndex = 0; nIndex < m_arTextLine.size(); ++nIndex)
        {
            if (m_arTextLine[nIndex]->m_bIsNotNecessaryToUse)
            {
                continue;
            }

            size_t nNextIndex = nIndex+1;
            CParagraph* pParagraph = nullptr;

            pCurrLine = new CTextLine(*m_arTextLine[nIndex]);
            nNextIndex == m_arTextLine.size() ?
                        pNextLine = nullptr :
                    pNextLine = m_arTextLine[nNextIndex];

            nIndex == 0 ?
                        pPrevLine = nullptr :
                    pPrevLine = m_arTextLine[nIndex-1];

            dPrevRight = dCurrRight;
            dCurrRight = pCurrLine->CalculateRightBorder(m_dWidth);
            dPrevBeforeSpacing = dCurrBeforeSpacing;
            dCurrBeforeSpacing = pCurrLine->CalculateBeforeSpacing(&dPreviousStringOffset);
            dPreviousStringOffset = pCurrLine->CalculateStringOffset();
            if (pNextLine)
            {
                dNextRight = pNextLine->CalculateRightBorder(m_dWidth);
                dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(&dPreviousStringOffset);
            }

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
            else */

            //Это не последняя строка
            bIf1 = pNextLine ? true : false;
            if (bIf1)
            {
                //Высота строк должна быть примерно одинаковой
                bIf2 = fabs(pCurrLine->m_dHeight - pNextLine->m_dHeight) <= c_dTHE_SAME_STRING_Y_PRECISION_MM;
                //
                bIf3 = true; //pCurrLine->AreAlignmentsAppropriate(pNextLine);
                //расстрояние между строк тоже одинаково
                bIf4 = fabs(dCurrBeforeSpacing - dNextBeforeSpacing) < c_dLINE_DISTANCE_ERROR_MM;
                //или
                bIf5 = dCurrBeforeSpacing > dNextBeforeSpacing;
                //есть отступ или нет отступа
                bIf6 = pCurrLine->m_dLeft >= pNextLine->m_dLeft;
                //следующая строка либо короче, либо такая же
                bIf7 = (fabs(dCurrRight - dNextRight) <= c_dERROR_OF_RIGHT_BORDERS_MM ||
                        dCurrRight < dNextRight);
            }

            if (bIf1 && bIf2 && bIf3 && (bIf4 || bIf5) && bIf6 && bIf7)
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
                dPrevRight = dCurrRight;
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
                      //pCurrLine->AreAlignmentsAppropriate(pNextLine) &&
                      ((fabs(pCurrLine->m_dLeft - pNextLine->m_dLeft) < c_dERROR_OF_LEFT_BORDERS_MM && //у последующих строк нет отступа относительно предыдущей непервой строки
                        (fabs(dCurrRight - dNextRight) < c_dERROR_OF_RIGHT_BORDERS_MM || //а следующая строка либо такая же
                         (dCurrRight < dNextRight && fabs(dPrevRight - dCurrRight) <= c_dERROR_OF_RIGHT_BORDERS_MM)))) //либо короче, но предыдущие равны
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
                    dPrevRight = dCurrRight;
                    dCurrRight = dNextRight;

                    if (pNextLine)
                    {
                        dNextBeforeSpacing = pNextLine->CalculateBeforeSpacing(&dPreviousStringOffset);
                        dNextRight = pNextLine->CalculateRightBorder(m_dWidth);
                    }

                    //Объединим 2 параграфа-строчки
                    pParagraph->m_arLines.back()->AddSpaceToEnd();
                    pParagraph->m_arLines.push_back(new CTextLine(*pCurrLine));
                    pParagraph->m_nNumLines++;

                    pParagraph->m_dRight = std::min(pParagraph->m_dRight, dCurrRight);
                    pParagraph->m_dLeft = std::min(pParagraph->m_dLeft, pCurrLine->m_dLeft);
                    pParagraph->m_dWidth = std::max(pParagraph->m_dWidth, pCurrLine->m_dWidth + pCurrLine->m_arConts.back()->m_dSpaceWidthMM);
                }

                //коррекция
                pParagraph->m_dHeight += dCorrectionBeforeSpacing;
                CTextLine* pMaxWidthLine = nullptr;
                for (auto pLine : pParagraph->m_arLines)
                {
                    if (!pMaxWidthLine || pLine->m_dWidth > pMaxWidthLine->m_dWidth)
                    {
                        pMaxWidthLine = pLine;
                    }
                }
                pParagraph->m_dRight -= RightBorderCorrection(pMaxWidthLine);
                pParagraph->m_dSpaceBefore = fabs(pParagraph->m_dSpaceBefore - dCorrectionBeforeSpacing);

                pParagraph->m_dSpaceBefore += std::max(dBeforeSpacingWithShapes, 0.0);
                dBeforeSpacingWithShapes = 0;

                pParagraph->RemoveHighlightColor();
                //todo записать все строки из одного параграфа в одну строчку
                //pParagraph->MergeLines();

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
        if (!pLine)
        {
            return c_dRightBorderCorrectionSize[0][0];
        }

        size_t nIndex = 0;
        double dHeight = 0;
        for (size_t i = 0; i < pLine->m_arConts.size(); ++i)
        {
            if (dHeight < pLine->m_arConts[i]->m_dHeight)
            {
                dHeight = pLine->m_arConts[i]->m_dHeight;
                nIndex = i;
            }
            else if (dHeight == pLine->m_arConts[i]->m_dHeight)
            {
                //note считаем что обычный < Italic < Bold < Bold-Italic
                if (pLine->m_arConts[nIndex]->m_oFont.GetTextFontStyle() <
                        pLine->m_arConts[i]->m_oFont.GetTextFontStyle())
                {
                    nIndex = i;
                }
            }
        }

        UINT lSize = static_cast<UINT>(2 * pLine->m_arConts[nIndex]->m_oFont.Size);
        UINT nType = pLine->m_arConts[nIndex]->m_oFont.GetTextFontStyle();
        if (nType > 3)
        {
            //Error!
            return 0.0;
        }

        if (lSize > 144)
        {
            lSize = 145;
        }

        //note нужно корректировать каждый размер отдельно
        return c_dRightBorderCorrectionSize[lSize][nType];
    }

    bool CPage::IsShadingPresent(const CTextLine *pLine1, const CTextLine *pLine2)
    {
        if (pLine1->m_pDominantShape && pLine2->m_pDominantShape &&
            pLine1->m_pDominantShape->m_oBrush.Color1 == pLine2->m_pDominantShape->m_oBrush.Color1 &&
            fabs(pLine1->m_pDominantShape->m_dLeft - pLine2->m_pDominantShape->m_dLeft) < c_dGRAPHICS_ERROR_IN_LINES_MM &&
            fabs(pLine1->m_pDominantShape->m_dWidth - pLine2->m_pDominantShape->m_dWidth) < c_dGRAPHICS_ERROR_IN_LINES_MM)
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

            for (auto pImage : m_arImages)
            {
                pImage->ToXml(oWriter);
            }

            oWriter.WriteString(L"</w:p>");
        }

        // drawings
        if (!m_arShapes.empty())
        {
            oWriter.WriteString(L"<w:p>");
            //note при удалении строки откуда-то добавляется <w:p/> в начале страницы (если есть графика и текст), что добавляет дополнительную строку и сдвигает текст
            oWriter.WriteString(L"<w:pPr><w:spacing w:line=\"1\" w:lineRule=\"exact\"/></w:pPr>");

            for (auto pShape : m_arShapes)
            {
                pShape->ToXml(oWriter);
            }

            oWriter.WriteString(L"</w:p>");
        }

        for (auto pParagraph : m_arParagraphs)
        {
            pParagraph->ToXml(oWriter);
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
