#ifndef CTRLOBJELEMENT_H
#define CTRLOBJELEMENT_H

#include "CtrlCommon.h"

namespace HWP
{
class CCtrlObjElement : public CCtrlCommon
{
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
	VECTOR<double> m_arMatrix;
	VECTOR<double> m_arMatrixSeq;

	void SetMatrix(CXMLNode& oNode, VECTOR<double>& arMatrix, int nOffset);
public:
	CCtrlObjElement();
	CCtrlObjElement(const HWP_STRING& sCtrlID);
	CCtrlObjElement(const CCtrlObjElement& oObjElement);
	CCtrlObjElement(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlObjElement(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion);

	int GetCurWidth() const;
	int GetCurHeight() const;

	static int ParseCtrl(CCtrlObjElement& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLOBJELEMENT_H
