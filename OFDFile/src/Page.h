#ifndef PAGE_H
#define PAGE_H

#include "Content/Content.h"

namespace OFD
{
class CPage
{
	CContent m_oContent;
public:
	CPage();

	static CPage* Read(const std::wstring& wsFilePath);
};
}

#endif // PAGE_H
