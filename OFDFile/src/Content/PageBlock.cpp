#include "PageBlock.h"

#include "TextObject.h"
#include "PathObject.h"
#include "ImageObject.h"

namespace OFD
{
CPageBlock::CPageBlock(CXmlReader& oLiteReader)
	: IPageBlock(oLiteReader)
{
	if (oLiteReader.MoveToFirstAttribute())
	{
		do
		{
			if ("Boundary" != oLiteReader.GetNameA())
				continue;

			m_oBoundary.Read(oLiteReader.GetTextA());
		} while (oLiteReader.MoveToNextAttribute());

		oLiteReader.MoveToElement();
	}

	if (oLiteReader.IsEmptyNode())
		return;

	CPageBlock::ReadIntoContainer(oLiteReader, m_arPageBlocks);
}

void CPageBlock::ReadIntoContainer(CXmlReader& oLiteReader, std::vector<IPageBlock*>& arPageBlocks)
{
	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	IPageBlock* pPageBlock = nullptr;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();
		pPageBlock = nullptr;

		if (L"ofd:TextObject" == wsNodeName)
			pPageBlock = new CTextObject(oLiteReader);
		else if (L"ofd:PathObject" == wsNodeName)
			pPageBlock = new CPathObject(oLiteReader);
		else if (L"ofd:PageBlock" == wsNodeName)
			pPageBlock = new CPageBlock(oLiteReader);
		else if (L"ofd:ImageObject" == wsNodeName)
			pPageBlock = new CImageObject(oLiteReader);

		if (nullptr != pPageBlock)
			arPageBlocks.push_back(pPageBlock);
	}
}

void CPageBlock::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer)
		return;

	double dM11, dM12, dM21, dM22, dDx, dDy;

	pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);
	pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx + m_oBoundary.m_dX, dDy + m_oBoundary.m_dY);

	for (const IPageBlock* pPageBlock : m_arPageBlocks)
		pPageBlock->Draw(pRenderer, oCommonData, ePageType);

	pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx, dDy);
}
}
