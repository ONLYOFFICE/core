#include "Layer.h"

#include "../../../OOXML/Base/Unit.h"

#include "TextObject.h"
#include "PathObject.h"

namespace OFD
{
CLayer::CLayer(CXmlReader& oLiteReader)
	: IPageBlock(oLiteReader), m_eType(EType::Body)
{
	CLayer::Read(oLiteReader);
}

CLayer::~CLayer()
{
	for (IPageBlock* pPageBlock : m_arPageBlocks)
		delete pPageBlock;
}

bool CLayer::Read(CXmlReader& oLiteReader)
{
	if (L"ofd:Layer" != oLiteReader.GetName())
		return false;

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

		if (nullptr != pPageBlock)
			m_arPageBlocks.push_back(pPageBlock);
	}

	return true;
}

void CLayer::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer)
		return;

	for (const IPageBlock* pPageBlock : m_arPageBlocks)
		pPageBlock->Draw(pRenderer);
}
}
