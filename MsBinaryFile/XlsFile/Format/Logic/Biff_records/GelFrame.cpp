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

#include "GelFrame.h"

namespace XLS
{

GelFrame::GelFrame()
{
}


GelFrame::~GelFrame()
{
}


BaseObjectPtr GelFrame::clone()
{
	return BaseObjectPtr(new GelFrame(*this));
}

void GelFrame::readFields(CFRecord& record)
{
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	while (!recs.empty())
	{
		record.appendRawData(recs.front());
		recs.pop_front();
	}
	record >> OPT1 >> OPT2;
}

void GelFrame::concatinate(GelFrame * addit)
{
	if (addit == NULL) return;

	if (addit->OPT1.fopt.options_count > 0)
	{
		OPT1.fopt.options_count += addit->OPT1.fopt.options_count;

		OPT1.fopt.GroupShape_props.insert	(OPT1.fopt.GroupShape_props.end()	, addit->OPT1.fopt.GroupShape_props.begin()	, addit->OPT1.fopt.GroupShape_props.end());
		OPT1.fopt.Shape_props.insert		(OPT1.fopt.Shape_props.end()		, addit->OPT1.fopt.Shape_props.begin()		, addit->OPT1.fopt.Shape_props.end());
		OPT1.fopt.Blip_props.insert			(OPT1.fopt.Blip_props.end()			, addit->OPT1.fopt.Blip_props.begin()		, addit->OPT1.fopt.Blip_props.end());
		OPT1.fopt.Text_props.insert			(OPT1.fopt.Text_props.end()			, addit->OPT1.fopt.Text_props.begin()		, addit->OPT1.fopt.Text_props.end());
		OPT1.fopt.Geometry_props.insert		(OPT1.fopt.Geometry_props.end()		, addit->OPT1.fopt.Geometry_props.begin()	, addit->OPT1.fopt.Geometry_props.end());
		OPT1.fopt.GeometryText_props.insert	(OPT1.fopt.GeometryText_props.end()	, addit->OPT1.fopt.GeometryText_props.begin(), addit->OPT1.fopt.GeometryText_props.end());
		OPT1.fopt.FillStyle_props.insert	(OPT1.fopt.FillStyle_props.end()	, addit->OPT1.fopt.FillStyle_props.begin()	, addit->OPT1.fopt.FillStyle_props.end());
		OPT1.fopt.LineStyle_props.insert	(OPT1.fopt.LineStyle_props.end()	, addit->OPT1.fopt.LineStyle_props.begin()	, addit->OPT1.fopt.LineStyle_props.end());
		OPT1.fopt.Shadow_props.insert		(OPT1.fopt.Shadow_props.end()		, addit->OPT1.fopt.Shadow_props.begin()		, addit->OPT1.fopt.Shadow_props.end());
		OPT1.fopt.Transform_props.insert	(OPT1.fopt.Transform_props.end()	, addit->OPT1.fopt.Transform_props.begin()	, addit->OPT1.fopt.Transform_props.end());	
	}

	if (addit->OPT2.fopt.options_count > 0)
	{
		OPT2.fopt.GroupShape_props.insert	(OPT2.fopt.GroupShape_props.end()	, addit->OPT2.fopt.GroupShape_props.begin()	, addit->OPT2.fopt.GroupShape_props.end());
		OPT2.fopt.Shape_props.insert		(OPT2.fopt.Shape_props.end()		, addit->OPT2.fopt.Shape_props.begin()		, addit->OPT2.fopt.Shape_props.end());
		OPT2.fopt.Blip_props.insert			(OPT2.fopt.Blip_props.end()			, addit->OPT2.fopt.Blip_props.begin()		, addit->OPT2.fopt.Blip_props.end());
		OPT2.fopt.Text_props.insert			(OPT2.fopt.Text_props.end()			, addit->OPT2.fopt.Text_props.begin()		, addit->OPT2.fopt.Text_props.end());
		OPT2.fopt.Geometry_props.insert		(OPT2.fopt.Geometry_props.end()		, addit->OPT2.fopt.Geometry_props.begin()	, addit->OPT2.fopt.Geometry_props.end());
		OPT2.fopt.GeometryText_props.insert	(OPT2.fopt.GeometryText_props.end()	, addit->OPT2.fopt.GeometryText_props.begin(), addit->OPT2.fopt.GeometryText_props.end());
		OPT2.fopt.FillStyle_props.insert	(OPT2.fopt.FillStyle_props.end()	, addit->OPT2.fopt.FillStyle_props.begin()	, addit->OPT2.fopt.FillStyle_props.end());
		OPT2.fopt.LineStyle_props.insert	(OPT2.fopt.LineStyle_props.end()	, addit->OPT2.fopt.LineStyle_props.begin()	, addit->OPT2.fopt.LineStyle_props.end());
		OPT2.fopt.Shadow_props.insert		(OPT2.fopt.Shadow_props.end()		, addit->OPT2.fopt.Shadow_props.begin()		, addit->OPT2.fopt.Shadow_props.end());
		OPT2.fopt.Transform_props.insert	(OPT2.fopt.Transform_props.end()	, addit->OPT2.fopt.Transform_props.begin()	, addit->OPT2.fopt.Transform_props.end());	
	}
}

} // namespace XLS

