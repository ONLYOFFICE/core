#include "Color.h"

namespace OFD
{
CColor::CColor(CXmlReader& oXmlReader)
    : m_oValue{0, 0, 0}, m_nIndex(0), m_unColorSpace(0), m_chAlpha(255)
{
	Read(oXmlReader);
}

bool CColor::Read(CXmlReader& oXmlReader)
{
	if (0 == oXmlReader.GetAttributesCount() || !oXmlReader.MoveToFirstAttribute())
		return false;

	std::string sAttributeName;

	do
	{
		sAttributeName = oXmlReader.GetNameA();

		if ("ColorSpace" == sAttributeName)
			m_unColorSpace = oXmlReader.GetUInteger(true);
		else if ("Value" == sAttributeName)
		{
			const std::vector<double> arValues{oXmlReader.GetArrayDoubles(true)};

			if (3 > arValues.size())
				continue;

			m_oValue.m_chRed   = static_cast<BYTE>(arValues[0]);
			m_oValue.m_chGreen = static_cast<BYTE>(arValues[1]);
			m_oValue.m_chBlue  = static_cast<BYTE>(arValues[2]);
		}
		else if ("Alpha" == sAttributeName)
			m_chAlpha = oXmlReader.GetUInteger(true);
		else if ("Index" == sAttributeName)
			m_nIndex = oXmlReader.GetInteger(true);
	} while (oXmlReader.MoveToNextAttribute());

	oXmlReader.MoveToElement();

	return true;
}

int CColor::ToInt() const
{
	return (255 << 24) | (m_oValue.m_chRed << 16) | (m_oValue.m_chGreen << 8) | (m_oValue.m_chBlue << 0);
}

BYTE CColor::GetAlpha() const
{
	return m_chAlpha;
}
}
