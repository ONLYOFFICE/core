#ifndef CTRLSHAPELINE_H
#define CTRLSHAPELINE_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlShapeLine : public CCtrlGeneralShape
{
	int m_nStartX;
	int m_nStartY;
	int m_nEndX;
	int m_nEndY;
	short m_shAttr;
public:
	CCtrlShapeLine();
	CCtrlShapeLine(const HWP_STRING& sCtrlID);
	CCtrlShapeLine(const CCtrlGeneralShape& oShape);
	CCtrlShapeLine(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapeLine(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion);

	EShapeType GetShapeType() const override;

	static void ParseElement(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static void ParseCtrl(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPELINE_H
