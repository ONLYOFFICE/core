#include "HWPRecordBorderFill.h"
#include <regex>

namespace HWP
{
EImageFillType GetImageFillType(int nType)
{
	switch (static_cast<EImageFillType>(nType))
	{
		case EImageFillType::TILE:
		case EImageFillType::TILE_HORZ_TOP:
		case EImageFillType::TILE_HORZ_BOTTOM:
		case EImageFillType::TILE_VERT_LEFT:
		case EImageFillType::TILE_VERT_RIGHT:
		case EImageFillType::TOTAL:
		case EImageFillType::CENTER:
		case EImageFillType::CENTER_TOP:
		case EImageFillType::CENTER_BOTTOM:
		case EImageFillType::LEFT_CENTER:
		case EImageFillType::LEFT_TOP:
		case EImageFillType::LEFT_BOTTOM:
		case EImageFillType::RIGHT_CENTER:
		case EImageFillType::RIGHT_TOP:
		case EImageFillType::RIGHT_BOTTOM:
		case EImageFillType::ZOOM:
			return static_cast<EImageFillType>(nType);
		default:
			return EImageFillType::NONE;
	}
}

EGradFillType GetGradFillType(int nType)
{
	switch(static_cast<EGradFillType>(nType))
	{
		case EGradFillType::LINEAR:
		case EGradFillType::RADIAL:
		case EGradFillType::CONICAL:
		case EGradFillType::SQUARE:
			return static_cast<EGradFillType>(nType);
		default:
			return EGradFillType::LINEAR;
	}
}

EColorFillPattern GetColorFillPattern(int nPattern)
{
	switch (static_cast<EColorFillPattern>(nPattern))
	{
		case EColorFillPattern::VERTICAL:
		case EColorFillPattern::HORIZONTAL:
		case EColorFillPattern::BACK_SLASH:
		case EColorFillPattern::SLASH:
		case EColorFillPattern::CROSS:
		case EColorFillPattern::CROSS_DIAGONAL:
			return static_cast<EColorFillPattern>(nPattern);
		default:
			return EColorFillPattern::NONE;
	}
}

void TBorder::ReadFromNode(XmlUtils::CXmlNode& oNode)
{
	m_eStyle = GetLineStyle2(oNode.GetAttributeInt(L"type"));

	HWP_STRING sColor = std::regex_replace(oNode.GetAttribute(L"color"), std::wregex(L"^#([0-9A-Fa-f]+)$"), L"$1");

	if (L"none" != sColor)
		m_nColor = std::stoi(sColor, 0, 16);

	HWP_STRING sWidthValue = oNode.GetAttribute(L"width");

	if (L"0.1" == sWidthValue || L"0.1 mm" == sWidthValue)
		m_chWidth = 0;
	else if (L"0.12" == sWidthValue || L"0.12 mm" == sWidthValue)
		m_chWidth = 1;
	else if (L"0.15" == sWidthValue || L"0.15 mm" == sWidthValue)
		m_chWidth = 2;
	else if (L"0.2" == sWidthValue || L"0.2 mm" == sWidthValue)
		m_chWidth = 3;
	else if (L"0.25" == sWidthValue || L"0.25 mm" == sWidthValue)
		m_chWidth = 4;
	else if (L"0.3" == sWidthValue || L"0.3 mm" == sWidthValue)
		m_chWidth = 5;
	else if (L"0.4" == sWidthValue || L"0.4 mm" == sWidthValue)
		m_chWidth = 6;
	else if (L"0.5" == sWidthValue || L"0.5 mm" == sWidthValue)
		m_chWidth = 7;
	else if (L"0.6" == sWidthValue || L"0.6 mm" == sWidthValue)
		m_chWidth = 8;
	else if (L"0.7" == sWidthValue || L"0.7 mm" == sWidthValue)
		m_chWidth = 9;
	else if (L"1.0" == sWidthValue || L"1.0 mm" == sWidthValue)
		m_chWidth = 10;
	else if (L"1.5" == sWidthValue || L"1.5 mm" == sWidthValue)
		m_chWidth = 11;
	else if (L"2.0" == sWidthValue || L"2.0 mm" == sWidthValue)
		m_chWidth = 12;
	else if (L"3.0" == sWidthValue || L"3.0 mm" == sWidthValue)
		m_chWidth = 13;
	else if (L"4.0" == sWidthValue || L"4.0 mm" == sWidthValue)
		m_chWidth = 14;
	else if (L"5.0" == sWidthValue || L"5.0 mm" == sWidthValue)
		m_chWidth = 15;
}

CFill::CFill()
{}

CFill::CFill(CHWPStream& oBuffer, int nOff, int nSize)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(m_nFillType);

	if (CHECK_FLAG(m_nFillType, 0x01))
	{
		oBuffer.ReadColor(m_nFaceColor);
		oBuffer.ReadColor(m_nHatchColor);

		int nPattern;
		oBuffer.ReadInt(nPattern);

		m_eHatchStyle = GetColorFillPattern(nPattern);
	}

	if (CHECK_FLAG(m_nFillType, 0x04))
	{
		HWP_BYTE chTypeNum;
		oBuffer.ReadByte(chTypeNum);
		m_eGradType = GetGradFillType((int)chTypeNum);

		oBuffer.ReadInt(m_nAngle);
		oBuffer.ReadInt(m_nCenterX);
		oBuffer.ReadInt(m_nCenterY);
		oBuffer.ReadInt(m_nStep);
		oBuffer.ReadInt(m_nColorNum);

		if (m_nColorNum > 0)
		{
			m_arColors.resize(m_nColorNum);

			if (m_nColorNum > 2)
			{
				oBuffer.Skip(4 * (m_nColorNum - 2));
			}

			for (int nIndex = 0; nIndex < m_nColorNum; ++nIndex)
				oBuffer.ReadColor(m_arColors[nIndex]);
		}
	}

	if (CHECK_FLAG(m_nFillType, 0x02))
	{
		HWP_BYTE nTypeNum;
		oBuffer.ReadByte(nTypeNum);
		m_eMode = GetImageFillType(nTypeNum);

		oBuffer.ReadByte(m_chBright);
		oBuffer.ReadByte(m_chContrast);
		oBuffer.ReadByte(m_chEffect);

		short shBinTemp;
		oBuffer.ReadShort(shBinTemp);

		m_sBinItemID = TO_HWP_STRING(shBinTemp);
	}

	int nMoreSize;
	oBuffer.ReadInt(nMoreSize);

	if (nMoreSize > 0)
	{
		oBuffer.ReadByte(m_chStepCenter);
		oBuffer.Skip(nMoreSize - 1);
	}

	if (m_nFillType > 0)
		oBuffer.ReadByte(m_chAlpha);

	if (CHECK_FLAG(m_nFillType, 0x02))
		oBuffer.ReadByte(m_chAlpha);

	m_nSize = oBuffer.GetDistanceToLastPos(true);
}

CFill::CFill(XmlUtils::CXmlNode& oNode)
{
	std::vector<XmlUtils::CXmlNode> arChilds;
	oNode.GetChilds(arChilds);

	for (XmlUtils::CXmlNode& oChild : arChilds)
	{
		if (L"hc:winBrush" == oChild.GetName())
		{
			ReadWinBrush(oChild);
			m_nFillType |= 0x01;
		}
		else if (L"hc:gradation" == oChild.GetName())
		{
			ReadGradation(oChild);
			m_nFillType |= 0x04;
		}
		else if (L"hc:imgBrush" == oChild.GetName())
		{
			ReadImgBrush(oChild);
			m_nFillType |= 0x02;
		}
	}
}

void CFill::ReadWinBrush(XmlUtils::CXmlNode& oNode)
{
	m_nFaceColor = 0xFFFFFFFF;

	HWP_STRING sColor = oNode.GetAttribute(L"faceColor");

	if (L"none" != sColor)
	{
		if (L'#' == sColor.front())
			sColor.erase(0, 1);

		m_nFaceColor = std::stoi(sColor, 0, 16);
	}

	m_nHatchColor = 0x000000;

	sColor = oNode.GetAttribute(L"hatchColor");

	if (L"none" != sColor)
	{
		if (L'#' == sColor.front())
			sColor.erase(0, 1);

		m_nHatchColor = std::stoi(sColor, 0, 16);
	}

	m_eHatchStyle = GetColorFillPattern(oNode.GetAttributeInt(L"hatchStyle", -1));
	m_chAlpha = (HWP_BYTE)oNode.GetAttributeInt(L"alpha", 255);
}

void CFill::ReadGradation(XmlUtils::CXmlNode& oNode)
{
	m_eGradType = GetGradFillType(oNode.GetAttributeInt(L"type"));
	m_nAngle = oNode.GetAttributeInt(L"angle");
	m_nCenterX = oNode.GetAttributeInt(L"centerX");
	m_nCenterY = oNode.GetAttributeInt(L"centerY");
	m_nStep = oNode.GetAttributeInt(L"step");
	m_nColorNum = oNode.GetAttributeInt(L"colorNum");
	m_chStepCenter = (HWP_BYTE)oNode.GetAttributeInt(L"stepCenter");
	m_chAlpha = (HWP_BYTE)oNode.GetAttributeInt(L"alpha", 255);

	std::vector<XmlUtils::CXmlNode> arChilds;

	oNode.GetNodes(L"Color", arChilds);

	m_arColors.resize(arChilds.size());

	for (unsigned int unIndex = 0; unIndex < arChilds.size(); ++unIndex)
		m_arColors[unIndex] = std::stoi(std::regex_replace(arChilds[unIndex].GetText(), std::wregex(L"\\D"), L""), 0, 16);
}

void CFill::ReadImgBrush(XmlUtils::CXmlNode& oNode)
{
	m_eMode = GetImageFillType(oNode.GetAttributeInt(L"mode", (int)EImageFillType::NONE));

	std::vector<XmlUtils::CXmlNode> arChilds;
	oNode.GetChilds(arChilds);

	for (XmlUtils::CXmlNode& oChild : arChilds)
	{
		if (L"hc:img" == oChild.GetName())
		{
			m_chBright = (HWP_BYTE)oChild.GetAttributeInt(L"bright");
			m_chContrast = (HWP_BYTE)oChild.GetAttributeInt(L"contrast");

			HWP_STRING sEffect = oChild.GetAttribute(L"effect");

			if (L"REAL_PIC" == sEffect)
				m_chEffect = 0;
			else if (L"GRAY_SCALE" == sEffect)
				m_chEffect = 1;
			else if (L"BLACK_WHITE" == sEffect)
				m_chEffect = 2;

			m_sBinItemID = oChild.GetAttribute(L"binaryItemIDRef");
			m_chAlpha = (HWP_BYTE)oNode.GetAttributeInt(L"alpha", 255);
		}
	}
}

int CFill::GetSize() const
{
	return m_nSize;
}

bool CFill::NoneFill() const
{
	return !ColorFill() && !GradFill() && !ImageFill();
}

bool CFill::ColorFill() const
{
	return CHECK_FLAG(m_nFillType, 0x01);
}

bool CFill::GradFill() const
{
	return CHECK_FLAG(m_nFillType, 0x04);
}

bool CFill::ImageFill() const
{
	return CHECK_FLAG(m_nFillType, 0x02);
}

int CFill::GetFaceColor() const
{
	return m_nFaceColor;
}

HWP_STRING CFill::GetBinItemID() const
{
	return m_sBinItemID;
}

CHWPRecordBorderFill::CHWPRecordBorderFill(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(nullptr), m_pFill(nullptr)
{}

CHWPRecordBorderFill::CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo), m_pFill(nullptr)
{
	short shTypeBits;
	oBuffer.ReadShort(shTypeBits);

	m_bThreeD = CHECK_FLAG(shTypeBits, 0x01);
	m_bShadow = CHECK_FLAG(shTypeBits, 0x02);
	m_chSlash = (HWP_BYTE)((shTypeBits >> 2) & 0x07);
	m_chBackSlash = (HWP_BYTE)((shTypeBits >> 5) & 0x07);
	m_chCrookedSlash = (HWP_BYTE)((shTypeBits >> 8) & 0x03);
	m_chCrookedBackSlash = (HWP_BYTE)((shTypeBits >> 10) & 0x01);
	m_bCounterSlash = CHECK_FLAG(shTypeBits, 0x800);
	m_bCounterBackSlash = CHECK_FLAG(shTypeBits, 0x1000);
	m_bBreakCellSeparateLine = CHECK_FLAG(shTypeBits, 0x2000);


	#define READ_SIDE(side) \
	oBuffer.ReadByte(chLineStyleNum); \
	side.m_eStyle = GetLineStyle2(chLineStyleNum); \
	oBuffer.ReadByte(side.m_chWidth);\
	oBuffer.ReadColor(side.m_nColor)

	HWP_BYTE chLineStyleNum;

	READ_SIDE(m_oLeft);
	READ_SIDE(m_oRight);
	READ_SIDE(m_oTop);
	READ_SIDE(m_oBottom);
	READ_SIDE(m_oDiagonal);

	m_pFill = new CFill(oBuffer, 0, 0); // TODO:: перейти от использования off и size
}

CHWPRecordBorderFill::CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, XmlUtils::CXmlNode& oNode, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_BORDER_FILL, 0, 0), m_pParent(&oDocInfo), m_pFill(nullptr)
{
	if (L"1" == oNode.GetAttribute(L"threeD"))
		m_bThreeD = true;
	else
		m_bThreeD = false;

	if (L"1" == oNode.GetAttribute(L"shadow"))
		m_bShadow = true;
	else
		m_bShadow = false;

	if (L"1" == oNode.GetAttribute(L"breakCellSeparateLine"))
		m_bBreakCellSeparateLine = true;
	else
		m_bBreakCellSeparateLine = false;

	std::vector<XmlUtils::CXmlNode> arChilds;
	oNode.GetChilds(arChilds);

	HWP_STRING sChildName;

	for (XmlUtils::CXmlNode& oChild : arChilds)
	{
		if (L"hh:slash" == oChild.GetName())
		{
			HWP_STRING sType = oChild.GetAttribute(L"type");

			if (L"NONE" == sType)
				m_chSlash = 0x0;
			else if (L"CENTER" == sType)
				m_chSlash = 0b010;
			else if (L"CENTER_BELOW" == sType)
				m_chSlash = 0b011;
			else if (L"CENTER_ABOVE" == sType)
				m_chSlash = 0b110;
			else if (L"ALL" == sType)
				m_chSlash = 0b111;

			if (L"1" == oChild.GetAttribute(L"Crooked"))
				m_chCrookedSlash = 1;
			else
				m_chCrookedSlash = 0;

			if (L"1" == oChild.GetAttribute(L"isCounter"))
				m_bCounterSlash = true;
			else
				m_bCounterSlash = false;
		}
		else if (L"hh:backSlash" == oChild.GetName())
		{
			HWP_STRING sType = oChild.GetAttribute(L"type");

			if (L"NONE" == sType)
				m_chBackSlash = 0x0;
			else if (L"CENTER" == sType)
				m_chBackSlash = 0b010;
			else if (L"CENTER_BELOW" == sType)
				m_chBackSlash = 0b011;
			else if (L"CENTER_ABOVE" == sType)
				m_chBackSlash = 0b110;
			else if (L"ALL" == sType)
				m_chBackSlash = 0b111;

			if (L"1" == oChild.GetAttribute(L"Crooked"))
				m_chCrookedBackSlash = 1;
			else
				m_chCrookedBackSlash = 0;

			if (L"1" == oChild.GetAttribute(L"isCounter"))
				m_bCounterBackSlash = true;
			else
				m_bCounterBackSlash = false;
		}
		else if (L"hh:leftBorder" == oChild.GetName())
			m_oLeft.ReadFromNode(oChild);
		else if (L"hh:rightBorder" == oChild.GetName())
			m_oRight.ReadFromNode(oChild);
		else if (L"hh:topBorder" == oChild.GetName())
			m_oTop.ReadFromNode(oChild);
		else if (L"hh:bottomBorder" == oChild.GetName())
			m_oBottom.ReadFromNode(oChild);
		else if (L"hh:diagonal" == oChild.GetName())
			m_oDiagonal.ReadFromNode(oChild);
		else if (L"hc:fillBrush" == oChild.GetName())
		{
			m_pFill = new CFill(oChild);
		}
	}
}

CHWPRecordBorderFill::~CHWPRecordBorderFill()
{
	if (nullptr != m_pFill)
		delete m_pFill;
}

TBorder CHWPRecordBorderFill::GetLeftBorder() const
{
	return m_oLeft;
}

TBorder CHWPRecordBorderFill::GetRightBorder() const
{
	return m_oRight;
}

TBorder CHWPRecordBorderFill::GetTopBorder() const
{
	return m_oTop;
}

TBorder CHWPRecordBorderFill::GetBottomBorder() const
{
	return m_oBottom;
}

const CFill* CHWPRecordBorderFill::GetFill() const
{
	return m_pFill;
}


}
