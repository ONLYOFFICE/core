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

#include <xml/simple_xml_writer.h>

#include "oox_chart_axis.h"
#include "oox_title.h"
#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {


_CP_PTR(oox_axis_content) oox_axis_content::create(int type, unsigned int id)
{
    return boost::make_shared<oox_axis_content>(type, id);
}

oox_axis_content::oox_axis_content(int type/*,std::wstring name*/, unsigned int id)
{
	if (type == 0)
	{
		id_  = 0;
	}
	else
	{
		id_  = id;
	}
	type_ = type;
}
void oox_axis_content::oox_serialize(std::wostream & _Wostream)
{
	if (id_ < 1 ) return; //not activate, blank axis
 
	CP_XML_WRITER(_Wostream)
    {
		switch(type_)
		{
		case 1:
			CP_XML_NODE(L"c:catAx")
			{	
				oox_serialize_content(CP_XML_STREAM());
				CP_XML_NODE(L"c:auto")
				{
					CP_XML_ATTR(L"val", 1);
				}
				//CP_XML_NODE(L"c:lblAlgn")
				//{
				//	CP_XML_ATTR(L"val", "ctr");
				//}
				//CP_XML_NODE(L"c:lblOffset")
				//{
				//	CP_XML_ATTR(L"val",100);
				//}
				//CP_XML_NODE(L"c:noMultiLvlLbl")
				//{
				//	CP_XML_ATTR(L"val", 1);
				//}
			}
			break;
		case 2:
			CP_XML_NODE(L"c:valAx")
			{	
				oox_serialize_content(CP_XML_STREAM());
				CP_XML_NODE(L"c:crossBetween")
				{
					CP_XML_ATTR(L"val", L"between");
				}
			}
			break;
		case 3:
			CP_XML_NODE(L"c:serAx")
			{	
				oox_serialize_content(CP_XML_STREAM());
			}
			break;
		case 4:
			CP_XML_NODE(L"c:dateAx")
			{	
				oox_serialize_content(CP_XML_STREAM());
				CP_XML_NODE(L"c:auto")
				{
					CP_XML_ATTR(L"val", 1);
				}
			}
			break;
		}
	}
}
void oox_axis_content::oox_serialize_content(std::wostream & _Wostream)
{
	oox_chart_shape			shape;
	_CP_OPT(bool)			boolVal;
	_CP_OPT(std::wstring)	strVal;
	_CP_OPT(double)			doubleVal;
	
	oox_title title;
	title.set_content(content_.title_);//todo нужно задать понармальному layout
  
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:axId")
        {
			CP_XML_ATTR(L"val", id_);
		}
		CP_XML_NODE(L"c:scaling")
		{
			CP_XML_NODE(L"c:orientation")
			{
				odf_reader::GetProperty(content_.properties_, L"reverse-direction",boolVal);
				if ((boolVal == true) && (boolVal.get()==true))
				{
					CP_XML_ATTR(L"val", "maxMin");
				}
				else
				{
					CP_XML_ATTR(L"val", "minMax");//default
				}
			}
			odf_reader::GetProperty(content_.properties_, L"maximum", doubleVal);
			if (doubleVal)
			{
				CP_XML_NODE(L"c:max")
				{
					CP_XML_ATTR(L"val", *doubleVal);
				}
			}
			odf_reader::GetProperty(content_.properties_,L"minimum", doubleVal);
			if (doubleVal)
			{
				CP_XML_NODE(L"c:min")
				{
					CP_XML_ATTR(L"val", *doubleVal);
				}
			}
			odf_reader::GetProperty(content_.properties_,L"logarithmic", boolVal);
			if ((boolVal == true) && (boolVal.get()==true))
			{
				CP_XML_NODE(L"c:logBase")
				{
					CP_XML_ATTR(L"val", "10");
				}
			}
		}
		
		CP_XML_NODE(L"c:delete")//visible item
		{
			odf_reader::GetProperty(content_.properties_, L"visible", boolVal);
			
			if ((boolVal) && (*boolVal == false))	CP_XML_ATTR(L"val", 1);			
			else									CP_XML_ATTR(L"val", 0);
		}
		CP_XML_NODE(L"c:axPos")
        {
			if (content_.dimension_ == L"x")
				CP_XML_ATTR(L"val", L"b");//  "b" | "l" |  "r" |  "t"// == bottom left right top
				
			if (content_.dimension_ == L"y")
				CP_XML_ATTR(L"val", L"l");//  "b" | "l" |  "r" |  "t"// == bottom left right top
		}
		//oox_serialize_ln(_Wostream, content_.graphic_properties_);
		_CP_OPT(std::wstring) strNumFormat, strPercentFormat, strAxisType;
		_CP_OPT(bool) bLinkData;
		_CP_OPT(int) nTypeFormat;

		odf_reader::GetProperty(content_.properties_, L"num_format", strNumFormat);
		odf_reader::GetProperty(content_.properties_, L"percentage_num_format", strPercentFormat);
		odf_reader::GetProperty(content_.properties_, L"data-label-number", nTypeFormat);
		odf_reader::GetProperty(content_.properties_, L"link-data-style-to-source", bLinkData);
		odf_reader::GetProperty(content_.properties_, L"axis-type", strAxisType);

		std::wstring formatCode;
		if ((nTypeFormat) && (*nTypeFormat == 2) && strPercentFormat)
		{
			formatCode = *strPercentFormat;
		}
		else if ((strNumFormat) && (strNumFormat->length() > 1))
		{
			formatCode = *strNumFormat;
		}

		if (formatCode.length() > 1)
		{
			CP_XML_NODE(L"c:numFmt")
			{
				CP_XML_ATTR(L"formatCode", formatCode);
				CP_XML_ATTR(L"sourceLinked", bLinkData.get_value_or(strAxisType ? false : true));
			}
		}

		for (size_t i = 0; i < content_.grids_.size(); i++)
		{
			_oox_fill fill_null;
			shape.set(content_.grids_[i].graphic_properties_, fill_null);
			
			if (content_.grids_[i].type_ == odf_reader::chart::axis::grid::major)
			{
				CP_XML_NODE(L"c:majorGridlines")
				{
					shape.oox_serialize(CP_XML_STREAM());
				}
				odf_reader::GetProperty(content_.properties_, L"display_label", boolVal);
				if ((boolVal == true) && (boolVal.get()==true))
				{
					CP_XML_NODE(L"c:majorTickMark")
					{
						CP_XML_ATTR(L"val", L"cross");//cross (Cross); in (Inside); none (None); out (Outside)
					}
				}
			}
			if (content_.grids_[i].type_ == odf_reader::chart::axis::grid::minor)
			{
				CP_XML_NODE(L"c:minorGridlines")
				{
					shape.oox_serialize(CP_XML_STREAM());
				}
				odf_reader::GetProperty(content_.properties_, L"display_label",boolVal);
				if ((boolVal == true) && (boolVal.get()==true))
				{
					CP_XML_NODE(L"c:minorTickMark")
					{
						CP_XML_ATTR(L"val", L"cross");
					}
				}			
			}
		}
		
		title.oox_serialize(_Wostream);
		
		odf_reader::GetProperty(content_.properties_, L"display_label", boolVal);
		if ((boolVal == true) && (boolVal.get()==true))
		{
			CP_XML_NODE(L"c:tickLblPos")
			{
				CP_XML_ATTR(L"val",L"nextTo");//"high" | "low" |  "nextTo" |  "none"
			}
		}		
		
		shape.set(content_.graphic_properties_, content_.fill_) ;
		shape.oox_serialize(_Wostream);

		oox_serialize_default_text(_Wostream, content_.text_properties_);
		
		for (size_t i = 0; i < cross_id_.size(); i++)
		{
			CP_XML_NODE(L"c:crossAx")
			{
				CP_XML_ATTR(L"val", cross_id_[i]);
			}
		}

//////////////////////////////////////////////////////
		CP_XML_NODE(L"c:crosses")
        {
			CP_XML_ATTR(L"val", L"autoZero"); //""autoZero" | "max" | "min"
		}
	}
}



}
}
