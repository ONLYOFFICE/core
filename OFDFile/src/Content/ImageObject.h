#ifndef IMAGEOBJECT_H
#define IMAGEOBJECT_H

#include "IPageBlock.h"
#include "GraphicUnit.h"

#include "../Types/MultiMedia.h"
#include "../Res.h"

namespace OFD
{
class CImageObject : public IPageBlock, public CGraphicUnit
{
	const CMultiMedia* m_pMultiMedia;
public:
	CImageObject(CXmlReader& oLiteReader, const CRes* pDocumentRes);

	void Draw(IRenderer* pRenderer, const CRes* pPublicRes) const override;
};
}

#endif // IMAGEOBJECT_H
