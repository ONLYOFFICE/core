#ifndef PAGE_H
#define PAGE_H

#include "Content/Content.h"
#include "Types/PageArea.h"

namespace OFD
{
class CPage
{
	CPageArea m_oArea;
	CContent  m_oContent;
public:
	CPage();

	static CPage* Read(const std::wstring& wsFilePath);
	void Draw(IRenderer* pRenderer) const;

	void GetPageSize(double& dWidth, double& dHeight) const;
};
}

#endif // PAGE_H
