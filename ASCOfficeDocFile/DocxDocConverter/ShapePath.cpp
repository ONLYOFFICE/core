#include "stdafx.h"

#include "ShapePath.h"

namespace ASCDocFileFormat
{
	LONG CFormula::Calculate(CFormulasManager* pManager)
	{
		if ((0 > m_lIndex) || (m_lIndex >= pManager->m_arResults.GetSize()))
			return 0;
		if (0xFFFFFFFF != pManager->m_arResults[m_lIndex])
		{
			return pManager->m_arResults[m_lIndex];
		}

		LONG lResult = 0;

		LONG lGuidesCount	= pManager->m_arFormulas.GetSize();
		LONG lAdjCount		= pManager->m_pAdjustments->GetSize();

		LONG a1 = m_lParam1;
		if (ptFormula == m_eType1)
		{
			a1 = (m_lParam1 >= lGuidesCount) ? 0 : pManager->m_arFormulas[m_lParam1].Calculate(pManager);
		}
		else if (ptAdjust == m_eType1)
		{
			a1 = (m_lParam1 >= lAdjCount) ? 0 : (*(pManager->m_pAdjustments))[m_lParam1];
		}

		LONG b1 = m_lParam2;
		if (ptFormula == m_eType2)
		{
			b1 = (m_lParam2 >= lGuidesCount) ? 0 : pManager->m_arFormulas[m_lParam2].Calculate(pManager);
		}
		else if (ptAdjust == m_eType2)
		{
			b1 = (m_lParam2 >= lAdjCount) ? 0 : (*(pManager->m_pAdjustments))[m_lParam2];
		}

		LONG c1 = m_lParam3;
		if (ptFormula == m_eType3)
		{
			c1 = (m_lParam3 >= lGuidesCount) ? 0 : pManager->m_arFormulas[m_lParam3].Calculate(pManager);
		}
		else if (ptAdjust == m_eType3)
		{
			c1 = (m_lParam3 >= lAdjCount) ? 0 : (*(pManager->m_pAdjustments))[m_lParam3];
		}

		double a = (double)a1;
		double b = (double)b1;
		double c = (double)c1;

		double dRes = 0.0;

		try
		{
			// теперь нужно просто посчитать
			switch (m_eFormulaType)
			{
			case ftSum:			{ dRes = a + b - c;				break; }
			case ftProduct:		{ 
				if (0 == c) 
					c = 1; 

				dRes = a * b / c; 
				break; 
								}
			case ftMid:			{ dRes = (a + b) / 2.0;			break; }
			case ftAbsolute:	{ dRes = abs(a);				break; }

			case ftMin:			{ dRes = min(a, b);				break; }
			case ftMax:			{ dRes = max(a, b);				break; }
			case ftIf:			{ dRes = (a > 0) ? b : c;		break; }
			case ftSqrt:		{ dRes = sqrt(a);				break; }
			case ftMod:			{ dRes = sqrt(a*a + b*b + c*c); break; }

			case ftSin:			{ 
				//dRes = a * sin(b);
				//dRes = a * sin(b / pow2_16);
				dRes = a * sin(M_PI * b / (pow2_16 * 180));
				break; 
								}
			case ftCos:			{ 
				//dRes = a * cos(b);
				//dRes = a * cos(b / pow2_16);
				dRes = a * cos(M_PI * b / (pow2_16 * 180));
				break; 
								}
			case ftTan:			{ 
				//dRes = a * tan(b);			
				dRes = a * tan(M_PI * b / (pow2_16 * 180));
				break; 
								}
			case ftAtan2:		{ 
				dRes =  180 * pow2_16 * atan2(b,a) / M_PI;	
				break; 
								}

			case ftSinatan2:	{ dRes = a * sin(atan2(c,b));	break; }
			case ftCosatan2:	{ dRes = a * cos(atan2(c,b));	break; }

			case ftSumangle:	{ 
				//dRes = a  + b - c;
				dRes = a + b * pow2_16 - c * pow2_16;

				/*while (23592960 < dRes)
				{
				dRes -= 23592960;
				}
				while (-23592960 > dRes)
				{
				dRes += 23592960;
				}*/

				break; 
								}
			case ftEllipse:		{ 
				if (0 == b)
					b = 1;
				dRes = c * sqrt(1-(a*a/(b*b)));	
				break; 
								}
			case ftVal:			{ dRes = a;						break; }
			default: break;
			};
		}
		catch (...)
		{
			dRes = 0;
		}

		lResult = (LONG)dRes;
		pManager->m_arResults[m_lIndex] = lResult;
		return lResult;
	}
}