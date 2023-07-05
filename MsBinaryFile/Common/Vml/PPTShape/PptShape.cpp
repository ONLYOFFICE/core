/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "PptShape.h"
#include "../../../DocFile/OfficeDrawing/PresetShapeTypes.h"

#include <boost/lexical_cast.hpp>
//-------------------------------------------------------------------------------
//#define CREATE_BY_SPT(SHAPE_TYPE, CLASS_SHAPE_NAME)							\
//	case SHAPE_TYPE: { pShape = new CLASS_SHAPE_NAME(); break; }				\
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
#define CREATE_BY_SPT(SHAPE_TYPE, CLASS_SHAPE_NAME)								\
	case SHAPE_TYPE:															\
	{																			\
		pShapeType = new CLASS_SHAPE_NAME();									\
		pShape = CreateByShapeType(pShapeType);									\
		delete pShapeType;														\
		pShapeType = NULL;														\
		break;																	\
	}																			\
//-------------------------------------------------------------------------------

using namespace ODRAW;

_3dOptions::_3dOptions() : bEnabled(false), bMetallic(false), bExtrusionColor(false), bLightFace(true),
	bConstrainRotation(true), bRotationCenterAuto(false), bParallel(true), bFillHarsh(true), bKeyHarsh(true),
	nRenderMode(0), dSpecularAmt(0), nEdgeThickness(0x0000319C), nExtrudeForward(0), nExtrudeBackward(0x0006F9F0),
	nSkewAmount(0x32)
{
	dFillIntensity = FixedPointToDouble(0x00009470);
	dKeyX		= FixedPointToDouble(0x0000C350);
	dKeyY		= FixedPointToDouble(0x00000000);
	dFillX		= FixedPointToDouble(0xFFFF3CB0);
	dFillY		= FixedPointToDouble(0x00000000);
	dOriginX	= FixedPointToDouble(0x00008000);
	dOriginY	= FixedPointToDouble(0xFFFF8000);
	dSkewAngle	= 225;//FixedPointToDouble(0xFF790000);
	dXViewpoint	= FixedPointToDouble(0x001312D0);
	dYViewpoint	= FixedPointToDouble(0xFFECED30);
}

CPPTShape::CPPTShape() : CBaseShape(), m_arStringTextRects()
{
    m_eType = PPTShapes::sptMin;

	m_arStringTextRects.clear();
    m_strPathLimoX.clear();
    m_strPathLimoY.clear();

    m_bIsShapeType = false;

    m_bIsFilled = true;
    m_bIsStroked = true;
}
CPPTShape::~CPPTShape()
{
}
bool CPPTShape::LoadFromXML(const std::wstring& xml)
{
	XmlUtils::CXmlNode oNodePict;
	if (oNodePict.FromXmlString(xml))
	{
		return LoadFromXML(oNodePict);
	}
	return false;
}
bool CPPTShape::LoadFromXML(XmlUtils::CXmlNode& oNodePict)
{
	std::wstring id = oNodePict.GetAttributeOrValue(L"type");
	bool isPathList = false;
	if (false == id.empty())
	{
		SetShapeType((PPTShapes::ShapeType)XmlUtils::GetInteger(id));
	}
	else
	{
		XmlUtils::CXmlNode oNodeTemplate;
		if (oNodePict.GetNode(L"template", oNodeTemplate))
		{
			std::wstring strAdj = oNodeTemplate.GetAttributeOrValue(L"adj");
			LoadAdjustValuesList(strAdj);

			XmlUtils::CXmlNode oNodeGuides;
			if (oNodeTemplate.GetNode(L"v:formulas", oNodeGuides))
			{
				LoadGuidesList(oNodeGuides.GetXml());
			}

			std::wstring strPath = oNodeTemplate.GetAttributeOrValue(L"path");
			if (false == strPath.empty())
			{
				LoadPathList(strPath);
				isPathList = true;
			}
		}
	}

	XmlUtils::CXmlNode oNodeGuides;
	if (oNodePict.GetNode(L"path", oNodeGuides))
	{
		std::wstring strPath = oNodeGuides.GetAttributeOrValue(L"val");
		if (false == strPath.empty())
		{
			LoadPathList(strPath);
			isPathList = true;
		}
	}

	if (!isPathList)
		ReCalculate();
	return true;
}
bool CPPTShape::LoadFromXMLShapeType(XmlUtils::CXmlNode& oNodeShapeType) // vml object
{		// из за особенносей форматирования vmlDrawing могут вылезти пустые текстовые значения - value ..
	std::wstring sId = oNodeShapeType.GetAttribute(L"o:spt");

	bool bIsNeedRecalc = true;
	bool bCustomSet = false;

	if (false == sId.empty())
	{
		int id = XmlUtils::GetInteger(sId);
		if (id > 0)
		{
			SetShapeType((PPTShapes::ShapeType)id);
			//ReCalculate();
			m_eType = (PPTShapes::ShapeType)id;
			bCustomSet = true;
		}
	}
	std::wstring strAdj = oNodeShapeType.GetAttribute(L"adj");
	if (false == strAdj.empty())
		LoadAdjustValuesList(strAdj);

	XmlUtils::CXmlNode oNodeGuides;
	if (oNodeShapeType.GetNode(L"v:formulas", oNodeGuides))
	{
		bCustomSet = LoadGuidesList(oNodeGuides.GetXml());
	}

	XmlUtils::CXmlNode oNodePath;
	if (oNodeShapeType.GetNode(L"v:path", oNodePath))
	{
		std::wstring strPath = oNodePath.GetAttribute(L"v");
		if (false == strPath.empty())
		{
			bCustomSet = LoadPathList(strPath);
		}
		std::wstring strTextR = oNodePath.GetAttribute(L"textboxrect");
		if (false == strTextR.empty())
			bCustomSet = LoadTextRect(strTextR);
	}

	XmlUtils::CXmlNode oNodeAHs;
	if (oNodeShapeType.GetNode(L"v:handles", oNodeAHs))
	{
		LoadAHList(oNodeAHs);
	}

	std::wstring strPath = oNodeShapeType.GetAttribute(L"path");
	if (false == strPath.empty())
	{
		bCustomSet = LoadPathList(strPath);
	}

	XmlUtils::CXmlNode oNodeTextPath;
	if (oNodeShapeType.GetNode(L"v:textpath", oNodeTextPath))
	{
		bCustomSet = true;
		m_textPath.bEnabled = true;

		if (m_eType < PPTShapes::ShapeType::sptCTextPlain || m_eType > PPTShapes::ShapeType::sptCTextCanDown)
			m_eType = PPTShapes::ShapeType::sptCTextPlain;
		nullable_bool oFitPath;

		XmlMacroReadAttributeBase(oNodeTextPath, L"fitpath", oFitPath);
		if (oFitPath.is_init() && (*oFitPath))
		{
			m_textPath.bFitPath = true;
		}

		nullable_bool oFitShape;
		XmlMacroReadAttributeBase(oNodeTextPath, L"fitshape", oFitShape);
		if (oFitShape.is_init() && (*oFitShape))
		{
			m_textPath.bFitShape = true;
		}

		nullable_bool oTrim;
		XmlMacroReadAttributeBase(oNodeTextPath, L"trim", oTrim);
		if (oTrim.is_init() && (*oTrim))
		{
			m_textPath.bTrim = true;
		}
		nullable_string oStyle;
		XmlMacroReadAttributeBase(oNodeTextPath, L"style", oStyle);
		if (oStyle.is_init() && (false == oStyle->empty()))
		{
			if (m_textPath.sStyle)
				m_textPath.sStyle = (*m_textPath.sStyle) + L";" + (*oStyle);
			else
				m_textPath.sStyle = *oStyle;
		}
	}

	std::wstring strFilled = oNodeShapeType.GetAttribute(L"filled");
	std::wstring strStroked = oNodeShapeType.GetAttribute(L"stroked");

	if (false == strFilled.empty())
	{
		if (strFilled == L"false" || strFilled == L"f")
			m_bIsFilled = false;
		else
			m_bIsFilled = true;
	}

	if (false == strStroked.empty())
	{
		if (strStroked == L"false" || strStroked == L"f")
			m_bIsStroked = false;
		else
			m_bIsStroked = true;
	}
	XmlUtils::CXmlNode oNodeSignature;
	if (oNodeShapeType.GetNode(L"o:signatureline", oNodeSignature))
	{
		m_oSignatureLine = oNodeSignature;
	}

	XmlUtils::CXmlNode oNodeImagedata;
	if (!bCustomSet && oNodeShapeType.GetNode(L"v:imagedata", oNodeImagedata))
	{
		std::wstring sRid = oNodeImagedata.GetAttribute(L"r:id");
		std::wstring sRelid = oNodeImagedata.GetAttribute(L"o:relid");
		std::wstring sPictId = oNodeImagedata.GetAttribute(L"r:pict");

		if (!sRid.empty() || !sRelid.empty() || !sPictId.empty())
			m_eType = PPTShapes::ShapeType::sptCFrame;
	}
	ReCalculate();
	return true;
}
bool CPPTShape::LoadAdjustValuesList(const std::wstring& xml)
{
	std::wstring strXml = xml;
	if (0 == strXml.find((wchar_t)','))
		strXml = (_T("defaultAdj") + strXml);

	LONG lLastIndex = (LONG)strXml.length() - 1;

	if (lLastIndex >= 0 && lLastIndex == strXml.rfind((wchar_t)','))
	{
		strXml = strXml + _T("defaultAdj");
	}
	ReplaceAll(strXml, L",,", L",defaultAdj,");

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
			long val = (long)XmlUtils::GetInteger(arAdj[nIndex]);
			if (nIndex < m_arAbsMaxAdjustments.size())
			{
				if (abs(val) > m_arAbsMaxAdjustments[nIndex])
					val = 0;
			}
			m_arAdjustments.push_back(val);
		}
	}

	size_t newLen = m_arAdjustments.size();

	for (size_t i = newLen; i < nOldLen; ++i)
	{
		m_arAdjustments.push_back(oOldAdj[i]);
	}
	return true;
}
bool CPPTShape::LoadGuidesList(const std::wstring& xml)
{
	std::wstring sBegin(_T("<root xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"));

	std::wstring sEnd(_T("</root>"));
	std::wstring sFormulasTemp = sBegin + xml + sEnd;

	XmlUtils::CXmlNode oNodeRoot;
	if (oNodeRoot.FromXmlString(sFormulasTemp) == false) return false;

	XmlUtils::CXmlNode oNodeGuides;
	if (oNodeRoot.GetNode(_T("v:formulas"), oNodeGuides) == false) return false;

	if (L"v:formulas" == oNodeGuides.GetName())
	{
		size_t lCount = 0;
		m_oManager.Clear();

		std::vector<XmlUtils::CXmlNode> oList;
		if (oNodeGuides.GetNodes(L"v:f", oList))
		{
			lCount = oList.size();
			for (size_t nIndex = 0; nIndex < lCount; ++nIndex)
			{
				XmlUtils::CXmlNode & oNodeFormula = oList[nIndex];

				m_oManager.AddFormula(oNodeFormula.GetAttributeOrValue(L"eqn"));
			}
		}

		m_oManager.Clear(&m_arAdjustments);
		m_oManager.CalculateResults();
		return (lCount > 0);
	}
	return false;
}
void CPPTShape::AddGuide(const std::wstring& strGuide)
{
	m_oManager.AddFormula(strGuide);
}
bool CPPTShape::LoadAdjustHandlesList(const std::wstring& xml)
{
	return true;
}
void CPPTShape::LoadAHList(XmlUtils::CXmlNode& oNode)
{
	m_arHandles.clear();

	std::vector<XmlUtils::CXmlNode> oNodes;
	if (oNode.GetNodes(_T("v:h"), oNodes))
	{
		size_t nCount = oNodes.size();
		for (int i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode & oNodeH = oNodes[i];

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
bool CPPTShape::LoadConnectorsList(const std::wstring& xml)
{
	return true;
}
bool CPPTShape::LoadTextRect(const std::wstring& xml)
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
bool CPPTShape::LoadPathList(const std::wstring& xml)
{
	m_strPath = xml;
	if (!m_bIsShapeType)
		m_oPath.FromXML(xml, m_oManager);
	return true;
}
bool CPPTShape::SetAdjustment(long index, long value)
{
	if (index < (long)m_arAdjustments.size() && index >= 0)
	{
		m_arAdjustments[index] = value;
		return TRUE;
	}
	return FALSE;
}
std::wstring CPPTShape::ToXML(CGeomShapeInfo& GeomInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen)
{
	if (false == m_strPathLimoX.empty() || false == m_strPathLimoY.empty())
	{
		m_strPath = (GeomInfo.m_dWidth >= GeomInfo.m_dHeight) ? m_strPathLimoX : m_strPathLimoY;
		ReCalculate();
		m_oPath.SetCoordsize(21600, 21600);
	}

	return m_oPath.ToXml(GeomInfo, StartTime, EndTime, Pen, Brush, NSBaseShape::ppt);
}
void CPPTShape::ReCalculate()
{
	m_oManager.Clear(&m_arAdjustments);

	if (!m_bIsShapeType)
		m_oManager.CalculateResults();

	if (false == m_strPath.empty())
	{
		LoadPathList(m_strPath);
	}
}
const ClassType CPPTShape::GetClassType() const
{
	return NSBaseShape::ppt;
}
bool CPPTShape::SetProperties(CBaseShape* Shape)
{
	if (Shape == NULL)
		return false;

	if (Shape->GetClassType() != NSBaseShape::ppt)
		return false;

	m_oManager = ((CPPTShape*)Shape)->m_oManager;

	m_strPathLimoX = ((CPPTShape*)Shape)->m_strPathLimoX;
	m_strPathLimoY = ((CPPTShape*)Shape)->m_strPathLimoY;

	m_arStringTextRects.clear();
	m_arStringTextRects.insert(m_arStringTextRects.end(), ((CPPTShape*)Shape)->m_arStringTextRects.begin(), ((CPPTShape*)Shape)->m_arStringTextRects.end());

	return CBaseShape::SetProperties(Shape);
}
bool CPPTShape::SetToDublicate(CBaseShape* Shape)
{
	if (Shape == NULL)
		return false;

	if (Shape->GetClassType() != NSBaseShape::ppt)
		return false;

	((CPPTShape*)Shape)->m_oManager = m_oManager;

	((CPPTShape*)Shape)->m_bIsFilled = m_bIsFilled;
	((CPPTShape*)Shape)->m_bIsStroked = m_bIsStroked;

	((CPPTShape*)Shape)->m_strPathLimoX = m_strPathLimoX;
	((CPPTShape*)Shape)->m_strPathLimoY = m_strPathLimoY;

	((CPPTShape*)Shape)->m_3dOptions = m_3dOptions;
	((CPPTShape*)Shape)->m_textPath = m_textPath;

	((CPPTShape*)Shape)->m_arStringTextRects.clear();
	((CPPTShape*)Shape)->m_arStringTextRects.insert(((CPPTShape*)Shape)->m_arStringTextRects.end(), m_arStringTextRects.begin(), m_arStringTextRects.end());

	return CBaseShape::SetToDublicate(Shape);
}
bool CPPTShape::SetShapeType(PPTShapes::ShapeType type)
{
	CBaseShapePtr pShape = CreateByType(type);

	if (pShape)
	{
		m_eType = type;

		SetProperties(pShape.get());
		return true;
	}

	m_eType = PPTShapes::sptCustom;
	return false;
}
void CPPTShape::CalcTextRectOffsets(double& dPercentLeft, double& dPercentTop, double& dPercentRight, double& dPercentBottom, LONG nIndex)
{
	dPercentLeft = 0;
	dPercentTop = 0;
	dPercentRight = 0;
	dPercentBottom = 0;

	if ((nIndex < 0) || (nIndex >= (LONG)m_arStringTextRects.size()))
		return;

	if (m_oPath.m_arParts.size() == 0)
		return;

	LONG lWidth = m_oPath.m_arParts[0].width;
	LONG lHeight = m_oPath.m_arParts[0].height;

	std::vector<std::wstring> oArray;
	boost::algorithm::split(oArray, m_arStringTextRects[nIndex], boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

	if (4 != oArray.size())
		return;

	LONG lLeft = 0;
	LONG lTop = 0;
	LONG lRight = 0;
	LONG lBottom = 0;

	bool bOk = true;

	bOk = (bOk && GetPos(oArray[0], lLeft));
	bOk = (bOk && GetPos(oArray[1], lTop));
	bOk = (bOk && GetPos(oArray[2], lRight));
	bOk = (bOk && GetPos(oArray[3], lBottom));

	if (!bOk)
		return;

	dPercentLeft = (double)lLeft / lWidth;
	dPercentTop = (double)lTop / lHeight;

	dPercentRight = (double)(lWidth - lRight) / lWidth;
	dPercentBottom = (double)(lHeight - lBottom) / lHeight;
}
bool CPPTShape::IsWordArt()
{
	if ((m_eType >= PPTShapes::ShapeType::sptCTextPlain && m_eType <= PPTShapes::ShapeType::sptCTextCanDown) || (m_textPath.bEnabled))
		return true;
	else
		return false;
}
bool CPPTShape::GetPos(std::wstring str, LONG& lValue)
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

	lValue = XmlUtils::GetInteger(str);

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
CPPTShape* CPPTShape::CreateByShapeType(DocFileFormat::ShapeType* pShapeType)
{
	CPPTShape* pShape = NULL;

	if (pShapeType)
	{
		pShape = new CPPTShape();

		// Path
		pShape->m_strPath = pShapeType->Path;

		// ConcentricFill
		pShape->m_bConcentricFill = pShapeType->ShapeConcentricFill;

		pShape->m_bIsFilled = pShapeType->Filled;
		pShape->m_bIsStroked = pShapeType->Stroked;

		// Join
		switch(pShapeType->Joins)
		{
			case DocFileFormat::miter:
				pShape->m_eJoin = ODRAW::lineJoinMiter;
				break;
			case DocFileFormat::round:
				pShape->m_eJoin = ODRAW::lineJoinRound;
				break;
			case DocFileFormat::bevel:
			pShape->m_eJoin = ODRAW::lineJoinBevel;
				break;
		}

		// Connectors
		if (false == pShapeType->ConnectorLocations.empty())
		{
			pShape->LoadConnectorsList(pShapeType->ConnectorLocations);
		}

		// TextRect
		if (false == pShapeType->TextBoxRectangle.empty())
		{
			pShape->LoadTextRect(pShapeType->TextBoxRectangle);
		}

		// Handles  // todooo - убрать лишнее определение Handle !!!
		for (size_t i = 0; i < pShapeType->Handles.size(); ++i)
		{
			CHandle_ oHandle;
			oHandle.position = pShapeType->Handles[i].position;
			oHandle.xrange = pShapeType->Handles[i].xrange;
			oHandle.yrange = pShapeType->Handles[i].yrange;
			oHandle.switchHandle = pShapeType->Handles[i].switchHandle;
			oHandle.polar = pShapeType->Handles[i].polar;
			oHandle.radiusrange = pShapeType->Handles[i].radiusrange;

			pShape->m_arHandles.push_back(oHandle);
		}

		// Formulas / Guides
		for (size_t i = 0; i < pShapeType->Formulas.size(); ++i)
		{
			pShape->AddGuide(pShapeType->Formulas[i]);
		}

		pShape->m_arAdjustments = pShapeType->Adjustments;

		pShape->m_arConnectorAngles = pShapeType->ConnectorAngles;

		// Limo
		if (false == pShapeType->Limo.empty())
		{
			std::vector<std::wstring> limoArray;
			boost::algorithm::split(limoArray, pShapeType->Limo, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
			if (limoArray.size() == 2)
			{
				pShape->m_lLimoX = boost::lexical_cast<int>(limoArray[0].c_str());
				pShape->m_lLimoY = boost::lexical_cast<int>(limoArray[1].c_str());
			}
		}
	}

	return pShape;
}
CBaseShapePtr CPPTShape::CreateByType(PPTShapes::ShapeType type)
{	
	CPPTShape* pShape = NULL;
	DocFileFormat::ShapeType* pShapeType = NULL;

	switch (type)
	{
		// msosptNotchedCircularArrow	0x00000064 A value that SHOULD NOT be used.
		// msosptHostControl			0x000000C9 A value that SHOULD NOT be used.

		case sptHostControl:
		{ pShape = new CPPTShape();  pShape->m_eType = type; break; }

		CREATE_BY_SPT(0, DocFileFormat::RectangleType)
		
		case sptNotchedCircularArrow:
		CREATE_BY_SPT(sptPictureFrame, DocFileFormat::RectangleType)

		CREATE_BY_SPT(sptAccentBorderCallout90, DocFileFormat::AccentBorderCallout90)
		CREATE_BY_SPT(sptAccentBorderCallout1, DocFileFormat::AccentBorderCallout1)
		CREATE_BY_SPT(sptAccentBorderCallout2, DocFileFormat::AccentBorderCallout2)
		CREATE_BY_SPT(sptAccentBorderCallout3, DocFileFormat::AccentBorderCallout3)

		CREATE_BY_SPT(sptAccentCallout90, DocFileFormat::AccentCallout90)
		CREATE_BY_SPT(sptAccentCallout1, DocFileFormat::AccentCallout1)
		CREATE_BY_SPT(sptAccentCallout2, DocFileFormat::AccentCallout2)
		CREATE_BY_SPT(sptAccentCallout3, DocFileFormat::AccentCallout3)

		CREATE_BY_SPT(sptBorderCallout90, DocFileFormat::BorderCallout90)
		CREATE_BY_SPT(sptBorderCallout1, DocFileFormat::BorderCallout1)
		CREATE_BY_SPT(sptBorderCallout2, DocFileFormat::BorderCallout2)
		CREATE_BY_SPT(sptBorderCallout3, DocFileFormat::BorderCallout3)

		CREATE_BY_SPT(sptCallout90, DocFileFormat::Callout90)
		CREATE_BY_SPT(sptCallout1, DocFileFormat::Callout1)
		CREATE_BY_SPT(sptCallout2, DocFileFormat::Callout2)
		CREATE_BY_SPT(sptCallout3, DocFileFormat::Callout3)

		CREATE_BY_SPT(sptActionButtonBlank, DocFileFormat::ActionButtonBlank)
		CREATE_BY_SPT(sptActionButtonHome, DocFileFormat::ActionButtonHome)
		CREATE_BY_SPT(sptActionButtonHelp, DocFileFormat::ActionButtonHelp)
		CREATE_BY_SPT(sptActionButtonInformation, DocFileFormat::ActionButtonInfo)
		CREATE_BY_SPT(sptActionButtonBackPrevious, DocFileFormat::ActionButtonBack)
		CREATE_BY_SPT(sptActionButtonForwardNext, DocFileFormat::ActionButtonNext)
		CREATE_BY_SPT(sptActionButtonBeginning, DocFileFormat::ActionButtonBegin)
		CREATE_BY_SPT(sptActionButtonEnd, DocFileFormat::ActionButtonEnd)
		CREATE_BY_SPT(sptActionButtonReturn, DocFileFormat::ActionButtonReturn)
		CREATE_BY_SPT(sptActionButtonDocument, DocFileFormat::ActionButtonDoc)
		CREATE_BY_SPT(sptActionButtonSound, DocFileFormat::ActionButtonSound)
		CREATE_BY_SPT(sptActionButtonMovie, DocFileFormat::ActionButtonMovie)
		
		CREATE_BY_SPT(sptArc, DocFileFormat::ArcType)
		CREATE_BY_SPT(sptLine, DocFileFormat::LineType)

		CREATE_BY_SPT(sptBentArrow, DocFileFormat::BentArrow)
		CREATE_BY_SPT(sptBentUpArrow, DocFileFormat::BentUpArrow)
		CREATE_BY_SPT(sptBevel, DocFileFormat::BevelType)
		CREATE_BY_SPT(sptBlockArc, DocFileFormat::BlockArcType)
		CREATE_BY_SPT(sptBracePair, DocFileFormat::BracePairType)
		CREATE_BY_SPT(sptBracketPair, DocFileFormat::BracketPairType)
		
		CREATE_BY_SPT(sptCan, DocFileFormat::CanType)
		CREATE_BY_SPT(sptChevron, DocFileFormat::Chevron)
		CREATE_BY_SPT(sptCircularArrow, DocFileFormat::CircularArrow)
		CREATE_BY_SPT(sptCloudCallout, DocFileFormat::CloudCallout)
		CREATE_BY_SPT(sptCube, DocFileFormat::CubeType)
		CREATE_BY_SPT(sptCurvedDownArrow, DocFileFormat::CurvedDownArrow)
		CREATE_BY_SPT(sptCurvedLeftArrow, DocFileFormat::CurvedLeftArrow)
		CREATE_BY_SPT(sptCurvedRightArrow, DocFileFormat::CurvedRightArrow)
		CREATE_BY_SPT(sptCurvedUpArrow, DocFileFormat::CurvedUpArrow)

		CREATE_BY_SPT(sptDiamond, DocFileFormat::DiamondType)
		CREATE_BY_SPT(sptDonut, DocFileFormat::DonutType)
		CREATE_BY_SPT(sptDownArrowCallout, DocFileFormat::DownArrowCallout)
		CREATE_BY_SPT(sptDownArrow, DocFileFormat::DownArrowType)

		CREATE_BY_SPT(sptEllipse, DocFileFormat::OvalType)
		CREATE_BY_SPT(sptEllipseRibbon, DocFileFormat::EllipseRibbon)
		CREATE_BY_SPT(sptEllipseRibbon2, DocFileFormat::EllipseRibbon2)

		CREATE_BY_SPT(sptFlowChartAlternateProcess, DocFileFormat::FlowChartAlternateProcess)
		CREATE_BY_SPT(sptFlowChartCollate, DocFileFormat::FlowChartCollate)
		CREATE_BY_SPT(sptFlowChartConnector, DocFileFormat::FlowChartConnector)
		CREATE_BY_SPT(sptFlowChartDecision, DocFileFormat::FlowChartDecision)
		CREATE_BY_SPT(sptFlowChartDisplay, DocFileFormat::FlowChartDisplay)
		CREATE_BY_SPT(sptFlowChartDelay, DocFileFormat::FlowChartDelay)
		CREATE_BY_SPT(sptFlowChartDocument, DocFileFormat::FlowChartDocument)
		CREATE_BY_SPT(sptFlowChartExtract, DocFileFormat::FlowChartExtract)
		CREATE_BY_SPT(sptFlowChartInputOutput, DocFileFormat::FlowChartInputOutput)
		CREATE_BY_SPT(sptFlowChartInternalStorage, DocFileFormat::FlowChartInternalStorage)
		CREATE_BY_SPT(sptFlowChartMagneticDisk, DocFileFormat::FlowChartMagneticDisk)
		CREATE_BY_SPT(sptFlowChartMagneticDrum, DocFileFormat::FlowChartMagneticDrum)
		CREATE_BY_SPT(sptFlowChartMagneticTape, DocFileFormat::FlowChartMagneticTape)
		CREATE_BY_SPT(sptFlowChartManualInput, DocFileFormat::FlowChartManualInput)
		CREATE_BY_SPT(sptFlowChartManualOperation, DocFileFormat::FlowChartManualOperation)
		CREATE_BY_SPT(sptFlowChartMerge, DocFileFormat::FlowChartMerge)
		CREATE_BY_SPT(sptFlowChartMultidocument, DocFileFormat::FlowChartMultidocument)
		CREATE_BY_SPT(sptFlowChartOffpageConnector, DocFileFormat::FlowChartOffpageConnector)
		CREATE_BY_SPT(sptFlowChartOnlineStorage, DocFileFormat::FlowChartOnlineStorage)
		CREATE_BY_SPT(sptFlowChartOr, DocFileFormat::FlowChartOr)
		CREATE_BY_SPT(sptFlowChartPredefinedProcess, DocFileFormat::FlowChartPredefinedProcess)
		CREATE_BY_SPT(sptFlowChartPreparation, DocFileFormat::FlowChartPreparation)
		CREATE_BY_SPT(sptFlowChartProcess, DocFileFormat::FlowChartProcess)
		CREATE_BY_SPT(sptFlowChartPunchedCard, DocFileFormat::FlowChartPunchedCard)
		CREATE_BY_SPT(sptFlowChartPunchedTape, DocFileFormat::FlowChartPunchedTape)
		CREATE_BY_SPT(sptFlowChartSort, DocFileFormat::FlowChartSort)
		CREATE_BY_SPT(sptFlowChartSummingJunction, DocFileFormat::FlowChartSummingJunction)
		CREATE_BY_SPT(sptFlowChartTerminator, DocFileFormat::FlowChartTerminator)
		CREATE_BY_SPT(sptFoldedCorner, DocFileFormat::FoldedCornerlType)

		CREATE_BY_SPT(sptHeart, DocFileFormat::HeartType)
		CREATE_BY_SPT(sptHexagon, DocFileFormat::HexagonType)
		CREATE_BY_SPT(sptHomePlate, DocFileFormat::HomePlate)

		CREATE_BY_SPT(sptIrregularSeal1, DocFileFormat::IrregularSeal1)
		CREATE_BY_SPT(sptIrregularSeal2, DocFileFormat::IrregularSeal2)
		CREATE_BY_SPT(sptIsocelesTriangle, DocFileFormat::IsoscelesTriangleType)

		CREATE_BY_SPT(sptLeftArrowCallout, DocFileFormat::LeftArrowCallout)
		CREATE_BY_SPT(sptLeftArrow, DocFileFormat::LeftArrowType)
		CREATE_BY_SPT(sptLeftBrace, DocFileFormat::LeftBraceType)
		CREATE_BY_SPT(sptLeftBracket, DocFileFormat::LeftBracketType)
		CREATE_BY_SPT(sptLeftRightArrowCallout, DocFileFormat::LeftRightArrowCallout)
		CREATE_BY_SPT(sptLeftRightArrow, DocFileFormat::LeftRightArrow)
		CREATE_BY_SPT(sptLeftRightUpArrow, DocFileFormat::LeftRightUpArrow)
		CREATE_BY_SPT(sptLeftUpArrow, DocFileFormat::LeftUpArrow)
		CREATE_BY_SPT(sptLightningBolt, DocFileFormat::LightningBoltType)

		CREATE_BY_SPT(sptMoon, DocFileFormat::MoonType)

		CREATE_BY_SPT(sptNoSmoking, DocFileFormat::NoSmokingType)
		CREATE_BY_SPT(sptNotchedRightArrow, DocFileFormat::NotchedRightArrow)

		CREATE_BY_SPT(sptOctagon, DocFileFormat::OctagonType)

		CREATE_BY_SPT(sptParallelogram, DocFileFormat::ParallelogramType)
		CREATE_BY_SPT(sptPentagon, DocFileFormat::PentagonType)
		CREATE_BY_SPT(sptPlaque, DocFileFormat::PlaqueType)
		CREATE_BY_SPT(sptPlus, DocFileFormat::PlusType)

		CREATE_BY_SPT(sptQuadArrowCallout, DocFileFormat::QuadArrowCallout)
		CREATE_BY_SPT(sptQuadArrow, DocFileFormat::QuadArrow)

		CREATE_BY_SPT(sptRectangle, DocFileFormat::RectangleType)
		CREATE_BY_SPT(sptRibbon, DocFileFormat::Ribbon)
		CREATE_BY_SPT(sptRibbon2, DocFileFormat::Ribbon2)
		CREATE_BY_SPT(sptRightArrowCallout, DocFileFormat::RightArrowCallout)
		CREATE_BY_SPT(sptArrow, DocFileFormat::ArrowType)
		CREATE_BY_SPT(sptRightBrace, DocFileFormat::RightBraceType)
		CREATE_BY_SPT(sptRightBracket, DocFileFormat::RightBracketType)
		CREATE_BY_SPT(sptRightTriangle, DocFileFormat::RightTriangleType)
		CREATE_BY_SPT(sptRoundRectangle, DocFileFormat::RoundedRectangleType)

		CREATE_BY_SPT(sptSeal16, DocFileFormat::Seal16)
		CREATE_BY_SPT(sptSeal24, DocFileFormat::Seal24)
		CREATE_BY_SPT(sptSeal32, DocFileFormat::Seal32)
		CREATE_BY_SPT(sptSeal4, DocFileFormat::Seal4)
		CREATE_BY_SPT(sptSeal8, DocFileFormat::Seal8)
		CREATE_BY_SPT(sptSmileyFace, DocFileFormat::SmileyFaceType)
		CREATE_BY_SPT(sptStar, DocFileFormat::StarType)
		CREATE_BY_SPT(sptStraightConnector1, DocFileFormat::StraightConnector)
		CREATE_BY_SPT(sptStripedRightArrow, DocFileFormat::StripedRightArrow)
		CREATE_BY_SPT(sptSun, DocFileFormat::SunType)

		CREATE_BY_SPT(sptTextBox, DocFileFormat::TextboxType)
		CREATE_BY_SPT(sptTrapezoid, DocFileFormat::TrapezoidType)

		CREATE_BY_SPT(sptUpArrowCallout, DocFileFormat::UpArrowCallout)
		CREATE_BY_SPT(sptUpArrow, DocFileFormat::UpArrowType)
		CREATE_BY_SPT(sptUpDownArrowCallout, DocFileFormat::UpDownArrowCallout)
		CREATE_BY_SPT(sptUpDownArrow, DocFileFormat::UpDownArrow)
		CREATE_BY_SPT(sptUturnArrow, DocFileFormat::UturnArrow)

		CREATE_BY_SPT(sptVerticalScroll, DocFileFormat::VerticalScroll)
		CREATE_BY_SPT(sptHorizontalScroll, DocFileFormat::HorizontalScroll)

		CREATE_BY_SPT(sptWedgeEllipseCallout, DocFileFormat::WedgeEllipseCallout)
		CREATE_BY_SPT(sptWedgeRectCallout, DocFileFormat::WedgeRectCallout)
		CREATE_BY_SPT(sptWedgeRRectCallout, DocFileFormat::WedgeRRectCallout)

		CREATE_BY_SPT(sptWave, DocFileFormat::Wave)
		CREATE_BY_SPT(sptDoubleWave, DocFileFormat::DoubleWave)

		case sptBentConnector2:
		case sptBentConnector3:    
		case sptBentConnector4:
		CREATE_BY_SPT(sptBentConnector5, DocFileFormat::BentConnector)

		case sptCurvedConnector2:
		case sptCurvedConnector3:    
		case sptCurvedConnector4:
		CREATE_BY_SPT(sptCurvedConnector5, DocFileFormat::CurvedConnector)

		case sptTextPlainText:    
		case sptTextStop:  
		case sptTextTriangle:   
		case sptTextTriangleInverted:
		case sptTextChevron:
		case sptTextChevronInverted:
		case sptTextRingInside:
		case sptTextRingOutside:
		case sptTextArchUpCurve:   
		case sptTextArchDownCurve: 
		case sptTextCircleCurve: 
		case sptTextButtonCurve: 
		case sptTextArchUpPour:  
		case sptTextArchDownPour: 
		case sptTextCirclePour:
		case sptTextButtonPour:  
		case sptTextCurveUp:  
		case sptTextCurveDown: 
		case sptTextCascadeUp:   
		case sptTextCascadeDown:
		case sptTextWave1:   
		case sptTextWave2:   
		case sptTextWave3:   
		case sptTextWave4: 
		case sptTextInflate:   
		case sptTextDeflate:    
		case sptTextInflateBottom:  
		case sptTextDeflateBottom:  
		case sptTextInflateTop:
		case sptTextDeflateTop:   
		case sptTextDeflateInflate:   
		case sptTextDeflateInflateDeflate:
		case sptTextFadeRight: 
		case sptTextFadeLeft:   
		case sptTextFadeUp:   
		case sptTextFadeDown:   
		case sptTextSlantUp:    
		case sptTextSlantDown:   
		case sptTextCanUp:   
		CREATE_BY_SPT(sptTextCanDown, DocFileFormat::TextboxType)

		default: break;
	};

	if (NULL != pShape)
		pShape->m_eType = type;

	return CBaseShapePtr(pShape);
}
