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

	bool CSvgParser::LoadFromString(const std::wstring &wsContent, CGraphicsContainer* pContainer, CSvgFile* pFile) const
	{
		if (wsContent.empty() || NULL == pFile)
			return false;

		CSvgReader oReader;
		if (!oReader.ReadFromString(wsContent))
			return false;

		ScanStyles(oReader, pFile);

		if (NULL != pContainer)
			pContainer->SetData(oReader);

		const CSvgCalculator *pSvgCalculator = pFile->GetSvgCalculator();

		if (NULL != pSvgCalculator)
			pSvgCalculator->SetData(pContainer);

		return ReadChildrens(oElement, pContainer, pFile, pContainer);
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

	bool CSvgParser::ScanStyles(CSvgReader& oReader, CSvgFile *pFile) const
	{
		if (oReader.IsEmptyNode() || NULL == pFile || !oReader.MoveToStart())
			return false;

		const std::string sElementName = oReader.GetName();

		if ("style" == sElementName)
		{
			ParseStyles(oReader.GetText(), pFile);
			return true;
		}

		bool bScanResult = false;

		if ("svg" == sElementName || "g" == sElementName || "defs" == sElementName)
		{
			WHILE_READ_NEXT_NODE(oReader)
				if (ScanStyles(oReader, pFile))
					bScanResult = true;
			END_WHILE
		}

		oReader.MoveToStart();

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
	bool CSvgParser::ReadObject(CSvgReader& oReader, CContainer<ObjectType> *pContainer, CSvgFile *pFile, CRenderedObject *pParent) const
	{
		if (NULL == pFile)
			return false;

		const std::string sElementName = oReader.GetName();

		CObject *pObject = NULL;

		if ("svg" == sElementName || "g" == sElementName || "a" == sElementName)
		{
			pObject = new CGraphicsContainer(oReader, pParent);
			if (!ReadChildrens(oReader, (CGraphicsContainer*)pObject, pFile, (CGraphicsContainer*)pObject))
			{
				RELEASEOBJECT(pObject);
				return false;
			}
		}
		else if ("line" == sElementName)
			pObject = new CLine(oReader, pParent);
		else if ("rect" == sElementName)
			pObject = new CRect(oReader, pParent);
		else if ("circle" == sElementName)
			pObject = new CCircle(oReader, pParent);
		else if ("ellipse" == sElementName)
			pObject = new CEllipse(oReader, pParent);
		else if ("path" == sElementName)
			pObject = new CPath(oReader, pParent);
		else if ("polyline" == sElementName)
			pObject = new CPolyline(oReader, pParent);
		else if ("polygon" == sElementName)
			pObject = new CPolygon(oReader, pParent);
		else if ("image" == sElementName)
			pObject = new CImage(oReader, pParent);
		else if ("use" == sElementName)
			pObject = new CUse(oReader, pParent);
		else if ("text" == sElementName)
		{
			pObject = CText::Create(oReader, pParent, m_pFontManager);
			ReadChildrens(oReader, (CText*)pObject, pFile, (CText*)pObject);
		}
		else if ("tspan" == sElementName)
		{
			pObject = CTSpan::Create(oReader, pParent, m_pFontManager);
			ReadChildrens(oReader, (CTSpan*)pObject, pFile, (CTSpan*)pObject);
		}
		else if ("textPath" == sElementName)
		{
			pObject = CTextPath::Create(oReader, pParent, m_pFontManager, pFile);
			ReadChildrens(oReader, (CTextPath*)pObject, pFile);
		}
		else if ("switch" == sElementName)
		{
			pObject = new CSwitch(oReader, pParent);
			ReadChildrens(oReader, (CSwitch*)pObject, pFile);
		}
		//defs
		else if ("defs" == sElementName)
			return ReadChildrens<CRenderedObject>(oReader, NULL, pFile);
		else if("linearGradient" == sElementName)
		{
			pObject = new CLinearGradient(oReader);
			ReadChildrens(oReader, (CLinearGradient*)pObject, pFile);
		}
		else if ("radialGradient" == sElementName)
		{
			pObject = new CRadialGradient(oReader);
			ReadChildrens(oReader, (CRadialGradient*)pObject, pFile);
		}
		else if ("stop" == sElementName)
		{
			CStopElement *pStopElement = new CStopElement(oReader);
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
		else if ("pattern" == sElementName)
		{
			pObject = new CPattern(oReader, m_pFontManager);
			ReadChildrens(oReader, (CGraphicsContainer*)(&((CPattern*)pObject)->GetContainer()), pFile);
		}
		else if ("clipPath" == sElementName)
		{
			pObject = new CClipPath(oReader);
			ReadChildrens(oReader, (CGraphicsContainer*)(&((CClipPath*)pObject)->GetContainer()), pFile);
		}
		else if ("marker" == sElementName)
		{
			pObject = new CMarker(oReader);
			ReadChildrens(oReader, (CMarker*)pObject, pFile);
		}
		else if ("mask" == sElementName)
		{
			pObject = new CMask(oReader);
			ReadChildrens(oReader, (CGraphicsContainer*)(&((CMask*)pObject)->GetContainer()), pFile);
		}
		else if ("symbo" == sElementName)
		{
			pObject = new CSymbol(oReader);
			if (ReadChildrens(oReader, (CSymbol*)pObject, pFile) && MarkObject(pObject, pFile))
				return true;
			else
				RELEASEOBJECT(pObject);
		}
		else if ("font" == sElementName)
		{
			pObject = new CFont(oReader);
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
	bool CSvgParser::ReadChildrens(CSvgReader& oReader, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject *pParent) const
	{
		bool bResult = false;

		WHILE_READ_NEXT_NODE(oReader)
			if (ReadObject(oReader, pContainer, pFile, pParent))
				bResult = true;
		END_WHILE

		return bResult;
	}
}
