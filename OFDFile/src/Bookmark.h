#ifndef BOOKMARK_H
#define BOOKMARK_H

#include "Utils/Types.h"

namespace OFD
{
class CBookmark
{
	std::wstring m_wsName;
	const TDest* m_pDest;
public:
	CBookmark(CXmlReader& oReader);
	~CBookmark();

	const std::wstring GetName() const;
	const TDest* GetDest() const;
};
}

#endif // BOOKMARK_H
