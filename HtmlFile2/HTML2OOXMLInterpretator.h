#ifndef HTML2OOXMLINTERPRETATOR_H
#define HTML2OOXMLINTERPRETATOR_H

// #include "HTMLInterpretator.h"
#include "../DesktopEditor/common/StringBuilder.h"

#include "../Common/3dParty/html/css/src/xhtml/CDocumentStyle.h"
#include "../TextSettings.h"
#include "HTMLInterpretator.h"

namespace HTML
{
using XmlString = NSStringUtils::CStringBuilder;

class CHTML2OOXMLInterpretator : public IHTMLInterpretator
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

	CTextSettings m_oTextSettings;

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
	CHTML2OOXMLInterpretator();

	void Begin(const std::wstring& wsDst, const THtmlParams* pParams) override;
	void End(const std::wstring& wsDst) override;

	bool OpenP();
	bool OpenR();
	bool OpenT();

	void CloseP(std::vector<NSCSS::CNode>& sSelectors);
	void CloseR();
	void CloseT();

	void BeginBlock(std::vector<NSCSS::CNode>& arSelectors) override;
	void EndBlock(bool bAddBlock, std::vector<NSCSS::CNode>& arSelectors) override;

	void OpenCrossHyperlink(const std::wstring& wsRef, std::vector<NSCSS::CNode>& arSelectors);
	void OpenExternalHyperlink(const std::wstring& wsRef, const std::wstring& wsTooltip, std::vector<NSCSS::CNode>& arSelectors);

	void CloseCrossHyperlink(std::vector<NSCSS::CNode>& arSelectors, std::wstring wsFootnote, const std::wstring& wsRef);
	void CloseExternalHyperlink();

	void OpenFldChar(const std::wstring& wsNote);
	void CloseFldChar();

	std::wstring WritePPr(const std::vector<NSCSS::CNode>& arSelectors);
	std::wstring WriteRPr(XmlString& oXml, const std::vector<NSCSS::CNode>& arSelectors);

	bool WriteText(const std::wstring& wsText, const std::vector<NSCSS::CNode>& arSelectors) override;
	void WriteBr(std::vector<NSCSS::CNode>& arSelectors);
	void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) override;
	void WriteSpace();

	void WriteEmptyBookmark(const std::wstring& wsId);
	std::wstring WriteBookmark(const std::wstring& wsId);
	std::wstring AddAnchor(const std::wstring& wsAnchorValue);

	std::wstring GetStyle(const NSCSS::CCompiledStyle& oStyle, bool bParagraphStyle);

	CTextSettings& GetTextSettings();

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

#endif // HTML2OOXMLINTERPRETATOR_H
