#include "HwpFileHeader.h"

namespace HWP
{
	CHwpFileHeader::CHwpFileHeader()
	{}

	bool CHwpFileHeader::Parse(CHWPStream& oBuffer)
	{
		unsigned int unOffset = 0;

		m_sSignature = STRING((char*)oBuffer.GetCurPtr(), 32);

		if ("HWP Document File" != m_sSignature)
			return false;

		unOffset += 32;

		//version
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

	bool CHwpFileHeader::GetDistributable() const
	{
		return m_bDistributable;
	}
}
