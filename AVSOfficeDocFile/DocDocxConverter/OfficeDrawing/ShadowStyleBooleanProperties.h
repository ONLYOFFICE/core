#pragma once

namespace DocFileFormat
{
	// 2.3.13.23 Shadow Style Boolean Properties	-	[MS-ODRAW] — v20130726

	class ShadowStyleBooleanProperties
	{
	public:
		ShadowStyleBooleanProperties(unsigned int op)
		{
			fshadowObscured		=	FormatUtils::BitmaskToBool(op, 0x1);
			fShadow				=	FormatUtils::BitmaskToBool(op, 0x1 << 1);

			fUsefshadowObscured	=	FormatUtils::BitmaskToBool(op, 0x1 << 16);
			fUsefShadow			=	FormatUtils::BitmaskToBool(op, 0x1 << 17);
		}

	public:

		bool fUsefShadow;
		bool fUsefshadowObscured;

		bool fShadow;
		bool fshadowObscured;
	};
}