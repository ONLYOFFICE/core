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
#ifndef _PDF_READER_SECURITY_HANDLER_H
#define _PDF_READER_SECURITY_HANDLER_H

#include "Object.h"

namespace PdfReader
{
	class StringExt;
	class PDFDoc;

	//-------------------------------------------------------------------------------------------------------------------------------
	// SecurityHandler
	//-------------------------------------------------------------------------------------------------------------------------------

	class SecurityHandler
	{
	public:

		static SecurityHandler *Make(PDFDoc *pDocument, Object *pEncryptDict);

		SecurityHandler(PDFDoc *pDocument);
		virtual ~SecurityHandler();

		// Проверяем шифрованный ли документ. Если документ шифрованный, тогда сначала продуем использовать <seOwnerPassword> и 
		// <seUserPassword>. Если оба пароля не подходят, тогда запрашиваем пароль у пользователя (не более 3-х раз). 
		// Возвращаемые значения: 
		// True, если документ может быть открыт( либо он вообще не зашифрован, либо пароль верный)
		// False, в противном случае ( документ зашрифован, а пароль неверный).
		bool CheckEncryption(StringExt *seOwnerPassword, StringExt *seUserPassword);

		// Для "Batch mode", т.е. когда пароли заданы и проверяются без пользователя. 
		virtual void *MakeAuthData(StringExt *ownerPassword, StringExt *userPassword) = 0;

		// Для запроса пароля от пользователя.
		virtual void *GetAuthData() = 0;

		// Освобождаем память выделенную функциями MakeAuthData или GetAuthData.
		virtual void FreeAuthData(void *pAuthData) = 0;

		// Пытаемся авторизоваться.
		virtual bool Authorize(void *pAuthData) = 0;

		// Считываем различные параметры.
		virtual int            GetPermissionFlags()   = 0;
		virtual bool           GetOwnerPasswordValid()= 0;
		virtual unsigned char *GetFileKey()           = 0;
		virtual int            GetFileKeyLength()     = 0;
		virtual int            GetEncodingVersion()   = 0;
		virtual CryptAlgorithm GetEncodingAlgorithm() = 0;

	protected:

		PDFDoc *m_pDocument;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// StandardSecurityHandler
	//-------------------------------------------------------------------------------------------------------------------------------

	class StandardSecurityHandler : public SecurityHandler
	{
	public:

		StandardSecurityHandler(PDFDoc *pDocument, Object *pEncryptDict);
		virtual ~StandardSecurityHandler();

		virtual void *MakeAuthData(StringExt *seOwnerPassword, StringExt *seUserPassword);
		virtual void *GetAuthData();
		virtual void FreeAuthData(void *pAuthData);
		virtual bool Authorize(void *pAuthData);
		virtual int GetPermissionFlags()
		{
			return m_nPermissionFlags;
		}
		virtual bool GetOwnerPasswordValid()
		{
			return m_bOwnerPasswordValid;
		}
		virtual unsigned char *GetFileKey()
		{
			return m_sFileKey;
		}
		virtual int GetFileKeyLength()
		{
			return m_nFileKeyLength;
		}
		virtual int GetEncodingVersion()
		{
			return m_nEncryptVersion;
		}
		virtual CryptAlgorithm GetEncodingAlgorithm()
		{
			return m_eCryptType;
		}

	private:

		int            m_nPermissionFlags;
		bool           m_bOwnerPasswordValid;
		unsigned char  m_sFileKey[16];
		int            m_nFileKeyLength;
		int            m_nEncryptVersion;
		int            m_nEncryptRevision;
		CryptAlgorithm m_eCryptType;
		bool           m_bEncryptMetadata;

		StringExt     *m_seOwnerKey;
		StringExt     *m_seUserKey;
		StringExt     *m_seFileID;

		bool          m_bValid;
	};
}

#endif // _PDF_READER_SECURITY_HANDLER_H
