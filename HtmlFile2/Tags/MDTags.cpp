#include "MDTags.h"

#include "../src/StringFinder.h"
#include "../Table.h"

#include <boost/tuple/tuple.hpp>

namespace HTML
{
CAnchor<CMDWriter>::CAnchor(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CAnchor<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->WriteString(L"[");
	return true;
}

void CAnchor<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->WriteString(L"]");

	std::wstring wsHref, wsTitle;

	arSelectors.back().GetAttributeValue(L"href", wsHref);
	arSelectors.back().GetAttributeValue(L"title", wsTitle);

	m_pWriter->WriteString(L'(' + wsHref);

	if (!wsTitle.empty())
		m_pWriter->WriteString(L" \"" + wsTitle + L'"');

	m_pWriter->WriteString(L")");
}

CBold<CMDWriter>::CBold(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CBold<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	if (m_pWriter->IsBold())
		return true;

	m_pWriter->WriteOpenSpecialString(L"**");
	m_pWriter->EnteredBold();

	return true;
}

void CBold<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter() || !m_pWriter->IsBold())
		return;

	m_pWriter->WriteCloseSpecialString(L"**");
	m_pWriter->OutBold();
}

CBreak<CMDWriter>::CBreak(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CBreak<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->WriteBreakLine();

	return true;
}

void CBreak<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CItalic<CMDWriter>::CItalic(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CItalic<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	if (m_pWriter->IsItalic())
		return true;

	m_pWriter->WriteOpenSpecialString(L"*");
	m_pWriter->EnteredItalic();

	return true;
}

void CItalic<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter() || !m_pWriter->IsItalic())
		return;

	m_pWriter->WriteCloseSpecialString(L"*");
	m_pWriter->OutItalic();
}

CStrike<CMDWriter>::CStrike(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CStrike<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	if (m_pWriter->IsStrike())
		return true;

	m_pWriter->WriteOpenSpecialString(L"~~");
	m_pWriter->EnteredStrike();

	return true;
}

void CStrike<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter() || !m_pWriter->IsStrike())
		return;

	m_pWriter->WriteCloseSpecialString(L"~~");
	m_pWriter->OutStrike();
}

CQuotation<CMDWriter>::CQuotation(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CQuotation<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	return true;
}

void CQuotation<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CPreformatted<CMDWriter>::CPreformatted(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CPreformatted<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->WriteOpenSpecialString(L"```");
	m_pWriter->EnteredPreformatted();

	return true;
}

void CPreformatted<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	bool bNeedBreakLine{false};

	for (std::vector<NSCSS::CNode>::const_reverse_iterator itElement{arSelectors.crbegin()}; itElement < arSelectors.crend(); ++itElement)
	{
		if (L"pre" == itElement->m_wsName)
		{
			bNeedBreakLine = true;
			break;
		}
	}

	if (bNeedBreakLine && !m_pWriter->InTable())
		m_pWriter->WriteBreakLine();

	m_pWriter->WriteCloseSpecialString(L"```");
	m_pWriter->OutPreformatted();

	if (bNeedBreakLine && !m_pWriter->InTable())
		m_pWriter->WriteBreakLine(false);
}

CHeader<CMDWriter>::CHeader(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CHeader<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	switch(arSelectors.back().m_wsName[1])
	{
		case L'1' : m_pWriter->WriteString(L"# ",      true); break;
		case L'2' : m_pWriter->WriteString(L"## ",     true); break;
		case L'3' : m_pWriter->WriteString(L"### ",    true); break;
		case L'4' : m_pWriter->WriteString(L"#### ",   true); break;
		case L'5' : m_pWriter->WriteString(L"##### ",  true); break;
		case L'6' : m_pWriter->WriteString(L"###### ", true); break;
		default:
			return false;
	}

	return true;
}

void CHeader<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;
}

CImage<CMDWriter>::CImage(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CImage<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	std::wstring wsAlt, wsSrc, wsTitle;

	if (!arSelectors.back().GetAttributeValue(L"src", wsSrc) &&
	    !arSelectors.back().GetAttributeValue(L"alt", wsAlt))
		return false;

	arSelectors.back().GetAttributeValue(L"title", wsTitle);

	m_pWriter->WriteString(L"![" + wsAlt + L"](" + wsSrc);

	if (!wsTitle.empty())
		m_pWriter->WriteString(L" \"" + wsTitle + L'"');

	m_pWriter->WriteString(L")");

	return true;
}

void CImage<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->WriteBreakLine();
}

CHorizontalRule<CMDWriter>::CHorizontalRule(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CHorizontalRule<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->WriteBreakLine(false);
	m_pWriter->WriteString(L"---");
	m_pWriter->WriteBreakLine(false);

	return true;
}

void CHorizontalRule<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CBlockquote<CMDWriter>::CBlockquote(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CBlockquote<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->WriteBreakLine();
	m_pWriter->EnteredBlockquote();

	return true;
}

void CBlockquote<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->OutBlockquote();
	m_pWriter->WriteBreakLine();
	m_pWriter->WriteBreakLine(false);
}

CTable<CMDWriter>::CTable(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CTable<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter() /*|| m_pWriter->InTable()*/) //В MD не поддерживаются вложенные таблицы (пока разруливаем в парсере)
		return false;

	m_pWriter->WriteBreakLine();
	m_pWriter->EnteredTable();

	if (m_pWriter->InCode())
	{
		if (!m_pWriter->InPreformatted())
			m_pWriter->WriteCloseSpecialString(L"`");

		m_pWriter->OutCode();
	}

	if (m_pWriter->InPreformatted())
	{
		m_pWriter->WriteBreakLine();
		m_pWriter->WriteCloseSpecialString(L"```");
		m_pWriter->WriteBreakLine(false);
		m_pWriter->OutPreformatted();
	}

	return true;
}

void CTable<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->OutTable();
	m_pWriter->WriteBreakLine();
}

CTableRow<CMDWriter>::CTableRow(CMDWriter* pWriter)
	: CTag(pWriter), m_unLastRowType(static_cast<UINT>(ERowParseMode::Foother))
{}

bool CTableRow<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	using DataForRow = boost::tuple<const TTableRowStyle*, const CStorageTable&, ERowParseMode, ERowPosition>;

	if (!ValidWriter() || oExtraData.empty() || typeid(DataForRow) != oExtraData.type())
		return false;

	m_pWriter->EnteredTable();

	const DataForRow& oDataForRow(boost::any_cast<DataForRow>(oExtraData));
	const CStorageTable& oStorageTable{boost::get<1>(oDataForRow)};

	if (nullptr == boost::get<0>(oDataForRow) && ERowParseMode::Header == boost::get<2>(oDataForRow))
	{
		for (UINT unIndex = 0; unIndex < oStorageTable.GetMaxColumns(); ++unIndex)
			m_pWriter->WriteOpenSpecialString(L"| ");

		m_pWriter->WriteOpenSpecialString(L"|");
		m_pWriter->WriteBreakLine();
		m_unLastRowType = static_cast<UINT>(ERowParseMode::Header);
		return true;
	}

	if (m_unLastRowType == static_cast<UINT>(ERowParseMode::Header))
	{
		for (UINT unIndex = 0; unIndex < oStorageTable.GetMaxColumns(); ++unIndex)
			m_pWriter->WriteString(L"|-");

		m_pWriter->WriteOpenSpecialString(L"|");
		m_pWriter->WriteBreakLine(false);
	}

	m_pWriter->WriteOpenSpecialString(L"| ");
	m_unLastRowType = static_cast<UINT>(boost::get<2>(oDataForRow));

	return true;
}

void CTableRow<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->WriteBreakLine(false);
}

CTableCell<CMDWriter>::CTableCell(CMDWriter* pWriter)
	: CTag(pWriter), m_unNeedEmptyCells(0)
{}

bool CTableCell<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	using DataForCell = boost::tuple<const CStorageTableCell&, const CStorageTable&, UINT, ERowParseMode, ERowPosition>;

	if (!ValidWriter() || oExtraData.empty() || typeid(DataForCell) != oExtraData.type())
		return false;

	const DataForCell& oDataForCell{boost::any_cast<const DataForCell>(oExtraData)};

	m_unNeedEmptyCells = boost::get<0>(oDataForCell).GetColspan() - 1;

	return true;
}

void CTableCell<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	for (UINT unIndex = 0; unIndex < m_unNeedEmptyCells; ++unIndex)
		m_pWriter->WriteOpenSpecialString(L" |");

	m_unNeedEmptyCells = 0;
	m_pWriter->WriteOpenSpecialString(L" | ");
}

CList<CMDWriter>::CList(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CList<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->WriteBreakLine();
	m_pWriter->EnteredList(L"ol" == arSelectors.back().m_wsName);

	if (!m_pWriter->InOrederedList())
		return true;

	std::wstring wsIndex;

	if (arSelectors.back().GetAttributeValue(L"start", wsIndex))
		m_pWriter->SetIndexOrderedList(NSStringFinder::ToInt(wsIndex, 1));

	return true;
}

void CList<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	m_pWriter->OutList();
}

CListElement<CMDWriter>::CListElement(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CListElement<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	if (0 !=  m_pWriter->GetLevelList())
	{
		for (UINT unLevelList = 0; unLevelList < m_pWriter->GetLevelList() - 1; ++unLevelList)
			m_pWriter->WriteString(L"  ");
	}

	if (m_pWriter->InOrederedList())
	{
		m_pWriter->WriteString(std::to_wstring(m_pWriter->GetIndexOrderedList()) + m_pWriter->GetParametrs().m_wchOrderedList + L' ');
		m_pWriter->IncreaseIndexOrderedList();
	}
	else
		m_pWriter->WriteString({m_pWriter->GetParametrs().m_wchUnorderedList, L' '});

	return true;
}

void CListElement<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CCode<CMDWriter>::CCode(CMDWriter* pWriter)
	: CTag(pWriter)
{}

bool CCode<CMDWriter>::Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData)
{
	if (!ValidWriter())
		return false;

	m_pWriter->EnteredCode();

	if (m_pWriter->InPreformatted())
	{
		if (!arSelectors.back().m_wsClass.empty() && arSelectors.back().m_wsClass.size() >= 9 &&
		    0 == arSelectors.back().m_wsClass.compare(0, 9, L"language-"))
			m_pWriter->WriteString(arSelectors.back().m_wsClass.substr(9, arSelectors.back().m_wsClass.size() - 9));
		m_pWriter->WriteBreakLine(false);
	}
	else
		m_pWriter->WriteOpenSpecialString(L"`");

	return true;
}

void CCode<CMDWriter>::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!ValidWriter())
		return;

	if (!m_pWriter->InPreformatted())
		m_pWriter->WriteCloseSpecialString(L"`");

	m_pWriter->OutCode();
}
}
