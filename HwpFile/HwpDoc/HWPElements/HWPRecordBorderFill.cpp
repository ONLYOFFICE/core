#include "HWPRecordBorderFill.h"

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

void TBorder::Read(CXMLReader& oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("type" == sAttributeName)
			m_eStyle = GetLineStyle2(oReader.GetText());
		else if ("color" == sAttributeName)
			m_nColor = oReader.GetInt();
		else if ("width" == sAttributeName)
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

CFill::CFill(CXMLReader& oReader)
	: m_nFillType(0), m_eHatchStyle(EColorFillPattern::NONE), m_eMode(EImageFillType::NONE), m_chAlpha(0xff)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hc:winBrush" == sNodeName)
		{
			ReadWinBrush(oReader);
			m_nFillType |= 0x01;
		}
		else if ("hc:gradation" == sNodeName)
		{
			ReadGradation(oReader);
			m_nFillType |= 0x04;
		}
		else if ("hc:imgBrush" == sNodeName)
		{
			ReadImgBrush(oReader);
			m_nFillType |= 0x02;
		}
	}
	END_WHILE
}

void CFill::ReadWinBrush(CXMLReader& oReader)
{
	m_eHatchStyle = EColorFillPattern::NONE;
	m_chAlpha = 0xff;

	START_READ_ATTRIBUTES(oReader)
	{
		if ("faceColor" == sAttributeName)
			m_nFaceColor = oReader.GetColor(0xFFFFFFFF);
		else if ("hatchColor" == sAttributeName)
			m_nHatchColor = oReader.GetColor();
		else if ("hatchStyle" == sAttributeName)
			m_eHatchStyle = GetColorFillPattern(oReader.GetInt());
		else if ("alpha" == sAttributeName)
			m_chAlpha = (HWP_BYTE)oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)
}

void CFill::ReadGradation(CXMLReader& oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("type" == sAttributeName)
			m_eGradType = ::HWP::GetGradFillType(oReader.GetInt());
		else if ("angle" == sAttributeName)
			m_nAngle = oReader.GetInt();
		else if ("centerX" == sAttributeName)
			m_nCenterX = oReader.GetInt();
		else if ("centerY" == sAttributeName)
			m_nCenterY = oReader.GetInt();
		else if ("step" == sAttributeName)
			m_nStep = oReader.GetInt();
		else if ("colorNum" == sAttributeName)
			m_nColorNum = oReader.GetInt();
		else if ("stepCenter" == sAttributeName)
			m_chStepCenter = (HWP_BYTE)oReader.GetInt();
		else if ("alpha" == sAttributeName)
			m_chAlpha = (HWP_BYTE)oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hc:color")
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("value" != oReader.GetName())
				continue;

			m_arColors.push_back(oReader.GetColor(true));
		}
		END_READ_ATTRIBUTES(oReader)
	}
	END_WHILE
}

void CFill::ReadImgBrush(CXMLReader& oReader)
{
	m_eMode = GetImageFillType(oReader.GetAttributeInt("mode"));

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hc:img")
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("bright" == sAttributeName)
				m_chBright = (HWP_BYTE)oReader.GetInt();
			else if ("contrast" == sAttributeName)
				m_chContrast = (HWP_BYTE)oReader.GetInt();
			else if ("effect" == sAttributeName)
			{
				const std::string sEffect{oReader.GetTextA()};

				if ("REAL_PIC" == sEffect)
					m_chEffect = 0;
				else if ("GRAY_SCALE" == sEffect)
					m_chEffect = 1;
				else if ("BLACK_WHITE" == sEffect)
					m_chEffect = 2;
			}
			else if ("binaryItemIDRef" == sAttributeName)
				m_sBinItemID = oReader.GetText();
			else if ("alpha" == sAttributeName)
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

CHWPRecordBorderFill::CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, CXMLReader& oReader, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_BORDER_FILL, 0, 0), m_pFill(nullptr)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("threeD" == sAttributeName)
			m_bThreeD = oReader.GetBool();
		else if ("shadow" == sAttributeName)
			m_bShadow = oReader.GetBool();
		else if ("breakCellSeparateLine" == sAttributeName)
			m_bBreakCellSeparateLine = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hh:slash" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("type" == sAttributeName)
				{
					const std::string sType{oReader.GetTextA()};

					if ("NONE" == sType)
						m_chSlash = 0x0;
					else if ("CENTER" == sType)
						m_chSlash = 0b010;
					else if ("CENTER_BELOW" == sType)
						m_chSlash = 0b011;
					else if ("CENTER_ABOVE" == sType)
						m_chSlash = 0b110;
					else if ("ALL" == sType)
						m_chSlash = 0b111;
				}
				else if ("Crooked" == sAttributeName)
					m_chCrookedSlash = oReader.GetBool();
				else if ("isCounter" == sAttributeName)
					m_bCounterSlash = oReader.GetBool();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hh:backSlash" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("type" == sAttributeName)
				{
					const std::string sType{oReader.GetTextA()};

					if ("NONE" == sType)
						m_chBackSlash = 0x0;
					else if ("CENTER" == sType)
						m_chBackSlash = 0b010;
					else if ("CENTER_BELOW" == sType)
						m_chBackSlash = 0b011;
					else if ("CENTER_ABOVE" == sType)
						m_chBackSlash = 0b110;
					else if ("ALL" == sType)
						m_chBackSlash = 0b111;
				}
				else if ("Crooked" == sAttributeName)
					m_chCrookedBackSlash = oReader.GetBool();
				else if ("isCounter" == sAttributeName)
					m_bCounterBackSlash = oReader.GetBool();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hh:leftBorder" == sNodeName)
			m_oLeft.Read(oReader);
		else if ("hh:rightBorder" == sNodeName)
			m_oRight.Read(oReader);
		else if ("hh:topBorder" == sNodeName)
			m_oTop.Read(oReader);
		else if ("hh:bottomBorder" == sNodeName)
			m_oBottom.Read(oReader);
		else if ("hh:diagonal" == sNodeName)
			m_oDiagonal.Read(oReader);
		else if ("hc:fillBrush" == sNodeName)
			m_pFill = new CFill(oReader);
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
