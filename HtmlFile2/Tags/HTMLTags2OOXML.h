#ifndef HTMLTAGS2OOXML_H
#define HTMLTAGS2OOXML_H

#include "HTMLTags.h"
#include "HTML2OOXMLInterpretator.h"

namespace HTML
{
inline bool GetTitle(const std::vector<NSCSS::CNode>& arSelectors, std::wstring& wsTitle);

template<>
struct TAnchor<CHTML2OOXMLInterpretator> : public TTag<CHTML2OOXMLInterpretator>
{
	TAnchor(CHTML2OOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return false;

		std::wstring wsRef, wsAlt, wsName;
		bool bCross = false;

		if (arSelectors.back().GetAttributeValue(L"href", wsRef) && wsRef.find('#') != std::wstring::npos)
			bCross = true;

		if (arSelectors.back().GetAttributeValue(L"name", wsName))
			m_pInterpretator->WriteBookmark(wsName);

		arSelectors.back().GetAttributeValue(L"alt", wsAlt);

		std::wstring wsTooltip(wsRef);

		GetTitle(arSelectors, wsTooltip);

		if (bCross)
			m_pInterpretator->OpenCrossHyperlink(wsRef, arSelectors);
		else
			m_pInterpretator->OpenExternalHyperlink(wsRef, wsTooltip, arSelectors);

		return true;
	}

	virtual void Close(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return;

		bool bCross = false;
		std::wstring wsFootnote;

		// std::map<std::wstring, std::wstring>& mAttributes(arSelectors.back().m_mAttributes);
		// std::map<std::wstring, std::wstring>::const_iterator itFound;

		if (arSelectors.back().m_wsStyle.find(L"mso-footnote-id") != std::wstring::npos)
			wsFootnote = arSelectors.back().m_wsStyle.substr(arSelectors.back().m_wsStyle.rfind(L':') + 1);
		else
		{
			if (arSelectors.back().GetAttributeValue(L"epub:type", wsFootnote) && wsFootnote.find(L"noteref"))
				wsFootnote = L"href";
		}

		std::wstring wsRef;

		if (arSelectors.back().GetAttributeValue(L"href", wsRef))
		{
			if(wsRef.find('#') != std::wstring::npos)
				bCross = true;
		}

		if (bCross)
		{
			if (wsFootnote == L"href")
				wsFootnote = wsRef.substr(wsRef.find('#') + 1);

			m_pInterpretator->CloseCrossHyperlink(arSelectors, wsFootnote, wsRef);
		}
		else
			m_pInterpretator->CloseExternalHyperlink();
	}
};

template<>
struct TAbbr<CHTML2OOXMLInterpretator> : public TTag<CHTML2OOXMLInterpretator>
{
	TAbbr(CHTML2OOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return false;

		std::wstring wsTitle;

		if (!GetTitle(arSelectors, wsTitle))
			return false;

		m_pInterpretator->WritePPr(arSelectors);
		m_pInterpretator->OpenFldChar(wsTitle);

		return true;
	}

	virtual void Close(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return;

		m_pInterpretator->CloseFldChar();
	}
};

template<>
struct TBr<CHTML2OOXMLInterpretator> : public TTag<CHTML2OOXMLInterpretator>
{
	TBr(CHTML2OOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return false;

		m_pInterpretator->WriteBr(arSelectors);

		return true;
	}

	virtual void Close(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return;
	}
};

template<>
struct TBidirectional<CHTML2OOXMLInterpretator> : public TTag<CHTML2OOXMLInterpretator>
{
	TBidirectional(CHTML2OOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return false;

		if (L"bdo" == arSelectors.back().m_wsName)
		{
			std::wstring wsDir;

			arSelectors.back().GetAttributeValue(L"dir", wsDir);

			m_pInterpretator->GetTextSettings().SetNewBidirectional(wsDir == L"rtl");
		}
		else
			//TODO:: по идее можно оптимизировать при условии, что и так false
			//Но основная проблема в RollBackBdo
			m_pInterpretator->GetTextSettings().SetNewBidirectional(false);

		return true;
	}

	virtual void Close(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return;

		m_pInterpretator->GetTextSettings().RollBackBidirectional();
	}
};

template<>
struct TQuotation<CHTML2OOXMLInterpretator> : public TTag<CHTML2OOXMLInterpretator>
{
	TQuotation(CHTML2OOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return false;

		m_pInterpretator->GetTextSettings().SetNewQuotation(true);

		return true;
	}

	virtual void Close(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return;

		m_pInterpretator->GetTextSettings().RollBackQuotation();
	}
};

template<>
struct TSpan<CHTML2OOXMLInterpretator> : public TTag<CHTML2OOXMLInterpretator>
{
	TSpan(CHTML2OOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return false;

		m_pInterpretator->GetTextSettings().SetNewAddSpaces(false);

		return true;
	}

	virtual void Close(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return;

		m_pInterpretator->GetTextSettings().RollBackAddSpaces();
	}
};

template<>
struct TPreformatted <CHTML2OOXMLInterpretator> : public TTag<CHTML2OOXMLInterpretator>
{
	TPreformatted (CHTML2OOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return false;

		m_pInterpretator->GetTextSettings().SetNewPreformatted(false);

		return true;
	}

	virtual void Close(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return;

		m_pInterpretator->GetTextSettings().RollBackPreformatted();
	}
};

#define CREATE_EMPTY_TAG(tag_name)\
template<>\
struct tag_name<CHTML2OOXMLInterpretator> : public TEmptyTag\
{\
	tag_name(){}\
}

CREATE_EMPTY_TAG(TBold);
CREATE_EMPTY_TAG(TCenter);
CREATE_EMPTY_TAG(TItalic);
CREATE_EMPTY_TAG(TCode);
CREATE_EMPTY_TAG(TKbd);
CREATE_EMPTY_TAG(TStrike);
CREATE_EMPTY_TAG(TUnderline);
CREATE_EMPTY_TAG(TMark);
CREATE_EMPTY_TAG(TSup);
CREATE_EMPTY_TAG(TSub);
CREATE_EMPTY_TAG(TDD);
CREATE_EMPTY_TAG(THeader);

inline bool GetTitle(const std::vector<NSCSS::CNode>& arSelectors, std::wstring& wsTitle)
{
	if (arSelectors.empty())
		return false;

	const std::map<std::wstring, std::wstring>::const_iterator itFound{arSelectors.back().m_mAttributes.find(L"title")};

	if (arSelectors.back().m_mAttributes.cend() == itFound)
		return false;

	wsTitle= itFound->second;
	return true;
}
}

#endif // HTMLTAGS2OOXML_H
