/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _PDF_WRITER_SRC_ENCRYPT_DICTIONARY_H
#define _PDF_WRITER_SRC_ENCRYPT_DICTIONARY_H

#include "Objects.h"

namespace PdfWriter
{
	class CEncrypt;
	class CInfoDict;
	class CStream;

	class CEncryptDict : public CDictObject
	{
	public:
		CEncryptDict();
		CEncryptDict(CXref* pXref);
		void Fix();
		~CEncryptDict();
		EDictType GetDictType() const
		{
			return dict_type_ENCRYPT;
		}

		static void CreateId(CInfoDict* pDict, CXref* pXref, BYTE* pBuffer);
        void SetPasswords(const std::wstring & wsOwnerPassword, const std::wstring & wsUserPassword);
		void Prepare(CInfoDict* pInfo, CXref* pXref);
		CEncrypt* GetEncrypt() const
		{
			return m_pEncrypt;
		}
		bool UpdateKey(int nCryptAlgorithm);
	private:
		CEncrypt* m_pEncrypt;
		std::string PadOrTrancatePassword(const std::wstring & wsPassword);
	};

	class CSignatureDict : public CDictObject
	{
	public:
		CSignatureDict(CXref* pXref);
		~CSignatureDict();
		EDictType GetDictType() const override
		{
			return dict_type_SIGNATURE;
		}

		void WriteToStream(CStream* pStream, CEncrypt* pEncrypt) override;
		void WriteToStream(CStream* pStream, int nFileEnd);
		bool FinalizeSignature(CStream* pStream, BYTE* pSignedData, DWORD dwDataLength);

		void SetContentsSize(unsigned int nSize);
		void SetName(const std::string& sName);
		void SetLocation(const std::string& sLocation);
		void SetReason(const std::string& sReason);
		void SetContact(const std::string& sContacts);
		void SetDate();
	private:
		unsigned int m_nContentsSize; // Contents size

		int m_nLen1;    // First interval length
		int m_nOffset2; // Second interval start

		int m_nByteRangeBegin; // ByteRange array start offset
		int m_nByteRangeEnd;   // ByteRange array end offset
	};
}
#endif // _PDF_WRITER_SRC_ENCRYPT_DICTIONARY_H
