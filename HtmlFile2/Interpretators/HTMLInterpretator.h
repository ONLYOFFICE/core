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

	virtual void UpdatePageStyle(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	//Conversion methods
	virtual bool OpenAnchor(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseAnchor(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual void Break(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenAbbreviation(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseAbbreviation(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenBold(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseBold(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenBidirectional(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseBidirectional(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenItalic(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseItalic(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenPreformatted(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void ClosePreformatted(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenKBD(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseKBD(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenStrikethrough(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseStrikethrough(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenUnderline(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseUnderline(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenQuotation(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseQuotation(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	virtual bool OpenHeader(const std::vector<NSCSS::CNode>& arSelectors) = 0;
	virtual void CloseHeader(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	#ifdef _DEBUG
	virtual void PrintData() = 0;
	#endif
};
}

#endif // HTMLINTERPRETATOR_H
