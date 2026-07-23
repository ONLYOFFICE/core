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

#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include "IPageBlock.h"
#include "GraphicUnit.h"

#include "../Types/Color.h"

namespace OFD
{
struct TCGTransform
{
	unsigned int m_unCodePosition;
	unsigned int m_unCodeCount;
	unsigned int m_unGlyphCount;

	std::vector<unsigned int> m_arGlyphs;

	static TCGTransform Read(CXmlReader& oReader);

	bool Draw(IRenderer* pRenderer, const LONG& lUnicode, unsigned int& unIndex, double dX, double dY, NSFonts::IFontManager* pFontManager) const;
};

class CTextCode
{
	double m_dX;
	double m_dY;

	std::vector<double> m_arDeltaX;
	std::vector<double> m_arDeltaY;

	std::wstring m_wsText;
public:
	CTextCode(CXmlReader& oReader);

	void Draw(IRenderer* pRenderer, unsigned int& unIndex, const std::vector<TCGTransform>& arCGTransforms, NSFonts::IFontManager* pFontManager = nullptr) const;
};

class CTextObject : public IPageBlock, public CGraphicUnit
{
	double m_dSize;
	bool m_bStroke;
	bool m_bFill;
	double m_dHScale;
	unsigned int m_unReadDirection;
	unsigned int m_unCharDirection;
	unsigned int m_unWeight;
	bool m_bItalic;

	CColor* m_pFillColor;
	CColor* m_pStrokeColor;

	unsigned int m_unFontID;

	std::vector<const CTextCode*> m_arTextCodes;
	std::vector<TCGTransform>     m_arCGTransforms;
public:
	CTextObject(CXmlReader& oReader);
	~CTextObject();

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const override;

	#ifdef BUILDING_WASM_MODULE
	virtual void GetLinks(NSWasm::CData& oRes) const override;
	#endif
};
}

#endif // TEXTOBJECT_H
