#pragma once
#include "BinaryReader.h"
//#include "ElementSettings.h"
#include "BaseShape.h"
//#include "../../../Attributes.h"
#include "FormulaShape.h"

#include "../../../../Common/DocxFormat/Source/SystemUtility/File.h"

namespace NSCustomVML
{
    using namespace NSGuidesVML;

	class CSegment
	{
	public:
		RulesType m_eRuler;
		WORD m_nCount;

	public:
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
		void Read(POLE::Stream* pStream)
		{
			WORD mem = StreamUtils::ReadWORD(pStream);

			BYTE type = mem & 0x07;
			if (type <= 4)
			{
				m_eRuler	= (RulesType)type;
				m_nCount	= (mem >> 3) & 0x1FFF;
				return;
			}

			mem = (mem >> 3) & 0x1FFF;
			type = mem & 0x1F;

			switch (type)
			{
			case 0x00:
				{
					m_eRuler = rtLineTo;
					break;
				}
			case 0x01:
				{
					m_eRuler = rtAngleEllipseTo;
					break;
				}
			case 0x02:
				{
					m_eRuler = rtAngleEllipse;
					break;
				}
			case 0x03:
				{
					m_eRuler = rtArcTo;
					break;
				}
			case 0x04:
				{
					m_eRuler = rtArc;
					break;
				}
			case 0x05:
				{
					m_eRuler = rtClockwiseArcTo;
					break;
				}
			case 0x06:
				{
					m_eRuler = rtClockwiseArc;
					break;
				}
			case 0x07:
				{
					m_eRuler = rtEllipticalQuadrX;
					break;
				}
			case 0x08:
				{
					m_eRuler = rtEllipticalQuadrY;
					break;
				}
			case 0x09:
				{
					m_eRuler = rtQuadrBesier;
					break;
				}
			case 0x0A:
				{
					m_eRuler = rtNoFill;
					break;
				}
			case 0x0B:
				{
					m_eRuler = rtNoStroke;
					break;
				}
			case 0x15:
				{
					m_eRuler = rtFillColor;
					break;
				}
			case 0x16:
				{
					m_eRuler = rtLineColor;
					break;
				}
			default:
				{
					m_eRuler = rtCurveTo;
				}
			};

			m_nCount = (mem >> 5) & 0x00FF;
		}
		void Read(CBinaryReader& oReader)
		{
			WORD mem = oReader.ReadWORD();

			BYTE type = (mem >> 13 & 0x07);
			if (type <= 4)
			{
				m_eRuler	= (RulesType)type;
				m_nCount	= (mem & 0x1FFF);
				//m_nCount = (WORD)GetCountPoints2(m_eRuler);
				m_nCount = (WORD)GetCountPoints2(m_eRuler, m_nCount);
				return;
			}

			type = (mem >> 8) & 0x1F;
			mem = mem & 0xFF;

			switch (type)
			{
			case 0x00:
				{
					m_eRuler = rtLineTo;
					break;
				}
			case 0x01:
				{
					m_eRuler = rtAngleEllipseTo;
					break;
				}
			case 0x02:
				{
					m_eRuler = rtAngleEllipse;
					break;
				}
			case 0x03:
				{
					m_eRuler = rtArcTo;
					break;
				}
			case 0x04:
				{
					m_eRuler = rtArc;
					break;
				}
			case 0x05:
				{
					m_eRuler = rtClockwiseArcTo;
					break;
				}
			case 0x06:
				{
					m_eRuler = rtClockwiseArc;
					break;
				}
			case 0x07:
				{
					m_eRuler = rtEllipticalQuadrX;
					break;
				}
			case 0x08:
				{
					m_eRuler = rtEllipticalQuadrY;
					break;
				}
			case 0x09:
				{
					m_eRuler = rtQuadrBesier;
					break;
				}
			case 0x0A:
				{
					m_eRuler = rtNoFill;
					break;
				}
			case 0x0B:
				{
					m_eRuler = rtNoStroke;
					break;
				}
			case 0x0C:
			case 0x10:
				{
					m_eRuler = rtLineTo;
					break;
				}
			case 0x0D:
			case 0x0E:
			case 0x0F:
			case 0x11:
			case 0x12:
			case 0x13:
			case 0x14:
				{
					m_eRuler = rtCurveTo;
					break;
				}
			case 0x15:
				{
					m_eRuler = rtFillColor;
					break;
				}
			case 0x16:
				{
					m_eRuler = rtLineColor;
					break;
				}
			default:
				{
					m_eRuler = rtCurveTo;
				}
			};

			m_nCount = (WORD)mem;
			m_nCount = (WORD)GetCountPoints2(m_eRuler, m_nCount);
			//m_nCount = (WORD)GetCountPoints2(m_eRuler);
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

		LONG m_lShapeWidth;
		LONG m_lShapeHeight;


		CGuide()
		{
			m_eType = ftVal;

			m_param_type1 = 0;
			m_param_type2 = 0;
			m_param_type3 = 0;

			m_param_value1 = 0;
			m_param_value2 = 0;
			m_param_value3 = 0;

			m_lShapeWidth	= ShapeSizeVML;
			m_lShapeHeight	= ShapeSizeVML;
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

			m_lShapeWidth	= oSrc.m_lShapeWidth;
			m_lShapeHeight	= oSrc.m_lShapeHeight;

			return *this;
		}

		void SetToFormula(NSGuidesVML::CFormula& oFormula)
		{
			oFormula.m_eFormulaType = m_eType;

			SetParam(m_param_type1, m_param_value1, oFormula.m_eType1, oFormula.m_lParam1);
			SetParam(m_param_type2, m_param_value2, oFormula.m_eType2, oFormula.m_lParam2);
			SetParam(m_param_type3, m_param_value3, oFormula.m_eType3, oFormula.m_lParam3);
		}

		void Read(POLE::Stream* pStream)
		{
			WORD ftType = StreamUtils::ReadWORD(pStream);

			m_eType = FormulaType(ftType & 0x1FFF);

			m_param_type1 = (BYTE)(ftType & 0x04);
			m_param_type2 = (BYTE)(ftType & 0x02);
			m_param_type3 = (BYTE)(ftType & 0x01);

			m_param_value1 = StreamUtils::ReadWORD(pStream);
			m_param_value2 = StreamUtils::ReadWORD(pStream);
			m_param_value3 = StreamUtils::ReadWORD(pStream);
		}
		void Read(CBinaryReader& oReader)
		{
			WORD ftType = oReader.ReadWORD();

			m_eType = FormulaType(ftType & 0x1FFF);

			m_param_type1 = (BYTE)(ftType & 0x04);
			m_param_type2 = (BYTE)(ftType & 0x02);
			m_param_type3 = (BYTE)(ftType & 0x01);

			m_param_value1 = oReader.ReadWORD();
			m_param_value2 = oReader.ReadWORD();
			m_param_value3 = oReader.ReadWORD();
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
	private:
		RulesType m_ePath;

        std::vector<Aggplus::POINT>		m_arVertices;
		std::vector<CSegment>			m_arSegments;
		std::vector<CGuide>				m_arGuides;
		std::vector<LONG>*				m_pAdjustValues;

        bool m_bIsVerticesPresent;
        bool m_bIsPathPresent;

		//CBrush	m_oBrush;
		//CPen	m_oPen;

	public:
		CCustomVML() : m_arVertices(), m_arSegments(), m_arGuides(), m_pAdjustValues(NULL)
		{
			m_ePath = rtCurveTo/*rtLineTo*/;
			
            m_bIsVerticesPresent	= false;
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
			m_bIsVerticesPresent	= oSrc.m_bIsVerticesPresent;

			//m_oBrush	= oSrc.m_oBrush;
			//m_oPen		= oSrc.m_oPen;

			return *this;
		}

		~CCustomVML()
		{
		}

	public:
		bool IsCustom()
		{
			return (m_bIsVerticesPresent && m_bIsPathPresent);
		}

	public:
		void SetPath(RulesType ePath)
		{
			m_ePath = ePath;
                        m_bIsPathPresent = true;
		}
		//void LoadVertices(CProperty* pProperty)
		//{
		//	CBinaryReader oReader(pProperty->m_pOptions, pProperty->m_lValue);

		//	m_arVertices.clear();
		//	
		//	WORD lCount = (WORD)(pProperty->m_lValue / 8);
		//	if (pProperty->m_bIsTruncated)
		//	{
		//		lCount = (WORD)(pProperty->m_lValue / 4);
		//	}

		//	if (lCount > 0)
		//	{
		//		m_bIsVerticesPresent = true;
		//	}

		//	for (WORD lIndex = 0; lIndex < lCount; ++lIndex)
		//	{
  //              Aggplus::POINT oPoint;
		//		if (pProperty->m_bIsTruncated)
		//		{
		//			oPoint.x = (short)oReader.ReadWORD();
		//			oPoint.y = (short)oReader.ReadWORD();
		//		}
		//		else
		//		{
		//			oPoint.x = oReader.ReadLONG();
		//			oPoint.y = oReader.ReadLONG();
		//		}

		//		LONG lMinF = (LONG)0x80000000;
		//		LONG lMaxF = (LONG)0x8000007F;
		//		if (lMinF <= (DWORD)oPoint.x)
		//		{
		//			int nGuideIndex = (DWORD)oPoint.x - 0x80000000;	

		//			bool b = false;
		//		}
		//		if (lMinF <= (DWORD)oPoint.y)
		//		{
		//			int nGuideIndex = (DWORD)oPoint.y - 0x80000000;					

		//			bool b = false;
		//		}

		//		m_arVertices.push_back(oPoint);
		//	}
		//}

		//void LoadAHs(CProperty* pProperty)
		//{
		//}
		//void LoadSegments(CProperty* pProperty)
		//{
		//	CBinaryReader oReader(pProperty->m_pOptions, pProperty->m_lValue);
		//	m_arSegments.clear();

		//	WORD lCount = (WORD)(pProperty->m_lValue / 2);

		//	if (lCount > 0)
		//	{
  //                              m_bIsPathPresent = true;
		//	}

		//	for (WORD lIndex = 0; lIndex < lCount; ++lIndex)
		//	{
		//		CSegment oInfo;
		//		oInfo.Read(oReader);

		//		if (0 == oInfo.m_nCount)
		//		{
		//			if ((rtEnd		!= oInfo.m_eRuler) &&
		//				(rtNoFill	!= oInfo.m_eRuler) &&
		//				(rtNoStroke != oInfo.m_eRuler) &&
		//				(rtClose	!= oInfo.m_eRuler))
		//			{
		//				continue;
		//			}
		//		}

		//		//if (rtClose == oInfo.m_eRuler)
		//		//{
		//		//	// проводим линию					
		//		//	CSegment oInfo2(rtLineTo, 1);
		//		//	m_arSegments.push_back(oInfo2);
		//		//}

		//		m_arSegments.push_back(oInfo);
		//	}
		//}
		//void LoadGuides(CProperty* pProperty)
		//{
		//	CBinaryReader oReader(pProperty->m_pOptions, pProperty->m_lValue);
		//	WORD lCount = (WORD)(pProperty->m_lValue / 4);

		//	for (WORD lIndex = 0; lIndex < lCount; ++lIndex)
		//	{
		//		CGuide oInfo;
		//		oInfo.Read(oReader);
		//		m_arGuides.push_back(oInfo);
		//	}

		//}
		void LoadAdjusts(LONG lIndex, LONG lValue)
		{
			if (NULL == m_pAdjustValues)
				return;
			
			LONG lCount = (LONG)m_pAdjustValues->size();
			
			while (lCount <= lIndex)
			{
				m_pAdjustValues->push_back(0);
				lCount = (LONG)m_pAdjustValues->size();
			}

			(*m_pAdjustValues)[lIndex] = lValue;
		}

		void SetAdjusts(std::vector<LONG>* pList)
		{
			m_pAdjustValues = pList;
		}

		void ToCustomShape(CBaseShape* pShape, NSGuidesVML::CFormulasManager& oManager)
		{
			if ((NULL == pShape) || (!IsCustom()))
				return;

			oManager.Clear();
			// сначала заполним формулы
			for (size_t nIndex = 0; nIndex < m_arGuides.size(); ++nIndex)
			{
				NSGuidesVML::CFormula oF;
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

					wchar_t buf[2]={};
					if (nGuideIndex_x >= 0 ) 
					{
						_itow(m_arGuides[nGuideIndex_x].m_param_value1, buf, 10);
						strPath += std::wstring(buf) + L",";
					}
					else
					{
						_itow(m_arVertices[nIndex].x, buf, 10);
						strPath += std::wstring(buf) + L",";
					}
					if (nGuideIndex_y >= 0)
					{
						_itow(m_arGuides[nGuideIndex_y].m_param_value1, buf, 10);
						strPath += std::wstring(buf) + L",";
					}
					else
					{
						_itow(m_arVertices[nIndex].y, buf, 10);
						strPath += std::wstring(buf) + L",";
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
								//m_oPen.Color	= (DWORD)m_arVertices[nStart].x;	odooooo
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

						wchar_t buf[20]={};
						if (nGuideIndex_x >= 0 )
						{
							_itow(m_arGuides[nGuideIndex_x].m_param_value1, buf, 10);
							strPath += std::wstring(buf) + L",";
						}
						else
						{
							_itow(m_arVertices[nV].x, buf, 10);
							strPath += std::wstring(buf) + L",";
						}
						if (nGuideIndex_y >= 0)
						{
							_itow(m_arGuides[nGuideIndex_y].m_param_value1, buf, 10);
							strPath += std::wstring(buf) + L",";
						}
						else
						{
							_itow(m_arVertices[nV].y, buf, 10);
							strPath += std::wstring(buf) + L",";
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
