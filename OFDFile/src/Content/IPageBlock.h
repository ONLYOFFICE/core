#ifndef IPAGEBLOCK_H
#define IPAGEBLOCK_H

#include "../Utils/XmlReader.h"

namespace OFD
{
class IPageBlock
{
	unsigned int m_unID;
public:
	IPageBlock(){};
	virtual ~IPageBlock(){};
	virtual bool Read(CXmlReader& oLiteReader) = 0;
};
}

#endif // IPAGEBLOCK_H
