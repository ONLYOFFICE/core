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

#include "odf_conversion_context.h"
#include "odp_slide_context.h"

namespace cpdoccore { 
namespace odf_writer {

class office_presentation;
class odf_text_context;

struct table_style_state
{
	std::wstring default_;
	std::wstring first_row_;
	std::wstring first_col_;
	std::wstring band_row_;
	std::wstring band_col_;
	std::wstring last_row_;
	std::wstring last_col_;
};
class odp_conversion_context : public odf_conversion_context
{
public:
	odp_conversion_context(package::odf_document * outputDocument);
	
	virtual void start_document();
	virtual void end_document();

	void start_slide();
	void end_slide();

	size_t get_pages_count();

	void start_master_slide(std::wstring name);
	void end_master_slide();

	void start_layout_slide();
	void end_layout_slide();

	odp_page_state & current_slide() { return slide_context_.state();}

/////////////////////////////////////////////////////

	virtual odf_drawing_context		* drawing_context();
	virtual odf_controls_context	* controls_context();
	virtual odf_text_context		* text_context();
			odp_slide_context		* slide_context();
			odf_comment_context		* comment_context();

	virtual odf_style_context		* styles_context();

	void start_comment			(int oox_comment_id);
	void end_comment			();
	void start_comment_content	();
	void end_comment_content	();

	void start_note(bool bMaster = false);
	void end_note();

	std::map<std::wstring, table_style_state> map_table_styles_;
private:
	odp_slide_context			slide_context_;
	office_presentation*		root_presentation_;
};


}
}
