#include "MDWriter.h"

#include <cwctype>

#include "../../DesktopEditor/common/File.h"
#include "../../Common/3dParty/html/css/src/CCompiledStyle.h"

namespace HTML
{
CMDWriter::CMDWriter(const TMarkdownParameters& oMDParametrs)
	: m_oMDParametrs(oMDParametrs)
{
	m_arStates.push(TState{});
	m_arStates.top().m_pCurrentDocument = &m_oDocument;
}

void CMDWriter::Begin(const std::wstring& wsDst)
{

}

void CMDWriter::End(const std::wstring& wsDst)
{
	NSFile::CFileBinary oDocument;

	if (!oDocument.CreateFileW(wsDst))
		return;

	oDocument.WriteStringUTF8(m_oDocument.GetData());
	oDocument.CloseFile();
}

inline void ReplaceSpaces(std::wstring& wsValue)
{
	// boost::wregex oRegex(L"\\s+");
	// wsValue = boost::regex_replace(wsValue, oRegex, L" ");

	std::wstring::const_iterator itBegin = std::find_if(wsValue.cbegin(), wsValue.cend(), [](wchar_t wchValue){ return std::iswspace(wchValue) && 0xa0 != wchValue; });
	std::wstring::const_iterator itEnd;

	while (wsValue.cend() != itBegin)
	{
		itEnd = std::find_if(itBegin, wsValue.cend(), [](wchar_t wchValue){ return !std::iswspace(wchValue) || 0xa0 == wchValue; });

		wsValue.replace(itBegin, itEnd, L" ");

		itBegin = std::find_if(itBegin + 1, wsValue.cend(), [](wchar_t wchValue){ return std::iswspace(wchValue) && 0xa0 != wchValue; });
	}
}

bool CMDWriter::WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors)
{
	bool bPreformatted{InPreformatted()};

	const NSCSS::CCompiledStyle* pCompiledStyle{arSelectors.back().m_pCompiledStyle};

	if (!bPreformatted && nullptr != pCompiledStyle)
	{
		// TODO::поведение должно быть немного разное (реализовать)
		switch(pCompiledStyle->m_oDisplay.GetWhiteSpace().ToInt())
		{
			case NSCSS::NSProperties::EWhiteSpace::Pre:
			case NSCSS::NSProperties::EWhiteSpace::Pre_Wrap:
			case NSCSS::NSProperties::EWhiteSpace::Pre_Line:
				bPreformatted = true;
			default:
				break;
		}
	}

	if (!bPreformatted && wsText.end() == std::find_if_not(wsText.begin(), wsText.end(), [](wchar_t wchChar){ return iswspace(wchChar) && 0xa0 != wchChar;}))
		return false;

	if (bPreformatted && !m_arStates.top().m_bEmptyLine && !m_arStates.top().m_bInTable)
	{
		for (std::vector<NSCSS::CNode>::const_reverse_iterator itElement{arSelectors.crbegin()}; itElement < arSelectors.crend(); ++itElement)
		{
			if (L"pre" == itElement->m_wsName)
			{
				GetCurrentDocument()->AddCharSafe(L'\n');
				break;
			}
		}
	}

	//Пока корректно работает только для текста (необходимо проверить и с другими нодами)
	if (m_arStates.top().m_bEmptyLine)
	{
		for (UINT unIndex = 0; unIndex < GetLevelBlockquote(); ++unIndex)
			WriteString(L"> ", true);
	}

	if (!bPreformatted && !InCode())
		ReplaceSpaces(wsText);

	bool bNeedBold{false}, bNeedItalic{false}, bNeedStrike{false};

	if (nullptr != pCompiledStyle)
	{
		if (!IsBold() && pCompiledStyle->m_oFont.Bold())
			bNeedBold = true;

		if (!IsItalic() && pCompiledStyle->m_oFont.Italic())
			bNeedItalic = true;

		if (!IsStrike() && pCompiledStyle->m_oText.LineThrough())
			bNeedStrike = true;
	}

	if (bNeedBold)
		WriteString(L"**");

	if (bNeedItalic)
		WriteString(L"*");

	if (bNeedStrike)
		WriteString(L"~~");

	ApplyAlternativeTags(pCompiledStyle);
	WriteString(wsText);
	ApplyAlternativeTags(pCompiledStyle, true);

	if (bNeedBold)
		WriteString(L"**");

	if (bNeedItalic)
		WriteString(L"*");

	if (bNeedStrike)
		WriteString(L"~~");

	if (L'\n' == wsText.back())
		m_arStates.top().m_bNeedBreakLine = false;

	return true;
}

void CMDWriter::WriteEmptyParagraph(bool bVahish, bool bInP)
{}

void CMDWriter::PageBreak()
{}

void CMDWriter::BeginBlock()
{
	if (!m_arStates.top().m_bInList)
		WriteBreakLine();
}

void CMDWriter::EndBlock(bool bAddBlock)
{
	if (!m_arStates.top().m_bInTable)
		WriteBreakLine();
}

void CMDWriter::SetDataOutput(XmlString* pOutputData)
{
	SaveState();
	m_arStates.top().m_pCurrentDocument = pOutputData;
}

void CMDWriter::RevertDataOutput()
{
	RollBackState();
}

TMarkdownParameters CMDWriter::GetParametrs() const
{
	return m_oMDParametrs;
}

void CMDWriter::WriteString(const std::wstring& wsString, bool bSpecialString)
{
	GetCurrentDocument()->WriteString(wsString);

	if (m_arStates.top().m_bEmptyLine)
		m_arStates.top().m_bEmptyLine = wsString.empty();

	if (!bSpecialString)
		m_arStates.top().m_bNeedBreakLine = true;
}

void CMDWriter::WriteOpenSpecialString(const std::wstring& wsString)
{
	if (m_arStates.top().m_wsLastSpecialString == wsString)
		GetCurrentDocument()->WriteString(L" ");

	m_arStates.top().m_wsLastSpecialString.clear();

	WriteString(wsString, true);
}

void CMDWriter::WriteCloseSpecialString(const std::wstring& wsString)
{
	m_arStates.top().m_wsLastSpecialString = wsString;

	WriteString(wsString, true);
}

XmlString* CMDWriter::GetCurrentDocument() const
{
	return m_arStates.top().m_pCurrentDocument;
}

bool CMDWriter::SupportNestedTables() const
{
	return false;
}

void CMDWriter::WriteBreakLine(bool bNeedChecked)
{
	if (bNeedChecked && !m_arStates.top().m_bNeedBreakLine)
		return;

	if (m_arStates.top().m_bInTable && bNeedChecked)
	{
		if (m_oMDParametrs.m_bUseAlternativeHTMLTags)
			WriteString(L"</br>");

		m_arStates.top().m_bNeedBreakLine = false;
		return;
	}

	if (m_arStates.top().m_bEmptyLine)
	{
		for (UINT unIndex = 0; unIndex < GetLevelBlockquote(); ++unIndex)
			WriteString(L"> ", true);
	}

	GetCurrentDocument()->WriteString(L"  \n");

	m_arStates.top().m_bEmptyLine = true;
	m_arStates.top().m_bNeedBreakLine = false;
	m_arStates.top().m_wsLastSpecialString.clear();
}

void CMDWriter::EnteredBold()
{
	m_arStates.top().m_bBold = true;
}

void CMDWriter::OutBold()
{
	m_arStates.top().m_bBold = false;
}

bool CMDWriter::IsBold()
{
	return m_arStates.top().m_bBold;
}

void CMDWriter::EnteredItalic()
{
	m_arStates.top().m_bItalic = true;
}

void CMDWriter::OutItalic()
{
	m_arStates.top().m_bItalic = false;
}

bool CMDWriter::IsItalic()
{
	return m_arStates.top().m_bItalic;
}

void CMDWriter::EnteredStrike()
{
	m_arStates.top().m_bStrike = true;
}

void CMDWriter::OutStrike()
{
	m_arStates.top().m_bStrike = false;
}

bool CMDWriter::IsStrike()
{
	return m_arStates.top().m_bStrike;
}

void CMDWriter::EnteredBlockquote()
{
	m_arStates.top().m_unLevelBlockquote++;
}

void CMDWriter::OutBlockquote()
{
	if (m_arStates.top().m_unLevelBlockquote > 0)
		m_arStates.top().m_unLevelBlockquote--;
}

UINT CMDWriter::GetLevelBlockquote()
{
	return m_arStates.top().m_unLevelBlockquote;
}

void CMDWriter::EnteredTable()
{
	m_arStates.top().m_bInTable = true;
}

void CMDWriter::OutTable()
{
	m_arStates.top().m_bInTable = false;
}

bool CMDWriter::InTable() const
{
	return m_arStates.top().m_bInTable;
}

void CMDWriter::EnteredPreformatted()
{
	m_arStates.top().m_bInPreformatted = true;
}

void CMDWriter::OutPreformatted()
{
	m_arStates.top().m_bInPreformatted = false;
}

bool CMDWriter::InPreformatted() const
{
	return m_arStates.top().m_bInPreformatted;
}

void CMDWriter::EnteredCode()
{
	m_arStates.top().m_bInCode = true;
}

void CMDWriter::OutCode()
{
	m_arStates.top().m_bInCode = false;
}

bool CMDWriter::InCode() const
{
	return m_arStates.top().m_bInCode;
}

void CMDWriter::EnteredList(bool bOrderedList)
{
	SaveState();
	m_arStates.top().m_bInList = true;
	m_arStates.top().m_bIsOrederedList = bOrderedList;
	m_arStates.top().m_unLevelList++;
}

void CMDWriter::OutList()
{
	RollBackState();
}

void CMDWriter::SetIndexOrderedList(UINT unIndex)
{
	m_arStates.top().m_unIndexListElement = unIndex;
}

void CMDWriter::IncreaseIndexOrderedList()
{
	m_arStates.top().m_unIndexListElement++;
}

bool CMDWriter::InOrederedList() const
{
	return m_arStates.top().m_bIsOrederedList;
}

UINT CMDWriter::GetIndexOrderedList() const
{
	return m_arStates.top().m_unIndexListElement;
}

UINT CMDWriter::GetLevelList() const
{
	return m_arStates.top().m_unLevelList;
}

void CMDWriter::SaveState()
{
	m_arStates.push(m_arStates.top());
}

void CMDWriter::RollBackState()
{
	if (m_arStates.size() > 1)
		m_arStates.pop();
}

void CMDWriter::ApplyAlternativeTags(const NSCSS::CCompiledStyle* pCompiledStyle, bool bIsCloseTag)
{
	if (nullptr == pCompiledStyle || !m_oMDParametrs.m_bUseAlternativeHTMLTags)
		return;

	if (pCompiledStyle->m_oText.Underline())
		GetCurrentDocument()->WriteString(bIsCloseTag ? L"</u>" : L"<u>");

	if (L"top" == pCompiledStyle->m_oDisplay.GetVAlign().ToWString())
		GetCurrentDocument()->WriteString(bIsCloseTag ? L"</sup>" : L"<sup>");
	else if (L"bottom" == pCompiledStyle->m_oDisplay.GetVAlign().ToWString())
		GetCurrentDocument()->WriteString(bIsCloseTag ? L"</sub>" : L"<sub>");

	if (L"FFFF00" == pCompiledStyle->m_oBackground.GetColor().ToHEX())
		GetCurrentDocument()->WriteString(bIsCloseTag ? L"</mark>" : L"<mark>");
}
}
