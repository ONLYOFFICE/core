#pragma once

namespace DocFileFormat
{
	// 2.3.7.43 Fill Style Boolean Properties	-	[MS-ODRAW] — v20130726

	class FillStyleBooleanProperties
	{
	public:
		FillStyleBooleanProperties(unsigned int op)
		{
			fNoFillHitTest				=	FormatUtils::BitmaskToBool(op, 0x1);
			fillUseRect					=	FormatUtils::BitmaskToBool(op, 0x1 << 1);
			fillShape					=	FormatUtils::BitmaskToBool(op, 0x1 << 2);
			fHitTestFill				=	FormatUtils::BitmaskToBool(op, 0x1 << 3);
			fFilled						=	FormatUtils::BitmaskToBool(op, 0x1 << 4);
			fUseShapeAnchor				=	FormatUtils::BitmaskToBool(op, 0x1 << 5);
			fRecolorFillAsPicture		=	FormatUtils::BitmaskToBool(op, 0x1 << 6);

			// 0x1 << 7-15 is ununsed

			fUsefNoFillHitTest			=	FormatUtils::BitmaskToBool(op, 0x1 << 16);
			fUsefillUseRect				=	FormatUtils::BitmaskToBool(op, 0x1 << 17);
			fUsefillShape				=	FormatUtils::BitmaskToBool(op, 0x1 << 18);
			fUseHitTestFill				=	FormatUtils::BitmaskToBool(op, 0x1 << 19);
			fUsefFilled					=	FormatUtils::BitmaskToBool(op, 0x1 << 20);
			fUsefUseShapeAnchor			=	FormatUtils::BitmaskToBool(op, 0x1 << 21);
			fUsefRecolorFillAsPicture	=	FormatUtils::BitmaskToBool(op, 0x1 << 22);
		}

	public:

		bool fNoFillHitTest;
		bool fillUseRect;
		bool fillShape;
		bool fHitTestFill;
		bool fFilled;
		bool fUseShapeAnchor;
		bool fRecolorFillAsPicture;
		bool fUsefNoFillHitTest;
		bool fUsefillUseRect;
		bool fUsefillShape;
		bool fUseHitTestFill;
		bool fUsefFilled;
		bool fUsefUseShapeAnchor;
		bool fUsefRecolorFillAsPicture;

	};
}