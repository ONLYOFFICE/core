#pragma once

namespace DocFileFormat
{
	// 2.3.4.44 Group Shape Boolean Properties	-	[MS-ODRAW] — v20130726

	class GroupShapeBooleanProperties
	{
	public:
		GroupShapeBooleanProperties(unsigned int op)
		{
			fPrint					=	FormatUtils::BitmaskToBool(op, 0x1);
			fHidden					=	FormatUtils::BitmaskToBool(op, 0x2);
			fOneD					=	FormatUtils::BitmaskToBool(op, 0x4);
			fIsButton				=	FormatUtils::BitmaskToBool(op, 0x8);

			fOnDblClickNotify		=	FormatUtils::BitmaskToBool(op, 0x10);
			fBehindDocument			=	FormatUtils::BitmaskToBool(op, 0x20);
			fEditedWrap				=	FormatUtils::BitmaskToBool(op, 0x40);
			fScriptAnchor			=	FormatUtils::BitmaskToBool(op, 0x80);

			fReallyHidden			=	FormatUtils::BitmaskToBool(op, 0x100);
			fAllowOverlap			=	FormatUtils::BitmaskToBool(op, 0x200);
			fUserDrawn				=	FormatUtils::BitmaskToBool(op, 0x400);
			fHorizRule				=	FormatUtils::BitmaskToBool(op, 0x800);

			fNoshadeHR				=	FormatUtils::BitmaskToBool(op, 0x1000);
			fStandardHR				=	FormatUtils::BitmaskToBool(op, 0x2000);
			fIsBullet				=	FormatUtils::BitmaskToBool(op, 0x4000);
			fLayoutInCell			=	FormatUtils::BitmaskToBool(op, 0x8000);

			fUsefPrint				=	FormatUtils::BitmaskToBool(op, 0x10000);
			fUsefHidden				=	FormatUtils::BitmaskToBool(op, 0x20000);
			fUsefOneD				=	FormatUtils::BitmaskToBool(op, 0x40000);
			fUsefIsButton			=	FormatUtils::BitmaskToBool(op, 0x80000);

			fUsefOnDblClickNotify	=	FormatUtils::BitmaskToBool(op, 0x100000);
			fUsefBehindDocument		=	FormatUtils::BitmaskToBool(op, 0x200000);
			fUsefEditedWrap			=	FormatUtils::BitmaskToBool(op, 0x400000);
			fUsefScriptAnchor		=	FormatUtils::BitmaskToBool(op, 0x800000);

			fUsefReallyHidden		=	FormatUtils::BitmaskToBool(op, 0x1000000);
			fUsefAllowOverlap		=	FormatUtils::BitmaskToBool(op, 0x2000000);
			fUsefUserDrawn			=	FormatUtils::BitmaskToBool(op, 0x4000000);
			fUsefHorizRule			=	FormatUtils::BitmaskToBool(op, 0x8000000);

			fUsefNoshadeHR			=	FormatUtils::BitmaskToBool(op, 0x10000000);
			fUsefStandardHR			=	FormatUtils::BitmaskToBool(op, 0x20000000);
			fUsefIsBullet			=	FormatUtils::BitmaskToBool(op, 0x40000000);
			fUsefLayoutInCell		=	FormatUtils::BitmaskToBool(op, 0x80000000);
		}

	public: 

		bool fPrint;
		bool fHidden;
		bool fOneD;
		bool fIsButton;

		bool fOnDblClickNotify;
		bool fBehindDocument;
		bool fEditedWrap;
		bool fScriptAnchor;

		bool fReallyHidden;
		bool fAllowOverlap;
		bool fUserDrawn;
		bool fHorizRule;

		bool fNoshadeHR;
		bool fStandardHR;
		bool fIsBullet;
		bool fLayoutInCell;

		bool fUsefPrint;
		bool fUsefHidden;
		bool fUsefOneD;
		bool fUsefIsButton;

		bool fUsefOnDblClickNotify;
		bool fUsefBehindDocument;
		bool fUsefEditedWrap;
		bool fUsefScriptAnchor;

		bool fUsefReallyHidden;
		bool fUsefAllowOverlap;
		bool fUsefUserDrawn;
		bool fUsefHorizRule;

		bool fUsefNoshadeHR;
		bool fUsefStandardHR;
		bool fUsefIsBullet;
		bool fUsefLayoutInCell;
	};
}