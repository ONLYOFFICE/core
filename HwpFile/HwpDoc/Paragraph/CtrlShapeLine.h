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
	CCtrlShapeLine(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPELINE_H
