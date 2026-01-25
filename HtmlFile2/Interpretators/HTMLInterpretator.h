#ifndef HTMLINTERPRETATOR_H
#define HTMLINTERPRETATOR_H

#include "../Common/3dParty/html/css/src/CNode.h"
#include "HTMLParams.h"

namespace HTML
{
class IHTMLInterpretator
{
public:
	IHTMLInterpretator() = default;
	virtual ~IHTMLInterpretator() = default;

	virtual void Begin(const std::wstring& wsDst, const THtmlParams* pParams) = 0;
	virtual void End(const std::wstring& wsDst) = 0;

	virtual bool WriteText(const std::wstring& wsText, const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) = 0;

	virtual void BeginBlock() = 0;
	virtual void EndBlock(bool bAddBlock) = 0;

	#ifdef _DEBUG
	virtual void PrintData() = 0;
	#endif
};
}

#endif // HTMLINTERPRETATOR_H
