#ifndef CHARSHAPE_H
#define CHARSHAPE_H

#include <list>
#include "../HWPStream.h"
#include "Ctrl.h"

namespace HWP
{
class CCharShape
{
	int m_nStart;
	int m_nCharShapeID;
public:
	CCharShape();
	CCharShape(int nStart, int nID);

	static std::list<CCharShape*> Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int FillCharShape(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion, const VECTOR<CCtrl*>& arParas);
};
}

#endif // CHARSHAPE_H
