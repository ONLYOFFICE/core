/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
		{
			this->Path		= _T( "m@7,l@8,m@5,21600l@6,21600e" ); 
			this->Textpath	= _T(" on=\"t\" style=\"v-text-kern:t\" fitpath=\"t\"");

			Formulas.push_back(_T("sum #0 0 10800"));
			Formulas.push_back(_T("prod #0 2 1"));
			Formulas.push_back(_T("sum 21600 0 @1"));
			Formulas.push_back(_T("sum 0 0 @2"));
			Formulas.push_back(_T("sum 21600 0 @3"));
			Formulas.push_back(_T("if @0 @3 0"));
			Formulas.push_back(_T("if @0 21600 @1"));
			Formulas.push_back(_T("if @0 0 @2"));
			Formulas.push_back(_T("if @0 @4 21600"));
			Formulas.push_back(_T("mid @5 @6"));
			Formulas.push_back(_T("mid @8 @5"));
			Formulas.push_back(_T("mid @7 @8"));
			Formulas.push_back(_T("mid @6 @7"));
			Formulas.push_back(_T("sum @6 0 @5"));

			 AdjustmentValues	=	_T("10800");
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
			this->Path				=	_T("m0@4l@2,l@3,l21600@4em0@5l@2,21600l@3,21600l21600@5e");

			Formulas.push_back(_T("val #0"));	
			Formulas.push_back(_T("prod width 1 3"));	
			Formulas.push_back(_T("sum 0 @1 0"));	
			Formulas.push_back(_T("sum width 0 @1"));	
			Formulas.push_back(_T("sum 0 @0 0"));	
			Formulas.push_back(_T("sum height 0 @0"));	

			AdjustmentValues	=	_T("5400");
			Handle one;
			one.position		=	_T("0,@0");
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
			Path				=	_T("al10800,10800,10800,10800@2@14e");

			Formulas.push_back(_T("val #1"));
			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum 0 0 #0"));
			Formulas.push_back(_T("sumangle #0 0 180"));
			Formulas.push_back(_T("sumangle #0 0 90"));
			Formulas.push_back(_T("prod @4 2 1"));
			Formulas.push_back(_T("sumangle #0 90 0"));
			Formulas.push_back(_T("prod @6 2 1"));
			Formulas.push_back(_T("abs #0"));
			Formulas.push_back(_T("sumangle @8 0 90"));
			Formulas.push_back(_T("if @9 @7 @5"));
			Formulas.push_back(_T("sumangle @10 0 360"));
			Formulas.push_back(_T("if @10 @11 @10"));
			Formulas.push_back(_T("sumangle @12 0 360"));
			Formulas.push_back(_T("if @12 @13 @12"));
			Formulas.push_back(_T("sum 0 0 @14"));
			Formulas.push_back(_T("val 10800"));
			Formulas.push_back(_T("cos 10800 #0"));
			Formulas.push_back(_T("sin 10800 #0"));
			Formulas.push_back(_T("sum @17 10800 0"));
			Formulas.push_back(_T("sum @18 10800 0"));
			Formulas.push_back(_T("sum 10800 0 @17"));
			Formulas.push_back(_T("if @9 0 21600"));
			Formulas.push_back(_T("sum 10800 0 @18"));

			AdjustmentValues	=	_T("11796480");
			Handle one;
			one.position		=	_T("@16,#0");
			one.polar			=	_T("10800,10800");
			Handles.push_back (one);
		}

		virtual ~WordArtTextArchUpCurve ()
		{

		}
	};
}
