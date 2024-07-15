#include "CSvgParser.h"

#include "SvgUtils.h"

#include <iostream>

#include "CSvgFile.h"

#include "SvgObjects/CContainer.h"
#include "SvgObjects/CPolyline.h"
#include "SvgObjects/CGradient.h"
#include "SvgObjects/CClipPath.h"
#include "SvgObjects/CPattern.h"
#include "SvgObjects/CEllipse.h"
#include "SvgObjects/CSymbol.h"
#include "SvgObjects/CSwitch.h"
#include "SvgObjects/CMarker.h"
#include "SvgObjects/CCircle.h"
#include "SvgObjects/CStyle.h"
#include "SvgObjects/CImage.h"
#include "SvgObjects/CRect.h"
#include "SvgObjects/CLine.h"
#include "SvgObjects/CPath.h"
#include "SvgObjects/CText.h"
#include "SvgObjects/CMask.h"
#include "SvgObjects/CUse.h"

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

	bool CSvgParser::LoadFromFile(const std::wstring &wsFile, CGraphicsContainer* pContainer, CSvgFile* pFile) const
	{
		if (wsFile.empty() || NULL == pFile)
			return false;

		std::wstring wsXml;
		NSFile::CFileBinary::ReadAllTextUtf8(wsFile, wsXml);

		XmlUtils::IXmlDOMDocument::DisableOutput();
		bool bResult = LoadFromString(wsXml, pContainer, pFile);
		XmlUtils::IXmlDOMDocument::EnableOutput();

		return bResult;
	}

	bool CSvgParser::LoadFromString(const std::wstring &wsContentent, CGraphicsContainer* pContainer, CSvgFile* pFile) const
	{
		if (wsContentent.empty() || NULL == pFile)
			return false;

		XmlUtils::CXmlNode oXml;
		if (!oXml.FromXmlString(wsContentent))
			return false;

		ScanStyles(oXml, pFile);

		if (NULL != pContainer)
			pContainer->SetData(oXml);

		return LoadFromXmlNode(oXml, pContainer, pFile);
	}

	bool CSvgParser::LoadFromXmlNode(XmlUtils::CXmlNode &oElement, CGraphicsContainer* pContainer, CSvgFile* pFile) const
	{
		if (NULL == pFile || !oElement.IsValid())
			return false;

		const CSvgCalculator *pSvgCalculator = pFile->GetSvgCalculator();

		if (NULL != pSvgCalculator)
			pSvgCalculator->SetData(pContainer);

		return ReadChildrens(oElement, pContainer, pFile, pContainer);
	}

	bool CSvgParser::ScanStyles(XmlUtils::CXmlNode &oElement, CSvgFile *pFile) const
	{
		if (!oElement.IsValid() || NULL == pFile)
			return false;

		std::wstring wsElementName = oElement.GetName();

		if (L"style" == wsElementName)
		{
			pFile->AddStyles(oElement.GetText());
			return true;
		}

		bool bScanResult = false;

		if (L"svg" == wsElementName || L"g" == wsElementName || L"defs" == wsElementName)
		{
			std::vector<XmlUtils::CXmlNode> arChilds;

			oElement.GetChilds(arChilds);

			for (XmlUtils::CXmlNode& oChild : arChilds)
			{
				if (ScanStyles(oChild, pFile))
					bScanResult = true;
			}
		}

		return bScanResult;
	}

	template <class ObjectType>
	bool CSvgParser::ReadObject(XmlUtils::CXmlNode &oElement, CContainer<ObjectType> *pContainer, CSvgFile *pFile, CRenderedObject *pParent) const
	{
		if (!oElement.IsValid() || NULL == pFile)
			return false;

		std::wstring wsElementName = oElement.GetName();

		CObject *pObject = NULL;

		if (L"svg" == wsElementName || L"g" == wsElementName || L"a" == wsElementName)
		{
			pObject = new CGraphicsContainer(oElement, pParent);
			if (!ReadChildrens(oElement, (CGraphicsContainer*)pObject, pFile, (CGraphicsContainer*)pObject))
			{
				RELEASEOBJECT(pObject);
				return false;
			}
		}
		else if (L"line" == wsElementName)
			pObject = new CLine(oElement, pParent);
		else if (L"rect" == wsElementName)
			pObject = new CRect(oElement, pParent);
		else if (L"circle" == wsElementName)
			pObject = new CCircle(oElement, pParent);
		else if (L"ellipse" == wsElementName)
			pObject = new CEllipse(oElement, pParent);
		else if (L"path" == wsElementName)
			pObject = new CPath(oElement, pParent);
		else if (L"polyline" == wsElementName)
			pObject = new CPolyline(oElement, pParent);
		else if (L"polygon" == wsElementName)
			pObject = new CPolygon(oElement, pParent);
		else if (L"image" == wsElementName)
			pObject = new CImage(oElement, pParent);
		else if (L"use" == wsElementName)
			pObject = new CUse(oElement, pParent);
		else if (L"text" == wsElementName)
		{
			pObject = CText::Create(oElement, pParent, m_pFontManager);
			ReadChildrens(oElement, (CText*)pObject, pFile, (CText*)pObject);
		}
		else if (L"tspan" == wsElementName)
		{
			pObject = CTSpan::Create(oElement, pParent, m_pFontManager);
			ReadChildrens(oElement, (CTSpan*)pObject, pFile, (CTSpan*)pObject);
		}
		else if (L"textPath" == wsElementName)
		{
			pObject = CTextPath::Create(oElement, pParent, m_pFontManager, pFile);
			ReadChildrens(oElement, (CTextPath*)pObject, pFile);
		}
		else if (L"switch" == wsElementName)
		{
			pObject = new CSwitch(oElement, pParent);
			ReadChildrens(oElement, (CSwitch*)pObject, pFile);
		}
		//defs
		else if (L"defs" == wsElementName)
			return ReadChildrens<CRenderedObject>(oElement, NULL, pFile);
		else if(L"linearGradient" == wsElementName)
		{
			pObject = new CLinearGradient(oElement);
			ReadChildrens(oElement, (CLinearGradient*)pObject, pFile);
		}
		else if (L"radialGradient" == wsElementName)
		{
			pObject = new CRadialGradient(oElement);
			ReadChildrens(oElement, (CRadialGradient*)pObject, pFile);
		}
		else if (L"stop" == wsElementName)
		{
			CStopElement *pStopElement = new CStopElement(oElement);
			if (AddObject((ObjectType*)pStopElement, pContainer))
			{
				UpdateStyles(pStopElement, pFile);
				return true;
			}
			else
			{
				RELEASEOBJECT(pStopElement);
				return false;
			}
		}
		else if (L"pattern" == wsElementName)
		{
			pObject = new CPattern(oElement, m_pFontManager);
			ReadChildrens(oElement, (CGraphicsContainer*)(&((CPattern*)pObject)->GetContainer()), pFile);
		}
		else if (L"clipPath" == wsElementName)
		{
			pObject = new CClipPath(oElement);
			ReadChildrens(oElement, (CGraphicsContainer*)(&((CClipPath*)pObject)->GetContainer()), pFile);
		}
		else if (L"marker" == wsElementName)
		{
			pObject = new CMarker(oElement);
			ReadChildrens(oElement, (CMarker*)pObject, pFile);
		}
		else if (L"mask" == wsElementName)
		{
			pObject = new CMask(oElement);
			ReadChildrens(oElement, (CGraphicsContainer*)(&((CMask*)pObject)->GetContainer()), pFile);
		}
		else if (L"symbol" == wsElementName)
		{
			pObject = new CSymbol(oElement);
			if (ReadChildrens(oElement, (CSymbol*)pObject, pFile) && MarkObject(pObject, pFile))
				return true;
			else
				RELEASEOBJECT(pObject);
		}

		if (NULL != pObject)
		{
			if ((MarkObject(pObject, pFile) && (AppliedObject == pObject->GetType() || NULL == pContainer)) ||
				(RendererObject == pObject->GetType() && AddObject((ObjectType*)pObject, pContainer)))
			{
				UpdateStyles(pObject, pFile);
				return true;
			}
			delete pObject;
		}

		return false;
	}

	void CSvgParser::UpdateStyles(CObject *pObject, CSvgFile *pFile) const
	{
		if (NULL == pObject || NULL == pFile)
			return;

		const CSvgCalculator *pSvgCalculator = pFile->GetSvgCalculator();

		if (NULL != pSvgCalculator)
			pSvgCalculator->SetData(pObject);
	}

	bool CSvgParser::MarkObject(CObject *pObject, CSvgFile *pFile) const
	{
		if (NULL == pObject || NULL == pFile)
			return false;

		return pFile->MarkObject(pObject);
	}

	template <class ObjectType>
	bool CSvgParser::AddObject(ObjectType *pObject, CContainer<ObjectType> *pContainer) const
	{
		return (NULL != pContainer && pContainer->AddObject(pObject));
	}

	template <class ObjectType>
	bool CSvgParser::ReadChildrens(XmlUtils::CXmlNode &oElement, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject *pParent) const
	{
		std::vector<XmlUtils::CXmlNode> arChilds;

		oElement.GetChilds(arChilds);

		if (arChilds.empty())
			return false;

		for (XmlUtils::CXmlNode& oChild : arChilds)
			ReadObject(oChild, pContainer, pFile, pParent);

		return true;
	}
}
