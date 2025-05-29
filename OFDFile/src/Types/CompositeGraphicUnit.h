#ifndef COMPOSITEGRAPHICUNIT_H
#define COMPOSITEGRAPHICUNIT_H

#include "../IOFDElement.h"

namespace OFD
{
class CCompositeGraphicUnit : public IOFDElement
{
public:
	CCompositeGraphicUnit(CXmlReader& oXmlReader);
};
}

#endif // COMPOSITEGRAPHICUNIT_H
