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

CCtrlShapeVideo::CCtrlShapeVideo(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, nVersion, eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("type" == sAttributeName)
		{
			const std::string sType{oReader.GetTextA()};

			if ("Local" == sType)
				m_nVideoType = 0;
			else if ("Web" == sType)
				m_nVideoType = 1;
		}
		else if ("fileIDRef" == sAttributeName)
			m_shVideoBinID = oReader.GetInt();
		else if ("imageIDRef" == sAttributeName)
			m_sThumnailBinID = oReader.GetInt();
		else if ("tag" == sAttributeName)
			m_sWebURL = oReader.GetText();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
		CCtrlGeneralShape::ParseChildren(oReader, nVersion, eType);
	END_WHILE
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
