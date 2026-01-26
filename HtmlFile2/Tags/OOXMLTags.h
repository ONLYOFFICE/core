#ifndef OOXMLTAGS_H
#define OOXMLTAGS_H

#include "HTMLTags.h"
#include "../Interpretators/OOXMLInterpretator.h"
#include <stack>

namespace XmlUtils { class CXmlLiteReader; }

namespace HTML
{
template<>
struct TAnchor<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TAnchor(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TAbbr<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TAbbr(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TBreak<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TBreak(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TDivision<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	std::stack<UINT> m_arFootnoteIDs;

	TDivision(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TImage<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	std::vector<std::wstring> m_arrImages;
	XmlUtils::CXmlLiteReader* m_pXmlReader; //TODO:: подумать как можно иначе получить текс в svg теге

	TImage(COOXMLInterpretator* pInterpretator, XmlUtils::CXmlLiteReader* pXmlReader);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TFont<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TFont(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TInput<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TInput(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TBaseFont<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TBaseFont(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TBlockquote<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	std::map<std::wstring, UINT>  m_mDivs;

	TBlockquote(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct THorizontalRule<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	UINT m_unShapeId;

	THorizontalRule(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TList<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	UINT m_unNumberingId;

	TList(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

template<>
struct TListElement<COOXMLInterpretator> : public TTag<COOXMLInterpretator>
{
	TListElement(COOXMLInterpretator* pInterpretator);
	virtual bool Open(const std::vector<NSCSS::CNode>& arSelectors) override;
	virtual void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};
}

#endif // OOXMLTAGS_H
