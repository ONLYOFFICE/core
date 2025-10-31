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

#include "../../../../Common/3dParty/html/css/src/StaticFunctions.h"

namespace SVG
{
	CSvgParser::CSvgParser(NSFonts::IFontManager* pFontManager)
		: m_pFontManager(pFontManager)
	{}

	CSvgParser::~CSvgParser()
	{}

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

	bool CSvgParser::LoadFromFile(const std::wstring &wsFile, CGraphicsContainer*& pContainer, CSvgFile* pFile) const
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

		return LoadFromString(wsContent, pContainer, pFile);
	}

	bool CSvgParser::LoadFromString(const std::wstring &wsContent, CGraphicsContainer*& pContainer, CSvgFile* pFile) const
	{
		if (wsContent.empty() || NULL == pFile)
			return false;

		CSvgReader oReader;
		if (!oReader.ReadFromString(wsContent))
			return false;

		ScanStyles(oReader, pFile);
		oReader.MoveToStart();

		RELEASEOBJECT(pContainer);

		pContainer = CObject::Create<CGraphicsContainer>(oReader, pFile);

		if (NULL == pContainer)
			return false;

		return ReadChildrens(oReader, pContainer, pFile, pContainer);
	}

	bool CSvgParser::ScanStyles(CSvgReader& oReader, CSvgFile *pFile) const
	{
		if (oReader.IsEmptyNode() || NULL == pFile)
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
			{
				if (ScanStyles(oReader, pFile))
					bScanResult = true;
			}
			END_WHILE
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
	bool CSvgParser::ReadObject(CSvgReader& oReader, CContainer<ObjectType> *pContainer, CSvgFile *pFile, CRenderedObject *pParent) const
	{
		if (NULL == pFile)
			return false;

		const std::string sElementName = oReader.GetName();

		CObject *pObject = NULL;

		if ("svg" == sElementName || "g" == sElementName || "a" == sElementName)
		{
			pObject = CObject::Create<CGraphicsContainer>(oReader, pFile, pParent);
			if (!ReadChildrens(oReader, (CGraphicsContainer*)pObject, pFile, (CGraphicsContainer*)pObject))
			{
				RELEASEINTERFACE(pObject);
				return false;
			}
		}
		else if ("line" == sElementName)
			pObject = CObject::Create<CLine>(oReader, pFile, pParent);
		else if ("rect" == sElementName)
			pObject = CObject::Create<CRect>(oReader, pFile, pParent);
		else if ("circle" == sElementName)
			pObject = CObject::Create<CCircle>(oReader, pFile, pParent);
		else if ("ellipse" == sElementName)
			pObject = CObject::Create<CEllipse>(oReader, pFile, pParent);
		else if ("path" == sElementName)
			pObject = CObject::Create<CPath>(oReader, pFile, pParent);
		else if ("polyline" == sElementName)
			pObject = CObject::Create<CPolyline>(oReader, pFile, pParent);
		else if ("polygon" == sElementName)
			pObject = CObject::Create<CPolygon>(oReader, pFile, pParent);
		else if ("image" == sElementName)
			pObject = CObject::Create<CImage>(oReader, pFile, pParent);
		else if ("use" == sElementName)
			pObject = CObject::Create<CUse>(oReader, pFile, pParent);
		else if ("text" == sElementName)
			pObject = CObject::Create<CText>(oReader, pFile, pParent, m_pFontManager);
		else if ("tspan" == sElementName)
			pObject = CObject::Create<CTSpan>(oReader, pFile, pParent, m_pFontManager);
		else if ("textPath" == sElementName)
			pObject = CObject::Create<CTextPath>(oReader, pFile, pParent, m_pFontManager);
		else if ("switch" == sElementName)
		{
			pObject = CObject::Create<CSwitch>(oReader, pFile, pParent);
			ReadChildrens(oReader, (CSwitch*)pObject, pFile);
		}
		//defs
		else if ("defs" == sElementName)
			return ReadChildrens<CRenderedObject>(oReader, NULL, pFile);
		else if("linearGradient" == sElementName)
			pObject = CObject::Create<CLinearGradient>(oReader, pFile);
		else if ("radialGradient" == sElementName)
			pObject = CObject::Create<CRadialGradient>(oReader, pFile);
		else if ("pattern" == sElementName)
		{
			pObject = CObject::Create<CPattern>(oReader, pFile);
			ReadChildrens(oReader, (CGraphicsContainer*)(&((CPattern*)pObject)->GetContainer()), pFile);
		}
		else if ("clipPath" == sElementName)
		{
			pObject = CObject::Create<CClipPath>(oReader, pFile);
			ReadChildrens(oReader, (CGraphicsContainer*)(&((CClipPath*)pObject)->GetContainer()), pFile);
		}
		else if ("marker" == sElementName)
		{
			pObject = CObject::Create<CMarker>(oReader, pFile);
			ReadChildrens(oReader, (CMarker*)pObject, pFile);
		}
		else if ("mask" == sElementName)
		{
			pObject = CObject::Create<CMask>(oReader, pFile);
			ReadChildrens(oReader, (CGraphicsContainer*)(&((CMask*)pObject)->GetContainer()), pFile);
		}
		else if ("symbol" == sElementName)
		{
			pObject = CObject::Create<CSymbol>(oReader, pFile);
			if (ReadChildrens(oReader, (CSymbol*)pObject, pFile))
				return true;
			else
				RELEASEINTERFACE(pObject);
		}
		else if ("font" == sElementName)
		{
			pObject = CObject::Create<CFont>(oReader, pFile);
		}

		if (NULL == pObject)
			return false;

		if ((RendererObject == pObject->GetType() && (AddObject((ObjectType*)pObject, pContainer) || pObject->Marked())) ||
		    AppliedObject == pObject->GetType())
			return true;

		RELEASEINTERFACE(pObject);
		return false;
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
