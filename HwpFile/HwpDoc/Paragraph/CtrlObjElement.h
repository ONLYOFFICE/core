#ifndef CTRLOBJELEMENT_H
#define CTRLOBJELEMENT_H

#include "CtrlCommon.h"

namespace HWP
{
struct TMatrix
{
	double m_dM11; // e1
	double m_dM12; // e2
	double m_dM21; // e3
	double m_dM22; // e4
	double m_dDX;  // e5
	double m_dDY;  // e6

	TMatrix();
	TMatrix(double dM11, double dM12, double dM21, double dM22, double dDX, double dDY);
	void Multiply(const TMatrix& oMatrix);

	void ApplyToPoint(double& dX, double& dY);
	void ApplyToSize(double& dW, double& dH);
};

class CCtrlObjElement : public CCtrlCommon
{
	int m_nXGrpOffset;
	int m_nYGrpOffset;
	short m_shNGrp;
	short m_shVer;
	int m_nOrgWidth;
	int m_nOrgHeight;
	int m_nCurWidth;
	int m_nCurHeight;
	bool m_bHorzFlip;
	bool m_bVerFlip;
	short m_shRotat;
	int m_nXCenter;
	int m_nYCenter;
	short m_shMatCnt;
	VECTOR<TMatrix> m_arMatrixs;
public:
	CCtrlObjElement();
	CCtrlObjElement(const HWP_STRING& sCtrlID);
	CCtrlObjElement(const CCtrlObjElement& oObjElement);
	CCtrlObjElement(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlObjElement(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion);

	int GetCurWidth() const;
	int GetCurHeight() const;

	int GetOrgWidth() const;
	int GetOrgHeight() const;

	int GetFinalWidth() const;
	int GetFinalHeight() const;

	TMatrix GetFinalMatrix() const;

	static int ParseCtrl(CCtrlObjElement& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLOBJELEMENT_H
