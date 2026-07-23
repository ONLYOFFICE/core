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

#include <string>
#include "../../Common/CPSharedPtr.h"
#include "../../Common/CPScopedPtr.h"

struct ProgressCallback;

namespace cpdoccore { 

namespace xml
{
    class sax;
}

namespace oox {
	class docx_conversion_context;
	class xlsx_conversion_context;
	class pptx_conversion_context;
}

namespace odf_reader {

class odf_read_context;

class odf_document
{
public:
    odf_document(const std::wstring & SrcPath, const std::wstring & TempPath, const std::wstring& Password);
	odf_document(xml::sax * Reader, const std::wstring & TempPath);
	
	~odf_document();

    odf_read_context & odf_context();
    
	bool docx_convert(oox::docx_conversion_context & Context);
    bool xlsx_convert(oox::xlsx_conversion_context & Context);
	bool pptx_convert(oox::pptx_conversion_context & Context);
	
	const std::wstring & get_folder()		const;
	const std::wstring & get_temp_folder()	const;
	
	int		get_office_mime_type();
	bool	get_encrypted();
	bool	get_error();

    class Impl;   
    Impl * get_impl() { return impl_.get(); }

private:
    _CP_SCOPED_PTR(Impl) impl_;
            
};
typedef shared_ptr<odf_document>::Type odf_document_ptr;

}
}
