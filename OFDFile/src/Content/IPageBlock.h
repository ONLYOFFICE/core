#ifndef IPAGEBLOCK_H
#define IPAGEBLOCK_H

#include "../IOFDElement.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"
#include "../Res.h"

namespace OFD
{
class IPageBlock : public IOFDElement
{
public:
	IPageBlock(CXmlReader& oLiteReader)
		: IOFDElement(oLiteReader){};
	virtual ~IPageBlock(){};
	virtual void Draw(IRenderer* pRenderer, const CRes* pPublicRes) const = 0;
};
}

#endif // IPAGEBLOCK_H
