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