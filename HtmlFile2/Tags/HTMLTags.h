#ifndef HTMLTAGS_H
#define HTMLTAGS_H

#include "../Common/3dParty/html/css/src/CNode.h"
#include "../TextSettings.h"

namespace HTML
{
struct ITag
{
	virtual ~ITag() = default;
	virtual bool Open(std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS)  = 0;
	virtual void Close(std::vector<NSCSS::CNode>& sSelectors) = 0;
};

template<class T>
struct TTag : public ITag
{
	T* m_pInterpretator;

	TTag(T* pInterpretator)
		: m_pInterpretator(pInterpretator)
	{}
	virtual ~TTag() = default;
	virtual bool Open(std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS)  = 0;
	virtual void Close(std::vector<NSCSS::CNode>& sSelectors) = 0;

	bool ValidInterpretator() const
	{
		return nullptr != m_pInterpretator;
	}
};

template<class T>
struct TAnchor : public TTag<T>
{
	virtual bool Open(std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS)  = 0;
	virtual void Close(std::vector<NSCSS::CNode>& sSelectors) = 0;
};

}

#endif // HTMLTAGS_H
