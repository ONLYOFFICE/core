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
	class StarType : public ShapeType
	{
	public: 
		StarType () : ShapeType(msosptStar)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;

            Path				=	L"m10800,l8280,8259,,8259r6720,5146l4200,21600r6600,-5019l17400,21600,14880,13405,21600,8259r-8280,xe";
            ConnectorLocations	=	L"10800,0;0,8259;4200,21600;17400,21600;21600,8259";
            TextBoxRectangle	=	L"6720,8259,14880,15628";
		}

		virtual ~StarType()
		{
		}
	};

	class IrregularSeal1 : public ShapeType
	{
	public:

		IrregularSeal1 () : ShapeType (msosptIrregularSeal1)
		{
			ShapeConcentricFill		=	false;
			Joins					=	miter;
            Path					=	L"m10800,5800l8352,2295,7312,6320,370,2295,4627,7617,,8615r3722,3160l135,14587r5532,-650l4762,17617,7715,15627r770,5973l10532,14935r2715,4802l14020,14457r4125,3638l16837,12942r4763,348l17607,10475,21097,8137,16702,7315,18380,4457r-4225,868l14522,xe";

            ConnectorLocations		=	L"14522,0;0,8615;8485,21600;21600,13290";
            ConnectorAngles			=	L"270,180,90,0";
            TextBoxRectangle		=	L"4627,6320,16702,13937";
		}
	};
	class IrregularSeal2 : public ShapeType
	{
	public:

		IrregularSeal2 () : ShapeType (msosptIrregularSeal2)
		{
			ShapeConcentricFill		=	false;
			Joins					=	miter;

            Path					=	L"m11462,4342l9722,1887,8550,6382,4502,3625r870,4192l1172,8270r2763,3322l,12877r3330,2493l1285,17825r3520,415l4917,21600,7527,18125r1173,1587l9872,17370r1740,1472l12180,15935r2762,1435l14640,14350r4237,1282l16380,12310r1890,-1020l16985,9402,21600,6645,16380,6532,18007,3172,14525,5777,14790,xe";
            ConnectorLocations		=	L"9722,1887;0,12877;11612,18842;21600,6645";
            ConnectorAngles			=	L"270,180,90,0";
            TextBoxRectangle		=	L"5372,6382,14640,15935";
		}
	};

	class Seal4 : public ShapeType
	{
	public:

		Seal4 () : ShapeType (msosptSeal4)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;

            Path					=	L"m21600,10800l@2@3,10800,0@3@3,,10800@3@2,10800,21600@2@2xe";

            Formulas.push_back(L"sum 10800 0 #0");
            Formulas.push_back(L"prod @0 23170 32768");
            Formulas.push_back(L"sum @1 10800 0");
            Formulas.push_back(L"sum 10800 0 @1");

            ConnectorLocations		=	L"Rectangle";
            TextBoxRectangle		=	L"@3,@3,@2,@2";

            AdjustmentValues		=	L"8100";

			Handle one;
            one.position			=	L"#0,center";
            one.xrange				=	L"0,10800";
			Handles.push_back (one);
		}
	};
	class Seal8 : public ShapeType
	{
	public:

		Seal8 () : ShapeType (msosptSeal8)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;

            Path					=	L"m21600,10800l@3@6,18436,3163@4@5,10800,0@6@5,3163,3163@5@6,,10800@5@4,3163,18436@6@3,10800,21600@4@3,18436,18436@3@4xe";

            Formulas.push_back(L"sum 10800 0 #0");
            Formulas.push_back(L"prod @0 30274 32768");
            Formulas.push_back(L"prod @0 12540 32768");
            Formulas.push_back(L"sum @1 10800 0");
            Formulas.push_back(L"sum @2 10800 0");
            Formulas.push_back(L"sum 10800 0 @1");
            Formulas.push_back(L"sum 10800 0 @2");
            Formulas.push_back(L"prod @0 23170 32768");
            Formulas.push_back(L"sum @7 10800 0");
            Formulas.push_back(L"sum 10800 0 @7");

            ConnectorLocations		=	L"Rectangle";
            TextBoxRectangle		=	L"@9,@9,@8,@8";

            AdjustmentValues		=	L"2538";

			Handle one;
            one.position			=	L"#0,center";
            one.xrange				=	L"0,10800";
			Handles.push_back (one);
		}
	};

	class Seal16 : public ShapeType
	{
	public:

		Seal16 () : ShapeType (msosptSeal16)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;

            Path					=	L"m21600,10800l@5@10,20777,6667@7@12,18436,3163@8@11,14932,822@6@9,10800,0@10@9,6667,822@12@11,3163,3163@11@12,822,6667@9@10,,10800@9@6,822,14932@11@8,3163,18436@12@7,6667,20777@10@5,10800,21600@6@5,14932,20777@8@7,18436,18436@7@8,20777,14932@5@6xe";

            Formulas.push_back(L"sum 10800 0 #0");
            Formulas.push_back(L"prod @0 32138 32768");
            Formulas.push_back(L"prod @0 6393 32768");
            Formulas.push_back(L"prod @0 27246 32768");
            Formulas.push_back(L"prod @0 18205 32768");
            Formulas.push_back(L"sum @1 10800 0");
            Formulas.push_back(L"sum @2 10800 0");
            Formulas.push_back(L"sum @3 10800 0");
            Formulas.push_back(L"sum @4 10800 0");
            Formulas.push_back(L"sum 10800 0 @1");
            Formulas.push_back(L"sum 10800 0 @2");
            Formulas.push_back(L"sum 10800 0 @3");
            Formulas.push_back(L"sum 10800 0 @4");
            Formulas.push_back(L"prod @0 23170 32768");
            Formulas.push_back(L"sum @13 10800 0");
            Formulas.push_back(L"sum 10800 0 @13");

            ConnectorLocations		=	L"Rectangle";
            TextBoxRectangle		=	L"@15,@15,@14,@14";

            AdjustmentValues		=	L"2700";

			Handle one;
            one.position			=	L"#0,center";
            one.xrange				=	L"0,10800";
			Handles.push_back (one);
		}
	};
	class Seal24 : public ShapeType
	{
	public:

		Seal24 () : ShapeType (msosptSeal24)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;

            Path					=	L"m21600,10800l@7@14,21232,8005@9@16,20153,5400@11@18,18437,3163@12@17,16200,1447@10@15,13595,368@8@13,10800,0@14@13,8005,368@16@15,5400,1447@18@17,3163,3163@17@18,1447,5400@15@16,368,8005@13@14,,10800@13@8,368,13595@15@10,1447,16200@17@12,3163,18437@18@11,5400,20153@16@9,8005,21232@14@7,10800,21600@8@7,13595,21232@10@9,16200,20153@12@11,18437,18437@11@12,20153,16200@9@10,21232,13595@7@8xe";

            Formulas.push_back(L"sum 10800 0 #0");
            Formulas.push_back(L"prod @0 32488 32768");
            Formulas.push_back(L"prod @0 4277 32768");
            Formulas.push_back(L"prod @0 30274 32768");
            Formulas.push_back(L"prod @0 12540 32768");
            Formulas.push_back(L"prod @0 25997 32768");
            Formulas.push_back(L"prod @0 19948 32768");
            Formulas.push_back(L"sum @1 10800 0");
            Formulas.push_back(L"sum @2 10800 0");
            Formulas.push_back(L"sum @3 10800 0");
            Formulas.push_back(L"sum @4 10800 0");
            Formulas.push_back(L"sum @5 10800 0");
            Formulas.push_back(L"sum @6 10800 0");
            Formulas.push_back(L"sum 10800 0 @1");
            Formulas.push_back(L"sum 10800 0 @2");
            Formulas.push_back(L"sum 10800 0 @3");
            Formulas.push_back(L"sum 10800 0 @4");
            Formulas.push_back(L"sum 10800 0 @5");
            Formulas.push_back(L"sum 10800 0 @6");
            Formulas.push_back(L"prod @0 23170 32768");
            Formulas.push_back(L"sum @19 10800 0");
            Formulas.push_back(L"sum 10800 0 @19");

            ConnectorLocations		=	L"Rectangle";
            TextBoxRectangle		=	L"@21,@21,@20,@20";

            AdjustmentValues		=	L"2700";

			Handle one;
            one.position			=	L"#0,center";
            one.xrange				=	L"0,10800";
			Handles.push_back (one);
		}
	};
	class Seal32 : public ShapeType
	{
	public:

		Seal32 () : ShapeType (msosptSeal32)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;

            Path					=	L"m21600,10800l@9@18,21392,8693@11@20,20777,6667@13@22,19780,4800@15@24,18436,3163@16@23,16800,1820@14@21,14932,822@12@19,12907,208@10@17,10800,0@18@17,8693,208@20@19,6667,822@22@21,4800,1820@24@23,3163,3163@23@24,1820,4800@21@22,822,6667@19@20,208,8693@17@18,,10800@17@10,208,12907@19@12,822,14932@21@14,1820,16800@23@16,3163,18436@24@15,4800,19780@22@13,6667,20777@20@11,8693,21392@18@9,10800,21600@10@9,12907,21392@12@11,14932,20777@14@13,16800,19780@16@15,18436,18436@15@16,19780,16800@13@14,20777,14932@11@12,21392,12907@9@10xe";

            Formulas.push_back(L"sum 10800 0 #0");
            Formulas.push_back(L"prod @0 32610 32768");
            Formulas.push_back(L"prod @0 3212 32768");
            Formulas.push_back(L"prod @0 31357 32768");
            Formulas.push_back(L"prod @0 9512 32768");
            Formulas.push_back(L"prod @0 28899 32768");
            Formulas.push_back(L"prod @0 15447 32768");
            Formulas.push_back(L"prod @0 25330 32768");
            Formulas.push_back(L"prod @0 20788 32768");
            Formulas.push_back(L"sum @1 10800 0");
            Formulas.push_back(L"sum @2 10800 0");
            Formulas.push_back(L"sum @3 10800 0");
            Formulas.push_back(L"sum @4 10800 0");
            Formulas.push_back(L"sum @5 10800 0");
            Formulas.push_back(L"sum @6 10800 0");
            Formulas.push_back(L"sum @7 10800 0");
            Formulas.push_back(L"sum @8 10800 0");
            Formulas.push_back(L"sum 10800 0 @1");
            Formulas.push_back(L"sum 10800 0 @2");
            Formulas.push_back(L"sum 10800 0 @3");
            Formulas.push_back(L"sum 10800 0 @4");
            Formulas.push_back(L"sum 10800 0 @5");
            Formulas.push_back(L"sum 10800 0 @6");
            Formulas.push_back(L"sum 10800 0 @7");
            Formulas.push_back(L"sum 10800 0 @8");
            Formulas.push_back(L"prod @0 23170 32768");
            Formulas.push_back(L"sum @25 10800 0");
            Formulas.push_back(L"sum 10800 0 @25");

            ConnectorLocations		=	L"Rectangle";
            TextBoxRectangle		=	L"@27,@27,@26,@26";

            AdjustmentValues		=	L"2700";

			Handle one;
            one.position			=	L"#0,center";
            one.xrange				=	L"0,10800";
			Handles.push_back (one);
		}
	};

	class Ribbon2 : public ShapeType
	{
	public:

		Ribbon2 () : ShapeType (msosptRibbon2)
		{
			ShapeConcentricFill		=	false;
			Joins					=	miter;

            Path					=	L"m0@29l@3@29qx@4@19l@4@10@5@10@5@19qy@6@29l@28@29@26@22@28@23@9@23@9@24qy@8,l@1,qx@0@24l@0@23,0@23,2700@22xem@4@19nfqy@3@20l@1@20qx@0@21@1@10l@4@10em@5@19nfqy@6@20l@8@20qx@9@21@8@10l@5@10em@0@21nfl@0@23em@9@21nfl@9@23e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum @0 675 0");
            Formulas.push_back(L"sum @1 675 0");
            Formulas.push_back(L"sum @2 675 0");
            Formulas.push_back(L"sum @3 675 0");
            Formulas.push_back(L"sum width 0 @4");
            Formulas.push_back(L"sum width 0 @3");
            Formulas.push_back(L"sum width 0 @2");
            Formulas.push_back(L"sum width 0 @1");
            Formulas.push_back(L"sum width 0 @0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"prod @10 1 4");
            Formulas.push_back(L"prod @10 1 2");
            Formulas.push_back(L"prod @10 3 4");
            Formulas.push_back(L"prod height 3 4");
            Formulas.push_back(L"prod height 1 2");
            Formulas.push_back(L"prod height 1 4");
            Formulas.push_back(L"prod height 3 2");
            Formulas.push_back(L"prod height 2 3");
            Formulas.push_back(L"sum @11 @14 0");
            Formulas.push_back(L"sum @12 @15 0");
            Formulas.push_back(L"sum @13 @16 0");
            Formulas.push_back(L"sum @17 0 @20");
            Formulas.push_back(L"sum height 0 @10");
            Formulas.push_back(L"sum height 0 @19");
            Formulas.push_back(L"prod width 1 2");
            Formulas.push_back(L"sum width 0 2700");
            Formulas.push_back(L"sum @25 0 2700");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");

            ConnectorLocations		=	L"@25,0;2700,@22;@25,@10;@26,@22";
            TextBoxRectangle		=	L"@0,0,@9,@10";

            ConnectorAngles			=	L"270,180,90,0";

            AdjustmentValues		=	L"5400,18900";

			Handle one;
            one.position			=	L"#0,center";
            one.xrange				=	L"2700,8100";
			Handles.push_back (one);

			Handle two;
            two.position			=	L"center,#1";
            two.yrange				=	L"14400,21600";
			Handles.push_back(two);
		}
	};

	class Ribbon : public ShapeType
	{
	public:

		Ribbon () : ShapeType (msosptRibbon)
		{
			ShapeConcentricFill		=	false;
			Joins					=	miter;

            Path					=	L"m,l@3,qx@4@11l@4@10@5@10@5@11qy@6,l@21,0@19@15@21@16@9@16@9@17qy@8@22l@1@22qx@0@17l@0@16,0@16,2700@15xem@4@11nfqy@3@12l@1@12qx@0@13@1@10l@4@10em@5@11nfqy@6@12l@8@12qx@9@13@8@10l@5@10em@0@13nfl@0@16em@9@13nfl@9@16e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum @0 675 0");
            Formulas.push_back(L"sum @1 675 0");
            Formulas.push_back(L"sum @2 675 0");
            Formulas.push_back(L"sum @3 675 0");
            Formulas.push_back(L"sum width 0 @4");
            Formulas.push_back(L"sum width 0 @3");
            Formulas.push_back(L"sum width 0 @2");
            Formulas.push_back(L"sum width 0 @1");
            Formulas.push_back(L"sum width 0 @0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"prod @10 1 4");
            Formulas.push_back(L"prod @11 2 1");
            Formulas.push_back(L"prod @11 3 1");
            Formulas.push_back(L"prod height 1 2");
            Formulas.push_back(L"sum @14 0 @12");
            Formulas.push_back(L"sum height 0 @10");
            Formulas.push_back(L"sum height 0 @11");
            Formulas.push_back(L"prod width 1 2");
            Formulas.push_back(L"sum width 0 2700");
            Formulas.push_back(L"sum @18 0 2700");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");

            ConnectorLocations		=	L"@18,@10;2700,@15;@18,21600;@19,@15";
            TextBoxRectangle		=	L"@0,@10,@9,21600";

            ConnectorAngles			=	L"270,180,90,0";

            AdjustmentValues		=	L"5400,2700";

			Handle one;
            one.position			=	L"#0,bottomRight";
            one.xrange				=	L"2700,8100";
			Handles.push_back (one);

			Handle two;
            two.position			=	L"center,#1";
            two.yrange				=	L"0,72000";
			Handles.push_back(two);
		}
	};

	class EllipseRibbon2 : public ShapeType
	{
	public:

		EllipseRibbon2 () : ShapeType (msosptEllipseRibbon2)
		{
			ShapeConcentricFill		=	false;
			Joins					=	miter;

            Path					=	L"wr@9@34@8@35,0@24@0@23@9,0@8@11@0@22@19@22@9@34@8@35@19@23@3@24l@7@36@3@4at@9@31@8@32@3@4@18@30@9@1@8@33@18@28@17@28@9@31@8@32@17@30,0@4l@5@36xear@9@1@8@33@17@28@0@29nfl@17@30ewr@9@1@8@33@18@28@19@29nfl@18@30em@0@23nfl@0@29em@19@23nfl@19@29e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"prod width 1 8");
            Formulas.push_back(L"prod width 1 2");
            Formulas.push_back(L"prod width 7 8");
            Formulas.push_back(L"prod width 3 2");
            Formulas.push_back(L"sum 0 0 @6");
            Formulas.push_back(L"prod #2 30573 4096");
            Formulas.push_back(L"prod @10 2 1");
            Formulas.push_back(L"sum @10 height #2");
            Formulas.push_back(L"sum @10 #1 0");
            Formulas.push_back(L"prod #1 1 2");
            Formulas.push_back(L"sum @10 @14 0");
            Formulas.push_back(L"sum @12 0 #1");
            Formulas.push_back(L"sum #0 @5 0");
            Formulas.push_back(L"sum width 0 @17");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"sum @6 0 #0");
            Formulas.push_back(L"ellipse @20 width @10");
            Formulas.push_back(L"sum @10 0 @21");
            Formulas.push_back(L"sum @22 @16 @10");
            Formulas.push_back(L"sum #2 @16 @10");
            Formulas.push_back(L"prod @10 2391 32768");
            Formulas.push_back(L"sum @6 0 @17");
            Formulas.push_back(L"ellipse @26 width @10");
            Formulas.push_back(L"sum @10 #1 @27");
            Formulas.push_back(L"sum @22 #1 0");
            Formulas.push_back(L"sum @12 0 @27");
            Formulas.push_back(L"sum height 0 #2");
            Formulas.push_back(L"sum @10 @12 0");
            Formulas.push_back(L"sum @32 @10 @16");
            Formulas.push_back(L"sum @31 @10 @13");
            Formulas.push_back(L"sum @32 @10 @13");
            Formulas.push_back(L"sum @25 @12 @15");
            Formulas.push_back(L"sum @16 0 @15");
            Formulas.push_back(L"prod @37 2 3");
            Formulas.push_back(L"sum @1 @38 0");
            Formulas.push_back(L"sum #2 @38 0");
            Formulas.push_back(L"max @40 675");
            Formulas.push_back(L"prod width 3 8");
            Formulas.push_back(L"sum @42 0 4");

            AdjustmentValues		=	L"5400,16200,2700";
            ConnectorLocations		=	L"@6,0;@5,@36;@6,@1;@7,@36";
            ConnectorAngles			=	L"270,180,90,0";
            TextBoxRectangle		=	L"@0,@22,@19,@1";

			Handle one;
            one.position			=	L"#0,topLeft";
            one.xrange				=	L"@5,@43";
			Handles.push_back (one);

			Handle two;
            two.position			=	L"center,#1";
            two.yrange				=	L"@39,@31";
			Handles.push_back(two);

			Handle three;
            three.position			=	L"topLeft,#2";
            three.yrange			=	L"@41,@24";
			Handles.push_back(three);
		}
	};

	class EllipseRibbon : public ShapeType
	{
	public:

		EllipseRibbon () : ShapeType (msosptEllipseRibbon)
		{
			ShapeConcentricFill		=	false;
			Joins					=	miter;

            Path					=	L"ar@9@38@8@37,0@27@0@26@9@13@8@4@0@25@22@25@9@38@8@37@22@26@3@27l@7@40@3,wa@9@35@8@10@3,0@21@33@9@36@8@1@21@31@20@31@9@35@8@10@20@33,,l@5@40xewr@9@36@8@1@20@31@0@32nfl@20@33ear@9@36@8@1@21@31@22@32nfl@21@33em@0@26nfl@0@32em@22@26nfl@22@32e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"prod width 1 8");
            Formulas.push_back(L"prod width 1 2");
            Formulas.push_back(L"prod width 7 8");
            Formulas.push_back(L"prod width 3 2");
            Formulas.push_back(L"sum 0 0 @6");
            Formulas.push_back(L"sum height 0 #2");
            Formulas.push_back(L"prod @10 30573 4096");
            Formulas.push_back(L"prod @11 2 1");
            Formulas.push_back(L"sum height 0 @12");
            Formulas.push_back(L"sum @11 #2 0");
            Formulas.push_back(L"sum @11 height #1");
            Formulas.push_back(L"sum height 0 #1");
            Formulas.push_back(L"prod @16 1 2");
            Formulas.push_back(L"sum @11 @17 0");
            Formulas.push_back(L"sum @14 #1 height");
            Formulas.push_back(L"sum #0 @5 0");
            Formulas.push_back(L"sum width 0 @20");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"sum @6 0 #0");
            Formulas.push_back(L"ellipse @23 width @11");
            Formulas.push_back(L"sum @24 height @11");
            Formulas.push_back(L"sum @25 @11 @19");
            Formulas.push_back(L"sum #2 @11 @19");
            Formulas.push_back(L"prod @11 2391 32768");
            Formulas.push_back(L"sum @6 0 @20");
            Formulas.push_back(L"ellipse @29 width @11");
            Formulas.push_back(L"sum #1 @30 @11");
            Formulas.push_back(L"sum @25 #1 height");
            Formulas.push_back(L"sum height @30 @14");
            Formulas.push_back(L"sum @11 @14 0");
            Formulas.push_back(L"sum height 0 @34");
            Formulas.push_back(L"sum @35 @19 @11");
            Formulas.push_back(L"sum @10 @15 @11");
            Formulas.push_back(L"sum @35 @15 @11");
            Formulas.push_back(L"sum @28 @14 @18");
            Formulas.push_back(L"sum height 0 @39");
            Formulas.push_back(L"sum @19 0 @18");
            Formulas.push_back(L"prod @41 2 3");
            Formulas.push_back(L"sum #1 0 @42");
            Formulas.push_back(L"sum #2 0 @42");
            Formulas.push_back(L"min @44 20925");
            Formulas.push_back(L"prod width 3 8");
            Formulas.push_back(L"sum @46 0 4");

            AdjustmentValues		=	L"5400,5400,18900";
            ConnectorLocations		=	L"@6,@1;@5,@40;@6,@4;@7,@40";
            ConnectorAngles			=	L"270,180,90,0";
            TextBoxRectangle		=	L"@0,@1,@22,@25";

			Handle one;
            one.position			=	L"#0,bottomRight";
            one.xrange				=	L"@5,@47";
			Handles.push_back (one);

			Handle two;
            two.position			=	L"center,#1";
            two.yrange				=	L"@10,@43";
			Handles.push_back(two);

			Handle three;
            three.position			=	L"topLeft,#2";
            three.yrange			=	L"@27,@45";
			Handles.push_back(three);
		}
	};

	class VerticalScroll : public ShapeType
	{
	public:

		VerticalScroll () : ShapeType (msosptVerticalScroll)
		{
			ShapeConcentricFill		=	false;
			Joins					=	round;

            Path					=	L"m@5,qx@1@2l@1@0@2@0qx0@7@2,21600l@9,21600qx@10@7l@10@1@11@1qx21600@2@11,xem@5,nfqx@6@2@5@1@4@3@5@2l@6@2em@5@1nfl@10@1em@2,21600nfqx@1@7l@1@0em@2@0nfqx@3@8@2@7l@1@7e";

            Formulas.push_back(L"sum height 0 #0");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"prod @1 1 2");
            Formulas.push_back(L"prod @1 3 4");
            Formulas.push_back(L"prod @1 5 4");
            Formulas.push_back(L"prod @1 3 2");
            Formulas.push_back(L"prod @1 2 1");
            Formulas.push_back(L"sum height 0 @2");
            Formulas.push_back(L"sum height 0 @3");
            Formulas.push_back(L"sum width 0 @5");
            Formulas.push_back(L"sum width 0 @1");
            Formulas.push_back(L"sum width 0 @2");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"prod height 1 2");
            Formulas.push_back(L"prod width 1 2");

            AdjustmentValues		=	L"2700";
            ConnectorLocations		=	L"@14,0;@1,@13;@14,@12;@10,@13";
            ConnectorAngles			=	L"270,180,90,0";
            TextBoxRectangle		=	L"@1,@1,@10,@7";
            Limo					=	L"10800,10800";

			Handle one;
            one.position			=	L"topLeft,#0";
            one.yrange				=	L"0,5400";
			Handles.push_back (one);
		}
	};

	class HorizontalScroll : public ShapeType
	{
	public:

		HorizontalScroll () : ShapeType (msosptHorizontalScroll)
		{
			ShapeConcentricFill		=	false;
			Joins					=	round;

            Path					=	L"m0@5qy@2@1l@0@1@0@2qy@7,,21600@2l21600@9qy@7@10l@1@10@1@11qy@2,21600,0@11xem0@5nfqy@2@6@1@5@3@4@2@5l@2@6em@1@5nfl@1@10em21600@2nfqy@7@1l@0@1em@0@2nfqy@8@3@7@2l@7@1e";

            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"prod @1 1 2");
            Formulas.push_back(L"prod @1 3 4");
            Formulas.push_back(L"prod @1 5 4");
            Formulas.push_back(L"prod @1 3 2");
            Formulas.push_back(L"prod @1 2 1");
            Formulas.push_back(L"sum width 0 @2");
            Formulas.push_back(L"sum width 0 @3");
            Formulas.push_back(L"sum height 0 @5");
            Formulas.push_back(L"sum height 0 @1");
            Formulas.push_back(L"sum height 0 @2");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"prod width 1 2");
            Formulas.push_back(L"prod height 1 2");

            AdjustmentValues		=	L"2700";
            ConnectorLocations		=	L"@13,@1;0,@14;@13,@10;@12,@14";
            ConnectorAngles			=	L"270,180,90,0";
            TextBoxRectangle		=	L"@1,@1,@7,@10";
            Limo					=	L"10800,10800";

			Handle one;
            one.position			=	L"#0,topLeft";
            one.yrange				=	L"0,5400";
			Handles.push_back (one);
		}
	};

	class Wave : public ShapeType
	{
	public:

		Wave () : ShapeType (msosptWave)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;

            Path					=	L"m@28@0c@27@1@26@3@25@0l@21@4c@22@5@23@6@24@4xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"prod @0 41 9");
            Formulas.push_back(L"prod @0 23 9");
            Formulas.push_back(L"sum 0 0 @2");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 @1");
            Formulas.push_back(L"sum 21600 0 @3");
            Formulas.push_back(L"sum #1 0 10800");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"prod @8 2 3");
            Formulas.push_back(L"prod @8 4 3");
            Formulas.push_back(L"prod @8 2 1");
            Formulas.push_back(L"sum 21600 0 @9");
            Formulas.push_back(L"sum 21600 0 @10");
            Formulas.push_back(L"sum 21600 0 @11");
            Formulas.push_back(L"prod #1 2 3");
            Formulas.push_back(L"prod #1 4 3");
            Formulas.push_back(L"prod #1 2 1");
            Formulas.push_back(L"sum 21600 0 @15");
            Formulas.push_back(L"sum 21600 0 @16");
            Formulas.push_back(L"sum 21600 0 @17");
            Formulas.push_back(L"if @7 @14 0");
            Formulas.push_back(L"if @7 @13 @15");
            Formulas.push_back(L"if @7 @12 @16");
            Formulas.push_back(L"if @7 21600 @17");
            Formulas.push_back(L"if @7 0 @20");
            Formulas.push_back(L"if @7 @9 @19");
            Formulas.push_back(L"if @7 @10 @18");
            Formulas.push_back(L"if @7 @11 21600");
            Formulas.push_back(L"sum @24 0 @21");
            Formulas.push_back(L"sum @4 0 @0");
            Formulas.push_back(L"max @21 @25");
            Formulas.push_back(L"min @24 @28");
            Formulas.push_back(L"prod @0 2 1");
            Formulas.push_back(L"sum 21600 0 @33");
            Formulas.push_back(L"mid @26 @27");
            Formulas.push_back(L"mid @24 @28");
            Formulas.push_back(L"mid @22 @23");
            Formulas.push_back(L"mid @21 @25");

            AdjustmentValues		=	L"2809,10800";
            ConnectorLocations		=	L"@35,@0;@38,10800;@37,@4;@36,10800";
            ConnectorAngles			=	L"270,180,90,0";
            TextBoxRectangle		=	L"@31,@33,@32,@34";

			Handle one;
            one.position			=	L"topLeft,#0";
            one.yrange				=	L"0,4459";
			Handles.push_back (one);
		
			Handle two;
            two.position			=	L"#1,bottomRight";
            two.yrange				=	L"8640,12960";
			Handles.push_back (two);
		}
	};	

	class DoubleWave : public ShapeType
	{
	public:

		DoubleWave () : ShapeType (msosptDoubleWave)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;

            Path					=	L"m@43@0c@42@1@41@3@40@0@39@1@38@3@37@0l@30@4c@31@5@32@6@33@4@34@5@35@6@36@4xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"prod @0 41 9");
            Formulas.push_back(L"prod @0 23 9");
            Formulas.push_back(L"sum 0 0 @2");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 @1");
            Formulas.push_back(L"sum 21600 0 @3");
            Formulas.push_back(L"sum #1 0 10800");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"prod @8 1 3");
            Formulas.push_back(L"prod @8 2 3");
            Formulas.push_back(L"prod @8 4 3");
            Formulas.push_back(L"prod @8 5 3");
            Formulas.push_back(L"prod @8 2 1");
            Formulas.push_back(L"sum 21600 0 @9");
            Formulas.push_back(L"sum 21600 0 @10");
            Formulas.push_back(L"sum 21600 0 @8");
            Formulas.push_back(L"sum 21600 0 @11");
            Formulas.push_back(L"sum 21600 0 @12");
            Formulas.push_back(L"sum 21600 0 @13");
            Formulas.push_back(L"prod #1 1 3");
            Formulas.push_back(L"prod #1 2 3");
            Formulas.push_back(L"prod #1 4 3");
            Formulas.push_back(L"prod #1 5 3");
            Formulas.push_back(L"prod #1 2 1");
            Formulas.push_back(L"sum 21600 0 @20");
            Formulas.push_back(L"sum 21600 0 @21");
            Formulas.push_back(L"sum 21600 0 @22");
            Formulas.push_back(L"sum 21600 0 @23");
            Formulas.push_back(L"sum 21600 0 @24");
            Formulas.push_back(L"if @7 @19 0");
            Formulas.push_back(L"if @7 @18 @20");
            Formulas.push_back(L"if @7 @17 @21");
            Formulas.push_back(L"if @7 @16 #1");
            Formulas.push_back(L"if @7 @15 @22");
            Formulas.push_back(L"if @7 @14 @23");
            Formulas.push_back(L"if @7 21600 @24");
            Formulas.push_back(L"if @7 0 @29");
            Formulas.push_back(L"if @7 @9 @28");
            Formulas.push_back(L"if @7 @10 @27");
            Formulas.push_back(L"if @7 @8 @8");
            Formulas.push_back(L"if @7 @11 @26");
            Formulas.push_back(L"if @7 @12 @25");
            Formulas.push_back(L"if @7 @13 21600");
            Formulas.push_back(L"sum @36 0 @30");
            Formulas.push_back(L"sum @4 0 @0");
            Formulas.push_back(L"max @30 @37");
            Formulas.push_back(L"min @36 @43");
            Formulas.push_back(L"prod @0 2 1");
            Formulas.push_back(L"sum 21600 0 @48");
            Formulas.push_back(L"mid @36 @43");
            Formulas.push_back(L"mid @30 @37");

            AdjustmentValues		=	L"1404,10800";
            ConnectorLocations		=	L"@40,@0;@51,10800;@33,@4;@50,10800";
            ConnectorAngles			=	L"270,180,90,0";
            TextBoxRectangle		=	L"@46,@48,@47,@49";

			Handle one;
            one.position			=	L"topLeft,#0";
            one.yrange				=	L"0,2229";
			Handles.push_back (one);
			
			Handle two;
            two.position			=	L"#1,bottomRight";
            two.xrange				=	L"8640,12960";
			Handles.push_back (two);
		}
	};	
}
