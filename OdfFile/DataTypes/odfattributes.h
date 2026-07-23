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

#include "../Common/xml/attributes.h"
#include "../Common/CPOptional.h"
#include "../Common/errors.h"
#include "../Common/logging.h"

namespace cpdoccore {
namespace xml {

template <class V>
class attributes_get_val_impl_parse
{
public:
    typedef typename optional<V>::Type optional_v_type;

    static optional_v_type get_val(attributes & attr, const std::wstring & QualifiedName)
    {
        try 
        { 
            if (attributes::value_type val = attr.get(QualifiedName))
            {
                return optional_v_type( V::parse(*val) );
            }
        }
        catch(odf_types::errors::invalid_attribute &)
        {         
            attributes::value_type val = attr.get(QualifiedName);

            _CP_LOG << L"[error] : invalud attribute value [" << QualifiedName << L":" << ( (val) ? (*val) : (L"?")) << L"]\n";
        }
        return optional_v_type();
    }
};

#define APPLY_PARSE_XML_ATTRIBUTES(V) \
namespace xml { \
template<> \
class attributes_get_val_impl<V >\
{\
public:\
    typedef optional<V>::Type optional_v_type; \
    static optional_v_type get_val(attributes & attr, const std::wstring & QualifiedName)\
    {\
        return attributes_get_val_impl_parse<V>::get_val(attr, QualifiedName);\
    }\
};\
}

}
}
