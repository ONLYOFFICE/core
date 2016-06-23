/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include <iosfwd>
#include <iostream>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/attributes.h>

#include "mediaitems.h"
#include "oox_drawing_fills.h"

#include "../../../Common/DocxFormat/Source/Base/Types_32.h"

static const int _odf_to_oox_ShapeType[]=
{ 4,4,4,34,};

static const std::wstring _ooxShapeType[]=
{
	L"rect", //frame
	L"rect", //text box
	L"rect", //shape
	L"ellipse",
	L"ellipse", 
	L"line", 
	L"path",
	L"custGeom",//uses sub-sub type,
	L"polygon", 
};

namespace cpdoccore {
namespace oox {

    struct _hlink_desc
    {
        std::wstring hId;
        std::wstring hRef;
        bool object;
    };
    struct _oox_drawing
    {
        _oox_drawing() : type(mediaitems::typeUnknown), id(0), x(0), y(0), cx(0), cy(0), sub_type(0), inGroup(false), name(L"object")
        {
        }
        mediaitems::Type type;

 		bool	inGroup;
		size_t	id;

        std::wstring name;

        _INT32 x, y;
        _INT32 cx, cy;

        _oox_fill fill;

        int sub_type; //odf
        std::wstring	chartId;

        std::vector<_hlink_desc> hlinks;

        std::vector<odf_reader::_property> additional;

    };
    void oox_serialize_xfrm		(std::wostream & strm, _oox_drawing & val, const std::wstring namespace_ = L"a");
    void oox_serialize_shape	(std::wostream & strm, _oox_drawing & val);
    void oox_serialize_ln		(std::wostream & strm, const std::vector<odf_reader::_property> & val, bool always_draw = false);
    void oox_serialize_hlink	(std::wostream & strm, const std::vector<_hlink_desc> & val);
	void oox_serialize_bodyPr	(std::wostream & strm, _oox_drawing & val, const std::wstring & namespace_ = L"a");
	void oox_serialize_aLst		(std::wostream & strm, const std::vector<odf_reader::_property> & val);
}
}
