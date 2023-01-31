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

#include "CameraType.h"

namespace PPTX
{
	namespace Limit
	{		
		CameraType::CameraType()
		{
			m_strValue = L"legacyObliqueFront";
		}
		void CameraType::set(const std::wstring& strValue)
		{
			m_strValue = strValue;
		}
		BYTE CameraType::GetBYTECode() const
		{
			if (m_strValue == L"isometricBottomDown")		return 0;
			if (m_strValue == L"isometricBottomUp")			return 1;
			if (m_strValue == L"isometricLeftDown")			return 2;
			if (m_strValue == L"isometricLeftUp")			return 3;
			if (m_strValue == L"isometricOffAxis1Left")		return 4;
			if (m_strValue == L"isometricOffAxis1Right")	return 5;
			if (m_strValue == L"isometricOffAxis1Top")		return 6;
			if (m_strValue == L"isometricOffAxis2Left")		return 7;
			if (m_strValue == L"isometricOffAxis2Right")	return 8;
			if (m_strValue == L"isometricOffAxis2Top")		return 9;
			if (m_strValue == L"isometricOffAxis3Bottom")	return 10;
			if (m_strValue == L"isometricOffAxis3Left")		return 11;
			if (m_strValue == L"isometricOffAxis3Right")	return 12;
			if (m_strValue == L"isometricOffAxis4Bottom")	return 13;
			if (m_strValue == L"isometricOffAxis4Left")		return 14;
			if (m_strValue == L"isometricOffAxis4Right")	return 15;
			if (m_strValue == L"isometricRightDown")		return 16;
			if (m_strValue == L"isometricRightUp")			return 17;
			if (m_strValue == L"isometricTopDown")			return 18;
			if (m_strValue == L"isometricTopUp")			return 19;
			if (m_strValue == L"legacyObliqueBottom")		return 20;
			if (m_strValue == L"legacyObliqueBottomLeft")	return 21;
			if (m_strValue == L"legacyObliqueBottomRight")	return 22;
			if (m_strValue == L"legacyObliqueFront")		return 23;
			if (m_strValue == L"legacyObliqueLeft")			return 24;
			if (m_strValue == L"legacyObliqueRight")		return 25;
			if (m_strValue == L"legacyObliqueTop")			return 26;
			if (m_strValue == L"legacyObliqueTopLeft")		return 27;
			if (m_strValue == L"legacyObliqueTopRight")		return 28;
			if (m_strValue == L"legacyPerspectiveBottom")	return 29;
			if (m_strValue == L"legacyPerspectiveBottomLeft")	return 30;
			if (m_strValue == L"legacyPerspectiveBottomRight")	return 31;
			if (m_strValue == L"legacyPerspectiveFront")	return 32;
			if (m_strValue == L"legacyPerspectiveLeft")		return 33;
			if (m_strValue == L"legacyPerspectiveRight")	return 34;
			if (m_strValue == L"legacyPerspectiveTop")		return 35;
			if (m_strValue == L"legacyPerspectiveTopLeft")	return 36;
			if (m_strValue == L"legacyPerspectiveTopRight")	return 37;
			if (m_strValue == L"obliqueBottom")				return 38;
			if (m_strValue == L"obliqueBottomLeft")			return 39;
			if (m_strValue == L"obliqueBottomRight")		return 40;
			if (m_strValue == L"obliqueLeft")				return 41;
			if (m_strValue == L"obliqueRight")				return 42;
			if (m_strValue == L"obliqueTop")				return 43;
			if (m_strValue == L"obliqueTopLeft")			return 44;
			if (m_strValue == L"obliqueTopRight")			return 45;
			if (m_strValue == L"orthographicFront")					return 46;
			if (m_strValue == L"perspectiveAbove")					return 47;
			if (m_strValue == L"perspectiveAboveLeftFacing")		return 48;
			if (m_strValue == L"perspectiveAboveRightFacing")		return 49;
			if (m_strValue == L"perspectiveBelow")					return 50;
			if (m_strValue == L"perspectiveContrastingLeftFacing")	return 51;
			if (m_strValue == L"perspectiveContrastingRightFacing")	return 52;
			if (m_strValue == L"perspectiveFront")					return 53;
			if (m_strValue == L"perspectiveHeroicExtremeLeftFacing")return 54;
			if (m_strValue == L"perspectiveHeroicExtremeRightFacing")return 55;
			if (m_strValue == L"perspectiveHeroicLeftFacing")	return 56;
			if (m_strValue == L"perspectiveHeroicRightFacing")	return 57;
			if (m_strValue == L"perspectiveLeft")				return 58;
			if (m_strValue == L"perspectiveRelaxed")			return 59;
			if (m_strValue == L"perspectiveRelaxedModerately")	return 60;
			if (m_strValue == L"perspectiveRight")				return 61;
			return 0;
		}
		void CameraType::SetBYTECode(const BYTE& val)
		{
			switch (val)
			{
				case 0:		m_strValue = L"isometricBottomDown";	break;
				case 1:		m_strValue = L"isometricBottomUp";		break;
				case 2:		m_strValue = L"isometricLeftDown";		break;
				case 3:		m_strValue = L"isometricLeftUp";		break;
				case 4:		m_strValue = L"isometricOffAxis1Left";	break;
				case 5:		m_strValue = L"isometricOffAxis1Right";	break;
				case 6:		m_strValue = L"isometricOffAxis1Top";	break;
				case 7:		m_strValue = L"isometricOffAxis2Left";	break;
				case 8:		m_strValue = L"isometricOffAxis2Right";	break;
				case 9:		m_strValue = L"isometricOffAxis2Top";	break;
				case 10:	m_strValue = L"isometricOffAxis3Bottom";break;
				case 11:	m_strValue = L"isometricOffAxis3Left";	break;
				case 12:	m_strValue = L"isometricOffAxis3Right";	break;
				case 13:	m_strValue = L"isometricOffAxis4Bottom";break;
				case 14:	m_strValue = L"isometricOffAxis4Left";	break;
				case 15:	m_strValue = L"isometricOffAxis4Right";	break;
				case 16:	m_strValue = L"isometricRightDown";		break;
				case 17:	m_strValue = L"isometricRightUp";		break;
				case 18:	m_strValue = L"isometricTopDown";		break;
				case 19:	m_strValue = L"isometricTopUp";			break;
				case 20:	m_strValue = L"legacyObliqueBottom";	break;
				case 21:	m_strValue = L"legacyObliqueBottomLeft";break;
				case 22:	m_strValue = L"legacyObliqueBottomRight";break;
				case 23:	m_strValue = L"legacyObliqueFront";		break;
				case 24:	m_strValue = L"legacyObliqueLeft";		break;
				case 25:	m_strValue = L"legacyObliqueRight";		break;
				case 26:	m_strValue = L"legacyObliqueTop";		break;
				case 27:	m_strValue = L"legacyObliqueTopLeft";	break;
				case 28:	m_strValue = L"legacyObliqueTopRight";	break;
				case 29:	m_strValue = L"legacyPerspectiveBottom";break;
				case 30:	m_strValue = L"legacyPerspectiveBottomLeft";break;
				case 31:	m_strValue = L"legacyPerspectiveBottomRight";break;
				case 32:	m_strValue = L"legacyPerspectiveFront";		break;
				case 33:	m_strValue = L"legacyPerspectiveLeft";		break;
				case 34:	m_strValue = L"legacyPerspectiveRight";		break;
				case 35:	m_strValue = L"legacyPerspectiveTop";		break;
				case 36:	m_strValue = L"legacyPerspectiveTopLeft";	break;
				case 37:	m_strValue = L"legacyPerspectiveTopRight";	break;
				case 38:	m_strValue = L"obliqueBottom";			break;
				case 39:	m_strValue = L"obliqueBottomLeft";		break;
				case 40:	m_strValue = L"obliqueBottomRight";		break;
				case 41:	m_strValue = L"obliqueLeft";			break;
				case 42:	m_strValue = L"obliqueRight";			break;
				case 43:	m_strValue = L"obliqueTop";				break;
				case 44:	m_strValue = L"obliqueTopLeft";			break;
				case 45:	m_strValue = L"obliqueTopRight";		break;
				case 46:	m_strValue = L"orthographicFront";		break;
				case 47:	m_strValue = L"perspectiveAbove";		break;
				case 48:	m_strValue = L"perspectiveAboveLeftFacing";	break;
				case 49:	m_strValue = L"perspectiveAboveRightFacing";break;
				case 50:	m_strValue = L"perspectiveBelow";			break;
				case 51:	m_strValue = L"perspectiveContrastingLeftFacing";	break;
				case 52:	m_strValue = L"perspectiveContrastingRightFacing";	break;
				case 53:	m_strValue = L"perspectiveFront";					break;
				case 54:	m_strValue = L"perspectiveHeroicExtremeLeftFacing";	break;
				case 55:	m_strValue = L"perspectiveHeroicExtremeRightFacing";break;
				case 56:	m_strValue = L"perspectiveHeroicLeftFacing";	break;
				case 57:	m_strValue = L"perspectiveHeroicRightFacing";	break;
				case 58:	m_strValue = L"perspectiveLeft";				break;
				case 59:	m_strValue = L"perspectiveRelaxed";				break;
				case 60:	m_strValue = L"perspectiveRelaxedModerately";	break;
				case 61:	m_strValue = L"perspectiveRight";				break;
				default:	m_strValue = L"isometricBottomDown";

			}
		}
	} // namespace Limit
} // namespace PPTX
