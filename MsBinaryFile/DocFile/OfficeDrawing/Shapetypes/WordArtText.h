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
	class WordArtTextType: public ShapeType
	{
	public:
		WordArtTextType(int txStyle):
		ShapeType((MSOSPT)txStyle)
		{//plant text
            this->Path		= L"m@7,l@8,m@5,21600l@6,21600e" ;
            this->Textpath	= L" on=\"t\" style=\"v-text-kern:t\" fitpath=\"t\"";

            Formulas.push_back(L"sum #0 0 10800");
            Formulas.push_back(L"prod #0 2 1");
            Formulas.push_back(L"sum 21600 0 @1");
            Formulas.push_back(L"sum 0 0 @2");
            Formulas.push_back(L"sum 21600 0 @3");
            Formulas.push_back(L"if @0 @3 0");
            Formulas.push_back(L"if @0 21600 @1");
            Formulas.push_back(L"if @0 0 @2");
            Formulas.push_back(L"if @0 @4 21600");
            Formulas.push_back(L"mid @5 @6");
            Formulas.push_back(L"mid @8 @5");
            Formulas.push_back(L"mid @7 @8");
            Formulas.push_back(L"mid @6 @7");
            Formulas.push_back(L"sum @6 0 @5");

            AdjustmentValues	=	L"10800";

			//<v:textpath on="t" fitshape="t"/>
		}

		virtual ~WordArtTextType()
		{
		}
	};

	class WordArtTextStop : public WordArtTextType
	{
	public:
		WordArtTextStop () : WordArtTextType(msosptTextStop)
		{
			this->Joins				=	miter;
            this->Path				=	L"m0@4l@2,l@3,l21600@4em0@5l@2,21600l@3,21600l21600@5e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"prod width 1 3");
            Formulas.push_back(L"sum 0 @1 0");
            Formulas.push_back(L"sum width 0 @1");
            Formulas.push_back(L"sum 0 @0 0");
            Formulas.push_back(L"sum height 0 @0");

            AdjustmentValues	=	L"5400";
			Handle one;
            one.position		=	L"0,@0";
			Handles.push_back (one);
		}

		virtual ~WordArtTextStop ()
		{

		}
	};

	class WordArtTextArchUpCurve : public WordArtTextType
	{
	public:
		WordArtTextArchUpCurve () : WordArtTextType(msosptTextArchUpCurve)
		{
            Path				=	L"al10800,10800,10800,10800@2@14e";

            Formulas.push_back(L"val #1");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 0 0 #0");
            Formulas.push_back(L"sumangle #0 0 180");
            Formulas.push_back(L"sumangle #0 0 90");
            Formulas.push_back(L"prod @4 2 1");
            Formulas.push_back(L"sumangle #0 90 0");
            Formulas.push_back(L"prod @6 2 1");
            Formulas.push_back(L"abs #0");
            Formulas.push_back(L"sumangle @8 0 90");
            Formulas.push_back(L"if @9 @7 @5");
            Formulas.push_back(L"sumangle @10 0 360");
            Formulas.push_back(L"if @10 @11 @10");
            Formulas.push_back(L"sumangle @12 0 360");
            Formulas.push_back(L"if @12 @13 @12");
            Formulas.push_back(L"sum 0 0 @14");
            Formulas.push_back(L"val 10800");
            Formulas.push_back(L"cos 10800 #0");
            Formulas.push_back(L"sin 10800 #0");
            Formulas.push_back(L"sum @17 10800 0");
            Formulas.push_back(L"sum @18 10800 0");
            Formulas.push_back(L"sum 10800 0 @17");
            Formulas.push_back(L"if @9 0 21600");
            Formulas.push_back(L"sum 10800 0 @18");

            AdjustmentValues	=	L"11796480";
			Handle one;
            one.position		=	L"@16,#0";
            one.polar			=	L"10800,10800";
			Handles.push_back (one);
		}

		virtual ~WordArtTextArchUpCurve ()
		{

		}
	};
	class WordArtTextDeflate : public WordArtTextType
	{
	public:
		WordArtTextDeflate () : WordArtTextType(msosptTextDeflate)
		{
            Path				=	L"m,c7200@0,14400@0,21600,m,21600c7200@1,14400@1,21600,21600e";

            Formulas.push_back(L"prod #0 4 3");
            Formulas.push_back(L"sum 21600 0 @0");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 #0");

            AdjustmentValues	=	L"4050";
			Handle one;
            one.position		=	L"enter,#0";
            one.yrange			=	L"0,8100";
			Handles.push_back (one);
			//<v:textpath on="t" fitshape="t" xscale="t"/>
		}

		virtual ~WordArtTextDeflate ()
		{

		}
	};
	class WordArtTextSlantUp : public WordArtTextType
	{
	public:
		WordArtTextSlantUp () : WordArtTextType(msosptTextSlantUp)
		{
            Path				=	L"m0@0l21600,m,21600l21600@1e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 @0");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"sum @2 10800 0");
            Formulas.push_back(L"prod @1 1 2");
            Formulas.push_back(L"sum @4 10800 0");

            AdjustmentValues	=	L"12000";
			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"0,15429";
			Handles.push_back (one);
		}
		virtual ~WordArtTextSlantUp ()
		{

		}
	};
	class WordArtTextCanDown : public WordArtTextType
	{
	public:
		WordArtTextCanDown () : WordArtTextType(msosptTextCanDown)
		{
            Path				=	L"m,qy10800@0,21600,m0@1qy10800,21600,21600@1e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"prod @1 1 2");
            Formulas.push_back(L"sum @2 10800 0");

            AdjustmentValues	=	L"3086";
			Handle one;
            one.position		=	L"center,#0";
            one.yrange			=	L"0,7200";
            Handles.push_back (one);
		}
		virtual ~WordArtTextCanDown ()
		{

		}
	};
	class WordArtTextWave1 : public WordArtTextType
	{
	public:
		WordArtTextWave1 () : WordArtTextType(msosptTextWave1)
		{
            Path				=	L"m@25@0c@26@3@27@1@28@0m@21@4c@22@5@23@6@24@4e";

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

            AdjustmentValues	=	L"2809,10800";
			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"0,4459";
			Handles.push_back (one);
			
			Handle two;
            two.position		=	L"#1,bottomRight";
            two.xrange			=	L"8640,12960";
            Handles.push_back (two);

			//<v:textpath on="t" fitshape="t"/>
		}
		virtual ~WordArtTextWave1 ()
		{

		}
	};
	class WordArtTextWave3 : public WordArtTextType
	{
	public:
		WordArtTextWave3 () : WordArtTextType(msosptTextWave3)
		{
            Path				=	L"m@37@0c@38@3@39@1@40@0@41@3@42@1@43@0m@30@4c@31@5@32@6@33@4@34@5@35@6@36@4e";

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

            AdjustmentValues	=	L"1404,10800";
			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"0,2229";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,bottomRight";
            two.xrange			=	L"8640,12960";
			Handles.push_back (two);
			//<v:textpath on="t" fitshape="t" xscale="t"/>
		}
		virtual ~WordArtTextWave3 ()
		{

		}
	};
	class WordArtTextWave4 : public WordArtTextType
	{
	public:
		WordArtTextWave4 () : WordArtTextType(msosptTextWave4)
		{
            Path				=	L"m@37@0c@38@1@39@3@40@0@41@1@42@3@43@0m@30@4c@31@6@32@5@33@4@34@6@35@5@36@4e";

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

            AdjustmentValues	=	L"1404,10800";
			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"0,2229";
			Handles.push_back (one);

			Handle two;
            two.position		=	L"#1,bottomRight";
            two.xrange			=	L"8640,12960";
			Handles.push_back (two);
			//<v:textpath on="t" fitshape="t" xscale="t"/>
		}
		virtual ~WordArtTextWave4 ()
		{

		}
	};
	class WordArtTextCurveUp : public WordArtTextType
	{
	public:
		WordArtTextCurveUp () : WordArtTextType(msosptTextCurveUp)
		{
            Path				=	L"m0@0c7200@2,14400@1,21600,m0@5c7200@6,14400@6,21600@5e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"prod #0 3 4");
            Formulas.push_back(L"prod #0 5 4");
            Formulas.push_back(L"prod #0 3 8");
            Formulas.push_back(L"prod #0 1 8");
            Formulas.push_back(L"sum 21600 0 @3");
            Formulas.push_back(L"sum @4 21600 0");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"prod @5 1 2");
            Formulas.push_back(L"sum @7 @8 0");
            Formulas.push_back(L"prod #0 7 8");
            Formulas.push_back(L"prod @5 1 3");
            Formulas.push_back(L"sum @1 @2 0");
            Formulas.push_back(L"sum @12 @0 0");
            Formulas.push_back(L"prod @13 1 4");
            Formulas.push_back(L"sum @11 14400 @14");

            AdjustmentValues	=	L"9931";
			Handle one;
            one.position		=	L"topLeft,#0";
            one.yrange			=	L"0,12169";
			Handles.push_back (one);
			//<v:textpath on="t" fitshape="t" xscale="t"/>
		}

		virtual ~WordArtTextCurveUp ()
		{

		}
	};

	class WordArtTextFadeUp : public WordArtTextType
	{
	public:
		WordArtTextFadeUp () : WordArtTextType(msosptTextFadeUp)
		{
            Path				=	L"m@0,l@1,m,21600r21600,e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 @0");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"sum 21600 0 @2");
            Formulas.push_back(L"sum @1 21600 @0");

            AdjustmentValues	=	L"7200";
			Handle one;
            one.position		=	L"#0,topLeft";
            one.xrange			=	L"0,10792";
			Handles.push_back (one);
			//<v:textpath on="t" fitshape="t"/>
		}

		virtual ~WordArtTextFadeUp ()
		{

		}
	};

	class WordArtTextTriangle : public WordArtTextType
	{
	public:
		WordArtTextTriangle () : WordArtTextType(msosptTextTriangle)
		{
            Path				=	L"m0@0l10800,,21600@0m,21600r10800,l21600,21600e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"sum @1 10800 0");
            Formulas.push_back(L"sum 21600 0 @1");

            AdjustmentValues	=	L"10800";
			Handle one;
            one.position		=	L"#topLeft,#0";
            one.xrange			=	L"0,21600";
			Handles.push_back (one);
			//<v:textpath on="t" fitshape="t"/>
		}

		virtual ~WordArtTextTriangle ()
		{

		}
	};

	class WordArtTextCascadeUp : public WordArtTextType
	{
	public:
		WordArtTextCascadeUp () : WordArtTextType(msosptTextCascadeUp)
		{
            Path				=	L"m0@2l21600,m,21600l21600@0e";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum 21600 0 #0");
            Formulas.push_back(L"prod @1 1 4");
            Formulas.push_back(L"prod #0 1 2");
            Formulas.push_back(L"prod @2 1 2");
            Formulas.push_back(L"sum @3 10800 0");
            Formulas.push_back(L"sum @4 10800 0");
            Formulas.push_back(L"sum @0 21600 @2");
            Formulas.push_back(L"prod @7 1 2");

            AdjustmentValues	=	L"9600";
			Handle one;
            one.position		=	L"bottomRight,#0";
            one.yrange			=	L"6171,21600";
			Handles.push_back (one);
			// <v:textpath on="t" fitshape="t"/>
		}

		virtual ~WordArtTextCascadeUp ()
		{

		}
	};

	class WordArtTextDeflateBottom : public WordArtTextType
	{
	public:
		WordArtTextDeflateBottom () : WordArtTextType(msosptTextDeflateBottom)
		{
            Path				=	L"m,l21600,m,21600c7200@1,14400@1,21600,21600e";

            Formulas.push_back(L"prod #0 4 3");
            Formulas.push_back(L"sum @0 0 7200");
            Formulas.push_back(L"val #0");
            Formulas.push_back(L"prod #0 2 3");
            Formulas.push_back(L"sum @3 7200 0");

            AdjustmentValues	=	L"11475";
			Handle one;
            one.position		=	L"center,#0";
            one.yrange			=	L"1350,21600";
			Handles.push_back (one);
			//<v:textpath on="t" fitshape="t" xscale="t"/>
		}

		virtual ~WordArtTextDeflateBottom ()
		{

		}
	};

}
