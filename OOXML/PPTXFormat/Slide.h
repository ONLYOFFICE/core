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

#include "WrapperFile.h"
#include "FileContainer.h"
#include "Comments.h"

namespace OOX
{
	class CVmlDrawing;
}

namespace PPTX
{
	namespace Logic
	{
		class ClrMapOvr;
		class CSld;
		class Transition;
		class Timing;
	}

	class SlideLayout;
	class SlideMaster;
	class NotesSlide;
	class Theme;
	class TableStyles;

	class Slide : public WrapperFile, public FileContainer
	{
	public:
		Slide(OOX::Document* pMain);
		Slide(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map);
		virtual ~Slide();

		virtual void read(const OOX::CPath& filename, FileMap& map);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const;

		virtual const OOX::FileType type() const;

		void fromXML(XmlUtils::CXmlNode& node);

		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

//-------------------------------------------------
		virtual DWORD GetRGBAFromMap(const std::wstring& str) const;
		virtual DWORD GetARGBFromMap(const std::wstring& str) const;
		virtual DWORD GetBGRAFromMap(const std::wstring& str) const;
		virtual DWORD GetABGRFromMap(const std::wstring& str) const;

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

		nullable_bool		show;
		nullable_bool		showMasterPhAnim;
		nullable_bool		showMasterSp;

		nullable<Logic::ClrMapOvr>	clrMapOvr;
		nullable<Logic::CSld>		cSld;
		nullable<Logic::Transition> transition;
		nullable<Logic::Timing>		timing;

		smart_ptr<SlideLayout>		Layout;
		smart_ptr<SlideMaster>		Master;
		smart_ptr<NotesSlide>		Note;
		smart_ptr<Theme>			theme;
		smart_ptr<OOX::CVmlDrawing>	Vml;
		smart_ptr<TableStyles>		tableStyles_;

		smart_ptr<PPTX::Comments>	comments;
		nullable_string ridModernComment;

//--------------------------------------------------------------------------------------
		void ApplyRels();
		std::wstring GetVmlXmlBySpid(const std::wstring& spid);
	};
} // namespace PPTX
