#include "ImageObject.h"

#include "../../../DesktopEditor/graphics/Image.h"

namespace OFD
{
CImageObject::CImageObject(CXmlReader& oLiteReader)
	: IPageBlock(oLiteReader), CGraphicUnit(oLiteReader), m_unMultiMediaID(0)
{
	if ("ofd:ImageObject" != oLiteReader.GetNameA() || 0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;

	do
	{
		sAttributeName = oLiteReader.GetNameA();

		if ("ResourceID" == sAttributeName)
		{
			m_unMultiMediaID = oLiteReader.GetUInteger(true);
			break;
		}
	} while(oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();
}

void CImageObject::Draw(IRenderer* pRenderer, const CCommonData& oCommonData) const
{
	if (nullptr == pRenderer || nullptr == oCommonData.GetDocumentRes())
		return;

	const CMultiMedia* pMultiMedia = oCommonData.GetDocumentRes()->GetMultiMedia(m_unMultiMediaID);

	if (nullptr == pMultiMedia)
		return;

	CGraphicUnit::Apply(pRenderer);

	const std::wstring wsFilePath = pMultiMedia->GetFilePath();

	if (wsFilePath.empty())
		return;

	pRenderer->DrawImageFromFile(wsFilePath, 0, 0, 1, 1);
}
}
