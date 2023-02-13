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

#include <CPScopedPtr.h>
#include <xml/attributes.h>

#include "oox_rels.h"

namespace cpdoccore {
namespace oox {

class xlsx_drawings;
typedef _CP_PTR(xlsx_drawings) xlsx_drawings_ptr;

struct drawing_elm
{
    drawing_elm(std::wstring const & _filename, std::wstring const & _content, xlsx_drawings_ptr _drawings, _rels_type const & _type)
        : filename(_filename), content(_content), drawings(_drawings), type(_type)
    {}
    
    std::wstring		filename;
    std::wstring		content;
    xlsx_drawings_ptr	drawings;
	_rels_type			type;
};

class _xlsx_drawing;

class xlsx_drawings
{
public:
    xlsx_drawings	(bool inGroup_);
    ~xlsx_drawings	();
    static xlsx_drawings_ptr create(bool inGroup);

    void add ( _xlsx_drawing & d,	bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type, bool sheet_rel = false );
    void add (						bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type, bool sheet_rel, bool vml_rel);
    
	bool empty() const;
	bool vml_empty() const;
   
	void dump_rels_sheet		(rels & Rels);
    void dump_rels_drawing		(rels & Rels);
    void dump_rels_vml_drawing	(rels & Rels);

    void serialize			(std::wostream & _Wostream, const std::wstring & ns, bool local);  
	void serialize_vml		(std::wostream & _Wostream);
	void serialize_objects	(std::wostream & _Wostream);  
	void serialize_controls	(std::wostream & _Wostream);  

	std::vector<_xlsx_drawing> get();
private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
    
};

}
}
