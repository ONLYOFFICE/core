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

CHWPRecordFaceName::CHWPRecordFaceName(CHWPDocInfo& oDocInfo, CXMLReader& oReader, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_FACE_NAME, 0, 0), m_pParent(&oDocInfo)
{
	m_sFaceName = oReader.GetAttribute("face");

	std::string sNodeName;

	WHILE_READ_NEXT_NODE(oReader)
	{
		sNodeName = oReader.GetNameA();

		if ("hh:substFont" == sNodeName)
		{
			m_bSubstExists = true;

			START_READ_ATTRIBUTES(oReader)
			{
				if ("face" == sAttributeName)
					m_sSubstFace = oReader.GetText2();
				else if ("type" == sAttributeName)
				{
					const std::string sType{oReader.GetText2A()};

					if ("TTF" == sType)
						m_eSubstType = EAltType::FFT;
					else if ("HFT" == sType)
						m_eSubstType = EAltType::HFT;
					else
						m_eSubstType = EAltType::UNKNOWN;
				}
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hh:typeInfo" == sNodeName)
		{
			m_bAttrExists = true;

			START_READ_ATTRIBUTES(oReader)
			{
				if ("familyType" == sAttributeName)
				{
					m_sBasicFaceName = oReader.GetText2();

					if (!m_sBasicFaceName.empty())
						m_bBasicFaceExists = true;
				}
				else if ("serifStyle" == sAttributeName)
					m_chSerifStyle = (HWP_BYTE)oReader.GetInt();
				else if ("weight" == sAttributeName)
					m_shWeight = oReader.GetInt();
				else if ("proportion" == sAttributeName)
					m_shPropotion = oReader.GetInt();
				else if ("contrast" == sAttributeName)
					m_shContrast = oReader.GetInt();
				else if ("strokeVariation" == sAttributeName)
					m_shStrokeVariation = oReader.GetInt();
				else if ("armStyle" == sAttributeName)
					m_shArmStyle = oReader.GetInt();
				else if ("letterform" == sAttributeName)
					m_shLetterform = oReader.GetInt();
				else if ("midline" == sAttributeName)
					m_shMidLine = oReader.GetInt();
				else if ("xHeight" == sAttributeName)
					m_shXHeight = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
	}
}

HWP_STRING CHWPRecordFaceName::GetFaceName() const
{
	return m_sFaceName;
}
}
