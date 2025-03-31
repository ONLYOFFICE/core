#include "Color.h"

namespace OFD
{
CColor::CColor(CXmlReader& oXmlReader)
    : m_oValues{0, 0, 0}, m_nIndex(0), m_unColorSpace(0), m_chAlpha(255)
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

			if (4 > arValues.size())
				continue;

			m_oValues[0] = static_cast<BYTE>(arValues[0]);
			m_oValues[1] = static_cast<BYTE>(arValues[1]);
			m_oValues[2] = static_cast<BYTE>(arValues[2]);
			m_oValues[3] = static_cast<BYTE>(arValues[3]);
		}
		else if ("Alpha" == sAttributeName)
			m_chAlpha = oXmlReader.GetUInteger(true);
		else if ("Index" == sAttributeName)
			m_nIndex = oXmlReader.GetInteger(true);
	} while (oXmlReader.MoveToNextAttribute());

	oXmlReader.MoveToElement();

	return true;
}

int CColor::ToInt(const CRes* pPublicRes) const
{
	if (nullptr != pPublicRes)
	{
		const CColorSpace *pColorSpace = pPublicRes->GetColorSpace(m_unColorSpace);

		if (nullptr != pColorSpace)
		{
			switch(pColorSpace->GetType())
			{
				case CColorSpace::EType::GRAY:
					return (255 << 24) | (128 << 16) | (128 << 8) | (128 << 0);
				case CColorSpace::EType::RGB:
					return (255 << 24) | (m_oValues[0] << 16) | (m_oValues[1] << 8) | (m_oValues[2] << 0);
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
		}
	}

	return (255 << 24) | (m_oValues[0] << 16) | (m_oValues[1] << 8) | (m_oValues[2] << 0);
}

BYTE CColor::GetAlpha() const
{
	return m_chAlpha;
}
}
