#ifndef CTRLSHAPEVIDEO_H
#define CTRLSHAPEVIDEO_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlShapeVideo : public CCtrlGeneralShape
{
	int m_nVideoType;
	short m_shVideoBinID;
	HWP_STRING m_sWebURL;
	HWP_STRING m_sThumnailBinID;
public:
	CCtrlShapeVideo(const HWP_STRING& sCtrlID);
	CCtrlShapeVideo(const CCtrlGeneralShape& oShape);
	CCtrlShapeVideo(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapeVideo(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType);

	EShapeType GetShapeType() const override;

	int GetVideoType() const;
	short GetVedeoBinID() const;
	HWP_STRING GetWebUrl() const;
	HWP_STRING GetThumnailBinID() const;

	static int ParseElement(CCtrlShapeVideo& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapeVideo& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEVIDEO_H
