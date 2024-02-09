﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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

#include <string>
#include <boost/noncopyable.hpp>
#include <CPScopedPtr.h>
#include <CPOptional.h>

#include <xml/attributes.h>


namespace cpdoccore {
namespace odf_reader
{
	class odf_read_context;

	class styles_container;
	typedef boost::shared_ptr<styles_container> styles_container_ptr;
	
	class text_format_properties;
	typedef boost::shared_ptr<text_format_properties> text_format_properties_ptr;
};
namespace oox {
	
class xlsx_conversion_context;

class xlsx_text_context: boost::noncopyable
{
public:
	xlsx_text_context	(odf_reader::odf_read_context & odfContext);
    ~xlsx_text_context	();

	void set_local_styles_container	(odf_reader::styles_container *local_styles_);
	void set_cell_text_properties	(odf_reader::text_format_properties_ptr text_properties);    
    
    void			start_paragraph(const std::wstring & styleName);
    void			end_paragraph();

    void			start_span(const std::wstring & styleName);
    void			end_span();
    std::wstring	end_span2();

    void			start_cell_content();
    int				end_cell_content(bool need_cache);

	void			start_only_text();
	std::wstring	end_only_text();

	void			start_comment_content();
	std::wstring	end_comment_content();
	
	void			start_drawing_content();
	std::wstring	end_drawing_content();

	void			start_hyperlink();
	void			end_hyperlink(std::wstring hId);

	void add_text(const std::wstring & text);
	void add_paragraph(const std::wstring & para);

    void serialize_shared_strings(std::wostream & strm);

	bool is_drawing_context();

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;

   
};

}
}
