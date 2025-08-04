#include "Color.h"

#include "../Res.h"

namespace OFD
{
CColor::CColor(CXmlReader& oXmlReader)
    : m_oValues{0, 0, 0, 0}, m_nIndex(0), m_chAlpha(255), m_unColodSpaceID(0)
{
	if (0 == oXmlReader.GetAttributesCount() || !oXmlReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;

	do
	{
		sAttributeName = oXmlReader.GetNameA();

		if ("ColorSpace" == sAttributeName)
			m_unColodSpaceID = oXmlReader.GetUInteger(true);
		else if ("Value" == sAttributeName)
		{
			const std::vector<double> arValues{oXmlReader.GetArrayDoubles(true)};

			for (unsigned int unIndex = 0; unIndex < (std::min)((unsigned int)arValues.size(), (unsigned int)4); ++unIndex)
				m_oValues[unIndex] = static_cast<BYTE>(arValues[unIndex]);
		}
		else if ("Alpha" == sAttributeName)
			m_chAlpha = oXmlReader.GetUInteger(true);
		else if ("Index" == sAttributeName)
			m_nIndex = oXmlReader.GetInteger(true);
	} while (oXmlReader.MoveToNextAttribute());

	oXmlReader.MoveToElement();
}

int CColor::ToInt(const CRes* pPublicRes) const
{
	if (nullptr == pPublicRes)
		return 0;

	const CColorSpace* pColorSpace = pPublicRes->GetColorSpace(m_unColodSpaceID);

	CColorSpace::EType eColoSpaceType{CColorSpace::EType::RGB};

	if (nullptr != pColorSpace)
		eColoSpaceType = pColorSpace->GetType();

	switch(eColoSpaceType)
	{
		case CColorSpace::EType::GRAY:
			return (255 << 24) | (m_oValues[0] << 16) | (m_oValues[0] << 8) | (m_oValues[0] << 0);
		case CColorSpace::EType::RGB:
			return (255 << 24) | (m_oValues[2] << 16) | (m_oValues[1] << 8) | (m_oValues[0] << 0);
		case CColorSpace::EType::CMYK:
		{
			const float cF = m_oValues[0] / 255.0f;
			const float mF = m_oValues[1] / 255.0f;
			const float yF = m_oValues[2] / 255.0f;
			const float kF = m_oValues[3] / 255.0f;

			const float r = (1.0f - cF) * (1.0f - kF);
			const float g = (1.0f - mF) * (1.0f - kF);
			const float b = (1.0f - yF) * (1.0f - kF);

			const unsigned char rByte = static_cast<unsigned char>(r * 255);
			const unsigned char gByte = static_cast<unsigned char>(g * 255);
			const unsigned char bByte = static_cast<unsigned char>(b * 255);

			return (rByte << 0) | (gByte << 8) | (bByte << 16);
		}
	}

	return 0;
}

BYTE CColor::GetAlpha() const
{
	return m_chAlpha;
}
}
