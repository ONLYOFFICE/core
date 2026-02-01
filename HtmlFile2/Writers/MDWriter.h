#ifndef MDWRITER_H
#define MDWRITER_H

#include "IWriter.h"
#include <stack>

namespace HTML
{
struct TMDParametrs
{
	bool m_bUseAlternativeHTMLTags = false; //Использовать html теги там, где нет стандартной реализации в md(например для подчеркиваний)
	WCHAR m_wchUnorderedList = L'-'; // Возможные варианты в md: -, +, *
	WCHAR m_wchOrderedList = L'.'; // Возможные варианты в md: ., )
};

class CMDWriter : public IWriter
{
	XmlString m_oDocument;

	struct TState
	{
		XmlString *m_pCurrentDocument{nullptr};

		bool m_bNeedBreakLine{false};
		bool m_bEmptyLine{true};

		UINT m_unLevelBlockquote{0};

		bool m_bInTable{false};
		bool m_bInPreformatted{false};
		bool m_bInCode{false};

		bool m_bInList{false};
		bool m_bIsOrederedList{false};
		UINT m_unLevelList{0};
		UINT m_unIndexListElement{0};
	};

	std::stack<TState> m_arStates;
	TMDParametrs m_oMDParametrs;
public:
	CMDWriter(const TMDParametrs& oMDParametrs);

	void Begin(const std::wstring& wsDst, const THtmlParams* pParams) override;
	void End(const std::wstring& wsDst)  override;

	bool WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors) override;

	void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) override;

	void BeginBlock() override;
	void EndBlock(bool bAddBlock) override;

	void SetDataOutput(XmlString* pOutputData) override;
	void RevertDataOutput() override;

	TMDParametrs GetParametrs() const;
	void WriteString(const std::wstring& wsString, bool bSpecialString = false);
	XmlString* GetCurrentDocument() const override;

	void WriteBreakLine(bool bNeedChecked = true);

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
	void IncreaseIndexOrderedList();

	bool InOrederedList() const;
	UINT GetIndexOrderedList() const;
	UINT GetLevelList() const;
private:
	void SaveState();
	void RollBackState();
};
}

#endif // MDWRITER_H
