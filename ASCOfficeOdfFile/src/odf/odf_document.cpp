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

#include <cpdoccore/odf/odf_document.h>

#include "odf_document_impl.h"
#include "../progressCallback.h"

namespace cpdoccore { 
namespace odf_reader {

odf_document::odf_document(const std::wstring & Folder, const ProgressCallback* CallBack) : impl_(new Impl(Folder,CallBack))
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

long odf_document::get_office_mime_type() 
{
	return impl_->get_office_mime_type();
}
bool odf_document::get_encrypted() 
{
	return impl_->get_encrypted();
}
bool odf_document::UpdateProgress(long Complete)
{
	if (!impl_) return true;
	return impl_->UpdateProgress(Complete);
}

}
}
