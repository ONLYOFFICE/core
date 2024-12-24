#ifndef CTRLOBJELEMENT_H
#define CTRLOBJELEMENT_H

#include "CtrlCommon.h"

namespace HWP
{
class CCtrlObjElement : public CCtrlCommon
{
	int m_nSize;

	int m_nXGrpOffset;
	int m_nYGrpOffset;
	short m_shNGrp;
	short m_shVer;
	int m_nIniWidth;
	int m_nIniHeight;
	int m_nCurWidth;
	int m_nCurHeight;
	bool m_bHorzFlip;
	bool m_bVerFlip;
	short m_shRotat;
	int m_nXCenter;
	int m_nYCenter;
	short m_shMatCnt;
	double m_arMatrix[6];
	VECTOR<double> m_arMatrixSeq;
public:
	CCtrlObjElement();
	CCtrlObjElement(const STRING& sCtrlID);
	CCtrlObjElement(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;

	static int ParseCtrl(CCtrlObjElement& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLOBJELEMENT_H
