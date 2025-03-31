#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Page.h"
#include "Res.h"

#include "Types/PageArea.h"

#include "../../DesktopEditor/graphics/IRenderer.h"

namespace OFD
{
class CCommonData
{
	unsigned int m_unMaxUnitID;
	CPageArea    m_oPageArea;
	CRes*        m_pPublicRes;
	CRes*        m_pDocumentRes;
public:
	CCommonData();
	~CCommonData();

	bool Read(CXmlReader& oLiteReader, const std::wstring& wsRootPath);

	void GetPageSize(double& dWidth, double &dHeight) const;

	const CRes* GetPublicRes()   const;
	const CRes* GetDocumentRes() const;
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
	CPermission m_oPermission;

	std::map<unsigned int, CPage*> m_mPages;
public:
	CDocument();
	~CDocument();

	bool Empty() const;

	bool Read(const std::wstring& wsFilePath);

	bool DrawPage(IRenderer* pRenderer, int nPageIndex) const;

	unsigned int GetPageCount() const;
	bool GetPageSize(int nPageIndex, double& dWidth, double &dHeight) const;
};
}

#endif // DOCUMENT_H
