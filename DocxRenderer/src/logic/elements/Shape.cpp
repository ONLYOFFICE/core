#include "Shape.h"
#include <limits.h>
#include "../../resources/Constants.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
    UINT CShape::m_gRelativeHeight = c_iStandartRelativeHeight;

    CShape::CShape() : CBaseItem(ElemType::etShape)
    {
        m_nRelativeHeight = m_gRelativeHeight;
        m_gRelativeHeight += c_iStandartRelativeHeight;
    }

    CShape::CShape(std::shared_ptr<CImageInfo> pInfo, const std::wstring& strDstMedia) : CBaseItem(ElemType::etShape),
        m_strPath(strDstMedia), m_pImageInfo(pInfo)
    {
        m_nRelativeHeight = m_gRelativeHeight;
        m_gRelativeHeight += c_iStandartRelativeHeight;
    }

    CShape::~CShape()
    {
        Clear();
    }

    void CShape::Clear()
    {
        m_arParagraphs.clear();
    }

    UINT CShape::GenerateShapeId()
    {
        static UINT iId = 0;
        iId++;
        return iId;
    }

    void CShape::ResetRelativeHeight()
    {
        m_gRelativeHeight = c_iStandartRelativeHeight;
    }

    void CShape::GetDataFromVector(const CVectorGraphics& oVector)
    {
        m_dLeft = oVector.m_dLeft;
        m_dTop = oVector.m_dTop;
        m_dWidth = oVector.m_dRight - m_dLeft;
        m_dHeight = oVector.m_dBottom - m_dTop;

        if (m_dWidth < 0.0001)
            m_dWidth = 0.0001;
        if (m_dHeight < 0.0001)
            m_dHeight = 0.0001;

        m_dBaselinePos = m_dTop + m_dHeight;
        m_dRight = m_dLeft + m_dWidth;

        WritePath(oVector);
    }

    void CShape::WritePath(const CVectorGraphics& oVector)
    {
        size_t nCount = oVector.GetCurSize();
        double *pData = oVector.m_pData;

        double dWidth = oVector.m_dRight - oVector.m_dLeft;
        double dHeight = oVector.m_dBottom - oVector.m_dTop;

        NSStringUtils::CStringBuilder oWriter;

        oWriter.WriteString(L"<a:path w=\"");
        oWriter.AddInt(static_cast<int>(dWidth * c_dMMToEMU));
        oWriter.WriteString(L"\" h=\"");
        oWriter.AddInt(static_cast<int>(dHeight * c_dMMToEMU));
        oWriter.WriteString(L"\">");

        size_t nPeacks = 0;
        size_t nCurves = 0;

        while (nCount > 0)
        {
            CVectorGraphics::VectorGraphicsType eType = static_cast<CVectorGraphics::VectorGraphicsType>((int)(0.5 + *pData++));

            switch (eType)
            {
            case CVectorGraphics::vgtMove:
            {
                LONG lX = static_cast<LONG>((*pData - m_dLeft) * c_dMMToEMU);
                ++pData;
                LONG lY = static_cast<LONG>((*pData - m_dTop) * c_dMMToEMU);
                ++pData;

                oWriter.WriteString(L"<a:moveTo><a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY));
                oWriter.WriteString(L"\"/></a:moveTo>");

                nPeacks++;
                nCount -= 3;
                break;
            }
            case CVectorGraphics::vgtLine:
            {
                LONG lX = static_cast<LONG>((*pData - m_dLeft)* c_dMMToEMU);
                ++pData;
                LONG lY = static_cast<LONG>((*pData - m_dTop)* c_dMMToEMU);
                ++pData;

                oWriter.WriteString(L"<a:lnTo><a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY));
                oWriter.WriteString(L"\"/></a:lnTo>");

                nPeacks++;
                nCount -= 3;
                break;
            }
            case CVectorGraphics::vgtCurve:
            {
                LONG lX1 = static_cast<LONG>((*pData - m_dLeft)* c_dMMToEMU);
                ++pData;
                LONG lY1 = static_cast<LONG>((*pData - m_dTop)* c_dMMToEMU);
                ++pData;
                LONG lX2 = static_cast<LONG>((*pData - m_dLeft)* c_dMMToEMU);
                ++pData;
                LONG lY2 = static_cast<LONG>((*pData - m_dTop)* c_dMMToEMU);
                ++pData;
                LONG lX3 = static_cast<LONG>((*pData - m_dLeft)* c_dMMToEMU);
                ++pData;
                LONG lY3 = static_cast<LONG>((*pData - m_dTop)* c_dMMToEMU);
                ++pData;

                oWriter.WriteString(L"<a:cubicBezTo>");

                oWriter.WriteString(L"<a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX1));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY1));
                oWriter.WriteString(L"\"/>");
                oWriter.WriteString(L"<a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX2));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY2));
                oWriter.WriteString(L"\"/>");
                oWriter.WriteString(L"<a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX3));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY3));
                oWriter.WriteString(L"\"/>");

                oWriter.WriteString(L"</a:cubicBezTo>");

                nCurves++;
                nCount -= 7;
                break;
            }
            case CVectorGraphics::vgtClose:
            default:
                --nCount;
                break;
            }
        }
        oWriter.WriteString(L"<a:close/>");
        oWriter.WriteString(L"</a:path>");

        m_strPath = oWriter.GetData();

        DetermineGraphicsType(dWidth, dHeight, nPeacks, nCurves);

        oWriter.ClearNoAttack();
    }

    void CShape::DetermineGraphicsType(double dWidth, double dHeight,size_t nPeacks, size_t nCurves)
    {
        //note параллельно для каждой текстовой строки создается шейп, который содержит цвет фона для данного текста.
        if ((m_bIsNoStroke && m_bIsNoFill) ||
            (m_oBrush.Color1 == c_iWhiteColor && m_oPen.Color == c_iWhiteColor))
        {
            m_eGraphicsType = eGraphicsType::gtNoGraphics;
        }
        else if ((nPeacks == 5 || nPeacks == 2) && !nCurves) //1 move + 4 Peacks или 2 Peacks
        {
            m_eGraphicsType = eGraphicsType::gtRectangle;

            if (dWidth > 2.0) //note длинное тире - 2.8mm у times new roman
            {
                m_eSimpleLineType = eSimpleLineType::sltLongDash;
            }
            else if (dWidth > 0.7) //минимальное тире - 0.75mm у dotDotDash
            {
                m_eSimpleLineType = eSimpleLineType::sltDash;
            }
            else //максимальна точка - 0.5mm
            {
                m_eSimpleLineType = eSimpleLineType::sltDot;
            }
        }
        else if (nCurves > 0 &&  nPeacks <= 1) //1 move
        {
            m_eGraphicsType = eGraphicsType::gtCurve;
            m_eSimpleLineType = eSimpleLineType::sltWave;
        }
        else if (nCurves > 0 &&  nPeacks > 1)
        {
            m_eGraphicsType = eGraphicsType::gtComplicatedFigure;
        }
    }

    bool CShape::IsItFitLine()
    {
        return (m_eGraphicsType == eGraphicsType::gtRectangle && (m_eSimpleLineType == eSimpleLineType::sltDot || m_eSimpleLineType == eSimpleLineType::sltDash || m_eSimpleLineType == eSimpleLineType::sltLongDash)) ||
               (m_eGraphicsType == eGraphicsType::gtCurve &&  m_eSimpleLineType == eSimpleLineType::sltWave);
    }

    bool CShape::IsCorrelated(const CShape *pShape)
    {
        return m_eGraphicsType == pShape->m_eGraphicsType;
    }

    void CShape::ChangeGeometryOfDesiredShape(CShape *pShape)
    {
        if (!pShape)
        {
            return;
        }

        CShape* pModObject;
        CShape* pDataObject;

        if (pShape->m_bIsNotNecessaryToUse)
        {
            pModObject = this;
            pDataObject = pShape;
        }
        else if (m_bIsNotNecessaryToUse)
        {
            pModObject = pShape;
            pDataObject = this;
        }
        else
        {
            return;
        }

        double dModRight = pModObject->m_dLeft + pModObject->m_dWidth;
        double dDataRight = pDataObject->m_dLeft + pDataObject->m_dWidth;
        double dModBottom = pModObject->m_dTop + pModObject->m_dHeight;
        double dDataBottom = pDataObject->m_dTop + pDataObject->m_dHeight;

        if (pModObject->m_dTop == pDataObject->m_dTop ||
            (pModObject->m_dTop < pDataObject->m_dTop && pModObject->m_dHeight > pDataObject->m_dHeight) ||
            (pModObject->m_dTop > pDataObject->m_dTop && pModObject->m_dHeight < pDataObject->m_dHeight))
        {
            pModObject->m_dHeight = std::max(pModObject->m_dHeight, pDataObject->m_dHeight);
        }
        else if (pModObject->m_dTop < pDataObject->m_dTop)
        {
            pModObject->m_dHeight += pDataObject->m_dHeight + pDataObject->m_dTop - dModBottom;
        }
        else
        {
            pModObject->m_dHeight += pDataObject->m_dHeight + dDataBottom - pModObject->m_dTop;
        }

        if (pModObject->m_dLeft == pDataObject->m_dLeft ||
            (pModObject->m_dLeft < pDataObject->m_dLeft && dModRight > dDataRight) ||
            (pModObject->m_dLeft > pDataObject->m_dLeft && dModRight < dDataRight))
        {
            pModObject->m_dWidth = std::max(pModObject->m_dWidth, pDataObject->m_dWidth);
        }
        else if (pModObject->m_dLeft < pDataObject->m_dLeft)
        {
            pModObject->m_dWidth += pDataObject->m_dWidth + pDataObject->m_dLeft - dModRight;
        }
        else
        {
            pModObject->m_dWidth += pDataObject->m_dWidth + dDataRight - pModObject->m_dLeft;
        }

        //note m_dWidth иногда меняет знак на "-"
        pModObject->m_dHeight = fabs(pModObject->m_dHeight);
        pModObject->m_dWidth = fabs(pModObject->m_dWidth);
        pModObject->m_dLeft = std::min(pModObject->m_dLeft, pDataObject->m_dLeft);
        pModObject->m_dTop = std::min(pModObject->m_dTop, pDataObject->m_dTop);
        pModObject->m_dBaselinePos = pModObject->m_dTop + pModObject->m_dHeight;
        pModObject->m_dRight = pModObject->m_dLeft + pModObject->m_dWidth;
    }

    void CShape::DetermineLineType(CShape *pShape, bool bIsLast)
    {
        if (!pShape)
        {
            //Если нашелся один шейп в линии
            if (m_eLineType == eLineType::ltUnknown && m_eSimpleLineType == eSimpleLineType::sltLongDash)
            {
                m_eLineType = m_dHeight > 0.3 ? eLineType::ltThick : eLineType::ltSingle;
            }
            else if (m_eLineType == eLineType::ltUnknown && m_eSimpleLineType == eSimpleLineType::sltWave)
            {
                m_eLineType = m_oPen.Size > 0.3 ? eLineType::ltWavyHeavy : eLineType::ltWave;
            }
            return;
        }

        if (!IsItFitLine() || !pShape->IsItFitLine() || !IsCorrelated(pShape) ||
            fabs(m_dHeight - pShape->m_dHeight) > c_dGRAPHICS_ERROR_IN_LINES_MM) //линия должна быть одного размера по высоте
        {
            return;
        }

        //Проверка на двойную линию
        if (m_eLineType == eLineType::ltDouble || m_eLineType == eLineType::ltWavyDouble)
        {
            if (m_eLineType == eLineType::ltDouble)
            {
                if (m_dTop < pShape->m_dTop)
                {
                    m_eLineType = eLineType::ltDouble;
                    pShape->m_bIsNotNecessaryToUse = true;
                    ChangeGeometryOfDesiredShape(pShape);
                }
                else
                {
                    pShape->m_eLineType = eLineType::ltDouble;
                    m_bIsNotNecessaryToUse = true;
                    ChangeGeometryOfDesiredShape(pShape);
                }
            }
            else if (m_eLineType == eLineType::ltWavyDouble)
            {
                if (m_dTop < pShape->m_dTop)
                {
                    m_eLineType = eLineType::ltWavyDouble;
                    pShape->m_bIsNotNecessaryToUse = true;
                    ChangeGeometryOfDesiredShape(pShape);
                }
                else
                {
                    pShape->m_eLineType = eLineType::ltWavyDouble;
                    m_bIsNotNecessaryToUse = true;
                    ChangeGeometryOfDesiredShape(pShape);
                }
            }
            return;
        }
        else if (fabs(m_dTop - pShape->m_dTop) < c_dGRAPHICS_ERROR_IN_LINES_MM * 5 &&
            fabs(m_dWidth - pShape->m_dWidth) < c_dGRAPHICS_ERROR_IN_LINES_MM &&
            fabs(m_dLeft - pShape->m_dLeft) < c_dGRAPHICS_ERROR_IN_LINES_MM)
        {
            //Условие первого определения
            if (m_eSimpleLineType == eSimpleLineType::sltLongDash && pShape->m_eSimpleLineType == eSimpleLineType::sltLongDash)
            {
                if (m_dTop < pShape->m_dTop)
                {
                    m_eLineType = eLineType::ltDouble;
                    pShape->m_bIsNotNecessaryToUse = true;
                    ChangeGeometryOfDesiredShape(pShape);
                }
                else
                {
                    pShape->m_eLineType = eLineType::ltDouble;
                    m_bIsNotNecessaryToUse = true;
                    ChangeGeometryOfDesiredShape(pShape);
                }
            }
            else if (m_eSimpleLineType == eSimpleLineType::sltWave && pShape->m_eSimpleLineType == eSimpleLineType::sltWave)
            {
                if (m_dTop < pShape->m_dTop)
                {
                    m_eLineType = eLineType::ltWavyDouble;
                    pShape->m_bIsNotNecessaryToUse = true;
                    ChangeGeometryOfDesiredShape(pShape);
                }
                else
                {
                    pShape->m_eLineType = eLineType::ltWavyDouble;
                    m_bIsNotNecessaryToUse = true;
                    ChangeGeometryOfDesiredShape(pShape);
                }
            }
            return;
        }
        else if (fabs(m_dTop - pShape->m_dTop) > c_dGRAPHICS_ERROR_IN_LINES_MM)
        {
           //все должно быть на одной линии
            return;
        }

        //Теперь считаем, что графика находится на одной линии
        if (fabs(m_dLeft + m_dWidth - pShape->m_dLeft) > c_dGRAPHICS_ERROR_IN_LINES_MM * 5)
        {
            //расстояние между объектами на одной линии должно быть небольшим
            if (m_eLineType == eLineType::ltUnknown && m_eSimpleLineType == eSimpleLineType::sltLongDash)
            {
                m_eLineType = m_dHeight > 0.3 ? eLineType::ltThick : eLineType::ltSingle;
            }
            else if (m_eLineType == eLineType::ltUnknown && m_eSimpleLineType == eSimpleLineType::sltWave)
            {
                m_eLineType = m_oPen.Size > 0.3 ? eLineType::ltWavyHeavy : eLineType::ltWave;
            }
            return;
        }

        if (bIsLast)
        {
            //note Если имеем всего 2 шейпа в линии, то нужно специально определять тип
            if (m_eLineType == eLineType::ltUnknown)
            {
                switch (m_eSimpleLineType)
                {
                case eSimpleLineType::sltDot:
                    if (pShape->m_eSimpleLineType == eSimpleLineType::sltDot)
                    {
                        m_eLineType = m_dHeight > 0.3 ? eLineType::ltDottedHeavy : eLineType::ltDotted;
                    }
                    break;

                case eSimpleLineType::sltDash:
                    if (pShape->m_eSimpleLineType == eSimpleLineType::sltDash)
                    {
                        m_eLineType = m_dHeight > 0.3 ? eLineType::ltDashedHeavy : eLineType::ltDash;
                    }
                    else if (pShape->m_eSimpleLineType == eSimpleLineType::sltDot)
                    {
                        m_eLineType = m_dHeight > 0.3 ? eLineType::ltDashDotHeavy : eLineType::ltDotDash;
                    }
                    break;

                case eSimpleLineType::sltLongDash:
                    if (fabs(m_dLeft + m_dWidth - pShape->m_dLeft) < 0.7)
                    {
                        m_eLineType = m_dHeight > 0.3 ? eLineType::ltThick : eLineType::ltSingle;
                    }
                    else
                    {
                        m_eLineType = m_dHeight > 0.3 ? eLineType::ltDashLongHeavy : eLineType::ltDashLong;
                    }
                    break;

                case eSimpleLineType::sltWave:
                    if (pShape->m_eSimpleLineType == eSimpleLineType::sltWave)
                    {
                        m_eLineType = m_oPen.Size > 0.3 ? eLineType::ltWavyHeavy : eLineType::ltWave;
                    }
                    break;
                default:
                    break;
                }
            }

            pShape->m_bIsNotNecessaryToUse = true;
            ChangeGeometryOfDesiredShape(pShape);
            return;
        }

        bool bIsConditionPassed = false;

        switch (m_eSimpleLineType)
        {
        case eSimpleLineType::sltDot:
            if (pShape->m_eSimpleLineType == eSimpleLineType::sltDot)
            {
                if ((m_eLineType == eLineType::ltUnknown || m_eLineType == eLineType::ltDotted ||
                     m_eLineType == eLineType::ltDottedHeavy) && pShape->m_eLineType == eLineType::ltUnknown)
                {
                    m_eLineType = m_dHeight > 0.3 ? eLineType::ltDottedHeavy : eLineType::ltDotted;
                    bIsConditionPassed = true;
                }
                else if ((m_eLineType == eLineType::ltDotDash || m_eLineType == eLineType::ltDashDotHeavy ||
                          m_eLineType == eLineType::ltDotDotDash || m_eLineType == eLineType::ltDashDotDotHeavy) &&
                         pShape->m_eLineType == eLineType::ltUnknown)
                {
                    m_eLineType = m_dHeight > 0.3 ? eLineType::ltDashDotDotHeavy : eLineType::ltDotDotDash;
                    m_eSimpleLineType = eSimpleLineType::sltDot;
                    bIsConditionPassed = true;
                }
            }
            else if (pShape->m_eSimpleLineType == eSimpleLineType::sltDash)
            {
                if ((m_eLineType == eLineType::ltDotDash || m_eLineType == eLineType::ltDashDotHeavy) &&
                        pShape->m_eLineType == eLineType::ltUnknown)
                {
                    m_eSimpleLineType = eSimpleLineType::sltDash;
                    bIsConditionPassed = true;
                }
                else if ((m_eLineType == eLineType::ltDotDotDash || m_eLineType == eLineType::ltDashDotDotHeavy) &&
                         pShape->m_eLineType == eLineType::ltUnknown)
                {
                    m_eSimpleLineType = eSimpleLineType::sltDash;
                    bIsConditionPassed = true;
                }
            }
            break;
        case eSimpleLineType::sltDash:
            if (pShape->m_eSimpleLineType == eSimpleLineType::sltDash)
            {
                if ((m_eLineType == eLineType::ltUnknown || m_eLineType == eLineType::ltDash ||
                     m_eLineType == eLineType::ltDashedHeavy) && pShape->m_eLineType == eLineType::ltUnknown)
                {
                    m_eLineType = m_dHeight > 0.3 ? eLineType::ltDashedHeavy : eLineType::ltDash;
                    bIsConditionPassed = true;
                }
                else if ((m_eLineType == eLineType::ltDotDash || m_eLineType == eLineType::ltDashDotHeavy) &&
                         pShape->m_eLineType == eLineType::ltUnknown)
                {
                    bIsConditionPassed = true;
                }
            }
            else if (pShape->m_eSimpleLineType == eSimpleLineType::sltDot)
            {
                if ((m_eLineType == eLineType::ltUnknown || m_eLineType == eLineType::ltDotDash ||
                     m_eLineType == eLineType::ltDashDotHeavy) && pShape->m_eLineType == eLineType::ltUnknown)
                {
                    m_eLineType = m_dHeight > 0.3 ? eLineType::ltDashDotHeavy : eLineType::ltDotDash;
                    m_eSimpleLineType = eSimpleLineType::sltDot;
                    bIsConditionPassed = true;
                }
                else if ((m_eLineType == eLineType::ltDotDotDash || m_eLineType == eLineType::ltDashDotDotHeavy) &&
                         pShape->m_eLineType == eLineType::ltUnknown)
                {
                    m_eSimpleLineType = eSimpleLineType::sltDot;
                    bIsConditionPassed = true;
                }
            }
            break;

        case eSimpleLineType::sltLongDash:
            if (fabs(m_dLeft + m_dWidth - pShape->m_dLeft) < 0.7 ||
                m_eLineType == eLineType::ltThick || m_eLineType == eLineType::ltSingle)
            {
                m_eLineType = m_dHeight > 0.3 ? eLineType::ltThick : eLineType::ltSingle;
                bIsConditionPassed = true;
            }
            else if ((m_eLineType == eLineType::ltUnknown || m_eLineType == eLineType::ltDashLong ||
                 m_eLineType == eLineType::ltDashLongHeavy) && pShape->m_eLineType == eLineType::ltUnknown)
            {
                m_eLineType = m_dHeight > 0.3 ? eLineType::ltDashLongHeavy : eLineType::ltDashLong;
                bIsConditionPassed = true;
            }
            break;

        case eSimpleLineType::sltWave:
            if ((m_eLineType == eLineType::ltUnknown || m_eLineType == eLineType::ltWave ||
                 m_eLineType == eLineType::ltWavyHeavy || m_eLineType == eLineType::ltWavyDouble) &&
                    pShape->m_eLineType == eLineType::ltUnknown)
            {
                m_eLineType = m_oPen.Size > 0.3 ? eLineType::ltWavyHeavy : eLineType::ltWave;
                bIsConditionPassed = true;
            }
            break;
        default:
            break;
        }

        if (bIsConditionPassed)
        {
            pShape->m_bIsNotNecessaryToUse = true;
            ChangeGeometryOfDesiredShape(pShape);
        }
    }

    void CShape::ToXml(NSStringUtils::CStringBuilder &oWriter)
    {
        //todo для уменьшения размера каждого шейпа ипользовавать только то, что необходимо - для графики, текста, графика+текст
        //todo добавить все возможные параметры/атрибуты

        if (m_bIsNotNecessaryToUse)
        {
            return;
        }
        oWriter.WriteString(L"<w:r>");

        oWriter.WriteString(L"<w:rPr><w:noProof/></w:rPr>"); //отключение проверки орфографии

        oWriter.WriteString(L"<w:drawing>");

        BuildGeneralProperties(oWriter);

        oWriter.WriteString(L"</w:drawing>");

        oWriter.WriteString(L"</w:r>");
    }

    void CShape::BuildGeneralProperties(NSStringUtils::CStringBuilder &oWriter)
    {
        oWriter.WriteString(L"<wp:anchor");
            oWriter.WriteString(L" distT=\"0\""); //Определяет минимальное расстояние, которое должно сохраняться между краем
            oWriter.WriteString(L" distB=\"0\""); //этого объекта рисования и любым последующим текстом в документе, когда
            oWriter.WriteString(L" distL=\"0\""); //этот графический объект объект отображается в содержимом документа.
            oWriter.WriteString(L" distR=\"0\"");
            oWriter.WriteString(L" simplePos=\"0\""); //true/1 Указывает, что этот объект должен быть позиционирован с использованием информации о позиционировании в дочернем элементе simplePos
            oWriter.WriteString(L" relativeHeight=\""); //Определяет относительное упорядочивание по Z всех объектов DrawingML в этом документе.
            oWriter.AddUInt(m_nRelativeHeight);
            oWriter.WriteString(L"\"");
            oWriter.WriteString(L" behindDoc=\""); //позади текста - 1, перед текстом - 0
            oWriter.AddUInt(static_cast<UINT>(m_bIsBehindDoc));
            oWriter.WriteString(L"\"");
            oWriter.WriteString(L" locked=\"0\""); //true/1 Указывает, что местоположение привязки для этого объекта не должно быть изменено во время выполнения, когда приложение редактирует содержимое этого документа.
            oWriter.WriteString(L" layoutInCell=\"0\""); //объект будет позиционирован, как указано, но таблица будет изменена в размерах и/или перемещена в документе, как это необходимо для размещения объекта.
            oWriter.WriteString(L" allowOverlap=\"1\""); //разрешается перекрывать содержимое другого объекта
            oWriter.WriteString(L" hidden=\"0\""); //Определяет, будет ли отображаться данный плавающий объект DrawingML.
        oWriter.WriteString(L">");

        oWriter.WriteString(L"<wp:simplePos x=\"0\" y=\"0\"/>");

        oWriter.WriteString(L"<wp:positionH relativeFrom=\"page\">");
            oWriter.WriteString(L"<wp:posOffset>");
            oWriter.AddInt(static_cast<int>(m_dLeft * c_dMMToEMU));
            oWriter.WriteString(L"</wp:posOffset>");
        oWriter.WriteString(L"</wp:positionH>");

        oWriter.WriteString(L"<wp:positionV relativeFrom=\"page\">");
            oWriter.WriteString(L"<wp:posOffset>");
            oWriter.AddInt(static_cast<int>(m_dTop * c_dMMToEMU));
            oWriter.WriteString(L"</wp:posOffset>");
        oWriter.WriteString(L"</wp:positionV>");

        //координаты конца границы шейпа
        oWriter.WriteString(L"<wp:extent");
        oWriter.WriteString(L" cx=\"");
        oWriter.AddInt(static_cast<int>(m_dWidth * c_dMMToEMU));
        oWriter.WriteString(L"\" cy=\"");
        oWriter.AddInt(static_cast<int>(m_dHeight * c_dMMToEMU));
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>"); //Этот элемент определяет дополнительное расстояние, которое должно быть добавлено к каждому краю изображения, чтобы компенсировать любые эффекты рисования, применяемые к объекту DrawingML

        oWriter.WriteString(L"<wp:wrapNone/>");

        m_nShapeId = GenerateShapeId();

        oWriter.WriteString(L"<wp:docPr id=\"");
        oWriter.AddUInt(m_nShapeId);
        switch (m_eType)
        {
        case eShapeType::stTextBox:
            oWriter.WriteString(L"\" name=\"Text Box ");
            break;
        case eShapeType::stPicture:
            oWriter.WriteString(L"\" name=\"Picture ");
            break;
        case eShapeType::stVectorGraphics:
            oWriter.WriteString(L"\" name=\"Freeform: Shape ");
            break;
        case eShapeType::stVectorTexture:
            oWriter.WriteString(L"\" name=\"Freeform: Texture ");
            break;
        case eShapeType::stCanvas:
            oWriter.WriteString(L"\" name=\"Canvas ");
            break;
        case eShapeType::stGroup:
            oWriter.WriteString(L"\" name=\"Group ");
            break;
        default:
            oWriter.WriteString(L"\" name=\"Shape ");
            break;
        }
        oWriter.AddUInt(m_nShapeId);
        //oWriter.WriteString(L" descr=\"Alt Text!\""); //Коммент к картинке
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"<wp:cNvGraphicFramePr/>");

        BuildSpecificProperties(oWriter);

        oWriter.WriteString(L"</wp:anchor>");
    }

    void CShape::BuildSpecificProperties(NSStringUtils::CStringBuilder &oWriter)
    {
        oWriter.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");

        switch (m_eType)
        {
        case eShapeType::stPicture:
        case eShapeType::stVectorTexture:
            BuildPictureProperties(oWriter);
            break;
        case eShapeType::stCanvas:
            BuildCanvasProperties(oWriter);
            break;
        case eShapeType::stGroup:
            BuildGroupProperties(oWriter);
            break;
        case eShapeType::stTextBox:
        case eShapeType::stVectorGraphics:
        default:
            BuildShapeProperties(oWriter);
            break;
        }

        oWriter.WriteString(L"</a:graphic>");
    }

    void CShape::BuildShapeProperties(NSStringUtils::CStringBuilder &oWriter)
    {
        oWriter.WriteString(L"<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");

        oWriter.WriteString(L"<wps:wsp>");

            oWriter.WriteString(L"<wps:cNvSpPr/>"); //non-visual shape properties. http://officeopenxml.com/drwSp-nvSpPr.php

        oWriter.WriteString(L"<wps:spPr>"); //shape properties. http://officeopenxml.com/drwSp-SpPr.php

        //не работает
        //oWriter.WriteString(L"<wps:style/>"); //shape styles. http://officeopenxml.com/drwSp-styles.php

        BuildGraphicProperties(oWriter);

        oWriter.WriteString(L"</wps:spPr>");

        BuildTextBox(oWriter);

        oWriter.WriteString(L"</wps:wsp>");

        oWriter.WriteString(L"</a:graphicData>");
    }

    void CShape::BuildPictureProperties(NSStringUtils::CStringBuilder &oWriter)
    {
        oWriter.WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");

        oWriter.WriteString(L"<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
            oWriter.WriteString(L"<pic:nvPicPr>");
                oWriter.WriteString(L"<pic:cNvPr id=\"");
                oWriter.AddUInt(m_pImageInfo->m_nId);
                oWriter.WriteString(L"\" name=\"Picture ");
                oWriter.AddUInt(m_pImageInfo->m_nId);
                oWriter.WriteString(L"\"");
                //oWriter.WriteString(L" descr=\"Alt Text!\""); //Коммент к картинке
                oWriter.WriteString(L"/>");
                oWriter.WriteString(L"<pic:cNvPicPr preferRelativeResize=\"0\">");
                    oWriter.WriteString(L"<a:picLocks noChangeArrowheads=\"1\"/>");
                oWriter.WriteString(L"</pic:cNvPicPr>");
            oWriter.WriteString(L"</pic:nvPicPr>");
            oWriter.WriteString(L"<pic:blipFill>");
                oWriter.WriteString(L"<a:blip r:embed=\"rId");
                oWriter.AddUInt(c_iStartingIdForImages + m_pImageInfo->m_nId);
                oWriter.WriteString(L"\">");
                    oWriter.WriteString(L"<a:alphaModFix/>");
                oWriter.WriteString(L"</a:blip>");
                oWriter.WriteString(L"<a:srcRect/>");
                oWriter.WriteString(L"<a:stretch>");
                    oWriter.WriteString(L"<a:fillRect/>");
                oWriter.WriteString(L"</a:stretch>");
            oWriter.WriteString(L"</pic:blipFill>");
            oWriter.WriteString(L"<pic:spPr bwMode=\"auto\">");

            BuildGraphicProperties(oWriter);

            oWriter.WriteString(L"</pic:spPr>");
        oWriter.WriteString(L"</pic:pic>");

        oWriter.WriteString(L"</a:graphicData>");
    }

    void CShape::BuildGroupProperties(NSStringUtils::CStringBuilder &oWriter)
    {
        oWriter.WriteString(L"<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\">");
            oWriter.WriteString(L"<wpg:wgp>");
                oWriter.WriteString(L"<wpg:cNvGrpSpPr/>");
                oWriter.WriteString(L"<wpg:grpSpPr>");
                    BuildGraphicProperties(oWriter);
                oWriter.WriteString(L"</wpg:grpSpPr>");

                //todo довабить любое количество элементов в группе
                BuildPictureProperties(oWriter);
                BuildShapeProperties(oWriter);

                oWriter.WriteString(L"</wps:wsp>");
            oWriter.WriteString(L"</wpg:wgp>");
        oWriter.WriteString(L"</a:graphicData>");
    }

    void CShape::BuildCanvasProperties(NSStringUtils::CStringBuilder &oWriter)
    {
        //todo добавить реализацию
        oWriter.WriteString(L"<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\">");
    }

    void CShape::BuildGraphicProperties(NSStringUtils::CStringBuilder &oWriter)
    {
        //отвечает за размеры прямоугольного фрейма шейпа
        oWriter.WriteString(L"<a:xfrm");
        if (fabs(m_dRotate) > 0.01)
        {
            oWriter.WriteString(L" rot=\"");
            oWriter.AddInt(static_cast<int>(m_dRotate * c_dDegreeToAngle));
            oWriter.WriteString(L"\"");
        }
        oWriter.WriteString(L">");
            oWriter.WriteString(L"<a:off x=\"0\" y=\"0\"/>");
            oWriter.WriteString(L"<a:ext");
            oWriter.WriteString(L" cx=\"");
            oWriter.AddInt(static_cast<int>(m_dWidth * c_dMMToEMU));
            oWriter.WriteString(L"\" cy=\"");
            oWriter.AddInt(static_cast<int>(m_dHeight * c_dMMToEMU));
            oWriter.WriteString(L"\"/>");
        oWriter.WriteString(L"</a:xfrm>");

        //Если просто текст без графики
        if (m_strPath.empty())
        {
            oWriter.WriteString(L"<a:prstGeom prst=\"rect\">");
                oWriter.WriteString(L"<a:avLst/>");
            oWriter.WriteString(L"</a:prstGeom>");
        }
        else
        {
            //Рисуем сложный путь
            oWriter.WriteString(L"<a:custGeom>");
            oWriter.WriteString(L"<a:avLst/>");
            oWriter.WriteString(L"<a:gdLst/>");
            oWriter.WriteString(L"<a:ahLst/>");
            oWriter.WriteString(L"<a:cxnLst/>");
            oWriter.WriteString(L"<a:rect l=\"l\" t=\"t\" r=\"r\" b=\"b\"/>");
            oWriter.WriteString(L"<a:pathLst>");
            oWriter.WriteString(m_strPath);
            oWriter.WriteString(L"</a:pathLst>");
            oWriter.WriteString(L"</a:custGeom>");
        }

        if (m_bIsNoFill)
        {
            //Нет заливки
            oWriter.WriteString(L"<a:noFill/>");
        }
        else
        {
            //Есть заливка
            oWriter.WriteString(L"<a:solidFill>");
            oWriter.WriteString(L"<a:srgbClr val=\"");
            oWriter.WriteHexInt3(static_cast<int>(ConvertColorBGRToRGB(m_oBrush.Color1)));
            if (0xFF != m_oBrush.TextureAlpha)
            {
                oWriter.WriteString(L"\"><a:alpha val=\"");
                oWriter.AddInt(static_cast<int>(m_oBrush.TextureAlpha / 255.0 * 100.0));
                oWriter.WriteString(L"%\"/></a:srgbClr>");
            }
            else
            {
                oWriter.WriteString(L"\"/>");
            }
            oWriter.WriteString(L"</a:solidFill>");
        }

        if (m_bIsNoStroke)
        {
            oWriter.WriteString(L"<a:ln>"); // w - width по умолчанию 0.75pt = 9525
                oWriter.WriteString(L"<a:noFill/>");
            oWriter.WriteString(L"</a:ln>");
        }
        else
        {
            oWriter.WriteString(L"<a:ln w=\"");
            //todo Некоторые m_oPen.Size приходят увеличенными в 10 раз
            oWriter.AddInt(static_cast<int>(m_oPen.Size * c_dMMToEMU)); //note можно писать в мм
            oWriter.WriteString(L"\">");

            oWriter.WriteString(L"<a:solidFill>");

            oWriter.WriteString(L"<a:srgbClr val=\"");
            oWriter.WriteHexInt3(static_cast<int>(ConvertColorBGRToRGB(m_oPen.Color))); //note можно вместо цвета использовать слова типа "black"
            if (0xFF != m_oPen.Alpha)
            {
                oWriter.WriteString(L"\"><a:alpha val=\"");
                oWriter.AddInt(static_cast<int>(m_oPen.Alpha / 255.0 * 100.0));
                oWriter.WriteString(L"%\"/></a:srgbClr>");
            }
            else
            {
                oWriter.WriteString(L"\"/>");
            }

            oWriter.WriteString(L"</a:solidFill>");
            oWriter.WriteString(L"</a:ln>");
        }
    }

    void CShape::BuildTextBox(NSStringUtils::CStringBuilder &oWriter)
    {
        if (m_eType == eShapeType::stTextBox && !m_arParagraphs.empty())
        {
            oWriter.WriteString(L"<wps:txbx>"); //text within the shape. http://officeopenxml.com/drwSp-text.php
            oWriter.WriteString(L"<w:txbxContent>");
            for (size_t i = 0; i < m_arParagraphs.size(); i++)
            {
                m_arParagraphs[i]->ToXml(oWriter);
            }
            oWriter.WriteString(L"</w:txbxContent>");
            oWriter.WriteString(L"</wps:txbx>");

            oWriter.WriteString(L"<wps:bodyPr"); //определяет свойства тела текста внутри фигуры
                oWriter.WriteString(L" rot=\"0\""); //Определяет поворот, который применяется к тексту в пределах ограничивающей рамки.
                oWriter.WriteString(L" spcFirstLastPara=\"0\""); //должен ли соблюдаться интервал между абзацами до и после, заданный пользователем.
                oWriter.WriteString(L" vertOverflow=\"overflow\""); //может ли текст выходить за пределы ограничительной рамки по вертикали
                oWriter.WriteString(L" horzOverflow=\"overflow\""); //может ли текст выходить за пределы ограничительной рамки по горизонтали.
                oWriter.WriteString(L" vert=\"horz\"");
                //oWriter.WriteString(L" wrap=\"none\""); //граница шейпа по ширине текста
                oWriter.WriteString(L" wrap=\"square\""); //Определяет параметры обертки, которые будут использоваться для данного текстового тела.
                //на сколько граница текста отступает от границы шейпа
                oWriter.WriteString(L" lIns=\"0\""); //left   по умолчанию 0.25см = 91440
                oWriter.WriteString(L" tIns=\"0\""); //top    по умолчанию 0.13см = 45720
                oWriter.WriteString(L" rIns=\"0\""); //right  по умолчанию 0.25см
                oWriter.WriteString(L" bIns=\"0\""); //bottom по умолчанию 0.13см
                oWriter.WriteString(L" numCol=\"1\""); //Определяет количество колонок текста в ограничивающем прямоугольнике.
                oWriter.WriteString(L" spcCol=\"0\""); //Определяет пространство между колонками текста в текстовой области (только если numCol >1)
                oWriter.WriteString(L" rtlCol=\"0\""); //используются ли столбцы в порядке справа налево (true) или слева направо (false).
                oWriter.WriteString(L" fromWordArt=\"0\""); //true/1 текст в этом текстовом поле является преобразованным текстом из объекта WordArt.
                oWriter.WriteString(L" anchor=\"t\""); //Вертикальное выравнивание текста в шейпе (t - top, b - bottom, ctr - middle) по умолчанию top
                oWriter.WriteString(L" anchorCtr=\"0\""); //true/1 Определяет центрирование текстового поля.
                oWriter.WriteString(L" forceAA=\"0\""); //true/1 Заставляет текст отображаться сглаженным независимо от размера шрифта.
                oWriter.WriteString(L" compatLnSpc=\"1\""); //межстрочный интервал для данного текста определяется упрощенным способом с помощью сцены шрифта.
            oWriter.WriteString(L">");

            oWriter.WriteString(L"<a:prstTxWarp prst=\"textNoShape\">");
                oWriter.WriteString(L"<a:avLst/>");
            oWriter.WriteString(L"</a:prstTxWarp>");
            oWriter.WriteString(L"<a:noAutofit/>");
            oWriter.WriteString(L"</wps:bodyPr>");
        }
        else
        {
            oWriter.WriteString(L"<wps:bodyPr/>");
        }
    }
}; // namespace NSDocxRenderer
