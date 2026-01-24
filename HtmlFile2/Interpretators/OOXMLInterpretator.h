#ifndef OOXMLINTERPRETATOR_H
#define OOXMLINTERPRETATOR_H

// #include "HTMLInterpretator.h"
#include "../DesktopEditor/common/StringBuilder.h"

#include "../Common/3dParty/html/css/src/xhtml/CDocumentStyle.h"
#include "../Common/3dParty/html/css/src/CCssCalculator.h"
#include "HTMLInterpretator.h"

namespace HTML
{
using XmlString = NSStringUtils::CStringBuilder;

struct TImageData;
class COOXMLInterpretator : public IHTMLInterpretator
{
	XmlString m_oStylesXml;   // styles.xml
	XmlString m_oDocXmlRels;  // document.xml.rels
	XmlString m_oNoteXmlRels; // footnotes.xml.rels
	XmlString m_oDocXml;      // document.xml
	XmlString m_oNoteXml;     // footnotes.xml
	XmlString m_oNumberXml;   // numbering.xml
	XmlString m_oWebSettings; // webSettings.xml

	XmlString *m_pCurrentDocument; //Текущее место записи

	NSCSS::CDocumentStyle m_oXmlStyle;      // Ooxml стиль
	NSCSS::NSProperties::CPage m_oPageData; // Стили страницы

	NSCSS::CCssCalculator *m_pStylesCalculator;

	struct TState
	{
		bool m_bInP;         // <w:p> открыт?
		bool m_bInR;         // <w:r> открыт?
		bool m_bInT;         // <w:t> открыт?
		bool m_bWasPStyle;   // <w:pStyle> записан?
		bool m_bWasSpace;    // Был пробел?
		bool m_bInHyperlink; // <w:hyperlink> открыт?

		bool m_bBanUpdatePageData; // Запретить обновление данных о странице?

		std::wstring m_wsNote;

		// HtmlTag m_eLastElement;

		TState()
			: m_bInP(false), m_bInR(false), m_bInT(false), m_bWasPStyle(false), m_bWasSpace(true), m_bInHyperlink(false), m_bBanUpdatePageData(false)/*, m_eLastElement(HTML_TAG(UNKNOWN))*/
		{}
	} m_oState;

	int m_nFootnoteId;  // ID сноски
	int m_nHyperlinkId; // ID ссылки
	int m_nNumberingId; // ID списка
	int m_nId;          // ID остальные элементы
	int m_nShapeId;     // Id shape's

	std::map<std::wstring, std::wstring> m_mFootnotes; // Сноски
	std::map<std::wstring, UINT>         m_mBookmarks; // Закладки
	using anchors_map = std::map<std::wstring, std::wstring>;
	anchors_map                          m_mAnchors; // Map якорей с индивидуальными id
	std::map<std::wstring, UINT>         m_mDivs;      // Div элементы
public:
	COOXMLInterpretator();

	void SetCSSCalculator(NSCSS::CCssCalculator* pCSSCalculator);

	void Begin(const std::wstring& wsDst, const THtmlParams* pParams) override;
	void End(const std::wstring& wsDst) override;

	bool OpenP();
	bool OpenR();
	bool OpenT();

	void CloseP();
	void CloseR();
	void CloseT();

	void BeginBlock() override;
	void EndBlock(bool bAddBlock) override;

	//Conversion methods
	bool OpenAnchor(const std::vector<NSCSS::CNode>& arSelectors) override;
	void CloseAnchor(const std::vector<NSCSS::CNode>& arSelectors) override;

	void Break(const std::vector<NSCSS::CNode>& arSelectors) override;

	bool OpenAbbreviation(const std::vector<NSCSS::CNode>& arSelectors) override;
	void CloseAbbreviation(const std::vector<NSCSS::CNode>& arSelectors) override;

	bool OpenBold(const std::vector<NSCSS::CNode>& arSelectors) override { return true; };
	void CloseBold(const std::vector<NSCSS::CNode>& arSelectors) override {};

	bool OpenBidirectional(const std::vector<NSCSS::CNode>& arSelectors) override { return true; }; //TODO:: проверить необходимо ли для md
	void CloseBidirectional(const std::vector<NSCSS::CNode>& arSelectors) override {};

	bool OpenItalic(const std::vector<NSCSS::CNode>& arSelectors) override { return true; };
	void CloseItalic(const std::vector<NSCSS::CNode>& arSelectors) override {};

	bool OpenPreformatted(const std::vector<NSCSS::CNode>& arSelectors) override { return true; }; //TODO:: проверить необходимо ли для md
	void ClosePreformatted(const std::vector<NSCSS::CNode>& arSelectors) override {};

	bool OpenKBD(const std::vector<NSCSS::CNode>& arSelectors) override { return true; }; //TODO:: проверить необходимо ли для md
	void CloseKBD(const std::vector<NSCSS::CNode>& arSelectors) override {};

	bool OpenStrikethrough(const std::vector<NSCSS::CNode>& arSelectors) override { return true; }; //TODO:: проверить необходимо ли для md
	void CloseStrikethrough(const std::vector<NSCSS::CNode>& arSelectors) override {};

	bool OpenUnderline(const std::vector<NSCSS::CNode>& arSelectors) override { return true; }; //TODO:: проверить необходимо ли для md
	void CloseUnderline(const std::vector<NSCSS::CNode>& arSelectors) override {};

	bool OpenQuotation(const std::vector<NSCSS::CNode>& arSelectors) override { return true; }; //TODO:: проверить необходимо ли для md
	void CloseQuotation(const std::vector<NSCSS::CNode>& arSelectors) override {}

	bool OpenHeader(const std::vector<NSCSS::CNode>& arSelectors) override { return true; };;
	void CloseHeader(const std::vector<NSCSS::CNode>& arSelectors) override {};

	std::wstring WritePPr(const std::vector<NSCSS::CNode>& arSelectors);
	std::wstring WriteRPr(XmlString& oXml, const std::vector<NSCSS::CNode>& arSelectors);

	bool WriteText(const std::wstring& wsText, const std::vector<NSCSS::CNode>& arSelectors) override;
	void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) override;
	void WriteSpace();

	void WriteEmptyBookmark(const std::wstring& wsId);
	std::wstring WriteBookmark(const std::wstring& wsId);
	std::wstring AddAnchor(const std::wstring& wsAnchorValue);

	void WriteImage(const std::vector<NSCSS::CNode>& arSelectors);
	void WriteAlternativeImage(const std::wstring& wsAlt, const std::wstring& wsSrc, const TImageData& oImageData);
	void WriteEmptyImage(int nWidth, int nHeight, const std::wstring& wsName = L"", const std::wstring& wsDescr = L"");

	std::wstring GetStyle(const NSCSS::CCompiledStyle& oStyle, bool bParagraphStyle);

	void UpdatePageStyle(const std::vector<NSCSS::CNode>& arSelectors);

	XmlString& GetStylesXml();
	XmlString& GetDocRelsXml();
	XmlString& GetNotesRelsXml();
	XmlString& GetDocumentXml();
	XmlString& GetNotesXml();
	XmlString& GetNumberingXml();
	XmlString& GetWebSettingsXml();
	XmlString& GetCurrentDocument();

	#ifdef _DEBUG
	virtual void PrintData() override;
	#endif
};
}

#endif // OOXMLINTERPRETATOR_H
