#include "HwpFileHeader.h"

namespace HWP
{
	CHwpFileHeader::CHwpFileHeader()
	{}

	bool CHwpFileHeader::Parse(CHWPStream& oBuffer)
	{
		unsigned int unOffset = 0;

		m_sSignature = std::string((char*)oBuffer.GetCurPtr(), 32);

		if ("HWP Document File" != m_sSignature)
			return false;

		unOffset += 32;

		//version
		unOffset += 4;

		#define CHECK_BOOL(value) (oBuffer[unOffset] & value) == value ? true : false

		m_bCompressed        = CHECK_BOOL(0x01);
		m_bPasswordEncrypted = CHECK_BOOL(0x02);
		m_bDistributable = CHECK_BOOL(0x04);
		m_bSaveScript = CHECK_BOOL(0x08);
		m_bDRMprotected = CHECK_BOOL(0x10);
		m_bHasXMLTemplateStorage = CHECK_BOOL(0x20);
		m_bHasDocumentHistory = CHECK_BOOL(0x40);
		m_bHasPkiSignature = CHECK_BOOL(0x80);

		++unOffset;

		m_bPkiEncrypted = CHECK_BOOL(0x01);
		m_bReservePkiSignature = CHECK_BOOL(0x02);
		m_bPkiCertificateDRM = CHECK_BOOL(0x04);
		m_bCCLDocument = CHECK_BOOL(0x08);
		m_bMobileOptimized = CHECK_BOOL(0x10);
		m_bPrivateInformation = CHECK_BOOL(0x20);
		m_bModifyTracking = CHECK_BOOL(0x40);
		m_bCopyrightKOGL = CHECK_BOOL(0x80);

		++unOffset;

		m_bHasVideoControl = CHECK_BOOL(0x01);
		m_bHasMarkFieldControl = CHECK_BOOL(0x02);

		unOffset += 2;

		m_bCopyrighted = CHECK_BOOL(0x01);
		m_bCopyProhibited = CHECK_BOOL(0x02);
		m_bCopyPermitted = CHECK_BOOL(0x04);

		unOffset += 4;

		//encryptVersion
		unOffset += 4;

		m_nCountryKOGLLicensed = (int)oBuffer[unOffset];

		return true;
	}
}
