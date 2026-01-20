#include "HTML2OOXMLInterpretator.h"

#include "../DesktopEditor/common/File.h"
#include <iostream>
#include <ostream>

namespace HTML
{
inline bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors);

CHTML2OOXMLInterpretator::CHTML2OOXMLInterpretator()
	: m_pCurrentDocument(&m_oDocXml), m_nFootnoteId(1), m_nHyperlinkId(1), m_nNumberingId(1), m_nId(1), m_nShapeId(1)
{}

bool CHTML2OOXMLInterpretator::OpenP()
{
	if (m_oState.m_bInP)
		return false;

	m_pCurrentDocument->WriteString(L"<w:p>");
	m_oState.m_bInP = true;
	m_oState.m_bWasPStyle = false;

	return true;
}

bool CHTML2OOXMLInterpretator::OpenR()
{
	if (m_oState.m_bInR)
		return false;

	m_pCurrentDocument->WriteString(L"<w:r>");
	m_oState.m_bInR = true;
	return true;
}

bool CHTML2OOXMLInterpretator::OpenT()
{
	if (m_oState.m_bInT)
		return false;

	m_pCurrentDocument->WriteString(L"<w:t xml:space=\"preserve\">");
	m_oState.m_bInT = true;
	return true;
}

void CHTML2OOXMLInterpretator::CloseP(std::vector<NSCSS::CNode>& arSelectors)
{
	m_oState.m_bWasSpace = true;

	if (!m_oState.m_bInP)
		return;

	CloseT();
	CloseR();

	if (m_oState.m_bInHyperlink)
	{
		if (arSelectors.rend() != std::find_if(arSelectors.rbegin(), arSelectors.rend(), [](const NSCSS::CNode& oNode) { return L"a" == oNode.m_wsName; }))
		{
			m_pCurrentDocument->WriteString(L"</w:hyperlink>");
			m_oState.m_bInHyperlink = false;
		}
	}

	m_pCurrentDocument->WriteString(L"</w:p>");
	m_oState.m_bInP = false;
}

void CHTML2OOXMLInterpretator::CloseR()
{
	if (!m_oState.m_bInR)
		return;

	m_pCurrentDocument->WriteString(L"</w:r>");
	m_oState.m_bInR = false;
}

void CHTML2OOXMLInterpretator::CloseT()
{
	if (!m_oState.m_bInT)
		return;

	m_pCurrentDocument->WriteString(L"</w:t>");
	m_oState.m_bInT = false;
}

void CHTML2OOXMLInterpretator::OpenCrossHyperlink(const std::wstring& wsRef, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
{
	if (!OpenP())
		CloseR();
	else
		WritePPr(arSelectors, oTS);

	m_oState.m_bInHyperlink = true;

	m_pCurrentDocument->WriteString(L"<w:hyperlink w:anchor=\"");
	const size_t nSharp = wsRef.find('#');

	std::wstring wsAnchorValue;

	if(nSharp == std::wstring::npos)
		wsAnchorValue = NSFile::GetFileName(wsRef);
	else
		wsAnchorValue = (wsRef.c_str() + nSharp + 1);

	if (!wsAnchorValue.empty())
		m_pCurrentDocument->WriteEncodeXmlString(AddAnchor(wsAnchorValue));

	m_pCurrentDocument->WriteString(L"\">");
}

void CHTML2OOXMLInterpretator::OpenExternalHyperlink(const std::wstring& wsRef, const std::wstring& wsTooltip, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
{
	if (!OpenP())
		CloseR();
	else
		WritePPr(arSelectors, oTS);

	// Пишем рельсы
	XmlString& oRelationshipXml(m_oDocXmlRels);

	if (*&m_pCurrentDocument == &m_oNoteXml)
		oRelationshipXml = m_oNoteXmlRels;

	oRelationshipXml.WriteString(L"<Relationship Id=\"rHyp");
	oRelationshipXml.WriteString(std::to_wstring(m_nHyperlinkId));
	oRelationshipXml.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"");
	oRelationshipXml.WriteEncodeXmlString(wsRef);
	oRelationshipXml.WriteString(L"\" TargetMode=\"External\"/>");

	m_oState.m_bInHyperlink = true;
	// Пишем в document.xml
	m_pCurrentDocument->WriteString(L"<w:hyperlink w:tooltip=\"");
	m_pCurrentDocument->WriteEncodeXmlString(wsTooltip);
	m_pCurrentDocument->WriteString(L"\" r:id=\"rHyp");
	m_pCurrentDocument->WriteString(std::to_wstring(m_nHyperlinkId++));
	m_pCurrentDocument->WriteString(L"\">");
}

void CHTML2OOXMLInterpretator::CloseCrossHyperlink(std::vector<NSCSS::CNode>& arSelectors, std::wstring wsFootnote, const std::wstring& wsRef)
{
	if (!m_oState.m_bInP)
		return;

	CloseExternalHyperlink();

	bool bFootnote = false;
	if (arSelectors.size() > 1)
	{
		const NSCSS::CNode& oNode = arSelectors[arSelectors.size() - 2];
		bFootnote = oNode.m_wsName == L"p" && oNode.m_wsClass == L"MsoFootnoteText";
	}

	// Сноска
	if (!wsFootnote.empty())
	{
		if (!bFootnote)
		{
			std::wstring sFootnoteID = std::to_wstring(m_nFootnoteId++);
			OpenR();
			m_pCurrentDocument->WriteString(L"<w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
			m_pCurrentDocument->WriteString(sFootnoteID);
			m_pCurrentDocument->WriteString(L"\"/>");
			CloseR();
			m_mFootnotes.insert(std::make_pair(wsFootnote, sFootnoteID));
		}
		else
		{
			OpenR();
			m_pCurrentDocument->WriteString(L"<w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteRef/>");
			CloseR();
		}
	}
}

void CHTML2OOXMLInterpretator::CloseExternalHyperlink()
{
	if (!m_oState.m_bInP && !m_oState.m_bInHyperlink)
		return;

	CloseT();
	CloseR();
	m_pCurrentDocument->WriteString(L"</w:hyperlink>");
	m_oState.m_bInHyperlink = false;
}

std::wstring CHTML2OOXMLInterpretator::WritePPr(std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
{
	OpenP();

	if (m_oState.m_bWasPStyle)
		return L"";

	std::wstring sPStyle = GetStyle(*arSelectors.back().m_pCompiledStyle, true);

	if (sPStyle.empty() && !ElementInTable(arSelectors))
		sPStyle = L"normal-web";

	if (sPStyle.empty() && oTS.sPStyle.empty() && 0 > oTS.nLi)
		return L"";

	m_oXmlStyle.WriteLitePStyle(oTS.oAdditionalStyle);
	const std::wstring sPSettings = m_oXmlStyle.GetStyle();
	m_oXmlStyle.Clear();

	m_pCurrentDocument->WriteNodeBegin(L"w:pPr");

	if (!sPStyle.empty())
	{
		m_pCurrentDocument->WriteString(L"<w:pStyle w:val=\"");
		m_pCurrentDocument->WriteString(sPStyle);
		m_pCurrentDocument->WriteString(L"\"/>");
	}

	if (oTS.nLi >= 0)
		m_pCurrentDocument->WriteString(L"<w:numPr><w:ilvl w:val=\"" + std::to_wstring(oTS.nLi) + L"\"/><w:numId w:val=\"" +
		                                (!oTS.bNumberingLi ? L"1" : std::to_wstring(m_nNumberingId)) + L"\"/></w:numPr>");

	m_pCurrentDocument->WriteString(oTS.sPStyle + sPSettings);
	m_pCurrentDocument->WriteNodeEnd(L"w:pPr");
	m_oState.m_bWasPStyle = true;

	return sPStyle;
}

void CHTML2OOXMLInterpretator::WriteEmptyParagraph(bool bVahish, bool bInP)
{
	if (!bInP)
		m_pCurrentDocument->WriteString(L"<w:p><w:pPr>");

	m_pCurrentDocument->WriteString(L"<w:rPr><w:rFonts w:eastAsia=\"Times New Roman\"/>");

	if (bVahish)
		m_pCurrentDocument->WriteString(L"<w:sz w:val=\"2\"/><w:szCs w:val=\"2\"/><w:vanish/>");

	m_pCurrentDocument->WriteString(L"</w:rPr>");

	if (!bInP)
		m_pCurrentDocument->WriteString(L"</w:pPr></w:p>");
}

std::wstring CHTML2OOXMLInterpretator::WriteBookmark(const std::wstring& wsId)
{
	const std::wstring sCrossId = std::to_wstring(m_mBookmarks.size() + 1);
	std::wstring sName;

	if (m_mBookmarks.end() != m_mBookmarks.find(wsId))
		sName = wsId + L"_" + std::to_wstring(++m_mBookmarks[wsId]);
	else
	{
		const anchors_map::const_iterator itFound{m_mAnchors.find(wsId)};

		if (m_mAnchors.end() != itFound)
			sName = itFound->second;
		else
			sName = AddAnchor(wsId);

		m_mBookmarks.insert({wsId, 1});
	}

	m_pCurrentDocument->WriteString(L"<w:bookmarkStart w:id=\"");
	m_pCurrentDocument->WriteString(sCrossId);
	m_pCurrentDocument->WriteString(L"\" w:name=\"");
	m_pCurrentDocument->WriteEncodeXmlString(sName);
	m_pCurrentDocument->WriteString(L"\"/>");

	return sCrossId;
}

std::wstring CHTML2OOXMLInterpretator::AddAnchor(const std::wstring& wsAnchorValue)
{
	const anchors_map::iterator itFound = m_mAnchors.find(wsAnchorValue);

	if (m_mAnchors.end() != itFound)
		return itFound->second;

	const std::wstring wsAnchorId{L"anchor-" + std::to_wstring(m_mAnchors.size() + 1)};
	m_mAnchors[wsAnchorValue] = wsAnchorId;

	return wsAnchorId;
}

std::wstring CHTML2OOXMLInterpretator::GetStyle(const NSCSS::CCompiledStyle& oStyle, bool bParagraphStyle)
{
	if ((bParagraphStyle && !m_oXmlStyle.WritePStyle(oStyle)) || (!bParagraphStyle && !m_oXmlStyle.WriteRStyle(oStyle)))
		return L"";

	m_oStylesXml.WriteString(m_oXmlStyle.GetStyle());
	return m_oXmlStyle.GetIdAndClear();
}

XmlString& CHTML2OOXMLInterpretator::GetStylesXml()
{
	return m_oStylesXml;
}

XmlString& CHTML2OOXMLInterpretator::GetDocRelsXml()
{
	return m_oDocXmlRels;
}

XmlString& CHTML2OOXMLInterpretator::GetNotesRelsXml()
{
	return m_oNoteXmlRels;
}

XmlString& CHTML2OOXMLInterpretator::GetDocumentXml()
{
	return m_oDocXml;
}

XmlString& CHTML2OOXMLInterpretator::GetNotesXml()
{
	return m_oNoteXml;
}

XmlString& CHTML2OOXMLInterpretator::GetNumberingXml()
{
	return m_oNumberXml;
}

XmlString& CHTML2OOXMLInterpretator::GetWebSettingsXml()
{
	return m_oWebSettings;
}

XmlString& CHTML2OOXMLInterpretator::GetCurrentDocument()
{
	return *m_pCurrentDocument;
}

void CHTML2OOXMLInterpretator::PrintData()
{
	std::wcout << m_oDocXml.GetData() << std::endl;
}

inline bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors)
{
	return arSelectors.crend() != std::find_if(arSelectors.crbegin(), arSelectors.crend(), [](const NSCSS::CNode& oNode) { return L"table" == oNode.m_wsName; });
}
}
