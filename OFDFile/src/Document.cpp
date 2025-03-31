#include "Document.h"

namespace OFD
{
CCommonData::CCommonData()
	: m_unMaxUnitID(0), m_pPublicRes(nullptr), m_pDocumentRes(nullptr)
{}

CCommonData::~CCommonData()
{
	if (nullptr != m_pPublicRes)
		delete m_pPublicRes;

	if (nullptr != m_pDocumentRes)
		delete m_pDocumentRes;
}

bool CCommonData::Read(CXmlReader& oLiteReader, const std::wstring& wsRootPath)
{
	if (L"ofd:CommonData" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:PageArea" == wsNodeName)
			m_oPageArea.Read(oLiteReader);
		else if (L"ofd:PublicRes" == wsNodeName)
		{
			if (nullptr == m_pPublicRes)
				m_pPublicRes = new CRes();

			m_pPublicRes->Read(NSSystemPath::Combine(wsRootPath, oLiteReader.GetText2()));
		}
		else if (L"ofd:DocumentRes" == wsNodeName)
		{
			if(nullptr == m_pDocumentRes)
				m_pDocumentRes = new CRes();

			m_pDocumentRes->Read(NSSystemPath::Combine(wsRootPath, oLiteReader.GetText2()));
		}
		else if (L"ofd:MaxUnitID" == wsNodeName)
			m_unMaxUnitID = oLiteReader.GetUInteger();
		// else if (L"ofd:TemplatePage" == wsNodeName)
		// else if (L"ofd:DefaultCS" == wsNodeName)
	}

	return true;
}

void CCommonData::GetPageSize(double& dWidth, double& dHeight) const
{
	TBox oPhysicalBox{m_oPageArea.GetPhysicalBox()};

	if (oPhysicalBox.Empty())
		return;

	dWidth  = oPhysicalBox.m_dWidth;
	dHeight = oPhysicalBox.m_dHeight;
}

const CRes* CCommonData::GetPublicRes() const
{
	return m_pPublicRes;
}

const CRes* CCommonData::GetDocumentRes() const
{
	return m_pDocumentRes;
}

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

bool CDocument::Read(const std::wstring& wsFilePath)
{
	if (wsFilePath.empty())
		return false;

	CXmlReader oLiteReader;
	if (!oLiteReader.FromFile(wsFilePath) || !oLiteReader.ReadNextNode() || L"ofd:Document" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:CommonData" == wsNodeName)
			m_oCommonData.Read(oLiteReader, NSSystemPath::GetDirectoryName(wsFilePath));
		else if (L"ofd:Pages" == wsNodeName)
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

				CPage* pPage = CPage::Read(NSSystemPath::Combine(NSSystemPath::GetDirectoryName(wsFilePath), wsBaseLoc), m_oCommonData.GetDocumentRes());

				if (nullptr != pPage)
					m_mPages.insert(std::make_pair(m_mPages.size(), pPage));

				wsBaseLoc.clear();
				oLiteReader.MoveToElement();
			}
		}
		else if (L"ofd:Permissions" == wsNodeName)
			m_oPermission.Read(oLiteReader);
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

	itFound->second->Draw(pRenderer, m_oCommonData.GetPublicRes());

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
