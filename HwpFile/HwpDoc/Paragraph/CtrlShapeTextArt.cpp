#include "CtrlShapeTextArt.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CCtrlShapeTextArt::CCtrlShapeTextArt()
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, nVersion, eType)
{
	switch(eType)
	{
		case EHanType::HWPX:  ReadFromHWPX (oReader, nVersion); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
	}
}

void CCtrlShapeTextArt::ReadFromHWPX(CXMLReader &oReader, int nVersion)
{
	m_sText = oReader.GetAttribute("text");

	#define READ_POINT(variable_point)\
	{\
		START_READ_ATTRIBUTES(oReader)\
		{\
			if ("x" == sAttributeName)\
				variable_point.m_nX = oReader.GetInt();\
			else if ("y" == sAttributeName)\
				variable_point.m_nY = oReader.GetInt();\
		}\
		END_READ_ATTRIBUTES(oReader)\
	}

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:pt0" == sNodeName)
			READ_POINT(m_oPt0)
		else if ("hp:pt1" == sNodeName)
			READ_POINT(m_oPt1)
		else if ("hp:pt2" == sNodeName)
			READ_POINT(m_oPt2)
		else if ("hp:pt3" == sNodeName)
			READ_POINT(m_oPt3)
		else if ("hp:textartPr" == sNodeName)
			ReadTextArtShape(oReader, EHanType::HWPX);
		else if ("hp:outline" == sNodeName)
			ReadOutlineData(oReader, EHanType::HWPX);
		else
			CCtrlGeneralShape::ParseChildren(oReader, nVersion, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeTextArt::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("Text" == sAttributeName)
			m_sText = oReader.GetText();
		else if ("X0" == sAttributeName)
			m_oPt0.m_nX = oReader.GetInt();
		else if ("Y0" == sAttributeName)
			m_oPt0.m_nY = oReader.GetInt();
		else if ("X1" == sAttributeName)
			m_oPt1.m_nX = oReader.GetInt();
		else if ("Y1" == sAttributeName)
			m_oPt1.m_nY = oReader.GetInt();
		else if ("X2" == sAttributeName)
			m_oPt2.m_nX = oReader.GetInt();
		else if ("Y2" == sAttributeName)
			m_oPt2.m_nY = oReader.GetInt();
		else if ("X3" == sAttributeName)
			m_oPt3.m_nX = oReader.GetInt();
		else if ("Y3" == sAttributeName)
			m_oPt3.m_nY = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("TEXTARTSHAPE" == sNodeName)
			ReadTextArtShape(oReader, EHanType::HWPML);
		else if ("OUTLINEDATA" == sNodeName)
			ReadOutlineData(oReader, EHanType::HWPML);
		else
			CCtrlGeneralShape::ParseChildren(oReader, 0, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeTextArt::ReadTextArtShape(CXMLReader &oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::FontName, eType) == sAttributeName)
			m_sFontName = oReader.GetText();
		else if (GetAttributeName(EAttribute::FontStyle, eType)== sAttributeName)
			m_sFontStyle = oReader.GetText();
		else if (GetAttributeName(EAttribute::FontType, eType) == sAttributeName)
			m_sFontType = oReader.GetText();
		else if (GetAttributeName(EAttribute::TextShape, eType) == sAttributeName)
			m_sTextShape = oReader.GetText();
		else if (GetAttributeName(EAttribute::Align, eType) == sAttributeName)
			m_sAlign = oReader.GetText();
		else if (GetAttributeName(EAttribute::LineSpacing, eType) == sAttributeName)
			m_shLineSpacing = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Spacing, eType) == sAttributeName)
			m_shSpacing = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	//TODO:: реализовать shadows
}

void CCtrlShapeTextArt::ReadOutlineData(CXMLReader &oReader, EHanType eType)
{
	TPoint oPoint{0, 0};

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::Point, eType))
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::X, eType) == sAttributeName)
				oPoint.m_nX = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Y, eType) == sAttributeName)
				oPoint.m_nY = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)

		m_arOutline.push_back(oPoint);

		oPoint.m_nX = 0;
		oPoint.m_nY = 0;
	}
	END_WHILE
}

EShapeType CCtrlShapeTextArt::GetShapeType() const
{
	return EShapeType::TextArt;
}

int CCtrlShapeTextArt::ParseElement(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	// TODO:: проверить

	// [HWP ambiguous] following 120bytes are unknown.
	// Document doesn't mention about this at all.

	oBuffer.Skip(nSize);
	return nSize;
}

int CCtrlShapeTextArt::ParseCtrl(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}
}
