#pragma once

#include "BaseShape.h"
#include "CustomGeomShape.h"
#include "../ShapeType.h"
#include "../../../ASCOfficePPTXFile/Editor/Drawing/Shapes/BaseShape/PPTShape/Enums.h"

class CCustomShape : public NSGuidesVML::CBaseShape
{
public:
	oox::MSOSPT						m_eType;
	NSGuidesVML::CFormulasManager	m_oManager;

	NSCustomVML::CCustomVML			m_oCustomVML;
	
	std::wstring					m_strPathLimoX;
	std::wstring					m_strPathLimoY;

	std::vector<std::wstring> m_arStringTextRects;
public:
	CCustomShape() : CBaseShape()
	{
		m_eType = oox::msosptNotPrimitive;
	}

	~CCustomShape()
	{
	}

	virtual void AddGuide(const std::wstring& strGuide)
	{
		m_oManager.AddFormula(strGuide);
	}

	virtual bool LoadAdjustHandlesList(const std::wstring& xml)
	{
		return true;
	}

	//void LoadAHList(XmlUtils::CXmlNode& oNode)
	//{
	//	m_arHandles.clear();

	//	XmlUtils::CXmlNodes oNodes;
	//	if (oNode.GetNodes(_T("v:h"), oNodes))
	//	{
	//		int nCount = oNodes.GetCount();
	//		for (int i = 0; i < nCount; ++i)
	//		{
	//			XmlUtils::CXmlNode oNodeH;
	//			oNodes.GetAt(i, oNodeH);

	//			CHandle_ oH;
	//			oH.polar		= oNodeH.GetAttribute(_T("polar"));
	//			oH.position		= oNodeH.GetAttribute(_T("position"));
	//			oH.radiusrange	= oNodeH.GetAttribute(_T("radiusrange"));
	//			oH.switchHandle = oNodeH.GetAttribute(_T("switch"));
	//			oH.xrange		= oNodeH.GetAttribute(_T("xrange"));
	//			oH.yrange		= oNodeH.GetAttribute(_T("yrange"));

	//			m_arHandles.push_back(oH);
	//		}
	//	}
	//}

	virtual bool LoadConnectorsList(const std::wstring& xml)
	{
		return true;
	}

	virtual bool LoadTextRect(const std::wstring& xml)
	{
		std::vector<std::wstring> oArray;
		NSStringUtils::ParseString(_T(";"), xml, oArray);

		LONG lCount = (LONG)oArray.size();

		if (lCount <= 0)
			return true;

		m_arStringTextRects.clear();
		for (LONG i = 0; i < lCount; ++i)
		{
			m_arStringTextRects.push_back(oArray[i]);
		}
		
		return true;
	}

	virtual bool LoadPathList(const std::wstring& xml)
	{
		m_strPath = xml;
	
		m_oPath.FromXML(xml, m_oManager);
		return true;
	}

	virtual bool SetAdjustment(long index, long value)
	{
		if (index < m_arAdjustments.size() && index >= 0)
		{
			m_arAdjustments[index] =  value;
			return TRUE;
		}
		return FALSE;
	}

	virtual void ReCalculate()
	{
		m_oManager.Clear(&m_arAdjustments);
		
		m_oManager.CalculateResults();

		if (_T("") == m_strPath)
			return;

		LoadPathList(m_strPath);
	}

	static CCustomShape* CreateByType(oox::MSOSPT type);

	virtual bool SetProperties(CBaseShape* Shape)
	{
		if(Shape == NULL)
			return false;

		m_oManager = ((CCustomShape*)Shape)->m_oManager;

		//m_strPathLimoX = ((CCustomShape*)Shape)->m_strPathLimoX;
		//m_strPathLimoY = ((CCustomShape*)Shape)->m_strPathLimoY;

		//m_arStringTextRects.clear();
		//m_arStringTextRects.insert(m_arStringTextRects.end(), ((CCustomShape*)Shape)->m_arStringTextRects.begin(), ((CCustomShape*)Shape)->m_arStringTextRects.end());
		
		return CBaseShape::SetProperties(Shape);
	}

	bool SetShapeType(oox::MSOSPT type)
	{
		CCustomShape* l_pShape = CreateByType(type);
		if(l_pShape != NULL)
		{
			m_eType = type;

			SetProperties(l_pShape);
			delete l_pShape;
			return true;
		}

		m_eType = oox::msosptNotPrimitive;
		return false;
	}

//protected:
//
//	bool GetPos(std::wstring str, LONG& lValue)
//	{
//		if (str.GetLength() == 0)
//			return false;
//
//		TCHAR mem = str.GetAt(0);
//
//		bool bFormula = false;
//		if ('@' == mem)
//		{
//			bFormula = true;
//			str.Delete(0);
//		}
//		
//		if (!NSStringUtils::IsNumber(str))
//			return false;
//
//		lValue = 0;
//		lValue = _ttoi(str);
//
//		if (bFormula)
//		{
//			if (lValue >= 0 || lValue < m_oManager.m_arResults.size())
//			{
//				lValue = m_oManager.m_arResults[lValue];
//				return true;
//			}
//			return false;
//		}
//
//		return true;
//	}
};
