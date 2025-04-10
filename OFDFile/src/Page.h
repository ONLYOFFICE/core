#ifndef PAGE_H
#define PAGE_H

#include "Content/Content.h"
#include "Types/CommonData.h"
#include "Types/TemplatePage.h"

namespace OFD
{
class CPage
{
	CPageArea m_oArea;
	CContent  m_oContent;

	std::pair<unsigned int, EZOrder> m_parTemplatePage;
public:
	CPage();
	~CPage();

	static CPage* Read(const std::wstring& wsFilePath);
	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData) const;

	void GetPageSize(double& dWidth, double& dHeight) const;
};
}

#endif // PAGE_H
