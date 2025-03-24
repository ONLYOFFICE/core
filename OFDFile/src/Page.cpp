#include "Page.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Path.h"

namespace OFD
{
CPage::CPage()
{

}

CPage* CPage::Read(const std::wstring& wsFilePath)
{
	if (wsFilePath.empty())
		return nullptr;

	std::wstring wsNormalizedPath = wsFilePath;

	if (L"xml" != NSFile::GetFileExtention(wsNormalizedPath))
		wsNormalizedPath = NSSystemPath::Combine(wsNormalizedPath, L"Content.xml");

	CXmlReader oLiteReader;
	if (!oLiteReader.FromFile(wsNormalizedPath) || !oLiteReader.ReadNextNode() || L"ofd:Page" != oLiteReader.GetName())
		return nullptr;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	CPage *pPage = new CPage();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:Content" == wsNodeName)
			pPage->m_oContent.Read(oLiteReader);
		else if (L"ofd:Area" == wsNodeName)
			pPage->m_oArea.Read(oLiteReader);
	}

	return pPage;
}

void CPage::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer)
		return;

	pRenderer->BeginCommand(c_nImageType);

	m_oContent.Draw(pRenderer);

	pRenderer->EndCommand(c_nImageType);
}

void CPage::GetPageSize(double& dWidth, double& dHeight) const
{
	TBox oPhysicalBox{m_oArea.GetPhysicalBox()};

	if (oPhysicalBox.Empty())
		return;

	dWidth  = oPhysicalBox.m_dWidth  * 25.4 / 96.;
	dHeight = oPhysicalBox.m_dHeight * 25.4 / 96.;
}
}
