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
#ifndef _CPDOCCORE_DOCX_HEADERS_FOOTERS_H_
#define _CPDOCCORE_DOCX_HEADERS_FOOTERS_H_

#include <string>
#include <boost/unordered_map.hpp>

#include "oox_rels.h"
namespace cpdoccore { 

namespace odf_reader 
{
	class header_footer_impl;
}

namespace oox {

//class rels;

class headers_footers
{
public:
    headers_footers() : size_(0) {}

    enum Type { header, footer, headerLeft, footerLeft, headerFirst, footerFirst, none };
    std::wstring add(const std::wstring & StyleName, const std::wstring & Content, Type type,rels &_rels);
   
	struct instance 
    {
        instance(const std::wstring & Id,
            const std::wstring & Content,
            Type _Type, const std::wstring & Name) : id_(Id), content_(Content), type_(_Type), name_(Name){}
        std::wstring id_;
        std::wstring content_;
        Type type_;
        std::wstring name_;
		rels rels_;
    };

    void dump_rels(rels & Rels) const;
    bool write_sectPr(const std::wstring & StyleName, bool next_page, std::wostream & _Wostream);

    typedef boost::shared_ptr<instance> instance_ptr;
    typedef std::vector<instance_ptr> instances_array;
    typedef boost::unordered_map<std::wstring, instances_array> instances_map; 
    
	const instances_map & instances() const { return instances_; }

private:
	std::wstring last_write_style_;

	static std::wstring create_id(size_t i);
	static std::wstring create_name(size_t i, Type _Type);
	instances_map instances_;
	size_t size_;
   
};

}
}


#endif

