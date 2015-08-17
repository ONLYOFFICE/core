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
