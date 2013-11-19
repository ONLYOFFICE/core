#pragma once

namespace DocFileFormat
{
	// 2.3.8.38 Line Style Boolean Properties 	-	[MS-ODRAW] — v20130726

	class LineStyleBooleanProperties
	{
	public:
		LineStyleBooleanProperties(unsigned int op)
		{
			fNoLineDrawDash				=	FormatUtils::BitmaskToBool(op, 0x1);
			fLineFillShape				=	FormatUtils::BitmaskToBool(op, 0x2);
			fHitTestLine				=	FormatUtils::BitmaskToBool(op, 0x4);
			fLine						=	FormatUtils::BitmaskToBool(op, 0x8);

			fArrowheadsOK				=	FormatUtils::BitmaskToBool(op, 0x10);
			fInsetPenOK					=	FormatUtils::BitmaskToBool(op, 0x20);
			fInsetPen					=	FormatUtils::BitmaskToBool(op, 0x40);

			//Reserved 0x80 0x100

			fLineOpaqueBackColor		=	FormatUtils::BitmaskToBool(op, 0x200);

			//Unused 0x400 0x800 0x1000 0x2000 0x4000 0x8000

			fUsefNoLineDrawDash			=	FormatUtils::BitmaskToBool(op, 0x10000);
			fUsefLineFillShape			=	FormatUtils::BitmaskToBool(op, 0x20000);
			fUsefHitTestLine			=	FormatUtils::BitmaskToBool(op, 0x40000);
			fUsefLine					=	FormatUtils::BitmaskToBool(op, 0x80000);
			fUsefArrowheadsOK			=	FormatUtils::BitmaskToBool(op, 0x100000);
			fUsefInsetPenOK				=	FormatUtils::BitmaskToBool(op, 0x200000);
			fUsefInsetPen				=	FormatUtils::BitmaskToBool(op, 0x400000);

			//Reserved 0x800000 0x1000000

			fUsefLineOpaqueBackColor	=	FormatUtils::BitmaskToBool(op, 0x2000000); 
		}

	public:

		bool fNoLineDrawDash;
		bool fLineFillShape;
		bool fHitTestLine;
		bool fLine;
		bool fArrowheadsOK;
		bool fInsetPenOK;
		bool fInsetPen;
		bool fLineOpaqueBackColor;

		bool fUsefNoLineDrawDash;
		bool fUsefLineFillShape;
		bool fUsefHitTestLine;
		bool fUsefLine;
		bool fUsefArrowheadsOK;
		bool fUsefInsetPenOK;
		bool fUsefInsetPen;
		bool fUsefLineOpaqueBackColor;
	};
}