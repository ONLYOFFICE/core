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
#pragma once
#ifndef PPTX_TABLESTYLES_FILE_INCLUDE_H_
#define PPTX_TABLESTYLES_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "Logic/TableStyle.h"
#include "Theme.h"
#include "FileTypes.h"

namespace PPTX
{
	class TableStyles: public WrapperFile
	{
	public:
		TableStyles(OOX::Document* pMain);
		TableStyles(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map);
		virtual ~TableStyles();

		virtual void read(const OOX::CPath& filename, FileMap& map);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const;

		virtual const OOX::FileType type() const;

		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;


		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

	public:
		std::wstring def;
		std::map<std::wstring, Logic::TableStyle> Styles;

		void SetTheme(const smart_ptr<PPTX::Theme> theme);

		virtual DWORD GetRGBAFromMap(const std::wstring& str) const;
		virtual DWORD GetARGBFromMap(const std::wstring& str) const;
		virtual DWORD GetBGRAFromMap(const std::wstring& str) const;
		virtual DWORD GetABGRFromMap(const std::wstring& str) const;

	private:
		smart_ptr<PPTX::Theme> m_Theme;
	};
} // namespace PPTX

#endif // PPTX_TABLESTYLES_FILE_INCLUDE_H_
