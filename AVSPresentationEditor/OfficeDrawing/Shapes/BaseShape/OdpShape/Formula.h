#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "stdafx.h"
#include <math.h>
#include "./../Common.h"
#include "./../../../../../Common/DocxFormat/Source/XML/XmlUtils.h"
#include "Parser.h"

namespace NSGuidesOdp
{
	const double dNonDefResult = 27273042316900;
	const long NonDefResult = 0xFFFFFF;
	const double RadKoef = M_PI/10800000.0;

	class CFormulaManager
	{
	private:
		CSimpleMap<CString, long> mapAdjustments;
		

	public:
		CSimpleMap<CString, long> mapGuides;
		CSimpleArray<CString> strGuides;

	private:
		CSimpleArray<long>* Adjustments;
		CSimpleArray<double>* Guides;

	public:

		double GetNum (CString str)
		{
			long numGuide = mapGuides.FindKey(str);
			long numAdj = mapAdjustments.FindKey(str);
			if (numGuide>=0)
				return numGuide;
			else if ( numAdj >=0)
				return numAdj;
			
			return 0;
		}

		CFormulaManager(CSimpleArray<long>& a, CSimpleArray<double>& g)
		{
			Adjustments = &a;
			Guides = &g;

			AddGuide(_T("left"), _T("0"));
			AddGuide(_T("top"), _T("0"));
			AddGuide(_T("right"), _T("21600"));
			AddGuide(_T("bottom"), _T("21600"));
			AddGuide(_T("width"), _T("right - left"));
			AddGuide(_T("height"), _T("bottom - top"));
		}

		~CFormulaManager()
		{
			mapAdjustments.RemoveAll();
			mapGuides.RemoveAll();
			strGuides.RemoveAll();
			Adjustments->RemoveAll();
			Guides->RemoveAll();
		}

		CFormulaManager& operator =(const CFormulaManager& manager)
		{
			mapAdjustments.RemoveAll();
			for(int i = 0; i < manager.mapAdjustments.GetSize(); i++)
				mapAdjustments.Add(manager.mapAdjustments.GetKeyAt(i), manager.mapAdjustments.GetValueAt(i));
			mapGuides.RemoveAll();
			for(int i = 0; i < manager.mapGuides.GetSize(); i++)
				mapGuides.Add(manager.mapGuides.GetKeyAt(i), manager.mapGuides.GetValueAt(i));

			strGuides.RemoveAll();
			for(int i = 0; i < manager.strGuides.GetSize(); i++)
				strGuides.Add(manager.strGuides[i]);

			Adjustments->RemoveAll();
			for(int i = 0; i < manager.Adjustments->GetSize(); i++)
				Adjustments->Add((*manager.Adjustments)[i]);
			Guides->RemoveAll();
			for(int i = 0; i < manager.Guides->GetSize(); i++)
				Guides->Add((*manager.Guides)[i]);

			return *this;
		}

		void AddAdjustment(const CString& name, const long value)
		{
			long num = mapAdjustments.FindKey(name);
			if(num >= 0)
			{
				(*Adjustments)[mapAdjustments.GetValueAt(num)] = value;
				return;
			}
			Adjustments->Add(value);
			mapAdjustments.Add(name, Adjustments->GetSize() - 1);
		}

		void AddGuide(const CString& name, const CString& fmla)
		{
			long num = mapGuides.FindKey(name);
			if(num >= 0)
			{
				strGuides[mapGuides.GetValueAt(num)] = fmla;
				(*Guides)[mapGuides.GetValueAt(num)] = dNonDefResult;
				return;
			}
			strGuides.Add(fmla);
			Guides->Add(dNonDefResult);
			mapGuides.Add(name, strGuides.GetSize() - 1);
		}

		double GetValue(CString str)
		{
			long numGuide = mapGuides.FindKey(str);
			long numAdj = mapAdjustments.FindKey(str);
			if(numGuide >= 0)
			{
				double res = (*Guides)[mapGuides.GetValueAt(numGuide)];
				if(res < dNonDefResult)
					return res;
				TParser parser;
				parser.Compile(strGuides[mapGuides.GetValueAt(numGuide)], *this);
				parser.Evaluate();
				parser.Decompile();
				(*Guides)[mapGuides.GetValueAt(numGuide)] = parser.GetResult();
				return parser.GetResult();
			}
			if(numAdj >= 0)
			{
				return (*Adjustments)[mapAdjustments.GetValueAt(numAdj)];
			}
			return XmlUtils::GetInteger(CString(str));
		}

		void Clear()
		{
			//for(long i = 0; i < Adjustments.GetSize(); i++)
			//	Adjustments[i] = NonDefResult;
			for(long i = 0; i < Guides->GetSize(); i++)
				(*Guides)[i] = dNonDefResult;
		}

		void ReCalculateGuides()
		{
			Clear();
			for(long i = 0; i < strGuides.GetSize(); i++)
			{
				TParser parser;
				parser.Compile(strGuides[i], *this);
				parser.Evaluate();
				parser.Decompile();
				(*Guides)[i] = parser.GetResult();
			}
		}
	};
}
