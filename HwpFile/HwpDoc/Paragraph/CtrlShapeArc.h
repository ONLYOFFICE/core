#ifndef CTRLSHAPEARC_H
#define CTRLSHAPEARC_H

#include "CtrlShapeEllipse.h"
#include "Point.h"

namespace HWP
{
class CCtrlShapeArc : public CCtrlGeneralShape
{
	EArcType m_eType;
	int m_nCenterX;
	int m_nCenterY;
	int m_nAxixX1;
	int m_nAxixY1;
	int m_nAxixX2;
	int m_nAxixY2;
public:
	CCtrlShapeArc();
	CCtrlShapeArc(const HWP_STRING& sCtrlID);
	CCtrlShapeArc(const CCtrlGeneralShape& oShape);
	CCtrlShapeArc(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapeArc(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion);

	EShapeType GetShapeType() const override;

	TPoint GetCenterPoint() const;
	TPoint GetFirstPoint()  const;
	TPoint GetSecondPoint() const;

	static int ParseElement(CCtrlShapeArc& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeArc& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEARC_H
