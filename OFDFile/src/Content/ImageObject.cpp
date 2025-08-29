#include "ImageObject.h"

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

void CImageObject::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer || nullptr == oCommonData.GetDocumentRes())
		return;

	const CMultiMedia* pMultiMedia = oCommonData.GetDocumentRes()->GetMultiMedia(m_unMultiMediaID);

	if (nullptr == pMultiMedia)
		return;

	TMatrix oOldTransform;
	CGraphicUnit::Apply(pRenderer, oOldTransform);

	const std::wstring wsFilePath = pMultiMedia->GetFilePath();

	if (wsFilePath.empty())
		return;

	pRenderer->DrawImageFromFile(wsFilePath, 0, 0, 1, 1);

	pRenderer->SetTransform(oOldTransform.m_dM11, oOldTransform.m_dM12, oOldTransform.m_dM21, oOldTransform.m_dM22, oOldTransform.m_dDx, oOldTransform.m_dDy);
}
}
