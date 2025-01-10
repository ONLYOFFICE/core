#ifndef CTRLSHAPETEXTART_H
#define CTRLSHAPETEXTART_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
class CCtrlShapeTextArt : public CCtrlGeneralShape
{
	HWP_STRING m_sText;
	TPoint m_oPt0;
	TPoint m_oPt1;
	TPoint m_oPt2;
	TPoint m_oPt3;

	HWP_STRING m_sFontName;
	HWP_STRING m_sFontStyle;
	HWP_STRING m_sFontType;
	HWP_STRING m_sTextShape;
	short m_shLineSpacing;
	short m_shSpacing;
	HWP_STRING m_sAlign;

	std::vector<TPoint> m_arOutline;
public:
	CCtrlShapeTextArt();
	CCtrlShapeTextArt(const HWP_STRING& sCtrlID);
	CCtrlShapeTextArt(const CCtrlGeneralShape& oShape);
	CCtrlShapeTextArt(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	EShapeType GetShapeType() const override;

	static int ParseElement(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPETEXTART_H
