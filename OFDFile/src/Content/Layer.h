#ifndef LAYER_H
#define LAYER_H

#include "IPageBlock.h"
#include "../Res.h"

namespace OFD
{
class CLayer : public IPageBlock
{
	enum class EType
	{
		Body,
		Foreground,
		Background
	} m_eType;

	unsigned int m_unID;
	std::vector<IPageBlock*> m_arPageBlocks;
public:
	CLayer(CXmlReader& oLiteReader, const CRes* pDocumentRes);
	~CLayer();

	void Draw(IRenderer* pRenderer, const CRes* pPublicRes) const override;
};
}

#endif // LAYER_H
