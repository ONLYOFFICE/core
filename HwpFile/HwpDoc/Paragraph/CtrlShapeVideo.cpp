#include "CtrlShapeVideo.h"

namespace HWP
{
CCtrlShapeVideo::CCtrlShapeVideo(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeVideo::CCtrlShapeVideo(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeVideo::CCtrlShapeVideo(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeVideo::CCtrlShapeVideo(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID, oNode, nVersion)
{
	HWP_STRING sType = oNode.GetAttribute(L"videotype");

	if (L"Local" == sType)
		m_nVideoType = 0;
	else if (L"Web" == sType)
		m_nVideoType = 1;

	m_shVideoBinID = oNode.GetAttributeInt(L"fileIDRef");
	m_sThumnailBinID = oNode.GetAttribute(L"imageIDRef");

	if (1 == m_nVideoType)
		m_sWebURL = oNode.GetAttribute(L"tag");
}

EShapeType CCtrlShapeVideo::GetShapeType() const
{
	return EShapeType::Video;
}

int CCtrlShapeVideo::GetVideoType() const
{
	return m_nVideoType;
}

short CCtrlShapeVideo::GetVedeoBinID() const
{
	return m_shVideoBinID;
}

HWP_STRING CCtrlShapeVideo::GetWebUrl() const
{
	return m_sWebURL;
}

HWP_STRING CCtrlShapeVideo::GetThumnailBinID() const
{
	return m_sThumnailBinID;
}

int CCtrlShapeVideo::ParseElement(CCtrlShapeVideo& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(oObj.m_nVideoType);

	if (0 == oObj.m_nVideoType)
		oBuffer.ReadShort(oObj.m_shVideoBinID);
	else if (1 == oObj.m_nVideoType)
		oBuffer.ReadString(oObj.m_sWebURL, EStringCharacter::UTF16);

	short m_sBinID;
	oBuffer.ReadShort(m_sBinID);
	oObj.m_sThumnailBinID = std::to_wstring(m_sBinID);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}

int CCtrlShapeVideo::ParseCtrl(CCtrlShapeVideo& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}
}
