#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Page.h"
#include "PublicRes.h"

namespace OFD
{
struct TBox
{
	double m_dX;
	double m_dY;
	double m_dWidth;
	double m_dHeight;

	TBox();

	bool Empty() const;
	bool Read(const std::wstring& wsValue);
};

class CPageArea
{
	TBox m_oPhysicalBox;
	TBox m_oApplicationBox;
	TBox m_oContentBox;
	TBox m_oBleedBox;
public:
	CPageArea();

	bool Read(XmlUtils::CXmlLiteReader& oLiteReader);
};

class CCommonData
{
	unsigned int m_unMaxUnitID;
	CPageArea    m_oPageArea;
	CPublicRes   m_oPublicRes;
public:
	CCommonData();

	bool Read(XmlUtils::CXmlLiteReader& oLiteReader);
};

class CDocument
{
	CCommonData m_oCommonData;

	std::map<unsigned int, CPage*> m_mPages;
public:
	CDocument();
	~CDocument();

	bool Empty() const;

	bool Read(const std::wstring& wsFilePath);
};
}

#endif // DOCUMENT_H
