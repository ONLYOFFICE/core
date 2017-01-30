#ifndef DOCX_RENDERER_ELEMENT_IMAGE_H
#define DOCX_RENDERER_ELEMENT_IMAGE_H

#include "Common.h"

namespace NSDocxRenderer
{
    class CImage : public CBaseItem
	{
	public:
        std::wstring m_strPath;
        int	m_lID;

		double m_dLeft;
		double m_dTop;
		double m_dWidth;
		double m_dHeight;

		double m_dRotate;

	public:
		CImage()
		{
			m_eType		= etImage;
            m_strPath	= L"";
			m_lID		= -1;
		}
		CImage(const CImage& oSrc)
		{
			*this = oSrc;
		}
        CImage(const CImageInfo& oInfo, const std::wstring& strDstMedia)
		{
			m_eType		= etImage;
			m_strPath	= strDstMedia;
            m_lID		= oInfo.m_nId;
		}
		CImage& operator=(const CImage& oSrc)
		{
			m_eType		= etImage;
			m_strPath	= oSrc.m_strPath;
			m_lID		= oSrc.m_lID;

			m_dLeft		= oSrc.m_dLeft;
			m_dTop		= oSrc.m_dTop;
			m_dWidth	= oSrc.m_dWidth;
			m_dHeight	= oSrc.m_dHeight;

			m_dRotate	= oSrc.m_dRotate;

			return *this;
		}

        virtual void ToXml(NSStringUtils::CStringBuilder& oWriter)
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
	};
}

#endif // DOCX_RENDERER_ELEMENT_IMAGE_H
