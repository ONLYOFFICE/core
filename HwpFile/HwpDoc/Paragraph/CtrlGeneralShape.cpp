#include "CtrlGeneralShape.h"

namespace HWP
{
CCtrlGeneralShape::CCtrlGeneralShape(const STRING& sCtrlID)
	: CCtrlObjElement(sCtrlID)
{}

CCtrlGeneralShape::CCtrlGeneralShape(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlObjElement(sCtrlID, nSize, oBuffer, nOff, nVersion), m_pFill(nullptr)
{}

CCtrlGeneralShape::~CCtrlGeneralShape()
{
	if (nullptr != m_pFill)
		delete m_pFill;
}

void CCtrlGeneralShape::SetParent(CHWPPargraph* pParent)
{
	m_pParent = pParent;
}

CHWPPargraph* CCtrlGeneralShape::GetParent()
{
	return m_pParent;
}

int CCtrlGeneralShape::GetSize()
{
	return m_nSize;
}

int CCtrlGeneralShape::ParseListHeaderApend(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	BYTE* pOldCurrentPos = oBuffer.GetCurPtr();

	if (nSize >= 16)
	{
		oBuffer.Skip(2);
		oBuffer.ReadInt(oObj.m_nCaptionAttr);
		oBuffer.ReadInt(oObj.m_nCaptionWidth);
		oObj.m_nCaptionSpacing = oBuffer.ReadShort();
		oBuffer.ReadInt(oObj.m_nCaptionMaxW);
	}

	if ((nSize - (int)(oBuffer.GetCurPtr() - pOldCurrentPos)) == 8)
		oBuffer.Skip(8);

	return nSize;
}

int CCtrlGeneralShape::ParseCtrl(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	BYTE* pOldCurrentPos = oBuffer.GetCurPtr();

	CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);

	oBuffer.ReadColor(oObj.m_nLineColor);
	oObj.m_nLineThick = oBuffer.ReadShort();

	oBuffer.Skip(2);

	int nLineAttr;
	oBuffer.ReadInt(nLineAttr);

	oObj.m_eLineStyle = GetLineStyle2(nLineAttr & 0x3F);
	oObj.m_eLineHead = GetLineArrowStyle(((nLineAttr >> 10) & 0x3F), CHECK_FLAG(nLineAttr >> 30, 0x1));
	oObj.m_eLineHeadSz = GetLineArrowSize((nLineAttr >> 22) & 0x0F);
	oObj.m_eLineTail = GetLineArrowStyle(((nLineAttr >> 16) & 0x3F), CHECK_FLAG(nLineAttr >> 31, 0x1));
	oObj.m_eLineTailSz = GetLineArrowSize((nLineAttr >> 26) & 0x0F);
	oBuffer.ReadByte(oObj.m_chOutline);

	oObj.m_pFill = new CFill(oBuffer, 0, nSize - (oBuffer.GetCurPtr() - pOldCurrentPos) - 22);

	oBuffer.Skip(22);

	return oBuffer.GetCurPtr() - pOldCurrentPos;
}
}
