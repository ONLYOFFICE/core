#ifndef CTRLGENERALSHAPE_H
#define CTRLGENERALSHAPE_H

#include "../HWPElements/HWPRecordBorderFill.h"
#include "CtrlObjElement.h"
#include "HWPPargraph.h"

namespace HWP
{
enum class EShapeType
{
	GeneralShape,
	Arc,
	ConnectLine,
	Curve,
	Ellipse,
	Line,
	Ole,
	Pic,
	Polygon,
	Rect,
	TextArt,
	Video,
	EqEdit,
	Container
};

class CCtrlGeneralShape : public CCtrlObjElement
{
	CHWPPargraph* m_pParent;

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

	void InitData();

	friend class CCtrlShapeEllipse;
	friend class CCtrlShapePolygon;
	friend class CCtrlShapeRect;

	void ReadSubList(CXMLReader& oReader);
public:
	CCtrlGeneralShape();
	CCtrlGeneralShape(const HWP_STRING& sCtrlID);
	CCtrlGeneralShape(const CCtrlGeneralShape& oGeneralShape);
	CCtrlGeneralShape(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlGeneralShape(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);
	virtual ~CCtrlGeneralShape();

	ECtrlObjectType GetCtrlType() const override;
	virtual EShapeType GetShapeType() const;

	void ParseChildren(CXMLReader& oReader, EHanType eType);

	void SetParent(CHWPPargraph* pParent);
	CHWPPargraph* GetParent();

	const CFill* GetFill() const;

	ELineStyle2 GetLineStyle() const;
	int GetLineColor() const;
	int GetLineThick() const;

	ELineArrowStyle GetLineHeadStyle() const;
	ELineArrowSize  GetLineHeadSize () const;
	ELineArrowStyle GetLineTailStyle() const;
	ELineArrowSize  GetLineTailSize () const;

	static CCtrlGeneralShape* Parse(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderApend(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLGENERALSHAPE_H
