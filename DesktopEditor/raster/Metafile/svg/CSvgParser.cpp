#include "CSvgParser.h"

#include <algorithm>
#include <cctype>

#include "CSvgFile.h"

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
#include "SvgObjects/CFont.h"
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

	std::string FindEncoding(const std::string& wsContent)
	{
		size_t unEncodingBegin = wsContent.find("encoding");

		if (std::string::npos == unEncodingBegin)
			return std::string();

		unEncodingBegin += 8;

		while (unEncodingBegin < wsContent.length() && std::isspace(wsContent[unEncodingBegin]))
			++unEncodingBegin;

		if (unEncodingBegin >= wsContent.length() || '=' != wsContent[unEncodingBegin++])
			return std::string();

		while (unEncodingBegin < wsContent.length() && std::isspace(wsContent[unEncodingBegin]))
			++unEncodingBegin;

		if (unEncodingBegin >= wsContent.length() || ('\'' != wsContent[unEncodingBegin] && '"' != wsContent[unEncodingBegin]))
			return std::string();

		std::string::const_iterator itEncodingValueBegin = std::find_if(wsContent.cbegin() + unEncodingBegin + 1, wsContent.cend(), [](char chElement){ return !isspace(chElement);});

		if (wsContent.cend() == itEncodingValueBegin)
			return std::string();

		std::string::const_iterator itEncodingValueEnd = std::find_if(itEncodingValueBegin, wsContent.cend(), [](char chElement){ return isspace(chElement) || '\'' == chElement || '\"' == chElement;});

		if (wsContent.cend() == itEncodingValueEnd)
			return std::string();

		return std::string(itEncodingValueBegin, itEncodingValueEnd);
	}

	bool CSvgParser::LoadFromFile(const std::wstring &wsFile, CGraphicsContainer* pContainer, CSvgFile* pFile) const
	{
		if (wsFile.empty() || NULL == pFile)
			return false;

		std::string sXml;
		if (!NSFile::CFileBinary::ReadAllTextUtf8A(wsFile, sXml))
			return false;

		size_t unFoundBegin = sXml.find("<svg");

		NSUnicodeConverter::CUnicodeConverter oConverter;
		std::wstring wsContent;

		if (std::string::npos != unFoundBegin)
		{
			std::string sEncoding = FindEncoding(sXml.substr(0, unFoundBegin));
			std::transform(sEncoding.begin(), sEncoding.end(), sEncoding.begin(), tolower);
			sXml.erase(0, unFoundBegin);

			if (!sEncoding.empty() && "utf-8" != sEncoding)
				wsContent = oConverter.toUnicode(sXml, sEncoding.c_str());
			else
				wsContent = UTF8_TO_U(sXml);
		}
		else
			return false;

		XmlUtils::IXmlDOMDocument::DisableOutput();
		bool bResult = LoadFromString(wsContent, pContainer, pFile);
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
			ParseStyles(oElement.GetText(), pFile);
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
	
	void CSvgParser::ParseStyles(const std::wstring &wsStyles, CSvgFile *pFile) const
	{
		if (NULL == pFile)
			return;

		pFile->AddStyles(wsStyles);

		std::wregex oRegex(L"@font-face\\s*(\\{[^}]*\\})");
		std::wsmatch oMatch;
		std::wstring::const_iterator oSearchStart(wsStyles.cbegin());
		
		while (std::regex_search(oSearchStart, wsStyles.cend(), oMatch, oRegex))
		{
			if (oMatch[1].str().empty())
				continue;

			std::wstring wsValue{oMatch[1].str()};

			std::wstring::const_iterator itStart = std::find_if(wsValue.cbegin(), wsValue.cend(), [](const wchar_t& wChar) { return !std::iswspace(wChar) && L'{' != wChar; });
			std::wstring::const_reverse_iterator itEnd = std::find_if(wsValue.crbegin(), wsValue.crend(), [](const wchar_t& wChar) { return !std::iswspace(wChar) && L'}' != wChar; });

			if (wsValue.cend() != itStart && wsValue.crend() != itEnd)
				wsValue = std::wstring(itStart, itEnd.base());

			const std::vector<std::wstring> arWords{NSCSS::NS_STATIC_FUNCTIONS::GetWordsW(wsValue, true, L":;")};

			SvgURL oURL;
			TFontArguments m_oArguments;

			for (unsigned int unIndex = 0; unIndex < arWords.size(); ++unIndex)
			{
				if (arWords[unIndex].length() > 3 && L"src" == arWords[unIndex].substr(0, 3) && L':' == arWords[unIndex].back() && 
				    unIndex + 1 < arWords.size() && oURL.SetValue(arWords[++unIndex]))
				{
					continue;
				}
				else if (arWords[unIndex].length() > 11 && L"font-family" == arWords[unIndex].substr(0, 11) && L':' == arWords[unIndex].back() &&
				         unIndex + 1 < arWords.size())
				{
					const std::vector<std::wstring> arFontFamily{NSCSS::NS_STATIC_FUNCTIONS::GetWordsW(arWords[++unIndex], false, L"\"\',;")};

					if (arFontFamily.empty())
						continue;

					m_oArguments.m_wsFontFamily = arFontFamily.back();
				}
			}

			if (!oURL.Empty() && !m_oArguments.m_wsFontFamily.empty())
				pFile->AddFontFace(m_oArguments, oURL.GetValue());

			oSearchStart = oMatch.suffix().first;
		}
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
		else if (L"font" == wsElementName)
		{
			pObject = new CFont(oElement);
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
