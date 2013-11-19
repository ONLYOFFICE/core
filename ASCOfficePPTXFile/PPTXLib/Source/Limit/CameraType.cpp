#include "CameraType.h"

namespace PPTX
{
	namespace Limit
	{
		CameraType::CameraType()
		{
			add("isometricBottomDown");
			add("isometricBottomUp");
			add("isometricLeftDown");
			add("isometricLeftUp");
			add("isometricOffAxis1Left");
			add("isometricOffAxis1Right");
			add("isometricOffAxis1Top");
			add("isometricOffAxis2Left");
			add("isometricOffAxis2Right");
			add("isometricOffAxis2Top");
			add("isometricOffAxis3Bottom");
			add("isometricOffAxis3Left");
			add("isometricOffAxis3Right");
			add("isometricOffAxis4Bottom");
			add("isometricOffAxis4Left");
			add("isometricOffAxis4Right");
			add("isometricRightDown");
			add("isometricRightUp");
			add("isometricTopDown");
			add("isometricTopUp");
			add("legacyObliqueBottom");
			add("legacyObliqueBottomLeft");
			add("legacyObliqueBottomRight");
			add("legacyObliqueFront");
			add("legacyObliqueLeft");
			add("legacyObliqueRight");
			add("legacyObliqueTop");
			add("legacyObliqueTopLeft");
			add("legacyObliqueTopRight");
			add("legacyPerspectiveBottom");
			add("legacyPerspectiveBottomLeft");
			add("legacyPerspectiveBottomRight");
			add("legacyPerspectiveFront");
			add("legacyPerspectiveLeft");
			add("legacyPerspectiveRight");
			add("legacyPerspectiveTop");
			add("legacyPerspectiveTopLeft");
			add("legacyPerspectiveTopRight");
			add("obliqueBottom");
			add("obliqueBottomLeft");
			add("obliqueBottomRight");
			add("obliqueLeft");
			add("obliqueRight");
			add("obliqueTop");
			add("obliqueTopLeft");
			add("obliqueTopRight");
			add("orthographicFront");
			add("perspectiveAbove");
			add("perspectiveAboveLeftFacing");
			add("perspectiveAboveRightFacing");
			add("perspectiveBelow");
			add("perspectiveContrastingLeftFacing");
			add("perspectiveContrastingRightFacing");
			add("perspectiveFront");
			add("perspectiveHeroicExtremeLeftFacing");
			add("perspectiveHeroicExtremeRightFacing");
			add("perspectiveHeroicLeftFacing");
			add("perspectiveHeroicRightFacing");
			add("perspectiveLeft");
			add("perspectiveRelaxed");
			add("perspectiveRelaxedModerately");
			add("perspectiveRight");
		}

		const std::string CameraType::no_find() const
		{
			return "legacyObliqueFront";
		}
	} // namespace Limit
} // namespace PPTX