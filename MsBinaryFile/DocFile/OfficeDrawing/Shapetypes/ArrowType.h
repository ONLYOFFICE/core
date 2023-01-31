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
	class ArrowType : public ShapeType
	{
	public:
		ArrowType () : ShapeType(msosptArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0,l@0@1,0@1,0@2@0@2@0,21600,21600,10800xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"sum height 0 #1");
            Formulas.push_back(L"sum 10800 0 #1");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"prod @4 @3 10800");
            Formulas.push_back(L"sum width 0 @5");

            AdjustmentValues	=	L"16200,5400";
            ConnectorLocations	=	L"@0,0;0,10800;@0,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"0,@1,@6,@2";

			Handle one;
            one.position		=	L"#0,#1";
            one.xrange			=	L"0,21600";
            one.yrange			=	L"0,10800";
			Handles.push_back (one);
		}
	};

	class DownArrowType : public ShapeType
	{
	public:
		DownArrowType () : ShapeType(msosptDownArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m0@0l@1@0@1,0@2,0@2@0,21600@0,10800,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"sum height 0 #1");
            Formulas.push_back(L"sum 10800 0 #1");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"prod @4 @3 10800");
            Formulas.push_back(L"sum width 0 @5");

            AdjustmentValues	=	L"16200,5400";
            ConnectorLocations	=	L"10800,0;0,@0;10800,21600;21600,@0";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"@1,0,@2,@6";

			Handle one;
            one.position		=	L"#1,#0";
            one.xrange			=	L"0,10800";
            one.yrange			=	L"0,21600";
			Handles.push_back (one);
		}
	};

	class UpArrowType : public ShapeType
	{
	public:
		UpArrowType () : ShapeType(msosptUpArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m0@0l@1@0@1,21600@2,21600@2@0,21600@0,10800,xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"prod #0 #1 10800");
            Formulas.push_back(L"sum #0 0 @3");

            AdjustmentValues	=	L"5400,5400";
            ConnectorLocations	=	L"10800,0;0,@0;10800,21600;21600,@0";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"@1,@4,@2,21600";

			Handle one;
            one.position		=	L"#1,#0";
            one.xrange			=	L"0,10800";
            one.yrange			=	L"0,21600";
			Handles.push_back (one);
		}
	};

	class LeftArrowType : public ShapeType
	{
	public:
		LeftArrowType () : ShapeType(msosptLeftArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0,l@0@1,21600@1,21600@2@0@2@0,21600,,10800xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"prod #0 #1 10800");
            Formulas.push_back(L"sum #0 0 @3");

            AdjustmentValues	=	L"5400,5400";
            ConnectorLocations	=	L"@0,0;0,10800;@0,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"@4,@1,21600,@2";

			Handle one;
            one.position		=	L"#0,#1";
            one.xrange			=	L"0,21600";
            one.yrange			=	L"0,10800";
			Handles.push_back (one);
		}
	};

	class LeftRightArrow : public ShapeType
	{
	public:
		LeftRightArrow () : ShapeType(msosptLeftRightArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m,10800l@0,21600@0@3@2@3@2,21600,21600,10800@2,0@2@1@0@1@0,xe";

            AdjustmentValues	=	L"4320,5400";
            ConnectorLocations	=	L"@2,0;10800,@1;@0,0;0,10800;@0,21600;10800,@3;@2,21600;21600,10800";
            ConnectorAngles		=	L"270,270,270,180,90,90,90,0";
            TextBoxRectangle	=	L"@5,@1,@6,@3";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"prod #0 #1 10800");
            Formulas.push_back(L"sum #0 0 @4");
            Formulas.push_back(L"sum 21600 0 @5");

			Handle one;
            one.position		=	L"#0,#1";
            one.xrange			=	L"0,10800";
            one.yrange			=	L"0,10800";
			Handles.push_back (one);
		}
	};

	class UpDownArrow : public ShapeType
	{
	public:
		UpDownArrow () : ShapeType(msosptUpDownArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m10800,l21600@0@3@0@3@2,21600@2,10800,21600,0@2@1@2@1@0,0@0xe";

            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"prod #1 #0 10800");
            Formulas.push_back(L"sum #1 0 @4");
            Formulas.push_back(L"sum 21600 0 @5");

            AdjustmentValues	=	L"5400,4320";
            ConnectorLocations	=	L"10800,0;0,@0;@1,10800;0,@2;10800,21600;21600,@2;@3,10800;21600,@0";
            ConnectorAngles		=	L"270,180,180,180,90,0,0,0";
            TextBoxRectangle	=	L"@1,@5,@3,@6";

			Handle one;
            one.position		=	L"#0,#1";
            one.xrange			=	L"0,10800";
            one.yrange			=	L"0,10800";
			Handles.push_back (one);
		}
	};

	class QuadArrow : public ShapeType
	{
	public:
		QuadArrow () : ShapeType(msosptQuadArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m10800,l@0@2@1@2@1@1@2@1@2@0,,10800@2@3@2@4@1@4@1@5@0@5,10800,21600@3@5@4@5@4@4@5@4@5@3,21600,10800@5@0@5@1@4@1@4@2@3@2xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"sum 21600 0 #2");
            Formulas.push_back(L"sum #0 0 10800");
            Formulas.push_back(L"sum #1 0 10800");
            Formulas.push_back(L"prod @7 #2 @6");
            Formulas.push_back(L"sum 21600 0 @8");

            AdjustmentValues	=	L"6480,8640,4320";
            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"@8,@1,@9,@4;@1,@8,@4,@9";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"@2,@1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,#2";
            two.xrange			=	L"@0,10800";
            two.yrange			=	L"0,@0";
			Handles.push_back (two);
		}
	};

	class LeftRightUpArrow : public ShapeType
	{
	public:
		LeftRightUpArrow () : ShapeType(msosptLeftRightUpArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m10800,l@0@2@1@2@1@6@7@6@7@5,0@8@7,21600@7@9@10@9@10,21600,21600@8@10@5@10@6@4@6@4@2@3@2xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"prod @0 21600 @3");
            Formulas.push_back(L"prod @1 21600 @3");
            Formulas.push_back(L"prod @2 @3 21600");
            Formulas.push_back(L"prod 10800 21600 @3");
            Formulas.push_back(L"prod @4 21600 @3");
            Formulas.push_back(L"sum 21600 0 @7");
            Formulas.push_back(L"sum @5 0 @8");
            Formulas.push_back(L"sum @6 0 @8");
            Formulas.push_back(L"prod @12 @7 @11");
            Formulas.push_back(L"sum 21600 0 @13");
            Formulas.push_back(L"sum @0 0 10800");
            Formulas.push_back(L"sum @1 0 10800");
            Formulas.push_back(L"prod @2 @16 @15");

            AdjustmentValues	=	L"6480,8640,6171";
            ConnectorLocations	=	L"10800,0;0,@8;10800,@9;21600,@8";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"@13,@6,@14,@9;@1,@17,@4,@9";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"@2,@1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,#2";
            two.xrange			=	L"@0,10800";
            two.yrange			=	L"0,@5";
			Handles.push_back (two);
		}
	};

	class BentArrow : public ShapeType			//	фигура превращается не имеет точек изменения
	{
	public:
		BentArrow () : ShapeType(msosptBentArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m21600,6079l15126,r,2912l12427,2912c5564,2912,,7052,,12158r,9442l6474,21600r,-9442c6474,10550,9139,9246,12427,9246r2699,l15126,12158,21600,6079xe";

            ConnectorLocations	=	L"2830950,0;2830950,2722432;605830,4836695;4042610,1361216";
            ConnectorAngles		=	L"270,90,90,0";
            TextBoxRectangle	=	L"12427,2912,18227,9246";
		}
	};

	class UturnArrow : public ShapeType			
	{
	public:
		UturnArrow () : ShapeType(msosptUturnArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m15662,14285l21600,8310r-2970,qy9250,,,8485l,21600r6110,l6110,8310qy8907,5842l9725,5842qx12520,8310l9725,8310xe";

            ConnectorLocations	=	L"9250,0;3055,21600;9725,8310;15662,14285;21600,8310";
            ConnectorAngles		=	L"270,90,90,90,0";
            TextBoxRectangle	=	L"0,8310,6110,21600";
		}
	};

	class LeftUpArrow : public ShapeType		
	{
	public:
		LeftUpArrow () : ShapeType(msosptLeftUpArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@4,l@0@2@5@2@5@5@2@5@2@0,0@4@2,21600@2@1@1@1@1@2,21600@2xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"sum @3 10800 0");
            Formulas.push_back(L"sum 21600 #0 #1");
            Formulas.push_back(L"sum #1 #2 0");
            Formulas.push_back(L"prod @6 1 2");
            Formulas.push_back(L"prod #1 2 1");
            Formulas.push_back(L"sum @8 0 21600");
            Formulas.push_back(L"sum @5 0 @4");
            Formulas.push_back(L"sum #0 0 @4");
            Formulas.push_back(L"prod @2 @10 @11");

            AdjustmentValues	=	L"9257,18514,6171";
            ConnectorLocations	=	L"@4,0;@0,@2;@2,@0;0,@4;@2,21600;@7,@1;@1,@7;21600,@2";
            ConnectorAngles		=	L"270,180,270,180,90,90,0,0";
            TextBoxRectangle	=	L"@12,@5,@1,@1;@5,@12,@1,@1";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"@2,@9";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,#2";
            two.xrange			=	L"@4,21600";
            two.yrange			=	L"0,@0";
			Handles.push_back (two);

		}
	};

	class BentUpArrow : public ShapeType		
	{
	public:
		BentUpArrow () : ShapeType(msosptBentUpArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@4,l@0@2@5@2@5@12,0@12,,21600@1,21600@1@2,21600@2xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"sum @3 10800 0");
            Formulas.push_back(L"sum 21600 #0 #1");
            Formulas.push_back(L"sum #1 #2 0");
            Formulas.push_back(L"prod @6 1 2");
            Formulas.push_back(L"prod #1 2 1");
            Formulas.push_back(L"sum @8 0 21600");
            Formulas.push_back(L"prod 21600 @0 @1");
            Formulas.push_back(L"prod 21600 @4 @1");
            Formulas.push_back(L"prod 21600 @5 @1");
            Formulas.push_back(L"prod 21600 @7 @1");
            Formulas.push_back(L"prod #1 1 2");
            Formulas.push_back(L"sum @5 0 @4");
            Formulas.push_back(L"sum @0 0 @4");
            Formulas.push_back(L"prod @2 @15 @16");

            AdjustmentValues	=	L"9257,18514,7200";
            ConnectorLocations	=	L"@4,0;@0,@2;0,@11;@14,21600;@1,@13;21600,@2";
            ConnectorAngles		=	L"270,180,180,90,0,0";
            TextBoxRectangle	=	L"0,@12,@1,21600;@5,@17,@1,21600";


			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"@2,@9";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,#2";
            two.xrange			=	L"@4,21600";
            two.yrange			=	L"0,@0";
			Handles.push_back (two);
		}
	};

	class CurvedRightArrow : public ShapeType	
	{
	public:
		CurvedRightArrow () : ShapeType(msosptCurvedRightArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"ar,0@23@3@22,,0@4,0@15@23@1,0@7@2@13l@2@14@22@8@2@12wa,0@23@3@2@11@26@17,0@15@23@1@26@17@22@15xear,0@23@3,0@4@26@17nfe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"sum #0 width #1");
            Formulas.push_back(L"prod @3 1 2");
            Formulas.push_back(L"sum #1 #1 width");
            Formulas.push_back(L"sum @5 #1 #0");
            Formulas.push_back(L"prod @6 1 2");
            Formulas.push_back(L"mid width #0");
            Formulas.push_back(L"sum height 0 #2");
            Formulas.push_back(L"ellipse @9 height @4");
            Formulas.push_back(L"sum @4 @10 0");
            Formulas.push_back(L"sum @11 #1 width");
            Formulas.push_back(L"sum @7 @10 0");
            Formulas.push_back(L"sum @12 width #0");
            Formulas.push_back(L"sum @5 0 #0");
            Formulas.push_back(L"prod @15 1 2");
            Formulas.push_back(L"mid @4 @7");
            Formulas.push_back(L"sum #0 #1 width");
            Formulas.push_back(L"prod @18 1 2");
            Formulas.push_back(L"sum @17 0 @19");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"prod height 2 1");
            Formulas.push_back(L"sum @17 0 @4");
            Formulas.push_back(L"ellipse @24 @4 height");
            Formulas.push_back(L"sum height 0 @25");
            Formulas.push_back(L"sum @8 128 0");
            Formulas.push_back(L"prod @5 1 2");
            Formulas.push_back(L"sum @5 0 128");
            Formulas.push_back(L"sum #0 @17 @12");
            Formulas.push_back(L"ellipse @20 @4 height");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"prod @32 1 2");
            Formulas.push_back(L"prod height height 1");
            Formulas.push_back(L"prod @9 @9 1");
            Formulas.push_back(L"sum @34 0 @35");
            Formulas.push_back(L"sqrt @36");
            Formulas.push_back(L"sum @37 height 0");
            Formulas.push_back(L"prod width height @38");
            Formulas.push_back(L"sum @39 64 0");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"ellipse @33 @41 height");
            Formulas.push_back(L"sum height 0 @42");
            Formulas.push_back(L"sum @43 64 0");
            Formulas.push_back(L"prod @4 1 2");
            Formulas.push_back(L"sum #1 0 @45");
            Formulas.push_back(L"prod height 4390 32768");
            Formulas.push_back(L"prod height 28378 32768");

            AdjustmentValues	=	L"12960,19440,14400";
            ConnectorLocations	=	L"0,@17;@2,@14;@22,@8;@2,@12;@22,@16";
            ConnectorAngles		=	L"180,90,0,0,0";
            TextBoxRectangle	=	L"@47,@45,@48,@46";

			Handle one;
            one.position		=	L"bottomRight,#0";
            one.yrange			=	L"@40,@29";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"bottomRight,#1";
            two.yrange			=	L"@27,@21";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#2,bottomRight";
            three.xrange		=	L"@44,@22";
			Handles.push_back (three);
		}
	};

	class CurvedLeftArrow : public ShapeType	
	{
	public:
		CurvedLeftArrow () : ShapeType(msosptCurvedLeftArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"wr@22,0@21@3,,0@21@4@22@14@21@1@21@7@2@12l@2@13,0@8@2@11at@22,0@21@3@2@10@24@16@22@14@21@1@24@16,0@14xear@22@14@21@1@21@7@24@16nfe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"sum #0 width #1");
            Formulas.push_back(L"prod @3 1 2");
            Formulas.push_back(L"sum #1 #1 width");
            Formulas.push_back(L"sum @5 #1 #0");
            Formulas.push_back(L"prod @6 1 2");
            Formulas.push_back(L"mid width #0");
            Formulas.push_back(L"ellipse #2 height @4");
            Formulas.push_back(L"sum @4 @9 0");
            Formulas.push_back(L"sum @10 #1 width");
            Formulas.push_back(L"sum @7 @9 0");
            Formulas.push_back(L"sum @11 width #0");
            Formulas.push_back(L"sum @5 0 #0");
            Formulas.push_back(L"prod @14 1 2");
            Formulas.push_back(L"mid @4 @7");
            Formulas.push_back(L"sum #0 #1 width");
            Formulas.push_back(L"prod @17 1 2");
            Formulas.push_back(L"sum @16 0 @18");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"sum 0 0 height");
            Formulas.push_back(L"sum @16 0 @4");
            Formulas.push_back(L"ellipse @23 @4 height");
            Formulas.push_back(L"sum @8 128 0");
            Formulas.push_back(L"prod @5 1 2");
            Formulas.push_back(L"sum @5 0 128");
            Formulas.push_back(L"sum #0 @16 @11");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"prod @29 1 2");
            Formulas.push_back(L"prod height height 1");
            Formulas.push_back(L"prod #2 #2 1");
            Formulas.push_back(L"sum @31 0 @32");
            Formulas.push_back(L"sqrt @33");
            Formulas.push_back(L"sum @34 height 0");
            Formulas.push_back(L"prod width height @35");
            Formulas.push_back(L"sum @36 64 0");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"ellipse @30 @38 height");
            Formulas.push_back(L"sum @39 0 64");
            Formulas.push_back(L"prod @4 1 2");
            Formulas.push_back(L"sum #1 0 @41");
            Formulas.push_back(L"prod height 4390 32768");
            Formulas.push_back(L"prod height 28378 32768");

            AdjustmentValues	=	L"12960,19440,14400";
            ConnectorLocations	=	L"0,@17;@2,@14;@22,@8;@2,@12;@22,@16";
            ConnectorAngles		=	L"180,90,0,0,0";
            TextBoxRectangle	=	L"@47,@45,@48,@46";

			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"@37,@27";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"topLeft,#1";
            two.yrange			=	L"@25,@20";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#2,bottomRight";
            three.xrange		=	L"0,@40";
			Handles.push_back (three);
		}
	};

	class CurvedUpArrow : public ShapeType	
	{
	public:
		CurvedUpArrow () : ShapeType(msosptCurvedUpArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"ar0@22@3@21,,0@4@21@14@22@1@21@7@21@12@2l@13@2@8,0@11@2wa0@22@3@21@10@2@16@24@14@22@1@21@16@24@14,xewr@14@22@1@21@7@21@16@24nfe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"sum #0 width #1");
            Formulas.push_back(L"prod @3 1 2");
            Formulas.push_back(L"sum #1 #1 width");
            Formulas.push_back(L"sum @5 #1 #0");
            Formulas.push_back(L"prod @6 1 2");
            Formulas.push_back(L"mid width #0");
            Formulas.push_back(L"ellipse #2 height @4");
            Formulas.push_back(L"sum @4 @9 0");
            Formulas.push_back(L"sum @10 #1 width");
            Formulas.push_back(L"sum @7 @9 0");
            Formulas.push_back(L"sum @11 width #0");
            Formulas.push_back(L"sum @5 0 #0");
            Formulas.push_back(L"prod @14 1 2");
            Formulas.push_back(L"mid @4 @7");
            Formulas.push_back(L"sum #0 #1 width");
            Formulas.push_back(L"prod @17 1 2");
            Formulas.push_back(L"sum @16 0 @18");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"sum 0 0 height");
            Formulas.push_back(L"sum @16 0 @4");
            Formulas.push_back(L"ellipse @23 @4 height");
            Formulas.push_back(L"sum @8 128 0");
            Formulas.push_back(L"prod @5 1 2");
            Formulas.push_back(L"sum @5 0 128");
            Formulas.push_back(L"sum #0 @16 @11");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"prod @29 1 2");
            Formulas.push_back(L"prod height height 1");
            Formulas.push_back(L"prod #2 #2 1");
            Formulas.push_back(L"sum @31 0 @32");
            Formulas.push_back(L"sqrt @33");
            Formulas.push_back(L"sum @34 height 0");
            Formulas.push_back(L"prod width height @35");
            Formulas.push_back(L"sum @36 64 0");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"ellipse @30 @38 height");
            Formulas.push_back(L"sum @39 0 64");
            Formulas.push_back(L"prod @4 1 2");
            Formulas.push_back(L"sum #1 0 @41");
            Formulas.push_back(L"prod height 4390 32768");
            Formulas.push_back(L"prod height 28378 32768");

            AdjustmentValues	=	L"12960,19440,7200";
            ConnectorLocations	=	L"@8,0;@11,@2;@15,0;@16,@21;@13,@2";
            ConnectorAngles		=	L"270,270,270,90,0";
            TextBoxRectangle	=	L"@41,@43,@42,@44";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"@37,@27";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,topLeft";
            two.xrange			=	L"@25,@20";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"bottomRight,#2";
            three.yrange		=	L"0,@40";
			Handles.push_back (three);
		}
	};

	class CurvedDownArrow : public ShapeType	
	{
	public:
		CurvedDownArrow () : ShapeType(msosptCurvedDownArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"wr,0@3@23,0@22@4,0@15,0@1@23@7,0@13@2l@14@2@8@22@12@2at,0@3@23@11@2@17@26@15,0@1@23@17@26@15@22xewr,0@3@23@4,0@17@26nfe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"sum #0 width #1");
            Formulas.push_back(L"prod @3 1 2");
            Formulas.push_back(L"sum #1 #1 width");
            Formulas.push_back(L"sum @5 #1 #0");
            Formulas.push_back(L"prod @6 1 2");
            Formulas.push_back(L"mid width #0");
            Formulas.push_back(L"sum height 0 #2");
            Formulas.push_back(L"ellipse @9 height @4");
            Formulas.push_back(L"sum @4 @10 0");
            Formulas.push_back(L"sum @11 #1 width");
            Formulas.push_back(L"sum @7 @10 0");
            Formulas.push_back(L"sum @12 width #0");
            Formulas.push_back(L"sum @5 0 #0");
            Formulas.push_back(L"prod @15 1 2");
            Formulas.push_back(L"mid @4 @7");
            Formulas.push_back(L"sum #0 #1 width");
            Formulas.push_back(L"prod @18 1 2");
            Formulas.push_back(L"sum @17 0 @19");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"prod height 2 1");
            Formulas.push_back(L"sum @17 0 @4");
            Formulas.push_back(L"ellipse @24 @4 height");
            Formulas.push_back(L"sum height 0 @25");
            Formulas.push_back(L"sum @8 128 0");
            Formulas.push_back(L"prod @5 1 2");
            Formulas.push_back(L"sum @5 0 128");
            Formulas.push_back(L"sum #0 @17 @12");
            Formulas.push_back(L"ellipse @20 @4 height");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"prod @32 1 2");
            Formulas.push_back(L"prod height height 1");
            Formulas.push_back(L"prod @9 @9 1");
            Formulas.push_back(L"sum @34 0 @35");
            Formulas.push_back(L"sqrt @36");
            Formulas.push_back(L"sum @37 height 0");
            Formulas.push_back(L"prod width height @38");
            Formulas.push_back(L"sum @39 64 0");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"ellipse @33 @41 height");
            Formulas.push_back(L"sum height 0 @42");
            Formulas.push_back(L"sum @43 64 0");
            Formulas.push_back(L"prod @4 1 2");
            Formulas.push_back(L"sum #1 0 @45");
            Formulas.push_back(L"prod height 4390 32768");
            Formulas.push_back(L"prod height 28378 32768");

            AdjustmentValues	=	L"12960,19440,14400";
            ConnectorLocations	=	L"@17,0;@16,@22;@12,@2;@8,@22;@14,@2";
            ConnectorAngles		=	L"270,90,90,90,0";
            TextBoxRectangle	=	L"@45,@47,@46,@48";

			Handle one;
            one.position		=	L"#0,bottomRight";
            one.xrange			=	L"@40,@29";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,bottomRight";
            two.xrange			=	L"@27,@21";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"bottomRight,#2";
            three.yrange		=	L"@44,@22";
			Handles.push_back (three);
		}
	};

	class StripedRightArrow : public ShapeType		
	{
	public:
		StripedRightArrow () : ShapeType(msosptStripedRightArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0,l@0@1,3375@1,3375@2@0@2@0,21600,21600,10800xem1350@1l1350@2,2700@2,2700@1xem0@1l0@2,675@2,675@1xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"sum height 0 #1");
            Formulas.push_back(L"sum 10800 0 #1");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"prod @4 @3 10800");
            Formulas.push_back(L"sum width 0 @5");

            AdjustmentValues	=	L"16200,5400";
            ConnectorLocations	=	L"@0,0;0,10800;@0,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"3375,@1,@6,@2";

			Handle one;
            one.position		=	L"#0,#1";
            one.xrange			=	L"3375,21600";
            one.yrange			=	L"0,10800";
			Handles.push_back (one);
		}
	};

	class NotchedRightArrow : public ShapeType		
	{
	public:
		NotchedRightArrow () : ShapeType(msosptNotchedRightArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0,l@0@1,0@1@5,10800,0@2@0@2@0,21600,21600,10800xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"sum height 0 #1");
            Formulas.push_back(L"sum 10800 0 #1");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"prod @4 @3 10800");
            Formulas.push_back(L"sum width 0 @5");

            AdjustmentValues	=	L"16200,5400";
            ConnectorLocations	=	L"@0,0;@5,10800;@0,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"@5,@1,@6,@2";

			Handle one;
            one.position		=	L"#0,#1";
            one.xrange			=	L"0,21600";
            one.yrange			=	L"0,10800";
			Handles.push_back (one);
		}
	};

	class HomePlate : public ShapeType		
	{
	public:
		HomePlate () : ShapeType(msosptHomePlate)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0,l,,,21600@0,21600,21600,10800xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"prod #0 1 2");

            AdjustmentValues	=	L"16200";
            ConnectorLocations	=	L"@1,0;0,10800;@1,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"0,0,10800,21600;0,0,16200,21600;0,0,21600,21600";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"0,21600";
			Handles.push_back (one);
		}
	};

	class Chevron : public ShapeType		
	{
	public:
		Chevron () : ShapeType(msosptChevron)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0,l,0@1,10800,,21600@0,21600,21600,10800xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 @0");
            Formulas.push_back(L"prod #0 1 2");

            AdjustmentValues	=	L"16200";
            ConnectorLocations	=	L"@2,0;@1,10800;@2,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"0,0,10800,21600;0,0,16200,21600;0,0,21600,21600";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"0,21600";
			Handles.push_back (one);
		}
	};

	class RightArrowCallout : public ShapeType		
	{
	public:
		RightArrowCallout () : ShapeType(msosptRightArrowCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m,l,21600@0,21600@0@5@2@5@2@4,21600,10800@2@1@2@3@0@3@0,xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"sum 21600 0 #3");
            Formulas.push_back(L"prod #0 1 2");

            AdjustmentValues	=	L"14400,5400,18000,8100";
            ConnectorLocations	=	L"@6,0;0,10800;@6,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"0,0,@0,21600";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"0,@2";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"bottomRight,#1";
            two.yrange			=	L"0,@3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#2,#3";
            three.xrange		=	L"@0,21600";
            three.yrange		=	L"@1,10800";
			Handles.push_back (three);
		}
	};

	class LeftArrowCallout : public ShapeType		
	{
	public:
		LeftArrowCallout () : ShapeType(msosptLeftArrowCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0,l@0@3@2@3@2@1,,10800@2@4@2@5@0@5@0,21600,21600,21600,21600,xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"sum 21600 0 #3");
            Formulas.push_back(L"sum #0 21600 0");

            AdjustmentValues	=	L"7200,5400,3600,8100";
            ConnectorLocations	=	L"@7,0;0,10800;@7,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"@0,0,21600,21600";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"@2,21600";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"topLeft,#1";
            two.yrange			=	L"0,@3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#2,#3";
            three.xrange		=	L"0,@0";
            three.yrange		=	L"@1,10800";
			Handles.push_back (three);
		}
	};

	class UpArrowCallout : public ShapeType		
	{
	public:
		UpArrowCallout () : ShapeType(msosptLeftArrowCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m0@0l@3@0@3@2@1@2,10800,0@4@2@5@2@5@0,21600@0,21600,21600,,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"sum 21600 0 #3");
            Formulas.push_back(L"sum #0 21600 0");
            Formulas.push_back(L"prod @6 1 2");

            AdjustmentValues	=	L"7200,5400,3600,8100";
            ConnectorLocations	=	L"10800,0;0,@7;10800,21600;21600,@7";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"0,@0,21600,21600";

			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"@2,21600";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,topLeft";
            two.xrange			=	L"0,@3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#3,#2";
            three.xrange		=	L"@1,10800";
            three.yrange		=	L"0,@0";
			Handles.push_back (three);
		}
	};

	class DownArrowCallout : public ShapeType		
	{
	public:
		DownArrowCallout () : ShapeType(msosptDownArrowCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m,l21600,,21600@0@5@0@5@2@4@2,10800,21600@1@2@3@2@3@0,0@0xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"sum 21600 0 #3");
            Formulas.push_back(L"prod #0 1 2");

            AdjustmentValues	=	L"14400,5400,18000,8100";
            ConnectorLocations	=	L"10800,0;0,@6;10800,21600;21600,@6";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"0,0,21600,@0";

			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"0,@2";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,bottomRight";
            two.xrange			=	L"0,@3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#3,#2";
            three.xrange		=	L"@1,10800";
            three.yrange		=	L"@0,21600";
			Handles.push_back (three);
		}
	};

	class LeftRightArrowCallout : public ShapeType		
	{
	public:
		LeftRightArrowCallout () : ShapeType(msosptDownArrowCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0,l@0@3@2@3@2@1,,10800@2@4@2@5@0@5@0,21600@8,21600@8@5@9@5@9@4,21600,10800@9@1@9@3@8@3@8,xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"sum 21600 0 #3");
            Formulas.push_back(L"sum #0 21600 0");
            Formulas.push_back(L"prod @6 1 2");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 #2");

            AdjustmentValues	=	L"5400,5400,2700,8100";
            ConnectorLocations	=	L"10800,0;0,10800;10800,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"@0,0,@8,21600";

			Handle one;
            one.position		=	L"#0,topLeft";
            one.yrange			=	L"@2,10800";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"topLeft,#1";
            two.yrange			=	L"0,@3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#2,#3";
            three.xrange		=	L"0,@0";
            three.yrange		=	L"@1,10800";
			Handles.push_back (three);
		}
	};

	class UpDownArrowCallout : public ShapeType		
	{
	public:
		UpDownArrowCallout () : ShapeType(msosptUpDownArrowCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m0@0l@3@0@3@2@1@2,10800,0@4@2@5@2@5@0,21600@0,21600@8@5@8@5@9@4@9,10800,21600@1@9@3@9@3@8,0@8xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"sum 21600 0 #3");
            Formulas.push_back(L"sum #0 21600 0");
            Formulas.push_back(L"prod @6 1 2");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 #2");

            AdjustmentValues	=	L"5400,5400,2700,8100";
            ConnectorLocations	=	L"10800,0;0,10800;10800,21600;21600,10800";
            ConnectorAngles		=	L"270,180,90,0";
            TextBoxRectangle	=	L"0,@0,21600,@8";

			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"@2,10800";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,topLeft";
            two.xrange			=	L"0,@3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#3,#2";
            three.xrange		=	L"@1,10800";
            three.yrange		=	L"0,@0";
			Handles.push_back (three);
		}
	};

	class QuadArrowCallout : public ShapeType		
	{
	public:
		QuadArrowCallout () : ShapeType(msosptQuadArrowCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m@0@0l@3@0@3@2@1@2,10800,0@4@2@5@2@5@0@8@0@8@3@9@3@9@1,21600,10800@9@4@9@5@8@5@8@8@5@8@5@9@4@9,10800,21600@1@9@3@9@3@8@0@8@0@5@2@5@2@4,,10800@2@1@2@3@0@3xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"sum 21600 0 #3");
            Formulas.push_back(L"sum #0 21600 0");
            Formulas.push_back(L"prod @6 1 2");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 #2");

            AdjustmentValues	=	L"5400,8100,2700,9450";
            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"@0,@0,@8,@8";

			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"@2,@1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,topLeft";
            two.xrange			=	L"0,@3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#3,#2";
            three.xrange		=	L"@1,10800";
            three.yrange		=	L"0,@0";
			Handles.push_back (three);
		}
	};

	class CircularArrow : public ShapeType				//	фигура превращается не имеет точек изменения
	{
	public:
		CircularArrow () : ShapeType(msosptCircularArrow)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;

            AdjustmentValues	=	L"-11796480,,5400";

            Path				=	L"al10800,10800@8@8@4@6,10800,10800,10800,10800@9@7l@30@31@17@18@24@25@15@16@32@33xe";

            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum #1 0 #0");
            Formulas.push_back(L"val 10800");
            Formulas.push_back(L"sum 0 0 #1");
            Formulas.push_back(L"sumangle @2 360 0");
            Formulas.push_back(L"if @2 @2 @5");
            Formulas.push_back(L"sum 0 0 @6");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"sum 0 0 #0");
            Formulas.push_back(L"sum #2 0 2700");
            Formulas.push_back(L"cos @10 #1");
            Formulas.push_back(L"sin @10 #1");
            Formulas.push_back(L"cos 13500 #1");
            Formulas.push_back(L"sin 13500 #1");
            Formulas.push_back(L"sum @11 10800 0");
            Formulas.push_back(L"sum @12 10800 0");
            Formulas.push_back(L"sum @13 10800 0");
            Formulas.push_back(L"sum @14 10800 0");
            Formulas.push_back(L"prod #2 1 2");
            Formulas.push_back(L"sum @19 5400 0");
            Formulas.push_back(L"cos @20 #1");
            Formulas.push_back(L"sin @20 #1");
            Formulas.push_back(L"sum @21 10800 0");
            Formulas.push_back(L"sum @12 @23 @22");
            Formulas.push_back(L"sum @22 @23 @11");
            Formulas.push_back(L"cos 10800 #1");
            Formulas.push_back(L"sin 10800 #1");
            Formulas.push_back(L"cos #2 #1");
            Formulas.push_back(L"sin #2 #1");
            Formulas.push_back(L"sum @26 10800 0");
            Formulas.push_back(L"sum @27 10800 0");
            Formulas.push_back(L"sum @28 10800 0");
            Formulas.push_back(L"sum @29 10800 0");
            Formulas.push_back(L"sum @19 5400 0");
            Formulas.push_back(L"cos @34 #0");
            Formulas.push_back(L"sin @34 #0");
            Formulas.push_back(L"mid #0 #1");
            Formulas.push_back(L"sumangle @37 180 0");
            Formulas.push_back(L"if @2 @37 @38");
            Formulas.push_back(L"cos 10800 @39");
            Formulas.push_back(L"sin 10800 @39");
            Formulas.push_back(L"cos #2 @39");
            Formulas.push_back(L"sin #2 @39");
            Formulas.push_back(L"sum @40 10800 0");
            Formulas.push_back(L"sum @41 10800 0");
            Formulas.push_back(L"sum @42 10800 0");
            Formulas.push_back(L"sum @43 10800 0");
            Formulas.push_back(L"sum @35 10800 0");
            Formulas.push_back(L"sum @36 10800 0");

            AdjustmentValues	=	L"-11796480,,5400";
            ConnectorLocations	=	L"@44,@45;@48,@49;@46,@47;@17,@18;@24,@25;@15,@16";
            TextBoxRectangle	=	L"3163,3163,18437,18437";

			Handle one;
            one.position		=	L"@3,#0";
            one.polar			=	L"10800,10800";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#1";
            two.polar			=	L"10800,10800";
            two.radiusrange		=	L"0,10800";
			Handles.push_back (two);
		}
	};
}
