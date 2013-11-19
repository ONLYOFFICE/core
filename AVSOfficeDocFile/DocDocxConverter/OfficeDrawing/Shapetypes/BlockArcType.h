#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
	class BlockArcType : public ShapeType
	{
	public:
		BlockArcType () : ShapeType(msosptBlockArc)
		{
			ShapeConcentricFill	=	false;
			Joins				=	miter;
			Path				=	_T("al10800,10800@0@0@2@14,10800,10800,10800,10800@3@15xe");

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
			Formulas.push_back(_T("sum 10800 0 #1"));
			Formulas.push_back(_T("prod #1 1 2"));
			Formulas.push_back(_T("sum @18 5400 0"));
			Formulas.push_back(_T("cos @19 #0"));
			Formulas.push_back(_T("sin @19 #0"));
			Formulas.push_back(_T("sum @20 10800 0"));
			Formulas.push_back(_T("sum @21 10800 0"));
			Formulas.push_back(_T("sum 10800 0 @20"));
			Formulas.push_back(_T("sum #1 10800 0"));
			Formulas.push_back(_T("if @9 @17 @25"));
			Formulas.push_back(_T("if @9 0 21600"));
			Formulas.push_back(_T("cos 10800 #0"));
			Formulas.push_back(_T("sin 10800 #0"));
			Formulas.push_back(_T("sin #1 #0"));
			Formulas.push_back(_T("sum @28 10800 0"));
			Formulas.push_back(_T("sum @29 10800 0"));
			Formulas.push_back(_T("sum @30 10800 0"));
			Formulas.push_back(_T("if @4 0 @31"));
			Formulas.push_back(_T("if #0 @34 0"));
			Formulas.push_back(_T("if @6 @35 @31"));
			Formulas.push_back(_T("sum 21600 0 @36"));
			Formulas.push_back(_T("if @4 0 @33"));
			Formulas.push_back(_T("if #0 @38 @32"));
			Formulas.push_back(_T("if @6 @39 0"));
			Formulas.push_back(_T("if @4 @32 21600"));
			Formulas.push_back(_T("if @6 @41 @33"));

			AdjustmentValues	=	_T("11796480,5400");
			ConnectorLocations	=	_T("10800,@27;@22,@23;10800,@26;@24,@23");
			TextBoxRectangle	=	_T("@36,@40,@37,@42");

			Handle one;
			one.position		=	_T("#1,#0");
			one.polar			=	_T("10800,10800"); 
			one.radiusrange		=	_T("0,10800"); 
			Handles.push_back (one);
		}
	};
}