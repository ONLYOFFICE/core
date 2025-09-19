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

EArcType GetArcType(int nValue);
EArcType GetArcType(const std::string &sValue, EHanType eType);

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

	void ReadFromHWPX(CXMLReader& oReader, int nVersion);
	void ReadFromHWPML(CXMLReader& oReader);
public:
	CCtrlShapeEllipse();
	CCtrlShapeEllipse(const HWP_STRING& sCtrlID);
	CCtrlShapeEllipse(const CCtrlGeneralShape& oShape);
	CCtrlShapeEllipse(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapeEllipse(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType);

	EShapeType GetShapeType() const override;

	static int ParseElement(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEELLIPSE_H
