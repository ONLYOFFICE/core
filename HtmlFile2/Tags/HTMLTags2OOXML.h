#ifndef HTMLTAGS2OOXML_H
#define HTMLTAGS2OOXML_H

#include "HTMLTags.h"
#include "HTML2OOXMLInterpretator.h"

namespace HTML
{
template<>
struct TAnchor<CHTML2OOXMLInterpretator> : public TTag<CHTML2OOXMLInterpretator>
{
	TAnchor(CHTML2OOXMLInterpretator* pInterpretator)
		: TTag(pInterpretator)
	{}

	virtual bool Open(std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (!ValidInterpretator())
			return false;

		std::wstring wsRef;
		std::wstring wsAlt;
		bool bCross = false;

		std::map<std::wstring, std::wstring>& mAttributes(arSelectors.back().m_mAttributes);

		std::map<std::wstring, std::wstring>::const_iterator itFound = mAttributes.find(L"href");

		bool bFounded = false;

		if (mAttributes.cend() != itFound)
		{
			wsRef = itFound->second;
			if(wsRef.find('#') != std::wstring::npos)
				bCross = true;

			bFounded = true;
		}
		else
			itFound = mAttributes.find(L"name");

		if (!bFounded)
		{
			if (mAttributes.cend() != itFound)
			{
				m_pInterpretator->WriteBookmark(itFound->second);
				bFounded = true;
			}
			else
				itFound = mAttributes.find(L"alt");
		}

		if (!bFounded)
		{
			if (mAttributes.cend() != itFound)
			{
				wsAlt = itFound->second;
				bFounded = true;
			}
		}

		if (!bFounded)
			return false;

		std::wstring wsTooltip(wsRef);

		itFound = mAttributes.find(L"title");

		if (mAttributes.cend() != itFound)
			wsTooltip = itFound->second;

		if (bCross)
			m_pInterpretator->OpenCrossHyperlink(wsRef, arSelectors, oTS);
		else
			m_pInterpretator->OpenExternalHyperlink(wsRef, wsTooltip, arSelectors, oTS);

		return true;
	}
	virtual void Close(std::vector<NSCSS::CNode>& arSelectors)
	{
		if (!ValidInterpretator())
			return;

		bool bCross = false;
		std::wstring wsFootnote;

		std::map<std::wstring, std::wstring>& mAttributes(arSelectors.back().m_mAttributes);
		std::map<std::wstring, std::wstring>::const_iterator itFound;

		if (arSelectors.back().m_wsStyle.find(L"mso-footnote-id") != std::wstring::npos)
			wsFootnote = arSelectors.back().m_wsStyle.substr(arSelectors.back().m_wsStyle.rfind(L':') + 1);
		else
		{
			itFound = mAttributes.find(L"epub:type");

			if (mAttributes.cend() != itFound && itFound->second.find(L"noteref"))
				wsFootnote = L"href";
		}

		std::wstring wsRef;
		itFound = mAttributes.find(L"href");

		if (mAttributes.cend() != itFound)
		{
			wsRef = itFound->second;
			if(itFound->second.find('#') != std::wstring::npos)
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
}

#endif // HTMLTAGS2OOXML_H
