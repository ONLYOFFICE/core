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

#include <string>
#include <vector>
#include <iosfwd>

#include "../../Common/Utils/OptPtr.h"

namespace  oox
{
class relationship
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

public:
    relationship() {}

    relationship(const std::wstring & Id,
        const std::wstring & Type,
        const std::wstring & Target,
        const std::wstring & TargetMode = L"") : id_(Id), type_(Type), target_(Target), target_mode_(TargetMode) 
    {}

	relationship(const std::wstring & Id, int Type, const std::wstring & Target, bool lExternal) ;

	virtual std::wostream & xml_to_stream(::std::wostream & _Wostream) const;

public:
    const std::wstring & id() const { return id_; }
    const std::wstring & type() const { return type_; }
    const std::wstring & target() const { return target_; }
    const std::wstring & target_mode() const { return target_mode_; }

private:
    std::wstring id_;
    std::wstring type_;
    std::wstring target_;
    std::wstring target_mode_;
};

class rels;
typedef _CP_PTR(rels) rels_ptr;

/// \class rels
class rels 
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

public:
    virtual ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;
    std::vector<relationship> & relationships() { return relationship_; }
    void add(relationship const & r);
    bool empty() { return relationship_.empty(); } 
    
private:
    std::vector<relationship> relationship_;

};
}
