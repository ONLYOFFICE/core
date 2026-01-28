#ifndef OOXMLTAGS_H
#define OOXMLTAGS_H

#include "HTMLTags.h"
#include "../Interpretators/OOXMLInterpretator.h"
#include <stack>

namespace HTML
{
template<>
class CAnchor<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CAnchor(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CAbbr<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CAbbr(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CBreak<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CBreak(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CDivision<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
	std::stack<UINT> m_arFootnoteIDs;
public:
	CDivision(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CImage<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
	std::vector<std::wstring> m_arrImages;
public:
	CImage(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CFont<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CFont(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CInput<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CInput(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CBaseFont<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CBaseFont(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CBlockquote<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
	std::map<std::wstring, UINT>  m_mDivs;
public:
	CBlockquote(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CHorizontalRule<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
	UINT m_unShapeId;
public:
	CHorizontalRule(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CList<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
	UINT m_unNumberingId;
public:
	CList(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CListElement<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CListElement(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CCaption<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CCaption(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CTable<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CTable(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CTableRow<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CTableRow(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CTableCell<COOXMLInterpretator> : public CTag<COOXMLInterpretator>
{
public:
	CTableCell(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};
}

#endif // OOXMLTAGS_H
