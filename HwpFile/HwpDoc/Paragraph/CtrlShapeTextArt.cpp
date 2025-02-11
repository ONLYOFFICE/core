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

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID, oNode, nVersion)
{
	m_sText = oNode.GetAttribute(L"text");

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:pt0" == oChild.GetName())
		{
			m_oPt0.m_nX = oNode.GetAttributeInt(L"x");
			m_oPt0.m_nY = oNode.GetAttributeInt(L"y");
		}
		else if (L"hp:pt1" == oChild.GetName())
		{
			m_oPt1.m_nX = oNode.GetAttributeInt(L"x");
			m_oPt1.m_nY = oNode.GetAttributeInt(L"y");
		}
		else if (L"hp:pt2" == oChild.GetName())
		{
			m_oPt2.m_nX = oNode.GetAttributeInt(L"x");
			m_oPt2.m_nY = oNode.GetAttributeInt(L"y");
		}
		else if (L"hp:pt3" == oChild.GetName())
		{
			m_oPt3.m_nX = oNode.GetAttributeInt(L"x");
			m_oPt3.m_nY = oNode.GetAttributeInt(L"y");
		}
		else if (L"hp:textartPr" == oChild.GetName())
		{
			m_sFontName = oChild.GetAttribute(L"fontName");
			m_sFontStyle = oChild.GetAttribute(L"fontStyle");
			m_sFontType = oChild.GetAttribute(L"fontType");
			m_sTextShape = oChild.GetAttribute(L"textShape");
			m_sFontStyle = oChild.GetAttribute(L"fontStyle");
			m_shLineSpacing = oChild.GetAttributeInt(L"lineSpacing");
			m_shSpacing = oChild.GetAttributeInt(L"spacing");
			m_sAlign = oChild.GetAttribute(L"align");

			//TODO:: реализовать shadows
		}
		else if (L"hp:outline" == oChild.GetName())
		{
			std::vector<CXMLNode> arGrandChilds{oChild.GetChilds(L"hp:pt")};
			m_arOutline.resize(arGrandChilds.size());

			for (unsigned int unIndex = 0; unIndex < arGrandChilds.size(); ++unIndex)
			{
				m_arOutline[unIndex].m_nX = arGrandChilds[unIndex].GetAttributeInt(L"x");
				m_arOutline[unIndex].m_nY = arGrandChilds[unIndex].GetAttributeInt(L"y");
			}
		}
	}
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
