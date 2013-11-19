#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
	class BentConnector2: public ShapeType
	{
	public:
		BentConnector2() : ShapeType(msosptBentConnector2)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,l21600,r,21600e");
			//AdjustmentValues	=	_T("10800");

			Filled				=	true;
		}
	};

	class BentConnector3: public ShapeType
	{
	public:
		BentConnector3() : ShapeType(msosptBentConnector3)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,l@0,0@0,21600,21600,21600e");
			AdjustmentValues	=	_T("10800");

			Filled				=	true;

			Formulas.push_back(_T("val #0"));

			Handle one;
			one.position		=	_T("#0,center");
			Handles.push_back (one);
		}
	};

	class BentConnector4: public ShapeType
	{
	public:
		BentConnector4() : ShapeType(msosptBentConnector4)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,l@0,0@0,21600,21600,21600e");
			AdjustmentValues	=	_T("10800");

			Filled				=	true;

			Formulas.push_back(_T("val #0"));

			Handle one;
			one.position		=	_T("#0,center");
			Handles.push_back (one);
		}
	};

	class BentConnector5 : public ShapeType
	{
	public:
		BentConnector5() : ShapeType(msosptBentConnector5)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,l@0,0@0,21600,21600,21600e");
			AdjustmentValues	=	_T("10800");

			Filled				=	true;

			Formulas.push_back(_T("val #0"));

			Handle one;
			one.position		=	_T("#0,center");
			Handles.push_back (one);
		}
	};

	
	class CurvedConnector2: public ShapeType
	{
	public:
		CurvedConnector2() : ShapeType(msosptCurvedConnector2)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e");

			Filled				=	true;

			Formulas.push_back(_T("mid #0 0"));
			Formulas.push_back(_T("val #0"));		
			Formulas.push_back(_T("mid #0 21600"));

			Handle one;
			one.position		=	_T("#0,center");
			Handles.push_back (one);
		}
	};
	class CurvedConnector3: public ShapeType
	{
	public:
		CurvedConnector3() : ShapeType(msosptCurvedConnector3)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e");

			Filled				=	true;

			Formulas.push_back(_T("mid #0 0"));
			Formulas.push_back(_T("val #0"));		
			Formulas.push_back(_T("mid #0 21600"));

			Handle one;
			one.position		=	_T("#0,center");
			Handles.push_back (one);
		}
	};
	class CurvedConnector4 : public ShapeType
	{
	public:
		CurvedConnector4() : ShapeType(msosptCurvedConnector4)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e");

			Filled				=	true;

			Formulas.push_back(_T("mid #0 0"));
			Formulas.push_back(_T("val #0"));		
			Formulas.push_back(_T("mid #0 21600"));

			Handle one;
			one.position		=	_T("#0,center");
			Handles.push_back (one);
		}
	};
	class CurvedConnector5 : public ShapeType
	{
	public:
		CurvedConnector5() : ShapeType(msosptCurvedConnector5)
		{
			ShapeConcentricFill =	true;
			Joins				=	miter;
			Path				=	_T("m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e");

			Filled				=	true;

			Formulas.push_back(_T("mid #0 0"));
			Formulas.push_back(_T("val #0"));		
			Formulas.push_back(_T("mid #0 21600"));

			Handle one;
			one.position		=	_T("#0,center");
			Handles.push_back (one);
		}
	};
}