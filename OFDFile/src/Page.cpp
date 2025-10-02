#include "Page.h"

#include "Utils/Utils.h"

#include "../../DesktopEditor/common/File.h"

namespace OFD
{
CPage::CPage()
    : m_parTemplatePage{0, EZOrder::Background}
{}

CPage::~CPage()
{}

CPage* CPage::Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, wsRootPath))
		return nullptr;

	std::wstring wsNormalizedPath = CombinePaths(wsRootPath, wsFilePath);

	if (L"xml" != NSFile::GetFileExtention(wsNormalizedPath))
		wsNormalizedPath = CombinePaths(wsNormalizedPath, L"Content.xml");

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
		else if (L"ofd:Template" == wsNodeName && 0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
		{
			std::string sAttributeName;

			do
			{
				sAttributeName = oLiteReader.GetNameA();

				if ("ZOrder" == sAttributeName)
					pPage->m_parTemplatePage.second = GetZOrderFromString(oLiteReader.GetTextA());
				else if ("TemplateID" == sAttributeName)
					pPage->m_parTemplatePage.first = oLiteReader.GetUInteger(true);
			} while (oLiteReader.MoveToNextAttribute());

			oLiteReader.MoveToElement();
		}
	}

	return pPage;
}

void CPage::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer)
		return;

	pRenderer->BeginCommand(c_nImageType);

	if (0 != m_parTemplatePage.first)
	{
		const CTemplatePage *pTemplatePage = oCommonData.GetTemplatePage(m_parTemplatePage.first, m_parTemplatePage.second);

		if (nullptr != pTemplatePage && EZOrder::Background == pTemplatePage->GetZOrder() && nullptr != pTemplatePage->GetPage())
			pTemplatePage->GetPage()->Draw(pRenderer, oCommonData, EPageType::TemplatePage);
	}

	m_oContent.Draw(pRenderer, oCommonData, ePageType);

	pRenderer->EndCommand(c_nImageType);
}

void CPage::GetPageSize(double& dWidth, double& dHeight) const
{
	TBox oPhysicalBox{m_oArea.GetPhysicalBox()};

	if (oPhysicalBox.Empty())
		return;

	dWidth  = oPhysicalBox.m_dWidth;
	dHeight = oPhysicalBox.m_dHeight;
}
}
