#include "Document.h"

namespace OFD
{
CPageArea::CPageArea()
{}

bool CPageArea::Read(CXmlReader& oLiteReader)
{
	if (L"ofd:PageArea" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:PhysicalBox" == wsNodeName)
			m_oPhysicalBox.Read(oLiteReader.GetText2A());
		else if (L"ofd:ApplicationBox" == wsNodeName)
			m_oApplicationBox.Read(oLiteReader.GetText2A());
		else if (L"ofd:ContentBox" == wsNodeName)
			m_oContentBox.Read(oLiteReader.GetText2A());
		else if (L"ofd:BleedBox" == wsNodeName)
			m_oBleedBox.Read(oLiteReader.GetText2A());
	}

	return true;
}

CCommonData::CCommonData()
	: m_unMaxUnitID(0)
{}

bool CCommonData::Read(CXmlReader& oLiteReader)
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
			// m_oPublicRes.Read();
		}
		else if (L"ofd:MaxUnitID" == wsNodeName)
			m_unMaxUnitID = oLiteReader.GetUInteger();
		// else if (L"ofd:DocumentRes" == wsNodeName)
		// else if (L"ofd:TemplatePage" == wsNodeName)
		// else if (L"ofd:DefaultCS" == wsNodeName)
	}

	return true;
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
			m_oCommonData.Read(oLiteReader);
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

				CPage* pPage = CPage::Read(NSSystemPath::Combine(NSSystemPath::GetDirectoryName(wsFilePath), wsBaseLoc));

				if (nullptr != pPage)
					m_mPages.insert(std::make_pair(nID, pPage));

				wsBaseLoc.clear();
				oLiteReader.MoveToElement();
			}
		}
	}

	return false;
}
}
