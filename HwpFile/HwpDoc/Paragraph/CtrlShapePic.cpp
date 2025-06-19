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

CPicColor::CPicColor(CXMLNode& oNode)
{
	//TODO:: проверить
	m_nType = 0;
}

CPicEffect::CPicEffect(EPicEffectType eType)
	: m_eType(eType), m_nSize(0)
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

CShadow::CShadow(CXMLNode& oNode, int nVersion)
	: CPicEffect(EPicEffectType::SHADOW), m_pColor(nullptr)
{
	m_nStyle = oNode.GetAttributeInt(L"style");
	m_nTransparency = oNode.GetAttributeInt(L"alpha");
	m_nBlur = oNode.GetAttributeInt(L"radius");
	m_nDirection = oNode.GetAttributeInt(L"direction");
	m_nDistance = oNode.GetAttributeInt(L"distance");
	m_nRotation = (int)oNode.GetAttributeBool(L"rotationStyle");

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:skew" == oChild.GetName())
		{
			m_fAngleX = oChild.GetAttributeDouble(L"x");
			m_fAngleY = oChild.GetAttributeDouble(L"y");
		}
		else if (L"hp:scale" == oChild.GetName())
		{
			m_fMagnifyX = oChild.GetAttributeDouble(L"x");
			m_fMagnifyY = oChild.GetAttributeDouble(L"y");
		}
		else if (L"hp:effectsColor" == oChild.GetName())
		{
			m_pColor = new CPicColor(oChild);
		}
	}
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

CNeon::CNeon(CXMLNode& oNode, int nVersion)
	: CPicEffect(EPicEffectType::GLOW), m_pColor(nullptr)
{
	m_fTransparency = oNode.GetAttributeDouble(L"alpha");
	m_fRadius = oNode.GetAttributeDouble(L"radius");

	CXMLNode oChild{oNode.GetChild(L"hp:effectsColor")};
	m_pColor = new CPicColor(oChild);
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

CSoftEdge::CSoftEdge(CXMLNode& oNode, int nVersion)
	: CPicEffect(EPicEffectType::SOFT_EDGE)
{
	m_fRadius = oNode.GetAttributeDouble(L"radius");
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

CReflect::CReflect(CXMLNode& oNode, int nVersion)
	: CPicEffect(EPicEffectType::REFLECT)
{
	m_fRadius = oNode.GetAttributeDouble(L"radius");
	m_fDirection = oNode.GetAttributeInt(L"direction");
	m_fDistance = oNode.GetAttributeInt(L"distance");
	m_nRotateStyle = (int)oNode.GetAttributeBool(L"rotationStyle");
	m_fOffsetDirection = oNode.GetAttributeInt(L"fadeDirection");

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:skew" == oChild.GetName())
		{
			m_fAngleX = oChild.GetAttributeDouble(L"x");
			m_fAngleY = oChild.GetAttributeDouble(L"y");
		}
		else if (L"hp:scale" == oChild.GetName())
		{
			m_fMagnifyX = oChild.GetAttributeDouble(L"x");
			m_fMagnifyY = oChild.GetAttributeDouble(L"y");
		}
		else if (L"hp:alpha" == oChild.GetName())
		{
			m_fStartTrans = oChild.GetAttributeDouble(L"start");
			m_fEndTrans = oChild.GetAttributeDouble(L"end");
		}
		else if (L"hp:pos" == oChild.GetName())
		{
			m_fStartPos = oChild.GetAttributeDouble(L"start");
			m_fEndPos = oChild.GetAttributeDouble(L"end");
		}
	}
}

CCtrlShapePic::CCtrlShapePic()
	: CCtrlGeneralShape()
{}

CCtrlShapePic::CCtrlShapePic(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapePic::CCtrlShapePic(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapePic::CCtrlShapePic(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapePic::CCtrlShapePic(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID, oNode, nVersion)
{
	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:imgRect" == oChild.GetName())
		{
			for (CXMLNode& oGrandChild : oChild.GetChilds())
			{
				for (unsigned int unIndex = 0; unIndex < 4; ++unIndex)
				{
					if ((L"hc:pt" + std::to_wstring(unIndex)) == oGrandChild.GetName())
					{
						m_arBorderPoints[unIndex].m_nX = oGrandChild.GetAttributeInt(L"x");
						m_arBorderPoints[unIndex].m_nY = oGrandChild.GetAttributeInt(L"y");
						break;
					}
				}
			}
		}
		else if (L"hp:imgClip" == oChild.GetName())
		{
			m_nCropLeft = oChild.GetAttributeInt(L"left");
			m_nCropRight = oChild.GetAttributeInt(L"right");
			m_nCropTop = oChild.GetAttributeInt(L"top");
			m_nCropBottom = oChild.GetAttributeInt(L"bottom");
		}
		else if (L"hp:effects" == oChild.GetName())
		{
			for (CXMLNode& oGrandChild : oChild.GetChilds())
			{
				if (L"hp:shadow" == oGrandChild.GetName())
					m_arPicEffect.push_back(new CShadow(oGrandChild, nVersion));
				else if (L"hp:glow" == oGrandChild.GetName())
					m_arPicEffect.push_back(new CNeon(oGrandChild, nVersion));
				else if (L"hp:softEdge" == oGrandChild.GetName())
					m_arPicEffect.push_back(new CSoftEdge(oGrandChild, nVersion));
				else if (L"hp:reflection" == oGrandChild.GetName())
					m_arPicEffect.push_back(new CReflect(oGrandChild, nVersion));
			}
		}
		else if (L"hc:img" == oChild.GetName())
		{
			m_chBright = (HWP_BYTE)oChild.GetAttributeInt(L"bright");
			m_chContrast = (HWP_BYTE)oChild.GetAttributeInt(L"contrast");

			HWP_STRING sType = oChild.GetAttribute(L"effect");

			if (L"REAL_PIC" == sType)
				m_chEffect = 0;
			else if (L"GRAY_SCALE" == sType)
				m_chEffect = 1;
			else if (L"BLACK_WHITE" == sType)
				m_chEffect = 2;

			m_sBinDataID = oChild.GetAttribute(L"binaryItemIDRef");
		}
		else if (L"hp:imgDim" == oChild.GetName())
		{
			m_nIniPicWidth = oChild.GetAttributeInt(L"dimwidth");
			m_nIniPicHeight = oChild.GetAttributeInt(L"dimheight");
		}
	}
}

CCtrlShapePic::~CCtrlShapePic()
{
	for (CPicEffect* pEffect : m_arPicEffect)
	{
		if (nullptr != pEffect)
			delete pEffect;
	}
}

EShapeType CCtrlShapePic::GetShapeType() const
{
	return EShapeType::Pic;
}

HWP_STRING CCtrlShapePic::GetBinDataID() const
{
	return m_sBinDataID;
}

int CCtrlShapePic::GetPicWidth() const
{
	return m_nIniPicWidth;
}

int CCtrlShapePic::GetPicHeight() const
{
	return m_nIniPicHeight;
}

ELineStyle2 CCtrlShapePic::GetBorderLineStyle() const
{
	return ::HWP::GetLineStyle2((m_nBorderAttr & ((1 << 5) - 1)));
}

HWP_BYTE CCtrlShapePic::GetBorderCompoundLineType() const
{
	return (HWP_BYTE)((m_nBorderAttr >> 6) & ((1 << 3) - 1));
}

int CCtrlShapePic::GetBorderColor() const
{
	return m_nBorderColor;
}

int CCtrlShapePic::GetBorderThick() const
{
	return m_nBorderThick;
}

int CCtrlShapePic::GetImageRectWidth() const
{
	return m_arBorderPoints[1].m_nX - m_arBorderPoints[0].m_nX;
}

int CCtrlShapePic::GetIMageRectHeight() const
{
	return m_arBorderPoints[2].m_nY - m_arBorderPoints[0].m_nY;
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
	oObj.m_sBinDataID = std::to_wstring(shBinItemID);

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
