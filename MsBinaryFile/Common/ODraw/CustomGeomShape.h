/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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


#include "BaseShape.h"

#include "FormulaShape.h"

#include "../../../OOXML/SystemUtility/File.h"

namespace NSCustomShapesConvert
{
	class CSegment
	{
	public:
		RulesType	m_eRuler;
		WORD		m_nCount;

		CSegment(RulesType eType = rtMoveTo, WORD nCount = 2)
		{
			m_eRuler = eType;
			m_nCount = nCount;
		}

		CSegment(const CSegment& oSrc)
		{
			m_eRuler = oSrc.m_eRuler;
			m_nCount = oSrc.m_nCount;
		}

		CSegment& operator=(const CSegment& oSrc)
		{
			m_eRuler = oSrc.m_eRuler;
			m_nCount = oSrc.m_nCount;
			
			return (*this);
		}

		~CSegment()
		{
		}
	};

	class CGuide
	{
	public:
		FormulaType m_eType;

		BYTE m_param_type1;
		BYTE m_param_type2;
		BYTE m_param_type3;

		WORD m_param_value1;
		WORD m_param_value2;
		WORD m_param_value3;

		CGuide()
		{
			m_eType = ftVal;

			m_param_type1 = 0;
			m_param_type2 = 0;
			m_param_type3 = 0;

			m_param_value1 = 0;
			m_param_value2 = 0;
			m_param_value3 = 0;

		}

		CGuide(const CGuide& oSrc)
		{
			*this = oSrc;
		}

		CGuide& operator=(const CGuide& oSrc)
		{
			m_eType			= oSrc.m_eType;

			m_param_type1	= oSrc.m_param_type1;
			m_param_type2	= oSrc.m_param_type2;
			m_param_type3	= oSrc.m_param_type3;

			m_param_value1	= oSrc.m_param_value1;
			m_param_value2	= oSrc.m_param_value2;
			m_param_value3	= oSrc.m_param_value3;

			return *this;
		}

		void SetToFormula(NSCustomShapesConvert::CFormula& oFormula)
		{
			oFormula.m_eFormulaType = m_eType;

			SetParam(m_param_type1, m_param_value1, oFormula.m_eType1, oFormula.m_lParam1);
			SetParam(m_param_type2, m_param_value2, oFormula.m_eType2, oFormula.m_lParam2);
			SetParam(m_param_type3, m_param_value3, oFormula.m_eType3, oFormula.m_lParam3);
		}

	private:
		void SetParam(BYTE type, WORD param, ParamType& param_type, LONG& param_value)
		{
			if (0 == type)
			{
				param_type	= ptValue;
				param_value = (LONG)param;
			}
			
			if (0x0140 == param)
			{
                param_type	= ptValue;
				param_value	= ShapeSizeVML / 2;
				return;
			}
			if (0x0141 == param)
			{
				param_type	= ptValue;
				param_value	= ShapeSizeVML / 2;
				return;
			}
			if (0x0142 == param)
			{
				param_type	= ptValue;
				param_value	= ShapeSizeVML;
				return;
			}
			if (0x0143 == param)
			{
				param_type	= ptValue;
				param_value	= ShapeSizeVML;
				return;
			}

			if ((0x0147 <= param) && (param <= 0x014E))
			{
				param_type	= ptAdjust;
				param_value	= (LONG)(param - 0x0147);
				return;
			}
			
			if ((0x0400 <= param) && (param <= 0x047F))
			{
				param_type	= ptFormula;
				param_value	= (LONG)(param - 0x0400);
				return;
			}

			param_type	= ptValue;
			param_value	= ShapeSizeVML;
		}
	};

	class CCustomVML
	{
	public:
		RulesType m_ePath;

        std::vector<Aggplus::POINT>		m_arVertices;
		std::vector<CSegment>			m_arSegments;
		std::vector<CGuide>				m_arGuides;
		std::vector<LONG>*				m_pAdjustValues;

        bool							m_bIsPathPresent;

		CCustomVML() : m_arVertices(), m_arSegments(), m_arGuides(), m_pAdjustValues(NULL)
		{
			m_ePath = rtCurveTo/*rtLineTo*/;
			
            m_bIsPathPresent		= false;
		}
		
		CCustomVML(const CCustomVML& oSrc)
		{
			*this = oSrc;
		}

		CCustomVML& operator=(const CCustomVML& oSrc)
		{
			m_ePath		= oSrc.m_ePath;

			m_arVertices.clear();
			m_arVertices.insert(m_arVertices.end(), oSrc.m_arVertices.begin(), oSrc.m_arVertices.end());

			m_arSegments.clear();
			m_arSegments.insert(m_arSegments.end(), oSrc.m_arSegments.begin(), oSrc.m_arSegments.end());

			m_arGuides.clear();
			m_arGuides.insert(m_arGuides.end(), oSrc.m_arGuides.begin(), oSrc.m_arGuides.end());

			m_pAdjustValues = oSrc.m_pAdjustValues;
			
			m_bIsPathPresent		= oSrc.m_bIsPathPresent;

			return *this;
		}

		~CCustomVML()
		{
		}

		bool IsCustom()
		{
			return (!m_arVertices.empty() && (m_bIsPathPresent || !m_arSegments.empty()));
		}

		void SetPath(RulesType ePath)
		{
			m_ePath = ePath;
			m_bIsPathPresent = true;
		}
		void LoadAHs(unsigned char* buffer, int buffer_size)
		{
		}
		void addSegment(RulesType eRuler, _UINT16	nCount)
		{
			CSegment oInfo(eRuler, nCount);
			m_arSegments.push_back(oInfo);
		}
		void addGuide(CGuide & oInfo)
		{
			m_arGuides.push_back(oInfo);
		}
		void addAdjust(int lIndex, int lValue)
		{
			if (NULL == m_pAdjustValues)
				return;
			
			int lCount = m_pAdjustValues->size();
			
			while (lCount <= lIndex)
			{
				m_pAdjustValues->push_back(0);
				lCount = m_pAdjustValues->size();
			}

			(*m_pAdjustValues)[lIndex] = lValue;
		}

		void SetAdjusts(std::vector<LONG>* pList)
		{
			m_pAdjustValues = pList;
		}

		void ToCustomShape(CBaseShape* pShape, NSCustomShapesConvert::CFormulasManager& oManager)
		{
			if ((NULL == pShape) || (!IsCustom()))
				return;

			oManager.Clear();
			// сначала заполним формулы
			for (size_t nIndex = 0; nIndex < m_arGuides.size(); ++nIndex)
			{
				NSCustomShapesConvert::CFormula oF;
				m_arGuides[nIndex].SetToFormula(oF);
				oManager.AddFormula(oF);
			}

			oManager.Clear(m_pAdjustValues);

			std::wstring strPath = _T("");

			bool bBreak = false;

			long lMinF = (_INT32)0x80000000;
			long lMaxF = (_INT32)0x8000007F;

			int nGuideIndex_x , nGuideIndex_y;

			if (0 == m_arSegments.size())
			{
				strPath = GetRulerVML(m_ePath);
				
				for (size_t nIndex = 0; nIndex < m_arVertices.size(); ++nIndex)
				{
					nGuideIndex_x = nGuideIndex_y = -1;
				
					if (lMaxF > m_arVertices[nIndex].x )	nGuideIndex_x = (DWORD)m_arVertices[nIndex].x - (DWORD)lMinF;	
					if (lMaxF > m_arVertices[nIndex].y )	nGuideIndex_y = (DWORD)m_arVertices[nIndex].y - (DWORD)lMinF;	

					if (nGuideIndex_x >= 0 && nGuideIndex_x < (int)m_arGuides.size()) 
					{
                        strPath += std::to_wstring(m_arGuides[nGuideIndex_x].m_param_value1) + L",";
					}
					else
					{
                       strPath += std::to_wstring(m_arVertices[nIndex].x) + L",";
					}
					if (nGuideIndex_y >= 0 && nGuideIndex_y < (int)m_arGuides.size())
					{
                        strPath += std::to_wstring(m_arGuides[nGuideIndex_y].m_param_value1) + L",";
					}
					else
					{
                        strPath += std::to_wstring(m_arVertices[nIndex].y) + L",";
					}
				}
				strPath.erase(strPath.length() - 1);
			}
			else
			{
				size_t nStart	= 0;
				size_t nEnd		= 0;
				for (size_t nS = 0; nS < m_arSegments.size(); ++nS)
				{
					if (bBreak)
					{
						if ((rtEnd		!= m_arSegments[nS].m_eRuler) &&
							(rtNoFill	!= m_arSegments[nS].m_eRuler) &&
							(rtNoStroke != m_arSegments[nS].m_eRuler) &&
							(rtClose	!= m_arSegments[nS].m_eRuler))
						{
							strPath += _T("e");
							break;
						}
					}

					if ((rtFillColor == m_arSegments[nS].m_eRuler) || (rtLineColor == m_arSegments[nS].m_eRuler))
					{
						if (nStart <  m_arVertices.size())
						{
							if (rtFillColor == m_arSegments[nS].m_eRuler)
							{
								//m_oBrush.Color1 = (DWORD)m_arVertices[nStart].x; todooooo
								//m_oBrush.Color2 = (DWORD)m_arVertices[nStart].y;
							}
							else
							{
								//m_oPen.Color	= (DWORD)m_arVertices[nStart].x;	todooooo
							}
						}
						nEnd = nStart + m_arSegments[nS].m_nCount;
						if (nEnd > m_arVertices.size())
							nEnd = m_arVertices.size();
						nStart = nEnd;

						if (nEnd == m_arVertices.size())
						{
							bBreak = true;
						}
						continue;
					}

					strPath += GetRulerVML(m_arSegments[nS].m_eRuler);
					
					nEnd = nStart + m_arSegments[nS].m_nCount;
					if (nEnd > m_arVertices.size())
						nEnd = m_arVertices.size();

					for (size_t nV = nStart; nV < nEnd; ++nV)
					{
						nGuideIndex_x = nGuideIndex_y = -1;
					
						if (lMaxF > m_arVertices[nV].x )	nGuideIndex_x = (DWORD)m_arVertices[nV].x - (DWORD)lMinF;	
						if (lMaxF > m_arVertices[nV].y )	nGuideIndex_y = (DWORD)m_arVertices[nV].y - (DWORD)lMinF;	

						if (nGuideIndex_x >= 0 && nGuideIndex_x < (int)m_arGuides.size() )
						{
                            strPath += std::to_wstring(m_arGuides[nGuideIndex_x].m_param_value1) + L",";
						}
						else
						{
                            strPath += std::to_wstring(m_arVertices[nV].x) + L",";
						}
						if (nGuideIndex_y >= 0 && nGuideIndex_y < (int)m_arGuides.size())
						{
                            strPath += std::to_wstring(m_arGuides[nGuideIndex_y].m_param_value1) + L",";
						}
						else
						{
                            strPath += std::to_wstring(m_arVertices[nV].y) + L",";
						}
					}

					if (nEnd != nStart)
					{
						strPath.erase(strPath.length() - 1);
					}
					nStart = nEnd;

					if (nEnd == m_arVertices.size())
					{
						bBreak = true;
					}
				}
			}
			oManager.CalculateResults();
			pShape->LoadPathList(strPath);
		}
	};
}
