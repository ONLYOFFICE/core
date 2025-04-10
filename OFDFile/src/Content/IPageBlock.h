#ifndef IPAGEBLOCK_H
#define IPAGEBLOCK_H

#include "../IOFDElement.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"
#include "../Types/CommonData.h"

namespace OFD
{
class IPageBlock : public IOFDElement
{
public:
	IPageBlock(CXmlReader& oLiteReader)
		: IOFDElement(oLiteReader){};
	virtual ~IPageBlock(){};
	virtual void Draw(IRenderer* pRenderer, const CCommonData& oCommonData) const = 0;
};
}

#endif // IPAGEBLOCK_H
