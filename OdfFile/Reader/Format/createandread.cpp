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
