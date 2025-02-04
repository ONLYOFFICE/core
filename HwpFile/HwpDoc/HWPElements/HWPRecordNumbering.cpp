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

		m_arNumbering[nIndex].m_chAlign = (HWP_BYTE)(nTypeBits & 0x03);
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

CHWPRecordNumbering::CHWPRecordNumbering(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion)
    : CHWPRecord(EHWPTag::HWPTAG_NUMBERING, 0, 0), m_pParent(&oDocInfo)
{
	m_shStart = oNode.GetAttributeInt(L"start", 1);

	unsigned int unIndex = 0;
	HWP_STRING sType;

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hh:paraHead" == oChild.GetName() ||
		    L"paraHead" == oChild.GetName())
		{
			sType = oChild.GetAttribute(L"align");

			if (L"LEFT" == sType)
				m_arNumbering[unIndex].m_chAlign = 0;
			else if (L"CENTER" == sType)
				m_arNumbering[unIndex].m_chAlign = 1;
			else if (L"RIGHT" == sType)
				m_arNumbering[unIndex].m_chAlign = 2;

			m_arNumbering[unIndex].m_bUseInstWidth = oChild.GetAttributeBool(L"useInstWidth");
			m_arNumbering[unIndex].m_bAutoIndent = oChild.GetAttributeBool(L"autoIndent");
			m_arNumbering[unIndex].m_shWidthAdjust = oChild.GetAttributeInt(L"widthAdjust");

			sType =  oChild.GetAttribute(L"textOffsetType");

			if (L"PERCENT" == sType)
				m_arNumbering[unIndex].m_chTextOffsetType = 0;
			else if (L"HWPUNIT" == sType)
				m_arNumbering[unIndex].m_chTextOffsetType = 1;

			m_arNumbering[unIndex].m_shTextOffset = oChild.GetAttributeInt(L"textOffset");

			short shLevel = oChild.GetAttributeInt(L"level");

			sType = oChild.GetAttribute(L"numFormat");

			if (L"DIGIT" == sType)
			{
				if (shLevel > 0 && shLevel < 11)
					m_arNumbering[unIndex].m_sNumFormat = L'^' + std::to_wstring(shLevel) + L'.';
			}
			else if (L"HANGUL_SYLLABLE" == sType ||
			         L"HANGUL_JAMO" == sType)
			{
				switch (shLevel)
				{
					case 1:  m_arNumbering[unIndex].m_sNumFormat = L"^가."; break;
					case 2:  m_arNumbering[unIndex].m_sNumFormat = L"^나."; break;
					case 3:  m_arNumbering[unIndex].m_sNumFormat = L"^다."; break;
					case 4:  m_arNumbering[unIndex].m_sNumFormat = L"^라."; break;
					case 5:  m_arNumbering[unIndex].m_sNumFormat = L"^마."; break;
					case 6:  m_arNumbering[unIndex].m_sNumFormat = L"^바."; break;
					case 7:  m_arNumbering[unIndex].m_sNumFormat = L"^사."; break;
					case 8:  m_arNumbering[unIndex].m_sNumFormat = L"^아."; break;
					case 9:  m_arNumbering[unIndex].m_sNumFormat = L"^자."; break;
					case 10: m_arNumbering[unIndex].m_sNumFormat = L"^차."; break;
				}
			}
			else if (L"CIRCLED_DIGIT" == sType)
			{
				switch (shLevel)
				{
					case 1:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2460."; break;
					case 2:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2461."; break;
					case 3:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2462."; break;
					case 4:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2463."; break;
					case 5:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2464."; break;
					case 6:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2465."; break;
					case 7:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2466."; break;
					case 8:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2467."; break;
					case 9:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2468."; break;
					case 10: m_arNumbering[unIndex].m_sNumFormat = L"^\u2469."; break;
				}
			}
			else if (L"LATIN_SMALL" == sType)
			{
				switch (shLevel)
				{
					case 1:  m_arNumbering[unIndex].m_sNumFormat = L"^a."; break;
					case 2:  m_arNumbering[unIndex].m_sNumFormat = L"^b."; break;
					case 3:  m_arNumbering[unIndex].m_sNumFormat = L"^c."; break;
					case 4:  m_arNumbering[unIndex].m_sNumFormat = L"^d."; break;
					case 5:  m_arNumbering[unIndex].m_sNumFormat = L"^e."; break;
					case 6:  m_arNumbering[unIndex].m_sNumFormat = L"^f."; break;
					case 7:  m_arNumbering[unIndex].m_sNumFormat = L"^g."; break;
					case 8:  m_arNumbering[unIndex].m_sNumFormat = L"^h."; break;
					case 9:  m_arNumbering[unIndex].m_sNumFormat = L"^i."; break;
					case 10: m_arNumbering[unIndex].m_sNumFormat = L"^j."; break;
				}
			}
			else if (L"CIRCLED_HANGUL_SYLLABLE" == sType)
			{
				switch (shLevel)
				{
					case 1:  m_arNumbering[unIndex].m_sNumFormat = L"^\u326E."; break;
					case 2:  m_arNumbering[unIndex].m_sNumFormat = L"^\u326F."; break;
					case 3:  m_arNumbering[unIndex].m_sNumFormat = L"^\u3270."; break;
					case 4:  m_arNumbering[unIndex].m_sNumFormat = L"^\u3271."; break;
					case 5:  m_arNumbering[unIndex].m_sNumFormat = L"^\u3272."; break;
					case 6:  m_arNumbering[unIndex].m_sNumFormat = L"^\u3273."; break;
					case 7:  m_arNumbering[unIndex].m_sNumFormat = L"^\u3274."; break;
					case 8:  m_arNumbering[unIndex].m_sNumFormat = L"^\u3275."; break;
					case 9:  m_arNumbering[unIndex].m_sNumFormat = L"^\u3276."; break;
					case 10: m_arNumbering[unIndex].m_sNumFormat = L"^\u3277."; break;
				}
			}
			else if (L"ROMAN_SMALL" == sType)
			{
				switch (shLevel)
				{
					case 1:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2170."; break;
					case 2:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2171."; break;
					case 3:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2172."; break;
					case 4:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2173."; break;
					case 5:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2174."; break;
					case 6:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2175."; break;
					case 7:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2176."; break;
					case 8:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2177."; break;
					case 9:  m_arNumbering[unIndex].m_sNumFormat = L"^\u2178."; break;
					case 10: m_arNumbering[unIndex].m_sNumFormat = L"^\u2179."; break;
				}
			}

			m_arNumbering[unIndex].m_nCharShape = std::abs(oChild.GetAttributeInt(L"charPrIDRef"));
			m_arNumbering[unIndex].m_nStartNumber = oChild.GetAttributeInt(L"start");
			++unIndex;
		}

		if (7 == unIndex)
			return;
	}
}

short CHWPRecordNumbering::GetStart() const
{
	return m_shStart;
}

HWP_STRING CHWPRecordNumbering::GetNumFormat(unsigned short ushIndex) const
{
	if (ushIndex >= 7)
		return HWP_STRING();

	return m_arNumbering[ushIndex].m_sNumFormat;
}

HWP_BYTE CHWPRecordNumbering::GetAlign(unsigned short ushIndex) const
{
	if (ushIndex >= 7)
		return 0;

	return m_arNumbering[ushIndex].m_chAlign;
}

int CHWPRecordNumbering::GetStartNumber(unsigned short ushIndex) const
{
	if (ushIndex >= 7)
		return 0;

	return m_arNumbering[ushIndex].m_nStartNumber;
}

int CHWPRecordNumbering::GetCharShape(unsigned short ushIndex) const
{
	if (ushIndex >= 7)
		return 0;

	return m_arNumbering[ushIndex].m_nCharShape;
}
}
