#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

//#include "stdafx.h"
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
		std::map<CString, long> mapAdjustments;
		

	public:
		std::map<CString, long> mapGuides;
		std::vector<CString> strGuides;

	private:
		std::vector<long>* Adjustments;
		std::vector<double>* Guides;

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

		CFormulaManager(std::vector<long>& a, std::vector<double>& g)
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
			mapAdjustments.clear();
			mapGuides.clear();
			strGuides.clear();
			Adjustments->clear();
			Guides->clear();
		}

		CFormulaManager& operator =(const CFormulaManager& manager)
		{
			mapAdjustments.clear();
			for (std::map<CString, long>::const_iterator pPair = manager.mapAdjustments.begin(); pPair != manager.mapAdjustments.end(); ++pPair)
			{
				mapAdjustments.insert(std::pair<CString, long>(pPair->first, pPair->second));
			}
			
			mapGuides.clear();
			for (std::map<CString, long>::const_iterator pPair = manager.mapGuides.begin(); pPair != manager.mapGuides.end(); ++pPair)
			{
				mapGuides.insert(std::pair<CString, long>(pPair->first, pPair->second));
			}

			strGuides.clear();
			for(int i = 0; i < manager.strGuides.size(); i++)
				strGuides.push_back(manager.strGuides[i]);

			Adjustments->clear();
			for(int i = 0; i < manager.Adjustments->size(); i++)
				Adjustments->push_back((*manager.Adjustments)[i]);
			Guides->clear();
			for(int i = 0; i < manager.Guides->size(); i++)
				Guides->push_back((*manager.Guides)[i]);

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
			Adjustments->push_back(value);
			mapAdjustments.push_back(name, Adjustments->size() - 1);
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
			strGuides.push_back(fmla);
			Guides->push_back(dNonDefResult);
			mapGuides.insert(name, strGuides.size() - 1);
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
			//for(long i = 0; i < Adjustments.size(); i++)
			//	Adjustments[i] = NonDefResult;
			for(long i = 0; i < Guides->size(); i++)
				(*Guides)[i] = dNonDefResult;
		}

		void ReCalculateGuides()
		{
			Clear();
			for(long i = 0; i < strGuides.size(); i++)
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
