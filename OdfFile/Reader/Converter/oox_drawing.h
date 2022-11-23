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

#include <iosfwd>
#include <iostream>

#include "mediaitems.h"
#include "oox_drawing_fills.h"

#include "../Format/svg_parser.h"

#include "../../../OOXML/Base/Base.h"

namespace cpdoccore {
namespace oox {

    struct _hlink_desc
    {
        std::wstring hId;
        std::wstring hRef;
   };
    struct _action_desc
    {
		_action_desc() : enabled(false), highlightClick(false) {}
		void clear()
		{
			enabled			= false;
			highlightClick	= false;
			
			hSoundId.clear();
			hSoundRef.clear();

			hId.clear(); 
			hRef.clear();

			action.clear();
		}
		bool			enabled;
        std::wstring	action;
        
		std::wstring	hSoundId;
		std::wstring	hSoundRef;

		std::wstring	hId;
        std::wstring	hRef;

		_rels_type		typeRels;
		bool			highlightClick;
    };
	class _oox_drawing
    {
	public:
        _oox_drawing() : type(typeUnknown), id(0), x(0), y(0), cx(0), cy(0), sub_type(0), hidden(false), inGroup(false), lined(false), connector(false), name(L"object"), extExternal(false)
        {
        }
        _rels_type		type;

 		bool			inGroup;
		size_t			id;
		bool			lined;
		bool			connector;
		bool			hidden;

        std::wstring	name;
        int				sub_type; //odf

        _INT32			x, y;
        _INT32			cx, cy;

        _oox_fill		fill;

        std::wstring	objectId;
		std::wstring	objectProgId; 

		std::wstring	extId;
		bool			extExternal;

		_action_desc				action;
		std::vector<_hlink_desc>	hlinks;

        std::vector<odf_reader::_property>	additional;

		virtual void serialize	(std::wostream & strm) = 0;

		void serialize_shape	(std::wostream & strm);
		void serialize_xfrm		(std::wostream & strm, const std::wstring & namespace_ = L"a", bool always_position = false);
		void serialize_bodyPr	(std::wostream & strm, const std::wstring & namespace_ = L"a");
    };   
	typedef _CP_PTR(_oox_drawing) oox_drawing_ptr;

	void oox_serialize_ln		(std::wostream & strm, const std::vector<odf_reader::_property> & val, bool always_draw = false, const std::wstring &ns = L"a");
	void oox_serialize_aLst		(std::wostream & strm, const std::vector<odf_reader::_property> & val, const std::wstring & shapeGeomPreset, const std::wstring &ns = L"a");
    void oox_serialize_action	(std::wostream & strm, const _action_desc & val);
	void oox_serialize_effects	(std::wostream & strm, const std::vector<odf_reader::_property> & val);
    
	void vml_serialize_ln		(std::wostream & strm, const std::vector<odf_reader::_property> & val);
	void vml_serialize_text		(std::wostream & strm, const std::vector<odf_reader::_property> & val);
}
}

namespace svg_path
{
	void oox_serialize	(std::wostream & strm, _point & val);
	void oox_serialize	(std::wostream & strm, _polyline & val);
	void oox_serialize	(std::wostream & strm, std::vector<_polyline> & path);
}