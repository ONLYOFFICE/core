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

