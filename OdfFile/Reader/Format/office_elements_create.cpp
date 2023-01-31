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

#include <iostream>

#include "office_elements_create.h"
#include "skipelement.h"

#include "../../Common/logging.h"

namespace cpdoccore { 
namespace odf_reader {

    
office_element_creator * office_element_creator::instance_ = NULL;

office_element_creator::office_element_creator()
{
}

office_element_ptr office_element_creator::create(const std::wstring & ns, const std::wstring & name, document_context * Context, bool isRoot) const
{
    const std::wstring fullName = ns + (ns.size() > 0 ? L":" : L"") + name;

	MapType::const_iterator i = map_.find( fullName );
	if (i != map_.end())
	{
		office_element_ptr element = (i->second)();

		if (element)
		{
			element->setContext(Context);

			if ( isRoot)
				element->set_root(true);
		}
        return element;
    }
    else
    {
        return office_element_ptr();
    }
}



office_element_creator * office_element_creator::get() 
{
    if (!instance_)
    {
        office_element_creator * temp = new office_element_creator();
        instance_ = temp;                
    }

    return instance_;
}

bool office_element_creator::register_element(const std::wstring &ns, const std::wstring & name, CreateFuncImpl f)
{
    const std::wstring fullName = ns + (ns.size() > 0 ? L":" : L"") + name;

    MapType::iterator i = map_.find( fullName );
    if ( i == map_.end() )
    {
        map_.insert(  MapType::value_type(fullName, f) );        
    }
    else
    {
        // TODO: элемент уже есть, непонятно что делать
        i->second = f;
    }
    return true;
}

void not_applicable_element(const std::wstring & Current, xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	std::wstringstream ss;
    ss << L"[error] : element " << Ns << L":" << Name << " not applicable in current place";
    if (!Current.empty())
        ss << L" ("<< Current << ")";
    ss << L"\n";
    _CP_LOG << ss.str();
    
}

void not_applicable_element(const office_element * CurrentElm, xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    std::wstringstream ss;
    if (CurrentElm)
        ss << CurrentElm->get_ns() << L":" << CurrentElm->get_name();
    not_applicable_element(ss.str(), Reader, Ns, Name);
}

void not_applicable_element(const office_element * CurrentElm, const std::wstring & Ns, const std::wstring & Name)
{
    return not_applicable_element(CurrentElm, NULL, Ns, Name);
}

void not_applicable_element(const std::wstring & Current, const std::wstring & Ns, const std::wstring & Name)
{
    return not_applicable_element(Current, NULL, Ns, Name);    
}

}
}

