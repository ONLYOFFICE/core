#include "Layer.h"

#include "../../../OOXML/Base/Unit.h"

#include "TextObject.h"

namespace OFD
{
CLayer::CLayer(CXmlReader& oLiteReader)
	: m_eType(EType::Body)
{
	Read(oLiteReader);
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

	if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
	{
		do
		{
			if (L"ID" == oLiteReader.GetName())
				m_unID = oLiteReader.GetUInteger(true);
		} while (oLiteReader.MoveToNextAttribute());

		oLiteReader.MoveToElement();
	}

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	IPageBlock* pPageBlock = nullptr;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();
		pPageBlock = nullptr;

		if (L"ofd:TextObject" == wsNodeName)
			pPageBlock = new CTextObject(oLiteReader);

		if (nullptr != pPageBlock)
			m_arPageBlocks.push_back(pPageBlock);
	}

	return true;
}
}
