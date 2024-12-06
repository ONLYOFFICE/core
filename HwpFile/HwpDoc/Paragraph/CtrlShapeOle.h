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
	std::string m_sBinDataID;
	int m_nBorderColor;
	int m_nBorderThick;
	int m_nBorderAttr;
public:
	CCtrlShapeOle(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEOLE_H
