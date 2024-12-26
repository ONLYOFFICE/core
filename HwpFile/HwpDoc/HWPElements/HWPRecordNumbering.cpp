#include "HWPRecordNumbering.h"

namespace HWP
{
CHWPRecordNumbering::CHWPRecordNumbering(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
    : CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.SavePosition();

	for (int nIndex = 0; nIndex < 7; ++nIndex)
	{
		int nTypeBits;
		oBuffer.ReadInt(nTypeBits);

		oBuffer.ReadByte(m_arNumbering[nIndex].m_chAlign);
		m_arNumbering[nIndex].m_bUseInstWidth = CHECK_FLAG(nTypeBits, 0x40);
		m_arNumbering[nIndex].m_bAutoIndent = CHECK_FLAG(nTypeBits, 0x80);
		m_arNumbering[nIndex].m_chTextOffsetType = (HWP_BYTE)((nTypeBits >> 4) & 0x01);

		oBuffer.ReadShort(m_arNumbering[nIndex].m_shWidthAdjust);
		oBuffer.ReadShort(m_arNumbering[nIndex].m_shTextOffset);
		oBuffer.ReadInt(m_arNumbering[nIndex].m_nCharShape);
		oBuffer.ReadString(m_arNumbering[nIndex].m_sNumFormat, EStringCharacter::UTF16);
	}

	oBuffer.ReadShort(m_shStart);

	if (nVersion > 5025 && (nSize > oBuffer.GetDistanceToLastPos())) //TODO:: добавить проверку offset-off < size
	{
		for (int nIndex = 0; nIndex < 7; ++nIndex)
			oBuffer.ReadInt(m_arNumbering[nIndex].m_nStartNumber);
	}

	if (nVersion > 5100 && (nSize > oBuffer.GetDistanceToLastPos())) //TODO:: добавить проверку offset-off < size
	{
		for (int nIndex = 0; nIndex < 3; ++nIndex)
		{
			//Содержимое неизвестно, но оно содержит 8 байт.
			oBuffer.Skip(8);

			//Содержимое неизвестно, но оно содержит 4 байт.
			oBuffer.Skip(4);

			short shLen;
			oBuffer.ReadShort(shLen);

			oBuffer.Skip(shLen);
		}

		for (int nIndex = 0; nIndex < 3; ++nIndex)
			oBuffer.ReadInt(m_arExtLevelStart[nIndex]);
	}

	oBuffer.RemoveLastSavedPos();
}
}
