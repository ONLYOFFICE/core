#include "Signature.h"

#include "../Utils/Utils.h"

#include "../OFDFile_Private.h"

namespace OFD
{
CSignature::CSignature()
{}

CSignature* CSignature::Read(const std::wstring& wsFilePath, IFolder* pFolder)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, pFolder->getFullFilePath(L"")))
		return nullptr;

	CXmlReader oLiteReader;
	if (!oLiteReader.FromFile(CombinePaths(pFolder->getFullFilePath(L""), wsFilePath)) || !oLiteReader.ReadNextNode() || L"ofd:Signature" != oLiteReader.GetName() || oLiteReader.IsEmptyNode())
		return nullptr;

	CSignature *pSignature = new CSignature();

	pSignature->m_wsRootPath = pFolder->getFullFilePath(L"");

	const int nDepth = oLiteReader.GetDepth();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:SignedInfo" == oLiteReader.GetNameA())
			pSignature->m_oSignedInfo.Read(oLiteReader);
		else if ("ofd:SignedValue" == oLiteReader.GetNameA())
			pSignature->m_wsSignedValue = CombinePaths(pSignature->m_wsRootPath, oLiteReader.GetText2());
	}

	return pSignature;
}

bool CSignature::Draw(IRenderer* pRenderer, unsigned int unPageIndex, NSFonts::IApplicationFonts* pFonts) const
{
	if (nullptr == pRenderer || m_wsSignedValue.empty() ||
	    m_oSignedInfo.m_oStampAnnot.m_unPageRef - 1 != unPageIndex ||
	    m_oSignedInfo.m_oStampAnnot.m_oBoundary.Empty())
		return false;

	COFDFile_Private oFile(pFonts);
	oFile.SetTempDir(m_wsRootPath);

	oFile.LoadFromFile(m_wsSignedValue);

	oFile.DrawPage(pRenderer, 0,
	               m_oSignedInfo.m_oStampAnnot.m_oBoundary.m_dX,
	               m_oSignedInfo.m_oStampAnnot.m_oBoundary.m_dY,
	               m_oSignedInfo.m_oStampAnnot.m_oBoundary.m_dWidth,
	               m_oSignedInfo.m_oStampAnnot.m_oBoundary.m_dHeight);

	return false;
}

void TProvider::Read(CXmlReader& oLiteReader)
{
	if (0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;

	do
	{
		sAttributeName = oLiteReader.GetNameA();

		if ("ProviderName" == sAttributeName)
			m_wsProviderName = oLiteReader.GetText();
		else if ("Version" == sAttributeName)
			m_wsVersion = oLiteReader.GetText();
		else if ("Company" == sAttributeName)
			m_wsCompany = oLiteReader.GetText();
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();
}

TReference* TReference::Read(CXmlReader& oLiteReader)
{
	if ("ofd:Reference" != oLiteReader.GetNameA() || oLiteReader.IsEmptyElement() || 0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return nullptr;

	TReference* pElement = new TReference();

	do
	{
		if ("FileRef" == oLiteReader.GetNameA())
			pElement->m_wsFileRef = oLiteReader.GetText();
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();

	const int nDepth = oLiteReader.GetDepth();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:CheckValue" == oLiteReader.GetNameA())
		{
			pElement->m_wsCheckValue = oLiteReader.GetText2();
			break;
		}
	}

	return pElement;
}

TReferences::~TReferences()
{
	ClearContainer(m_arValues);
}

void TReferences::Read(CXmlReader& oLiteReader)
{
	if (oLiteReader.IsEmptyElement() || 0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	do
	{
		if ("CheckMethod" == oLiteReader.GetNameA())
			m_wsCheckMethod = oLiteReader.GetText();
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();

	const int nDepth = oLiteReader.GetDepth();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
		AddToContainer(TReference::Read(oLiteReader), m_arValues);
}

TStampAnnot::TStampAnnot()
	: m_unID(0), m_unPageRef(0)
{}

void TStampAnnot::Read(CXmlReader& oLiteReader)
{
	if (0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;

	do
	{
		sAttributeName = oLiteReader.GetNameA();

		if ("ID" == sAttributeName)
			m_unID = oLiteReader.GetUInteger(true);
		else if ("PageRef" == sAttributeName)
			m_unPageRef = oLiteReader.GetUInteger(true);
		else if ("Boundary" == sAttributeName)
			m_oBoundary.Read(oLiteReader.GetTextA());
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();
}

void TSignedInfo::Read(CXmlReader& oLiteReader)
{
	if (oLiteReader.IsEmptyNode())
		return;

	const int nDepth = oLiteReader.GetDepth();

	std::string sNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:Provider" == sNodeName)
			m_oProvider.Read(oLiteReader);
		else if ("ofd:SignatureMethod" == sNodeName)
			m_wsSignatureMethod = oLiteReader.GetText2();
		else if ("ofd:SignatureDateTime" == sNodeName)
			m_wsSignatureDateTime = oLiteReader.GetText2();
		else if ("ofd:References" == sNodeName)
			m_oReferences.Read(oLiteReader);
		else if ("ofd:StampAnnot" == sNodeName)
			m_oStampAnnot.Read(oLiteReader);
	}
}

}
