#include "MDTags.h"

#include "../Table.h"

#include <boost/tuple/tuple.hpp>

namespace HTML
{
CAnchor<CMDWriter>::CAnchor(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CAnchor<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	m_pWriter->GetCurrentDocument()->AddCharSafe(L'[');
	return true;
}

void CAnchor<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	XmlString* pCurrentDocument{m_pWriter->GetCurrentDocument()};

	pCurrentDocument->AddCharSafe(L']');

	std::wstring wsHref, wsTitle;

	if (!arSelectors.back().GetAttributeValue(L"href", wsHref) ||
	    !arSelectors.back().GetAttributeValue(L"title", wsTitle))
		return;

	pCurrentDocument->WriteString(L'(' + wsHref);

	if (!wsTitle.empty())
		pCurrentDocument->WriteString(L" \"" + wsTitle + L'"');

	pCurrentDocument->AddCharSafe(L')');
}

CBold<CMDWriter>::CBold(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CBold<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	m_pWriter->GetCurrentDocument()->WriteString(L"**");

	return true;
}

void CBold<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pWriter->GetCurrentDocument()->WriteString(L"**");
}

CBreak<CMDWriter>::CBreak(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CBreak<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	m_pWriter->GetCurrentDocument()->WriteString(m_pWriter->GetParametrs().m_wsLineBreak);

	return true;
}

void CBreak<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CItalic<CMDWriter>::CItalic(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CItalic<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	m_pWriter->GetCurrentDocument()->AddCharSafe(L'*');

	return true;
}

void CItalic<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pWriter->GetCurrentDocument()->AddCharSafe(L'*');
}

CStrike<CMDWriter>::CStrike(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CStrike<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	m_pWriter->GetCurrentDocument()->WriteString(L"~~");

	return true;
}

void CStrike<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pWriter->GetCurrentDocument()->WriteString(L"~~");
}

CUnderline<CMDWriter>::CUnderline(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CUnderline<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	if (m_pWriter->GetParametrs().m_bUseAlternativeHTMLTags)
		m_pWriter->GetCurrentDocument()->WriteString(L"<u>");

	return true;
}

void CUnderline<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	if (m_pWriter->GetParametrs().m_bUseAlternativeHTMLTags)
		m_pWriter->GetCurrentDocument()->WriteString(L"</u>");
}

CQuotation<CMDWriter>::CQuotation(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CQuotation<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	m_pWriter->GetCurrentDocument()->AddCharSafe(L'>');

	return true;
}

void CQuotation<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CPreformatted<CMDWriter>::CPreformatted(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CPreformatted<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	m_pWriter->GetCurrentDocument()->WriteString(L"```" + m_pWriter->GetParametrs().m_wsLineBreak);

	return true;
}

void CPreformatted<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pWriter->GetCurrentDocument()->WriteString(m_pWriter->GetParametrs().m_wsLineBreak + L"```" + m_pWriter->GetParametrs().m_wsLineBreak);
}

CHeader<CMDWriter>::CHeader(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CHeader<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	switch(arSelectors.back().m_wsName[1])
	{
		case L'1' : m_pWriter->GetCurrentDocument()->WriteString(L"# "); break;
		case L'2' : m_pWriter->GetCurrentDocument()->WriteString(L"## "); break;
		case L'3' : m_pWriter->GetCurrentDocument()->WriteString(L"### "); break;
		case L'4' : m_pWriter->GetCurrentDocument()->WriteString(L"#### "); break;
		case L'5' : m_pWriter->GetCurrentDocument()->WriteString(L"##### "); break;
		case L'6' : m_pWriter->GetCurrentDocument()->WriteString(L"###### "); break;
		default:
			return false;
	}

	return true;
}

void CHeader<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CImage<CMDWriter>::CImage(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CImage<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	std::wstring wsAlt, wsSrc, wsTitle;

	if (!arSelectors.back().GetAttributeValue(L"src", wsSrc) &&
	    !arSelectors.back().GetAttributeValue(L"alt", wsAlt))
		return false;

	arSelectors.back().GetAttributeValue(L"title", wsTitle);

	XmlString* pCurrentDocument{m_pWriter->GetCurrentDocument()};

	pCurrentDocument->WriteString(L"![" + wsAlt + L"](" + wsSrc);

	if (!wsTitle.empty())
		pCurrentDocument->WriteString(L" \"" + wsTitle + L'"');

	pCurrentDocument->AddCharSafe(L')');

	return true;
}

void CImage<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CHorizontalRule<CMDWriter>::CHorizontalRule(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool HTML::CHorizontalRule<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	m_pWriter->GetCurrentDocument()->WriteString(L"\n\n---\n\n");

	return true;
}

void CHorizontalRule<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CBlockquote<CMDWriter>::CBlockquote(CMDWriter* pWriter)
	: CTag(pWriter), m_unIndex(0)
{}

bool CBlockquote<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	++m_unIndex;

	XmlString* pCurrentDocument{m_pWriter->GetCurrentDocument()};

	pCurrentDocument->WriteString(m_pWriter->GetParametrs().m_wsLineBreak);

	for (UINT unIndex = 0; unIndex < m_unIndex; ++m_unIndex)
		pCurrentDocument->WriteString(L"> ");

	return true;
}

void CBlockquote<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	--m_unIndex;
}

CTable<CMDWriter>::CTable(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CTable<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	//В MD не поддерживаются вложенные таблицы
	for (std::vector<NSCSS::CNode>::const_iterator itElement = arSelectors.cbegin(); itElement < arSelectors.cend() - 1; ++itElement)
		if (L"table" == itElement->m_wsName)
			return false;

	m_pWriter->GetCurrentDocument()->AddCharSafe(L'\n');

	return true;
}

void CTable<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	m_pWriter->GetCurrentDocument()->AddCharSafe(L'\n');
}

CTableRow<CMDWriter>::CTableRow(CMDWriter* pWriter)
	: CTag(pWriter), m_unLastRowType(static_cast<UINT>(ERowParseMode::Foother))
{}

bool CTableRow<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	using DataForRow = boost::tuple<const TTableRowStyle&, const CStorageTable&, ERowParseMode, ERowPosition>;

	if (!ValidInterpretator() || oExtraData.empty() || typeid(DataForRow) != oExtraData.type())
		return false;

	const DataForRow& oDataForRow(boost::any_cast<DataForRow>(oExtraData));
	const CStorageTable& oStorageTable{boost::get<1>(oDataForRow)};
	XmlString* pCurrentDocument{m_pWriter->GetCurrentDocument()};

	if (m_unLastRowType == static_cast<UINT>(ERowParseMode::Header))
	{
		for (UINT unIndex = 0; unIndex < oStorageTable.GetMaxColumns(); ++unIndex)
			pCurrentDocument->WriteString(L"|-");

		pCurrentDocument->WriteString(L"|\n");
	}
	else if (ERowParseMode::Header != boost::get<2>(oDataForRow) &&
	         ERowPosition::First == boost::get<3>(oDataForRow) &&
	         m_unLastRowType == static_cast<UINT>(ERowParseMode::Foother))
	{
		for (UINT unIndex = 0; unIndex < oStorageTable.GetMaxColumns(); ++unIndex)
			pCurrentDocument->WriteString(L"| ");

		pCurrentDocument->WriteString(L"|\n");

		for (UINT unIndex = 0; unIndex < oStorageTable.GetMaxColumns(); ++unIndex)
			pCurrentDocument->WriteString(L"|-");

		pCurrentDocument->WriteString(L"|\n");
	}

	m_unLastRowType = static_cast<UINT>(boost::get<2>(oDataForRow));

	pCurrentDocument->WriteString(L"| ");

	return true;
}

void CTableRow<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pWriter->GetCurrentDocument()->AddCharSafe(L'\n');
}

CTableCell<CMDWriter>::CTableCell(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CTableCell<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidInterpretator())
		return false;

	return true;
}

void CTableCell<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidInterpretator())
		return;

	m_pWriter->GetCurrentDocument()->WriteString(L" | ");
}
}
