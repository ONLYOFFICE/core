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
	class WedgeRectCallout : public ShapeType
	{
	public:
		WedgeRectCallout () : ShapeType(msosptWedgeRectCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m,l0@8@12@24,0@9,,21600@6,21600@15@27@7,21600,21600,21600,21600@9@18@30,21600@8,21600,0@7,0@21@33@6,xe";

            Formulas.push_back(L"sum 10800 0 #0");
            Formulas.push_back(L"sum 10800 0 #1");
            Formulas.push_back(L"sum #0 0 #1");
            Formulas.push_back(L"sum @0 @1 0");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"if @0 3600 12600");
            Formulas.push_back(L"if @0 9000 18000");
            Formulas.push_back(L"if @1 3600 12600");
            Formulas.push_back(L"if @1 9000 18000");
            Formulas.push_back(L"if @2 0 #0");
            Formulas.push_back(L"if @3 @10 0");
            Formulas.push_back(L"if #0 0 @11");
            Formulas.push_back(L"if @2 @6 #0");
            Formulas.push_back(L"if @3 @6 @13");
            Formulas.push_back(L"if @5 @6 @14");
            Formulas.push_back(L"if @2 #0 21600");
            Formulas.push_back(L"if @3 21600 @16");
            Formulas.push_back(L"if @4 21600 @17");
            Formulas.push_back(L"if @2 #0 @6");
            Formulas.push_back(L"if @3 @19 @6");
            Formulas.push_back(L"if #1 @6 @20");
            Formulas.push_back(L"if @2 @8 #1");
            Formulas.push_back(L"if @3 @22 @8");
            Formulas.push_back(L"if #0 @8 @23");
            Formulas.push_back(L"if @2 21600 #1");
            Formulas.push_back(L"if @3 21600 @25");
            Formulas.push_back(L"if @5 21600 @26");
            Formulas.push_back(L"if @2 #1 @8");
            Formulas.push_back(L"if @3 @8 @28");
            Formulas.push_back(L"if @4 @8 @29");
            Formulas.push_back(L"if @2 #1 0");
            Formulas.push_back(L"if @3 @31 0");
            Formulas.push_back(L"if #1 0 @32");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");

            AdjustmentValues	=	L"1350,25920";
            ConnectorLocations	=	L"10800,0;0,10800;10800,21600;21600,10800;@34,@35";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);
		}
	};

	class WedgeRRectCallout : public ShapeType
	{
	public:
		WedgeRRectCallout () : ShapeType(msosptWedgeRRectCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"m3600,qx,3600l0@8@12@24,0@9,,18000qy3600,21600l@6,21600@15@27@7,21600,18000,21600qx21600,18000l21600@9@18@30,21600@8,21600,3600qy18000,l@7,0@21@33@6,xe";

            Formulas.push_back(L"sum 10800 0 #0");
            Formulas.push_back(L"sum 10800 0 #1");
            Formulas.push_back(L"sum #0 0 #1");
            Formulas.push_back(L"sum @0 @1 0");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"sum 21600 0 #1");
            Formulas.push_back(L"if @0 3600 12600");
            Formulas.push_back(L"if @0 9000 18000");
            Formulas.push_back(L"if @1 3600 12600");
            Formulas.push_back(L"if @1 9000 18000");
            Formulas.push_back(L"if @2 0 #0");
            Formulas.push_back(L"if @3 @10 0");
            Formulas.push_back(L"if #0 0 @11");
            Formulas.push_back(L"if @2 @6 #0");
            Formulas.push_back(L"if @3 @6 @13");
            Formulas.push_back(L"if @5 @6 @14");
            Formulas.push_back(L"if @2 #0 21600");
            Formulas.push_back(L"if @3 21600 @16");
            Formulas.push_back(L"if @4 21600 @17");
            Formulas.push_back(L"if @2 #0 @6");
            Formulas.push_back(L"if @3 @19 @6");
            Formulas.push_back(L"if #1 @6 @20");
            Formulas.push_back(L"if @2 @8 #1");
            Formulas.push_back(L"if @3 @22 @8");
            Formulas.push_back(L"if #0 @8 @23");
            Formulas.push_back(L"if @2 21600 #1");
            Formulas.push_back(L"if @3 21600 @25");
            Formulas.push_back(L"if @5 21600 @26");
            Formulas.push_back(L"if @2 #1 @8");
            Formulas.push_back(L"if @3 @8 @28");
            Formulas.push_back(L"if @4 @8 @29");
            Formulas.push_back(L"if @2 #1 0");
            Formulas.push_back(L"if @3 @31 0");
            Formulas.push_back(L"if #1 0 @32");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");

            AdjustmentValues	=	L"1350,25920";
            ConnectorLocations	=	L"10800,0;0,10800;10800,21600;21600,10800;@34,@35";
            TextBoxRectangle	=	L"791,791,20809,20809";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);
		}
	};

	class WedgeEllipseCallout : public ShapeType
	{
	public:
		WedgeEllipseCallout () : ShapeType(msosptWedgeEllipseCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"wr,,21600,21600@15@16@17@18l@21@22xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"sum 10800 0 #0");
            Formulas.push_back(L"sum 10800 0 #1");
            Formulas.push_back(L"atan2 @2 @3");
            Formulas.push_back(L"sumangle @4 11 0");
            Formulas.push_back(L"sumangle @4 0 11");
            Formulas.push_back(L"cos 10800 @4");
            Formulas.push_back(L"sin 10800 @4");
            Formulas.push_back(L"cos 10800 @5");
            Formulas.push_back(L"sin 10800 @5");
            Formulas.push_back(L"cos 10800 @6");
            Formulas.push_back(L"sin 10800 @6");
            Formulas.push_back(L"sum 10800 0 @7");
            Formulas.push_back(L"sum 10800 0 @8");
            Formulas.push_back(L"sum 10800 0 @9");
            Formulas.push_back(L"sum 10800 0 @10");
            Formulas.push_back(L"sum 10800 0 @11");
            Formulas.push_back(L"sum 10800 0 @12");
            Formulas.push_back(L"mod @2 @3 0");
            Formulas.push_back(L"sum @19 0 10800");
            Formulas.push_back(L"if @20 #0 @13");
            Formulas.push_back(L"if @20 #1 @14");

            AdjustmentValues	=	L"1350,25920";
            ConnectorLocations	=	L"10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163;@21,@22";
            TextBoxRectangle	=	L"3163,3163,18437,18437";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);
		}
	};

	class CloudCallout : public ShapeType
	{
	public:
		CloudCallout () : ShapeType(msosptCloudCallout)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
            Path				=	L"ar,7165,4345,13110,1950,7185,1080,12690,475,11732,4835,17650,1080,12690,2910,17640,2387,9757,10107,20300,2910,17640,8235,19545,7660,12382,14412,21597,8235,19545,14280,18330,12910,11080,18695,18947,14280,18330,18690,15045,14822,5862,21597,15082,18690,15045,20895,7665,15772,2592,21105,9865,20895,7665,19140,2715,14330,,19187,6595,19140,2715,14910,1170,10992,,15357,5945,14910,1170,11250,1665,6692,650,12025,7917,11250,1665,7005,2580,1912,1972,8665,11162,7005,2580,1950,7185xear,7165,4345,13110,1080,12690,2340,13080nfear475,11732,4835,17650,2910,17640,3465,17445nfear7660,12382,14412,21597,7905,18675,8235,19545nfear7660,12382,14412,21597,14280,18330,14400,17370nfear12910,11080,18695,18947,18690,15045,17070,11475nfear15772,2592,21105,9865,20175,9015,20895,7665nfear14330,,19187,6595,19200,3345,19140,2715nfear14330,,19187,6595,14910,1170,14550,1980nfear10992,,15357,5945,11250,1665,11040,2340nfear1912,1972,8665,11162,7650,3270,7005,2580nfear1912,1972,8665,11162,1950,7185,2070,7890nfem@23@37qx@35@24@23@36@34@24@23@37xem@16@33qx@31@17@16@32@30@17@16@33xem@38@29qx@27@39@38@28@26@39@38@29xe";

            Formulas.push_back(L"sum #0 0 10800");
            Formulas.push_back(L"sum #1 0 10800");
            Formulas.push_back(L"cosatan2 10800 @0 @1");
            Formulas.push_back(L"sinatan2 10800 @0 @1");
            Formulas.push_back(L"sum @2 10800 0");
            Formulas.push_back(L"sum @3 10800 0");
            Formulas.push_back(L"sum @4 0 #0");
            Formulas.push_back(L"sum @5 0 #1");
            Formulas.push_back(L"mod @6 @7 0");
            Formulas.push_back(L"prod 600 11 1");
            Formulas.push_back(L"sum @8 0 @9");
            Formulas.push_back(L"prod @10 1 3");
            Formulas.push_back(L"prod 600 3 1");
            Formulas.push_back(L"sum @11 @12 0");
            Formulas.push_back(L"prod @13 @6 @8");
            Formulas.push_back(L"prod @13 @7 @8");
            Formulas.push_back(L"sum @14 #0 0");
            Formulas.push_back(L"sum @15 #1 0");
            Formulas.push_back(L"prod 600 8 1");
            Formulas.push_back(L"prod @11 2 1");
            Formulas.push_back(L"sum @18 @19 0");
            Formulas.push_back(L"prod @20 @6 @8");
            Formulas.push_back(L"prod @20 @7 @8");
            Formulas.push_back(L"sum @21 #0 0");
            Formulas.push_back(L"sum @22 #1 0");
            Formulas.push_back(L"prod 600 2 1");
            Formulas.push_back(L"sum #0 600 0");
            Formulas.push_back(L"sum #0 0 600");
            Formulas.push_back(L"sum #1 600 0");
            Formulas.push_back(L"sum #1 0 600");
            Formulas.push_back(L"sum @16 @25 0");
            Formulas.push_back(L"sum @16 0 @25");
            Formulas.push_back(L"sum @17 @25 0");
            Formulas.push_back(L"sum @17 0 @25");
            Formulas.push_back(L"sum @23 @12 0");
            Formulas.push_back(L"sum @23 0 @12");
            Formulas.push_back(L"sum @24 @12 0");
            Formulas.push_back(L"sum @24 0 @12");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");

            AdjustmentValues	=	L"1350,25920";
            ConnectorLocations	=	L"67,10800;10800,21577;21582,10800;10800,1235;@38,@39";
            TextBoxRectangle	=	L"2977,3262,17087,17337";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);
		}
	};

	class BorderCallout90 : public ShapeType
	{
	public:
		BorderCallout90 () : ShapeType(msosptBorderCallout90)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3nfem,l21600,r,21600l,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");

            AdjustmentValues	=	L"-1800,24300,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);
		}
	};

	class BorderCallout1 : public ShapeType
	{
	public:
		BorderCallout1 () : ShapeType(msosptBorderCallout1)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3nfem,l21600,r,21600l,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");

            AdjustmentValues	=	L"-8280,24300,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);
		}
	};

	class BorderCallout2 : public ShapeType
	{
	public:
		BorderCallout2 () : ShapeType(msosptBorderCallout2)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3nfem,l21600,r,21600l,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");

            AdjustmentValues	=	L"-10080,24300,-3600,4050,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);
		}
	};

	class BorderCallout3 : public ShapeType
	{
	public:
		BorderCallout3 () : ShapeType(msosptBorderCallout3)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3@4@5nfem,l21600,r,21600l,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"val #4");
            Formulas.push_back(L"val #5");

            AdjustmentValues	=	L"-10080,24300,-3600,4050,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#4,#5";
			Handles.push_back (three);
		}
	};

	class AccentCallout90 : public ShapeType
	{
	public:
		AccentCallout90 () : ShapeType(msosptAccentCallout90)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3nfem,l21600,r,21600l,21600nsxe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");

            AdjustmentValues	=	L"-1800,24300,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);
		}
	};

	class AccentCallout1 : public ShapeType
	{
	public:
		AccentCallout1 () : ShapeType(msosptAccentCallout1)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3nfem@2,l@2,21600nfem,l21600,r,21600l,21600nsxe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");

            AdjustmentValues	=	L"-8280,24300,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);
		}
	};

	class AccentCallout2 : public ShapeType
	{
	public:
		AccentCallout2 () : ShapeType(msosptAccentCallout2)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3@4@5nfem@4,l@4,21600nfem,l21600,r,21600l,21600nsxe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"val #4");
            Formulas.push_back(L"val #5");

            AdjustmentValues	=	L"-8280,24300,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#4,#5";
			Handles.push_back (three);
		}
	};

	class AccentCallout3 : public ShapeType
	{
	public:
		AccentCallout3 () : ShapeType(msosptAccentCallout3)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3@4@5@6@7nfem@6,l@6,21600nfem,l21600,r,21600l,21600nsxe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"val #4");
            Formulas.push_back(L"val #5");
            Formulas.push_back(L"val #6");
            Formulas.push_back(L"val #7");

            AdjustmentValues	=	L"23400,24400,25200,21600,25200,4050,23400,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#4,#5";
			Handles.push_back (three);

			Handle four;
            four.position		=	L"#6,#7";
			Handles.push_back (four);
		}
	};

	class Callout90 : public ShapeType
	{
	public:
		Callout90 () : ShapeType(msosptCallout90)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3nfem,l21600,r,21600l,21600nsxe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");

            AdjustmentValues	=	L"-1800,24300,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);
		}
	};

	class Callout1 : public ShapeType
	{
	public:
		Callout1 () : ShapeType(msosptCallout1)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3nfem,l21600,r,21600l,21600nsxe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");

            AdjustmentValues	=	L"-8280,24300,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);
		}
	};

	class Callout2 : public ShapeType
	{
	public:
		Callout2 () : ShapeType(msosptCallout2)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3@4@5nfem,l21600,r,21600l,21600nsxe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"val #4");
            Formulas.push_back(L"val #5");

            AdjustmentValues	=	L"-10080,24300,-3600,4050,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#4,#5";
			Handles.push_back (three);
		}
	};

	class Callout3 : public ShapeType
	{
	public:
		Callout3 () : ShapeType(msosptCallout3)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3@4@5@6@7nfem,l21600,r,21600l,21600nsxe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"val #4");
            Formulas.push_back(L"val #5");
            Formulas.push_back(L"val #6");
            Formulas.push_back(L"val #7");

            AdjustmentValues	=	L"23400,24400,25200,21600,25200,4050,23400,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#4,#5";
			Handles.push_back (three);

			Handle four;
            four.position		=	L"#6,#7";
			Handles.push_back (four);
		}
	};

	class AccentBorderCallout90 : public ShapeType
	{
	public:
		AccentBorderCallout90 () : ShapeType(msosptAccentBorderCallout90)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3nfem,l21600,r,21600l,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");

            AdjustmentValues	=	L"-1800,24300,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);
		}
	};

	class AccentBorderCallout1 : public ShapeType
	{
	public:
		AccentBorderCallout1 () : ShapeType(msosptAccentBorderCallout1)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3nfem@2,l@2,21600nfem,l21600,r,21600l,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");

            AdjustmentValues	=	L"-8280,24300,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);
		}
	};

	class AccentBorderCallout2 : public ShapeType
	{
	public:
		AccentBorderCallout2 () : ShapeType(msosptAccentBorderCallout2)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3@4@5nfem@4,l@4,21600nfem,l21600,r,21600l,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"val #4");
            Formulas.push_back(L"val #5");

            AdjustmentValues	=	L"-10080,24300,-3600,4050,-1800,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#4,#5";
			Handles.push_back (three);
		}
	};

	class AccentBorderCallout3 : public ShapeType
	{
	public:
		AccentBorderCallout3 () : ShapeType(msosptAccentBorderCallout3)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m@0@1l@2@3@4@5@6@7nfem@6,l@6,21600nfem,l21600,r,21600l,21600xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #2");
            Formulas.push_back(L"val #3");
            Formulas.push_back(L"val #4");
            Formulas.push_back(L"val #5");
            Formulas.push_back(L"val #6");
            Formulas.push_back(L"val #7");

            AdjustmentValues	=	L"23400,24400,25200,21600,25200,4050,23400,4050";
            ConnectorLocations	=	L"@0,@1;10800,0;10800,21600;0,10800;21600,10800";

			Handle one;
            one.position		=	L"#0,#1";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#2,#3";
			Handles.push_back (two);

			Handle three;
            three.position		=	L"#4,#5";
			Handles.push_back (three);

			Handle four;
            four.position		=	L"#6,#7";
			Handles.push_back (four);
		}
	};
}
