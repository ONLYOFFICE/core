#ifndef CTRLSHAPEELLIPSE_H
#define CTRLSHAPEELLIPSE_H

#include "CtrlGeneralShape.h"

namespace HWP
{
enum class EArcType
{
	NORMAL,
	PIE,
	CHORD
};

EArcType GetArcType(int nValue)
{
	switch(static_cast<EArcType>(nValue))
	{
		case EArcType::NORMAL:
		default:
			return EArcType::NORMAL;
		case EArcType::PIE: return EArcType::PIE;
		case EArcType::CHORD: return EArcType::CHORD;
	}
}

class CCtrlShapeEllipse : public CCtrlGeneralShape
{
	bool m_bIntervalDirty;
	bool m_bHasArcProperty;
	EArcType m_eArcType;
	int m_nCenterX;
	int m_nCenterY;
	int m_nAxixX1;
	int m_nAxixY1;
	int m_nAxixX2;
	int m_nAxixY2;
	int m_nSrartX1;
	int m_nSrartY1;
	int m_nEndX1;
	int m_nEndY1;
	int m_nSrartX2;
	int m_nSrartY2;
	int m_nEndX2;
	int m_nEndY2;
public:
	CCtrlShapeEllipse(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEELLIPSE_H
