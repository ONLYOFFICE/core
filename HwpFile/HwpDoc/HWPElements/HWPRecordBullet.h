#ifndef HWPRECORDBULLET_H
#define HWPRECORDBULLET_H

#include "HWPRecord.h"
#include "../HWPStream.h"
#include "../HWPDocInfo.h"
#include "HWPRecordNumbering.h"
#include "../Common/XMLReader.h"

namespace HWP
{
class CHWPRecordBullet : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	TNumbering m_oHeaderInfo;
	HWP_CHAR m_chBulletChar;
	int m_nBulletImage;

	HWP_BYTE m_chBright;
	HWP_BYTE m_chContrast;
	HWP_BYTE m_chImageEffect;
	HWP_STRING m_sBinItemRefID;
	HWP_CHAR m_chCheckBulletChar;
public:
	CHWPRecordBullet(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordBullet(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType);
};
}

#endif // HWPRECORDBULLET_H
