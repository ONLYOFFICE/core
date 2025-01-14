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

inline EArcType GetArcType(int nValue)
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
	int m_nStartX1;
	int m_nStartY1;
	int m_nEndX1;
	int m_nEndY1;
	int m_nStartX2;
	int m_nStartY2;
	int m_nEndX2;
	int m_nEndY2;
public:
	CCtrlShapeEllipse();
	CCtrlShapeEllipse(const HWP_STRING& sCtrlID);
	CCtrlShapeEllipse(const CCtrlGeneralShape& oShape);
	CCtrlShapeEllipse(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapeEllipse(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion);

	EShapeType GetShapeType() const override;

	static int ParseElement(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEELLIPSE_H
