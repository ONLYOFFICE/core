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

#include "skipelement.h"
#include "../../Common/logging.h"
#include <xml/attributes.h>

namespace cpdoccore { 
namespace odf_reader {

_skip_element _skip_element::skip_element_;

const wchar_t * _skip_element::ns = L"";
const wchar_t * _skip_element::name = L"";

void _skip_element::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    Attributes->reset_check();
    return;
}

void _skip_element::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    _CP_LOG << L"[error] : skip element " << Ns << L":" << Name << "\n";
    
    if (Reader)
        skip_element_.read_sax(Reader);        
}

void _skip_element::add_text(const std::wstring & Text)
{
     _CP_LOG << L"[error] : skip element text\n";
}

}
}
