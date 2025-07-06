#ifndef CTRLSHAPECURVE_H
#define CTRLSHAPECURVE_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
class CCtrlShapeCurve : public CCtrlGeneralShape
{
	int m_nPoints;
	VECTOR<TPoint> m_arPoints;
	VECTOR<HWP_BYTE> m_arSegmentType;
public:
	CCtrlShapeCurve();
	CCtrlShapeCurve(const HWP_STRING& sCtrlID);
	CCtrlShapeCurve(const CCtrlGeneralShape& oShape);
	CCtrlShapeCurve(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapeCurve(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion);

	EShapeType GetShapeType() const override;

	VECTOR<TPoint> GetPoints() const;
	VECTOR<HWP_BYTE> GetSegmentsType() const;

	static int ParseElement(CCtrlShapeCurve& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeCurve& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPECURVE_H
