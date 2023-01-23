/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
