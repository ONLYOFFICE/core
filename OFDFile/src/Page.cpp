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
	}

	return pPage;
}
}
