#include "HWPRecordBullet.h"

namespace HWP
{
CHWPRecordBullet::CHWPRecordBullet(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.SavePosition();

	int nTypeBits;
	oBuffer.ReadInt(nTypeBits);

	m_oHeaderInfo.m_chAlign = (HWP_BYTE)((nTypeBits) & 0x03);
	m_oHeaderInfo.m_bUseInstWidth = CHECK_FLAG(nTypeBits, 0x40);
	m_oHeaderInfo.m_bAutoIndent = CHECK_FLAG(nTypeBits, 0x80);
	m_oHeaderInfo.m_chTextOffsetType = (HWP_BYTE)((nTypeBits >> 4) & 0x01);

	oBuffer.ReadShort(m_oHeaderInfo.m_shWidthAdjust);
	oBuffer.ReadShort(m_oHeaderInfo.m_shTextOffset);
	oBuffer.ReadInt(m_oHeaderInfo.m_nCharShape);

	oBuffer.ReadChar(m_chBulletChar);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadInt(m_nBulletImage);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadByte(m_chBright);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadByte(m_chContrast);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadByte(m_chImageEffect);

	if (nSize > oBuffer.GetDistanceToLastPos())
	{
		short shValue;
		oBuffer.ReadShort(shValue);

		m_sBinItemRefID = TO_HWP_STRING(shValue);
	}

	if (nSize > oBuffer.GetDistanceToLastPos(true))
		oBuffer.ReadChar(m_chCheckBulletChar);
}

CHWPRecordBullet::CHWPRecordBullet(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_BULLET, 0, 0), m_pParent(&oDocInfo)
{
	std::wstring wsAttributeValue = oNode.GetAttribute(L"char");

	if (!wsAttributeValue.empty())
		m_chBulletChar = wsAttributeValue.at(0);

	wsAttributeValue = oNode.GetAttribute(L"checkedChar");

	if (!wsAttributeValue.empty())
		m_chCheckBulletChar = wsAttributeValue.at(0);

	m_nBulletImage = oNode.GetAttributeInt(L"useImage");

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hc:img" == oChild.GetName())
			m_sBinItemRefID = oChild.GetAttribute(L"binaryItemIDRef");
	}
}
}
