#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Page.h"
#include "Annotation.h"

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../OfficeUtils/src/ZipFolder.h"

namespace OFD
{
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
	CAnnotation m_oAnnotation;

	std::map<unsigned int, CPage*> m_mPages;
public:
	CDocument();
	~CDocument();

	bool Empty() const;

	bool Read(const std::wstring& wsFilePath, IFolder* pFolder);

	bool DrawPage(IRenderer* pRenderer, int nPageIndex) const;

	unsigned int GetPageCount() const;
	bool GetPageSize(int nPageIndex, double& dWidth, double &dHeight) const;
};
}

#endif // DOCUMENT_H
