#ifndef OOXMLTAGS_H
#define OOXMLTAGS_H

#include "HTMLTags.h"
#include "../Writers/OOXMLWriter.h"

namespace HTML
{
template<>
class CAnchor<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CAnchor(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CAbbr<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CAbbr(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CBreak<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CBreak(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CDivision<COOXMLWriter> : public CTag<COOXMLWriter>
{
	std::stack<UINT> m_arFootnoteIDs;
public:
	CDivision(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CImage<COOXMLWriter> : public CTag<COOXMLWriter>
{
	std::vector<std::wstring> m_arrImages;
public:
	CImage(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CFont<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CFont(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CInput<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CInput(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CBaseFont<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CBaseFont(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CBlockquote<COOXMLWriter> : public CTag<COOXMLWriter>
{
	std::map<std::wstring, UINT>  m_mDivs;
public:
	CBlockquote(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CHorizontalRule<COOXMLWriter> : public CTag<COOXMLWriter>
{
	UINT m_unShapeId;
public:
	CHorizontalRule(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CList<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CList(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CListElement<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CListElement(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CCaption<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CCaption(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CTable<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CTable(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CTableRow<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CTableRow(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CTableCell<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CTableCell(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CHTML<COOXMLWriter> : public CTag<COOXMLWriter>
{
public:
	CHTML(COOXMLWriter* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};
}

#endif // OOXMLTAGS_H
