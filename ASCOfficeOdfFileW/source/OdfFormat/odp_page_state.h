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

#include <string>
#include <vector>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include"../../../Common/DocxFormat/Source/XML/Utils.h"

#include "odf_drawing_context.h"

#include "office_elements_create.h"



namespace cpdoccore {

namespace odf_types
{
	class color;
}

namespace odf_writer {

class odp_conversion_context;
class odf_text_context;

//class table_table;
class style;


struct odp_element_state
{
	office_element_ptr	elm;
	
    short				repeated;
	std::wstring		style_name;
	office_element_ptr	style_elm;
};


class odp_page_state
{
public:
	odp_page_state(odf_conversion_context * Context, office_element_ptr & elm);
		void set_page_name(std::wstring name);
		void set_page_style(office_element_ptr & _style);
		void set_master_page(std::wstring name);
	
	void add_child_element( const office_element_ptr & child_element);

///////////////////////////////
	odf_drawing_context   *	drawing_context(){return  &drawing_context_;}

	std::wstring		office_page_name_;
	office_element_ptr	draw_page_;
private:

    odf_conversion_context * context_;   
	
	style*				office_page_style_;
	

	odf_drawing_context		drawing_context_;	

	friend class odp_slide_context;

};


}
}

