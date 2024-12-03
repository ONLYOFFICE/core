#include "CharShape.h"

namespace HWP
{
CCharShape::CCharShape()
{
}

CCharShape::CCharShape(int nStart, int nID)
	: m_nStart(nStart), m_nCharShapeID(nID)
{
}

std::list<CCharShape*> CCharShape::Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	BYTE *pOldCurentPos = oBuffer.GetCurPtr();

	#define CHECK_SIZE (nSize - 8 >= (oBuffer.GetCurPtr() - pOldCurentPos))

	std::list<CCharShape*> arCharShapes;

	int nStart, nCharShapeID;

	while (CHECK_SIZE)
	{
		oBuffer.ReadInt(nStart);
		oBuffer.ReadInt(nCharShapeID);

		arCharShapes.push_back(new CCharShape(nStart, nCharShapeID));
	}

	return arCharShapes;
}

int CCharShape::FillChasrShape(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion, const std::list<CCtrl*>& arParas)
{
	if (arParas.empty())
		return 0;

	// std::list<CCharShape*> arCharShape = Parse(nTagNum, nLevel, nSize, oBuffer, nOff, nVersion);

	//TODO:: перенести реализацию

	return 0;
}
}
