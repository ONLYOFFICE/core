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
#include <sstream>

#include "office_elements_create.h"
#include <xml/sax.h>
#include "../../Common/logging.h"

namespace cpdoccore { 
namespace odf_reader {



bool create_element_and_read(xml::sax * Reader,
							 const std::wstring & Ns,
							 const std::wstring & Name,
							 office_element_ptr & _Element,
							 document_context * Context,
							 bool isRoot)
{
    if (office_element_ptr elm = office_element_creator::get()->create(Ns, Name, Context, isRoot))
    {
		elm->afterCreate();
			elm->read_sax( Reader);
        elm->afterReadContent();
        
        if (_Element) // element is being read repeatedly
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
                             const std::wstring & Ns,
                             const std::wstring & Name,
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
//----------------------------------------------------------------------------------------------------

void office_element::afterCreate()
{
	if (!context_) return;

	ElementType type_ = this->get_type();

	context_->levels.push_back(this);
}
void office_element::afterReadContent()
{
	if (!context_) return;

	ElementType type_ = this->get_type();

	std::wstring style = element_style_name.get_value_or(L"");

	if ((type_ == typeTextH || type_ == typeTextP) && element_style_name)
	{
		if ((context_->last_paragraph) && (!context_->last_paragraph->next_element_style_name))
		{
			context_->last_paragraph->next_element_style_name = element_style_name;
		}

		context_->last_paragraph = this;
	}

	if (false == context_->levels.empty())
		context_->levels.pop_back();
}

}
}
