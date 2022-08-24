#include "Image.h"
#include "../../resources/Constants.h"
#include <math.h>

namespace NSDocxRenderer
{
        CImage::CImage() : CBaseItem(ElemType::etImage)
        {
        }
        CImage::CImage(const CImageInfo& oInfo, const std::wstring& strDstMedia) : CBaseItem(ElemType::etImage),
            m_oImageInfo(oInfo), m_strPath(strDstMedia)
        {
        }
        void CImage::Clear(){}

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
            oWriter.AddInt(c_iStartingIdForImages + m_oImageInfo.m_nId);
            oWriter.WriteString(L"\" o:title=\"\"/>");

            oWriter.WriteString(L"</v:shape></w:pict></w:r>");
        }
}
