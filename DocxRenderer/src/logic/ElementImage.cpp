#include "ElementImage.h"

namespace NSDocxRenderer
{
        CImage::CImage() : CBaseItem(etImage)
        {
            m_strPath	= L"";
            m_lID		= -1;
        }
        CImage::CImage(const CImage& oSrc) : CBaseItem(etImage)
        {
            *this = oSrc;
        }
        CImage::CImage(const CImageInfo& oInfo, const std::wstring& strDstMedia) : CBaseItem(etImage)
        {
            m_eType		= etImage;
            m_strPath	= strDstMedia;
            m_lID		= oInfo.m_nId;
        }
        void CImage::Clear(){}

        CImage& CImage::operator=(const CImage& oSrc)
        {
            if (this == &oSrc)
            {
                return *this;
            }

            CBaseItem::operator=(oSrc);

            m_strPath	= oSrc.m_strPath;
            m_lID		= oSrc.m_lID;

            m_dLeft		= oSrc.m_dLeft;
            m_dTop		= oSrc.m_dTop;
            m_dWidth	= oSrc.m_dWidth;
            m_dHeight	= oSrc.m_dHeight;

            m_dRotate	= oSrc.m_dRotate;

            return *this;
        }

        void CImage::ToXml(NSStringUtils::CStringBuilder& oWriter)
        {
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
