#pragma once

#include "Formula.h"

double NSGuidesOOXML::CFormula::Calculate(NSGuidesOOXML::CFormulaManager& pManager)
{
	if ((0 == m_lIndex) || (-m_lIndex > pManager.Guides->size()) || (m_lIndex > pManager.Adjustments->size()))
		return 0.0;
	if((m_lIndex < 0) && (dNonDefResult > (*pManager.Guides)[-m_lIndex-1]))
		return (*pManager.Guides)[-m_lIndex-1];
	if((m_lIndex > 0) && (NonDefResult != (*pManager.Adjustments)[m_lIndex-1]))
		return (*pManager.Adjustments)[m_lIndex-1];
	
	double a = pManager.GetValue(m_lParam1);
	double b = pManager.GetValue(m_lParam2);
	double c = pManager.GetValue(m_lParam3);
	//double c = (double)c1;

	double dRes = 0.0;

	try
	{
		// теперь нужно просто посчитать
		switch (m_eFormulaType)
		{
		case ftOOXMLSum:			{ dRes = a + b - c;						break; }
		case ftOOXMLProduct:		{/*if(0==c1)c=1.0;*/ dRes = (a * b)/c;	break; }
		case ftOOXMLAddDivide:		{/*if(0==c1)c=1.0;*/ dRes = (a + b)/c;	break; }
		case ftOOXMLAbsolute:		{ dRes = abs(a);						break; }
		case ftOOXMLMin:			{ dRes = min(a, b);						break; }
		case ftOOXMLMax:			{ dRes = max(a, b);						break; }
		case ftOOXMLIf:				{ dRes = (a > 0) ? b : c;				break; }
		case ftOOXMLSqrt:			{ dRes = sqrt(a);						break; }
		case ftOOXMLMod:			{ dRes = sqrt(a*a + b*b + c*c);			break; }
		case ftOOXMLSin:			{ dRes = a * sin(b * RadKoef);			break; }
		case ftOOXMLCos:			{ dRes = a * cos(b * RadKoef);			break; }
		case ftOOXMLTan:			{ dRes = a * tan(b * RadKoef);			break; }
		case ftOOXMLAtan2:			{ dRes = atan2(b,a)/RadKoef;			break; }
		case ftOOXMLSinatan2:		{ dRes = a * sin(atan2(c,b));			break; }
		case ftOOXMLCosatan2:		{ dRes = a * cos(atan2(c,b));			break; }
		case ftOOXMLPin:			{ dRes = (b < a) ? a :((b > c) ? c : b);break; }
		case ftOOXMLVal:			{ dRes = a;								break; }
		default: break;
		};
	}
	catch (...)
	{
		dRes = 0.0;
	}

	//long lResult = (long)dRes;
	if(m_lIndex < 0)
	{
		(*pManager.Guides)[-m_lIndex-1] = dRes;
		// переопределим формулу
		pManager.mapGuides.insert(std::pair<CString, long>(m_sName, -m_lIndex-1));
	}
	else
		(*pManager.Adjustments)[m_lIndex-1] = (long)dRes;
	return dRes;
}
