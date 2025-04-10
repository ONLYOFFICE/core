#ifndef COMMONDATA_H
#define COMMONDATA_H

#include "../Res.h"
#include "PageArea.h"
#include "TemplatePage.h"

#include "../../../DesktopEditor/graphics/pro/Fonts.h"

namespace OFD
{
class CCommonData
{
	unsigned int m_unMaxUnitID;
	CPageArea    m_oPageArea;
	CRes*        m_pPublicRes;
	CRes*        m_pDocumentRes;

	std::vector<const CTemplatePage*> m_arTemplatePages;
public:
	CCommonData();
	~CCommonData();

	bool Read(CXmlReader& oLiteReader, const std::wstring& wsRootPath);

	void GetPageSize(double& dWidth, double &dHeight) const;

	const CRes* GetPublicRes()   const;
	const CRes* GetDocumentRes() const;

	const CTemplatePage* GetTemplatePage(unsigned int unTemplateID, EZOrder eZOrder) const;
};
}

#endif // COMMONDATA_H
