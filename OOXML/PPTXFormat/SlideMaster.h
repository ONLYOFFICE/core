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
