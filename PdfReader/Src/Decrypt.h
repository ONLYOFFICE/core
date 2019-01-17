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
#ifndef _PDF_READER_DECRYPT_H
#define _PDF_READER_DECRYPT_H

#include "StringExt.h"
#include "Object.h"
#include "Stream.h"

namespace PdfReader
{
	class StandardSecurityHandler;
	//-------------------------------------------------------------------------------------------------------------------------------
	// Decrypt
	//-------------------------------------------------------------------------------------------------------------------------------

	class Decrypt
	{
	public:
		// Строим FileKey. Параметр <sFileKey> должен иметь место как минимум под 16 байт. Проверяем <seOwnerPassword> и <seUserPassword>
		// и возвращаем true, если какой-нибудь корректен. Пишем <pbOwnerPasswordValide> = true, если <seOwnerPassword> был правильный.  
		// Один какой-нибудь или оба пароля могут быть NULL, что мы будем понимать как пустую строку.
		static bool MakeFileKey(StandardSecurityHandler *handler, const std::wstring &ownerPassword, const std::wstring &userPassword);

	private:

		static bool MakeFileKey3(const std::string &sPassword, unsigned char *pHash, int nHashSize, unsigned char *pHash2 = NULL, int nHashSize2 = 0);
		static bool MakeFileKey2(StandardSecurityHandler *handler, const std::string &userPassword);
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// DecryptStream
	//-------------------------------------------------------------------------------------------------------------------------------

	class DecryptStream : public FilterStream
	{

	public:

		DecryptStream(Stream *pStream, unsigned char *sFileKey, CryptAlgorithm eType, int nKeyLength, int nObjectNum, int nObjectGen);
		virtual ~DecryptStream();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual bool IsBinary(bool bLast);
		virtual Stream *getUndecodedStream() { return this; }

	private:

		CryptAlgorithm m_eCryptType;
		int            m_nObjectKeyLength;
		unsigned char  m_sObjectKey[(16 + 9) * 2];

		unsigned char*	m_pUncryptedData;
		unsigned int	m_pUncryptedDataSize;
		unsigned int	m_pUncryptedDataPosition;
	};
}

#endif // _PDF_READER_DECRYPT_H
