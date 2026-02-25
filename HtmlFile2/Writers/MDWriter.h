#ifndef MDWRITER_H
#define MDWRITER_H

#include "IWriter.h"
#include "../MarkdownParameters.h"

#include <stack>

namespace HTML
{
class CMDWriter : public IWriter
{
	XmlString m_oDocument;

	struct TState
	{
		XmlString *m_pCurrentDocument{nullptr};

		bool m_bNeedBreakLine{false};
		bool m_bEmptyLine{true};

		UINT m_unLevelBlockquote{0};

		bool m_bBold{false};
		bool m_bItalic{false};
		bool m_bStrike{false};

		bool m_bInTable{false};
		bool m_bInPreformatted{false};
		bool m_bInCode{false};

		bool m_bInList{false};
		bool m_bIsOrederedList{false};
		UINT m_unLevelList{0};
		UINT m_unIndexListElement{1};

		std::wstring m_wsLastSpecialString;
	};

	std::stack<TState> m_arStates;
	TMarkdownParameters m_oMDParametrs;
public:
	CMDWriter(const TMarkdownParameters& oMDParametrs);

	void Begin(const std::wstring& wsDst) override;
	void End(const std::wstring& wsDst)  override;

	bool WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors) override;

	void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) override;

	void PageBreak() override;

	void BeginBlock() override;
	void EndBlock(bool bAddBlock) override;

	void SetDataOutput(XmlString* pOutputData) override;
	void RevertDataOutput() override;

	TMarkdownParameters GetParametrs() const;

	void WriteString(const std::wstring& wsString, bool bSpecialString = false);
	void WriteOpenSpecialString(const std::wstring& wsString);
	void WriteCloseSpecialString(const std::wstring& wsString);

	XmlString* GetCurrentDocument() const override;

	bool SupportNestedTables() const override;

	void WriteBreakLine(bool bNeedChecked = true);

	void EnteredBold();
	void OutBold();
	bool IsBold();

	void EnteredItalic();
	void OutItalic();
	bool IsItalic();

	void EnteredStrike();
	void OutStrike();
	bool IsStrike();

	void EnteredBlockquote();
	void OutBlockquote();
	UINT GetLevelBlockquote();

	void EnteredTable();
	void OutTable();
	bool InTable() const;

	void EnteredPreformatted();
	void OutPreformatted();
	bool InPreformatted() const;

	void EnteredCode();
	void OutCode();
	bool InCode() const;

	void EnteredList(bool bOrderedList);
	void OutList();
	void SetIndexOrderedList(UINT unIndex);
	void IncreaseIndexOrderedList();

	bool InOrederedList() const;
	UINT GetIndexOrderedList() const;
	UINT GetLevelList() const;

	void SetLinePrefix(const std::wstring& wsPrefix);
	void ClearLinePrefix();
private:
	void SaveState();
	void RollBackState();

	void ApplyAlternativeTags(const NSCSS::CCompiledStyle* pCompiledStyle, bool bIsCloseTag = false);
};
}

#endif // MDWRITER_H
