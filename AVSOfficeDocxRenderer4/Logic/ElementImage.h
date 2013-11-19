#pragma once
#include "Common.h"

namespace NSDocxRenderer
{
	static _bstr_t g_bstr_image_1			= L"<w:r><w:pict><v:shape id=\"\" type=\"\" style=\"position:absolute;";
	static _bstr_t g_bstr_image_2			= L"z-index:-1;mso-position-horizontal-relative:page;mso-position-vertical-relative:page\" filled=\"f\">";
	static _bstr_t g_bstr_image_3			= L"</v:shape></w:pict></w:r>";

	static CString g_string_image_position			= _T("margin-left:%.2lfmm;margin-top:%.2lfmm;width:%.2lfmm;height:%.2lfmm;");
	static CString g_string_image_position_rotate	= _T("margin-left:%.2lfmm;margin-top:%.2lfmm;width:%.2lfmm;height:%.2lfmm;rotation:%d;");
	static CString g_string_image_rid				= _T("<v:imagedata r:id=\"rId%d\" o:title=\"\"/>");

	class CImage : public CBaseItem
	{
	public:
		CString m_strPath;
		LONG	m_lID;

		double m_dLeft;
		double m_dTop;
		double m_dWidth;
		double m_dHeight;

		double m_dRotate;

	public:
		CImage()
		{
			m_eType		= etImage;
			m_strPath	= _T("");
			m_lID		= -1;
		}
		CImage(const CImage& oSrc)
		{
			*this = oSrc;
		}
		CImage(const CImageInfo& oInfo, const CString& strDstMedia)
		{
			m_eType		= etImage;
			m_strPath	= strDstMedia;
			m_lID		= oInfo.m_lID;
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

		virtual void ToXml(NSDocxRenderer::CStringWriter& oWriter)
		{
			oWriter.WriteString(g_bstr_image_1);

			if (0.0 == m_dRotate)
			{
				CString strPosition = _T("");
				strPosition.Format(g_string_image_position, m_dLeft, m_dTop, m_dWidth, m_dHeight);
				oWriter.WriteString(strPosition);
			}
			else
			{
				CString strPosition = _T("");
				strPosition.Format(g_string_image_position_rotate, m_dLeft, m_dTop, m_dWidth, m_dHeight, (int)m_dRotate);
				oWriter.WriteString(strPosition);
			}

			oWriter.WriteString(g_bstr_image_2);

			CString strRid = _T("");
			strRid.Format(g_string_image_rid, 10 + m_lID);

			oWriter.WriteString(strRid);

			oWriter.WriteString(g_bstr_image_3);
		}
	};
}