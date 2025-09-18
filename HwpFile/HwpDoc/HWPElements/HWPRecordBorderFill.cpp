#include "HWPRecordBorderFill.h"

#include "../Common/NodeNames.h"

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

void TBorder::Read(CXMLReader& oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
			m_eStyle = GetLineStyle2(oReader.GetText());
		else if (GetAttributeName(EAttribute::Color, eType) == sAttributeName)
			m_nColor = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Width, eType) == sAttributeName)
			m_chWidth = (HWP_BYTE)ConvertWidthToHWP(oReader.GetTextA());
	}
	END_READ_ATTRIBUTES(oReader)
}

CFill::CFill()
	: m_nFillType(0), m_eHatchStyle(EColorFillPattern::NONE), m_eMode(EImageFillType::NONE), m_chAlpha(0xff)
{}

CFill::CFill(CHWPStream& oBuffer, int nOff, int nSize)
	: m_nFillType(0), m_eHatchStyle(EColorFillPattern::NONE), m_eMode(EImageFillType::NONE), m_chAlpha(0xff)
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
		m_eGradType = ::HWP::GetGradFillType((int)chTypeNum);

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

CFill::CFill(CXMLReader& oReader, EHanType eType)
	: m_nFillType(0), m_eHatchStyle(EColorFillPattern::NONE), m_eMode(EImageFillType::NONE), m_chAlpha(0xff)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if (GetNodeName(ENode::WindowBrush, eType) == sNodeName)
		{
			ReadWinBrush(oReader, eType);
			m_nFillType |= 0x01;
		}
		else if (GetNodeName(ENode::Gradation, eType) == sNodeName)
		{
			ReadGradation(oReader, eType);
			m_nFillType |= 0x04;
		}
		else if (GetNodeName(ENode::ImageBrush, eType) == sNodeName)
		{
			ReadImgBrush(oReader, eType);
			m_nFillType |= 0x02;
		}
	}
	END_WHILE
}

void CFill::ReadWinBrush(CXMLReader& oReader, EHanType eType)
{
	m_eHatchStyle = EColorFillPattern::NONE;
	m_chAlpha = 0xff;

	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::FaceColor, eType) == sAttributeName)
			m_nFaceColor = oReader.GetColor(0xFFFFFFFF);
		else if (GetAttributeName(EAttribute::HatchColor, eType) == sAttributeName)
			m_nHatchColor = oReader.GetColor();
		else if (GetAttributeName(EAttribute::HatchStyle, eType) == sAttributeName)
			m_eHatchStyle = GetColorFillPattern(oReader.GetInt());
		else if (GetAttributeName(EAttribute::Alpha, eType) == sAttributeName)
			m_chAlpha = (HWP_BYTE)oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)
}

void CFill::ReadGradation(CXMLReader& oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
			m_eGradType = ::HWP::GetGradFillType(oReader.GetInt());
		else if (GetAttributeName(EAttribute::Angle, eType) == sAttributeName)
			m_nAngle = oReader.GetInt();
		else if (GetAttributeName(EAttribute::CenterX, eType) == sAttributeName)
			m_nCenterX = oReader.GetInt();
		else if (GetAttributeName(EAttribute::CenterY, eType) == sAttributeName)
			m_nCenterY = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Step, eType) == sAttributeName)
			m_nStep = oReader.GetInt();
		else if (GetAttributeName(EAttribute::ColorNum, eType) == sAttributeName)
			m_nColorNum = oReader.GetInt();
		else if (GetAttributeName(EAttribute::StepCenter, eType) == sAttributeName)
			m_chStepCenter = (HWP_BYTE)oReader.GetInt();
		else if (GetAttributeName(EAttribute::Alpha, eType) == sAttributeName)
			m_chAlpha = (HWP_BYTE)oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::Color, eType))
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::Value, eType) != sAttributeName)
				continue;

			m_arColors.push_back(oReader.GetColor());
		}
		END_READ_ATTRIBUTES(oReader)
	}
	END_WHILE
}

void CFill::ReadImgBrush(CXMLReader& oReader, EHanType eType)
{
	m_eMode = GetImageFillType(oReader.GetAttributeInt(GetAttributeName(EAttribute::Mode, eType)));

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::Image, eType))
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::Bright, eType) == sAttributeName)
				m_chBright = (HWP_BYTE)oReader.GetInt();
			else if (GetAttributeName(EAttribute::Contrast, eType) == sAttributeName)
				m_chContrast = (HWP_BYTE)oReader.GetInt();
			else if (GetAttributeName(EAttribute::Effect, eType) == sAttributeName)
			{
				const std::string sEffect{oReader.GetTextA()};

				if (GetValueName(EValue::RealPic, eType))
					m_chEffect = 0;
				else if (GetValueName(EValue::GrayScale, eType))
					m_chEffect = 1;
				else if (GetValueName(EValue::BlackWhite, eType))
					m_chEffect = 2;
			}
			else if (GetAttributeName(EAttribute::BinItem, eType))
				m_sBinItemID = oReader.GetText();
			else if (GetAttributeName(EAttribute::Alpha, eType) == sAttributeName)
				m_chAlpha = (HWP_BYTE)oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)
	}
	END_WHILE
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

EGradFillType CFill::GetGradFillType() const
{
	return m_eGradType;
}

int CFill::GetGradAngle() const
{
	return m_nAngle;
}

int CFill::GetGradCenterX() const
{
	return m_nCenterX;
}

int CFill::GetGradCenterY() const
{
	return m_nCenterY;
}

int CFill::GetGradStep() const
{
	return m_nStep;
}

int CFill::GetGradColorNum() const
{
	return m_nColorNum;
}

std::vector<int> CFill::GetGradColors() const
{
	return m_arColors;
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

CHWPRecordBorderFill::CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, CXMLReader& oReader, int nVersion, EHanType eType)
	: CHWPRecord(EHWPTag::HWPTAG_BORDER_FILL, 0, 0), m_pFill(nullptr)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::ThreeD, eType) == sAttributeName)
			m_bThreeD = oReader.GetBool();
		else if (GetAttributeName(EAttribute::Shadow, eType) == sAttributeName)
			m_bShadow = oReader.GetBool();
		else if (GetAttributeName(EAttribute::BreakCellSeparateLine, eType) == sAttributeName)
			m_bBreakCellSeparateLine = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if (GetNodeName(ENode::LeftBorder, eType) == sNodeName)
			m_oLeft.Read(oReader, eType);
		else if (GetNodeName(ENode::RightBorder, eType) == sNodeName)
			m_oRight.Read(oReader, eType);
		else if (GetNodeName(ENode::TopBorder, eType) == sNodeName)
			m_oTop.Read(oReader, eType);
		else if (GetNodeName(ENode::BottomBorder, eType) == sNodeName)
			m_oBottom.Read(oReader, eType);
		else if (GetNodeName(ENode::Diagonal, eType) == sNodeName)
			m_oDiagonal.Read(oReader, eType);
		else if (GetNodeName(ENode::FillBrush, eType) == sNodeName)
			m_pFill = new CFill(oReader, eType);
		else if (Equals(ENode::Slash, eType, sNodeName))
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
				{
					const std::string sType{oReader.GetTextA()};
					
					if (GetValueName(EValue::None, eType) == sType)
						m_chSlash = 0x0;
					else if (GetValueName(EValue::Center, eType) == sType)
						m_chSlash = 0b010;
					else if (GetValueName(EValue::CenterBelow, eType) == sType)
						m_chSlash = 0b011;
					else if (GetValueName(EValue::CenterAbove, eType) == sType)
						m_chSlash = 0b110;
					else if (GetValueName(EValue::All, eType) == sType)
						m_chSlash = 0b111;
				}
				else if ("Crooked" == sAttributeName)
					m_chCrookedSlash = oReader.GetBool();
				else if ("isCounter" == sAttributeName)
					m_bCounterSlash = oReader.GetBool();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if (GetNodeName(ENode::BackSlash, eType) == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
				{
					const std::string sType{oReader.GetTextA()};

					if (GetValueName(EValue::None, eType) == sType)
						m_chBackSlash = 0x0;
					else if (GetValueName(EValue::Center, eType) == sType)
						m_chBackSlash = 0b010;
					else if (GetValueName(EValue::CenterBelow, eType) == sType)
						m_chBackSlash = 0b011;
					else if (GetValueName(EValue::CenterAbove, eType) == sType)
						m_chBackSlash = 0b110;
					else if (GetValueName(EValue::All, eType) == sType)
						m_chBackSlash = 0b111;
				}
				else if ("Crooked" == sAttributeName)
					m_chCrookedBackSlash = oReader.GetBool();
				else if ("isCounter" == sAttributeName)
					m_bCounterBackSlash = oReader.GetBool();
			}
			END_READ_ATTRIBUTES(oReader)
		}
	}
	END_WHILE
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
