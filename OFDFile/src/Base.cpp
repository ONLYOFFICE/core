#include "Base.h"
#include "Utils/Utils.h"

namespace OFD
{
#define IF_CHECK_NODE(node_name, varible_name)\
if (node_name == sNodeName)\
	varible_name = oLiteReader.GetText2()

#define ELSE_IF_CHECK_NODE(node_name, varible_name)\
else if (node_name == sNodeName)\
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

	std::string sNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		IF_CHECK_NODE("ofd:DocID", m_wsDocId);
		ELSE_IF_CHECK_NODE("ofd:Title", m_wsTitle);
		ELSE_IF_CHECK_NODE("ofd:Author", m_wsAuthor);
		ELSE_IF_CHECK_NODE("ofd:Subject", m_wsSubject);
		ELSE_IF_CHECK_NODE("ofd:Abstruct", m_wsAbstact);
		ELSE_IF_CHECK_NODE("ofd:CreationDate", m_wsCreationDate);
		ELSE_IF_CHECK_NODE("ofd:ModDate", m_wsModDate);
		ELSE_IF_CHECK_NODE("ofd:Cover", m_wsCover);
		ELSE_IF_CHECK_NODE("ofd:Creator", m_wsCreator);
		ELSE_IF_CHECK_NODE("ofd:CreatorVersion", m_wsCreatorVersion);
		else if ("ofd:DocUsage" == sNodeName)
			m_eDocUsage = GetDocUsage(oLiteReader.GetText2());
	}

	return true;
}

void CDocBody::ReadSignatures(const std::wstring& wsFilePath, const std::wstring& wsRootPath)
{
	CXmlReader oLiteReader;
	if (!oLiteReader.FromFile(CombinePaths(wsRootPath, wsFilePath)) || !oLiteReader.ReadNextNode() || L"ofd:Signatures" != oLiteReader.GetName())
		return;

	const int nDepth = oLiteReader.GetDepth();
	std::string sNodeName;
	unsigned int unMaxSignId = 0;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:MaxSignId" == sNodeName)
			unMaxSignId = oLiteReader.GetUInteger();
		else if ("ofd:Signature" == sNodeName)
		{
			if (0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
				continue;

			do
			{
				if ("BaseLoc" == oLiteReader.GetNameA())
					AddToContainer(CSignature::Read(oLiteReader.GetText(), wsRootPath), m_arSignatures);
			} while (oLiteReader.MoveToNextAttribute());

			oLiteReader.MoveToElement();
		}
	}
}

CDocBody::CDocBody()
{}

CDocBody::~CDocBody()
{
	ClearContainer(m_arSignatures);
}

CDocBody* CDocBody::Read(CXmlReader& oLiteReader, IFolder* pFolder)
{
	if (L"ofd:DocBody" != oLiteReader.GetName())
		return nullptr;

	const int nDepth = oLiteReader.GetDepth();
	std::string sNodeName;

	CDocBody *pDocBody = new CDocBody();

	if (nullptr == pDocBody)
		return nullptr;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:DocInfo" == sNodeName)
		{
			if (!pDocBody->m_oDocInfo.Read(oLiteReader))
			{
				delete pDocBody;
				return nullptr;
			}
		}
		else if ("ofd:DocRoot" == sNodeName)
		{
			const std::wstring wsPath = NSSystemPath::ShortenPath(oLiteReader.GetText2());

			if (!wsPath.empty() && L'.' != wsPath.front())
				pDocBody->m_oDocument.Read(pFolder->getFullFilePath(wsPath));
		}
		else if ("ofd:Signatures" == sNodeName)
			pDocBody->ReadSignatures(oLiteReader.GetText2(), pFolder->getFullFilePath(L""));
	}

	return pDocBody;
}

bool CDocBody::DrawPage(IRenderer* pRenderer, int nPageIndex) const
{
	const bool bResult = m_oDocument.DrawPage(pRenderer, nPageIndex);

	for (const CSignature* pSignature : m_arSignatures)
		if (pSignature->Draw(pRenderer, nPageIndex, nullptr))
			break;

	return bResult;
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
