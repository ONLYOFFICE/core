#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
	class TextboxType: public ShapeType
	{
	public:
		TextboxType () : ShapeType(msosptTextBox)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,l,21600r21600,l21600,xe");
		}

		virtual ~TextboxType()
		{

		}
	};

	class TextStop : public ShapeType
	{
	public:
		TextStop () : ShapeType(msosptTextStop)
		{
			Joins				=	miter;
			Path				=	_T("m0@4l@2,l@3,l21600@4em0@5l@2,21600l@3,21600l21600@5e");

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

		virtual ~TextStop ()
		{

		}
	};
}