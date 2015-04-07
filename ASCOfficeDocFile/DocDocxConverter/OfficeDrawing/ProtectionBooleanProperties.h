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