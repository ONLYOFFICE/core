#ifndef CTRLGENERALSHAPE_H
#define CTRLGENERALSHAPE_H

#include "../HWPElements/HWPRecordBorderFill.h"
#include "CtrlObjElement.h"
#include "HWPPargraph.h"

namespace HWP
{
class CCtrlGeneralShape : public CCtrlObjElement
{
	CHWPPargraph* m_pParent;
	int m_nSize;

	int m_nLineColor;
	int m_nLineThick;
	ELineArrowStyle m_eLineHead;
	ELineArrowStyle m_eLineTail;
	ELineArrowSize m_eLineHeadSz;
	ELineArrowSize m_eLineTailSz;
	ELineStyle2 m_eLineStyle;
	HWP_BYTE m_chOutline;

	int m_nFillType;
	CFill* m_pFill;

	short m_shLeftSpace;
	short m_shRightSpace;
	short m_shTopSpace;
	short m_shBottomSpace;
	int m_nMaxTxtWidth;

	friend class CCtrlShapeEllipse;
	friend class CCtrlShapePolygon;
	friend class CCtrlShapeRect;
public:
	CCtrlGeneralShape();
	CCtrlGeneralShape(const HWP_STRING& sCtrlID);
	CCtrlGeneralShape(const CCtrlGeneralShape& oGeneralShape);
	CCtrlGeneralShape(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	virtual ~CCtrlGeneralShape();

	void SetParent(CHWPPargraph* pParent);
	CHWPPargraph* GetParent();

	int GetSize() override;

	const CFill* GetFill() const;

	ELineStyle2 GetLineStyle() const;
	int GetLineColor() const;
	int GetLineThick() const;

	static CCtrlGeneralShape* Parse(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderApend(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLGENERALSHAPE_H
