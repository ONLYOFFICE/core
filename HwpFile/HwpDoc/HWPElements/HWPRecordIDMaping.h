#ifndef HWPRECORDIDMAPING_H
#define HWPRECORDIDMAPING_H

#include "HwpDoc/HWPDocInfo.h"
#include "../HWPStream.h"
#include "HWPRecord.h"
#include <list>

namespace HWP
{
enum class EIndex
{
	BIN_DATA,
	FACENAME_HANGUL,
	FACENAME_ENGLISH,
	FACENAME_CHINESE,
	FACENAME_JAPANESE,
	FACENAME_ETC,
	FACENAME_SYMBOL,
	FACENAME_USER,
	BORDER_FILL,
	HWP_CHAR_SHAPE,
	TAB_DEF,
	NUMBERING,
	BULLET,
	PARA_SHAPE,
	STYLE,
	MEMO_SHAPE,
	TRACK_CHANGE,
	TRACK_CHANGE_USER,
	MAX,
	null
};

class CHWPRecordIDMaping : public CHWPRecord
{
	CHWPDocInfo *m_pParent;
	LIST<int> m_arIdMappingNum;

	int m_arCounts[static_cast<int>(EIndex::MAX)];

public:
	CHWPRecordIDMaping(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDIDMAPING_H
