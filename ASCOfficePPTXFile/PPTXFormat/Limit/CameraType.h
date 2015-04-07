#pragma once
#ifndef PPTX_LIMIT_CAMERATYPE_INCLUDE_H_
#define PPTX_LIMIT_CAMERATYPE_INCLUDE_H_

#include "BaseLimit.h"


namespace PPTX
{
	namespace Limit
	{
		class CameraType : public BaseLimit
		{
		public:
			CameraType()
			{
				m_strValue = _T("legacyObliqueFront");
			}

			_USE_STRING_OPERATOR
				
            virtual void set(const CString& strValue)
			{
				if ((_T("isometricBottomDown") == strValue) ||
					(_T("isometricBottomUp") == strValue) ||
					(_T("isometricLeftDown") == strValue) ||
					(_T("isometricLeftUp") == strValue) ||
					(_T("isometricOffAxis1Left") == strValue) ||
					(_T("isometricOffAxis1Right") == strValue) ||
					(_T("isometricOffAxis1Top") == strValue) ||
					(_T("isometricOffAxis2Left") == strValue) ||
					(_T("isometricOffAxis2Right") == strValue) ||
					(_T("isometricOffAxis2Top") == strValue) ||
					(_T("isometricOffAxis3Bottom") == strValue) ||
					(_T("isometricOffAxis3Left") == strValue) ||
					(_T("isometricOffAxis3Right") == strValue) ||
					(_T("isometricOffAxis4Bottom") == strValue) ||
					(_T("isometricOffAxis4Left") == strValue) ||
					(_T("isometricOffAxis4Right") == strValue) ||
					(_T("isometricRightDown") == strValue) ||
					(_T("isometricRightUp") == strValue) ||
					(_T("isometricTopDown") == strValue) ||
					(_T("isometricTopUp") == strValue) ||
					(_T("legacyObliqueBottom") == strValue) ||
					(_T("legacyObliqueBottomLeft") == strValue) ||
					(_T("legacyObliqueBottomRight") == strValue) ||
					(_T("legacyObliqueFront") == strValue) ||
					(_T("legacyObliqueLeft") == strValue) ||
					(_T("legacyObliqueRight") == strValue) ||
					(_T("legacyObliqueTop") == strValue) ||
					(_T("legacyObliqueTopLeft") == strValue) ||
					(_T("legacyObliqueTopRight") == strValue) ||
					(_T("legacyPerspectiveBottom") == strValue) ||
					(_T("legacyPerspectiveBottomLeft") == strValue) ||
					(_T("legacyPerspectiveBottomRight") == strValue) ||
					(_T("legacyPerspectiveFront") == strValue) ||
					(_T("legacyPerspectiveLeft") == strValue) ||
					(_T("legacyPerspectiveRight") == strValue) ||
					(_T("legacyPerspectiveTop") == strValue) ||
					(_T("legacyPerspectiveTopLeft") == strValue) ||
					(_T("legacyPerspectiveTopRight") == strValue) ||
					(_T("obliqueBottom") == strValue) ||
					(_T("obliqueBottomLeft") == strValue) ||
					(_T("obliqueBottomRight") == strValue) ||
					(_T("obliqueLeft") == strValue) ||
					(_T("obliqueRight") == strValue) ||
					(_T("obliqueTop") == strValue) ||
					(_T("obliqueTopLeft") == strValue) ||
					(_T("obliqueTopRight") == strValue) ||
					(_T("orthographicFront") == strValue) ||
					(_T("perspectiveAbove") == strValue) ||
					(_T("perspectiveAboveLeftFacing") == strValue) ||
					(_T("perspectiveAboveRightFacing") == strValue) ||
					(_T("perspectiveBelow") == strValue) ||
					(_T("perspectiveContrastingLeftFacing") == strValue) ||
					(_T("perspectiveContrastingRightFacing") == strValue) ||
					(_T("perspectiveFront") == strValue) ||
					(_T("perspectiveHeroicExtremeLeftFacing") == strValue) ||
					(_T("perspectiveHeroicExtremeRightFacing") == strValue) ||
					(_T("perspectiveHeroicLeftFacing") == strValue) ||
					(_T("perspectiveHeroicRightFacing") == strValue) ||
					(_T("perspectiveLeft") == strValue) ||
					(_T("perspectiveRelaxed") == strValue) ||
					(_T("perspectiveRelaxedModerately") == strValue) ||
					(_T("perspectiveRight") == strValue))
				{
					m_strValue = strValue;
				}
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_CAMERATYPE_INCLUDE_H_
