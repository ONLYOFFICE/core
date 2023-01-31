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
