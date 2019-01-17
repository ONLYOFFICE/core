/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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


#include "StyleBooleans.h"
#include "../Common/FormatUtils.h"

namespace ASCDocFileFormat
{
	LineStyleBooleans::LineStyleBooleans ()
	{
		fNoLineDrawDash				=	false;
		fLineFillShape				=	false;
		fHitTestLine				=	false;
		fLine						=	false;
		fArrowheadsOK				=	false;
		fInsetPenOK					=	false;
		fInsetPen					=	false;
		fLineOpaqueBackColor		=	false;

		fUsefNoLineDrawDash			=	false;
		fUsefLineFillShape			=	false;
		fUsefHitTestLine			=	false;
		fUsefLine					=	false;
		fUsefArrowheadsOK			=	false;
		fUsefInsetPenOK				=	false;
		fUsefInsetPen				=	false;
		fUsefLineOpaqueBackColor	=	false;
	}

	unsigned int LineStyleBooleans::Get() const
	{
		ASCDocFormatUtils::BitSet oBits(4);

		oBits.SetBit(fNoLineDrawDash,			0);		//	A		
		oBits.SetBit(fLineFillShape,			1);		//	B		
		oBits.SetBit(fHitTestLine,				2);		//	C		
		oBits.SetBit(fLine,						3);		//	D		
		oBits.SetBit(fArrowheadsOK,				4);		//	E		
		oBits.SetBit(fInsetPenOK,				5);		//	F
		oBits.SetBit(fInsetPen,					6);		//	G			

		oBits.SetBit(false,						7);		//	H	-	ignore		
		oBits.SetBit(false,						8);		//	I	-	ignore		

		oBits.SetBit(fLineOpaqueBackColor,		9);		//	J

		oBits.SetBit(false,						10);	//	1	-	ignore			
		oBits.SetBit(false,						11);	//	2	-	ignore				
		oBits.SetBit(false,						12);	//	3	-	ignore				
		oBits.SetBit(false,						13);	//	4	-	ignore				
		oBits.SetBit(false,						14);	//	5	-	ignore				
		oBits.SetBit(false,						15);	//	6	-	ignore					

		oBits.SetBit(fUsefNoLineDrawDash,		16);	//	K	
		oBits.SetBit(fUsefLineFillShape,		17);	//	L		
		oBits.SetBit(fUsefHitTestLine,			18);	//	M			
		oBits.SetBit(fUsefLine,					19);	//	N			
		oBits.SetBit(fUsefArrowheadsOK,			20);	//	O			
		oBits.SetBit(fUsefInsetPenOK,			21);	//	P			
		oBits.SetBit(fUsefInsetPen,				22);	//	Q			

		oBits.SetBit(false,						23);	//	1	-	ignore				
		oBits.SetBit(false,						24);	//	2	-	ignore			

		oBits.SetBit(fUsefLineOpaqueBackColor,	25);	//	T			

		oBits.SetBit(false,						26);	//	1	-	ignore			
		oBits.SetBit(false,						27);	//	2	-	ignore				
		oBits.SetBit(false,						28);	//	3	-	ignore				
		oBits.SetBit(false,						29);	//	4	-	ignore				
		oBits.SetBit(false,						30);	//	5	-	ignore				
		oBits.SetBit(false,						31);	//	6	-	ignore					

		return FormatUtils::BytesToInt32 (oBits.GetBytes(), 0, sizeof(unsigned int));
	}

	FillStyleBooleans::FillStyleBooleans()
	{
		fNoFillHitTest				=	false;
		fillUseRect					=	false;
		fillShape					=	false;
		fHitTestFill				=	false;
		fFilled						=	false;
		fUseShapeAnchor				=	false;
		fRecolorFillAsPicture		=	false;

		fUsefNoFillHitTest			=	false;
		fUsefillUseRect				=	false;
		fUsefillShape				=	false;
		fUseHitTestFill				=	false;
		fUsefFilled					=	false;
		fUsefUseShapeAnchor			=	false;
		fUsefRecolorFillAsPicture	=	false;
	}

	unsigned int FillStyleBooleans::Get() const
	{
		ASCDocFormatUtils::BitSet oBits(4);

		oBits.SetBit(false,						0);		//	unused (9 bits)	
		oBits.SetBit(false,						1);		//	
		oBits.SetBit(false,						2);		//	
		oBits.SetBit(false,						3);		//	
		oBits.SetBit(false,						4);		//	
		oBits.SetBit(false,						5);		//	
		oBits.SetBit(false,						6);		//	
		oBits.SetBit(false,						7);		//	
		oBits.SetBit(false,						8);		//	

		oBits.SetBit(fNoFillHitTest,			9);		//	A		
		oBits.SetBit(fillUseRect,				10);	//	B		
		oBits.SetBit(fillShape,					11);	//	C		
		oBits.SetBit(fHitTestFill,				12);	//	D		
		oBits.SetBit(fFilled,					13);	//	E		
		oBits.SetBit(fUseShapeAnchor,			14);	//	F
		oBits.SetBit(fRecolorFillAsPicture,		15);	//	G
		
		oBits.SetBit(false,						16);	//	unused (9 bits)	
		oBits.SetBit(false,						17);	//	
		oBits.SetBit(false,						18);	//	
		oBits.SetBit(false,						19);	//	
		oBits.SetBit(false,						20);	//	
		oBits.SetBit(false,						21);	//	
		oBits.SetBit(false,						22);	//	
		oBits.SetBit(false,						23);	//	
		oBits.SetBit(false,						24);	//			

		oBits.SetBit(fUsefNoFillHitTest,		25);	//	
		oBits.SetBit(fUsefillUseRect,			26);	//	
		oBits.SetBit(fUsefillShape,				27);	//	
		oBits.SetBit(fUseHitTestFill,			28);	//	
		oBits.SetBit(fUsefFilled,				29);	//	
		oBits.SetBit(fUsefUseShapeAnchor,		30);	//	
		oBits.SetBit(fUsefRecolorFillAsPicture,	31);	//	

		return FormatUtils::BytesToInt32 (oBits.GetBytes(), 0, sizeof(unsigned int));
	}

	GeometryBooleans::GeometryBooleans()
	{
		fFillOK					=	false;
		fFillShadeShapeOK		=	false;
		fGtextOK				=	false;
		fLineOK					=	false;
		f3DOK					=	false;
		fShadowOK				=	false;

		fUsefFillOK				=	false;
		fUsefFillShadeShapeOK	=	false;
		fUsefGtextOK			=	false;
		fUsefLineOK				=	false;
		fUsef3DOK				=	false;
		fUsefShadowOK			=	false;
	}

	unsigned int GeometryBooleans::Get() const
	{
		ASCDocFormatUtils::BitSet oBits(4);

		oBits.SetBit(fFillOK,					0);		//	A		
		oBits.SetBit(fFillShadeShapeOK,			1);		//	B		
		oBits.SetBit(fGtextOK,					2);		//	C		
		oBits.SetBit(fLineOK,					3);		//	D		
		oBits.SetBit(f3DOK,						4);		//	E		
		oBits.SetBit(fShadowOK,					5);		//	F

		oBits.SetBit(false,						6);		//	G	-	ignore								
		oBits.SetBit(false,						7);		//	1	-	ignore		
		oBits.SetBit(false,						8);		//	2	-	ignore		
		oBits.SetBit(false,						9);		//	3	-	ignore		
		oBits.SetBit(false,						10);	//	4	-	ignore			
		oBits.SetBit(false,						11);	//	5	-	ignore				
		oBits.SetBit(false,						12);	//	6	-	ignore				
		oBits.SetBit(false,						13);	//	7	-	ignore				
		oBits.SetBit(false,						14);	//	8	-	ignore				
		oBits.SetBit(false,						15);	//	9	-	ignore					

		oBits.SetBit(fUsefFillOK,				16);	//	H		
		oBits.SetBit(fUsefFillShadeShapeOK,		17);	//	I			
		oBits.SetBit(fUsefGtextOK,				18);	//	J			
		oBits.SetBit(fUsefLineOK,				19);	//	K			
		oBits.SetBit(fUsef3DOK,					20);	//	L			
		oBits.SetBit(fUsefShadowOK,				21);	//	M			

		oBits.SetBit(false,						22);	//	N	-	ignore				
		oBits.SetBit(false,						23);	//	1	-	ignore				
		oBits.SetBit(false,						24);	//	2	-	ignore			
		oBits.SetBit(false,						25);	//	3	-	ignore					
		oBits.SetBit(false,						26);	//	4	-	ignore			
		oBits.SetBit(false,						27);	//	5	-	ignore				
		oBits.SetBit(false,						28);	//	6	-	ignore				
		oBits.SetBit(false,						29);	//	7	-	ignore				
		oBits.SetBit(false,						30);	//	8	-	ignore				
		oBits.SetBit(false,						31);	//	9	-	ignore					

		return FormatUtils::BytesToInt32 (oBits.GetBytes(), 0, sizeof(int));
	}

	GroupShapeBooleans::GroupShapeBooleans()
	{	
		fPrint					=	false;
		fHidden					=	false;
		fOneD					=	false;
		fIsButton				=	false;

		fOnDblClickNotify		=	false;
		fBehindDocument			=	false;
		fEditedWrap				=	false;
		fScriptAnchor			=	false;

		fReallyHidden			=	false;
		fAllowOverlap			=	false;
		fUserDrawn				=	false;
		fHorizRule				=	false;

		fNoshadeHR				=	false;
		fStandardHR				=	false;
		fIsBullet				=	false;
		fLayoutInCell			=	false;

		fUsefPrint				=	false;
		fUsefHidden				=	false;
		fUsefOneD				=	false;
		fUsefIsButton			=	false;

		fUsefOnDblClickNotify	=	false;
		fUsefBehindDocument		=	false;
		fUsefEditedWrap			=	false;
		fUsefScriptAnchor		=	false;

		fUsefReallyHidden		=	false;
		fUsefAllowOverlap		=	false;
		fUsefUserDrawn			=	false;
		fUsefHorizRule			=	false;

		fUsefNoshadeHR			=	false;
		fUsefStandardHR			=	false;
		fUsefIsBullet			=	false;
		fUsefLayoutInCell		=	false;
	}

	unsigned int GroupShapeBooleans::Get() const
	{
		ASCDocFormatUtils::BitSet oBits(4);

		oBits.SetBit(fPrint,					0);		//	A		
		oBits.SetBit(fHidden,					1);		//	B	
		oBits.SetBit(fOneD,						2);		//	C	
		oBits.SetBit(fIsButton,					3);		//	D

		oBits.SetBit(fOnDblClickNotify,			4);		//	E	
		oBits.SetBit(fBehindDocument,			5);		//	F
		oBits.SetBit(fEditedWrap,				6);		//	G							
		oBits.SetBit(fScriptAnchor,				7);		//	H

		oBits.SetBit(fReallyHidden,				8);		//	I	
		oBits.SetBit(fAllowOverlap,				9);		//	J	
		oBits.SetBit(fUserDrawn,				10);	//	K		
		oBits.SetBit(fHorizRule,				11);	//	L

		oBits.SetBit(fNoshadeHR,				12);	//				
		oBits.SetBit(fStandardHR,				13);	//				
		oBits.SetBit(fIsBullet,					14);	//				
		oBits.SetBit(fLayoutInCell,				15);	//					

		oBits.SetBit(fUsefPrint,				16);	//	
		oBits.SetBit(fUsefHidden,				17);	//	
		oBits.SetBit(fUsefOneD,					18);	//	
		oBits.SetBit(fUsefIsButton,				19);	//	

		oBits.SetBit(fUsefOnDblClickNotify,		20);	//	
		oBits.SetBit(fUsefBehindDocument,		21);	//			
		oBits.SetBit(fUsefEditedWrap,			22);	//				
		oBits.SetBit(fUsefScriptAnchor,			23);	//

		oBits.SetBit(fUsefReallyHidden,			24);	//			
		oBits.SetBit(fUsefAllowOverlap,			25);	//					
		oBits.SetBit(fUsefUserDrawn,			26);	//			
		oBits.SetBit(fUsefHorizRule,			27);	//

		oBits.SetBit(fUsefNoshadeHR,			28);	//				
		oBits.SetBit(fUsefStandardHR,			29);	//				
		oBits.SetBit(fUsefIsBullet,				30);	//				
		oBits.SetBit(fUsefLayoutInCell,			31);	//					

		return FormatUtils::BytesToUInt32 (oBits.GetBytes(), 0, sizeof(unsigned int));
	}

	BlipBooleans::BlipBooleans()
	{
		fPictureActive				=	false;
		fPictureBiLevel				=	false;
		fPictureGray				=	false;
		fNoHitTestPicture			=	false;
		fLooping					=	false;
		fRewind						=	false;
		fPicturePreserveGrays		=	false;

		fUsefPictureActive			=	false;
		fUsefPictureBiLevel			=	false;
		fUsefPictureGray			=	false;
		fUsefNoHitTestPicture		=	false;
		fUsefLooping				=	false;
		fUsefRewind					=	false;
		fUsefPicturePreserveGrays	=	false;
	}

	unsigned int BlipBooleans::Get() const
	{
		ASCDocFormatUtils::BitSet oBits(4);

		oBits.SetBit(fPictureActive,			0);		//	A		
		oBits.SetBit(fPictureBiLevel,			1);		//	B	
		oBits.SetBit(fPictureGray,				2);		//	C	
		oBits.SetBit(fNoHitTestPicture,			3);		//	D

		oBits.SetBit(fLooping,					4);		//	E	
		oBits.SetBit(fRewind,					5);		//	F
		oBits.SetBit(fPicturePreserveGrays,		6);		//	G							
		oBits.SetBit(false,						7);		//	H

		oBits.SetBit(false,						8);		//	I	
		oBits.SetBit(false,						9);		//	J	
		oBits.SetBit(false,						10);	//	K		
		oBits.SetBit(false,						11);	//	L

		oBits.SetBit(false,						12);	//				
		oBits.SetBit(false,						13);	//				
		oBits.SetBit(false,						14);	//				
		oBits.SetBit(false,						15);	//					

		oBits.SetBit(fUsefPictureActive,		16);	//	
		oBits.SetBit(fUsefPictureBiLevel,		17);	//	
		oBits.SetBit(fUsefPictureGray,			18);	//	
		oBits.SetBit(fUsefNoHitTestPicture,		19);	//	

		oBits.SetBit(fUsefLooping,				20);	//	
		oBits.SetBit(fUsefRewind,				21);	//			
		oBits.SetBit(fUsefPicturePreserveGrays,	22);	//				
		oBits.SetBit(false,						23);	//

		oBits.SetBit(false,						24);	//			
		oBits.SetBit(false,						25);	//					
		oBits.SetBit(false,						26);	//			
		oBits.SetBit(false,						27);	//

		oBits.SetBit(false,						28);	//				
		oBits.SetBit(false,						29);	//				
		oBits.SetBit(false,						30);	//				
		oBits.SetBit(false,						31);	//					

		return FormatUtils::BytesToUInt32 (oBits.GetBytes(), 0, sizeof(unsigned int));
	}

	TextBooleans::TextBooleans()
	{
		fFitShapeToText				=	false;
		fAutoTextMargin				=	false;
		fSelectText					=	false;

		fUsefFitShapeToText			=	false;
		fUsefAutoTextMargin			=	false;
		fUsefSelectText				=	false;
	}

	unsigned int TextBooleans::Get() const
	{
		ASCDocFormatUtils::BitSet oBits(4);

		oBits.SetBit(false,						0);		//	A		
		oBits.SetBit(fFitShapeToText,			1);		//	B	
		oBits.SetBit(false,						2);		//	C	
		oBits.SetBit(fAutoTextMargin,			3);		//	D

		oBits.SetBit(fSelectText,				4);		//	E	
		oBits.SetBit(false,						5);		//	F
		oBits.SetBit(false,						6);		//	G							
		oBits.SetBit(false,						7);		//	H

		oBits.SetBit(false,						8);		//	I	
		oBits.SetBit(false,						9);		//	J	
		oBits.SetBit(false,						10);	//	K		
		oBits.SetBit(false,						11);	//	L

		oBits.SetBit(false,						12);	//				
		oBits.SetBit(false,						13);	//				
		oBits.SetBit(false,						14);	//				
		oBits.SetBit(false,						15);	//					

		oBits.SetBit(false,						16);	//	
		oBits.SetBit(fUsefFitShapeToText,		17);	//	
		oBits.SetBit(false,						18);	//	
		oBits.SetBit(fUsefAutoTextMargin,		19);	//	

		oBits.SetBit(fUsefSelectText,			20);	//	
		oBits.SetBit(false,						21);	//			
		oBits.SetBit(false,						22);	//				
		oBits.SetBit(false,						23);	//

		oBits.SetBit(false,						24);	//			
		oBits.SetBit(false,						25);	//					
		oBits.SetBit(false,						26);	//			
		oBits.SetBit(false,						27);	//

		oBits.SetBit(false,						28);	//				
		oBits.SetBit(false,						29);	//				
		oBits.SetBit(false,						30);	//				
		oBits.SetBit(false,						31);	//					

		return FormatUtils::BytesToUInt32 (oBits.GetBytes(), 0, sizeof(unsigned int));
	}
	bool TextBooleans::IsValid ()
	{
		return (fFitShapeToText || fAutoTextMargin || fSelectText ||  fUsefFitShapeToText ||	fUsefAutoTextMargin	|| fUsefSelectText);
	}
	ShadowBooleans::ShadowBooleans()
	{
		fUsefShadow					=	false;
		fUsefShadowObscured			=	false;

		fShadow						=	false;
		fShadowObscured				=	false;
	}

	unsigned int ShadowBooleans::Get() const
	{
		// данные формируются не по спецификации

		// 11 00000 00000 000 0 10	-	obscured
		// 11 00000 00000 000 0 11	-	fill

		ASCDocFormatUtils::BitSet oBits(4);

		oBits.SetBit(fShadowObscured,			0);				
		oBits.SetBit(fShadow,					1);					

		oBits.SetBit(false,						2);		
		oBits.SetBit(false,						3);		
		oBits.SetBit(false,						4);		
		oBits.SetBit(false,						5);		
		oBits.SetBit(false,						6);								

		oBits.SetBit(false,						7);		
		oBits.SetBit(false,						8);		
		oBits.SetBit(false,						9);		
		oBits.SetBit(false,						10);		
		oBits.SetBit(false,						11);	

		oBits.SetBit(false,						12);			
		oBits.SetBit(false,						13);			
		oBits.SetBit(false,						14);	

		oBits.SetBit(false,						15);				

		oBits.SetBit(fUsefShadowObscured,		16);	
		oBits.SetBit(fUsefShadow,				17);	
		oBits.SetBit(false,						18);	
		oBits.SetBit(false,						19);	

		oBits.SetBit(false,						20);	
		oBits.SetBit(false,						21);			
		oBits.SetBit(false,						22);				
		oBits.SetBit(false,						23);	

		oBits.SetBit(false,						24);			
		oBits.SetBit(false,						25);					
		oBits.SetBit(false,						26);			
		oBits.SetBit(false,						27);	

		oBits.SetBit(false,						28);				
		oBits.SetBit(false,						29);				
		oBits.SetBit(false,						30);				
		oBits.SetBit(false,						31);					

		return FormatUtils::BytesToUInt32 (oBits.GetBytes(), 0, sizeof(unsigned int));
	}
}
