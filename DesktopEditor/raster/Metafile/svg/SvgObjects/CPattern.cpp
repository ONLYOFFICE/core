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

#include "CPattern.h"
#include <iostream>

#include "../../../graphics/pro/Graphics.h"

namespace SVG
{
	CPattern::CPattern(CSvgReader& oReader, NSFonts::IFontManager *pFontManager)
		: CAppliedObject(oReader), m_oContainer(oReader), m_pFontManager(pFontManager),
		  m_pImage(NULL), m_enPatternUnits(objectBoundingBox)
	{}

	CPattern::~CPattern()
	{
		RELEASEINTERFACE(m_pImage);
	}

	void CPattern::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"patternUnits"))
		{
			const std::wstring& wsValue = mAttributes.at(L"patternUnits");

			if (L"objectBoundingBox" == wsValue)
				m_enPatternUnits = objectBoundingBox;
			else if (L"userSpaceOnUse" == wsValue)
				m_enPatternUnits = userSpaceOnUse;
		}
	}

	void CPattern::Update(const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (NULL != m_pImage)
			RELEASEINTERFACE(m_pImage);

		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		pGrRenderer->SetFontManager(m_pFontManager);

		double dMMtoPx = 96. / 25.4;

		double dKoefWidth  = m_oContainer.m_oWindow.m_oWidth  .ToDouble(NSCSS::Pixel);
		double dKoefHeight = m_oContainer.m_oWindow.m_oHeight .ToDouble(NSCSS::Pixel);

		if (objectBoundingBox == m_enPatternUnits)
		{
			dKoefWidth  *= oObjectBounds.m_dRight  - oObjectBounds.m_dLeft;
			dKoefHeight *= oObjectBounds.m_dBottom - oObjectBounds.m_dTop;
		}

		int nWidth  = dKoefWidth  * dMMtoPx + 1;
		int nHeight = dKoefHeight * dMMtoPx + 1;

		if (0 == nWidth || 0 == nHeight)
			return;

		BYTE* pBgraData = new(std::nothrow) BYTE[nWidth * nHeight * 4];

		if (!pBgraData)
			return;

		unsigned int alfa = 0xffffff;
		//default tone should be transparent, not white
		//memset(pBgraData, 0xff, nWidth * nHeight * 4);
		for (int i = 0; i < nWidth * nHeight; i++)
			((unsigned int*)pBgraData)[i] = alfa;

		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		pGrRenderer->CreateFromBgraFrame(&oFrame);
		pGrRenderer->put_Width(nWidth);
		pGrRenderer->put_Height(nHeight);

		pGrRenderer->SetSwapRGB(false);
		pGrRenderer->BeginCommand(c_nImageType);

		pGrRenderer->SetTransform(dMMtoPx, 0., 0., dMMtoPx, 0., 0.);

		//Rendering
		m_oContainer.Draw(pGrRenderer, pFile);

		pGrRenderer->EndCommand(c_nImageType);
		RELEASEINTERFACE(pGrRenderer);

		oFrame.put_Data(NULL);

		m_pImage = new Aggplus::CImage;
		m_pImage->Create(pBgraData, oFrame.get_Width(), oFrame.get_Height(), oFrame.get_Stride());
	}

	bool CPattern::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		Update(pFile, oObjectBounds);

		if (NULL == pRenderer || NULL == m_pImage)
			return false;

		pRenderer->put_BrushType(c_BrushTypeTexture);
		pRenderer->put_BrushTextureMode(c_BrushTextureModeTile);
		pRenderer->put_BrushTextureImage(m_pImage);

		RELEASEINTERFACE(m_pImage);

		return true;
	}

	CGraphicsContainer &CPattern::GetContainer()
	{
		return m_oContainer;
	}
}
