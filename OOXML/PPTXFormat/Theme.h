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
#ifndef PPTX_THEME_FILE_INCLUDE_H_
#define PPTX_THEME_FILE_INCLUDE_H_

#include "FileContainer.h"
#include "WrapperFile.h"

#include "Theme/ThemeElements.h"
#include "Logic/DefaultShapeDefinition.h"
#include "Theme/ExtraClrScheme.h"

#include "Logic/ClrMap.h"

#include "Presentation.h"
#include "../DocxFormat/Docx.h"

#include "../DocxFormat/Media/Image.h"
#include "../DocxFormat/Media/OleObject.h"
#include "../DocxFormat/External/HyperLink.h"

namespace PPTX
{
	class Theme : public PPTX::WrapperFile, public PPTX::FileContainer
	{
	public:
		Theme(OOX::Document *pMain);
		Theme(OOX::Document *pMain, const std::string &contentTheme);
		Theme(OOX::Document *pMain, const OOX::CPath& filename);
		Theme(OOX::Document *pMain, const OOX::CPath& filename, FileMap& map);

		void read(const std::string &contentTheme, FileMap& map);
		virtual void read(const OOX::CPath& filename, FileMap& map);
		void read(XmlUtils::CXmlNode &oNode, FileMap& map);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const;

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual const OOX::FileType type() const;

		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	//////
		DWORD GetRGBAFromScheme(const std::wstring& str) const;
		DWORD GetARGBFromScheme(const std::wstring& str) const;
		DWORD GetBGRAFromScheme(const std::wstring& str) const;
		DWORD GetABGRFromScheme(const std::wstring& str) const;

	//////
		DWORD GetRGBAFromMap(const std::wstring& str) const;
		DWORD GetARGBFromMap(const std::wstring& str) const;
		DWORD GetBGRAFromMap(const std::wstring& str) const;
		DWORD GetABGRFromMap(const std::wstring& str) const;

		void GetLineStyle(int number, Logic::Ln& lnStyle) const;
		void GetFillStyle(int number, Logic::UniFill& fillStyle) const;

		nullable_string							name;
		nsTheme::ThemeElements					themeElements;
		nullable<Logic::DefaultShapeDefinition> spDef;
		nullable<Logic::DefaultShapeDefinition> lnDef;
		nullable<Logic::DefaultShapeDefinition> txDef;
		
		std::vector<nsTheme::ExtraClrScheme>	extraClrSchemeLst;

		bool									isThemeOverride;

        smart_ptr<Presentation>					presentation;

		void SetColorMap(const Logic::ClrMap& map);
		bool isMapPresent();

	private:
		Logic::ClrMap const* m_map;
	};
} // namespace PPTX

#endif // PPTX_THEME_FILE_INCLUDE_H_
