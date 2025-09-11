#include "CtrlShapeOle.h"

namespace HWP
{
CCtrlShapeOle::CCtrlShapeOle()
{}

CCtrlShapeOle::CCtrlShapeOle(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeOle::CCtrlShapeOle(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeOle::CCtrlShapeOle(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeOle::CCtrlShapeOle(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, nVersion)
{
	m_sBinDataID = oReader.GetAttribute("binaryItemIDRef");

	WHILE_READ_NEXT_NODE(oReader)
	{
		if ("hc:extent" == oReader.GetName())
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nExtentX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nExtentY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, nVersion, eType);
	}
	END_WHILE
}

EShapeType CCtrlShapeOle::GetShapeType() const
{
	return EShapeType::Ole;
}

HWP_STRING CCtrlShapeOle::GetBinDataID() const
{
	return m_sBinDataID;
}

int CCtrlShapeOle::ParseElement(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(oObj.m_nAttr);
	oBuffer.ReadInt(oObj.m_nExtentX);
	oBuffer.ReadInt(oObj.m_nExtentY);
	oObj.m_sBinDataID = std::to_wstring(oBuffer.ReadShort());
	oBuffer.ReadColor(oObj.m_nBorderColor);
	oBuffer.ReadInt(oObj.m_nBorderThick);
	oBuffer.ReadInt(oObj.m_nBorderAttr);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}

int CCtrlShapeOle::ParseCtrl(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

}
