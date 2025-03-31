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

bool CContent::Read(CXmlReader& oLiteReader, const CRes* pDocumentRes)
{
	if (L"ofd:Content" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:Layer" == wsNodeName)
			m_arLayers.push_back(new CLayer(oLiteReader, pDocumentRes));
	}

	return false;
}

void CContent::Draw(IRenderer* pRenderer, const CRes* pPublicRes) const
{
	if (nullptr == pRenderer)
		return;

	for (const CLayer* pLayer : m_arLayers)
		pLayer->Draw(pRenderer, pPublicRes);
}
}
