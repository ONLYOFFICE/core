/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "Converter.h"

#include <boost/foreach.hpp>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>

#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Word.h"

namespace OOX 
{
	class CTheme;

}
namespace PPTX
{
	class TableStyles;
	class Presentation;
	class Folder;

	namespace Logic
	{
		class CSld;
		class Bg;
		class Transition;
		class Timing;
		class TablePartStyle;
		class Table;
		class TableProperties;
		class TableRow;
		class TableCell;
		class TableCellProperties;
		class TcBdr;
	}
}

namespace cpdoccore 
{
	namespace odf_writer 
	{
		namespace package 
		{		
			class odf_document;
		}
		class ods_conversion_context;
		class odf_conversion_context;
		class odt_conversion_context;
		class odp_conversion_context;
	}
}

using namespace cpdoccore;

namespace Oox2Odf
{
	class PptxConverter : public OoxConverter
	{
	public:
		PptxConverter(const std::wstring & path, const ProgressCallback* ffCallBack);
		~PptxConverter();

		virtual void convertDocument();
		virtual void write(const std::wstring & path);
		
		virtual odf_writer::odf_conversion_context		*odf_context();		
		virtual PPTX::Theme								*oox_theme();
		virtual PPTX::Logic::ClrMap						*oox_clrMap();
        
		virtual std::wstring							find_link_by_id (std::wstring sId, int t);
		virtual NSCommon::smart_ptr<OOX::File>			find_file_by_id	(std::wstring sId);

		void convert(OOX::WritingElement *oox_unknown);
 		
		void convert(PPTX::Logic::CSld					*oox_slide, bool placeholders = true);
		void convert(PPTX::Logic::Bg					*oox_background);
		
		void convert(PPTX::Logic::Table					*oox_table);
		void convert(PPTX::Logic::TableRow				*oox_table_row);
		void convert(PPTX::Logic::TableCell				*oox_table_cell, int numCol);
		
 		void convert(PPTX::Logic::TablePartStyle		*oox_table_part_style);
		bool convert(PPTX::Logic::TableCellProperties	*oox_table_cell_pr, int col);
		bool convert(PPTX::Logic::TableCellProperties	*oox_table_cell_pr); 
		
		void convert(PPTX::Logic::TableProperties		*oox_table_pr);
		void convert(PPTX::Logic::TcBdr					*oox_table_borders);

		void convert(PPTX::Logic::Ln *oox_ln_border, std::wstring & odf_border);
private:
		PPTX::Folder									*pptx_document;
		PPTX::Presentation								*presentation;
		cpdoccore::odf_writer::package::odf_document	*output_document;
		
		PPTX::Theme										*current_theme;		
		PPTX::TableStyles								*current_tableStyles;

		OOX::IFileContainer								*current_slide;
		PPTX::Logic::ClrMap								*current_clrMap;
		
		odf_writer::odp_conversion_context				*odp_context;

		std::map<std::wstring, std::wstring>			m_mapMasters;
		std::vector<std::wstring>						m_arLayouts;
		
		void convert_slides			();
		void convert_styles			();
		void convert_settings		();

		
//--------------------------------------------------------------------------------
	};
}
