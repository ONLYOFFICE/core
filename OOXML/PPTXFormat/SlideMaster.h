﻿/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#ifndef PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_
#define PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"

#include "Logic/ClrMap.h"
#include "Logic/CSld.h"
#include "Logic/XmlId.h"

namespace OOX
{
	class CVmlDrawing;
}

namespace PPTX
{
	namespace Logic
	{
		class Transition;
		class Timing;
		class HF;
		class TxStyles;
	}

	class Theme;
	class TableStyles;

	class SlideMaster : public WrapperFile, public FileContainer
	{
	public:
		SlideMaster(OOX::Document* pMain);
		SlideMaster(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map);
		virtual ~SlideMaster();

		virtual void read(const OOX::CPath& filename, FileMap& map);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const;

		virtual const OOX::FileType type() const;

		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

		void GetLevelUp(WrapperWritingElement* pElem);

//---------------------Colors from map---------------------------------------
		DWORD GetRGBAFromMap(const std::wstring& str) const;
		DWORD GetARGBFromMap(const std::wstring& str) const;
		DWORD GetBGRAFromMap(const std::wstring& str) const;
		DWORD GetABGRFromMap(const std::wstring& str) const;

//---------------------------Colors from scheme------------------------------
		DWORD GetRGBAFromScheme(const std::wstring& str) const;
		DWORD GetARGBFromScheme(const std::wstring& str) const;
		DWORD GetBGRAFromScheme(const std::wstring& str) const;
		DWORD GetABGRFromScheme(const std::wstring& str) const;

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

		Logic::CSld					cSld;
		Logic::ClrMap				clrMap;
		std::vector<Logic::XmlId>	sldLayoutIdLst;
		nullable<Logic::Transition> transition;
		nullable<Logic::Timing>		timing;
		nullable<Logic::HF>			hf;
		nullable<Logic::TxStyles>	txStyles;
		nullable_bool				preserve;

        smart_ptr<Theme>			theme;
        smart_ptr<TableStyles>		tableStyles;
		smart_ptr<OOX::CVmlDrawing>	Vml;
		
		void ApplyRels();
		std::wstring GetVmlXmlBySpid(const std::wstring& spid);
	};
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_
