#include "CtrlShapeConnectLine.h"

namespace HWP
{
EConnectLineType GetConnectLineType(int nValue)
{
	switch(static_cast<EConnectLineType>(nValue))
	{
		case EConnectLineType::STRAIGHT_NOARROW:
		case EConnectLineType::STRAIGHT_ONEWAY:
		case EConnectLineType::STRAIGHT_BOTH:
		case EConnectLineType::STROKE_NOARROW:
		case EConnectLineType::STROKE_ONEWAY:
		case EConnectLineType::STROKE_BOTH:
		case EConnectLineType::ARC_NOARROW:
		case EConnectLineType::ARC_ONEWAY:
		case EConnectLineType::ARC_BOTH:
			return static_cast<EConnectLineType>(nValue);
		default:
			return EConnectLineType::null;
	}
}

CCtrlShapeConnectLine::CCtrlShapeConnectLine(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}
}
