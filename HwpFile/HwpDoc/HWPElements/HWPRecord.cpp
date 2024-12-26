#include "HWPRecord.h"

namespace HWP
{
CHWPRecord::CHWPRecord(int nTagNum, int nLevel, int nSize)
	: m_nLevel(nLevel), m_nSize(nSize)
{
	m_eTag = GetTagFromNum(nTagNum);
}

CHWPRecord::CHWPRecord(EHWPTag eTag, int nLevel, int nSize)
	: m_eTag(eTag), m_nLevel(nLevel), m_nSize(nSize)
{}

CHWPRecord::~CHWPRecord()
{}
}
