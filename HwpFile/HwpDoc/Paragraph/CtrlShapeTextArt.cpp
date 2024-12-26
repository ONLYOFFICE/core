#include "CtrlShapeTextArt.h"

namespace HWP
{
CCtrlShapeTextArt::CCtrlShapeTextArt()
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeTextArt::CCtrlShapeTextArt(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlShapeTextArt::ParseElement(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	// TODO:: проверить

	// [HWP ambiguous] following 120bytes are unknown.
	// Document doesn't mention about this at all.

	oBuffer.Skip(nSize);
	return nSize;
}

int CCtrlShapeTextArt::ParseCtrl(CCtrlShapeTextArt& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}
}
