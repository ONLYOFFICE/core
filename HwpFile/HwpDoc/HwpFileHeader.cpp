#include "HwpFileHeader.h"

namespace HWP
{
CHwpFileHeader::CHwpFileHeader()
{}

bool CHwpFileHeader::Parse(CHWPStream& oBuffer)
{
	if (!oBuffer.IsValid())
		return false;

	oBuffer.ReadString(m_sSignature, 32, EStringCharacter::ASCII);

	if (L"HWP Document File" != m_sSignature)
		return false;

	//version
	m_sVersion = std::to_wstring((int)oBuffer[3]) + std::to_wstring((int)oBuffer[2]) + std::to_wstring((int)oBuffer[1]) + std::to_wstring((int)oBuffer[0]);
	oBuffer.Skip(4);

	m_bCompressed        = CHECK_FLAG(oBuffer[0], 0x01);
	m_bPasswordEncrypted = CHECK_FLAG(oBuffer[0], 0x02);
	m_bDistributable = CHECK_FLAG(oBuffer[0], 0x04);
	m_bSaveScript = CHECK_FLAG(oBuffer[0], 0x08);
	m_bDRMprotected = CHECK_FLAG(oBuffer[0], 0x10);
	m_bHasXMLTemplateStorage =CHECK_FLAG(oBuffer[0], 0x20);
	m_bHasDocumentHistory = CHECK_FLAG(oBuffer[0], 0x40);
	m_bHasPkiSignature = CHECK_FLAG(oBuffer[0], 0x80);

	oBuffer.Skip(1);

	m_bPkiEncrypted = CHECK_FLAG(oBuffer[0], 0x01);
	m_bReservePkiSignature = CHECK_FLAG(oBuffer[0], 0x02);
	m_bPkiCertificateDRM = CHECK_FLAG(oBuffer[0], 0x04);
	m_bCCLDocument = CHECK_FLAG(oBuffer[0], 0x08);
	m_bMobileOptimized = CHECK_FLAG(oBuffer[0], 0x10);
	m_bPrivateInformation = CHECK_FLAG(oBuffer[0], 0x20);
	m_bModifyTracking = CHECK_FLAG(oBuffer[0], 0x40);
	m_bCopyrightKOGL = CHECK_FLAG(oBuffer[0], 0x80);

	oBuffer.Skip(1);

	m_bHasVideoControl = CHECK_FLAG(oBuffer[0], 0x01);
	m_bHasMarkFieldControl = CHECK_FLAG(oBuffer[0], 0x02);

	oBuffer.Skip(2);

	m_bCopyrighted = CHECK_FLAG(oBuffer[0], 0x01);
	m_bCopyProhibited = CHECK_FLAG(oBuffer[0], 0x02);
	m_bCopyPermitted = CHECK_FLAG(oBuffer[0], 0x04);

	oBuffer.Skip(4);

	//encryptVersion
	oBuffer.Skip(4);

	m_nCountryKOGLLicensed = (int)oBuffer[0];

	return true;
}

bool CHwpFileHeader::Parse(CXMLReader& oReader)
{
	HWP_STRING wsMajor, wsMinor, wsMicro, wsBuildNumber;

	START_READ_ATTRIBUTES(oReader)
	{
		if ("major" == sAttributeName)
			wsMajor = oReader.GetText();
		else if ("minor" == sAttributeName)
			wsMinor = oReader.GetText();
		else if ("micro" == sAttributeName)
			wsMicro = oReader.GetText();
		else if ("buildNumber" == sAttributeName)
			wsBuildNumber = oReader.GetText();
	}
	END_READ_ATTRIBUTES(oReader)

	m_sVersion = ((!wsMajor.empty()) ? wsMajor : L"0") +
	             ((!wsMinor.empty()) ? wsMinor : L"0") +
	             ((!wsMicro.empty()) ? wsMicro : L"0") +
	             ((!wsBuildNumber.empty()) ? wsBuildNumber : L"0");

	return true;
}

bool CHwpFileHeader::Compressed() const
{
	return m_bCompressed;
}

bool CHwpFileHeader::PasswordEncrypted() const
{
	return m_bPasswordEncrypted;
}

bool CHwpFileHeader::Distributable() const
{
	return m_bDistributable;
}

bool CHwpFileHeader::SignatureEmpty() const
{
	return m_sSignature.empty();
}

bool CHwpFileHeader::VersionEmpty() const
{
	return m_sVersion.empty();
}

HWP_STRING CHwpFileHeader::GetVersion() const
{
	return m_sVersion;
}
}
