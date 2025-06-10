#ifndef MULTIMEDIA_H
#define MULTIMEDIA_H

#include "../IOFDElement.h"

namespace OFD
{
class CMultiMedia : public IOFDElement
{
public:
	CMultiMedia(CXmlReader& oXmlReader, const std::wstring& wsRootPath);

	enum class EType
	{
		Image,
		Audio,
		Video
	};

	EType GetType() const;
	std::wstring GetFilePath() const;
private:
	EType        m_eType;
	std::wstring m_wsFilePath;
};
}

#endif // MULTIMEDIA_H
