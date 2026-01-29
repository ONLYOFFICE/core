#ifndef MDWRITER_H
#define MDWRITER_H

#include "IWriter.h"
#include <stack>

namespace HTML
{
struct TMDParametrs
{
	bool m_bUseAlternativeHTMLTags = false; //Использовать html теги там, где нет стандартной реализации в md(например для подчеркиваний)
	std::wstring m_wsLineBreak = L"  "; // По умолчанию для переноса строки используем двойной пробел
};

class CMDWriter : public IWriter
{
	XmlString m_oDocument;

	struct TState
	{
		XmlString *m_pCurrentDocument;
	};

	std::stack<TState> m_arStates;
	TMDParametrs m_oMDParametrs;
public:
	CMDWriter(const TMDParametrs& oMDParametrs);

	void Begin(const std::wstring& wsDst, const THtmlParams* pParams) override;
	void End(const std::wstring& wsDst)  override;

	bool WriteText(const std::wstring& wsText, const std::vector<NSCSS::CNode>& arSelectors) override;

	void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) override;

	void BeginBlock() override;
	void EndBlock(bool bAddBlock) override;

	void SetDataOutput(XmlString* pOutputData) override;
	void RevertDataOutput() override;

	TMDParametrs GetParametrs() const;
	XmlString* GetCurrentDocument() const override;
private:
	void SaveState();
	void RollBackState();
};
}

#endif // MDWRITER_H
