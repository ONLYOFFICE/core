#include "CSvgParser.h"

#include "SvgUtils.h"

#include <iostream>

#include "SvgObjects/CEllipse.h"
#include "SvgObjects/CHeader.h"
#include "SvgObjects/CCircle.h"
#include "SvgObjects/CRect.h"
#include "SvgObjects/CLine.h"
#include "SvgObjects/CPath.h"

namespace SVG
{
	CSvgParser::CSvgParser()
	    : m_pStorage(NULL)
	{

	}

	CSvgParser::~CSvgParser()
	{

	}

	bool CSvgParser::LoadFromFile(const std::wstring &wsFile, CSvgStorage* pStorage)
	{
		if (NULL == pStorage)
			return false;

		std::wstring wsXml;
		NSFile::CFileBinary::ReadAllTextUtf8(wsFile, wsXml);

		XmlUtils::IXmlDOMDocument::DisableOutput();
		bool bResult = LoadFromString(wsXml, pStorage);
		XmlUtils::IXmlDOMDocument::EnableOutput();

		return bResult;
	}

	bool CSvgParser::LoadFromString(const std::wstring &wsContent, CSvgStorage *pStorage)
	{
		if (NULL == pStorage)
			return false;

		m_pStorage = pStorage;

		XmlUtils::CXmlNode oXml;
		if (!oXml.FromXmlString(wsContent))
			return false;

		std::wstring sNodeName = oXml.GetName();
		if (L"svg" != sNodeName &&
		    L"g"   != sNodeName   &&
		    L"xml" != sNodeName)
			return false;

		return ReadElement(oXml);
	}

	void CSvgParser::Clear()
	{

	}

	bool CSvgParser::ReadElement(XmlUtils::CXmlNode &oElement, CObjectBase *pParent)
	{
		std::wstring wsElementName = oElement.GetName();

		XmlUtils::CXmlNodes arChilds;

		oElement.GetChilds(arChilds);

		CObjectBase *pObject = NULL;

		if (L"svg" == wsElementName)
			pObject = new CHeader(pParent);
		else if (L"line" == wsElementName)
			pObject = new CLine(pParent);
		else if (L"rect" == wsElementName)
			pObject = new CRect(pParent);
		else if (L"circle" == wsElementName)
			pObject = new CCircle(pParent);
		else if (L"ellipse" == wsElementName)
			pObject = new CEllipse(pParent);
		else if (L"path" == wsElementName)
			pObject = new CPath(pParent);

		if (NULL != pObject)
		{
			if (!pObject->ReadFromXmlNode(oElement))
				return false;

			m_pStorage->AddObject(pObject);
		}

		XmlUtils::CXmlNode oChild;

		for (unsigned int unChildrenIndex = 0; unChildrenIndex < arChilds.GetCount(); ++unChildrenIndex)
		{
			if (!arChilds.GetAt(unChildrenIndex, oChild))
				break;

			ReadElement(oChild, pObject);

			oChild.Clear();
		}

		return true;
	}
}
