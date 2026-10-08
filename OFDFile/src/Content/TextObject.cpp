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

#include "TextObject.h"

#include "../Utils/Utils.h"
#include "../Utils/CFontChecker.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"

#ifdef BUILDING_WASM_MODULE
#include "../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../PdfFile/SrcReader/FontsWasm.h"
#endif

#include <functional>

namespace OFD
{
CTextCode::CTextCode(CXmlReader& oReader)
{
	if (L"ofd:TextCode" != oReader.GetName() || oReader.IsEmptyElement() || !oReader.IsValid())
		return;

	if (0 != oReader.GetAttributesCount() && oReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oReader.GetName();

			if (L"X" == wsAttributeName)
				m_dX = oReader.GetDouble(true);
			else if (L"Y" == wsAttributeName)
				m_dY = oReader.GetDouble(true);
			else if (L"DeltaX" == wsAttributeName ||
			         L"DeltaY" == wsAttributeName)
			{
				const std::vector<std::string> arValues{oReader.GetArrayStrings(true)};

				std::vector<double>& arDelta{L"DeltaX" == wsAttributeName ? m_arDeltaX : m_arDeltaY};

				arDelta.reserve(arValues.size());
				double dValue = 0.;

				for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
				{
					if ("g" == arValues[unIndex] && unIndex + 2 < arValues.size())
					{
						unsigned int unCount = 0;

						if (!StringToUInteger(arValues[unIndex + 1], unCount) || !StringToDouble(arValues[unIndex + 2], dValue))
							continue;

						unIndex += 2;

						arDelta.insert(arDelta.end(), unCount, dValue);
					}
					else if (StringToDouble(arValues[unIndex], dValue))
						arDelta.push_back(dValue);
					else
						arDelta.push_back(0.);
				}
			}
			} while (oReader.MoveToNextAttribute());
	}

		oReader.MoveToElement();

		m_wsText = oReader.GetText2();
}

HRESULT DrawGlyphWithFontUpdate(IRenderer* pRenderer, NSFonts::IFontManager* pFontManager, unsigned int unUnicode, std::function<HRESULT()> CommandDrawTextCHAR)
{
	if (nullptr == pRenderer)
		return S_FALSE;

	std::wstring sFontPath;
	double dSize{0.};
	bool bReplace{false};

	pRenderer->get_FontPath(&sFontPath);
	pRenderer->get_FontSize(&dSize);

	#ifdef BUILDING_WASM_MODULE
	if (nullptr != pFontManager && !sFontPath.empty())
	{
		long lStyle;
		double dDpiX, dDpiY;
		pRenderer->get_FontStyle(&lStyle);
		pRenderer->get_DpiX(&dDpiX);
		pRenderer->get_DpiY(&dDpiY);
		pFontManager->SetStringGID(FALSE);
		pFontManager->LoadFontFromFile(sFontPath, 0, dSize, dDpiX, dDpiY);

		NSFonts::IFontFile* pFontFile{pFontManager->GetFile()};

		if (nullptr != pFontFile)
		{
			int nCMapIndex = 0;
			int GID = pFontFile->SetCMapForCharCode(unUnicode, &nCMapIndex);
			if (GID <= 0 && unUnicode < 0xF000)
				GID = pFontFile->SetCMapForCharCode(unUnicode + 0xF000, &nCMapIndex);

			if (GID <= 0)
			{
				std::wstring sName{pFontManager->GetApplication()->GetFontBySymbol(unUnicode)};
				int bBold   = lStyle & 0x01 ? 1 : 0;
				int bItalic = lStyle & 0x02 ? 1 : 0;

				if (!sName.empty())
				{
					if (!NSWasm::IsJSEnv())
					{
						NSFonts::CFontSelectFormat oFormat;
						oFormat.wsName  = new std::wstring(sName);
						oFormat.bBold   = new INT(bBold);
						oFormat.bItalic = new INT(bItalic);
						NSFonts::CFontInfo* pFontInfo = pFontManager->GetFontInfoByParams(oFormat);

						sName = pFontInfo->m_wsFontPath;
					}

					std::wstring wsFileName{NSWasm::LoadFont(sName, bBold, bItalic)};

					if (wsFileName.empty())
						return S_FALSE;

					pRenderer->put_FontPath(wsFileName);
					pFontManager->LoadFontFromFile(wsFileName, 0, dSize, dDpiX, dDpiY);
					bReplace = true;
				}
			}
		}
	}
	#endif

	HRESULT res{CommandDrawTextCHAR()};

	if(bReplace)
		pRenderer->put_FontPath(sFontPath);

	return res;
}

void CTextCode::Draw(IRenderer* pRenderer, unsigned int& unIndex, const std::vector<TCGTransform>& arCGTransforms, double dScale, NSFonts::IFontManager* pFontManager) const
{
	if (nullptr == pRenderer || m_wsText.empty())
		return;

	double dX = m_dX, dY = m_dY, dDeltaX = 0, dDeltaY = 0;

	for (unsigned int unGlyphIndex = 0; unGlyphIndex < m_wsText.length(); ++unGlyphIndex)
	{
		const double dGlyphX = dX * dScale, dGlyphY = dY * dScale;
		bool bDrawed = false;

		for (const TCGTransform& oCGTransform : arCGTransforms)
		{
			if (oCGTransform.Draw(pRenderer, m_wsText[unGlyphIndex], unIndex, dGlyphX, dGlyphY, pFontManager))
			{
				bDrawed = true;
				break;
			}
		}

		if (!bDrawed)
			DrawGlyphWithFontUpdate(pRenderer, pFontManager, m_wsText[unGlyphIndex], [pRenderer, chChar = m_wsText[unGlyphIndex], dGlyphX, dGlyphY](){ return pRenderer->CommandDrawTextCHAR(chChar, dGlyphX, dGlyphY, 0, 0); });

		++unIndex;

		if (unGlyphIndex < m_arDeltaX.size())
			dDeltaX = m_arDeltaX[unGlyphIndex];

		if (unGlyphIndex < m_arDeltaY.size())
			dDeltaY = m_arDeltaY[unGlyphIndex];

		dX += dDeltaX;
		dY += dDeltaY;
	}
}

CTextObject::CTextObject(CXmlReader& oReader)
	: IPageBlock(oReader), CGraphicUnit(oReader),
	  m_bStroke(false), m_bFill(true), m_dHScale(1.),
	  m_unReadDirection(0), m_unCharDirection(0), m_unWeight(400),
	  m_bItalic(false),
	  m_pFillColor(nullptr), m_pStrokeColor(nullptr), m_unFontID(0)
{
	if (L"ofd:TextObject" != oReader.GetName() || oReader.IsEmptyElement() || !oReader.IsValid())
		return;

	if (0 != oReader.GetAttributesCount() && oReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oReader.GetName();

			if (L"Font" == wsAttributeName)
				m_unFontID = oReader.GetUInteger(true);
			else if (L"Size" == wsAttributeName)
				m_dSize = oReader.GetDouble(true);
			else if (L"Stroke" == wsAttributeName)
				m_bStroke = oReader.GetBoolean(true);
			else if (L"Fill" == wsAttributeName)
				m_bFill = oReader.GetBoolean(true);
			else if (L"HScale" == wsAttributeName)
				m_dHScale = oReader.GetDouble(true);
			else if (L"ReadDirection" == wsAttributeName)
				m_unReadDirection = oReader.GetUInteger(true);
			else if (L"CharDirection" == wsAttributeName)
				m_unCharDirection =oReader.GetUInteger(true);
			else if (L"Weight" == wsAttributeName)
				m_unWeight = oReader.GetUInteger(true);
			else if (L"Italic" == wsAttributeName)
				m_bItalic = oReader.GetBoolean(true);
		} while (oReader.MoveToNextAttribute());
	}

	oReader.MoveToElement();

	const int nDepth = oReader.GetDepth();
	std::wstring wsNodeName;

	while (oReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oReader.GetName();

		if (L"ofd:FillColor" == wsNodeName)
		{
			if (nullptr != m_pFillColor)
				delete m_pFillColor;

			m_pFillColor = new CColor(oReader);
		}
		else if (L"ofd:StrokeColor" == wsNodeName)
		{
			if (nullptr != m_pStrokeColor)
				delete m_pStrokeColor;

			m_pStrokeColor = new CColor(oReader);
		}
		else if (L"ofd:TextCode" == wsNodeName)
			m_arTextCodes.push_back(new CTextCode(oReader));
		else if (L"ofd:CGTransform" == wsNodeName)
			m_arCGTransforms.push_back(TCGTransform::Read(oReader));
		else
			ReadChildren(oReader);
	}
}

CTextObject::~CTextObject()
{
	if (nullptr != m_pFillColor)
		delete m_pFillColor;

	if (nullptr != m_pStrokeColor)
		delete m_pStrokeColor;

	for (const CTextCode* pTextCode : m_arTextCodes)
		delete pTextCode;
}

void CTextObject::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer || m_arTextCodes.empty())
		return;

	const CRes* pPublicRes{oCommonData.GetPublicRes()};

	if (nullptr == pPublicRes)
		return;

	const CFont* pFont{pPublicRes->GetFont(m_unFontID)};

	if (nullptr == pFont)
		return;

	pFont->Apply(pRenderer);

	TMatrix oOldTransform;
	CGraphicUnit::Apply(pRenderer, oOldTransform);

	double dM11, dM12, dM21, dM22, dDx, dDy;
	pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

	double dTextScale = std::min(std::sqrt(dM11 * dM11 + dM12 * dM12), std::sqrt(dM21 * dM21 + dM22 * dM22));

	if (IsZeroValue(dTextScale))
		dTextScale = 1.;
	else
		pRenderer->SetTransform(dM11 / dTextScale, dM12 / dTextScale, dM21 / dTextScale, dM22 / dTextScale, dDx, dDy);

	std::vector<const CDrawParam*> arDrawParams{pPublicRes->GetDrawParams()};

	if (m_bFill)
	{
		pRenderer->put_BrushType(c_BrushTypeSolid);

		if (nullptr != m_pFillColor)
		{
			pRenderer->put_BrushColor1(m_pFillColor->ToInt(pPublicRes));
			pRenderer->put_BrushAlpha1(m_pFillColor->GetAlpha());
		}
		else
		{
			pRenderer->put_BrushColor1(BLACK_COLOR_INT);
			pRenderer->put_BrushAlpha1(255);

			if (EPageType::TemplatePage == ePageType)
				for (const CDrawParam* pDrawParam : arDrawParams)
					if (pDrawParam->ApplyFillColor(pRenderer, pPublicRes))
						break;
		}
	}
	else
		pRenderer->put_BrushType(c_BrushTypeNotSet);

	pRenderer->put_FontSize(m_dSize * dTextScale * 72. / 25.4);

	unsigned int unGlyphsIndex = 0;

	for (const CTextCode* pTextCode : m_arTextCodes)
		#ifdef BUILDING_WASM_MODULE
		pTextCode->Draw(pRenderer, unGlyphsIndex, m_arCGTransforms, dTextScale, pFont->GetFontManager());
		#else
		pTextCode->Draw(pRenderer, unGlyphsIndex, m_arCGTransforms, dTextScale);
		#endif

	pRenderer->SetTransform(oOldTransform.m_dM11, oOldTransform.m_dM12, oOldTransform.m_dM21, oOldTransform.m_dM22, oOldTransform.m_dDx, oOldTransform.m_dDy);
}

#ifdef BUILDING_WASM_MODULE
void CTextObject::GetLinks(NSWasm::CData& oRes) const
{
	CGraphicUnit::GetLinks(oRes);
}
#endif

TCGTransform TCGTransform::Read(CXmlReader& oReader)
{
	TCGTransform oCGTransform;

	if (L"ofd:CGTransform" != oReader.GetName() || oReader.IsEmptyElement())
		return oCGTransform;

	if (0 != oReader.GetAttributesCount() && oReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oReader.GetName();

			if (L"CodePosition" == wsAttributeName)
				oCGTransform.m_unCodePosition = oReader.GetUInteger(true);
			else if (L"CodeCount" == wsAttributeName)
				oCGTransform.m_unCodeCount = oReader.GetUInteger(true);
			else if (L"GlyphCount" == wsAttributeName)
				oCGTransform.m_unGlyphCount = oReader.GetUInteger(true);
		} while (oReader.MoveToNextAttribute());
	}

	oReader.MoveToElement();

	const int nDepth = oReader.GetDepth();

	while (oReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:Glyphs" != oReader.GetNameA())
			continue;

		const std::vector<unsigned int> arValues{oReader.GetArrayUInteger()};
		oCGTransform.m_arGlyphs.insert(oCGTransform.m_arGlyphs.end(), arValues.begin(), arValues.end());
	}

	return oCGTransform;
}

bool TCGTransform::Draw(IRenderer* pRenderer, const LONG& lUnicode, unsigned int unIndex, double dX, double dY, NSFonts::IFontManager* pFontManager) const
{
	if (nullptr == pRenderer || 0 == m_unCodeCount || m_arGlyphs.empty())
		return false;

	if (unIndex < m_unCodePosition || unIndex >= m_unCodePosition + m_unCodeCount)
		return false;

	if (m_arGlyphs.size() != m_unCodeCount)
		return false;

	std::wstring wsFontPath;
	pRenderer->get_FontPath(&wsFontPath);

	if (wsFontPath.empty())
		return false;

	const HRESULT hRes{pRenderer->CommandDrawTextExCHAR(lUnicode, (LONG)m_arGlyphs[unIndex - m_unCodePosition], dX, dY, 0, 0)};

	pRenderer->put_FontStringGID(FALSE);

	return S_OK == hRes;
}
}
