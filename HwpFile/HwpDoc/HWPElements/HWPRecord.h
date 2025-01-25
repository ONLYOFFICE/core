#ifndef HWPRECORD_H
#define HWPRECORD_H

#include "HWPTag.h"

namespace HWP
{
class CHWPRecord
{
	EHWPTag m_eTag;
	int m_nLevel;
	int m_nSize;
public:
	CHWPRecord(int nTagNum, int nLevel, int nSize);
	CHWPRecord(EHWPTag eTag, int nLevel, int nSize);
	virtual ~CHWPRecord();
};
}

#endif // HWPRECORD_H
