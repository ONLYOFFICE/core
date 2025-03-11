#ifndef IPAGEBLOCK_H
#define IPAGEBLOCK_H

#include "../../../DesktopEditor/xml/include/xmlutils.h"

namespace OFD
{
class IPageBlock
{
	unsigned int m_unID;
public:
	IPageBlock(){};
	virtual ~IPageBlock(){};
	virtual bool Read(XmlUtils::CXmlLiteReader& oLiteReader) = 0;
};
}

#endif // IPAGEBLOCK_H
