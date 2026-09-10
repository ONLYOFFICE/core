/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "CSvgParser.h"

#include <algorithm>
#include <cctype>
#include <cwctype>

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

#include "html2svg.h"

#include "../../../../UnicodeConverter/UnicodeConverter.h"

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

	size_t FindSvgBegin(const std::string& sContent)
	{
		size_t unPosition = 0;

		while (unPosition < sContent.length())
		{
			while (unPosition < sContent.length() && std::isspace(static_cast<unsigned char>(sContent[unPosition])))
				++unPosition;

			if (unPosition >= sContent.length() || '<' != sContent[unPosition])
				return std::string::npos;

			if (0 == sContent.compare(unPosition, 4, "<!--"))
			{
				const size_t unEnd = sContent.find("-->", unPosition + 4);

				if (std::string::npos == unEnd)
					return std::string::npos;

				unPosition = unEnd + 3;
			}
			else if (0 == sContent.compare(unPosition, 2, "<?"))
			{
				const size_t unEnd = sContent.find("?>", unPosition + 2);

				if (std::string::npos == unEnd)
					return std::string::npos;

				unPosition = unEnd + 2;
			}
			else if (0 == sContent.compare(unPosition, 2, "<!"))
			{
				size_t unEnd = unPosition + 2;
				unsigned int unSubsetDepth = 0;

				while (unEnd < sContent.length())
				{
					const char chElement = sContent[unEnd];

					if ('\'' == chElement || '\"' == chElement)
					{
						unEnd = sContent.find(chElement, unEnd + 1);

						if (std::string::npos == unEnd)
							return std::string::npos;

						++unEnd;
					}
					else if (0 == sContent.compare(unEnd, 4, "<!--"))
					{
						const size_t unCommentEnd = sContent.find("-->", unEnd + 4);

						if (std::string::npos == unCommentEnd)
							return std::string::npos;

						unEnd = unCommentEnd + 3;
					}
					else if (0 == sContent.compare(unEnd, 2, "<?"))
					{
						const size_t unInstructionEnd = sContent.find("?>", unEnd + 2);

						if (std::string::npos == unInstructionEnd)
							return std::string::npos;

						unEnd = unInstructionEnd + 2;
					}
					else if ('[' == chElement)
					{
						++unSubsetDepth;
						++unEnd;
					}
					else if (']' == chElement && 0 != unSubsetDepth)
					{
						--unSubsetDepth;
						++unEnd;
					}
					else if ('>' == chElement && 0 == unSubsetDepth)
						break;
					else
						++unEnd;
				}

				if (unEnd >= sContent.length())
					return std::string::npos;

				unPosition = unEnd + 1;
			}
			else
				return (0 == sContent.compare(unPosition, 4, "<svg")) ? unPosition : std::string::npos;
		}

		return std::string::npos;
	}

	bool CSvgParser::LoadFromFile(const std::wstring &wsFile, CGraphicsContainer*& pContainer, CSvgFile* pFile) const
	{
		if (wsFile.empty() || NULL == pFile)
			return false;

		std::string sXml;
		if (!NSFile::CFileBinary::ReadAllTextUtf8A(wsFile, sXml))
			return false;

		size_t unFoundBegin = FindSvgBegin(sXml);

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

		RELEASEINTERFACE(pContainer);

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
	
	typedef std::pair<std::wstring, std::wstring> StylePropertie;
	typedef std::vector<StylePropertie> StyleProperties;

	StyleProperties ParseFontFace(const std::wstring& wsValue)
	{
		StyleProperties arResult;
		size_t unIndex{0};
		const size_t unMaxSize{wsValue.size()};

		while (unIndex < unMaxSize)
		{
			while (unIndex < unMaxSize && std::iswspace(static_cast<unsigned char>(wsValue[unIndex])))
				++unIndex;

			if (unIndex >= unMaxSize)
				break;

			size_t unColonPos{std::wstring::npos};
			int nQuoteType{0}; // 0 - no quotes, 1 - double quotes, 2 - single quotes
			size_t unKeyStart{unIndex};

			while (unIndex < unMaxSize)
			{
				char c = wsValue[unIndex];
				if (nQuoteType == 0)
				{
					if (c == '"')
						nQuoteType = 1;
					else if (c == '\'')
						nQuoteType = 2;
					else if (c == ':')
					{
						unColonPos = unIndex;
						break;
					}
				}
				else
				{
					if ((nQuoteType == 1 && c == '"') || (nQuoteType == 2 && c == '\''))
						nQuoteType = 0;
				}
				++unIndex;
			}

			if (std::string::npos == unColonPos)
				break;

			std::wstring wsKey{wsValue.substr(unKeyStart, unColonPos - unKeyStart)};

			wsKey.erase(0, wsKey.find_first_not_of(L" \t\n\r\f\v"));
			wsKey.erase(wsKey.find_last_not_of(L" \t\n\r\f\v") + 1);

			unIndex = unColonPos + 1;
			nQuoteType = 0;
			size_t unValStart{unIndex};

			while (unIndex < unMaxSize)
			{
				char c = wsValue[unIndex];
				if (nQuoteType == 0)
				{
					if (c == '"')
						nQuoteType = 1;
					else if (c == '\'')
						nQuoteType = 2;
					else if (c == ';')
						break;
				}
				else
				{
					if ((nQuoteType == 1 && c == '"') || (nQuoteType == 2 && c == '\''))
						nQuoteType = 0;
				}
				++unIndex;
			}

			std::wstring wsKeyValue{wsValue.substr(unValStart, unIndex - unValStart)};

			wsKeyValue.erase(0, wsKeyValue.find_first_not_of(L" \t\n\r\f\v"));
			wsKeyValue.erase(wsKeyValue.find_last_not_of (L" \t\n\r\f\v") + 1);

			arResult.emplace_back(std::move(wsKey), std::move(wsKeyValue));

			if (unIndex < unMaxSize && wsValue[unIndex] == ';')
				++unIndex;
		}

		return arResult;
	}

	void TrimQuotes(std::wstring& wsValue)
	{
		if (wsValue.size() < 2)
			return;

		const wchar_t chFirst{wsValue.front()};

		if ((chFirst == L'\'' || chFirst == L'"') && wsValue.back() == chFirst)
		{
			wsValue.erase(wsValue.begin());
			wsValue.pop_back();
		}
	}

	void CSvgParser::ParseStyles(const std::wstring &wsStyles, CSvgFile *pFile) const
	{
		if (NULL == pFile)
			return;

		pFile->AddStyles(wsStyles);

		const std::wstring wsTag{L"@font-face"};
		size_t unPos = 0;

		while ((unPos = wsStyles.find(wsTag, unPos)) != std::wstring::npos)
		{
			size_t unCur{unPos + wsTag.length()};

			while (unCur < wsStyles.length() && std::iswspace(wsStyles[unCur]))
				++unCur;

			if (unCur >= wsStyles.length() || L'{' != wsStyles[unCur])
			{
				unPos += wsTag.length();
				continue;
			}

			const size_t unClose{wsStyles.find(L'}', unCur + 1)};

			if (std::wstring::npos == unClose)
				break;

			size_t unInnerStart{unCur + 1};
			size_t unInnerEnd{unClose};

			while (unInnerStart < unInnerEnd && std::iswspace(wsStyles[unInnerStart]))
				++unInnerStart;

			while (unInnerEnd > unInnerStart && std::iswspace(wsStyles[unInnerEnd - 1]))
				--unInnerEnd;

			if (unInnerStart < unInnerEnd)
			{
				const std::wstring wsValue{wsStyles.substr(unInnerStart, unInnerEnd - unInnerStart)};

				SvgURL oURL;
				TFontArguments oArguments;

				for (const StylePropertie& oPropertie : ParseFontFace(wsValue))
				{
					if (L"font-family" == oPropertie.first)
					{
						oArguments.m_wsFontFamily = oPropertie.second;
						TrimQuotes(oArguments.m_wsFontFamily);
					}
					else if (L"src" == oPropertie.first)
						oURL.SetValue(oPropertie.second);
				}

				if (!oURL.Empty() && !oArguments.m_wsFontFamily.empty())
					pFile->AddFontFace(oArguments, oURL.GetValue());
			}

			unPos = unClose + 1;
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
			pObject = CObject::Create<CFont>(oReader, pFile, pFile);
		}
		else if ("foreignObject" == sElementName)
		{
			double dWidth{300};

			START_READ_ATTRIBUTES(oReader)
				if ("width" == ATTRIBUTE_NAME)
					dWidth = oReader.GetDouble();
			END_READ_ATTRIBUTES(oReader)

			CSvgReader oNewReader;

			const std::wstring wsSVG{ConvertForeignObjectToSvg(oReader.GetInnerXML(), dWidth)};

			std::wcout << wsSVG << std::endl;
			std::wcout << std::endl;

			if (oNewReader.ReadFromString(wsSVG))
			{
				pObject = CObject::Create<CGraphicsContainer>(oNewReader, pFile, pParent);

				if (!ReadChildrens(oNewReader, (CGraphicsContainer*)pObject, pFile, (CGraphicsContainer*)pObject))
				{
					RELEASEINTERFACE(pObject);
					return false;
				}
			}
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
