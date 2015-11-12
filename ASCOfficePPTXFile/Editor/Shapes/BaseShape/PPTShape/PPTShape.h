#pragma once
#include "./../BaseShape.h"
#include "CustomGeomShape.h"
#include "PPTShapeEnum.h"

using namespace NSOfficeDrawing;
using namespace NSPresentationEditor;

class CPPTShape : public CBaseShape
{
public:
	PPTShapes::ShapeType			m_eType;
	NSGuidesVML::CFormulasManager	m_oManager;

	NSCustomVML::CCustomVML m_oCustomVML;

	CString m_strPathLimoX;
	CString m_strPathLimoY;

	std::vector<CString> m_arStringTextRects;
	bool m_bIsShapeType;

	bool m_bIsFilled;
	bool m_bIsStroked;

public:
	CPPTShape() : CBaseShape(), m_arStringTextRects()
	{
		m_eType = PPTShapes::sptMin;

		m_arStringTextRects.push_back(_T("0,0,21600,21600"));

		m_strPathLimoX = _T("");
		m_strPathLimoY = _T("");
		m_bIsShapeType = false;

		m_bIsFilled = true;
		m_bIsStroked = true;
	}

	~CPPTShape()
	{
	}
	virtual bool LoadFromXML(const CString& xml)
	{
		XmlUtils::CXmlNode oNodePict;
		if (oNodePict.FromXmlString(xml))
		{
			return LoadFromXML(oNodePict);
		}
		return false;
	}

	//virtual bool LoadFromXML(const CString& xml)
	//{
	//	XmlUtils::CXmlNode oNodePict;
	//	if (oNodePict.FromXmlString(xml))
	//	{
	//		if (_T("shape") == oNodePict.GetName())
	//		{
	//			CString id = oNodePict.GetAttributeOrValue(_T("type"));
	//			if (id != _T(""))
	//			{
	//				SetShapeType((PPTShapes::ShapeType)Strings::ToInteger(id));
	//			}
	//			else
	//			{
	//				XmlUtils::CXmlNode oNodeTemplate;
	//				if (oNodePict.GetNode(_T("template"), oNodeTemplate))
	//				{	
	//					CString strAdj = oNodeTemplate.GetAttributeOrValue(_T("adj"));
	//					LoadAdjustValuesList(strAdj);

	//					XmlUtils::CXmlNode oNodeGuides;
	//					if (oNodeTemplate.GetNode(_T("v:formulas"), oNodeGuides))
	//					{
	//						LoadGuidesList(oNodeGuides.GetXml());
	//					}

	//					CString strPath = oNodeTemplate.GetAttributeOrValue(_T("path"));
	//					LoadPathList(strPath);
	//				}
	//			}

	//			XmlUtils::CXmlNode oNodeGuides;
	//			if (oNodePict.GetNode(_T("path"), oNodeGuides))
	//			{
	//				CString strPath = oNodeGuides.GetAttributeOrValue(_T("val"));
	//				LoadPathList(strPath);
	//			}

	//			/*XmlUtils::CXmlNode oNodeShapeType;
	//			if (oNodePict.GetNode(_T("v:shapetype"), oNodeShapeType))
	//			{
	//				CString strAdj = oNodeShapeType.GetAttributeOrValue(_T("adj"));
	//				LoadAdjustValuesList(strAdj);

	//				XmlUtils::CXmlNode oNodeGuides;
	//				if (oNodeShapeType.GetNode(_T("v:formulas"), oNodeGuides))
	//				{
	//					LoadGuidesList(oNodeGuides.GetXml());
	//				}

	//				CString strPath = oNodeShapeType.GetAttributeOrValue(_T("path"));
	//				LoadAdjustValuesList(strPath);
	//			}
	//			else
	//			{
	//				XmlUtils::CXmlNode oNodeShape;
	//				if (oNodePict.GetNode(_T("v:shape"), oNodeShape))
	//				{
	//					CString strAdj = oNodeShape.GetAttributeOrValue(_T("adj"));
	//					LoadAdjustValuesList(strAdj);

	//					XmlUtils::CXmlNode oNodeGuides;
	//					if (oNodeShape.GetNode(_T("v:formulas"), oNodeGuides))
	//					{
	//						LoadGuidesList(oNodeGuides.GetXml());
	//					}

	//					CString strPath = oNodeShape.GetAttributeOrValue(_T("path"));
	//					LoadPathList(strPath);
	//				}
	//			}*/

	//			return true;
	//		}
	//	}

	//	return false;
	//}

	virtual bool LoadFromXML(XmlUtils::CXmlNode& oNodePict)
	{		
		CString id = oNodePict.GetAttributeOrValue(_T("type"));
		bool isPathList = false;
		if (id != _T(""))
		{
			SetShapeType((PPTShapes::ShapeType)XmlUtils::GetInteger(id));
		}
		else
		{
			XmlUtils::CXmlNode oNodeTemplate;
			if (oNodePict.GetNode(_T("template"), oNodeTemplate))
			{	
				CString strAdj = oNodeTemplate.GetAttributeOrValue(_T("adj"));
				LoadAdjustValuesList(strAdj);

				XmlUtils::CXmlNode oNodeGuides;
				if (oNodeTemplate.GetNode(_T("v:formulas"), oNodeGuides))
				{
					LoadGuidesList(oNodeGuides.GetXml());
				}

				CString strPath = oNodeTemplate.GetAttributeOrValue(_T("path"));
				if (strPath != _T(""))
				{
					LoadPathList(strPath);
					isPathList = true;
				}
			}
		}

		XmlUtils::CXmlNode oNodeGuides;
		if (oNodePict.GetNode(_T("path"), oNodeGuides))
		{
			CString strPath = oNodeGuides.GetAttributeOrValue(_T("val"));
			if (strPath != _T(""))
			{
				LoadPathList(strPath);
				isPathList = true;
			}
		}

		if (!isPathList)
			ReCalculate();
		return true;
	}

	virtual bool LoadFromXMLShapeType(XmlUtils::CXmlNode& oNodeShapeType) // vml object
	{		// из за особенносей форматирования vmlDrawing могут вылезти пустые текстовые значения - value ..
		CString sId = oNodeShapeType.GetAttribute(_T("o:spt"));

		bool bIsNeedRecalc = true;
		if (sId != _T(""))
		{
			int id = XmlUtils::GetInteger(sId);
			if (id > 0)
			{
				SetShapeType((PPTShapes::ShapeType)id);
				//ReCalculate();
				m_eType = (PPTShapes::ShapeType)id;	
			}
		}
		//else
		{
			CString strAdj = oNodeShapeType.GetAttribute(_T("adj"));
			if (strAdj != _T(""))
				LoadAdjustValuesList(strAdj);

			XmlUtils::CXmlNode oNodeGuides;
			if (oNodeShapeType.GetNode(_T("v:formulas"), oNodeGuides))
			{
				LoadGuidesList(oNodeGuides.GetXml());
			}

			XmlUtils::CXmlNode oNodePath;
			if (oNodeShapeType.GetNode(_T("v:path"), oNodePath))
			{
				CString strTextR = oNodePath.GetAttribute(_T("textboxrect"));
				if (strTextR != _T(""))
					LoadTextRect(strTextR);
			}

			XmlUtils::CXmlNode oNodeAHs;
			if (oNodeShapeType.GetNode(_T("v:handles"), oNodeAHs))
			{
				LoadAHList(oNodeAHs);
			}

			CString strPath = oNodeShapeType.GetAttribute(_T("path"));
			if (strPath != _T(""))
			{
				LoadPathList(strPath);
			}
		}

		CString strFilled = oNodeShapeType.GetAttribute(_T("filled"));
		CString strStroked = oNodeShapeType.GetAttribute(_T("stroked"));

		if (strFilled != _T(""))
		{
			if (strFilled == _T("false") || strFilled == _T("f"))
				m_bIsFilled = false;
			else
				m_bIsFilled = true;
		}
		
		if (strStroked != _T(""))
		{
			if (strStroked == _T("false") || strStroked == _T("f"))
				m_bIsStroked = false;
			else
				m_bIsStroked = true;
		}

		ReCalculate();
		return true;
	}
	
	virtual bool LoadAdjustValuesList(const CString& xml)
	{
		CString strXml = xml;
		if (0 == strXml.Find((TCHAR)','))
			strXml = (_T("defaultAdj") + strXml);
		LONG lLastIndex = strXml.GetLength() - 1;
		if (lLastIndex >= 0 && lLastIndex == strXml.ReverseFind((TCHAR)','))
		{
			strXml = strXml + _T("defaultAdj");
		}
		strXml.Replace(_T(",,"), _T(",defaultAdj,"));

		std::vector<CString> arAdj;
		NSStringUtils::ParseString(_T(","), strXml, &arAdj);
		
		std::vector<long> oOldAdj;
		int nOldLen = m_arAdjustments.size();
		for (int ii = 0; ii < nOldLen; ++ii)
			oOldAdj.push_back(m_arAdjustments[ii]);

		m_arAdjustments.clear();
		for (int nIndex = 0; nIndex < arAdj.size(); ++nIndex)
		{
			if (_T("defaultAdj") == arAdj[nIndex])
			{
				if (nIndex >= nOldLen)
					m_arAdjustments.push_back(0);
				else
					m_arAdjustments.push_back(oOldAdj[nIndex]);
			}
			else
			{
				m_arAdjustments.push_back((LONG)XmlUtils::GetInteger(arAdj[nIndex]));
			}
		}

		int newLen = m_arAdjustments.size();
		for (int i = newLen; i < nOldLen; ++i)
			m_arAdjustments.push_back(oOldAdj[i]);
		return true;
	}

	virtual bool LoadGuidesList(const CString& xml)
	{
           // CString sFormulasTemp;
           // sFormulasTemp.Format(_T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">%ls</root>"),xml);
        
        
            CString sBegin(_T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"));
        
            CString sEnd(_T("</root>"));
            CString sFormulasTemp = sBegin + xml+ sEnd;
        

            XmlUtils::CXmlNode oNodeRoot;
            if (oNodeRoot.FromXmlString(sFormulasTemp) == false) return false;

            XmlUtils::CXmlNode oNodeGuides;
            if (oNodeRoot.GetNode(_T("v:formulas"),oNodeGuides) == false) return false;

            if ((_T("v:formulas") == oNodeGuides.GetName()))
            {
                m_oManager.Clear();

                XmlUtils::CXmlNodes oList;
                if (oNodeGuides.GetNodes(_T("v:f"), oList))
                {
                    int lCount = oList.GetCount();
                    for (int nIndex = 0; nIndex < lCount; ++nIndex)
                    {
                        XmlUtils::CXmlNode oNodeFormula;
                        oList.GetAt(nIndex, oNodeFormula);

                        m_oManager.AddFormula(oNodeFormula.GetAttributeOrValue(_T("eqn")));
                    }
                }

                m_oManager.Clear(&m_arAdjustments);
                m_oManager.CalculateResults();
                return true;
            }

            return false;
	}

	virtual void AddGuide(const CString& strGuide)
	{
		m_oManager.AddFormula(strGuide);
	}

	virtual bool LoadAdjustHandlesList(const CString& xml)
	{
		return true;
	}

	void LoadAHList(XmlUtils::CXmlNode& oNode)
	{
		m_arHandles.clear();

		XmlUtils::CXmlNodes oNodes;
		if (oNode.GetNodes(_T("v:h"), oNodes))
		{
			int nCount = oNodes.GetCount();
			for (int i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode oNodeH;
				oNodes.GetAt(i, oNodeH);

				CHandle_ oH;
				oH.polar = oNodeH.GetAttribute(_T("polar"));
				oH.position = oNodeH.GetAttribute(_T("position"));
				oH.radiusrange = oNodeH.GetAttribute(_T("radiusrange"));
				oH.switchHandle = oNodeH.GetAttribute(_T("switch"));
				oH.xrange = oNodeH.GetAttribute(_T("xrange"));
				oH.yrange = oNodeH.GetAttribute(_T("yrange"));

				m_arHandles.push_back(oH);
			}
		}
	}

	virtual bool LoadConnectorsList(const CString& xml)
	{
		return true;
	}

	virtual bool LoadTextRect(const CString& xml)
	{
		std::vector<CString> oArray;
		NSStringUtils::ParseString(_T(";"), xml, &oArray);

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

	virtual bool LoadPathList(const CString& xml)
	{
		m_strPath = xml;
		if (!m_bIsShapeType)
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

	virtual CString ToXML(CGeomShapeInfo& GeomInfo, CMetricInfo& MetricInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen)
	{
		if ((_T("") != m_strPathLimoX) || _T("") != m_strPathLimoY)
		{
			m_strPath = (GeomInfo.m_dWidth >= GeomInfo.m_dHeight) ? m_strPathLimoX : m_strPathLimoY;
			ReCalculate();
			m_oPath.SetCoordsize(21600, 21600);
		}
		
		return m_oPath.ToXml(GeomInfo, StartTime, EndTime, Pen, Brush, MetricInfo, NSBaseShape::ppt);
	}

	virtual void ReCalculate()
	{
		m_oManager.Clear(&m_arAdjustments);
		
		if (!m_bIsShapeType)
			m_oManager.CalculateResults();

		if (_T("") == m_strPath)
			return;

		LoadPathList(m_strPath);
	}

	static CPPTShape* CreateByType(PPTShapes::ShapeType type);
	virtual const ClassType GetClassType()const
	{
		return NSBaseShape::ppt;
	}

	virtual bool SetProperties(CBaseShape* Shape)
	{
		if(Shape == NULL)
			return false;

		if(Shape->GetClassType() != NSBaseShape::ppt)
			return false;

		m_oManager = ((CPPTShape*)Shape)->m_oManager;

		m_strPathLimoX = ((CPPTShape*)Shape)->m_strPathLimoX;
		m_strPathLimoY = ((CPPTShape*)Shape)->m_strPathLimoY;

		m_arStringTextRects.clear();
		m_arStringTextRects.insert(m_arStringTextRects.end(), ((CPPTShape*)Shape)->m_arStringTextRects.begin(), ((CPPTShape*)Shape)->m_arStringTextRects.end());
		
		return CBaseShape::SetProperties(Shape);
	}

	virtual bool SetToDublicate(CBaseShape* Shape)
	{
		if(Shape == NULL)
			return false;

		if(Shape->GetClassType() != NSBaseShape::ppt)
			return false;

		((CPPTShape*)Shape)->m_oManager = m_oManager;

		((CPPTShape*)Shape)->m_bIsFilled = m_bIsFilled;
		((CPPTShape*)Shape)->m_bIsStroked = m_bIsStroked;

		((CPPTShape*)Shape)->m_strPathLimoX = m_strPathLimoX;
		((CPPTShape*)Shape)->m_strPathLimoY = m_strPathLimoY;

		((CPPTShape*)Shape)->m_arStringTextRects.clear();
		((CPPTShape*)Shape)->m_arStringTextRects.insert(((CPPTShape*)Shape)->m_arStringTextRects.end(), m_arStringTextRects.begin(), m_arStringTextRects.end());

		return CBaseShape::SetToDublicate(Shape);
	}

	bool SetShapeType(PPTShapes::ShapeType type)
	{
		CPPTShape* l_pShape = CreateByType(type);
		if(l_pShape != NULL)
		{
			m_eType = type;

			SetProperties(l_pShape);
			delete l_pShape;
			return true;
		}

		m_eType = PPTShapes::sptCustom;
		return false;
	}

	
	void CalcTextRectOffsets(double& dPercentLeft, double& dPercentTop, double& dPercentRight, double& dPercentBottom, LONG nIndex = 0)
	{
		dPercentLeft	= 0;
		dPercentTop		= 0;
		dPercentRight	= 0;
		dPercentBottom	= 0;

		if ((nIndex < 0) || (nIndex >= (LONG)m_arStringTextRects.size()))
			return;

		if (m_oPath.m_arParts.size() == 0)
			return;

		LONG lWidth		= m_oPath.m_arParts[0].width;
		LONG lHeight	= m_oPath.m_arParts[0].height;

		std::vector<CString> oArray;
		NSStringUtils::ParseString(_T(","), m_arStringTextRects[nIndex], &oArray);

		if (4 != oArray.size())
			return;
		
		LONG lLeft		= 0;
		LONG lTop		= 0;
		LONG lRight		= 0;
		LONG lBottom	= 0;

		bool bOk = true;

		bOk = (bOk && GetPos(oArray[0], lLeft));
		bOk = (bOk && GetPos(oArray[1], lTop));
		bOk = (bOk && GetPos(oArray[2], lRight));
		bOk = (bOk && GetPos(oArray[3], lBottom));

		if (!bOk)
			return;

		dPercentLeft	= (double)lLeft		/ lWidth;
		dPercentTop		= (double)lTop		/ lHeight;

		dPercentRight	= (double)(lWidth - lRight)		/ lWidth;
		dPercentBottom	= (double)(lHeight - lBottom)	/ lHeight;
	}

	bool IsWordArt()
	{
		if (m_eType >= PPTShapes::ShapeType::sptCTextPlain && m_eType <= PPTShapes::ShapeType::sptCTextCanDown)
			return true;
		else
			return false;
	}

protected:

	bool GetPos(CString str, LONG& lValue)
	{
		if (str.GetLength() == 0)
			return false;

		TCHAR mem = str.GetAt(0);

		bool bFormula = false;
		if ('@' == mem)
		{
			bFormula = true;
			str.Delete(0);
		}
		
		if (!NSStringUtils::IsNumber(str))
			return false;

		lValue = 0;
		lValue = _ttoi(str);

		if (bFormula)
		{
			if (lValue >= 0 || lValue < m_oManager.m_arResults.size())
			{
				lValue = m_oManager.m_arResults[lValue];
				return true;
			}
			return false;
		}

		return true;
	}
};
