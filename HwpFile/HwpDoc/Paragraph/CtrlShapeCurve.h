#ifndef CTRLSHAPECURVE_H
#define CTRLSHAPECURVE_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
class CCtrlShapeCurve : public CCtrlGeneralShape
{
	int m_nPoints;
	std::vector<TPoint> m_arPoints;
	std::vector<BYTE> m_arSegmentType;
public:
	CCtrlShapeCurve();
	CCtrlShapeCurve(const STRING& sCtrlID);
	CCtrlShapeCurve(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlShapeCurve& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeCurve& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPECURVE_H
