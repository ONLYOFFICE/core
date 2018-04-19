﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#include "./../BaseShape.h"
#include "CustomGeomShape.h"
#include "PptShapeEnum.h"
#include "../../../../../../Common/DocxFormat/Source/DocxFormat/Logic/VmlOfficeDrawing.h"

using namespace NSOfficeDrawing;
using namespace NSPresentationEditor;

class CPPTShape : public CBaseShape
{
public:
    PPTShapes::ShapeType		m_eType;
    NSGuidesVML::CFormulasManager	m_oManager;

    NSCustomVML::CCustomVML m_oCustomVML;

    std::wstring m_strPathLimoX;
    std::wstring m_strPathLimoY;

    std::vector<std::wstring> m_arStringTextRects;
    bool m_bIsShapeType;

    bool m_bIsFilled;
    bool m_bIsStroked;
    nullable<OOX::VmlOffice::CSignatureLine> m_oSignatureLine;

    CPPTShape();
    virtual ~CPPTShape();

    virtual bool LoadFromXML(const std::wstring& xml);
    virtual bool LoadFromXML(XmlUtils::CXmlNode& oNodePict);
    virtual bool LoadFromXMLShapeType(XmlUtils::CXmlNode& oNodeShapeType);
    virtual bool LoadAdjustValuesList(const std::wstring& xml)
    {
        std::wstring strXml = xml;
        if (0 == strXml.find((wchar_t)','))
            strXml = (_T("defaultAdj") + strXml);
        
		LONG lLastIndex = (LONG)strXml.length() - 1;

        if (lLastIndex >= 0 && lLastIndex == strXml.rfind((wchar_t)','))
        {
            strXml = strXml + _T("defaultAdj");
        }
        ReplaceAll(strXml,L",,", L",defaultAdj,");

        std::vector<std::wstring> arAdj;
        boost::algorithm::split(arAdj, strXml, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

        std::vector<long> oOldAdj;
        size_t nOldLen = m_arAdjustments.size();

        for (size_t ii = 0; ii < nOldLen; ++ii)
            oOldAdj.push_back(m_arAdjustments[ii]);

        m_arAdjustments.clear();
        for (size_t nIndex = 0; nIndex < arAdj.size(); ++nIndex)
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

        size_t newLen = m_arAdjustments.size();
        
		for (size_t i = newLen; i < nOldLen; ++i)
		{
            m_arAdjustments.push_back(oOldAdj[i]);
		}
        return true;
    }

    virtual bool LoadGuidesList(const std::wstring& xml)
    {
        // std::wstring sFormulasTemp;
        // sFormulasTemp.Format(_T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">%ls</root>"),xml);
        
        
        std::wstring sBegin(_T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"));
        
        std::wstring sEnd(_T("</root>"));
        std::wstring sFormulasTemp = sBegin + xml+ sEnd;
        

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

    virtual void AddGuide(const std::wstring& strGuide)
    {
        m_oManager.AddFormula(strGuide);
    }

    virtual bool LoadAdjustHandlesList(const std::wstring& xml)
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

    virtual bool LoadConnectorsList(const std::wstring& xml)
    {
        return true;
    }

    virtual bool LoadTextRect(const std::wstring& xml)
    {
        std::vector<std::wstring> oArray;
        boost::algorithm::split(oArray, xml, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

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
        if (!m_bIsShapeType)
            m_oPath.FromXML(xml, m_oManager);
        return true;
    }

    virtual bool SetAdjustment(long index, long value)
    {
        if (index < (long)m_arAdjustments.size() && index >= 0)
        {
            m_arAdjustments[index] =  value;
            return TRUE;
        }
        return FALSE;
    }

    virtual std::wstring ToXML(CGeomShapeInfo& GeomInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen)
    {
        if ((_T("") != m_strPathLimoX) || _T("") != m_strPathLimoY)
        {
            m_strPath = (GeomInfo.m_dWidth >= GeomInfo.m_dHeight) ? m_strPathLimoX : m_strPathLimoY;
            ReCalculate();
            m_oPath.SetCoordsize(21600, 21600);
        }

        return m_oPath.ToXml(GeomInfo, StartTime, EndTime, Pen, Brush, NSBaseShape::ppt);
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

    static CBaseShapePtr CreateByType(PPTShapes::ShapeType type);
    
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
        CBaseShapePtr pShape = CreateByType(type);
       
		if(pShape)
        {
            m_eType = type;

            SetProperties(pShape.get());
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

        std::vector<std::wstring> oArray;
        boost::algorithm::split(oArray, m_arStringTextRects[nIndex], boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

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

    bool GetPos(std::wstring str, LONG& lValue)
    {
        if (str.length() == 0)
            return false;

        wchar_t mem = str[0];

        bool bFormula = false;
        if ('@' == mem)
        {
            bFormula = true;
            str.erase(0, 1);
        }

        if (!NSStringUtils::IsNumber(str))
            return false;

        lValue = 0;
        lValue = _ttoi(str.c_str());

        if (bFormula)
        {
            if (lValue >= 0 || lValue < (LONG)m_oManager.m_arResults.size())
            {
                lValue = m_oManager.m_arResults[lValue];
                return true;
            }
            return false;
        }

        return true;
    }
};
