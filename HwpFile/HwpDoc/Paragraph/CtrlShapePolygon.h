#ifndef CTRLSHAPEPOLYGON_H
#define CTRLSHAPEPOLYGON_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
class CCtrlShapePolygon : public CCtrlGeneralShape
{
	int m_nPoints;
	VECTOR<TPoint> m_arPoints;
public:
	CCtrlShapePolygon();
	CCtrlShapePolygon(const HWP_STRING& sCtrlID);
	CCtrlShapePolygon(const CCtrlGeneralShape& oShape);
	CCtrlShapePolygon(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapePolygon(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion);

	EShapeType GetShapeType() const override;

	static int ParseElement(CCtrlShapePolygon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapePolygon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlShapePolygon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEPOLYGON_H
