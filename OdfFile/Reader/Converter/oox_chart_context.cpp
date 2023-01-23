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


#include <vector>
#include <xml/simple_xml_writer.h>

#include "oox_rels.h"
#include "oox_chart_context.h"
#include "mediaitems.h"

namespace cpdoccore {
namespace oox {

oox_chart_context::oox_chart_context(mediaitems_ptr & m, std::wstring name) : mediaitems_(m), no_used_local_tables_(false)
{
}

void oox_chart_context::reset_fill(oox::_oox_fill &f)
{
	if (f.bitmap)
	{
		bool isInternal = true;
		std::wstring ref;
		f.bitmap->rId = mediaitems_->add_or_find(f.bitmap->xlink_href_, typeImage, isInternal, ref, oox::chart_place); 

		rels_.push_back(_rel(isInternal, f.bitmap->rId, ref, typeImage));
	}
}
void oox_chart_context::set_externalData(const std::wstring & href)
{
	bool isInternal = true;
	std::wstring href_out;
	
	externalDataId_ = mediaitems_->add_or_find(href, typeMsObject, isInternal, href_out, oox::chart_place);
	rels_.push_back(_rel(isInternal, externalDataId_, href_out, typeMsObject));
}
void oox_chart_context::set_userShapes(std::pair<std::wstring, std::wstring> &link)
{
	bool isInternal = true;
	userShapesId_ = link.second;
	rels_.push_back(_rel(isInternal, userShapesId_, link.first, typeChartUserShapes));
}

void oox_chart_context::dump_rels(rels & Rels)
{
	for (size_t i = 0; i < rels_.size(); i++)
    {
		_rel & r = rels_[i];

		if (r.type == typeImage)
		{
			Rels.add(relationship(
						r.rid,
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image",
						r.is_internal ? std::wstring(L"../") + r.ref : r.ref,
						(r.is_internal ? L"" : L"External")) 
				);
		}
		else if (r.type == typeHyperlink)
		{
			Rels.add(relationship(
						r.rid,
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink",
						r.ref,
						L"External")
			);
		}
		else if (r.type == typeMsObject)
		{
			Rels.add(relationship(
						r.rid,
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package",
						r.is_internal ? std::wstring(L"../") + r.ref : r.ref,
						(r.is_internal ? L"" : L"External"))
			);
		}
		else if (r.type == typeChartUserShapes)
		{
			Rels.add(relationship(
						r.rid,
						L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartUserShapes",
						r.is_internal ? std::wstring(L"../drawings/") + r.ref : r.ref,
						(r.is_internal ? L"" : L"External"))
			);			
		}
	}
}

void oox_chart_context::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"c:chartSpace")
		{          
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
            CP_XML_ATTR(L"xmlns:c", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
		
			CP_XML_NODE(L"c:lang")
			{
				CP_XML_ATTR(L"val", L"en-US");
			}
			CP_XML_NODE(L"c:roundedCorners")
			{
				CP_XML_ATTR(L"val", 0);
			}
			if (pivot_source_.empty() == false)
			{
				set_cache_only(true);

				CP_XML_NODE(L"c:pivotSource")
				{
				  CP_XML_NODE(L"c:name")
				  {
					  CP_XML_STREAM() << pivot_source_;
				  }
				  CP_XML_NODE(L"c:fmtId")
				  {
					  CP_XML_ATTR(L"val", 0);
				  }
				}
			}
			CP_XML_NODE(L"c:chart")
			{
				if (plot_area_.current_chart_->is3D_)
				{
					plot_area_.oox_serialize_view3D(CP_XML_STREAM());
					//CP_XML_NODE(L"c:view3D");
					CP_XML_NODE(L"c:floor");
					CP_XML_NODE(L"c:backWall");
				}
				title_.oox_serialize	(CP_XML_STREAM());
				plot_area_.oox_serialize(CP_XML_STREAM());
				legend_.oox_serialize	(CP_XML_STREAM());
				
				CP_XML_NODE(L"c:plotVisOnly")
				{
					CP_XML_ATTR(L"val",1);
				}	
				CP_XML_NODE(L"c:dispBlanksAs")
				{
					CP_XML_ATTR(L"val", plot_area_.current_chart_->dispBlanksAs_);
				}
				CP_XML_NODE(L"c:showDLblsOverMax")
				{
					CP_XML_ATTR(L"val",1);
				}
			}
			oox_chart_shape shape;

			shape.set(graphic_properties_, fill_);
			shape.oox_serialize(CP_XML_STREAM());
	
			if (pivot_source_.empty() == false)
			{
				CP_XML_NODE(L"c:extLst")
				{
					CP_XML_NODE(L"c:ext")
					{
						CP_XML_ATTR(L"uri", L"{781A3756-C4B2-4CAC-9D66-4F8BD8637D16}");
						CP_XML_ATTR(L"xmlns:c14", L"http://schemas.microsoft.com/office/drawing/2007/8/2/chart");
						CP_XML_NODE(L"c14:pivotOptions")
						{
							CP_XML_NODE(L"c14:dropZoneFilter")
							{
								CP_XML_ATTR(L"val", 1);
							}		
							CP_XML_NODE(L"c14:dropZoneCategories")
							{
								CP_XML_ATTR(L"val", 1);
							}
							CP_XML_NODE(L"c14:dropZoneData")
							{
								CP_XML_ATTR(L"val", 1);
							}
							CP_XML_NODE(L"c14:dropZoneSeries")
							{
								CP_XML_ATTR(L"val", 1);
							}
							CP_XML_NODE(L"c14:dropZonesVisible")
							{
								CP_XML_ATTR(L"val", 1);
							}
						}
					}
				}
			}

			if (externalDataId_.empty() == false)
			{
				CP_XML_NODE(L"c:externalData")
				{
					CP_XML_ATTR(L"r:id", externalDataId_);
					CP_XML_NODE(L"c:autoUpdate")
					{
						CP_XML_ATTR(L"val", false);
					}
				}
			}
			if (userShapesId_.empty() == false)
			{
				CP_XML_NODE(L"c:userShapes")
				{
					CP_XML_ATTR(L"r:id", userShapesId_);
				}
			}
		}
	}
}
oox_chart_context::~oox_chart_context()
{
}

void oox_chart_context::set_cache_only	(bool val)
{
	for (size_t i = 0 ; i < plot_area_.charts_.size(); i++)
	{
		plot_area_.charts_[i]->set_cache_only(val);
	}
}



}
}

