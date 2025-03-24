#include "Base.h"

namespace OFD
{
#define IF_CHECK_NODE(node_name, varible_name)\
if (node_name == wsNodeName)\
	varible_name = oLiteReader.GetText2()

#define ELSE_IF_CHECK_NODE(node_name, varible_name)\
	else if (node_name == wsNodeName)\
		varible_name = oLiteReader.GetText2()

EDocUsege GetDocUsage(const std::wstring& wsValue)
{
	if (L"EBook" == wsValue)
		return EDocUsege::EBook;
	else if (L"ENewsPaper" == wsValue)
		return EDocUsege::ENewsPaper;
	else if (L"EMagnize" == wsValue)
		return EDocUsege::EMagzine;
	else
		return EDocUsege::Normal;
}

CDocInfo::CDocInfo()
	: m_eDocUsage(EDocUsege::Normal)
{}

bool CDocInfo::Read(CXmlReader& oLiteReader)
{
	if (L"ofd:DocInfo" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();

	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		IF_CHECK_NODE(L"ofd:DocID", m_wsDocId);
		ELSE_IF_CHECK_NODE(L"ofd:Title", m_wsTitle);
		ELSE_IF_CHECK_NODE(L"ofd:Author", m_wsAuthor);
		ELSE_IF_CHECK_NODE(L"ofd:Subject", m_wsSubject);
		ELSE_IF_CHECK_NODE(L"ofd:Abstruct", m_wsAbstact);
		ELSE_IF_CHECK_NODE(L"ofd:CreationDate", m_wsCreationDate);
		ELSE_IF_CHECK_NODE(L"ofd:ModDate", m_wsModDate);
		ELSE_IF_CHECK_NODE(L"ofd:Cover", m_wsCover);
		ELSE_IF_CHECK_NODE(L"ofd:Creator", m_wsCreator);
		ELSE_IF_CHECK_NODE(L"ofd:CreatorVersion", m_wsCreatorVersion);
		else if (L"ofd:DocUsage" == wsNodeName)
			m_eDocUsage = GetDocUsage(oLiteReader.GetText2());
	}

	return true;
}

CDocBody::CDocBody()
{

}

CDocBody* CDocBody::Read(CXmlReader& oLiteReader, IFolder* pFolder)
{
	if (L"ofd:DocBody" != oLiteReader.GetName())
		return nullptr;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	CDocBody *pDocBody = new CDocBody();

	if (nullptr == pDocBody)
		return nullptr;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:DocInfo" == wsNodeName)
		{
			if (!pDocBody->m_oDocInfo.Read(oLiteReader))
			{
				delete pDocBody;
				return nullptr;
			}
		}
		else if (L"ofd:DocRoot" == wsNodeName)
		{
			const std::wstring wsPath = NSSystemPath::ShortenPath(oLiteReader.GetText2());

			if (!wsPath.empty() && L'.' != wsPath.front())
				pDocBody->m_oDocument.Read(pFolder->getFullFilePath(wsPath));
		}
		ELSE_IF_CHECK_NODE(L"ofd:Signatures", pDocBody->m_wsSignature);
	}

	return pDocBody;
}

bool CDocBody::DrawPage(IRenderer* pRenderer, int nPageIndex) const
{
	return m_oDocument.DrawPage(pRenderer, nPageIndex);
}

unsigned int CDocBody::GetPageCount() const
{
	return m_oDocument.GetPageCount();
}

bool CDocBody::GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const
{
	return m_oDocument.GetPageSize(nPageIndex, dWidth, dHeight);
}

CBase::CBase()
{}

CBase::~CBase()
{
	for (CDocBody* pDocBody : m_arDocBodies)
		RELEASEOBJECT(pDocBody);
}

bool CBase::Read(IFolder* pFolder)
{
	if (nullptr == pFolder || !pFolder->existsXml(L"OFD.xml"))
		return false;

	CXmlReader oLiteReader;
	if (!oLiteReader.FromFile(pFolder->getFullFilePath(L"OFD.xml")) || !oLiteReader.ReadNextNode() || L"ofd:OFD" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();

	CDocBody* pDocBody = nullptr;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		pDocBody = CDocBody::Read(oLiteReader, pFolder);
		if (nullptr != pDocBody)
			m_arDocBodies.push_back(pDocBody);
	}

	return false;
}

void CBase::DrawPage(IRenderer* pRenderer, int nPageIndex) const
{
	for (const CDocBody* pDocBody : m_arDocBodies)
		if (pDocBody->DrawPage(pRenderer, nPageIndex))
			return;
}

unsigned int CBase::GetPageCount() const
{
	unsigned int unCount = 0;

	for (const CDocBody* pDocBody : m_arDocBodies)
		unCount += pDocBody->GetPageCount();

	return unCount;
}

void CBase::GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const
{
	for (const CDocBody* pDocBody : m_arDocBodies)
		if (pDocBody->GetPageSize(nPageIndex, dWidth, dHeight))
			return;
}
}
