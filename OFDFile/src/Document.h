#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Page.h"
#include "PublicRes.h"

#include "Utils/Types.h"

namespace OFD
{
class CPageArea
{
	TBox m_oPhysicalBox;
	TBox m_oApplicationBox;
	TBox m_oContentBox;
	TBox m_oBleedBox;
public:
	CPageArea();

	bool Read(CXmlReader& oLiteReader);
};

class CCommonData
{
	unsigned int m_unMaxUnitID;
	CPageArea    m_oPageArea;
	CPublicRes   m_oPublicRes;
public:
	CCommonData();

	bool Read(CXmlReader& oLiteReader);
};

class CPermission
{
	bool m_bEdit;
	bool m_bAnnot;
	bool m_bExport;
	bool m_bSignature;
	bool m_bWatermark;
	bool m_bPrintScreen;
public:
	CPermission();

	bool Read(CXmlReader& oLiteReader);
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
