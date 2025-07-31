#include "Annotation.h"
#include "Utils/Utils.h"

namespace OFD
{
CParameter::CParameter(CXmlReader& oLiteReader)
{
	if (oLiteReader.MoveToFirstAttribute())
	{
		do
		{
			if ("Name" != oLiteReader.GetNameA())
				continue;

			m_wsName = oLiteReader.GetText();
		} while (oLiteReader.MoveToNextAttribute());

		oLiteReader.MoveToElement();
	}

	m_wsValue = oLiteReader.GetText2();
}

CAnnot::CAnnot(CXmlReader& oLiteReader)
	: m_bVisible(true), m_bPrint(true), m_bNoZoom(false), m_bNoRotate(false), m_bReadOnly(true)
{
	if (oLiteReader.MoveToFirstAttribute())
	{
		std::string wsAttributeName;
		do
		{
			wsAttributeName = oLiteReader.GetNameA();

			if ("Type" == wsAttributeName)
			{
				const std::string sValue{oLiteReader.GetTextA()};

				if ("Link" == sValue)
					m_eType = EAnnotType::Link;
				else if ("Path" == sValue)
					m_eType = EAnnotType::Path;
				else if ("Highlight" == sValue)
					m_eType = EAnnotType::Highlight;
				else if ("Stamp" == sValue)
					m_eType = EAnnotType::Stamp;
				else if ("Watermark" == sValue)
					m_eType = EAnnotType::Watermark;
			}
			else if ("Visible" == wsAttributeName)
			{
				if ("false" == oLiteReader.GetTextA())
					m_bVisible = false;
			}
			else if ("Print" == wsAttributeName)
			{
				if ("false" == oLiteReader.GetTextA())
					m_bPrint = false;
			}
			else if ("NoZoom" == wsAttributeName)
			{
				if ("true" == oLiteReader.GetTextA())
					m_bNoZoom = true;
			}
			else if ("NoRotate" == wsAttributeName)
			{
				if ("true" == oLiteReader.GetTextA())
					m_bNoRotate = true;
			}
			else if ("ReadOnly" == wsAttributeName)
			{
				if ("true" == oLiteReader.GetTextA())
					m_bReadOnly = true;
			}
		}while(oLiteReader.MoveToNextAttribute());

		oLiteReader.MoveToElement();
	}

	const int nDepth = oLiteReader.GetDepth();
	std::string sNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:Parameters" == sNodeName)
		{
			const int nChildDepth = oLiteReader.GetDepth();

			while (oLiteReader.ReadNextSiblingNode(nChildDepth))
			{
				if ("ofd:Parameter" != oLiteReader.GetNameA())
					continue;

				m_arParameters.push_back(new CParameter(oLiteReader));
			}
		}
		else if ("ofd:Appearance" == sNodeName)
			m_arAppearances.push_back(new CAppearance(oLiteReader));
	}
}

CAnnot::~CAnnot()
{
	ClearContainer(m_arParameters);
	ClearContainer(m_arAppearances);
}

void CAnnot::Draw(IRenderer* pRenderer, const CCommonData& oCommonData) const
{
	if (!m_bPrint)
		return;

	for (const CAppearance* pAppearance : m_arAppearances)
		pAppearance->Draw(pRenderer, oCommonData);
}

CPageAnnot::CPageAnnot()
{}

CPageAnnot* CPageAnnot::Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, wsRootPath))
		return nullptr;

	const std::wstring wsNormalizedPath = CombinePaths(wsRootPath, wsFilePath);

	CXmlReader oLiteReader;
	if (!oLiteReader.FromFile(wsNormalizedPath) || !oLiteReader.ReadNextNode() || "ofd:PageAnnot" != oLiteReader.GetNameA())
		return nullptr;

	CPageAnnot *pPageAnnot = new CPageAnnot();

	const int nDepth = oLiteReader.GetDepth();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:Annot" != oLiteReader.GetNameA())
			continue;

		pPageAnnot->m_arAnnots.push_back(new CAnnot(oLiteReader));
	}

	return pPageAnnot;
}

void CPageAnnot::Draw(IRenderer* pRenderer, const CCommonData& oCommonData) const
{
	for (const CAnnot* pAnnot : m_arAnnots)
		pAnnot->Draw(pRenderer, oCommonData);
}

CAnnotation::CAnnotation()
	: m_pPageAnnot(nullptr)
{}

CAnnotation::~CAnnotation()
{
	if (nullptr != m_pPageAnnot)
		delete m_pPageAnnot;
}

bool CAnnotation::Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, wsRootPath))
		return false;

	std::wstring wsNormalizedPath = CombinePaths(wsRootPath, wsFilePath);

	if (L"xml" != NSFile::GetFileExtention(wsNormalizedPath))
		wsNormalizedPath = CombinePaths(wsNormalizedPath, L"Annotations.xml");

	CXmlReader oLiteReader;
	if (!oLiteReader.FromFile(wsNormalizedPath) || !oLiteReader.ReadNextNode() || L"ofd:Annotations" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		if (L"ofd:Page" != oLiteReader.GetName())
			continue;

		const int nChildDepth = oLiteReader.GetDepth();
		while (oLiteReader.ReadNextSiblingNode(nChildDepth))
		{
			if (L"ofd:FileLoc" != oLiteReader.GetName())
				continue;

			if (nullptr == m_pPageAnnot)
				m_pPageAnnot = CPageAnnot::Read(oLiteReader.GetText2(), NSSystemPath::GetDirectoryName(wsNormalizedPath));
		}
	}

	return nullptr != m_pPageAnnot;
}

void CAnnotation::Draw(IRenderer* pRenderer, const CCommonData& oCommonData) const
{
	if (nullptr != m_pPageAnnot)
		m_pPageAnnot->Draw(pRenderer, oCommonData);
}
}
