#pragma once

namespace DocFileFormat
{
	class PathSegment
	{
	public:

		enum MSOPATHTYPE
		{
			msopathLineTo,
			msopathCurveTo,
			msopathMoveTo,
			msopathClose,
			msopathEnd,
			msopathEscape,
			msopathClientEscape,
			msopathInvalid
		};

		enum MSOPATHESCAPE
		{
			msopathEscapeExtension 				=	0x00000000,	
			msopathEscapeAngleEllipseTo  		=	0x00000001,
			msopathEscapeAngleEllipse 	 		=	0x00000002,
			msopathEscapeArcTo  				=	0x00000003,
			msopathEscapeArc  					=	0x00000004,
			msopathEscapeClockwiseArcTo  		=	0x00000005,
			msopathEscapeClockwiseArc 	 		=	0x00000006,
			msopathEscapeEllipticalQuadrantX  	=	0x00000007,
			msopathEscapeEllipticalQuadrantY 	=	0x00000008,
			msopathEscapeQuadraticBezier 	 	=	0x00000009,
			msopathEscapeNoFill  				=	0x0000000A,
			msopathEscapeNoLine  				=	0x0000000B,
			msopathEscapeAutoLine  				=	0x0000000C,
			msopathEscapeAutoCurve  			=	0x0000000D,
			msopathEscapeCornerLine  			=	0x0000000E,
			msopathEscapeCornerCurve 			=	0x0000000F,
			msopathEscapeSmoothLine  			=	0x00000010,
			msopathEscapeSmoothCurve  			=	0x00000011,
			msopathEscapeSymmetricLine 			=	0x00000012,
			msopathEscapeSymmetricCurve 		=	0x00000013,
			msopathEscapeFreeform 				=	0x00000014,
			msopathEscapeFillColor 				=	0x00000015,
			msopathEscapeLineColor 				=	0x00000016
		};
		
		PathSegment (unsigned short segment): Type(msopathInvalid), Count(0), EscapeCode(msopathEscapeExtension), VertexCount(0)
		{
			Type			=	(MSOPATHTYPE)FormatUtils::BitmaskToInt (segment, 0xE000);

			if (msopathEscape == Type)
			{
				EscapeCode	=	(MSOPATHESCAPE)FormatUtils::BitmaskToInt (segment, 0x1F00);
				VertexCount	=	FormatUtils::BitmaskToInt (segment, 0x00FF);
			}
			else
			{
				Count		=	FormatUtils::BitmaskToInt (segment, 0x1FFF);
			}
		}

	public:

		MSOPATHTYPE		Type;
		int				Count;
		int				VertexCount;
		MSOPATHESCAPE	EscapeCode;
	};
}