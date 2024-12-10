#ifndef CTRLSHAPETEXTART_H
#define CTRLSHAPETEXTART_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
class CCtrlShapeTextArt : public CCtrlGeneralShape
{
	STRING m_sText;
	TPoint m_oPt0;
	TPoint m_oPt1;
	TPoint m_oPt2;
	TPoint m_oPt3;

	STRING m_sFontName;
	STRING m_sFontStyle;
	STRING m_sFontType;
	STRING m_sTextShape;
	short m_shLineSpacing;
	short m_shSpacing;
	STRING m_sAlign;

	std::vector<TPoint> m_arOutline;
public:
	CCtrlShapeTextArt(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPETEXTART_H
