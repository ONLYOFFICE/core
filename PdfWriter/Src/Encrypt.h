/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _PDF_WRITER_SRC_ENCRYPT_H
#define _PDF_WRITER_SRC_ENCRYPT_H

#include "Utils.h"

#define ID_LEN              16
#define PASSWD_LEN          32
#define ENCRYPT_KEY_MAX     16
#define MD5_KEY_LEN         16
#define PERMISSION_PAD      0xFFFFFFC0
#define ARC4_BUF_SIZE       256

namespace PdfWriter
{
	void PadOrTrancatePassword(const char *sPassword, BYTE *pNewPassword);

	//----------------------------------------------------------------------------------------
	//
	// The code implements MD5 message-digest algorithm.
	// To compute the message digest of a chunk of bytes, declare an
	// MD5Context structure, pass it to MD5Init, call MD5Update as
	// needed on buffers full of bytes, and then call MD5Final, which
	// will fill a supplied 16-byte array with the digest.
	//
	//----------------------------------------------------------------------------------------
	class CMd5
	{
	public:

		CMd5(); 
		void Init();
		void Update(const BYTE* pBuffer, unsigned int unLen);
		void Final(BYTE anDigest[16]);

	private:

		void Clear();

	private:

		unsigned int m_anBuf[4];
		unsigned int m_anBits[2];
		BYTE         m_anIn[64];
	};
	class CArc4
	{
	public:

		void Init(const BYTE* pKey, unsigned int unKeyLen);
		void CryptBuf(const BYTE* pIn, BYTE* pOut, unsigned int unLen);

	private:
		BYTE m_nIndex1;
		BYTE m_nIndex2;
		BYTE m_anState[ARC4_BUF_SIZE];
	};
	class CEncrypt
	{
	public:

		CEncrypt();

		void Init();
		void CreateUserKey();
		void CreateOwnerKey();
		void CreateEncryptionKey();
		void InitKey(unsigned int unObjectId, unsigned short unGenNo);
		void Reset();
		void CryptBuf(const BYTE* pSrc, BYTE* pDst, unsigned int unLen);
		void SetPermission(unsigned int unPermission);
		void SetMode(EEncryptMode eMode, unsigned int unKeyLen);

	private:

		EEncryptMode   m_eMode;
		unsigned int   m_unKeyLen;                         // unKeyLen должно быть кратно 8, и лежать в отрезке от 40 и до 128	
		BYTE           m_anOwnerPassword[PASSWD_LEN];      // Owner-Password (не шифрованный)	
		BYTE           m_anUserPassword[PASSWD_LEN];       // User-Password (не шифрованный)
		BYTE           m_anOwnerKey[PASSWD_LEN];           // Owner-Password (шифрованный)
		BYTE           m_anUserKey[PASSWD_LEN];            // User-Password (шифрованный)
		unsigned int   m_unPermission;
		BYTE           m_anEncryptID[ID_LEN];
		BYTE           m_anEncryptionKey[MD5_KEY_LEN + 5];
		BYTE           m_anMD5EncryptionKey[MD5_KEY_LEN];
		CArc4          m_oArc4Context;

		friend class CEncryptDict;
		friend class CDocument;
	};
}

#endif // _PDF_WRITER_SRC_ENCRYPT_H


