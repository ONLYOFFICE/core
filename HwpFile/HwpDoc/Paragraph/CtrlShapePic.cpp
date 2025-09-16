#include "CtrlShapePic.h"

#include "../Common/NodeNames.h"

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

CPicColor::CPicColor(CXMLReader& oReader)
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

CShadow::CShadow(CXMLReader& oReader, EHanType eType)
	: CPicEffect(EPicEffectType::SHADOW), m_pColor(nullptr)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Style, eType) == sAttributeName)
			m_nStyle = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Alpha, eType) == sAttributeName)
			m_nTransparency = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Radius, eType) == sAttributeName)
			m_nBlur = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Direction, eType) == sAttributeName)
			m_nDirection = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Distance, eType) == sAttributeName)
			m_nDistance = oReader.GetInt();
		else if (GetAttributeName(EAttribute::RotationStyle, eType) == sAttributeName)
			m_nRotation = (int)oReader.GetBool();

		// Далее идут атрибуты, которые встречаются только в HWPML
		else if (EHanType::HWPML != eType)
			continue;
		else if ("SkewX" == sAttributeName)
			m_fAngleX = oReader.GetDouble();
		else if ("SkewY" == sAttributeName)
			m_fAngleY = oReader.GetDouble();
		else if ("ScaleX" == sAttributeName)
			m_fMagnifyX = oReader.GetDouble();
		else if ("ScaleY" == sAttributeName)
			m_fMagnifyY = oReader.GetDouble();
	}
	END_READ_ATTRIBUTES(oReader)

	// Далее идут ноды, которые встречаются только в HWPX
	if (EHanType::HWPX != eType)
		return;

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:skew" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_fAngleX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_fAngleY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:scale" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_fMagnifyX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_fMagnifyY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		// TODO:: реализовать для HWPX и HWPML при встрече
		else if ("hp:effectsColor" == sNodeName)
			m_pColor = new CPicColor(oReader);
	}
	END_WHILE
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

CNeon::CNeon(CXMLReader& oReader, EHanType eType)
	: CPicEffect(EPicEffectType::GLOW), m_pColor(nullptr)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Alpha, eType) == sAttributeName)
			m_fTransparency = oReader.GetDouble();
		else if (GetAttributeName(EAttribute::Radius, eType) == sAttributeName)
			m_fRadius = oReader.GetDouble();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::EffectsColor, eType))
	{
		m_pColor = new CPicColor(oReader);
		break;
	}
	END_WHILE
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

CSoftEdge::CSoftEdge(CXMLReader& oReader, EHanType eType)
	: CPicEffect(EPicEffectType::SOFT_EDGE)
{
	m_fRadius = oReader.GetAttributeDouble(GetAttributeName(EAttribute::Radius, eType));
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

CReflect::CReflect(CXMLReader& oReader, EHanType eType)
	: CPicEffect(EPicEffectType::REFLECT)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Radius, eType) == sAttributeName)
			m_fRadius = oReader.GetDouble();
		else if (GetAttributeName(EAttribute::Direction, eType) == sAttributeName)
			m_fDirection = oReader.GetDouble();
		else if (GetAttributeName(EAttribute::Distance, eType) == sAttributeName)
			m_fDistance = oReader.GetDouble();
		else if (GetAttributeName(EAttribute::RotationStyle, eType) == sAttributeName)
			m_nRotateStyle = (int)oReader.GetBool();
		else if (GetAttributeName(EAttribute::FadeDirection, eType) == sAttributeName)
			m_fOffsetDirection = oReader.GetDouble();

		// Далее идут атрибуты, которые встречаются только в HWPML
		else if (EHanType::HWPML != eType)
			continue;
		else if ("SkewX" == sAttributeName)
			m_fAngleX = oReader.GetDouble();
		else if ("SkewY" == sAttributeName)
			m_fAngleY = oReader.GetDouble();
		else if ("ScaleX" == sAttributeName)
			m_fMagnifyX = oReader.GetDouble();
		else if ("ScaleY" == sAttributeName)
			m_fMagnifyY = oReader.GetDouble();
		else if ("StartAlpha" == sAttributeName)
			m_fStartTrans = oReader.GetDouble();
		else if ("StartPos" == sAttributeName)
			m_fStartPos = oReader.GetDouble();
		else if ("EndAlpha" == sAttributeName)
			m_fEndTrans = oReader.GetDouble();
		else if ("EndPos" == sAttributeName)
			m_fEndPos = oReader.GetDouble();
	}
	END_READ_ATTRIBUTES(oReader)

	// Далее идут ноды, которые встречаются только в HWPX
	if (EHanType::HWPX != eType)
		return;

	#define READ_VALUES(value_1_name, value_1_variable, value_2_name, value_2_variable)\
	{\
		START_READ_ATTRIBUTES(oReader)\
		{\
			if (value_1_name == sAttributeName)\
				value_1_variable = oReader.GetDouble();\
			else if (value_2_name == sAttributeName)\
				value_2_variable = oReader.GetDouble();\
		}\
		END_READ_ATTRIBUTES(oReader)\
	}

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:skew" == sNodeName)
			READ_VALUES("x", m_fAngleX, "y", m_fAngleY)
		else if ("hp:scale" == sNodeName)
			READ_VALUES("x", m_fMagnifyX, "y", m_fMagnifyY)
		else if ("hp:alpha" == sNodeName)
			READ_VALUES("start", m_fStartTrans, "end", m_fEndTrans)
		else if ("hp:pos" == sNodeName)
			READ_VALUES("start", m_fStartPos, "end", m_fEndPos)
	}
	END_WHILE
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

CCtrlShapePic::CCtrlShapePic(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, nVersion, eType)
{
	switch (eType)
	{
		case EHanType::HWPX : ReadFromHWPX (oReader, nVersion); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
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

void CCtrlShapePic::ReadFromHWPX(CXMLReader &oReader, int nVersion)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:imgRect" == sNodeName)
		{
			#define READ_POINT(index_point)\
			{\
				START_READ_ATTRIBUTES(oReader)\
				{\
					if ("x" == sAttributeName)\
						m_arBorderPoints[index_point].m_nX = oReader.GetInt();\
					else if ("y" == sAttributeName)\
						m_arBorderPoints[index_point].m_nY = oReader.GetInt();\
				}\
				END_READ_ATTRIBUTES(oReader)\
			}

			WHILE_READ_NEXT_NODE_WITH_DEPTH_AND_NAME(oReader, Child)
			{
				if ("hc:pt0" == sNodeChildName)
					READ_POINT(0)
				else if ("hc:pt1" == sNodeChildName)
					READ_POINT(1)
				else if ("hc:pt2" == sNodeChildName)
					READ_POINT(2)
				else if ("hc:pt3" == sNodeChildName)
					READ_POINT(3)
			}
			END_WHILE
		}
		else if ("hp:imgClip" == sNodeName)
			ReadImageClip(oReader, EHanType::HWPX);
		else if ("hp:effects" == sNodeName)
			ReadEffects(oReader, EHanType::HWPX);
		else if ("hc:img" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("bright" == sAttributeName)
					m_chBright = (HWP_BYTE)oReader.GetInt();
				else if ("contrast" == sAttributeName)
					m_chContrast = (HWP_BYTE)oReader.GetInt();
				else if ("effect" == sAttributeName)
				{
					const std::string sType{oReader.GetTextA()};

					if ("REAL_PIC" == sType)
						m_chEffect = 0;
					else if ("GRAY_SCALE" == sType)
						m_chEffect = 1;
					else if ("BLACK_WHITE" == sType)
						m_chEffect = 2;
				}
				else if ("binaryItemIDRef" == sAttributeName)
					m_sBinDataID = oReader.GetText();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:imgDim" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("dimwidth" == sAttributeName)
					m_nIniPicWidth = oReader.GetInt();
				else if ("dimheight" == sAttributeName)
					m_nIniPicHeight = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, nVersion, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapePic::ReadFromHWPML(CXMLReader &oReader)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("IMAGERECT" == sNodeName)
		{
			#define READ_POINT(point, index) (STR(##point##index) == sAttributeName) m_arBorderPoints[index].m_n##point = oReader.GetInt()

			START_READ_ATTRIBUTES(oReader)
			{
				     if READ_POINT(X, 0);
				else if READ_POINT(Y, 0);
				else if READ_POINT(X, 1);
				else if READ_POINT(Y, 1);
				else if READ_POINT(X, 2);
				else if READ_POINT(Y, 2);
				else if READ_POINT(X, 3);
				else if READ_POINT(Y, 3);
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("IMAGECLIP" == sNodeName)
			ReadImageClip(oReader, EHanType::HWPML);
		else if ("EFFECTS" == sNodeName)
			ReadEffects(oReader, EHanType::HWPML);
	}
	END_WHILE
}

void CCtrlShapePic::ReadImageClip(CXMLReader &oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Left, eType) == sAttributeName)
			m_nCropLeft = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Right, eType) == sAttributeName)
			m_nCropRight = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Top, eType) == sAttributeName)
			m_nCropTop = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Bottom, eType) == sAttributeName)
			m_nCropBottom = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)
}

void CCtrlShapePic::ReadEffects(CXMLReader &oReader, EHanType eType)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if (GetNodeName(ENode::ShadowEffect, eType) == sNodeName)
			m_arPicEffect.push_back(new CShadow(oReader, eType));
		else if (GetNodeName(ENode::GlowEffect, eType) == sNodeName)
			m_arPicEffect.push_back(new CNeon(oReader, eType));
		else if (GetNodeName(ENode::SoftEdgeEffect, eType) == sNodeName)
			m_arPicEffect.push_back(new CSoftEdge(oReader, eType));
		else if (GetNodeName(ENode::ReflectionEffect, eType) == sNodeName)
			m_arPicEffect.push_back(new CReflect(oReader, eType));
	}
	END_WHILE
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
