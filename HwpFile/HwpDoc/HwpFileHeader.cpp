#include "HwpFileHeader.h"

namespace HWP
{
CHwpFileHeader::CHwpFileHeader()
{}

bool CHwpFileHeader::Parse(CHWPStream& oBuffer)
{
	if (!oBuffer.IsValid())
		return false;

	unsigned int unOffset = 0;

	oBuffer.ReadString(m_sSignature, 32, EStringCharacter::ASCII);

	if (L"HWP Document File" != m_sSignature)
		return false;

	//version
	m_sVersion = std::to_wstring((int)oBuffer[unOffset + 3]) + std::to_wstring((int)oBuffer[unOffset + 2]) + std::to_wstring((int)oBuffer[unOffset + 1]) + std::to_wstring((int)oBuffer[unOffset + 0]);
	unOffset += 4;

	m_bCompressed        = CHECK_FLAG(oBuffer[unOffset], 0x01);
	m_bPasswordEncrypted = CHECK_FLAG(oBuffer[unOffset], 0x02);
	m_bDistributable = CHECK_FLAG(oBuffer[unOffset], 0x04);
	m_bSaveScript = CHECK_FLAG(oBuffer[unOffset], 0x08);
	m_bDRMprotected = CHECK_FLAG(oBuffer[unOffset], 0x10);
	m_bHasXMLTemplateStorage =CHECK_FLAG(oBuffer[unOffset], 0x20);
	m_bHasDocumentHistory = CHECK_FLAG(oBuffer[unOffset], 0x40);
	m_bHasPkiSignature = CHECK_FLAG(oBuffer[unOffset], 0x80);

	++unOffset;

	m_bPkiEncrypted = CHECK_FLAG(oBuffer[unOffset], 0x01);
	m_bReservePkiSignature = CHECK_FLAG(oBuffer[unOffset], 0x02);
	m_bPkiCertificateDRM = CHECK_FLAG(oBuffer[unOffset], 0x04);
	m_bCCLDocument = CHECK_FLAG(oBuffer[unOffset], 0x08);
	m_bMobileOptimized = CHECK_FLAG(oBuffer[unOffset], 0x10);
	m_bPrivateInformation = CHECK_FLAG(oBuffer[unOffset], 0x20);
	m_bModifyTracking = CHECK_FLAG(oBuffer[unOffset], 0x40);
	m_bCopyrightKOGL = CHECK_FLAG(oBuffer[unOffset], 0x80);

	++unOffset;

	m_bHasVideoControl = CHECK_FLAG(oBuffer[unOffset], 0x01);
	m_bHasMarkFieldControl = CHECK_FLAG(oBuffer[unOffset], 0x02);

	unOffset += 2;

	m_bCopyrighted = CHECK_FLAG(oBuffer[unOffset], 0x01);
	m_bCopyProhibited = CHECK_FLAG(oBuffer[unOffset], 0x02);
	m_bCopyPermitted = CHECK_FLAG(oBuffer[unOffset], 0x04);

	unOffset += 4;

	//encryptVersion
	unOffset += 4;

	m_nCountryKOGLLicensed = (int)oBuffer[unOffset];

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

STRING CHwpFileHeader::GetVersion() const
{
	return m_sVersion;
}
}
