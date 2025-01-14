#ifndef CTRLSHAPEOLE_H
#define CTRLSHAPEOLE_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlShapeOle : public CCtrlGeneralShape
{
	int m_nAttr;
	int m_nExtentX;
	int m_nExtentY;
	HWP_STRING m_sBinDataID;
	int m_nBorderColor;
	int m_nBorderThick;
	int m_nBorderAttr;
public:
	CCtrlShapeOle();
	CCtrlShapeOle(const HWP_STRING& sCtrlID);
	CCtrlShapeOle(const CCtrlGeneralShape& oShape);
	CCtrlShapeOle(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	EShapeType GetShapeType() const override;

	HWP_STRING GetBinDataID() const;

	static int ParseElement(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEOLE_H
