#include "CtrlShapePic.h"

namespace HWP
{
EPicEffectType GetPicEffectType(int nValue)
{
	switch(static_cast<EPicEffectType>(nValue))
	{
		case EPicEffectType::SHADOW:
		case EPicEffectType::GLOW:
		case EPicEffectType::SOFT_EDGE:
		case EPicEffectType::REFLECT:
			return static_cast<EPicEffectType>(nValue);
		default:
			return EPicEffectType::NONE;
	}
}

CPicColor::CPicColor(CHWPStream& oBuffer, int nOff, int nSize)
{
	oBuffer.ReadInt(m_nType);
	oBuffer.ReadInt(m_nRGB);

	oBuffer.Skip(nSize - 8);
	m_nSize = nSize;
}

CPicEffect::CPicEffect(EPicEffectType eType)
	: m_eType(eType)
{}

CPicEffect::CPicEffect(int nType)
{
	m_eType = GetPicEffectType(nType);
}

int CPicEffect::GetSize()
{
	return m_nSize;
}

CShadow::CShadow(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize)
	: CPicEffect(nTypeNum), m_pColor(nullptr)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(m_nStyle);
	oBuffer.ReadInt(m_nTransparency);
	oBuffer.ReadInt(m_nBlur);
	oBuffer.ReadInt(m_nDirection);
	oBuffer.ReadInt(m_nDistance);
	oBuffer.ReadFloat(m_fAngleX);
	oBuffer.ReadFloat(m_fAngleY);
	oBuffer.ReadFloat(m_fMagnifyX);
	oBuffer.ReadFloat(m_fMagnifyY);
	oBuffer.ReadInt(m_nRotation);
	m_pColor = new CPicColor(oBuffer, nOff, nSize - oBuffer.GetDistanceToLastPos());

	m_nSize = oBuffer.GetDistanceToLastPos(true);
}

CShadow::~CShadow()
{
	if (nullptr != m_pColor)
		delete m_pColor;
}

CNeon::CNeon(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize)
	: CPicEffect(nTypeNum), m_pColor(nullptr)
{
	oBuffer.SavePosition();

	oBuffer.ReadFloat(m_fTransparency);
	oBuffer.ReadFloat(m_fRadius);
	m_pColor = new CPicColor(oBuffer, nOff, nSize - oBuffer.GetDistanceToLastPos());

	m_nSize = oBuffer.GetDistanceToLastPos(true);
}

CNeon::~CNeon()
{
	if (nullptr != m_pColor)
		delete m_pColor;
}

CSoftEdge::CSoftEdge(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize)
	: CPicEffect(nTypeNum)
{
	oBuffer.ReadFloat(m_fRadius);

	m_nSize = 4;
}

CReflect::CReflect(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize)
	: CPicEffect(nTypeNum)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(m_nStyle);
	oBuffer.ReadFloat(m_fRadius);
	oBuffer.ReadFloat(m_fDirection);
	oBuffer.ReadFloat(m_fDistance);
	oBuffer.ReadFloat(m_fAngleX);
	oBuffer.ReadFloat(m_fAngleY);
	oBuffer.ReadFloat(m_fMagnifyX);
	oBuffer.ReadFloat(m_fMagnifyY);
	oBuffer.ReadInt(m_nRotateStyle);
	oBuffer.ReadFloat(m_fStartTrans);
	oBuffer.ReadFloat(m_fStartPos);
	oBuffer.ReadFloat(m_fEndTrans);
	oBuffer.ReadFloat(m_fEndPos);
	oBuffer.ReadFloat(m_fOffsetDirection);

	m_nSize = oBuffer.GetDistanceToLastPos(true);
}

CCtrlShapePic::CCtrlShapePic()
	: CCtrlGeneralShape()
{}

CCtrlShapePic::CCtrlShapePic(const STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapePic::CCtrlShapePic(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapePic::~CCtrlShapePic()
{
	for (CPicEffect* pEffect : m_arPicEffect)
	{
		if (nullptr != pEffect)
			delete pEffect;
	}
}

int CCtrlShapePic::ParseElement(CCtrlShapePic& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadColor(oObj.m_nBorderColor);
	oBuffer.ReadInt(oObj.m_nBorderThick);
	oBuffer.ReadInt(oObj.m_nBorderAttr);

	for (unsigned int unIndex = 0; unIndex < 4; ++unIndex)
	{
		oBuffer.ReadInt(oObj.m_arBorderPoints[unIndex].m_nX);
		oBuffer.ReadInt(oObj.m_arBorderPoints[unIndex].m_nY);
	}

	oBuffer.ReadInt(oObj.m_nCropLeft);
	oBuffer.ReadInt(oObj.m_nCropTop);
	oBuffer.ReadInt(oObj.m_nCropRight);
	oBuffer.ReadInt(oObj.m_nCropBottom);

	for (unsigned int unIndex = 0; unIndex < 4; ++unIndex)
		oBuffer.ReadShort(oObj.m_arInnerSpaces[unIndex]);

	oBuffer.ReadByte(oObj.m_chBright);
	oBuffer.ReadByte(oObj.m_chContrast);
	oBuffer.ReadByte(oObj.m_chEffect);

	short shBinItemID;
	oBuffer.ReadShort(shBinItemID);
	oObj.m_sBinDataID = std::to_wstring(shBinItemID - 1);

	oBuffer.ReadByte(oObj.m_chBorderAlpha);

	#define CAN_READ() \
	if (oBuffer.GetDistanceToLastPos() >= nSize) \
	{ \
		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true)); \
		return nSize; \
	}

	CAN_READ();

	oBuffer.ReadInt(oObj.m_nInstanceID);

	CAN_READ();

	oBuffer.ReadInt(oObj.m_nPicEffectInfo);

	#define ADD_EFFECT(flag, effect, size) \
	if (CHECK_FLAG(oObj.m_nPicEffectInfo, flag)) \
	{ \
		CPicEffect* pEffect = new effect(oObj.m_nPicEffectInfo, oBuffer, nOff, size); \
		if (nullptr != pEffect) \
			oObj.m_arPicEffect.push_back(pEffect); \
	}

	if (oObj.m_nPicEffectInfo && oBuffer.GetDistanceToLastPos() < nSize)
	{
		ADD_EFFECT(0x1, CShadow,   56)
		ADD_EFFECT(0x2, CNeon,     28)
		ADD_EFFECT(0x4, CSoftEdge, 4 )
		ADD_EFFECT(0x8, CReflect,  56)
	}

	CAN_READ();

	oBuffer.ReadInt(oObj.m_nIniPicWidth);
	oBuffer.ReadInt(oObj.m_nIniPicHeight);

	if (nSize - oBuffer.GetDistanceToLastPos() >= 1)
		oBuffer.ReadByte(oObj.m_chPicAlpha);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return nSize;
}

int CCtrlShapePic::ParseCtrl(CCtrlShapePic& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

}
