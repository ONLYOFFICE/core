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

#ifndef CSVGFILE_H
#define CSVGFILE_H

#include "../../../graphics/IRenderer.h"
#include "../../../graphics/pro/Fonts.h"

#include "SvgObjects/CStyle.h"

namespace SVG 
{
	struct TFontArguments;
	class CFont;
	class CGraphicsContainer;
}

class CSvgFile
{
	public:
		CSvgFile();
		~CSvgFile();

		bool ReadFromBuffer(BYTE* pBuffer, unsigned int unSize);
		bool ReadFromWString(const std::wstring& wsContext);
		bool OpenFromFile(const std::wstring& wsFile);

		bool GetBounds(double& dX, double& dY, double& dWidth, double& dHeight) const;
		const SVG::CSvgCalculator* GetSvgCalculator() const;

		void SetFontManager(NSFonts::IFontManager* pFontManager);
		void SetWorkingDirectory(const std::wstring& wsWorkingDirectory);

		bool MarkObject(SVG::CObject* pObject);
		SVG::CObject* GetMarkedObject(const std::wstring& wsId) const;

		SVG::CFont* GetFont(const std::wstring& wsFontFamily) const;

		std::wstring GetWorkingDirectory() const;

		void AddStyles(const std::wstring& wsStyles);
		void AddFontFace(const SVG::TFontArguments& oArguments, const std::wstring& wsId);

		bool Draw(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);
	private:
		void Clear();

		bool CalculateFinalSize(bool bUseViewBox, double& dX, double& dY, double& dWidth, double& dHeight) const;

		SVG::CGraphicsContainer *m_pContainer;
		SVG::CSvgCalculator      m_oSvgCalculator;
		NSFonts::IFontManager   *m_pFontManager;

		typedef std::map<std::wstring, SVG::CObject*> MarkedMap;

		MarkedMap    m_mMarkedObjects;
		std::wstring m_wsWorkingDirectory;

		typedef std::map<std::wstring, std::wstring> FontsFaceMap;
		FontsFaceMap m_mFontsFace;
};

#endif // CSVGFILE_H
