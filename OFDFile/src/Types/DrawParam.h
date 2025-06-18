#ifndef DRAWPARAM_H
#define DRAWPARAM_H

#include "../IOFDElement.h"

namespace OFD
{
class CDrawParam : public IOFDElement
{
public:
	CDrawParam(CXmlReader& oXmlReader);
};
}
#endif // DRAWPARAM_H
