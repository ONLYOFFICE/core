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

#include "headers_footers.h"
#include <boost/lexical_cast.hpp>

namespace cpdoccore { 
namespace oox {

std::wstring headers_footers::create_id(size_t i)
{
    return std::wstring(L"rHFId") + std::to_wstring(i); 
}

std::wstring headers_footers::create_name(size_t i, headers_footers::Type _Type)
{
    return ((_Type == header || _Type == headerLeft || _Type == headerFirst) ?  std::wstring(L"header") : std::wstring(L"footer") )
        + std::to_wstring(i) + L".xml";
}

std::wstring headers_footers::add(const std::wstring & StyleName,
                                  const std::wstring & Content,
                                  Type type,
								  rels &_rels
                                  )
{
	std::wstring id, name;
	if (type != headers_footers::none)
	{
		size_++;
		
		id = create_id(size_);
		name = create_name(size_, type);
	}
    instance_ptr inst = instance_ptr( new instance(id, Content, type, name) );
  
	std::vector<relationship> & rels = _rels.relationships();
	for (size_t i = 0; i < rels.size(); i++)
    {
		inst->rels_.add(rels[i]);
	}
    instances_[StyleName].push_back(inst);
	return id;
}

namespace 
{

std::wstring get_rel_type(headers_footers::Type _Type)
{
    if (_Type == headers_footers::header || _Type == headers_footers::headerLeft || _Type == headers_footers::headerFirst)
        return std::wstring(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/header");
    else
        return std::wstring(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer");
}
    
}

void headers_footers::dump_rels(rels & Rels) const//внешние релсы
{
	for (boost::unordered_map<std::wstring, instances_array>::const_iterator it = instances_.begin(); it != instances_.end(); ++it)
    {
		for (size_t i = 0; i < it->second.size(); i++)
        {
			if (it->second[i]->type_ == none) continue;
            Rels.add( relationship( it->second[i]->id_,  get_rel_type(it->second[i]->type_),  it->second[i]->name_, L"" ) );
        }
    }        
}

bool headers_footers::write_sectPr(const std::wstring & StyleName, bool next_page/*not used*/, std::wostream & _Wostream)
{
    if (!instances_.count(StyleName))	return false;
	if (last_write_style_ == StyleName)	return true;

	last_write_style_ = StyleName;

	bool first	=	false, left	=	false;
	bool res	=	false;

	instances_array & pFind = instances_.at(StyleName);
   
	for (size_t i = 0; i < pFind.size(); i++)
    {
		instance_ptr & inst = pFind[i];

        std::wstring type = L"default";
		
		if ( inst->type_ == headerFirst || inst->type_ == footerFirst )
		{
			type = L"first";
			first = true;
		}
		else if ( inst->type_ == footerLeft || inst->type_ == headerLeft )
		{
			type = L"even";
			left = true;
		}

        std::wstring name;  
		if (inst->type_ == footer || inst->type_ == footerLeft  || inst->type_ == footerFirst ) 
			name =L"w:footerReference";
		else if (inst->type_ == header || inst->type_ == headerLeft  || inst->type_ == headerFirst ) 
			name =L"w:headerReference";

		if (!name.empty())
		{
			_Wostream << L"<" << name << L" r:id=\"" << inst->id_ << L"\" w:type=\"" << type << "\" />";
		}
		res=true;
	}
	
	if (first)
	{
		_Wostream << L"<w:titlePg/>";
	}
	return res;
}

}
}
