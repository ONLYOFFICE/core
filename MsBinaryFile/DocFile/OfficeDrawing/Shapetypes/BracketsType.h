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

#include "../ShapeType.h"

namespace DocFileFormat
{
	class LeftBracketType : public ShapeType
	{
	public:
		LeftBracketType () : ShapeType(msosptLeftBracket)	
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m21600,qx0@0l0@1qy21600,21600e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum #1 0 #0");
            Formulas.push_back(L"sum #1 #0 0");
            Formulas.push_back(L"prod #0 9598 32768");
            Formulas.push_back(L"sum 21600 0 @4");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"min #1 @6");
            Formulas.push_back(L"prod @7 1 2");
            Formulas.push_back(L"prod #0 2 1");
            Formulas.push_back(L"sum 21600 0 @9");
            Formulas.push_back(L"val #1");

            AdjustmentValues	=	L"1800";
            ConnectorLocations	=	L"0,0;21600,@11;0,21600";
            TextBoxRectangle	=	L"0,@4,7637,@5";

			Handle one;
            one.position		=	L"center,#0";
            one.yrange			=	L"0,@8";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"bottomRight,#1";
            two.yrange			=	L"@9,@10";
			Handles.push_back (two);
		}
	};

	class RightBracketType : public ShapeType
	{
	public:
		RightBracketType () : ShapeType(msosptRightBracket)	
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m,qx21600@0l21600@1qy,21600e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"prod #0 9598 32768");
            Formulas.push_back(L"sum 21600 0 @2");

            AdjustmentValues	=	L"1800";
            ConnectorLocations	=	L"0,0;0,21600;21600,10800";
            TextBoxRectangle	=	L"0,@2,15274,@3";

			Handle one;
            one.position		=	L"bottomRight,#0";
            one.yrange			=	L"0,10800";
			Handles.push_back (one);
		}
	};

	class LeftBraceType : public ShapeType
	{
	public:
		LeftBraceType () : ShapeType(msosptLeftBrace)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m21600,qx10800@0l10800@2qy0@11,10800@3l10800@1qy21600,21600e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum #1 0 #0");
            Formulas.push_back(L"sum #1 #0 0");
            Formulas.push_back(L"prod #0 9598 32768");
            Formulas.push_back(L"sum 21600 0 @4");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"min #1 @6");
            Formulas.push_back(L"prod @7 1 2");
            Formulas.push_back(L"prod #0 2 1");
            Formulas.push_back(L"sum 21600 0 @9");
            Formulas.push_back(L"val #1");

            AdjustmentValues	=	L"1800,10800";
            ConnectorLocations	=	L"21600,0;0,10800;21600,21600";
            TextBoxRectangle	=	L"13963,@4,21600,@5";

			Handle one;
            one.position		=	L"center,#0";
            one.yrange			=	L"0,@8";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"topLeft,#1";
            two.yrange			=	L"@9,@10";
			Handles.push_back (two);
		}
	};
	class RightBraceType : public ShapeType
	{
	public:
		RightBraceType () : ShapeType(msosptRightBrace)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m,qx10800@0l10800@2qy21600@11,10800@3l10800@1qy,21600e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum #1 0 #0");
            Formulas.push_back(L"sum #1 #0 0");
            Formulas.push_back(L"prod #0 9598 32768");
            Formulas.push_back(L"sum 21600 0 @4");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"min #1 @6");
            Formulas.push_back(L"prod @7 1 2");
            Formulas.push_back(L"prod #0 2 1");
            Formulas.push_back(L"sum 21600 0 @9");
            Formulas.push_back(L"val #1");

            AdjustmentValues	=	L"1800,10800";
            ConnectorLocations	=	L"0,0;21600,@11;0,21600";
            TextBoxRectangle	=	L"0,@4,7637,@5";

			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"0,@8";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"bottomRight,#1";
            two.yrange			=	L"@9,@10";
			Handles.push_back (two);	
		}
	};
	class BracePairType : public ShapeType
	{
	public:
		BracePairType () : ShapeType(msosptBracePair)	
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@9,nfqx@0@0l@0@7qy0@4@0@8l@0@6qy@9,21600em@10,nfqx@5@0l@5@7qy21600@4@5@8l@5@6qy@10,21600em@9,nsqx@0@0l@0@7qy0@4@0@8l@0@6qy@9,21600l@10,21600qx@5@6l@5@8qy21600@4@5@7l@5@0qy@10,xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"prod width 1 2");
            Formulas.push_back(L"prod height 1 2");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"sum height 0 #0");
            Formulas.push_back(L"sum @4 0 #0");
            Formulas.push_back(L"sum @4 #0 0");
            Formulas.push_back(L"prod #0 2 1");
            Formulas.push_back(L"sum width 0 @9");
            Formulas.push_back(L"prod #0 9598 32768");
            Formulas.push_back(L"sum height 0 @11");
            Formulas.push_back(L"sum @11 #0 0");
            Formulas.push_back(L"sum width 0 @13");

            AdjustmentValues	=	L"1800";
            ConnectorLocations	=	L"@3,0;0,@4;@3,@2;@1,@4";
            TextBoxRectangle	=	L"@13,@11,@14,@12";

            Limo				=	L"10800,10800";

			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"0,5400";
			Handles.push_back (one);
		}
	};


	class BracketPairType : public ShapeType
	{
	public:
		BracketPairType () : ShapeType(msosptBracketPair)	
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0,nfqx0@0l0@2qy@0,21600em@1,nfqx21600@0l21600@2qy@1,21600em@0,nsqx0@0l0@2qy@0,21600l@1,21600qx21600@2l21600@0qy@1,xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"sum height 0 #0");
            Formulas.push_back(L"prod @0 2929 10000");
            Formulas.push_back(L"sum width 0 @3");
            Formulas.push_back(L"sum height 0 @3");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"prod width 1 2");
            Formulas.push_back(L"prod height 1 2");

            AdjustmentValues	=	L"1800";
            ConnectorLocations	=	L"@8,0;0,@9;@8,@7;@6,@9";
            TextBoxRectangle	=	L"@3,@3,@4,@5";

            Limo				=	L"10800,10800";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"0,10800";
			Handles.push_back (one);
		}
	};


}
