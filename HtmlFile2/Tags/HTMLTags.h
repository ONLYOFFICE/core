#ifndef HTMLTAGS_H
#define HTMLTAGS_H

#include "../Common/3dParty/html/css/src/CNode.h"

namespace HTML
{
struct ITag
{
	virtual ~ITag() = default;
	virtual bool Open(std::vector<NSCSS::CNode>& sSelectors)  = 0;
	virtual void Close(std::vector<NSCSS::CNode>& sSelectors) = 0;
};

struct TEmptyTag : public ITag
{
	virtual ~TEmptyTag() = default;
	virtual bool Open(std::vector<NSCSS::CNode>& sSelectors)
	{
		return true;
	};
	virtual void Close(std::vector<NSCSS::CNode>& sSelectors) {};
};

template<class T>
struct TTag : public ITag
{
	T* m_pInterpretator;

	TTag(T* pInterpretator)
		: m_pInterpretator(pInterpretator)
	{}
	virtual ~TTag() = default;
	virtual bool Open(std::vector<NSCSS::CNode>& sSelectors) = 0;
	virtual void Close(std::vector<NSCSS::CNode>& sSelectors) = 0;

	bool ValidInterpretator() const
	{
		return nullptr != m_pInterpretator;
	}
};

#define CREATE_TAG(tag_name)\
template<class T>\
struct tag_name : public TTag<T>\
{\
	virtual bool Open(std::vector<NSCSS::CNode>& sSelectors)  = 0;\
	virtual void Close(std::vector<NSCSS::CNode>& sSelectors) = 0;\
}

CREATE_TAG(TAnchor);
CREATE_TAG(TAbbr);
CREATE_TAG(TBold);
CREATE_TAG(TBidirectional);
CREATE_TAG(TBr);
CREATE_TAG(TCenter);
CREATE_TAG(TItalic);
CREATE_TAG(TCode);
CREATE_TAG(TKbd);
CREATE_TAG(TStrike);
CREATE_TAG(TUnderline);
CREATE_TAG(TMark);
CREATE_TAG(TQuotation);
CREATE_TAG(TSup);
CREATE_TAG(TSub);
CREATE_TAG(TSpan);
CREATE_TAG(TDD);
CREATE_TAG(TPreformatted);
CREATE_TAG(THeader);

}

#endif // HTMLTAGS_H
