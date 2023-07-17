/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
	class ActionButtonBlank : public ShapeType
	{
	public:
		ActionButtonBlank () : ShapeType(msosptActionButtonBlank)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0e";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonHome : public ShapeType
	{
	public:
		ActionButtonHome () : ShapeType(msosptActionButtonHome)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@3@9nfl@11@4@28@4@28@10@33@10@33@4@12@4@32@26@32@24@31@24@31@25xem@31@25nfl@32@26em@28@4nfl@33@4em@29@10nfl@29@27@30@27@30@10e";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @10 0 @9");
			Formulas.push_back( L"prod @13 1 16");
			Formulas.push_back( L"prod @13 1 8");
			Formulas.push_back( L"prod @13 3 16");
			Formulas.push_back( L"prod @13 5 16");
			Formulas.push_back( L"prod @13 7 16");
			Formulas.push_back( L"prod @13 9 16");
			Formulas.push_back( L"prod @13 11 16");
			Formulas.push_back( L"prod @13 3 4");
			Formulas.push_back( L"prod @13 13 16");
			Formulas.push_back( L"prod @13 7 8");
			Formulas.push_back( L"sum @9 @14 0");
			Formulas.push_back( L"sum @9 @16 0");
			Formulas.push_back( L"sum @9 @17 0");
			Formulas.push_back( L"sum @9 @21 0");
			Formulas.push_back( L"sum @11 @15 0");
			Formulas.push_back( L"sum @11 @18 0");
			Formulas.push_back( L"sum @11 @19 0");
			Formulas.push_back( L"sum @11 @20 0");
			Formulas.push_back( L"sum @11 @22 0");
			Formulas.push_back( L"sum @11 @23 0");
			Formulas.push_back( L"sum @3 @5 0");
			Formulas.push_back( L"sum @4 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");
			Formulas.push_back( L"sum @24 @5 0");
			Formulas.push_back( L"sum @25 @5 0");
			Formulas.push_back( L"sum @26 @5 0");
			Formulas.push_back( L"sum @27 @5 0");
			Formulas.push_back( L"sum @28 @5 0");
			Formulas.push_back( L"sum @29 @5 0");
			Formulas.push_back( L"sum @30 @5 0");
			Formulas.push_back( L"sum @31 @5 0");
			Formulas.push_back( L"sum @32 @5 0");
			Formulas.push_back( L"sum @33 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonHelp : public ShapeType
	{
	public:
		ActionButtonHelp () : ShapeType(msosptActionButtonHelp)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@33@27nfqy@3@9@40@27@39@4@37@29l@37@30@36@30@36@29qy@37@28@39@27@3@26@34@27xem@3@31nfqx@35@32@3@10@38@32@3@31xe";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @10 0 @9");
			Formulas.push_back( L"prod @13 1 7");
			Formulas.push_back( L"prod @13 3 14");
			Formulas.push_back( L"prod @13 2 7");
			Formulas.push_back( L"prod @13 5 14");
			Formulas.push_back( L"prod @13 11 28");
			Formulas.push_back( L"prod @13 3 7");
			Formulas.push_back( L"prod @13 4 7");
			Formulas.push_back( L"prod @13 17 28");
			Formulas.push_back( L"prod @13 9 14");
			Formulas.push_back( L"prod @13 21 28");
			Formulas.push_back( L"prod @13 11 14");
			Formulas.push_back( L"prod @13 25 28");
			Formulas.push_back( L"sum @9 @14 0");
			Formulas.push_back( L"sum @9 @16 0");
			Formulas.push_back( L"sum @9 @18 0");
			Formulas.push_back( L"sum @9 @21 0");
			Formulas.push_back( L"sum @9 @23 0");
			Formulas.push_back( L"sum @9 @24 0");
			Formulas.push_back( L"sum @9 @25 0");
			Formulas.push_back( L"sum @11 @15 0");
			Formulas.push_back( L"sum @11 @17 0");
			Formulas.push_back( L"sum @11 @18 0");
			Formulas.push_back( L"sum @11 @19 0");
			Formulas.push_back( L"sum @11 @20 0");
			Formulas.push_back( L"sum @11 @21 0");
			Formulas.push_back( L"sum @11 @22 0");
			Formulas.push_back( L"sum @11 @24 0");
			Formulas.push_back( L"sum @3 @5 0");
			Formulas.push_back( L"sum @4 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");
			Formulas.push_back( L"sum @26 @5 0");
			Formulas.push_back( L"sum @27 @5 0");
			Formulas.push_back( L"sum @28 @5 0");
			Formulas.push_back( L"sum @29 @5 0");
			Formulas.push_back( L"sum @30 @5 0");
			Formulas.push_back( L"sum @31 @5 0");
			Formulas.push_back( L"sum @32 @5 0");
			Formulas.push_back( L"sum @33 @5 0");
			Formulas.push_back( L"sum @34 @5 0");
			Formulas.push_back( L"sum @35 @5 0");
			Formulas.push_back( L"sum @36 @5 0");
			Formulas.push_back( L"sum @37 @5 0");
			Formulas.push_back( L"sum @38 @5 0");
			Formulas.push_back( L"sum @39 @5 0");
			Formulas.push_back( L"sum @40 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonInfo : public ShapeType
	{
	public:
		ActionButtonInfo () : ShapeType(msosptActionButtonInformation)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@3@9nfqx@11@4@3@10@12@4@3@9xem@3@25nfqx@33@26@3@27@36@26@3@25xem@32@28nfl@32@29@34@29@34@30@32@30@32@31@37@31@37@30@35@30@35@28xe";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @10 0 @9");
			Formulas.push_back( L"prod @13 1 32");
			Formulas.push_back( L"prod @13 5 32");
			Formulas.push_back( L"prod @13 9 32");
			Formulas.push_back( L"prod @13 5 16");
			Formulas.push_back( L"prod @13 3 8");
			Formulas.push_back( L"prod @13 13 32");
			Formulas.push_back( L"prod @13 19 32");
			Formulas.push_back( L"prod @13 5 8");
			Formulas.push_back( L"prod @13 11 16");
			Formulas.push_back( L"prod @13 13 16");
			Formulas.push_back( L"prod @13 7 8");
			Formulas.push_back( L"sum @9 @14 0");
			Formulas.push_back( L"sum @9 @15 0");
			Formulas.push_back( L"sum @9 @16 0");
			Formulas.push_back( L"sum @9 @17 0");
			Formulas.push_back( L"sum @9 @18 0");
			Formulas.push_back( L"sum @9 @23 0");
			Formulas.push_back( L"sum @9 @24 0");
			Formulas.push_back( L"sum @11 @17 0");
			Formulas.push_back( L"sum @11 @18 0");
			Formulas.push_back( L"sum @11 @19 0");
			Formulas.push_back( L"sum @11 @20 0");
			Formulas.push_back( L"sum @11 @21 0");
			Formulas.push_back( L"sum @11 @22 0");
			Formulas.push_back( L"sum @3 @5 0");
			Formulas.push_back( L"sum @4 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");
			Formulas.push_back( L"sum @25 @5 0");
			Formulas.push_back( L"sum @26 @5 0");
			Formulas.push_back( L"sum @27 @5 0");
			Formulas.push_back( L"sum @28 @5 0");
			Formulas.push_back( L"sum @29 @5 0");
			Formulas.push_back( L"sum @30 @5 0");
			Formulas.push_back( L"sum @31 @5 0");
			Formulas.push_back( L"sum @32 @5 0");
			Formulas.push_back( L"sum @33 @5 0");
			Formulas.push_back( L"sum @34 @5 0");
			Formulas.push_back( L"sum @35 @5 0");
			Formulas.push_back( L"sum @36 @5 0");
			Formulas.push_back( L"sum @37 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonBack : public ShapeType
	{
	public:
		ActionButtonBack () : ShapeType(msosptActionButtonBackPrevious)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@12@9nfl@11@4@12@10xe";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @4 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonNext : public ShapeType
	{
	public:
		ActionButtonNext () : ShapeType(msosptActionButtonForwardNext)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@11@9nfl@12@4@11@10xe";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @4 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonBegin : public ShapeType
	{
	public:
		ActionButtonBegin () : ShapeType(msosptActionButtonBeginning)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@12@9l@17@4@12@10xem@11@9l@16@9@16@10@11@10xe";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @10 0 @9");
			Formulas.push_back( L"prod @13 1 8");
			Formulas.push_back( L"prod @13 1 4");
			Formulas.push_back( L"sum @11 @14 0");
			Formulas.push_back( L"sum @11 @15 0");
			Formulas.push_back( L"sum @4 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");
			Formulas.push_back( L"sum @16 @5 0");
			Formulas.push_back( L"sum @17 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonEnd : public ShapeType
	{
	public:
		ActionButtonEnd () : ShapeType(msosptActionButtonEnd)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@11@9l@16@4@11@10xem@17@9l@12@9@12@10@17@10xe";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @10 0 @9");
			Formulas.push_back( L"prod @13 3 4");
			Formulas.push_back( L"prod @13 7 8");
			Formulas.push_back( L"sum @11 @14 0");
			Formulas.push_back( L"sum @11 @15 0");
			Formulas.push_back( L"sum @4 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");
			Formulas.push_back( L"sum @16 @5 0");
			Formulas.push_back( L"sum @17 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonReturn : public ShapeType
	{
	public:
		ActionButtonReturn () : ShapeType(msosptActionButtonReturn)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@12@21nfl@23@9@3@21@24@21@24@20qy@3@19l@25@19qx@26@20l@26@21@11@21@11@20qy@25@10l@3@10qx@22@20l@22@21xe";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @10 0 @9");
			Formulas.push_back( L"prod @13 7 8");
			Formulas.push_back( L"prod @13 3 4");
			Formulas.push_back( L"prod @13 5 8");
			Formulas.push_back( L"prod @13 3 8");
			Formulas.push_back( L"prod @13 1 4");
			Formulas.push_back( L"sum @9 @15 0");
			Formulas.push_back( L"sum @9 @16 0");
			Formulas.push_back( L"sum @9 @18 0");
			Formulas.push_back( L"sum @11 @14 0");
			Formulas.push_back( L"sum @11 @15 0");
			Formulas.push_back( L"sum @11 @16 0");
			Formulas.push_back( L"sum @11 @17 0");
			Formulas.push_back( L"sum @11 @18 0");
			Formulas.push_back( L"sum @3 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");
			Formulas.push_back( L"sum @19 @5 0");
			Formulas.push_back( L"sum @20 @5 0");
			Formulas.push_back( L"sum @21 @5 0");
			Formulas.push_back( L"sum @22 @5 0");
			Formulas.push_back( L"sum @23 @5 0");
			Formulas.push_back( L"sum @24 @5 0");
			Formulas.push_back( L"sum @25 @5 0");
			Formulas.push_back( L"sum @26 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonDoc : public ShapeType
	{
	public:
		ActionButtonDoc () : ShapeType(msosptActionButtonDocument)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@12@9nfl@12@10@13@10@13@14@15@9xem@15@9nfl@15@14@13@14e";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"prod #0 3 4");
			Formulas.push_back( L"sum @3 @11 6075");
			Formulas.push_back( L"sum @3 6075 @11");
			Formulas.push_back( L"sum @4 @5 4050");
			Formulas.push_back( L"sum @13 @5 4050");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @12 @5 0");
			Formulas.push_back( L"sum @13 @5 0");
			Formulas.push_back( L"sum @14 @5 0");
			Formulas.push_back( L"sum @15 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonSound : public ShapeType
	{
	public:
		ActionButtonSound () : ShapeType(msosptActionButtonSound)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@11@21nfl@11@22@24@22@25@10@25@9@24@21xem@26@21nfl@12@20em@26@4nfl@12@4em@26@22nfl@12@23e";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @10 0 @9");
			Formulas.push_back( L"prod @13 1 8");
			Formulas.push_back( L"prod @13 5 16");
			Formulas.push_back( L"prod @13 5 8");
			Formulas.push_back( L"prod @13 11 16");
			Formulas.push_back( L"prod @13 3 4");
			Formulas.push_back( L"prod @13 7 8");
			Formulas.push_back( L"sum @9 @14 0");
			Formulas.push_back( L"sum @9 @15 0");
			Formulas.push_back( L"sum @9 @17 0");
			Formulas.push_back( L"sum @9 @19 0");
			Formulas.push_back( L"sum @11 @15 0");
			Formulas.push_back( L"sum @11 @16 0");
			Formulas.push_back( L"sum @11 @18 0");
			Formulas.push_back( L"sum @4 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");
			Formulas.push_back( L"sum @20 @5 0");
			Formulas.push_back( L"sum @21 @5 0");
			Formulas.push_back( L"sum @22 @5 0");
			Formulas.push_back( L"sum @23 @5 0");
			Formulas.push_back( L"sum @24 @5 0");
			Formulas.push_back( L"sum @25 @5 0");
			Formulas.push_back( L"sum @26 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};

	class ActionButtonMovie : public ShapeType
	{
	public:
		ActionButtonMovie () : ShapeType(msosptActionButtonMovie)
		{
			ShapeConcentricFill		=	true;
			Joins					=	miter;
			Path					=	L"m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@11@39nfl@11@44@31@44@32@43@33@43@33@47@35@47@35@45@36@45@38@46@12@46@12@41@38@41@37@42@35@42@35@41@34@40@32@40@31@39xe";

			Formulas.push_back( L"val #0");
			Formulas.push_back( L"sum width 0 #0");
			Formulas.push_back( L"sum height 0 #0");
			Formulas.push_back( L"prod width 1 2");
			Formulas.push_back( L"prod height 1 2");
			Formulas.push_back( L"prod #0 1 2");
			Formulas.push_back( L"prod #0 3 2");
			Formulas.push_back( L"sum @1 @5 0");
			Formulas.push_back( L"sum @2 @5 0");
			Formulas.push_back( L"sum @0 @4 8100");
			Formulas.push_back( L"sum @2 8100 @4");
			Formulas.push_back( L"sum @0 @3 8100");
			Formulas.push_back( L"sum @1 8100 @3");
			Formulas.push_back( L"sum @10 0 @9");
			Formulas.push_back( L"prod @13 1455 21600");
			Formulas.push_back( L"prod @13 1905 21600");
			Formulas.push_back( L"prod @13 2325 21600");
			Formulas.push_back( L"prod @13 16155 21600");
			Formulas.push_back( L"prod @13 17010 21600");
			Formulas.push_back( L"prod @13 19335 21600");
			Formulas.push_back( L"prod @13 19725 21600");
			Formulas.push_back( L"prod @13 20595 21600");
			Formulas.push_back( L"prod @13 5280 21600");
			Formulas.push_back( L"prod @13 5730 21600");
			Formulas.push_back( L"prod @13 6630 21600");
			Formulas.push_back( L"prod @13 7492 21600");
			Formulas.push_back( L"prod @13 9067 21600");
			Formulas.push_back( L"prod @13 9555 21600");
			Formulas.push_back( L"prod @13 13342 21600");
			Formulas.push_back( L"prod @13 14580 21600");
			Formulas.push_back( L"prod @13 15592 21600");
			Formulas.push_back( L"sum @11 @14 0");
			Formulas.push_back( L"sum @11 @15 0");
			Formulas.push_back( L"sum @11 @16 0");
			Formulas.push_back( L"sum @11 @17 0");
			Formulas.push_back( L"sum @11 @18 0");
			Formulas.push_back( L"sum @11 @19 0");
			Formulas.push_back( L"sum @11 @20 0");
			Formulas.push_back( L"sum @11 @21 0");
			Formulas.push_back( L"sum @9 @22 0");
			Formulas.push_back( L"sum @9 @23 0");
			Formulas.push_back( L"sum @9 @24 0");
			Formulas.push_back( L"sum @9 @25 0");
			Formulas.push_back( L"sum @9 @26 0");
			Formulas.push_back( L"sum @9 @27 0");
			Formulas.push_back( L"sum @9 @28 0");
			Formulas.push_back( L"sum @9 @29 0");
			Formulas.push_back( L"sum @9 @30 0");
			Formulas.push_back( L"sum @9 @31 0");
			Formulas.push_back( L"sum @4 @5 0");
			Formulas.push_back( L"sum @9 @5 0");
			Formulas.push_back( L"sum @10 @5 0");
			Formulas.push_back( L"sum @11 @5 0");
			Formulas.push_back( L"sum @12 @5 0");
			Formulas.push_back( L"sum @31 @5 0");
			Formulas.push_back( L"sum @32 @5 0");
			Formulas.push_back( L"sum @33 @5 0");
			Formulas.push_back( L"sum @34 @5 0");
			Formulas.push_back( L"sum @35 @5 0");
			Formulas.push_back( L"sum @36 @5 0");
			Formulas.push_back( L"sum @37 @5 0");
			Formulas.push_back( L"sum @38 @5 0");
			Formulas.push_back( L"sum @39 @5 0");
			Formulas.push_back( L"sum @40 @5 0");
			Formulas.push_back( L"sum @41 @5 0");
			Formulas.push_back( L"sum @42 @5 0");
			Formulas.push_back( L"sum @43 @5 0");
			Formulas.push_back( L"sum @44 @5 0");
			Formulas.push_back( L"sum @45 @5 0");
			Formulas.push_back( L"sum @46 @5 0");
			Formulas.push_back( L"sum @47 @5 0");

			Adjustments.push_back(1350);

			ConnectorLocations		=	L"0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0";
			TextBoxRectangle		=	L"@0,@0,@1,@2";

			Handle HandleOne;
			HandleOne.position		=	L"#0,topLeft";
			HandleOne.switchHandle	=	L"true";
			HandleOne.xrange		=	L"0,5400";
			Handles.push_back( HandleOne );

			Limo					=	L"10800,10800";
		}
	};
}
