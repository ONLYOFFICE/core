#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
	class LeftBracketType : public ShapeType
	{
	public:
		LeftBracketType () : ShapeType(msosptLeftBracket)	
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
			Path				=	_T("m21600,qx0@0l0@1qy21600,21600e");

			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum 21600 0 #0"));
			Formulas.push_back(_T("sum #1 0 #0"));
			Formulas.push_back(_T("sum #1 #0 0"));
			Formulas.push_back(_T("prod #0 9598 32768"));
			Formulas.push_back(_T("sum 21600 0 @4"));
			Formulas.push_back(_T("sum 21600 0 #1"));
			Formulas.push_back(_T("min #1 @6"));
			Formulas.push_back(_T("prod @7 1 2"));
			Formulas.push_back(_T("prod #0 2 1"));
			Formulas.push_back(_T("sum 21600 0 @9"));
			Formulas.push_back(_T("val #1"));

			AdjustmentValues	=	_T("1800");
			ConnectorLocations	=	_T("0,0;21600,@11;0,21600");
			TextBoxRectangle	=	_T("0,@4,7637,@5");

			Handle one;
			one.position		=	_T("center,#0");
			one.yrange			=	_T("0,@8"); 
			Handles.push_back (one);

			Handle two;
			two.position		=	_T("bottomRight,#1");
			two.yrange			=	_T("@9,@10"); 
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
			Path				=	_T("m,qx21600@0l21600@1qy,21600e");

			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum 21600 0 #0"));
			Formulas.push_back(_T("prod #0 9598 32768"));
			Formulas.push_back(_T("sum 21600 0 @2"));

			AdjustmentValues	=	_T("1800");
			ConnectorLocations	=	_T("0,0;0,21600;21600,10800");
			TextBoxRectangle	=	_T("0,@2,15274,@3");

			Handle one;
			one.position		=	_T("bottomRight,#0");
			one.yrange			=	_T("0,10800"); 
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
			Path				=	_T("m21600,qx10800@0l10800@2qy0@11,10800@3l10800@1qy21600,21600e");

			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum 21600 0 #0"));
			Formulas.push_back(_T("sum #1 0 #0"));
			Formulas.push_back(_T("sum #1 #0 0"));
			Formulas.push_back(_T("prod #0 9598 32768"));
			Formulas.push_back(_T("sum 21600 0 @4"));
			Formulas.push_back(_T("sum 21600 0 #1"));
			Formulas.push_back(_T("min #1 @6"));
			Formulas.push_back(_T("prod @7 1 2"));
			Formulas.push_back(_T("prod #0 2 1"));
			Formulas.push_back(_T("sum 21600 0 @9"));
			Formulas.push_back(_T("val #1"));

			AdjustmentValues	=	_T("1800,10800");
			ConnectorLocations	=	_T("21600,0;0,10800;21600,21600");
			TextBoxRectangle	=	_T("13963,@4,21600,@5");

			Handle one;
			one.position		=	_T("center,#0");
			one.yrange			=	_T("0,@8"); 
			Handles.push_back (one);

			Handle two;
			two.position		=	_T("topLeft,#1");
			two.yrange			=	_T("@9,@10"); 
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
			Path				=	_T("m,qx10800@0l10800@2qy21600@11,10800@3l10800@1qy,21600e");

			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum 21600 0 #0"));
			Formulas.push_back(_T("sum #1 0 #0"));
			Formulas.push_back(_T("sum #1 #0 0"));
			Formulas.push_back(_T("prod #0 9598 32768"));
			Formulas.push_back(_T("sum 21600 0 @4"));
			Formulas.push_back(_T("sum 21600 0 #1"));
			Formulas.push_back(_T("min #1 @6"));
			Formulas.push_back(_T("prod @7 1 2"));
			Formulas.push_back(_T("prod #0 2 1"));
			Formulas.push_back(_T("sum 21600 0 @9"));
			Formulas.push_back(_T("val #1"));

			AdjustmentValues	=	_T("1800,10800");
			ConnectorLocations	=	_T("0,0;21600,@11;0,21600");
			TextBoxRectangle	=	_T("0,@4,7637,@5");

			Handle one;
			one.position		=	_T("topLeft,#0");
			one.yrange			=	_T("0,@8"); 
			Handles.push_back (one);

			Handle two;
			two.position		=	_T("bottomRight,#1");
			two.yrange			=	_T("@9,@10"); 
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
			Path				=	_T("m@9,nfqx@0@0l@0@7qy0@4@0@8l@0@6qy@9,21600em@10,nfqx@5@0l@5@7qy21600@4@5@8l@5@6qy@10,21600em@9,nsqx@0@0l@0@7qy0@4@0@8l@0@6qy@9,21600l@10,21600qx@5@6l@5@8qy21600@4@5@7l@5@0qy@10,xe");

			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("val width"));
			Formulas.push_back(_T("val height"));
			Formulas.push_back(_T("prod width 1 2"));
			Formulas.push_back(_T("prod height 1 2"));
			Formulas.push_back(_T("sum width 0 #0"));
			Formulas.push_back(_T("sum height 0 #0"));
			Formulas.push_back(_T("sum @4 0 #0"));
			Formulas.push_back(_T("sum @4 #0 0"));
			Formulas.push_back(_T("prod #0 2 1"));
			Formulas.push_back(_T("sum width 0 @9"));
			Formulas.push_back(_T("prod #0 9598 32768"));
			Formulas.push_back(_T("sum height 0 @11"));
			Formulas.push_back(_T("sum @11 #0 0"));
			Formulas.push_back(_T("sum width 0 @13"));

			AdjustmentValues	=	_T("1800");
			ConnectorLocations	=	_T("@3,0;0,@4;@3,@2;@1,@4");
			TextBoxRectangle	=	_T("@13,@11,@14,@12");

			Limo				=	_T("10800,10800");

			Handle one;
			one.position		=	_T("topLeft,#0");
			one.yrange			=	_T("0,5400"); 
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
			Path				=	_T("m@0,nfqx0@0l0@2qy@0,21600em@1,nfqx21600@0l21600@2qy@1,21600em@0,nsqx0@0l0@2qy@0,21600l@1,21600qx21600@2l21600@0qy@1,xe");

			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum width 0 #0"));
			Formulas.push_back(_T("sum height 0 #0"));
			Formulas.push_back(_T("prod @0 2929 10000"));
			Formulas.push_back(_T("sum width 0 @3"));
			Formulas.push_back(_T("sum height 0 @3"));
			Formulas.push_back(_T("val width"));
			Formulas.push_back(_T("val height"));
			Formulas.push_back(_T("prod width 1 2"));
			Formulas.push_back(_T("prod height 1 2"));

			AdjustmentValues	=	_T("1800");
			ConnectorLocations	=	_T("@8,0;0,@9;@8,@7;@6,@9");
			TextBoxRectangle	=	_T("@3,@3,@4,@5");

			Limo				=	_T("10800,10800");

			Handle one;
			one.position		=	_T("#0,topLeft");
			one.xrange			=	_T("0,10800"); 
			Handles.push_back (one);
		}
	};


}