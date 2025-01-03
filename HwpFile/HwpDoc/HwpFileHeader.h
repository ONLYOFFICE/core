#ifndef HWPFILEHEADER_H
#define HWPFILEHEADER_H

#include "HWPStream.h"

namespace HWP
{
class CHwpFileHeader
{
	HWP_STRING m_sSignature;
	HWP_STRING m_sVersion;
	bool m_bCompressed;
	bool m_bPasswordEncrypted;
	bool m_bDistributable;
	bool m_bSaveScript;
	bool m_bDRMprotected;
	bool m_bHasXMLTemplateStorage;
	bool m_bHasDocumentHistory;
	bool m_bHasPkiSignature;
	bool m_bPkiEncrypted;
	bool m_bReservePkiSignature;
	bool m_bPkiCertificateDRM;
	bool m_bCCLDocument;
	bool m_bMobileOptimized;
	bool m_bPrivateInformation;
	bool m_bModifyTracking;
	bool m_bCopyrightKOGL;
	bool m_bHasVideoControl;
	bool m_bHasMarkFieldControl;

	bool m_bCopyrighted;
	bool m_bCopyProhibited;
	bool m_bCopyPermitted;

	int m_nEncryptVersion;
	int m_nCountryKOGLLicensed;
public:
	CHwpFileHeader();

	bool Compressed() const;
	bool PasswordEncrypted() const;
	bool Distributable() const;
	bool SignatureEmpty() const;
	bool VersionEmpty() const;

	HWP_STRING GetVersion() const;

	bool Parse(CHWPStream& oBuffer);
};
}

#endif // HWPFILEHEADER_H
