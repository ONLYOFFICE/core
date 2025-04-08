#include "Page.h"

#include "Utils/Utils.h"

#include "../../DesktopEditor/common/File.h"

namespace OFD
{
CPage::CPage()
	: m_pTemplatePage(nullptr)
{}

CPage::~CPage()
{
	if (nullptr != m_pTemplatePage)
		delete m_pTemplatePage;
}

CPage* CPage::Read(const std::wstring& wsFilePath, const CCommonData& oCommonData, NSFonts::IFontManager* pFontManager)
{
	if (wsFilePath.empty())
		return nullptr;

	std::wstring wsNormalizedPath = wsFilePath;

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
			pPage->m_oContent.Read(oLiteReader, oCommonData.GetDocumentRes(), oCommonData.GetPublicRes(), pFontManager);
		else if (L"ofd:Area" == wsNodeName)
			pPage->m_oArea.Read(oLiteReader);
		else if (L"ofd:Template" == wsNodeName && 0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
		{
			EZOrder eZorder;
			unsigned int unTemplateID;

			std::string sAttributeName;

			do
			{
				sAttributeName = oLiteReader.GetNameA();

				if ("ZOrder" == sAttributeName)
					eZorder = GetZOrderFromString(oLiteReader.GetTextA());
				else if ("TemplateID" == sAttributeName)
					unTemplateID = oLiteReader.GetUInteger(true);
			} while (oLiteReader.MoveToNextAttribute());

			oLiteReader.MoveToElement();

			pPage->m_pTemplatePage = oCommonData.GetTemplatePage(unTemplateID, eZorder);
		}
	}

	return pPage;
}

void CPage::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer)
		return;

	pRenderer->BeginCommand(c_nImageType);

	if (nullptr != m_pTemplatePage && EZOrder::Background == m_pTemplatePage->GetZOrder())
		m_pTemplatePage->GetPage()->Draw(pRenderer);

	m_oContent.Draw(pRenderer);

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
