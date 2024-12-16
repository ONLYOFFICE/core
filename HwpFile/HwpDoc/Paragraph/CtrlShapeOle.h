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
	STRING m_sBinDataID;
	int m_nBorderColor;
	int m_nBorderThick;
	int m_nBorderAttr;
public:
	CCtrlShapeOle();
	CCtrlShapeOle(const STRING& sCtrlID);
	CCtrlShapeOle(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEOLE_H
