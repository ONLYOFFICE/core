#include "ElementImage.h"

namespace NSDocxRenderer
{
        CImage::CImage() : CBaseItem(ElemType::etImage)
        {
        }
        CImage::CImage(const CImage& oSrc) : CBaseItem(ElemType::etImage)
        {
            *this = oSrc;
        }
        CImage::CImage(const CImageInfo& oInfo, const std::wstring& strDstMedia) : CBaseItem(ElemType::etImage),
            m_strPath(strDstMedia), m_lID(oInfo.m_nId)
        {
        }
        void CImage::Clear(){}

        CImage& CImage::operator=(const CImage& oSrc)
        {
            if (this == &oSrc)
            {
                return *this;
            }

            Clear();

            CBaseItem::operator=(oSrc);

            m_strPath	= oSrc.m_strPath;
            m_lID		= oSrc.m_lID;

            m_dRotate	= oSrc.m_dRotate;

            return *this;
        }

        void CImage::ToXml(NSStringUtils::CStringBuilder& oWriter)
        {
            if (m_bIsNotNecessaryToUse)
            {
                return;
            }

            oWriter.WriteString(L"<w:r><w:pict><v:shape id=\"\" type=\"\" style=\"position:absolute;");

            oWriter.WriteString(L"margin-left:");
            oWriter.AddDouble(m_dLeft, 2);
            oWriter.WriteString(L"mm;margin-top:");
            oWriter.AddDouble(m_dTop, 2);
            oWriter.WriteString(L"mm;width:");
            oWriter.AddDouble(m_dWidth, 2);
            oWriter.WriteString(L"mm;height:");
            oWriter.AddDouble(m_dHeight, 2);
            oWriter.WriteString(L"mm;");

            if (fabs(m_dRotate) > 0.01)
            {
                oWriter.WriteString(L"rotation:");
                oWriter.AddInt((int)m_dRotate);
                oWriter.AddCharSafe(';');
            }

            oWriter.WriteString(L"z-index:-1;mso-position-horizontal-relative:page;mso-position-vertical-relative:page\" filled=\"f\">");

            oWriter.WriteString(L"<v:imagedata r:id=\"rId");
            oWriter.AddInt(10 + m_lID);
            oWriter.WriteString(L"\" o:title=\"\"/>");

            oWriter.WriteString(L"</v:shape></w:pict></w:r>");
        }
}
