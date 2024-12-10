#ifndef CTRLSHAPEARC_H
#define CTRLSHAPEARC_H

#include "CtrlShapeEllipse.h"

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
	CCtrlShapeArc(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlShapeArc& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeArc& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEARC_H
