/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

