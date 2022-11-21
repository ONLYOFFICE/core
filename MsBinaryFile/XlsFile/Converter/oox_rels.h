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
