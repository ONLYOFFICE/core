#ifndef OOXMLWRITER_H
#define OOXMLWRITER_H

#include "../../Common/3dParty/html/css/src/xhtml/CDocumentStyle.h"
#include "../../Common/3dParty/html/css/src/CCssCalculator.h"
#include "../HTMLParameters.h"
#include "IWriter.h"

#include <stack>

namespace NSFonts { class IApplicationFonts; }

namespace HTML
{
struct TImageData
{
	UINT m_unWidth;
	UINT m_unHeight;

	int m_nHSpace;
	int m_nVSpace;

	std::wstring m_wsAlign;

	TImageData()
	    : m_unWidth(0), m_unHeight(0), m_nHSpace(0), m_nVSpace(0), m_wsAlign(L"left")
	{}

	bool ZeroSize() const
	{
		return 0 == m_unWidth || 0 == m_unHeight;
	}

	bool ZeroSpaces() const
	{
		return 0 == m_nHSpace && 0 == m_nVSpace;
	}
};

class COOXMLWriter : public IWriter
{
	const std::wstring *m_pDstPath;  // Директория назначения
	const std::wstring *m_pTempDir;  // Temp папка
	const std::wstring *m_pSrcPath;  // Директория источника
	const std::wstring *m_pBasePath; // Полный базовый адрес
	const std::wstring *m_pCorePath; // Путь до корневого файла (используется для работы с Epub)

	XmlString m_oStylesXml;   // styles.xml
	XmlString m_oDocXmlRels;  // document.xml.rels
	XmlString m_oNoteXmlRels; // footnotes.xml.rels
	XmlString m_oDocXml;      // document.xml
	XmlString m_oNoteXml;     // footnotes.xml
	XmlString m_oNumberXml;   // numbering.xml
	XmlString m_oWebSettings; // webSettings.xml

	NSCSS::CDocumentStyle m_oXmlStyle;      // Ooxml стиль
	NSCSS::NSProperties::CPage m_oPageData; // Стили страницы

	NSCSS::CCssCalculator *m_pStylesCalculator;
	THTMLParameters *m_pHTMLParameters;

	struct TState
	{
		bool m_bInP;         // <w:p> открыт?
		bool m_bInR;         // <w:r> открыт?
		bool m_bInT;         // <w:t> открыт?
		bool m_bWasPStyle;   // <w:pStyle> записан?
		bool m_bWasSpace;    // Был пробел?

		bool m_bInHyperlink; // <w:hyperlink> открыт?
		std::wstring m_wsTooltip;
		std::wstring m_wsHref;
		std::wstring m_wsFootnote;
		bool m_bIsFootnote;
		bool m_bISCrossHyperlink;

		XmlString *m_pCurrentDocument; //Текущее место записи
		bool m_bRemoveCurrentDocument;

		TState(XmlString *pCurrentDocument)
			: m_bInP(false), m_bInR(false), m_bInT(false),
			  m_bWasPStyle(false), m_bWasSpace(true), m_bInHyperlink(false),
			  m_bIsFootnote(false), m_bISCrossHyperlink(false),
			  m_pCurrentDocument(pCurrentDocument), m_bRemoveCurrentDocument(false)
		{}

		~TState()
		{
			if (m_bRemoveCurrentDocument && nullptr != m_pCurrentDocument)
				delete m_pCurrentDocument;
		}

		void CreateNewCurrentDocument()
		{
			m_pCurrentDocument = new XmlString();
			m_bRemoveCurrentDocument = true;
		}
	};

	std::stack<TState> m_arStates;

	int m_nFootnoteId;  // ID сноски
	int m_nHyperlinkId; // ID ссылки
	int m_nListId;      // ID списка
	int m_nElementId;   // ID остальные элементы

	bool m_bBanUpdatePageData; // Запретить обновление данных о странице?

	std::stack<std::wstring> m_arDivId;
	bool m_bWasDivs;

	std::map<std::wstring, std::wstring> m_mFootnotes; // Сноски
	std::map<std::wstring, UINT>         m_mBookmarks; // Закладки
	using anchors_map = std::map<std::wstring, std::wstring>;
	anchors_map                          m_mAnchors; // Map якорей с индивидуальными id

	NSFonts::IApplicationFonts*          m_pFonts;     // Необходимо для оптимизации работы со шрифтами
public:
	COOXMLWriter(THTMLParameters* pHTMLParameters = nullptr, NSCSS::CCssCalculator* pCSSCalculator = nullptr);

	void SetSrcDirectory (const std::wstring& wsPath);
	void SetDstDirectory (const std::wstring& wsPath);
	void SetTempDirectory(const std::wstring& wsPath);
	void SetBaseDirectory(const std::wstring& wsPath);
	void SetCoreDirectory(const std::wstring& wsPath);

	void Begin(const std::wstring& wsDst) override;
	void End(const std::wstring& wsDst) override;

	bool OpenP();
	bool OpenR();
	bool OpenT();
	void OpenHyperlink();

	void CloseP();
	void CloseR();
	void CloseT();
	void CloseHyperlink();

	void PageBreak() override;

	void BeginBlock() override;
	void EndBlock(bool bAddBlock) override;

	void SetDataOutput(XmlString* pOutputData) override;
	void RevertDataOutput() override;

	void SaveState();
	void RollBackState();

	void SetCurrentDocument(XmlString* pNewDocument);

	void Break(const std::vector<NSCSS::CNode>& arSelectors);

	void SetHyperlinkData(const std::wstring& wsRef, const std::wstring& wsTooltip, bool bIsCross, const std::wstring& wsFootnote, bool bIsFootnote);
	void ClearHyperlinkData();

	void OpenCrossHyperlink(const std::wstring& wsRef);
	void OpenExternalHyperlink(const std::wstring& wsRef, const std::wstring& wsTooltip);
	void CloseCrossHyperlink(const std::vector<NSCSS::CNode>& arSelectors, std::wstring wsFootnote, const std::wstring& wsRef);
	void CloseExternalHyperlink();

	std::wstring WritePPr(const std::vector<NSCSS::CNode>& arSelectors);
	std::wstring WriteRPr(XmlString& oXml, const std::vector<NSCSS::CNode>& arSelectors);

	bool WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors) override;
	void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) override;
	void WriteSpace();

	void WriteEmptyBookmark(const std::wstring& wsId);
	std::wstring AddLiteBookmark();
	std::wstring WriteBookmark(const std::wstring& wsId);
	std::wstring AddAnchor(const std::wstring& wsAnchorValue);

	void WriteImage(const TImageData& oImageData, const std::wstring& wsId);
	void WriteAlternativeImage(const std::wstring& wsAlt, const std::wstring& wsSrc, const TImageData& oImageData);
	void WriteEmptyImage(int nWidth, int nHeight, const std::wstring& wsName = L"", const std::wstring& wsDescr = L"");
	void WriteImageRels(const std::wstring& wsImageId, const std::wstring& wsImageName);

	std::wstring GetStyle(const NSCSS::CCompiledStyle& oStyle, bool bParagraphStyle);

	void UpdatePageStyle(const std::vector<NSCSS::CNode>& arSelectors);
	void SetBaseFont(const std::wstring& wsFontStyles);
	void SetDivId(const std::wstring& wsDivId);
	void RollBackDivId();

	void IncreaseListId();
	int GetListId() const;

	std::wstring FindFootnote(const std::wstring& wsId);
	void OpenFootnote(const std::wstring& wsFootnoteID);
	void CloseFootnote();

	XmlString& GetStylesXml();
	XmlString& GetDocRelsXml();
	XmlString& GetNotesRelsXml();
	XmlString& GetDocumentXml();
	XmlString& GetNotesXml();
	XmlString& GetNumberingXml();
	XmlString& GetWebSettingsXml();
	XmlString* GetCurrentDocument() const override;

	bool SupportNestedTables() const override;

	const NSCSS::NSProperties::CPage* GetPageData() const;
	NSFonts::IApplicationFonts* GetFonts();

	std::wstring GetMediaDir() const;
	std::wstring GetTempDir()  const;
	std::wstring GetSrcPath()  const;
	std::wstring GetBasePath() const;
	std::wstring GetCorePath() const;
};
}

#endif // OOXMLWRITER_H
