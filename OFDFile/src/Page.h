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

	const CTemplatePage* m_pTemplatePage;
public:
	CPage();
	~CPage();

	static CPage* Read(const std::wstring& wsFilePath, const CCommonData& oCommonData, NSFonts::IFontManager* pFontManager);
	void Draw(IRenderer* pRenderer) const;

	void GetPageSize(double& dWidth, double& dHeight) const;
};
}

#endif // PAGE_H
