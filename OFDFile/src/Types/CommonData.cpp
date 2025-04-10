#include "CommonData.h"
#include "../Utils/Utils.h"

#include "../../../DesktopEditor/common/Path.h"

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
	if ("ofd:CommonData" != oLiteReader.GetNameA())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::string sNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:PageArea" == sNodeName)
			m_oPageArea.Read(oLiteReader);
		else if ("ofd:PublicRes" == sNodeName)
		{
			if (nullptr == m_pPublicRes)
				m_pPublicRes = new CRes();

			m_pPublicRes->Read(CombinePaths(wsRootPath, oLiteReader.GetText2()));
		}
		else if ("ofd:DocumentRes" == sNodeName)
		{
			if(nullptr == m_pDocumentRes)
				m_pDocumentRes = new CRes();

			m_pDocumentRes->Read(CombinePaths(wsRootPath, oLiteReader.GetText2()));
		}
		else if ("ofd:MaxUnitID" == sNodeName)
			m_unMaxUnitID = oLiteReader.GetUInteger();
		else if ("ofd:TemplatePage" == sNodeName)
			AddToContainer(new const CTemplatePage(oLiteReader, wsRootPath), m_arTemplatePages);
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

const CTemplatePage* CCommonData::GetTemplatePage(unsigned int unTemplateID, EZOrder eZOrder) const
{
	for (const CTemplatePage* pTemplatePage : m_arTemplatePages)
	{
		if (unTemplateID == pTemplatePage->GetID() && eZOrder == pTemplatePage->GetZOrder())
			return pTemplatePage;
	}

	return nullptr;
}
}
