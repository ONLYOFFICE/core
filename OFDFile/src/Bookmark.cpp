#include "Bookmark.h"

#include "Utils/XmlReader.h"

namespace OFD
{
CBookmark::CBookmark(CXmlReader& oReader)
{
	if (oReader.MoveToFirstAttribute())
	{
		do
		{
			if ("Name" == oReader.GetNameA())
				m_wsName = oReader.GetText();
		}while(oReader.MoveToNextAttribute());

		oReader.MoveToElement();
	}

	const int nDepth{oReader.GetDepth()};

	while (oReader.ReadNextSiblingNode2(nDepth))
	{
		if ("ofd:Dest" == oReader.GetNameA())
		{
			m_pDest = TDest::Read(oReader);

			if (nullptr != m_pDest)
				return;
		}
	}
}

CBookmark::~CBookmark()
{
	if (nullptr != m_pDest)
		delete m_pDest;
}

const std::wstring OFD::CBookmark::GetName() const
{
	return m_wsName;
}

const TDest* CBookmark::GetDest() const
{
	return m_pDest;
}
}
