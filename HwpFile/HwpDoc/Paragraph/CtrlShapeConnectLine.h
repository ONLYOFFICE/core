#ifndef CTRLSHAPECONNECTLINE_H
#define CTRLSHAPECONNECTLINE_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
struct TConnectPoint : public TPoint
{
	short m_shSubjectIDRef;
	short m_shSubjectIdx;
};

enum class EConnectLineType
{
	STRAIGHT_NOARROW,
	STRAIGHT_ONEWAY,
	STRAIGHT_BOTH,
	STROKE_NOARROW,
	STROKE_ONEWAY,
	STROKE_BOTH,
	ARC_NOARROW,
	ARC_ONEWAY,
	ARC_BOTH,
	null
};

class CCtrlShapeConnectLine : public CCtrlGeneralShape
{
	EConnectLineType m_eType;
	TConnectPoint m_oStartPt;
	TConnectPoint m_oEndPt;
public:
	CCtrlShapeConnectLine(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	EShapeType GetShapeType() const override;
};
}

#endif // CTRLSHAPECONNECTLINE_H
