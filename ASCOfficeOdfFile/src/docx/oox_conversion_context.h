/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include <sstream>
#include <iosfwd>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace cpdoccore { 

namespace odf_reader{
	class style_instance;
};

class styles_context : boost::noncopyable
{
public:
    void start();
   
	std::wstringstream & text_style();
    std::wstringstream & paragraph_nodes();
    std::wstringstream & paragraph_attr();
    std::wstringstream & table_style();
    std::wstringstream & list_style();
	std::wstringstream & math_text_style();

    void docx_serialize_text_style(std::wostream & strm, std::wstring parenStyleId);
    void docx_serialize_table_style(std::wostream & strm);
  
	void pptx_serialize_table_style(std::wostream & strm);

	std::wstring & extern_node(){return extern_node_;}
	std::wstring & hlinkClick(){return hlinkClick_;}

	const odf_reader::style_instance * get_current_processed_style() const { return current_processed_style_; }
    void start_process_style(const odf_reader::style_instance * Instance);
    void end_process_style();

private:
	const odf_reader::style_instance *	current_processed_style_;
	
	std::wstring				extern_node_;
	std::wstring				hlinkClick_;

    std::wstringstream			list_style_;
    std::wstringstream			text_style_;
    std::wstringstream			paragraph_nodes_;
    std::wstringstream			paragraph_attr_;
    std::wstringstream			table_style_;
	std::wstringstream			math_text_style_;
};


namespace oox {
	

}
}

