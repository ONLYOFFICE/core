
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ShapeType.h"

namespace Odt
{
	namespace Limit
	{
		ShapeType::ShapeType()
		{
			add("rectangle");

			add("bang");
			add("block-arc");
			add("brace-pair");
			add("bracket-pair");
			add("can");
			add("circular-arrow");
			add("cloud-callout");
			add("col-502ad400");
			add("col-60da8460");
			add("cross");
			add("cube");
			add("diamond");
			add("down-arrow");
			add("down-arrow-callout");
			add("ellipse");
			add("flowchart-card");
			add("flowchart-collate");
			add("flowchart-connector");
			add("flowchart-data");
			add("flowchart-decision");
			add("flowchart-delay");
			add("flowchart-direct-access-storage");
			add("flowchart-display");
			add("flowchart-document");
			add("flowchart-extract");
			add("flowchart-internal-storage");
			add("flowchart-manual-input");
			add("flowchart-manual-operation");
			add("flowchart-magnetic-disk");
			add("flowchart-merge");
			add("flowchart-multidocument");
			add("flowchart-off-page-connector");
			add("flowchart-or");
			add("flowchart-predefined-process");
			add("flowchart-preparation");
			add("flowchart-process");
			add("flowchart-punched-tape");
			add("flowchart-sequential-access");
			add("flowchart-sort");
			add("flowchart-stored-data");
			add("flowchart-summing-junction");
			add("flowchart-terminator");
			add("fontwork-arch-up-curve");
			add("fontwork-curve-down");
			add("fontwork-fade-up");
			add("fontwork-plain-text");
			add("fontwork-slant-up");
			add("fontwork-triangle-down");
			add("fontwork-wave");
			add("forbidden");
			add("heart");
			add("hexagon");
			add("horizontal-scroll");
			add("isosceles-triangle");
			add("line-callout-1");
			add("line-callout-2");
			add("line-callout-3");
			add("left-arrow");
			add("left-arrow-callout");
			add("left-brace");
			add("left-bracket");
			add("left-right-arrow");
			add("left-right-arrow-callout");
			add("moon");
			add("mso-spt100");
			add("mso-spt21");
			add("mso-spt89");
			add("non-primitive");
			add("notched-right-arrow");
			add("octagon");
			add("paper");
			add("parallelogram");
			add("pentagon");
			add("pentagon-right");
			add("perspective");
			add("quad-arrow");
			add("quad-arrow-callout");
			add("quad-bevel");
			add("rectangular-callout");
			add("right-arrow");
			add("right-arrow-callout");
			add("right-brace");
			add("right-bracket");
			add("right-triangle");
			add("ring");
			add("round-callout");
			add("round-rectangle");
			add("round-rectangular-callout");
			add("smiley");
			add("star4");
			add("star5");
			add("star8");
			add("star24");
			add("sun");
			add("trapezoid");
			add("up-arrow");
			add("up-down-arrow");
			add("up-down-arrow-callout");
			add("up-arrow-callout");
			add("vertical-scroll");
		}

		const std::string ShapeType::no_find() const
		{
			return "rectangle";
		}
	} // namespace Limit
} // namespace Odt