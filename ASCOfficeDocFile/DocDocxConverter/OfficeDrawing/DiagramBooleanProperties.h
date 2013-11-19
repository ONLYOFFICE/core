#pragma once

namespace DocFileFormat
{
	// 2.3.17.11 Diagram Boolean Properties		-	[MS-ODRAW] — v20130726

	class DiagramBooleanProperties
	{
	public:
		DiagramBooleanProperties(unsigned int op)
		{
			fPseudoInline		=	FormatUtils::BitmaskToBool(op, 0x1);
			fDoLayout			=	FormatUtils::BitmaskToBool(op, 0x2);
			fReverse			=	FormatUtils::BitmaskToBool(op, 0x4);
			fDoFormat			=	FormatUtils::BitmaskToBool(op, 0x8);

			//unused: 0x10 - 0x8000

			fUsefPseudoInline	=	FormatUtils::BitmaskToBool(op, 0x10000);
			fUsefDoLayout		=	FormatUtils::BitmaskToBool(op, 0x20000);
			fUsefReverse		=	FormatUtils::BitmaskToBool(op, 0x40000);
			fUsefDoFormat		=	FormatUtils::BitmaskToBool(op, 0x80000);
		}

	public:

		bool fPseudoInline;
		bool fDoLayout;
		bool fReverse;
		bool fDoFormat;

		bool fUsefPseudoInline;
		bool fUsefDoLayout;
		bool fUsefReverse;
		bool fUsefDoFormat;
	};
}