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

#include <iosfwd>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "oox_rels.h"

namespace oox {

class xlsx_xml_worksheet;
typedef boost::shared_ptr<xlsx_xml_worksheet> xlsx_xml_worksheet_ptr;

class xlsx_xml_worksheet: boost::noncopyable
{
public:
    xlsx_xml_worksheet();
    ~xlsx_xml_worksheet();

	std::wstring	name;
    std::wstring	state;
	int				type;
 	int				id;
  
    std::wostream & dimension();
	std::wostream & sheetViews();
	std::wostream & cols();
	std::wostream & sheetProperties();
    std::wostream & sheetFormat();
    std::wostream & sheetData();
    std::wostream & hyperlinks();
    std::wostream & mergeCells();
    std::wostream & drawing(); 
	std::wostream & ole_objects();
	std::wostream & activeXs();
    //std::wostream & comments(); 
	std::wostream & sheetSortAndFilters();
	std::wostream & pageProperties();
	std::wostream & customViews();
	std::wostream & conditionalFormatting();
	std::wostream & picture_background();
	std::wostream & dataValidations();
	std::wostream & protection();
	std::wostream & tableParts();
	std::wostream & sheetCalcPr();

	rels & sheet_rels();//hyperlink, background image, external, media ...

    void write_to(std::wostream & strm);

    void set_drawing_link		(std::wstring const & fileName, std::wstring const & id);
    void set_vml_drawing_link	(std::wstring const & fileName, std::wstring const & id);
    void set_comments_link		(std::wstring const & fileName, std::wstring const & id);
    void set_vml_HF_drawing_link(std::wstring const & fileName, std::wstring const & id);
	
	std::pair<std::wstring, std::wstring> get_drawing_link()		const;
	std::pair<std::wstring, std::wstring> get_vml_drawing_link()	const;
	std::pair<std::wstring, std::wstring> get_vml_drawing_HF_link() const;
	std::pair<std::wstring, std::wstring> get_comments_link()		const;

    static xlsx_xml_worksheet_ptr create();

private:
    class Impl;
    boost::scoped_ptr<Impl> impl_;
};


}

