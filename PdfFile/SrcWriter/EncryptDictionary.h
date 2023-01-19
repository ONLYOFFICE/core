/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _PDF_WRITER_SRC_ENCRYPT_DICTIONARY_H
#define _PDF_WRITER_SRC_ENCRYPT_DICTIONARY_H

#include "Objects.h"

#include "../../DesktopEditor/xmlsec/src/include/Certificate.h"

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
		void UpdateKey(int nCryptAlgorithm);
	private:
		CEncrypt* m_pEncrypt;
		std::string PadOrTrancatePassword(const std::wstring & wsPassword);
	};

	class CSignatureDict : public CDictObject
	{
	public:
		CSignatureDict(CXref* pXref);
		~CSignatureDict();
		EDictType GetDictType() const
		{
			return dict_type_SIGNATURE;
		}

		void SetByteRange(int nLen1, int nOffset2);
		void ByteRangeOffset(int nBegin, int nEnd);
		void WriteToStream(CStream* pStream, int nFileEnd);
		void SetCert(ICertificate* pCert);

		void SetName(const std::string& sName);
		void SetReason(const std::string& sReason);
		void SetContact(const std::string& sContacts);
		void SetDate();
	private:
		ICertificate* m_pCertificate;

		int m_nLen1;    // Длина  первого интервала сигнатуры
		int m_nOffset2; // Начало второго интервала сигнатуры

		int m_nByteRangeBegin; // Смещение начала массива ByteRange
		int m_nByteRangeEnd;   // Смещение конца  массива ByteRange
	};
}
#endif // _PDF_WRITER_SRC_ENCRYPT_DICTIONARY_H
