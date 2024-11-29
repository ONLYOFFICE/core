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

	BYTE m_chBright;
	BYTE m_chContrast;
	BYTE m_chImageEffect;
	std::string m_sBinItemRefID;
	wchar_t m_wchCheckBulletChar;
public:
	CHWPRecordBullet(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDBULLET_H
