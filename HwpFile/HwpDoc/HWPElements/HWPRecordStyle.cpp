#include "HWPRecordStyle.h"

namespace HWP
{
CHWPRecordStyle::CHWPRecordStyle(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(nullptr)
{}

CHWPRecordStyle::CHWPRecordStyle(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.ReadString(m_sName, EStringCharacter::UTF16);
	oBuffer.ReadString(m_sEngName, EStringCharacter::UTF16);

	m_chType = (BYTE)(oBuffer.ReadByte() & 0x00FF);
	m_chNextStyle = (BYTE)(oBuffer.ReadByte() & 0x00FF);

	oBuffer.ReadShort(m_shLangID);
	m_nParaShape = oBuffer.ReadShort();
	m_nCharShape = oBuffer.ReadShort();
}
}
