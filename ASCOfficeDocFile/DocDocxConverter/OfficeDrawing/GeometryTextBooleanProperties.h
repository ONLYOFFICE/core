#pragma once

namespace DocFileFormat
{
	// 2.3.6.31 Geometry Boolean Properties		-	[MS-ODRAW] — v20130726

	class GeometryTextBooleanProperties
	{
	public: 
		GeometryTextBooleanProperties(unsigned int op)
		{
			gtextFStrikethrough			=	FormatUtils::BitmaskToBool(op, 0x1);
			gtextFSmallcaps				=	FormatUtils::BitmaskToBool(op, 0x1 << 1);
			gtextFShadow				=	FormatUtils::BitmaskToBool(op, 0x1 << 2);
			gtextFUnderline				=	FormatUtils::BitmaskToBool(op, 0x1 << 3);
			gtextFItalic				=	FormatUtils::BitmaskToBool(op, 0x1 << 4);
			gtextFBold					=	FormatUtils::BitmaskToBool(op, 0x1 << 5);
			gtextFDxMeasure				=	FormatUtils::BitmaskToBool(op, 0x1 << 6);
			gtextFNormalize				=	FormatUtils::BitmaskToBool(op, 0x1 << 7);
			gtextFBestFit				=	FormatUtils::BitmaskToBool(op, 0x1 << 8);
			gtextFShrinkFit				=	FormatUtils::BitmaskToBool(op, 0x1 << 9);
			gtextFStretch				=	FormatUtils::BitmaskToBool(op, 0x1 << 10);
			gtextFTight					=	FormatUtils::BitmaskToBool(op, 0x1 << 11);
			gtextFKern					=	FormatUtils::BitmaskToBool(op, 0x1 << 12);
			gtextFVertical				=	FormatUtils::BitmaskToBool(op, 0x1 << 13);
			fGtext						=	FormatUtils::BitmaskToBool(op, 0x1 << 14);
			gtextFReverseRows			=	FormatUtils::BitmaskToBool(op, 0x1 << 15);

			fUsegtextFSStrikeThrough	=	FormatUtils::BitmaskToBool(op, 0x1 << 16);
			fUsegtextFSmallcaps			=	FormatUtils::BitmaskToBool(op, 0x1 << 17);
			fUsegtextFShadow			=	FormatUtils::BitmaskToBool(op, 0x1 << 18);
			fUsegtextFUnderline			=	FormatUtils::BitmaskToBool(op, 0x1 << 19);
			fUsegtextFItalic			=	FormatUtils::BitmaskToBool(op, 0x1 << 20);
			fUsegtextFBold				=	FormatUtils::BitmaskToBool(op, 0x1 << 21);
			fUsegtextFDxMeasure			=	FormatUtils::BitmaskToBool(op, 0x1 << 22);
			fUsegtextFNormalize			=	FormatUtils::BitmaskToBool(op, 0x1 << 23);
			fUsegtextFBestFit			=	FormatUtils::BitmaskToBool(op, 0x1 << 24);
			fUsegtextFShrinkFit			=	FormatUtils::BitmaskToBool(op, 0x1 << 25);
			fUsegtextFStretch			=	FormatUtils::BitmaskToBool(op, 0x1 << 26);
			fUsegtextFTight				=	FormatUtils::BitmaskToBool(op, 0x1 << 27);
			fUsegtextFKern				=	FormatUtils::BitmaskToBool(op, 0x1 << 28);
			fUsegtextFVertical			=	FormatUtils::BitmaskToBool(op, 0x1 << 29);
			fUsefGtext					=	FormatUtils::BitmaskToBool(op, 0x1 << 30);
			fUsegtextFReverseRows		=	FormatUtils::BitmaskToBool(op, 0x40000000);
		}

	public: 

		bool gtextFStrikethrough;
		bool gtextFSmallcaps;
		bool gtextFShadow;
		bool gtextFUnderline;
		bool gtextFItalic;
		bool gtextFBold;
		bool gtextFDxMeasure;
		bool gtextFNormalize;
		bool gtextFBestFit;
		bool gtextFShrinkFit;
		bool gtextFStretch;
		bool gtextFTight;
		bool gtextFKern;
		bool gtextFVertical;
		bool fGtext;
		bool gtextFReverseRows;

		bool fUsegtextFSStrikeThrough;
		bool fUsegtextFSmallcaps;
		bool fUsegtextFShadow;
		bool fUsegtextFUnderline;
		bool fUsegtextFItalic;
		bool fUsegtextFBold;
		bool fUsegtextFDxMeasure;
		bool fUsegtextFNormalize;
		bool fUsegtextFBestFit;
		bool fUsegtextFShrinkFit;
		bool fUsegtextFStretch;
		bool fUsegtextFTight;
		bool fUsegtextFKern;
		bool fUsegtextFVertical;
		bool fUsefGtext;
		bool fUsegtextFReverseRows;
	};
}