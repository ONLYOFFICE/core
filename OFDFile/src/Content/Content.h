#ifndef CONTENT_H
#define CONTENT_H

#include "Layer.h"

namespace OFD
{
class CContent
{
	std::vector<CLayer*> m_arLayers;
public:
	CContent();
	~CContent();

	bool Read(XmlUtils::CXmlLiteReader& oLiteReader);
};
}

#endif // CONTENT_H
