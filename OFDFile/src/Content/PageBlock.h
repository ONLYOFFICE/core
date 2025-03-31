#ifndef PAGEBLOCK_H
#define PAGEBLOCK_H

#include "IPageBlock.h"
#include "../Res.h"

namespace OFD
{
class CPageBlock : public IPageBlock
{
	std::vector<IPageBlock*> m_arPageBlocks;
public:
	CPageBlock(CXmlReader& oLiteReader, const CRes* pDocumentRes);

	static void ReadIntoContainer(CXmlReader& oLiteReader, std::vector<IPageBlock*>& arPageBlocks, const CRes* pDocumentRes);

	void Draw(IRenderer* pRenderer, const CRes* pPublicRes) const override;
};
}

#endif // PAGEBLOCK_H
