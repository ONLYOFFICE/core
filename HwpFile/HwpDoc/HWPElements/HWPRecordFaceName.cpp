#include "HWPRecordFaceName.h"

namespace HWP
{

EAltType GetAltType(int nValue)
{
	switch(static_cast<EAltType>(nValue))
	{
		case EAltType::UNKNOWN:
		default: return EAltType::UNKNOWN;
		case EAltType::FFT: return EAltType::FFT;
		case EAltType::HFT: return EAltType::HFT;
	}
}

CHWPRecordFaceName::CHWPRecordFaceName(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	HWP_BYTE chDataBits;
	oBuffer.ReadByte(chDataBits);

	m_bBasicFaceExists = CHECK_FLAG(chDataBits, 0x20);
	m_bAttrExists = CHECK_FLAG(chDataBits, 0x40);
	m_bSubstExists = CHECK_FLAG(chDataBits, 0x80);

	oBuffer.ReadString(m_sFaceName, EStringCharacter::UTF16);

	if (m_bSubstExists)
	{
		char chSubsType;
		oBuffer.ReadByte(chSubsType);
		m_eSubstType = GetAltType(chSubsType & 0x0F);

		oBuffer.ReadString(m_sSubstFace, EStringCharacter::UTF16);
	}

	if (m_bAttrExists)
	{
		oBuffer.ReadByte(m_chFamilyType);
		oBuffer.ReadByte(m_chSerifStyle);
		m_shWeight = oBuffer.ReadByte();
		m_shPropotion = oBuffer.ReadByte();
		m_shContrast = oBuffer.ReadByte();
		m_shStrokeVariation = oBuffer.ReadByte();
		m_shArmStyle = oBuffer.ReadByte();
		m_shLetterform = oBuffer.ReadByte();
		m_shMidLine = oBuffer.ReadByte();
		m_shXHeight = oBuffer.ReadByte();
	}

	if (m_bBasicFaceExists)
		oBuffer.ReadString(m_sBasicFaceName, EStringCharacter::UTF16);
}

HWP_STRING CHWPRecordFaceName::GetFaceName() const
{
	return m_sFaceName;
}
}
