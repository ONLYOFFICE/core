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

#ifndef RES_H
#define RES_H

#include <string>

#include "Utils/CFontChecker.h"
#include "Types/ColorSpace.h"
#include "Types/DrawParam.h"
#include "Types/Font.h"
#include "Types/MultiMedia.h"
#include "Types/CompositeGraphicUnit.h"

class IFolder;

namespace OFD
{
class CRes
{
	std::map<unsigned int, CColorSpace*>           m_mColorSpaces;
	std::map<unsigned int, CDrawParam*>            m_mDrawParams;
	std::map<unsigned int, CFont*>                 m_mFonts;
	std::map<unsigned int, CMultiMedia*>           m_mMultiMedias;
	std::map<unsigned int, CCompositeGraphicUnit*> m_mCCompositeGraphicUnits;
public:
	CRes();
	~CRes();

	bool Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath, IFolder* pFolder);

	const CColorSpace*           GetColorSpace(unsigned int unId) const;
	const CDrawParam*            GetDrawParam(unsigned int unId) const;
	const CFont*                 GetFont(unsigned int unId) const;
	const CMultiMedia*           GetMultiMedia(unsigned int unId) const;
	const CCompositeGraphicUnit* GetCompositeGraphicUnit(unsigned int unId) const;

	std::vector<const CDrawParam*> GetDrawParams() const;

	void UpdateFonts(CFontChecker* pFontChecker);
};
}

#endif // RES_H
