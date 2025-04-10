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
	CPageBlock(CXmlReader& oLiteReader);

	static void ReadIntoContainer(CXmlReader& oLiteReader, std::vector<IPageBlock*>& arPageBlocks);

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData) const override;
};
}

#endif // PAGEBLOCK_H
