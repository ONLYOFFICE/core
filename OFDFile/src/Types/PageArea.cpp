#include "PageArea.h"

namespace OFD
{
CPageArea::CPageArea()
{}

bool CPageArea::Read(CXmlReader& oLiteReader)
{
	if (L"ofd:PageArea" != oLiteReader.GetName() &&
	    L"ofd:Area"     != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:PhysicalBox" == wsNodeName)
			m_oPhysicalBox.Read(oLiteReader.GetText2A());
		else if (L"ofd:ApplicationBox" == wsNodeName)
			m_oApplicationBox.Read(oLiteReader.GetText2A());
		else if (L"ofd:ContentBox" == wsNodeName)
			m_oContentBox.Read(oLiteReader.GetText2A());
		else if (L"ofd:BleedBox" == wsNodeName)
			m_oBleedBox.Read(oLiteReader.GetText2A());
	}

	return true;
}

TBox CPageArea::GetPhysicalBox() const
{
	return m_oPhysicalBox;
}
}
