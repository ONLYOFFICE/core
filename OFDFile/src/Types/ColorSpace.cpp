#include "ColorSpace.h"

namespace OFD
{
CColorSpace::CColorSpace(CXmlReader& oXmlReader)
	: IOFDElement(oXmlReader)
{
	if (0 != oXmlReader.GetAttributesCount() && oXmlReader.MoveToFirstAttribute())
	{
		std::string sArgumentName;
		do
		{
			sArgumentName = oXmlReader.GetNameA();

			 if ("Type" == sArgumentName)
			{
				const std::string sValue{oXmlReader.GetTextA()};

				if ("Gray" == sValue)
					m_eType = EType::GRAY;
				else if ("RGB" == sValue)
					m_eType = EType::RGB;
				else if ("CMYK" == sValue)
					m_eType = EType::CMYK;
			}
		} while (oXmlReader.MoveToNextAttribute());
	}
	oXmlReader.MoveToElement();
}

CColorSpace::EType CColorSpace::GetType() const
{
	return m_eType;
}
}
