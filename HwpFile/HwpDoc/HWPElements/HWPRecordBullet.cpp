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

	oBuffer.Skip(2); //TODO:: ByteBuffer.wrap(buf, offset, 2).order(ByteOrder.LITTLE_ENDIAN).getChar();

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadInt(m_nBulletImage);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadByte(m_chBright);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadByte(m_chContrast);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadByte(m_chImageEffect);

	if (nSize > oBuffer.GetDistanceToLastPos(true))
	{
		short shValue;
		oBuffer.ReadShort(shValue);

		m_sBinItemRefID = TO_HWP_STRING(shValue);
	}
}
}
