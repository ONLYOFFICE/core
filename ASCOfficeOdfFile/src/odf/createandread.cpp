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


#include "office_elements_create.h"
#include <cpdoccore/xml/sax.h>
#include <sstream>
#include <logging.h>

namespace cpdoccore { 
namespace odf_reader {

class document_context;

bool create_element_and_read(xml::sax * Reader,
                             const ::std::wstring & Ns,
                             const ::std::wstring & Name,
                             office_element_ptr & _Element,
                             document_context * Context,
                             bool isRoot)
{
    if (office_element_ptr elm = office_element_creator::get()->create(Ns, Name, Context, isRoot))
    {
		elm->read_sax( Reader );
        elm->afterReadContent();
        
        if (_Element) // элемент читается повторно
        {
            _CP_LOG << L"[error] : duplicate element (" << Ns << L":" << Name << L")\n";
        }

        _Element = elm;
        return true;
    }
    else
    {
        _CP_LOG << L"[error] : create element failed (" << Ns << L":" << Name << L")\n";

        not_applicable_element(L"[!!!]", Reader, Ns, Name);
    }
    return false;
}

bool create_element_and_read(xml::sax * Reader,
                             const ::std::wstring & Ns,
                             const ::std::wstring & Name,
                             office_element_ptr_array & _Elements,
                             document_context * Context,
                             bool isRoot)
{
    office_element_ptr elm;
    if (create_element_and_read(Reader, Ns, Name, elm, Context, isRoot))
    {
        _Elements.push_back(elm);
        return true;
    }
    return false;
}



}
}
