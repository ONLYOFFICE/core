#include "Document.h"

#include "Utils/Utils.h"

#include "../../DesktopEditor/common/Path.h"

namespace OFD
{
CPermission::CPermission()
	: m_bEdit(true), m_bAnnot(true), m_bExport(true),
	  m_bSignature(true), m_bWatermark(true), m_bPrintScreen(true)
{}

bool CPermission::Read(CXmlReader& oLiteReader)
{
	if (L"ofd:Permission" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:Edit" == wsNodeName)
			m_bEdit = oLiteReader.GetBoolean();
	}

	return true;
}

CDocument::CDocument()
{}

CDocument::~CDocument()
{
	for (std::pair<int, CPage*> oElement : m_mPages)
		delete oElement.second;
}

bool CDocument::Empty() const
{
	return m_mPages.empty();
}

bool CDocument::Read(const std::wstring& wsFilePath, IFolder* pFolder)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, pFolder->getFullFilePath(L"")))
		return false;

	CXmlReader oLiteReader;
	if (!oLiteReader.FromFile(pFolder->getFullFilePath(wsFilePath)) || !oLiteReader.ReadNextNode() || L"ofd:Document" != oLiteReader.GetName())
		return false;

	const std::wstring wsCoreDirectory{pFolder->getFullFilePath(NSSystemPath::GetDirectoryName(wsFilePath))};
	const int nDepth = oLiteReader.GetDepth();
	std::string sNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:CommonData" == sNodeName)
			m_oCommonData.Read(oLiteReader, wsCoreDirectory);
		else if ("ofd:Pages" == sNodeName)
		{
			const int nPagesDepth = oLiteReader.GetDepth();

			int nID = -1;
			std::wstring wsBaseLoc;


			while (oLiteReader.ReadNextSiblingNode(nPagesDepth))
			{
				if (L"ofd:Page" != oLiteReader.GetName() || 2 > oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
					continue;

				do
				{
					if (L"ID" == oLiteReader.GetName())
						nID = oLiteReader.GetInteger(true);
					else if (L"BaseLoc" == oLiteReader.GetName())
						wsBaseLoc = oLiteReader.GetText();
				}while (oLiteReader.MoveToNextAttribute());

				if (wsBaseLoc.empty())
					continue;

				if (-1 == nID)
					nID = m_mPages.size() + 1;

				CPage* pPage = CPage::Read(wsBaseLoc, wsCoreDirectory);

				if (nullptr != pPage)
					m_mPages.insert(std::make_pair(m_mPages.size(), pPage));

				wsBaseLoc.clear();
				oLiteReader.MoveToElement();
			}
		}
		else if ("ofd:Permissions" == sNodeName)
			m_oPermission.Read(oLiteReader);
		else if ("ofd:Annotations" == sNodeName)
			m_oAnnotation.Read(oLiteReader.GetText2(), wsCoreDirectory);
	}

	return false;
}

bool CDocument::DrawPage(IRenderer* pRenderer, int nPageIndex) const
{
	if (nullptr == pRenderer)
		return false;

	std::map<unsigned int, CPage*>::const_iterator itFound = m_mPages.find(nPageIndex);

	if (itFound == m_mPages.cend())
		return false;

	itFound->second->Draw(pRenderer, m_oCommonData);

	m_oAnnotation.Draw(pRenderer, m_oCommonData);

	return true;
}

unsigned int CDocument::GetPageCount() const
{
	return m_mPages.size();
}

bool CDocument::GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const
{
	m_oCommonData.GetPageSize(dWidth, dHeight);

	std::map<unsigned int, CPage*>::const_iterator itFound = m_mPages.find(nPageIndex);

	if (itFound == m_mPages.cend())
		return false;

	itFound->second->GetPageSize(dWidth, dHeight);

	return true;
}
}
