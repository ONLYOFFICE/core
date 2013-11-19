#include "DgmBuildType.h"

namespace PPTX
{
	namespace Limit
	{
		DgmBuildType::DgmBuildType()
		{
			add("allAtOnce");
			add("breadthByLvl");
			add("breadthByNode");
			add("ccw");
			add("ccwIn");
			add("ccwOut");
			add("cust");
			add("cw");
			add("cwIn");
			add("cwOut");
			add("depthByBranch");
			add("depthByNode");
			add("down");
			add("inByRing");
			add("outByRing");
			add("up");
			add("whole");
		}

		const std::string DgmBuildType::no_find() const
		{
			return "whole";
		}
	} // namespace Limit
} // namespace PPTX