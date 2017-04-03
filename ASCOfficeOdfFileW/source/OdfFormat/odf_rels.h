/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#pragma once

#include <vector>
#include <string>

#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/CPSharedPtr.h>

namespace cpdoccore { 
namespace odf_writer {

class relationship /*: public xml::element_impl<relationship>*/
{
public:
    relationship() {}

    relationship(const std::wstring & Type,const std::wstring & Target) :  type_(Type), target_(Target)
    {}

    void serialize(std::wostream & _Wostream);

    const std::wstring & type() const { return type_; }
    const std::wstring & target() const { return target_; }

private:
    std::wstring type_;
    std::wstring target_;
};

class rels;
typedef _CP_PTR(rels) rels_ptr;

/// \class rels
class rels
{
public:
     rels() {}

    void serialize(std::wostream & _Wostream);
    
	std::vector<relationship> & relationships() { return relationship_; }
    void add(relationship const & r);
    bool empty() { return relationship_.empty(); } 
    
private:
    std::vector<relationship> relationship_;

};

}
}
