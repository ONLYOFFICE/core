#pragma once

namespace DocFileFormat
{
	// 2.3.6.31 Geometry Boolean Properties		-	[MS-ODRAW] — v20130726

	class GeometryBooleanProperties
	{
	public:

		GeometryBooleanProperties(unsigned int op)
		{
			fFillOK					=	FormatUtils::BitmaskToBool (op, 0x1);
			fFillShadeShapeOK		=	FormatUtils::BitmaskToBool (op, 0x2);
			fGtextOK				=	FormatUtils::BitmaskToBool (op, 0x4);
			fLineOK					=	FormatUtils::BitmaskToBool (op, 0x8);
			f3DOK					=	FormatUtils::BitmaskToBool (op, 0x10);
			fShadowOK				=	FormatUtils::BitmaskToBool (op, 0x20);

			fUsefFillOK				=	FormatUtils::BitmaskToBool (op, 0x10000);
			fUsefFillShadeShapeOK	=	FormatUtils::BitmaskToBool (op, 0x20000);
			fUsefGtextOK			=	FormatUtils::BitmaskToBool (op, 0x40000);
			fUsefLineOK				=	FormatUtils::BitmaskToBool (op, 0x80000);
			fUsef3DOK				=	FormatUtils::BitmaskToBool (op, 0x100000);
			fUsefShadowOK			=	FormatUtils::BitmaskToBool (op, 0x200000);
		}

	public:

		bool fFillOK;
		bool fFillShadeShapeOK;
		bool fGtextOK;
		bool fLineOK;
		bool f3DOK;
		bool fShadowOK;

		bool fUsefFillOK;
		bool fUsefFillShadeShapeOK;
		bool fUsefGtextOK;
		bool fUsefLineOK;
		bool fUsef3DOK;
		bool fUsefShadowOK;
	};
}