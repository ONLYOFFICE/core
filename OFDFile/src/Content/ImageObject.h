#ifndef IMAGEOBJECT_H
#define IMAGEOBJECT_H

#include "IPageBlock.h"
#include "GraphicUnit.h"

namespace OFD
{
class CImageObject : public IPageBlock, public CGraphicUnit
{
	unsigned int m_unMultiMediaID;
public:
	CImageObject(CXmlReader& oLiteReader);

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData) const override;
};
}

#endif // IMAGEOBJECT_H
