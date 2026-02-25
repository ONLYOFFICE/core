#ifndef MDTAGS_H
#define MDTAGS_H

#include "HTMLTags.h"
#include "../Writers/MDWriter.h"

namespace HTML
{
template<>
class CAnchor<CMDWriter> : public CTag<CMDWriter>
{
public:
	CAnchor(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CBold<CMDWriter> : public CTag<CMDWriter>
{
public:
	CBold(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CBreak<CMDWriter> : public CTag<CMDWriter>
{
public:
	CBreak(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CItalic<CMDWriter> : public CTag<CMDWriter>
{
public:
	CItalic(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CStrike<CMDWriter> : public CTag<CMDWriter>
{
public:
	CStrike(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CQuotation<CMDWriter> : public CTag<CMDWriter>
{
public:
	CQuotation(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CPreformatted<CMDWriter> : public CTag<CMDWriter>
{
public:
	CPreformatted(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CHeader<CMDWriter> : public CTag<CMDWriter>
{
public:
	CHeader(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CImage<CMDWriter> : public CTag<CMDWriter>
{
public:
	CImage(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CHorizontalRule<CMDWriter> : public CTag<CMDWriter>
{
public:
	CHorizontalRule(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CBlockquote<CMDWriter> : public CTag<CMDWriter>
{
public:
	CBlockquote(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CTable<CMDWriter> : public CTag<CMDWriter>
{
public:
	CTable(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CTableRow<CMDWriter> : public CTag<CMDWriter>
{
	UINT m_unLastRowType;
public:
	CTableRow(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CTableCell<CMDWriter> : public CTag<CMDWriter>
{
	UINT m_unNeedEmptyCells;
public:
	CTableCell(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CList<CMDWriter> : public CTag<CMDWriter>
{
public:
	CList(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CListElement<CMDWriter> : public CTag<CMDWriter>
{
public:
	CListElement(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
class CCode<CMDWriter> : public CTag<CMDWriter>
{
public:
	CCode(CMDWriter* pWriter);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors, const boost::any& oExtraData = boost::any()) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};
}

#endif // MDTAGS_H
