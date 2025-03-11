#include "Types.h"

#include <vector>
#include "../../../DesktopEditor/common/StringExt.h"
#include "../../../OOXML/Base/Unit.h"

namespace OFD
{
TBox::TBox()
	: m_dX(0.), m_dY(0.), m_dWidth(0.), m_dHeight(0.)
{}

bool TBox::Empty() const
{
	return m_dWidth < OFD_EPSILON || m_dHeight < OFD_EPSILON;
}

bool TBox::Read(const std::wstring& wsValue)
{
	const std::vector<std::wstring> arValues{NSStringExt::Split(wsValue, L' ')};

	if (4 > arValues.size())
		return false;

	m_dX      = XmlUtils::GetDouble(arValues[0]);
	m_dY      = XmlUtils::GetDouble(arValues[1]);
	m_dWidth  = XmlUtils::GetDouble(arValues[2]);
	m_dHeight = XmlUtils::GetDouble(arValues[3]);

	return true;
}



TColor::TColor()
{
}

TColor::TColor(XmlUtils::CXmlLiteReader& oLiteReader)
{
	Read(oLiteReader);
}

bool TColor::Read(XmlUtils::CXmlLiteReader& oLiteReader)
{
	return false;
}

}
