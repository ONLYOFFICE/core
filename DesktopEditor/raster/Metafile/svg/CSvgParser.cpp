#include "CSvgParser.h"

#include "SvgUtils.h"

#include <iostream>

#include "CSvgFile.h"

#include "SvgObjects/CContainer.h"
#include "SvgObjects/CPolyline.h"
#include "SvgObjects/CEllipse.h"
#include "SvgObjects/CCircle.h"
#include "SvgObjects/CStyle.h"
#include "SvgObjects/CRect.h"
#include "SvgObjects/CLine.h"
#include "SvgObjects/CPath.h"
#include "SvgObjects/CText.h"

namespace SVG
{
	CSvgParser::CSvgParser()
	    : m_pFontManager(NULL)
	{
	}

	CSvgParser::~CSvgParser()
	{
	}

	void CSvgParser::SetFontManager(NSFonts::IFontManager *pFontManager)
	{
		m_pFontManager = pFontManager;
	}

	bool CSvgParser::LoadFromFile(const std::wstring &wsFile, CContainer* pContainer, CSvgFile* pFile) const
	{
		if (NULL == pContainer)
			return false;

		std::wstring wsXml;
		NSFile::CFileBinary::ReadAllTextUtf8(wsFile, wsXml);

		XmlUtils::IXmlDOMDocument::DisableOutput();
		bool bResult = LoadFromString(wsXml, pContainer, pFile);
		XmlUtils::IXmlDOMDocument::EnableOutput();

		return bResult;
	}

	bool CSvgParser::LoadFromString(const std::wstring &wsContent, CContainer* pContainer, CSvgFile* pFile) const
	{
		if (NULL == pContainer)
			return false;

		XmlUtils::CXmlNode oXml;
		if (!oXml.FromXmlString(wsContent))
			return false;

		return LoadFromXmlNode(oXml, pContainer, pFile);
	}

	bool CSvgParser::LoadFromXmlNode(XmlUtils::CXmlNode &oElement, CContainer* pContainer, CSvgFile* pFile) const
	{
		if (NULL != pFile && NULL != pContainer && NULL != pFile && pContainer->ReadFromXmlNode(oElement))
		{
			const CSvgCalculator *pSvgCalculator = pFile->GetSvgCalculator();

			if (NULL != pSvgCalculator)
				pSvgCalculator->SetData(*pContainer);

			return ReadChildrens(oElement, pContainer, pFile);
		}
		return false;
	}

	bool CSvgParser::ReadElement(XmlUtils::CXmlNode &oElement, CContainer *pContainer, CSvgFile *pFile) const
	{
		if (NULL == pContainer || NULL == pFile)
			return false;

		std::wstring wsElementName = oElement.GetName();

		CObjectBase *pObject = NULL;

		if (L"svg" == wsElementName || L"g" == wsElementName)
		{
//			CContainer *pNewContainer = new CContainer(pContainer);

//			if (NULL == pNewContainer)
//				return false;

//			if (pNewContainer->ReadFromXmlNode(oElement, *this, pFile))
//			{
//				pContainer->AddObject(pNewContainer);
//				return true;
//			}
//			else
//				return false;
		}
		else if (L"style" == wsElementName)
		{
			if (NULL != pFile)
				pFile->AddStyles(oElement.GetText());
		}
		else if (L"line" == wsElementName)
			pObject = new CLine(pContainer);
		else if (L"rect" == wsElementName)
			pObject = new CRect(pContainer);
		else if (L"circle" == wsElementName)
			pObject = new CCircle(pContainer);
		else if (L"ellipse" == wsElementName)
			pObject = new CEllipse(pContainer);
		else if (L"path" == wsElementName)
			pObject = new CPath(pContainer);
		else if (L"text" == wsElementName)
			pObject = new CText(pContainer, m_pFontManager);
		else if (L"polyline" == wsElementName)
			pObject = new CPolyline(pContainer);
		else if (L"polygon" == wsElementName)
			pObject = new CPolygon(pContainer);

		if (NULL != pObject)
		{
			if (pObject->ReadFromXmlNode(oElement))
			{
				pContainer->AddObject(pObject);
				const CSvgCalculator *pSvgCalculator = pFile->GetSvgCalculator();

				if (NULL != pSvgCalculator)
					pSvgCalculator->SetData(*pObject);
			}
			else
				RELEASEOBJECT(pObject);
		}
		else
			return false;

		return true;
	}

	bool CSvgParser::ReadChildrens(XmlUtils::CXmlNode &oElement, CContainer* pContainer, CSvgFile* pFile) const
	{
		XmlUtils::CXmlNodes arChilds;

		oElement.GetChilds(arChilds);

		XmlUtils::CXmlNode oChild;

		for (unsigned int unChildrenIndex = 0; unChildrenIndex < arChilds.GetCount(); ++unChildrenIndex)
		{
			if (!arChilds.GetAt(unChildrenIndex, oChild))
				break;

			ReadElement(oChild, pContainer, pFile);

			oChild.Clear();
		}

		return true;
	}
}
