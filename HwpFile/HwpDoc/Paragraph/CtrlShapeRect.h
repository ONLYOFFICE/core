#ifndef CTRLSHAPERECT_H
#define CTRLSHAPERECT_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
class CCtrlShapeRect : public CCtrlGeneralShape
{
	HWP_BYTE m_chCurv;
	TPoint m_arPoints[4];
public:
	CCtrlShapeRect();
	CCtrlShapeRect(const HWP_STRING& sCtrlID);
	CCtrlShapeRect(const CCtrlGeneralShape& oShape);
	CCtrlShapeRect(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapeRect(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType);

	EShapeType GetShapeType() const override;

	static int ParseElement(CCtrlShapeRect& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeRect& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlShapeRect& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPERECT_H
