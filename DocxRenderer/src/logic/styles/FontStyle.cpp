#include "FontStyle.h"
#include "../../resources/Constants.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
	CFontStyle::CFontStyle()
	{
		static LONG lId = 0;
		lId++;
		m_wsFontStyleId = m_wsIdStart;

		if(lId < 10)
			m_wsFontStyleId += L"0" + std::to_wstring(lId);
		else
			m_wsFontStyleId += std::to_wstring(lId);
	}
	CFontStyle::CFontStyle(const CFontStyle& oFontStyle) : CFontStyle()
	{
		*this = oFontStyle;
	}
	CFontStyle::~CFontStyle()
	{
	}

	CFontStyle& CFontStyle::operator=(const CFontStyle& oSrc)
	{
		if (this == &oSrc)
			return *this;

		m_dFontSize = oSrc.m_dFontSize;
		m_oBrush = oSrc.m_oBrush;

		m_wsFontName = oSrc.m_wsFontName;
		m_bBold = oSrc.m_bBold;
		m_bItalic = oSrc.m_bItalic;
		return *this;
	}
	bool CFontStyle::operator==(const CFontStyle& oSrc)
	{
		bool bIf1 = m_oBrush.Type == oSrc.m_oBrush.Type;
		bool bIf2 = m_oBrush.Color1 == oSrc.m_oBrush.Color1;
		bool bIf3 = m_oBrush.Color2 == oSrc.m_oBrush.Color2;
		bool bIf4 = m_oBrush.Alpha1 == oSrc.m_oBrush.Alpha1;
		bool bIf5 = m_oBrush.Alpha2 == oSrc.m_oBrush.Alpha2;
		bool bIf6 = m_oBrush.LinearAngle == oSrc.m_oBrush.LinearAngle;

		bool bIf7 = m_dFontSize == oSrc.m_dFontSize;
		bool bIf8 = m_wsFontName == oSrc.m_wsFontName;
		bool bIf9 = (m_bItalic == oSrc.m_bItalic) && (m_bBold == oSrc.m_bBold);

		//todo
		//        (TexturePath == pBrush->TexturePath) && (TextureAlpha == pBrush->TextureAlpha) && (TextureMode == pBrush->TextureMode) &&
		//        (Rectable == pBrush->Rectable) && (Rect.Equals(pBrush->Rect)));
		//bool bIf7 = m_oBrush.IsEqual(&oSrc->m_oBrush);

		return (bIf1 && bIf2 && bIf3 && bIf4 && bIf5 && bIf6 &&
				bIf7 && bIf8 && bIf9);
	}

	const std::wstring& CFontStyle::GetFontStyleId() const noexcept
	{
		return m_wsFontStyleId;
	}
	const std::wstring& CFontStyle::GetFontName() const noexcept
	{
		return m_wsFontName;
	}
	const NSStructures::CBrush& CFontStyle::GetBrush() const noexcept
	{
		return m_oBrush;
	}

	double CFontStyle::GetFontSize() const noexcept
	{
		return m_dFontSize;
	}
	bool CFontStyle::IsBold() const noexcept
	{
		return m_bBold;
	}
	bool CFontStyle::IsItalic() const noexcept
	{
		return m_bItalic;
	}

	void CFontStyle::SetFontName(const std::wstring& wsFontName)
	{
		m_wsFontName = wsFontName;
	}
	void CFontStyle::SetBrush(const NSStructures::CBrush& oBrush)
	{
		m_oBrush = oBrush;
	}

	void CFontStyle::SetFontSize(double dFontSize)
	{
		m_dFontSize = dFontSize;
	}
	void CFontStyle::SetBold(bool bBold)
	{
		m_bBold = bBold;
	}
	void CFontStyle::SetItalic(double bItalic)
	{
		m_bItalic = bItalic;
	}

	void CFontStyle::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		oWriter.WriteString(L"<w:style");
		oWriter.WriteString(L" w:type=\"character\"");
		oWriter.WriteString(L" w:customStyle=\"1\"");
		oWriter.WriteString(L" w:styleId=\"");
		oWriter.WriteString(m_wsFontStyleId);
		oWriter.WriteString(L"\">");

		oWriter.WriteString(L"<w:name w:val=\"");
		oWriter.WriteString(m_wsFontStyleId);
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"<w:basedOn w:val=\"");
		oWriter.WriteString(L"DefaultParagraphFont"); //todo сделать дерево зависимостей
		oWriter.WriteString(L"\"/>");

		//oWriter.WriteString(L"<w:rsid w:val=\"00BB76B0\"/>");

		oWriter.WriteString(L"<w:rPr>");

		oWriter.WriteString(L"<w:rFonts w:ascii=\"");
		oWriter.WriteEncodeXmlString(m_wsFontName);
		oWriter.WriteString(L"\" w:hAnsi=\"");
		oWriter.WriteEncodeXmlString(m_wsFontName);
		oWriter.WriteString(L"\" w:cs=\"");
		oWriter.WriteEncodeXmlString(m_wsFontName);
		oWriter.WriteString(L"\" w:hint=\"default\"/>");

		if (m_bBold)
		{
			oWriter.WriteString(L"<w:b/>");
			oWriter.WriteString(L"<w:bCs/>");
		}
		if (m_bItalic)
		{
			oWriter.WriteString(L"<w:i/>");
			oWriter.WriteString(L"<w:iCs/>");
		}

		if (ConvertColorBGRToRGB(m_oBrush.Color1) != c_iBlackColor2)
		{
			oWriter.WriteString(L"<w:color w:val=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_oBrush.Color1));
			oWriter.WriteString(L"\"/>");
		}

		int lSize = static_cast<int>(2 * m_dFontSize);
		oWriter.WriteString(L"<w:sz w:val=\"");
		oWriter.AddInt(lSize);
		oWriter.WriteString(L"\"/><w:szCs w:val=\"");
		oWriter.AddInt(lSize);
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"</w:rPr>");
		oWriter.WriteString(L"</w:style>");
	}
}
