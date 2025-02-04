#include "FontStyle.h"

#include "../../resources/Constants.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
	CFontStyle::CFontStyle()
	{
		static LONG lId = 0;
		lId++;
		wsFontStyleId = m_wsIdStart;

		if(lId < 10)
			wsFontStyleId += L"0" + std::to_wstring(lId);
		else
			wsFontStyleId += std::to_wstring(lId);
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

		dFontSize = oSrc.dFontSize;
		oBrush = oSrc.oBrush;

		wsFontName = oSrc.wsFontName;
		bBold = oSrc.bBold;
		bItalic = oSrc.bItalic;
		return *this;
	}
	bool CFontStyle::operator==(const CFontStyle& oSrc)
	{
		bool bIf1 = oBrush.Type == oSrc.oBrush.Type;
		bool bIf2 = oBrush.Color1 == oSrc.oBrush.Color1;
		bool bIf3 = oBrush.Color2 == oSrc.oBrush.Color2;
		bool bIf4 = oBrush.Alpha1 == oSrc.oBrush.Alpha1;
		bool bIf5 = oBrush.Alpha2 == oSrc.oBrush.Alpha2;
		bool bIf6 = oBrush.LinearAngle == oSrc.oBrush.LinearAngle;

		bool bIf7 = dFontSize == oSrc.dFontSize;
		bool bIf8 = wsFontName == oSrc.wsFontName;
		bool bIf9 = (bItalic == oSrc.bItalic) && (bBold == oSrc.bBold);

		//todo
		//        (TexturePath == pBrush->TexturePath) && (TextureAlpha == pBrush->TextureAlpha) && (TextureMode == pBrush->TextureMode) &&
		//        (Rectable == pBrush->Rectable) && (Rect.Equals(pBrush->Rect)));
		//bool bIf7 = m_oBrush.IsEqual(&oSrc->m_oBrush);

		return (bIf1 && bIf2 && bIf3 && bIf4 && bIf5 && bIf6 &&
				bIf7 && bIf8 && bIf9);
	}

	void CFontStyle::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		oWriter.WriteString(L"<w:style");
		oWriter.WriteString(L" w:type=\"character\"");
		oWriter.WriteString(L" w:customStyle=\"1\"");
		oWriter.WriteString(L" w:styleId=\"");
		oWriter.WriteString(wsFontStyleId);
		oWriter.WriteString(L"\">");

		oWriter.WriteString(L"<w:name w:val=\"");
		oWriter.WriteString(wsFontStyleId);
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"<w:basedOn w:val=\"");
		oWriter.WriteString(L"DefaultParagraphFont"); //todo сделать дерево зависимостей
		oWriter.WriteString(L"\"/>");

		//oWriter.WriteString(L"<w:rsid w:val=\"00BB76B0\"/>");

		oWriter.WriteString(L"<w:rPr>");

		oWriter.WriteString(L"<w:rFonts w:ascii=\"");
		oWriter.WriteEncodeXmlString(wsFontName);
		oWriter.WriteString(L"\" w:hAnsi=\"");
		oWriter.WriteEncodeXmlString(wsFontName);
		oWriter.WriteString(L"\" w:cs=\"");
		oWriter.WriteEncodeXmlString(wsFontName);
		oWriter.WriteString(L"\" w:hint=\"default\"/>");

		if (bBold)
		{
			oWriter.WriteString(L"<w:b/>");
			oWriter.WriteString(L"<w:bCs/>");
		}
		if (bItalic)
		{
			oWriter.WriteString(L"<w:i/>");
			oWriter.WriteString(L"<w:iCs/>");
		}

		if (ConvertColorBGRToRGB(oBrush.Color1) != c_iBlackColor2)
		{
			oWriter.WriteString(L"<w:color w:val=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(oBrush.Color1));
			oWriter.WriteString(L"\"/>");
		}

		int lSize = static_cast<int>(2 * dFontSize);
		oWriter.WriteString(L"<w:sz w:val=\"");
		oWriter.AddInt(lSize);
		oWriter.WriteString(L"\"/><w:szCs w:val=\"");
		oWriter.AddInt(lSize);
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"</w:rPr>");
		oWriter.WriteString(L"</w:style>");
	}
	void CFontStyle::UpdateAvgSpaceWidth(double dWidth)
	{
		dAvgSpaceWidth = (dAvgSpaceWidth / (m_nN + 1)) * m_nN + (dWidth / (m_nN + 1));
		m_nN++;
	}
	double CFontStyle::GetAvgSpaceWidth() const
	{
		return dAvgSpaceWidth;
	}
}
