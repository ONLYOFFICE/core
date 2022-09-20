#include "OldShape.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
    const double COldShape::POSITION_CORRECTION_FOR_X_MM = 3.0;
    const double COldShape::POSITION_CORRECTION_FOR_Y_MM = 2.0;
    const double COldShape::SIZE_CORRECTION_FOR_X_MM = 10.0;
    const double COldShape::SIZE_CORRECTION_FOR_Y_MM = 5.0;

    COldShape::COldShape() : CBaseItem(ElemType::etOldShape)
    {
    }

    COldShape::~COldShape()
    {
        Clear();
    }

    void COldShape::Clear()
    {
        m_arParagraphs.clear();
    }

    void COldShape::GetDataFromVector(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize)
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

        m_lCoordSizeX = lCoordSize;
        m_lCoordSizeY = lCoordSize;

        if (0x00 == (lType & 0x01))
        {
            m_bIsNoStroke = true;
        }
        if (0x00 == (lType >> 8))
        {
            m_bIsNoFill = true;
        }

       WritePath(oVector, lType, lCoordSize);
    }

    void COldShape::WritePath(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize)
    {
        size_t nCount = oVector.GetCurSize();
        double *pData = oVector.m_pData;

        double dWidth = oVector.m_dRight - m_dLeft;
        double dHeight = oVector.m_dBottom - m_dTop;

        NSStringUtils::CStringBuilder oWriter;

        while (nCount > 0)
        {
            CVectorGraphics::VectorGraphicsType eType = static_cast<CVectorGraphics::VectorGraphicsType>((int)(0.5 + *pData++));

            switch (eType)
            {
            case CVectorGraphics::vgtMove:
            {
                LONG lX = static_cast<LONG>((*pData - m_dLeft) * lCoordSize / dWidth);
                ++pData;
                LONG lY = static_cast<LONG>((*pData - m_dTop) * lCoordSize / dHeight);
                ++pData;

                oWriter.AddCharSafe('m');
                oWriter.AddInt(lX);
                oWriter.AddCharSafe(',');
                oWriter.AddInt(lY);

                nCount -= 3;
                break;
            }
            case CVectorGraphics::vgtLine:
            {
                LONG lX = static_cast<LONG>((*pData - m_dLeft) * lCoordSize / dWidth);
                ++pData;
                LONG lY = static_cast<LONG>((*pData - m_dTop) * lCoordSize / dHeight);
                ++pData;

                oWriter.AddCharSafe('l');
                oWriter.AddInt(lX);
                oWriter.AddCharSafe(',');
                oWriter.AddInt(lY);

                nCount -= 3;
                break;
            }
            case CVectorGraphics::vgtCurve:
            {
                LONG lX1 = static_cast<LONG>((*pData - m_dLeft) * lCoordSize / dWidth);
                ++pData;
                LONG lY1 = static_cast<LONG>((*pData - m_dTop) * lCoordSize / dHeight);
                ++pData;

                LONG lX2 = static_cast<LONG>((*pData - m_dLeft) * lCoordSize / dWidth);
                ++pData;
                LONG lY2 = static_cast<LONG>((*pData - m_dTop) * lCoordSize / dHeight);
                ++pData;

                LONG lX3 = static_cast<LONG>((*pData - m_dLeft) * lCoordSize / dWidth);
                ++pData;
                LONG lY3 = static_cast<LONG>((*pData - m_dTop) * lCoordSize / dHeight);
                ++pData;

                oWriter.AddCharSafe('c');
                oWriter.AddInt(lX1);
                oWriter.AddCharSafe(',');
                oWriter.AddInt(lY1);
                oWriter.AddCharSafe(',');
                oWriter.AddInt(lX2);
                oWriter.AddCharSafe(',');
                oWriter.AddInt(lY2);
                oWriter.AddCharSafe(',');
                oWriter.AddInt(lX3);
                oWriter.AddCharSafe(',');
                oWriter.AddInt(lY3);

                nCount -= 7;
                break;
            }
            case CVectorGraphics::vgtClose:
            default:
                oWriter.AddCharSafe('x');
                --nCount;
                break;
            }
        }

        if (0x00 == (lType & 0x01))
        {
            //m_bIsNoStroke = true;
            oWriter.WriteString(L"ns");
        }
        if (0x00 == (lType >> 8))
        {
            //m_bIsNoFill = true;
            oWriter.WriteString(L"nf");
        }

        oWriter.AddCharSafe('e');

        m_strPath = oWriter.GetData();
        oWriter.ClearNoAttack();
    }

    void COldShape::ToXml(NSStringUtils::CStringBuilder &oWriter)
    {
        if (m_bIsNotNecessaryToUse)
        {
            return;
        }

        oWriter.WriteString(
                    L"<w:r><w:pict><v:shape id=\"\" o:spid=\"\" style=\"position:absolute;");

        //oWriter.WriteString(L"left:0;text-align:left;");

        oWriter.WriteString(L"margin-left:");
        oWriter.AddDouble(m_dLeft, 2);
        oWriter.WriteString(L"mm;margin-top:");
        oWriter.AddDouble(m_dTop, 2);
        oWriter.WriteString(L"mm;width:");
        oWriter.AddDouble(m_dWidth, 2);
        oWriter.WriteString(L"mm;height:");
        oWriter.AddDouble(m_dHeight, 2);
        oWriter.WriteString(L"mm;");

        oWriter.WriteString(L"z-index:-1;mso-position-horizontal-relative:page;mso-"
                            L"position-vertical-relative:page;\"");

        oWriter.WriteString(L" coordsize=\"");
        oWriter.AddInt(static_cast<int>(m_lCoordSizeX));
        oWriter.AddCharSafe(',');
        oWriter.AddInt(static_cast<int>(m_lCoordSizeY));
        oWriter.WriteString(L"\" path=\"");
        oWriter.WriteString(m_strPath);
        if (c_BrushTypeTexture == m_oBrush.Type)
        {
            // у нас нет таких шейпов в pdf/xps
            oWriter.WriteString(L"\" fillcolor=\"transparent");
        }
        else
        {
            oWriter.WriteString(L"\" fillcolor=\"#");
            oWriter.WriteHexInt3(static_cast<int>(ConvertColorBGRToRGB(m_oBrush.Color1)));
        }
        oWriter.WriteString(L"\" strokecolor=\"#");
        oWriter.WriteHexInt3(static_cast<int>(ConvertColorBGRToRGB(m_oPen.Color)));
        oWriter.WriteString(L"\" strokeweight=\"");
        oWriter.AddDouble(m_oPen.Size, 2);
        oWriter.WriteString(L"mm\">");

        std::wstring g_string_fill_opacity = L"<v:fill opacity=\"%.2lf\"/>";
        std::wstring g_string_stroke_opacity = L"<v:stroke opacity=\"%.2lf\"/>";

        if (c_BrushTypeTexture == m_oBrush.Type && !m_bIsNoFill)
        {
            oWriter.WriteString(L"<v:imagedata r:id=\"rId");
            oWriter.AddInt(c_iStartingIdForImages + m_lTxId);
            oWriter.WriteString(L"\" o:title=\"\"/>");

            if (0xFF != m_oBrush.TextureAlpha)
            {
                oWriter.WriteString(L"<v:fill opacity=\"");
                oWriter.AddDouble(static_cast<double>(m_oBrush.TextureAlpha / 255.0), 2);
                oWriter.WriteString(L"\"/>");
            }
        }
        else
        {
            if (0xFF != m_oBrush.Alpha1)
            {
                oWriter.WriteString(L"<v:fill opacity=\"");
                oWriter.AddDouble(static_cast<double>(m_oBrush.Alpha1 / 255.0), 2);
                oWriter.WriteString(L"\"/>");
            }
            if (0xFF != m_oPen.Alpha)
            {
                oWriter.WriteString(L"<v:stroke opacity=\"");
                oWriter.AddDouble(static_cast<double>(m_oPen.Alpha / 255.0), 2);
                oWriter.WriteString(L"\"/>");
            }
        }

        oWriter.WriteString(L"<w10:wrap anchorx=\"page\" "
                            L"anchory=\"page\"/>");

        if (!m_arParagraphs.empty())
        {
            oWriter.WriteString(L"<v:textbox><w:txbxContent>");

            for (const auto& pParagraph : m_arParagraphs)
            {
                pParagraph->ToXml(oWriter);
            }
            oWriter.WriteString(L"</w:txbxContent></v:textbox>");
        }

        oWriter.WriteString(L"</v:shape></w:pict></w:r>");
    }
} // namespace NSDocxRenderer
