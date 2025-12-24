#include "Content.h"

namespace OFD
{
CContent::CContent()
{}

CContent::~CContent()
{
	for (CLayer* pLayer : m_arLayers)
		delete pLayer;
}

bool CContent::Read(CXmlReader& oLiteReader)
{
	if (L"ofd:Content" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:Layer" == wsNodeName)
			m_arLayers.push_back(new CLayer(oLiteReader));
	}

	return false;
}

void CContent::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer)
		return;

	for (const CLayer* pLayer : m_arLayers)
		pLayer->Draw(pRenderer, oCommonData, ePageType);
}
}
