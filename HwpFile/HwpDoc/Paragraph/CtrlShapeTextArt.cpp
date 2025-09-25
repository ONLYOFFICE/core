#include "CtrlShapeTextArt.h"

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

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion)
    : CCtrlGeneralShape(sCtrlID, oReader, nVersion)
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
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("fontName" == sAttributeName)
					m_sFontName = oReader.GetText();
				else if ("fontStyle" == sAttributeName)
					m_sFontStyle = oReader.GetText();
				else if ("fontType" == sAttributeName)
					m_sFontType = oReader.GetText();
				else if ("textShape" == sAttributeName)
					m_sTextShape = oReader.GetText();
				else if ("align" == sAttributeName)
					m_sAlign = oReader.GetText();
				else if ("lineSpacing" == sAttributeName)
					m_shLineSpacing = oReader.GetInt();
				else if ("spacing" == sAttributeName)
					m_shSpacing = oReader.GetInt();

				//TODO:: реализовать shadows
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:outline" == sNodeName)
		{
			TPoint oPoint{0, 0};

			WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hp:pt")
			{
				START_READ_ATTRIBUTES(oReader)
				{
					if ("x" == sAttributeName)
						oPoint.m_nX = oReader.GetInt();
					else if ("y" == sAttributeName)
						oPoint.m_nY = oReader.GetInt();
				}
				END_READ_ATTRIBUTES(oReader)

				m_arOutline.push_back(oPoint);

				oPoint = {0, 0};
			}
			END_WHILE
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, nVersion);
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
