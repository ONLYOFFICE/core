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


#include <boost/foreach.hpp>
#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "mediaitems_utils.h"
#include "docx_rels.h"

#include "oox_chart_context.h"

namespace cpdoccore {
namespace oox {

class oox_chart_context::Impl
{
public:
	Impl(){}
    Impl(std::wstring const & name){}
    std::wstring name_;

    std::wstringstream  chartData_;

    std::wstring drawingName_;
    std::wstring drawingId_;
};

oox_chart_context_ptr oox_chart_context::create()
{
    return boost::make_shared<oox_chart_context>();
}
oox_chart_context_ptr oox_chart_context::create(std::wstring const & name)
{
    return boost::make_shared<oox_chart_context>(name);
}
oox_chart_context::oox_chart_context() : impl_(new oox_chart_context::Impl())
{
}
oox_chart_context::oox_chart_context(std::wstring const & name) : impl_(new oox_chart_context::Impl(name))
{
	
}
std::wostream & oox_chart_context::chartData()
{
    return impl_->chartData_;
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
				CP_XML_ATTR(L"val",L"en-US");
			}
			CP_XML_NODE(L"c:chart")
			{
				if (plot_area_.current_chart_->is3D_)
				{
					//CP_XML_NODE(L"c:view3D");
					CP_XML_NODE(L"c:floor");
					CP_XML_NODE(L"c:backWall");
				}
				title_.oox_serialize(CP_XML_STREAM());
				plot_area_.oox_serialize(CP_XML_STREAM());
				legend_.oox_serialize(CP_XML_STREAM());
				
				CP_XML_NODE(L"c:plotVisOnly")
				{
					CP_XML_ATTR(L"val",1);
				}	
				CP_XML_NODE(L"c:dispBlanksAs")
				{
					CP_XML_ATTR(L"val", L"zero");
				}
				CP_XML_NODE(L"c:showDLblsOverMax")
				{
					CP_XML_ATTR(L"val",1);
				}
			}
			oox_chart_shape shape;
			shape.set(graphic_properties_, fill_);
			shape.set(graphic_properties_, fill_);
			shape.oox_serialize(CP_XML_STREAM());
	

		}
	}

}

oox_chart_context::~oox_chart_context()
{
}

bool oox_chart_context::empty() const
{
    return false;//impl_->empty();
}

void oox_chart_context::set_cache_only	(bool val)
{
	for (int i = 0 ; i < plot_area_.charts_.size(); i++)
	{
		plot_area_.charts_[i]->set_cache_only(val);
	}
}



}
}

