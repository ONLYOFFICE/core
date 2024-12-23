#include "HWPRecordIDMaping.h"

namespace HWP
{

EIndex GetIndex(int nValue)
{
	switch(static_cast<EIndex>(nValue))
	{
		case EIndex::BIN_DATA:
		case EIndex::FACENAME_HANGUL:
		case EIndex::FACENAME_ENGLISH:
		case EIndex::FACENAME_CHINESE:
		case EIndex::FACENAME_JAPANESE:
		case EIndex::FACENAME_ETC:
		case EIndex::FACENAME_SYMBOL:
		case EIndex::FACENAME_USER:
		case EIndex::BORDER_FILL:
		case EIndex::CHAR_SHAPE:
		case EIndex::TAB_DEF:
		case EIndex::NUMBERING:
		case EIndex::BULLET:
		case EIndex::PARA_SHAPE:
		case EIndex::STYLE:
		case EIndex::MEMO_SHAPE:
		case EIndex::TRACK_CHANGE:
		case EIndex::TRACK_CHANGE_USER:
		case EIndex::MAX:
			return static_cast<EIndex>(nValue);
		default:
			return EIndex::null;
	}
}

CHWPRecordIDMaping::CHWPRecordIDMaping(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	for (int nIndex = 0; nIndex < (nSize / 4); ++nIndex)
	{
		int nCount;
		oBuffer.ReadInt(nCount);
		m_arIdMappingNum.push_back(nCount);

		m_arCounts[nIndex] = nCount;

	}
}
}
