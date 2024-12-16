#ifndef CTRLEQEDIT_H
#define CTRLEQEDIT_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlEqEdit : public CCtrlGeneralShape
{
	int m_nAttr;
	STRING m_sEqn;
	int m_nCharSize;
	int m_nColor;
	int m_nBaseline;
	STRING m_sVersion;
	STRING m_sFont;
public:
	CCtrlEqEdit();
	CCtrlEqEdit(const STRING& sCtrlID);
	CCtrlEqEdit(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLEQEDIT_H
