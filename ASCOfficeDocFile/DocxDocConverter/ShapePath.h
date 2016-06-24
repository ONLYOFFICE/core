/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once


#include <math.h>

#include "OfficeArt/Common.h"
#include "OfficeArt/MSOArray.h"
#include "OfficeArt/Enumerations.h"

#include "../../Common/DocxFormat/Source/XML/xmlutils.h"
#include "../Common/FormatUtils.h"

#define M_PI				3.14159265358979323846

const double ShapeSize		=	43200.0;
const LONG ShapeSizeVML		=	21600;
const double RadKoef		=	M_PI/10800000.0;

#define	pow2_16	65536

namespace ASCDocFileFormat
{
	enum FormulaType
	{
		// VML
		ftSum			= 0,	// a + b - c  
		ftProduct		= 1,	// a * b / c  
		ftMid			= 2,	// (a + b) / 2  
		ftAbsolute		= 3,	// abs(a)  
		ftMin			= 4,	// min(a,b)  
		ftMax			= 5,	// max(a,b)  
		ftIf			= 6,	// if  a > 0 ? b : c  
		ftMod			= 7,	// sqrt(a)  
		ftAtan2			= 8,	// atan2(b,a)  
		ftSin			= 9,	// a * sin(b)  
		ftCos			= 10,	// a * cos(b)  
		ftCosatan2		= 11,	// a * cos(atan2(c,b))
		ftSinatan2		= 12,	// a * sin(atan2(c,b))  
		ftSqrt			= 13,	// sqrt(a*a + b*b + c*c)
		ftSumangle		= 14,	// a + b° - c°
		ftEllipse		= 15,	// c * sqrt(1-(a/b)2)  
		ftTan			= 16,	// a * tan(b)  
		ftVal			= 17	// a
	};	

	enum ParamType
	{
		ptFormula			= 0,
		ptAdjust			= 1,
		ptValue				= 2
	};

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

	static void ParseString(CString strDelimeters, CString strSource, std::vector<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayResults)
			return;

		if (bIsCleared)
			pArrayResults->clear();

		CString resToken;
		int curPos= 0;

		resToken = strSource.Tokenize(strDelimeters, curPos);
		while (resToken != _T(""))
		{
			pArrayResults->push_back(resToken);
			resToken = strSource.Tokenize(strDelimeters, curPos);
		};
	}
	static void ParseString(std::vector<char>*	pArrayDelimeters,	CString strSource, std::vector<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayDelimeters)
			return;

		CString strDelimeters = _T("");
		for (int nIndex = 0; nIndex < pArrayDelimeters->size(); ++nIndex)
			strDelimeters += (*pArrayDelimeters)[nIndex];

		return ParseString(strDelimeters, strSource, pArrayResults, bIsCleared);
	}

	inline static void ParsePath(CString strSource, std::vector<CString>* pArrayResults, bool bIsCleared = true)
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

	static LONG GetValue(CString strParam, ParamType& ptType, bool& bRes, 
		long lShapeWidth = ShapeSizeVML, long lShapeHeight = ShapeSizeVML)
	{
		ptType = ptValue;
		bRes = true;
		if ('#' == strParam[0])
		{
			ptType = ptAdjust;
			return (LONG)XmlUtils::GetInteger(strParam.Mid(1));
		}
		else if ('@' == strParam[0])
		{
			ptType = ptFormula;
			return (LONG)XmlUtils::GetInteger(strParam.Mid(1));
		}
		else if (!IsNumber(strParam))
		{
			if (_T("width") == strParam)
			{
				return lShapeWidth;
			}
			else if (_T("height") == strParam)
			{
				return lShapeHeight;
			}
			else if (_T("pixelWidth") == strParam)
			{
				return lShapeWidth;
			}
			else if (_T("pixelHeight") == strParam)
			{
				return lShapeHeight;
			}
			else if (_T("pixelLineWidth") == strParam || _T("lineDrawn") == strParam)
			{
				return 1;
			}
			else
			{
				bRes = false;
				return 0;
			}
		}	
		else
		{
			ptType = ptValue;
			return (LONG)XmlUtils::GetInteger(strParam);
		}
	}

	static FormulaType GetFormula(CString strName, bool& bRes)
	{
		bRes = true;
		if		(_T("sum") == strName)									return ftSum;
		else if ((_T("prod") == strName) || (_T("product") == strName)) return ftProduct;
		else if (_T("mid") == strName)									return ftMid;
		else if ((_T("absolute") == strName) || (_T("abs") == strName)) return ftAbsolute;
		else if (_T("min") == strName)									return ftMin;
		else if (_T("max") == strName)									return ftMax;
		else if (_T("if") == strName)									return ftIf;
		else if (_T("sqrt") == strName)									return ftSqrt;
		else if (_T("mod") == strName)									return ftMod;
		else if (_T("sin") == strName)									return ftSin;
		else if (_T("cos") == strName)									return ftCos;
		else if (_T("tan") == strName)									return ftTan;
		else if (_T("atan2") == strName)								return ftAtan2;
		else if (_T("sinatan2") == strName)								return ftSinatan2;
		else if (_T("cosatan2") == strName)								return ftCosatan2;
		else if (_T("sumangle") == strName)								return ftSumangle;
		else if (_T("ellipse") == strName)								return ftEllipse;
		else if (_T("val") == strName)									return ftVal;
		else bRes = false;

		return ftVal;
	}
}

namespace ASCDocFileFormat
{
	class CFormulasManager;
	class CFormula
	{
	public:
		FormulaType m_eFormulaType;
		int m_lIndex;

		LONG m_lParam1;
		ParamType m_eType1;

		LONG m_lParam2;
		ParamType m_eType2;

		LONG m_lParam3;
		ParamType m_eType3;

	private:
		long m_lCountRecurs;

	public:
		CFormula()
		{
			m_eFormulaType = ftSum;
			m_lIndex = 0;
			m_lParam1 = 0;	m_eType1 = ptValue;
			m_lParam2 = 0;	m_eType2 = ptValue;
			m_lParam3 = 0;	m_eType3 = ptValue;

			m_lCountRecurs = 0;
		}

		CFormula(int nIndex)
		{
			m_eFormulaType = ftSum;
			m_lIndex = nIndex;
			m_lParam1 = 0;	m_eType1 = ptValue;
			m_lParam2 = 0;	m_eType2 = ptValue;
			m_lParam3 = 0;	m_eType3 = ptValue;

			m_lCountRecurs = 0;
		}

		CFormula& operator =(const CFormula& oSrc)
		{
			m_eFormulaType	= oSrc.m_eFormulaType;
			m_lIndex		= oSrc.m_lIndex;

			m_lParam1		= oSrc.m_lParam1;
			m_eType1		= oSrc.m_eType1;

			m_lParam2		= oSrc.m_lParam2;
			m_eType2		= oSrc.m_eType2;

			m_lParam3		= oSrc.m_lParam3;
			m_eType3		= oSrc.m_eType3;

			m_lCountRecurs = 0;
			return (*this);
		}

		void FromString(CString strFormula, long lShapeWidth = ShapeSizeVML, long lShapeHeight = ShapeSizeVML)
		{
			std::vector<CString> oArrayParams;
			ParseString(_T(" "), strFormula, &oArrayParams);
			int nCount = oArrayParams.size();
			if (0 >= nCount)
				return;

			bool bRes = true;
			m_eFormulaType = GetFormula(oArrayParams[0], bRes);

			ParamType ptType = ptValue;
			if (1 < nCount)
			{
				m_lParam1 = GetValue(oArrayParams[1], ptType, bRes, lShapeWidth, lShapeHeight);
				m_eType1 = ptType;		
			}
			if (2 < nCount)
			{
				m_lParam2 = GetValue(oArrayParams[2], ptType, bRes, lShapeWidth, lShapeHeight);
				m_eType2 = ptType;		
			}
			if (3 < nCount)
			{
				m_lParam3 = GetValue(oArrayParams[3], ptType, bRes, lShapeWidth, lShapeHeight);
				m_eType3 = ptType;		
			}
		}
		LONG Calculate(CFormulasManager* pManager);
	};

	class CFormulasManager
	{
	public:
		std::vector<LONG>* m_pAdjustments;
		std::vector<LONG> m_arResults;

		std::vector<CFormula> m_arFormulas;

		long m_lShapeWidth;
		long m_lShapeHeight;

	public:
		CFormulasManager() : m_arFormulas(), m_arResults()
		{
			m_pAdjustments	= NULL;
			m_lShapeWidth	= ShapeSizeVML;
			m_lShapeHeight	= ShapeSizeVML;
		}
		CFormulasManager& operator =(const CFormulasManager& oSrc)
		{
			m_pAdjustments	= oSrc.m_pAdjustments;
			m_lShapeWidth	= oSrc.m_lShapeWidth;
			m_lShapeHeight	= oSrc.m_lShapeHeight;

			m_arResults.clear();
			for (int nIndex = 0; nIndex < oSrc.m_arResults.size(); ++nIndex)
			{
				m_arResults.push_back(oSrc.m_arResults[nIndex]);
			}
			m_arFormulas.clear();
			for (int nIndex = 0; nIndex < oSrc.m_arFormulas.size(); ++nIndex)
			{
				m_arFormulas.push_back(oSrc.m_arFormulas[nIndex]);
			}

			return (*this);
		}

		void RemoveAll()
		{
			m_pAdjustments	= NULL;
			m_lShapeWidth	= ShapeSizeVML;
			m_lShapeHeight	= ShapeSizeVML;

			m_arFormulas.clear();
			m_arResults.clear();
		}

		void Clear(std::vector<LONG>* pAdjusts)
		{
			m_pAdjustments = pAdjusts;

			//m_arFormulas.RemoveAll();
			//m_arResults.RemoveAll();
			for (int nIndex = 0; nIndex < m_arResults.size(); ++nIndex)
			{
				m_arResults[nIndex] = 0xFFFFFFFF;
			}
		}
		void AddFormula(CString strFormula)
		{
			CFormula oFormula(m_arFormulas.size());
			oFormula.FromString(strFormula, m_lShapeWidth, m_lShapeHeight);
			m_arFormulas.push_back(oFormula);
			m_arResults.push_back(0xFFFFFFFF);
		}
		void AddFormula(CFormula oFormula)
		{
			oFormula.m_lIndex = m_arFormulas.size();
			m_arFormulas.push_back(oFormula);
			m_arResults.push_back(0xFFFFFFFF);
		}
		void CalculateResults()
		{
			for (int index = 0; index < m_arFormulas.size(); ++index)
			{
				LONG lResult = m_arFormulas[index].Calculate(this);
			}

			//m_pAdjustments = NULL;
			//m_arFormulas.RemoveAll();
		}
	};
}

namespace ASCDocFileFormat
{
	// TODO : add escape segments

	class CPathSegment
	{
	public:
		enum MSOPATHTYPE
		{
			msopathLineTo,
			msopathCurveTo,
			msopathMoveTo,
			msopathClose,
			msopathEnd,
			msopathEscape,
			msopathClientEscape,
			msopathInvalid
		};

		enum MSOPATHESCAPE
		{
			msopathEscapeExtension 				=	0x00000000,	
			msopathEscapeAngleEllipseTo  		=	0x00000001,
			msopathEscapeAngleEllipse 	 		=	0x00000002,
			msopathEscapeArcTo  				=	0x00000003,
			msopathEscapeArc  					=	0x00000004,
			msopathEscapeClockwiseArcTo  		=	0x00000005,
			msopathEscapeClockwiseArc 	 		=	0x00000006,
			msopathEscapeEllipticalQuadrantX  	=	0x00000007,
			msopathEscapeEllipticalQuadrantY 	=	0x00000008,
			msopathEscapeQuadraticBezier 	 	=	0x00000009,
			msopathEscapeNoFill  				=	0x0000000A,
			msopathEscapeNoLine  				=	0x0000000B,
			msopathEscapeAutoLine  				=	0x0000000C,
			msopathEscapeAutoCurve  			=	0x0000000D,
			msopathEscapeCornerLine  			=	0x0000000E,
			msopathEscapeCornerCurve 			=	0x0000000F,
			msopathEscapeSmoothLine  			=	0x00000010,
			msopathEscapeSmoothCurve  			=	0x00000011,
			msopathEscapeSymmetricLine 			=	0x00000012,
			msopathEscapeSymmetricCurve 		=	0x00000013,
			msopathEscapeFreeform 				=	0x00000014,
			msopathEscapeFillColor 				=	0x00000015,
			msopathEscapeLineColor 				=	0x00000016
		};

	public:

		CPathSegment () : m_oBits(2)
		{

		}

		CPathSegment (CString Command, int Segments) : m_oBits(2)
		{
			m_oBits.SetBits<int>(Segments, 0, 13);

			if (CString("m") == Command)
				m_oBits.SetBits<unsigned short>(msopathMoveTo,  13, 3);

			if (CString("l") == Command)
				m_oBits.SetBits<unsigned short>(msopathLineTo,  13, 3);

			if (CString("c") == Command)
				m_oBits.SetBits<unsigned short>(msopathCurveTo,	13, 3);

			if (CString("e") == Command)
				m_oBits.SetBits<unsigned short>(msopathEnd,		13, 3);

			if (CString("x") == Command)
				m_oBits.SetBits<unsigned short>(msopathClose,	13, 3);
		}

		CPathSegment (CString Command, int Escape, int Segments) : m_oBits(2)
		{
			m_oBits.SetBits<int>(Segments, 0, 13);
			
			if (CString("nf") == Command)
				m_oBits.SetBits<MSOPATHESCAPE>(msopathEscapeNoFill, 13, 3);

			if (CString("ns") == Command)
				m_oBits.SetBits<MSOPATHESCAPE>(msopathEscapeNoLine, 13, 3);

			//m_oBits.SetBits<int>(Escape, 3, 5);		
			//m_oBits.SetBits<int>(Segments, 8, 8);			
		}

		inline unsigned short Get()
		{
			return FormatUtils::BytesToUInt16 (m_oBits.GetBytes(), 0, sizeof(unsigned short));
		}

	private:

		ASCDocFormatUtils::BitSet m_oBits;
	};

	class CShapePath
	{
	public:

		CShapePath () : m_bIsSimple (TRUE)
		{
			m_nType	=	-1;
		}

		inline bool IsValid ()
		{
			return (m_nType >= 0) && (m_oPoints.GetSize() > 0) && (m_oSegments.GetSize() > 0);
		}

		inline bool IsSimple ()
		{
			return m_bIsSimple;
		}

		inline bool Build (const std::string& strPath, const std::string& strAdj, const std::string& strFormulas)
		{
			m_oManager.RemoveAll();

			m_oPoints.Clear ();
			m_oSegments.Clear ();

			if (0==strPath.length())
				return FALSE;

			std::vector<CString> oArray;
			ParsePath(CString(strPath.c_str()), &oArray);

			if (oArray.size())
			{
				m_nType	=	1;

				ParamType eParamType = ptValue;
				LONG lValue;
				bool bRes = true;
				CString strCommand;	
				OfficeArt::CPoint32 point;
				OfficeArt::CPoint32 movePt;

				LoadAdjustValuesList (CString(strAdj.c_str()));
				m_oManager.m_pAdjustments	=	&m_arAdjustments;

				LoadGuidesList (CString(strFormulas.c_str()));
				if (m_arAdjustments.size())
					m_oManager.CalculateResults();

				CString oldCommand = L"";

				for (int nIndex = 0; nIndex < oArray.size(); ++nIndex)
				{
					lValue	=	GetFormatedValue (nIndex, oArray);

					if (IsCommand(oArray[nIndex]))
					{
						//ATLTRACE (L"COMMAND : %s\n",oArray[nIndex]);

						strCommand = oArray[nIndex];

						if (CString("x") == strCommand)
						{
							m_oSegments.Add(CPathSegment(strCommand,1).Get());
						}
						else if (CString("e") == strCommand)
						{
							m_oSegments.Add(CPathSegment(strCommand,0).Get());
						}
						else if (CString("n") == oldCommand)
						{
							if (CString("s") == strCommand)
							{
								//m_oSegments.Add(CPathSegment(L"ns", 1, 0).Get());
							}
							else if (CString("f") == strCommand)
							{
								//m_oSegments.Add(CPathSegment(L"nf", 1, 0).Get());
							}
						}

						oldCommand	=	strCommand;
					}
					else
					{
						if (CString("m") == strCommand || CString("l") == strCommand || CString("r") == strCommand || CString("c") == strCommand || CString("v") == strCommand)
						{
							LONG nValue0	=	GetFormatedValue (nIndex, oArray);
							LONG nValue1	=	GetFormatedValue (nIndex + 1, oArray);

							OfficeArt::CPoint32 curPoint = OfficeArt::CPoint32(nValue0,nValue1);

							if (CString("m") == strCommand)
							{
								movePt		=	curPoint;

								m_oSegments.Add(CPathSegment(CString("m"),0).Get());
								m_oPoints.Add(curPoint);

								++nIndex;
							}
							else if(CString("r") == strCommand)
							{
								curPoint.X	+=	point.X;
								curPoint.Y	+=	point.Y;

								m_oPoints.Add(curPoint);
								m_oSegments.Add(CPathSegment(CString("l"),1).Get());

								++nIndex;
							}
							else if(CString("c") == strCommand)
							{
								m_bHaveCurves	=	TRUE;

								LONG nValue2	=	GetFormatedValue (nIndex + 2, oArray);
								LONG nValue3	=	GetFormatedValue (nIndex + 3, oArray);
								LONG nValue4	=	GetFormatedValue (nIndex + 4, oArray);
								LONG nValue5	=	GetFormatedValue (nIndex + 5, oArray);

								OfficeArt::CPoint32 curPoint2 = OfficeArt::CPoint32(nValue2, nValue3);
								OfficeArt::CPoint32 curPoint3 = OfficeArt::CPoint32(nValue4, nValue5);

								m_oPoints.Add(curPoint);
								m_oPoints.Add(curPoint2);
								m_oPoints.Add(curPoint3);

								m_oSegments.Add(CPathSegment(CString("c"),1).Get());

								nIndex += 5;

								point = curPoint3;
								continue;
							}
							else if(CString("v") == strCommand)
							{
								m_bHaveCurves	=	TRUE;

								LONG nValue2	=	GetFormatedValue (nIndex + 2, oArray);
								LONG nValue3	=	GetFormatedValue (nIndex + 3, oArray);
								LONG nValue4	=	GetFormatedValue (nIndex + 4, oArray);
								LONG nValue5	=	GetFormatedValue (nIndex + 5, oArray);

								OfficeArt::CPoint32 curPoint2 = OfficeArt::CPoint32(nValue2, nValue3);
								OfficeArt::CPoint32 curPoint3 = OfficeArt::CPoint32(nValue4,nValue5);

								curPoint.X	+=	point.X;
								curPoint.Y	+=	point.Y;

								curPoint2.X	+=	point.X;
								curPoint2.Y	+=	point.Y;

								curPoint3.X	+=	point.X;
								curPoint3.Y	+=	point.Y;

								m_oPoints.Add(curPoint);
								m_oPoints.Add(curPoint2);
								m_oPoints.Add(curPoint3);

								m_oSegments.Add(CPathSegment(CString("c"),1).Get());

								nIndex += 5;

								point = curPoint3;
								continue;
							}
							else
							{
								m_oPoints.Add(curPoint);
								m_oSegments.Add(CPathSegment(strCommand,1).Get());

								++nIndex;
							}

							point	=	curPoint;
						}
						else
						{
							m_bIsSimple	=	FALSE;
						}
					}
				}

				return TRUE;
			}

			return FALSE;
		}

		inline int GetType () const
		{
			if (m_bHaveCurves)
				return 3;

			return m_nType;
		}

		inline OfficeArt::CMSOArray<OfficeArt::CPoint32>& GetPoints()
		{
			return m_oPoints;
		}

		inline OfficeArt::CMSOArray<unsigned short>& GetSegments()
		{
			return m_oSegments;
		}

		// FORMULA
		inline bool LoadAdjustValuesList(const CString& xml)
		{
			m_arAdjustments.clear ();

			std::vector<CString> arAdj;
			ParseString(_T(","), xml, &arAdj);

			m_arAdjustments.clear();
			for (int nIndex = 0; nIndex < arAdj.size(); ++nIndex)
				m_arAdjustments.push_back((LONG)XmlUtils::GetInteger(arAdj[nIndex]));

			return true;
		}

		inline bool LoadGuidesList(const CString& strXml)
		{
			XmlUtils::CXmlNode oNodeGuides;
			if (oNodeGuides.FromXmlString(strXml))
			{
				XmlUtils::CXmlNodes oList;
				if (oNodeGuides.GetNodes(_T("f"), oList))
				{
					int lCount = oList.GetCount();
					for (int nIndex = 0; nIndex < lCount; ++nIndex)
					{
						XmlUtils::CXmlNode oNodeFormula;
						oList.GetAt(nIndex, oNodeFormula);

						m_oManager.AddFormula(oNodeFormula.GetAttributeOrValue(_T("eqn")));
					}
				}

				return true;
			}

			return false;
		}

	private:

		inline LONG GetFormatedValue(LONG nIndex, const std::vector<CString>& oArray)
		{
			ParamType eParamType	=	ptValue;
			bool bRes				=	true;
			LONG lValue				=	GetValue (oArray[nIndex], eParamType, bRes);

			switch (eParamType)
			{
			case ptFormula:
				{ 
					lValue			=	m_oManager.m_arResults[lValue]; 
					break; 
				}
			case ptAdjust: 
				{ 
					lValue = (*(m_oManager.m_pAdjustments))[lValue];
					break;
				}
			default:
				break;
			};

			return lValue;
		}

		inline static void ParsePath (const CString& strSource, std::vector<CString>* pArrayResults, bool bIsCleared = true)
		{
			if (NULL == pArrayResults)
				return;

			CString strPath =	strSource;
			int nIndexOld	=	0;

			int nLength		=	strPath.GetLength();
			for (int nIndex = 0; nIndex < nLength; ++nIndex)
			{
				if (nIndex == (nLength - 1))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld));
				}

				TCHAR strChar	= strPath[nIndex];
				TCHAR strChar2	= strPath[nIndex + 1];
				
				if (' ' == strChar)
					strChar = ',';
				
				if (' ' == strChar2)
					strChar2 = ',';

				if (IsAlpha(strChar) && (',' == strChar2))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));	//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );
					pArrayResults->push_back(_T("0"));											//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );
				}
				else if (IsDigit(strChar) && (',' == strChar2))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));	//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );
				}
				else if ((',' == strChar) && (',' == strChar2))
				{
					pArrayResults->push_back(_T("0"));
				}
				else if ((',' == strChar) && IsAlpha(strChar2))
				{
					pArrayResults->push_back(_T("0"));
					nIndexOld = nIndex + 1;
				}
				else if ((',' == strChar) && IsDigit(strChar2))
				{
					nIndexOld = nIndex + 1;
				}
				else if (IsAlpha(strChar) && IsDigit(strChar2))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));		//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );
					nIndexOld = nIndex + 1;
				}
				else if (IsDigit(strChar) && IsAlpha(strChar2))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));		//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );
					nIndexOld = nIndex + 1;
				}
				else if (IsDigit(strChar) && ('@' == strChar2))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));		//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (IsDigit(strChar) && ('#' == strChar2))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));		//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (IsAlpha(strChar) && ('@' == strChar2))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));		//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (IsAlpha(strChar) && ('#' == strChar2))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));		//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (('x' == strChar) && IsAlpha(strChar2))
				{
					pArrayResults->push_back(_T("x"));											//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );
					nIndexOld = nIndex + 1;
				}
				else if (IsAlpha(strChar) && IsAlpha(strChar2))
				{
					//if ((('n'==strChar) && ('f'==strChar2)) || (('n'==strChar) && ('s'==strChar2)))
					//{
					//	pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 2));		//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );
					//	++nIndex;
					//	nIndexOld = nIndex + 1;
					//}
					//else
					{
						pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));		//ATLTRACE (L"COMMAND : %s\n",pArrayResults->operator[](pArrayResults->GetSize()-1) );
						nIndexOld = nIndex + 1;
					}
				}
			}

			return;
		}

		inline static bool IsDigit (const TCHAR& c)
		{
			return (((c >= '0') && (c <= '9')) || (c == '-'));
		}
		inline static bool IsAlpha (const TCHAR& c)
		{
			return (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')));
		}

		inline static bool IsNumber (const CString& str)
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

		inline static bool IsCommand (const CString& str)
		{
			if (str == CString(_T("m")) || str == CString(_T("l")) || str == CString(_T("x")) ||
				str == CString(_T("e")) || str == CString(_T("r")) || str == CString(_T("c")) || str == CString(_T("v")) ||
				str == CString(_T("nf")) || str == CString(_T("ns")) || str == CString(_T("n")) || str == CString(_T("s")) || str == CString(_T("f")) )
				return true;

			return false;
		}

		inline static LONG GetValue (const CString& strParam, ParamType& ptType, bool& bRes, long lShapeWidth = ShapeSizeVML, long lShapeHeight = ShapeSizeVML)
		{
			ptType = ptValue;
			bRes = true;
			if ('#' == strParam[0])
			{
				ptType = ptAdjust;
				return (LONG)XmlUtils::GetInteger(strParam.Mid(1));
			}
			else if ('@' == strParam[0])
			{
				ptType = ptFormula;
				return (LONG)XmlUtils::GetInteger(strParam.Mid(1));
			}
			else if (!IsNumber(strParam))
			{
				if (_T("width") == strParam)
				{
					return lShapeWidth;
				}
				else if (_T("height") == strParam)
				{
					return lShapeHeight;
				}
				else
				{
					bRes = false;
					return 0;
				}
			}	
			else
			{
				ptType = ptValue;
				return (LONG)XmlUtils::GetInteger(strParam);
			}
		}

	public:

		int		m_nType;
		bool	m_bIsSimple;

		bool	m_bHaveCurves;		

		OfficeArt::CMSOArray<OfficeArt::CPoint32>	m_oPoints;
		OfficeArt::CMSOArray<unsigned short>		m_oSegments;

		CFormulasManager		m_oManager;
		std::vector<long>		m_arAdjustments;
		std::vector<double>		Guides;

	};
}
