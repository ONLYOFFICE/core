#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "math.h"

const double ShapeSize		= 43200.0;
const LONG ShapeSizeVML		= 21600;
const double RadKoef = M_PI/10800000.0;

#ifndef		pow2_16
#define		pow2_16			65536
#endif

namespace NSMath
{
	inline LONG round(double dVal)
	{
		return (LONG)(2 * dVal) - (LONG)(dVal);
	}
}

namespace NSStringUtils
{
	static bool IsDigit(const TCHAR& c)
	{
		return (((c >= '0') && (c <= '9')) || (c == '-'));
	}
	static bool IsAlpha(const TCHAR& c)
	{
		return (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')));
	}
	static bool IsNumber(CString str)
	{
		for (int nIndex = 0; nIndex < str.GetLength(); ++nIndex)
		{
			if (!IsDigit(str[nIndex]))
			{
				return false;
			}
		}
		return true;
	}

	static CString ToString(LONG val)
	{
		CString str = _T("");
		str.Format(_T("%d"), val);
		return str;
	}
	
	static void ParseString(CString strDelimeters, CString strSource, 
		CSimpleArray<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayResults)
			return;

		if (bIsCleared)
			pArrayResults->RemoveAll();

		CString resToken;
		int curPos= 0;

		resToken = strSource.Tokenize(strDelimeters, curPos);
		while (resToken != _T(""))
		{
			pArrayResults->Add(resToken);
			resToken = strSource.Tokenize(strDelimeters, curPos);
		};

	}
	static void ParseString(CSimpleArray<char>* pArrayDelimeters, CString strSource, 
		CSimpleArray<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayDelimeters)
			return;

		CString strDelimeters = _T("");
		for (int nIndex = 0; nIndex < pArrayDelimeters->GetSize(); ++nIndex)
			strDelimeters += (*pArrayDelimeters)[nIndex];

		return ParseString(strDelimeters, strSource, pArrayResults, bIsCleared);
	}
	static void ParsePath(CString strSource, CSimpleArray<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayResults)
			return;
		
		CString strPath = strSource;
		//strPath.Replace(_T(" "), _T(","));
		for (int nIndex = 0; nIndex < strPath.GetLength(); ++nIndex)
		{
			if (nIndex == (strPath.GetLength() - 1))
				continue;

			if (IsAlpha(strPath[nIndex]) && (',' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
				strPath.Insert(nIndex + 1, '0');
				++nIndex;
			}
			else if ((',' == strPath[nIndex]) && (',' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, '0');
				++nIndex;
			}
			else if ((',' == strPath[nIndex]) && IsAlpha(strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, '0');
				++nIndex;
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && IsDigit(strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && IsAlpha(strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('@' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('#' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('@' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('#' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('$' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('?' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('$' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('?' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if ((IsAlpha(strPath[nIndex]) && IsAlpha(strPath[nIndex + 1])) && ('x' == strPath[nIndex]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
		}

		ParseString(_T(","), strPath, pArrayResults, bIsCleared);
		return;
	}

	static void ParsePath2(CString strSource, CSimpleArray<CString>* pArrayResults, bool bIsCleared = true)
    {
        if (NULL == pArrayResults)
            return;
        
        CString strPath = strSource;
        //strPath.Replace(_T(" "), _T(","));

        int nIndexOld = 0;

        int nLength = strPath.GetLength();
        for (int nIndex = 0; nIndex < nLength; ++nIndex)
        {
            if (nIndex == (nLength - 1))
			{
				pArrayResults->Add(strPath.Mid(nIndexOld));
				//continue;
			}

            if (IsAlpha(strPath[nIndex]) && (',' == strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, ',');
                ++nIndex;
                strPath.Insert(nIndex + 1, '0');
                ++nIndex;*/
                pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
				pArrayResults->Add(_T("0"));
            }
			else if (IsDigit(strPath[nIndex]) && (',' == strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, ',');
                ++nIndex;
                strPath.Insert(nIndex + 1, '0');
                ++nIndex;*/
                pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
            }
            else if ((',' == strPath[nIndex]) && (',' == strPath[nIndex + 1]))
            {
                //strPath.Insert(nIndex + 1, '0');
                //++nIndex;
				pArrayResults->Add(_T("0"));
            }
            else if ((',' == strPath[nIndex]) && IsAlpha(strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, '0');
                ++nIndex;
                strPath.Insert(nIndex + 1, ',');
                ++nIndex;*/
                pArrayResults->Add(_T("0"));
				nIndexOld = nIndex + 1;
            }
			else if ((',' == strPath[nIndex]) && IsDigit(strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, '0');
                ++nIndex;
                strPath.Insert(nIndex + 1, ',');
                ++nIndex;*/
                nIndexOld = nIndex + 1;
            }
            else if (IsAlpha(strPath[nIndex]) && IsDigit(strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, ',');
                ++nIndex;*/
                pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
                nIndexOld = nIndex + 1;
            }
            else if (IsDigit(strPath[nIndex]) && IsAlpha(strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, ',');
                ++nIndex;*/
                pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
                nIndexOld = nIndex + 1;
            }
            else if (IsDigit(strPath[nIndex]) && ('@' == strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, ',');
                ++nIndex;*/
                /*pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
                pArrayResults->Add(_T("@"));

                ++nIndex;
                nIndexOld = nIndex + 1;*/
				pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

                ++nIndex;
                nIndexOld = nIndex;
            }
            else if (IsDigit(strPath[nIndex]) && ('#' == strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, ',');
                ++nIndex;*/
                /*pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
                pArrayResults->Add(_T("#"));

                ++nIndex;
                nIndexOld = nIndex + 1;*/
				pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

                ++nIndex;
                nIndexOld = nIndex;
            }
            else if (IsAlpha(strPath[nIndex]) && ('@' == strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, ',');
                ++nIndex;*/
                pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

                ++nIndex;
                nIndexOld = nIndex;
            }
            else if (IsAlpha(strPath[nIndex]) && ('#' == strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, ',');
                ++nIndex;*/
                pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

                ++nIndex;
                nIndexOld = nIndex;
            }
            else if (('x' == strPath[nIndex]) && IsAlpha(strPath[nIndex + 1]))
            {
                /*strPath.Insert(nIndex + 1, ',');
                ++nIndex;*/

                pArrayResults->Add(_T("x"));
                nIndexOld = nIndex + 1;
            }
        }

        //ParseString(_T(","), strPath, pArrayResults, bIsCleared);
        return;
    }

	static void CheckPathOn_Fill_Stroke(CString& strPath, bool& bFill, bool& bStroke)
	{
		bFill = true;
		bStroke = true;
		int nIndex = strPath.Find(_T("nf"));
		if (-1 != nIndex)
		{
			bFill = false;
			while (-1 != nIndex)
			{
				strPath.Delete(nIndex, 2);
				nIndex = strPath.Find(_T("nf"));
			}
		}
		nIndex = strPath.Find(_T("ns"));
		if (-1 != nIndex)
		{
			bStroke = false;
			while (-1 != nIndex)
			{
				strPath.Delete(nIndex, 2);
				nIndex = strPath.Find(_T("ns"));
			}
		}

		nIndex = strPath.Find(_T("F"));
		if (-1 != nIndex)
		{
			bFill = false;
			while (-1 != nIndex)
			{
				strPath.Delete(nIndex, 2);
				nIndex = strPath.Find(_T("F"));
			}
		}

		nIndex = strPath.Find(_T("S"));
		if (-1 != nIndex)
		{
			bStroke = false;
			while (-1 != nIndex)
			{
				strPath.Delete(nIndex, 2);
				nIndex = strPath.Find(_T("S"));
			}
		}
	}
}