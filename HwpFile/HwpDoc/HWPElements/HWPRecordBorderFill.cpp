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

void TBorder::ReadFromNode(CXMLNode& oNode)
{
	m_eStyle = GetLineStyle2(oNode.GetAttributeInt(L"type"));

	HWP_STRING sColor = std::regex_replace(oNode.GetAttribute(L"color"), std::wregex(L"^#([0-9A-Fa-f]+)$"), L"$1");

	if (L"none" != sColor)
		m_nColor = std::stoi(sColor, 0, 16);

	m_chWidth = (HWP_BYTE)ConvertWidthToHWP(oNode.GetAttribute(L"width"));
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

CFill::CFill(CXMLNode& oNode)
{
	for (CXMLNode& oChild : oNode.GetChilds())
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

void CFill::ReadWinBrush(CXMLNode& oNode)
{
	m_nFaceColor = oNode.GetAttributeColor(L"faceColor", 0xFFFFFFFF);
	m_nHatchColor = oNode.GetAttributeColor(L"hatchColor", 0x000000);
	m_eHatchStyle = GetColorFillPattern(oNode.GetAttributeInt(L"hatchStyle", -1));
	m_chAlpha = (HWP_BYTE)oNode.GetAttributeInt(L"alpha", 255);
}

void CFill::ReadGradation(CXMLNode& oNode)
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

void CFill::ReadImgBrush(CXMLNode& oNode)
{
	m_eMode = GetImageFillType(oNode.GetAttributeInt(L"mode", (int)EImageFillType::NONE));

	for (CXMLNode& oChild : oNode.GetChilds())
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
			m_chAlpha = (HWP_BYTE)oChild.GetAttributeInt(L"alpha", 255);
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
	: CHWPRecord(nTagNum, nLevel, nSize), m_pFill(nullptr)
{}

CHWPRecordBorderFill::CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pFill(nullptr)
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

CHWPRecordBorderFill::CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_BORDER_FILL, 0, 0), m_pFill(nullptr)
{
	m_bThreeD = oNode.GetAttributeBool(L"threeD");
	m_bShadow = oNode.GetAttributeBool(L"shadow");
	m_bBreakCellSeparateLine = oNode.GetAttributeBool(L"breakCellSeparateLine");

	HWP_STRING sChildName;

	for (CXMLNode& oChild : oNode.GetChilds())
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

			m_chCrookedSlash = oChild.GetAttributeBool(L"Crooked");
			m_bCounterSlash = oChild.GetAttributeBool(L"isCounter");
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

			m_chCrookedBackSlash = oChild.GetAttributeBool(L"Crooked");
			m_bCounterBackSlash = oChild.GetAttributeBool(L"isCounter");
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
			m_pFill = new CFill(oChild);
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
