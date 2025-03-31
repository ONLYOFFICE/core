#include "ImageObject.h"

namespace OFD
{
CImageObject::CImageObject(CXmlReader& oLiteReader, const CRes* pDocumentRes)
	: IPageBlock(oLiteReader), CGraphicUnit(oLiteReader)
{
	if (nullptr == pDocumentRes || "ofd:ImageObject" != oLiteReader.GetNameA() || 0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;

	do
	{
		sAttributeName = oLiteReader.GetNameA();

		if ("ResourceID" == sAttributeName)
		{
			m_pMultiMedia = pDocumentRes->GetMultiMedia(oLiteReader.GetUInteger(true));
			break;
		}
	} while(oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();
}

void CImageObject::Draw(IRenderer* pRenderer, const CRes* pPublicRes) const
{
	if (nullptr == pRenderer || nullptr == m_pMultiMedia)
		return;

	CGraphicUnit::Apply(pRenderer);

	const std::wstring wsFilePath = m_pMultiMedia->GetFilePath();

	if (wsFilePath.empty())
		return;

	const TBox oBoundry{GetBoundary()};

	pRenderer->DrawImageFromFile(wsFilePath, 0, 0, oBoundry.m_dWidth, oBoundry.m_dHeight);
}
}
