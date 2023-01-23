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

#include "WrapperFile.h"
#include "FileContainer.h"

#include "Logic/ClrMapOvr.h"
#include "Logic/CSld.h"
#include "Logic/Transitions/Transition.h"
#include "Logic/Timing/Timing.h"

#include "Logic/Bg.h"

#include "Theme.h"
#include "SlideMaster.h"
#include "SlideLayout.h"
#include "NotesSlide.h"
#include "TableStyles.h"

#include "../DocxFormat/External/HyperLink.h"
#include "../DocxFormat/VmlDrawing.h"

#include "Comments.h"

namespace PPTX
{
	class Slide : public WrapperFile, public FileContainer
	{
	public:
		Slide(OOX::Document* pMain);
		Slide(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map);
		virtual ~Slide();

		virtual void read(const OOX::CPath& filename, FileMap& map);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const;

		virtual const OOX::FileType type() const;

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

//--------------------------------------------------------------------------------------
		void ApplyRels();
		std::wstring GetVmlXmlBySpid(const std::wstring& spid);
	};
} // namespace PPTX
