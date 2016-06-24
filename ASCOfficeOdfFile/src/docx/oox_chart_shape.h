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
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>

#include "../odf/chart_objects.h"
#include "oox_drawing_fills.h"
            
namespace cpdoccore {
namespace oox {

void oox_serialize_default_text(std::wostream & _Wostream, std::vector<odf_reader::_property>& properties);


class oox_chart_shape: boost::noncopyable
{
public:
	oox_chart_shape(){}
	~oox_chart_shape(){}

	void set(std::vector<odf_reader::_property> & prop,_oox_fill & fill);
 
    void oox_serialize(std::wostream & _Wostream);
			
private:
	std::vector<odf_reader::_property>	content_;
	_oox_fill							fill_;

    
private:

	//grpFill (Group Fill) §20.1.8.35
	//pattFill (Pattern Fill) §20.1.8.47	
	//noFill (No Fill) §20.1.8.44
	//blipFill (Picture Fill) §20.1.8.14
	//solidFill (Solid Fill) §20.1.8.54
	//gradFill (Gradient Fill) §20.1.8.33
	//ln (Outline) §20.1.2.2.24



    //friend void xlsx_serialize(std::wostream & _Wostream, const oox_chart_shape & legend_);


	//custGeom (Custom Geometry) §20.1.9.8
	//effectDag (Effect Container) §20.1.8.25
	//effectLst (Effect Container) §20.1.8.26
	//extLst (Extension List) §20.1.2.2.15

	//prstGeom (Preset geometry) §20.1.9.18
	//scene3d (3D Scene Properties) §20.1.4.1.26
	//sp3d (Apply 3D shape properties) §20.1.5.12
	//xfrm (2D Transform for Individual Objects) §20.1.7.6
};

}
}
