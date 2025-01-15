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
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(nullptr)
{}

CHWPRecordBorderFill::CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
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

	m_oFill = CFill(oBuffer, 0, 0); // TODO:: перейти от использования off и size
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
	return &m_oFill;
}

}
