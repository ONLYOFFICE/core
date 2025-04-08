#include "Layer.h"

#include "../../../OOXML/Base/Unit.h"

#include "PageBlock.h"

namespace OFD
{
CLayer::CLayer(CXmlReader& oLiteReader, const CRes* pDocumentRes, const CRes* pPublicRes, NSFonts::IFontManager* pFontManager)
	: IPageBlock(oLiteReader), m_eType(EType::Body)
{
	if (L"ofd:Layer" != oLiteReader.GetName())
		return;

	CPageBlock::ReadIntoContainer(oLiteReader, m_arPageBlocks, pDocumentRes, pPublicRes, pFontManager);
}

CLayer::~CLayer()
{
	for (IPageBlock* pPageBlock : m_arPageBlocks)
		delete pPageBlock;
}

void CLayer::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer)
		return;

	for (const IPageBlock* pPageBlock : m_arPageBlocks)
		pPageBlock->Draw(pRenderer);
}
}
