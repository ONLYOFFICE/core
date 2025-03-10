#ifndef PAGE_H
#define PAGE_H

#include "../../OfficeUtils/src/ZipFolder.h"

namespace OFD
{
class CPage
{
public:
	CPage();

	static CPage* Read(const std::wstring& wsFilePath);
};
}

#endif // PAGE_H
