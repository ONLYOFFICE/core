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

#include "odf_document.h"
#include "odf_document_impl.h"

namespace cpdoccore { 
namespace odf_reader {

odf_document::odf_document(const std::wstring & SrcPath, const std::wstring & TempPath, const std::wstring & Password)
    : impl_(new Impl(SrcPath, TempPath, Password))
{}

odf_document::odf_document( xml::sax * Reader, const std::wstring & TempPath) : impl_(new Impl(Reader, TempPath))
{}

odf_document::~odf_document()
{}

odf_read_context & odf_document::odf_context()
{
    return impl_->odf_context();
}

bool odf_document::docx_convert(oox::docx_conversion_context & Context)
{
    return impl_->docx_convert(Context);
}

bool odf_document::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    return impl_->xlsx_convert(Context);
}
bool odf_document::pptx_convert(oox::pptx_conversion_context & Context)
{
    return impl_->pptx_convert(Context);
}

const std::wstring & odf_document::get_folder() const
{
    return impl_->get_folder();
}

const std::wstring & odf_document::get_temp_folder() const
{
    return impl_->get_temp_folder();
}

int odf_document::get_office_mime_type() 
{
	return impl_->get_office_mime_type();
}
bool odf_document::get_encrypted() 
{
	return impl_->get_encrypted();
}
bool odf_document::get_error() 
{
	return impl_->get_error();
}


}
}
