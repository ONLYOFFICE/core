/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#pragma once

namespace DocFileFormat
{
	class ThreeDStyleBooleanProperties
	{
	public:
		ThreeDStyleBooleanProperties(unsigned int op)
		{
			fUsefc3DConstrainRotation	=	GETBIT(op, 0);
			fUsefc3DRotationCenterAuto	=	GETBIT(op, 1);
			fUsefc3DParallel			=	GETBIT(op, 2);
			fUsefc3DKeyHarsh			=	GETBIT(op, 3);
			fUsefc3DFillHarsh			=	GETBIT(op, 4);

			fc3DConstrainRotation		=	GETBIT(op, 16);
			fc3DRotationCenterAuto		=	GETBIT(op, 17);
			fc3DParallel				=	GETBIT(op, 18);
			fc3DKeyHarsh				=	GETBIT(op, 19);
			fc3DFillHarsh				=	GETBIT(op, 20);
		}
		bool fUsefc3DConstrainRotation;
		bool fUsefc3DRotationCenterAuto;
		bool fUsefc3DParallel;
		bool fUsefc3DKeyHarsh;
		bool fUsefc3DFillHarsh;

		bool fc3DConstrainRotation;
		bool fc3DRotationCenterAuto;
		bool fc3DParallel;
		bool fc3DKeyHarsh;
		bool fc3DFillHarsh;
	};
	class ThreeDObjectBooleanProperties
	{
	public:
		ThreeDObjectBooleanProperties(unsigned int op)
		{
			fUsef3D						=	GETBIT(op, 0);
			fUsefc3DMetallic			=	GETBIT(op, 1);
			fUsefc3DUseExtrusionColor	=	GETBIT(op, 2);
			fUsefc3DLightFace			=	GETBIT(op, 3);

			// 12 unused

			f3D							=	GETBIT(op, 16);
			fc3DMetallic				=	GETBIT(op, 17);
			fc3DUseExtrusionColor		=	GETBIT(op, 18);
			fc3DLightFace				=	GETBIT(op, 19);
		}
		bool fUsef3D;
		bool fUsefc3DMetallic;
		bool fUsefc3DUseExtrusionColor;
		bool fUsefc3DLightFace;

		bool f3D;
		bool fc3DMetallic;
		bool fc3DUseExtrusionColor;
		bool fc3DLightFace;
	};
}