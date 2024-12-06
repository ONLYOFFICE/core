#ifndef CTRLSHAPETEXTART_H
#define CTRLSHAPETEXTART_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
class CCtrlShapeTextArt : public CCtrlGeneralShape
{
	std::string m_sText;
	TPoint m_oPt0;
	TPoint m_oPt1;
	TPoint m_oPt2;
	TPoint m_oPt3;

	std::string m_sFontName;
	std::string m_sFontStyle;
	std::string m_sFontType;
	std::string m_sTextShape;
	short m_shLineSpacing;
	short m_shSpacing;
	std::string m_sAlign;

	std::vector<TPoint> m_arOutline;
public:
	CCtrlShapeTextArt(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPETEXTART_H
