#ifndef HWPRECORDBULLET_H
#define HWPRECORDBULLET_H

#include "HWPRecord.h"
#include "../HWPStream.h"
#include "../HWPDocInfo.h"
#include "HWPRecordNumbering.h"

namespace HWP
{
class CHWPRecordBullet : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	TNumbering m_oHeaderInfo;
	wchar_t m_wchBulletChar;
	int m_nBulletImage;

	HWP_BYTE m_chBright;
	HWP_BYTE m_chContrast;
	HWP_BYTE m_chImageEffect;
	HWP_STRING m_sBinItemRefID;
	wchar_t m_wchCheckBulletChar;
public:
	CHWPRecordBullet(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDBULLET_H
