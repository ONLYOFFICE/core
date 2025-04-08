#include "PageBlock.h"

#include "TextObject.h"
#include "PathObject.h"
#include "ImageObject.h"

namespace OFD
{
CPageBlock::CPageBlock(CXmlReader& oLiteReader, const CRes* pDocumentRes, const CRes* pPublicRes, NSFonts::IFontManager* pFontManager)
	: IPageBlock(oLiteReader)
{
	if ("ofd:PageBlock" != oLiteReader.GetNameA() || oLiteReader.IsEmptyNode())
		return;

	CPageBlock::ReadIntoContainer(oLiteReader, m_arPageBlocks, pDocumentRes, pPublicRes, pFontManager);
}

void CPageBlock::ReadIntoContainer(CXmlReader& oLiteReader, std::vector<IPageBlock*>& arPageBlocks, const CRes* pDocumentRes, const CRes* pPublicRes, NSFonts::IFontManager* pFontManager)
{
	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	IPageBlock* pPageBlock = nullptr;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();
		pPageBlock = nullptr;

		if (L"ofd:TextObject" == wsNodeName)
			pPageBlock = new CTextObject(oLiteReader, pPublicRes, pFontManager);
		else if (L"ofd:PathObject" == wsNodeName)
			pPageBlock = new CPathObject(oLiteReader, pPublicRes);
		else if (L"ofd:PageBlock" == wsNodeName)
			pPageBlock = new CPageBlock(oLiteReader, pDocumentRes, pPublicRes, pFontManager);
		else if (L"ofd:ImageObject" == wsNodeName)
			pPageBlock = new CImageObject(oLiteReader, pDocumentRes);

		if (nullptr != pPageBlock)
			arPageBlocks.push_back(pPageBlock);
	}
}

void CPageBlock::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer)
		return;

	for (const IPageBlock* pPageBlock : m_arPageBlocks)
		pPageBlock->Draw(pRenderer);
}
}
