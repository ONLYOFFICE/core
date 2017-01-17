/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "../../Common/FormatUtils.h"

namespace DocFileFormat
{
	// 2.3.20.1 Protection Boolean Properties 	-	[MS-ODRAW] — v20130726

	class ProtectionBooleanProperties
	{
	public:
		ProtectionBooleanProperties(unsigned int op)
		{
			fLockAgainstGrouping		=	FormatUtils::BitmaskToBool(op,0x1);
			fLockAdjustHandles			=	FormatUtils::BitmaskToBool(op,0x2);
			fLockText					=	FormatUtils::BitmaskToBool(op,0x4);
			fLockVertices				=	FormatUtils::BitmaskToBool(op,0x8);

			fLockCropping				=	FormatUtils::BitmaskToBool(op,0x10);
			fLockAgainstSelect			=	FormatUtils::BitmaskToBool(op,0x20);
			fLockPosition				=	FormatUtils::BitmaskToBool(op,0x30);
			fLockAspectRatio			=	FormatUtils::BitmaskToBool(op,0x40);

			fLockRotation				=	FormatUtils::BitmaskToBool(op,0x100);
			fLockAgainstUngrouping		=	FormatUtils::BitmaskToBool(op,0x200);

			//unused 0x400 0x800 0x1000 0x2000 0x4000 0x8000

			fUsefLockAgainstGrouping	=	FormatUtils::BitmaskToBool(op,0x10000);
			fUsefLockAdjustHandles		=	FormatUtils::BitmaskToBool(op,0x20000);
			fUsefLockText				=	FormatUtils::BitmaskToBool(op,0x40000);
			fUsefLockVertices			=	FormatUtils::BitmaskToBool(op,0x80000);

			fUsefLockCropping			=	FormatUtils::BitmaskToBool(op,0x100000);
			fUsefLockAgainstSelect		=	FormatUtils::BitmaskToBool(op,0x200000);
			fUsefLockPosition			=	FormatUtils::BitmaskToBool(op,0x400000);
			fUsefLockAspectRatio		=	FormatUtils::BitmaskToBool(op,0x800000);

			fUsefLockRotation			=	FormatUtils::BitmaskToBool(op,0x1000000);
			fUsefLockAgainstUngrouping	=	FormatUtils::BitmaskToBool(op,0x2000000);
		}

	public: 

		bool fLockAgainstGrouping;
		bool fLockAdjustHandles;
		bool fLockText;
		bool fLockVertices;
		bool fLockCropping;
		bool fLockAgainstSelect;
		bool fLockPosition;
		bool fLockAspectRatio;
		bool fLockRotation;
		bool fLockAgainstUngrouping;

		bool fUsefLockAgainstGrouping;
		bool fUsefLockAdjustHandles;
		bool fUsefLockText;
		bool fUsefLockVertices;
		bool fUsefLockCropping;
		bool fUsefLockAgainstSelect;
		bool fUsefLockPosition;
		bool fUsefLockAspectRatio;
		bool fUsefLockRotation;
		bool fUsefLockAgainstUngrouping;
	};
}