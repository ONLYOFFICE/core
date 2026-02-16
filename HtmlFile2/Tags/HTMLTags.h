#ifndef HTMLTAGS_H
#define HTMLTAGS_H

#include "../Common/3dParty/html/css/src/CNode.h"
#include <boost/any.hpp>

namespace HTML
{
class ITag
{
public:
	virtual ~ITag() = default;
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any())  = 0;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) = 0;
};

class CEmptyTag : public ITag
{
public:
	virtual ~CEmptyTag() = default;
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any())
	{
		return true;
	};
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) {};
};

template<class T>
class CTag : public ITag
{
protected:
	T* m_pWriter;
public:
	CTag(T* pWriter)
		: m_pWriter(pWriter)
	{}
	virtual ~CTag() = default;
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) = 0;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) = 0;

	bool ValidWriter() const
	{
		return nullptr != m_pWriter;
	}
};

#define CREATE_TAG(tag_name)\
template<class T>\
class tag_name : public CTag<T>\
{\
public:\
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors)  = 0;\
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) = 0;\
}

CREATE_TAG(CAnchor);
CREATE_TAG(CAbbr);
CREATE_TAG(CBold);
CREATE_TAG(CBidirectional);
CREATE_TAG(CBreak);
CREATE_TAG(CCenter);
CREATE_TAG(CItalic);
CREATE_TAG(CKbd);
CREATE_TAG(CStrike);
CREATE_TAG(CUnderline);
CREATE_TAG(CMark);
CREATE_TAG(CQuotation);
CREATE_TAG(CSup);
CREATE_TAG(CSpan);
CREATE_TAG(CDD);
CREATE_TAG(CPreformatted);
CREATE_TAG(CHeader);
CREATE_TAG(CDivision);
CREATE_TAG(CImage);
CREATE_TAG(CFont);
CREATE_TAG(CInput);
CREATE_TAG(CBaseFont);
CREATE_TAG(CBlockquote);
CREATE_TAG(CHorizontalRule);
CREATE_TAG(CList);
CREATE_TAG(CListElement);
CREATE_TAG(CCaption);
CREATE_TAG(CTable);
CREATE_TAG(CTableRow);
CREATE_TAG(CTableCell);
CREATE_TAG(CCode);
CREATE_TAG(CHTML);

}
#endif // HTMLTAGS_H
