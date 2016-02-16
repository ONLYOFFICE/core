#pragma once

#include "BaseShape.h"
#include "CustomGeomShape.h"
#include "../ShapeType.h"
#include "../../../ASCOfficePPTXFile/Editor/Drawing/Shapes/BaseShape/PPTShape/Enums.h"

class CCustomShape : public NSGuidesVML::CBaseShape
{
public:
	oox::MSOSPT			m_eType;
	NSGuidesVML::CFormulasManager	m_oManager;

	NSCustomVML::CCustomVML m_oCustomVML;
	
	std::wstring m_strPathLimoX;
	std::wstring m_strPathLimoY;

	std::vector<std::wstring> m_arStringTextRects;
public:
	CCustomShape() : CBaseShape()
	{
		m_eType = oox::msosptNotPrimitive;

	}

	~CCustomShape()
	{
	}

	//virtual bool LoadFromXML(const std::wstring& xml)
	//{
	//	XmlUtils::CXmlNode oNodePict;
	//	if (oNodePict.FromXmlString(xml))
	//	{
	//		if (_T("shape") == oNodePict.GetName())
	//		{
	//			std::wstring id = oNodePict.GetAttributeOrValue(_T("type"));
	//			if (id != _T(""))
	//			{
	//				SetShapeType((PPTShapes::oox::MSOSPT)Strings::ToInteger(id));
	//			}
	//			else
	//			{
	//				XmlUtils::CXmlNode oNodeTemplate;
	//				if (oNodePict.GetNode(_T("template"), oNodeTemplate))
	//				{	
	//					std::wstring strAdj = oNodeTemplate.GetAttributeOrValue(_T("adj"));
	//					LoadAdjustValuesList(strAdj);

	//					XmlUtils::CXmlNode oNodeGuides;
	//					if (oNodeTemplate.GetNode(_T("v:formulas"), oNodeGuides))
	//					{
	//						LoadGuidesList(oNodeGuides.GetXml());
	//					}

	//					std::wstring strPath = oNodeTemplate.GetAttributeOrValue(_T("path"));
	//					LoadPathList(strPath);
	//				}
	//			}

	//			XmlUtils::CXmlNode oNodeGuides;
	//			if (oNodePict.GetNode(_T("path"), oNodeGuides))
	//			{
	//				std::wstring strPath = oNodeGuides.GetAttributeOrValue(_T("val"));
	//				LoadPathList(strPath);
	//			}

	//			/*XmlUtils::CXmlNode oNodeShapeType;
	//			if (oNodePict.GetNode(_T("v:shapetype"), oNodeShapeType))
	//			{
	//				std::wstring strAdj = oNodeShapeType.GetAttributeOrValue(_T("adj"));
	//				LoadAdjustValuesList(strAdj);

	//				XmlUtils::CXmlNode oNodeGuides;
	//				if (oNodeShapeType.GetNode(_T("v:formulas"), oNodeGuides))
	//				{
	//					LoadGuidesList(oNodeGuides.GetXml());
	//				}

	//				std::wstring strPath = oNodeShapeType.GetAttributeOrValue(_T("path"));
	//				LoadAdjustValuesList(strPath);
	//			}
	//			else
	//			{
	//				XmlUtils::CXmlNode oNodeShape;
	//				if (oNodePict.GetNode(_T("v:shape"), oNodeShape))
	//				{
	//					std::wstring strAdj = oNodeShape.GetAttributeOrValue(_T("adj"));
	//					LoadAdjustValuesList(strAdj);

	//					XmlUtils::CXmlNode oNodeGuides;
	//					if (oNodeShape.GetNode(_T("v:formulas"), oNodeGuides))
	//					{
	//						LoadGuidesList(oNodeGuides.GetXml());
	//					}

	//					std::wstring strPath = oNodeShape.GetAttributeOrValue(_T("path"));
	//					LoadPathList(strPath);
	//				}
	//			}*/

	//			return true;
	//		}
	//	}

	//	return false;
	//}

	//virtual bool LoadFromXML(XmlUtils::CXmlNode& oNodePict)
	//{		
	//	std::wstring id = oNodePict.GetAttributeOrValue(_T("type"));
	//	bool isPathList = false;
	//	if (id != _T(""))
	//	{
	//		SetShapeType((oox::MSOSPT)XmlUtils::GetInteger(id));
	//	}
	//	else
	//	{
	//		XmlUtils::CXmlNode oNodeTemplate;
	//		if (oNodePict.GetNode(_T("template"), oNodeTemplate))
	//		{	
	//			std::wstring strAdj = oNodeTemplate.GetAttributeOrValue(_T("adj"));
	//			LoadAdjustValuesList(strAdj);

	//			XmlUtils::CXmlNode oNodeGuides;
	//			if (oNodeTemplate.GetNode(_T("v:formulas"), oNodeGuides))
	//			{
	//				LoadGuidesList(oNodeGuides.GetXml());
	//			}

	//			std::wstring strPath = oNodeTemplate.GetAttributeOrValue(_T("path"));
	//			if (strPath != _T(""))
	//			{
	//				LoadPathList(strPath);
	//				isPathList = true;
	//			}
	//		}
	//	}

	//	XmlUtils::CXmlNode oNodeGuides;
	//	if (oNodePict.GetNode(_T("path"), oNodeGuides))
	//	{
	//		std::wstring strPath = oNodeGuides.GetAttributeOrValue(_T("val"));
	//		if (strPath != _T(""))
	//		{
	//			LoadPathList(strPath);
	//			isPathList = true;
	//		}
	//	}

	//	if (!isPathList)
	//		ReCalculate();
	//	return true;
	//}

	//virtual bool LoadFromXMLShapeType(XmlUtils::CXmlNode& oNodeShapeType) // vml object
	//{		// из за особенносей форматирования vmlDrawing могут вылезти пустые текстовые значения - value ..
	//	std::wstring sId = oNodeShapeType.GetAttribute(_T("o:spt"));

	//	bool bIsNeedRecalc = true;
	//	if (sId != _T(""))
	//	{
	//		int id = XmlUtils::GetInteger(sId);
	//		if (id > 0)
	//		{
	//			SetShapeType((oox::MSOSPT)id);
	//			//ReCalculate();
	//			m_eType = (oox::MSOSPT)id;	
	//		}
	//	}
	//	//else
	//	{
	//		std::wstring strAdj = oNodeShapeType.GetAttribute(_T("adj"));
	//		if (strAdj != _T(""))
	//			LoadAdjustValuesList(strAdj);

	//		XmlUtils::CXmlNode oNodeGuides;
	//		if (oNodeShapeType.GetNode(_T("v:formulas"), oNodeGuides))
	//		{
	//			LoadGuidesList(oNodeGuides.GetXml());
	//		}

	//		XmlUtils::CXmlNode oNodePath;
	//		if (oNodeShapeType.GetNode(_T("v:path"), oNodePath))
	//		{
	//			std::wstring strTextR = oNodePath.GetAttribute(_T("textboxrect"));
	//			if (strTextR != _T(""))
	//				LoadTextRect(strTextR);
	//		}

	//		XmlUtils::CXmlNode oNodeAHs;
	//		if (oNodeShapeType.GetNode(_T("v:handles"), oNodeAHs))
	//		{
	//			LoadAHList(oNodeAHs);
	//		}

	//		std::wstring strPath = oNodeShapeType.GetAttribute(_T("path"));
	//		if (strPath != _T(""))
	//		{
	//			LoadPathList(strPath);
	//		}
	//	}

	//	std::wstring strFilled = oNodeShapeType.GetAttribute(_T("filled"));
	//	std::wstring strStroked = oNodeShapeType.GetAttribute(_T("stroked"));

	//	if (strFilled != _T(""))
	//	{
	//		if (strFilled == _T("false") || strFilled == _T("f"))
	//			m_bIsFilled = false;
	//		else
	//			m_bIsFilled = true;
	//	}
	//	
	//	if (strStroked != _T(""))
	//	{
	//		if (strStroked == _T("false") || strStroked == _T("f"))
	//			m_bIsStroked = false;
	//		else
	//			m_bIsStroked = true;
	//	}

	//	ReCalculate();
	//	return true;
	//}
	
	//virtual bool LoadAdjustValuesList(const std::wstring& xml)
	//{
	//	std::wstring strXml = xml;
	//	if (0 == strXml.Find((TCHAR)','))
	//		strXml = (_T("defaultAdj") + strXml);
	//	LONG lLastIndex = strXml.GetLength() - 1;
	//	if (lLastIndex >= 0 && lLastIndex == strXml.ReverseFind((TCHAR)','))
	//	{
	//		strXml = strXml + _T("defaultAdj");
	//	}
	//	strXml.Replace(_T(",,"), _T(",defaultAdj,"));

	//	std::vector<std::wstring> arAdj;
	//	NSStringUtils::ParseString(_T(","), strXml, &arAdj);
	//	
	//	std::vector<long> oOldAdj;
	//	int nOldLen = m_arAdjustments.size();
	//	for (int ii = 0; ii < nOldLen; ++ii)
	//		oOldAdj.push_back(m_arAdjustments[ii]);

	//	m_arAdjustments.clear();
	//	for (int nIndex = 0; nIndex < arAdj.size(); ++nIndex)
	//	{
	//		if (_T("defaultAdj") == arAdj[nIndex])
	//		{
	//			if (nIndex >= nOldLen)
	//				m_arAdjustments.push_back(0);
	//			else
	//				m_arAdjustments.push_back(oOldAdj[nIndex]);
	//		}
	//		else
	//		{
	//			m_arAdjustments.push_back((LONG)XmlUtils::GetInteger(arAdj[nIndex]));
	//		}
	//	}

	//	int newLen = m_arAdjustments.size();
	//	for (int i = newLen; i < nOldLen; ++i)
	//		m_arAdjustments.push_back(oOldAdj[i]);
	//	return true;
	//}

	//virtual bool LoadGuidesList(const std::wstring& xml)
	//{
 //          // std::wstring sFormulasTemp;
 //          // sFormulasTemp.Format(_T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">%ls</root>"),xml);
 //       
 //       
 //           std::wstring sBegin(_T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"));
 //       
 //           std::wstring sEnd(_T("</root>"));
 //           std::wstring sFormulasTemp = sBegin + xml+ sEnd;
 //       

 //           XmlUtils::CXmlNode oNodeRoot;
 //           if (oNodeRoot.FromXmlString(sFormulasTemp) == false) return false;

 //           XmlUtils::CXmlNode oNodeGuides;
 //           if (oNodeRoot.GetNode(_T("v:formulas"),oNodeGuides) == false) return false;

 //           if ((_T("v:formulas") == oNodeGuides.GetName()))
 //           {
 //               m_oManager.Clear();

 //               XmlUtils::CXmlNodes oList;
 //               if (oNodeGuides.GetNodes(_T("v:f"), oList))
 //               {
 //                   int lCount = oList.GetCount();
 //                   for (int nIndex = 0; nIndex < lCount; ++nIndex)
 //                   {
 //                       XmlUtils::CXmlNode oNodeFormula;
 //                       oList.GetAt(nIndex, oNodeFormula);

 //                       m_oManager.AddFormula(oNodeFormula.GetAttributeOrValue(_T("eqn")));
 //                   }
 //               }

 //               m_oManager.Clear(&m_arAdjustments);
 //               m_oManager.CalculateResults();
 //               return true;
 //           }

 //           return false;
	//}

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
	
		//if (!m_bIsShapeType)
		//	m_oPath.FromXML(xml, m_oManager);
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

	//virtual std::wstring ToXML(CGeomShapeInfo& GeomInfo, CMetricInfo& MetricInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen)
	//{
	//	if ((_T("") != m_strPathLimoX) || _T("") != m_strPathLimoY)
	//	{
	//		m_strPath = (GeomInfo.m_dWidth >= GeomInfo.m_dHeight) ? m_strPathLimoX : m_strPathLimoY;
	//		ReCalculate();
	//		m_oPath.SetCoordsize(21600, 21600);
	//	}
	//	
	//	return m_oPath.ToXml(GeomInfo, StartTime, EndTime, Pen, Brush, MetricInfo, NSBaseShape::ppt);
	//}

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
		
		return true;// CBaseShape::SetProperties(Shape);
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
