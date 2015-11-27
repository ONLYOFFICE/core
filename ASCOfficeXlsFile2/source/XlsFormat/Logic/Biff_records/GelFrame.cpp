
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


void GelFrame::writeFields(CFRecord& record)
{
	record << OPT1 << OPT2;
}


void GelFrame::readFields(CFRecord& record)
{
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

