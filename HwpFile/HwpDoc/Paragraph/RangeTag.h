#ifndef RANGETAG_H
#define RANGETAG_H

#include "../Common/Common.h"

namespace HWP
{
struct TRangeTag
{
	int m_nStartPos;
	int m_nEndPos;
	HWP_BYTE m_chType;
	HWP_BYTE m_arData[3];
};
}

#endif // RANGETAG_H
