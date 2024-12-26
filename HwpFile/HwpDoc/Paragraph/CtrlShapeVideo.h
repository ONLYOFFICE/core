#ifndef CTRLSHAPEVIDEO_H
#define CTRLSHAPEVIDEO_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlShapeVideo : public CCtrlGeneralShape
{
	int m_nVideoType;
	short m_shVidoeBinID;
	HWP_STRING m_sWebURL;
	HWP_STRING m_sThumnailBinID;
public:
	CCtrlShapeVideo(const HWP_STRING& sCtrlID);
	CCtrlShapeVideo(const CCtrlGeneralShape& oShape);
	CCtrlShapeVideo(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	static int ParseElement(CCtrlShapeVideo& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeVideo& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEVIDEO_H
