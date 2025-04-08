#include "MultiMedia.h"

#include "../Utils/Utils.h"

namespace OFD
{
CMultiMedia::CMultiMedia(CXmlReader& oXmlReader, const std::wstring& wsRootPath)
	: IOFDElement(oXmlReader), m_eType(EType::Image)
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

				if ("Image" == sValue)
					m_eType = EType::Image;
				else if ("Audio" == sValue)
					m_eType = EType::Audio;
				else if ("Video" == sValue)
					m_eType = EType::Video;
			}
		} while (oXmlReader.MoveToNextAttribute());
	}

	oXmlReader.MoveToElement();

	if (oXmlReader.IsEmptyNode())
		return;

	const int nDepth = oXmlReader.GetDepth();

	while (oXmlReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:MediaFile" == oXmlReader.GetNameA())
		{
			m_wsFilePath = CombinePaths(wsRootPath, oXmlReader.GetText2());
			break;
		}
	}
}

CMultiMedia::EType CMultiMedia::GetType() const
{
	return m_eType;
}

std::wstring CMultiMedia::GetFilePath() const
{
	return m_wsFilePath;
}
}
