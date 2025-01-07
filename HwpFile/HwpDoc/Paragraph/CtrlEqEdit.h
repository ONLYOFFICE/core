#ifndef CTRLEQEDIT_H
#define CTRLEQEDIT_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlEqEdit : public CCtrlGeneralShape
{
	int m_nAttr;
	HWP_STRING m_sEqn;
	int m_nCharSize;
	int m_nColor;
	int m_nBaseline;
	HWP_STRING m_sVersion;
	HWP_STRING m_sFont;
public:
	CCtrlEqEdit();
	CCtrlEqEdit(const HWP_STRING& sCtrlID);
	CCtrlEqEdit(const CCtrlGeneralShape& oShape);
	CCtrlEqEdit(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	HWP_STRING GetEqn() const;

	static int ParseElement(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLEQEDIT_H
