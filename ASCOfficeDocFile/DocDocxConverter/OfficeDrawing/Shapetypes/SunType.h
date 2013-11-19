#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
	class SunType : public ShapeType
	{
	public:
		SunType () : ShapeType(msosptSun)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
			Path				=	_T("m21600,10800l@15@14@15@18xem18436,3163l@17@12@16@13xem10800,l@14@10@18@10xem3163,3163l@12@13@13@12xem,10800l@10@18@10@14xem3163,18436l@13@16@12@17xem10800,21600l@18@15@14@15xem18436,18436l@16@17@17@16xem10800@19qx@19,10800,10800@20@20,10800,10800@19xe");

			Formulas.push_back(_T("sum 10800 0 #0"));
			Formulas.push_back(_T("prod @0 30274 32768"));
			Formulas.push_back(_T("prod @0 12540 32768"));
			Formulas.push_back(_T("sum @1 10800 0"));
			Formulas.push_back(_T("sum @2 10800 0"));
			Formulas.push_back(_T("sum 10800 0 @1"));
			Formulas.push_back(_T("sum 10800 0 @2"));
			Formulas.push_back(_T("prod @0 23170 32768"));
			Formulas.push_back(_T("sum @7 10800 0"));
			Formulas.push_back(_T("sum 10800 0 @7"));
			Formulas.push_back(_T("prod @5 3 4"));
			Formulas.push_back(_T("prod @6 3 4"));
			Formulas.push_back(_T("sum @10 791 0"));
			Formulas.push_back(_T("sum @11 791 0"));
			Formulas.push_back(_T("sum @11 2700 0"));
			Formulas.push_back(_T("sum 21600 0 @10"));
			Formulas.push_back(_T("sum 21600 0 @12"));
			Formulas.push_back(_T("sum 21600 0 @13"));
			Formulas.push_back(_T("sum 21600 0 @14"));
			Formulas.push_back(_T("val #0"));
			Formulas.push_back(_T("sum 21600 0 #0"));

			AdjustmentValues	=	_T("5400");
			ConnectorLocations	=	_T("0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0");
			TextBoxRectangle	=	_T("@9,@9,@8,@8");

			Handle one;
			one.position		=	_T("#0,center");
			one.xrange			=	_T("2700,10125"); 
			Handles.push_back (one);
		}
	};
}