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
#include "EncryptDictionary.h"
#include "Encrypt.h"
#include "Info.h"

#include <ctime>

#include "../../Common/3dParty/cryptopp/md5.h"
#include "../../UnicodeConverter/UnicodeConverter.h"

namespace PdfWriter
{
    //----------------------------------------------------------------------------------------
	// CEncryptDict
	//----------------------------------------------------------------------------------------
	CEncryptDict::CEncryptDict(CXref* pXref)
	{
		m_pEncrypt = new CEncrypt();

		pXref->Add(this);
	}
	CEncryptDict::~CEncryptDict()
	{
		if (m_pEncrypt)
			delete m_pEncrypt;
	}
	void CEncryptDict::CreateId(CInfoDict* pInfo, CXref* pXref, BYTE* pBuffer)
	{
        CryptoPP::MD5 hash;

		std::time_t oTime = time(0);
        hash.Update( (BYTE*)&oTime, sizeof(oTime));

		// Создаем идентификатор файла по элементам библиотеки Info.
		if (pInfo)
		{
			const char *sTemp = NULL;
			unsigned int nLen = 0;

			// Author
			sTemp = pInfo->GetInfo(InfoAuthor);
			if ((nLen = StrLen(sTemp, -1)) > 0)
               hash.Update( (const BYTE *)sTemp, nLen );

			// Creator
			sTemp = pInfo->GetInfo(InfoCreator);
			if ((nLen = StrLen(sTemp, -1)) > 0)
                hash.Update( (const BYTE *)sTemp, nLen);

			// Producer   
			sTemp = pInfo->GetInfo(InfoProducer);
			if ((nLen = StrLen(sTemp, -1)) > 0)
                hash.Update( (const BYTE *)sTemp, nLen);

			// Title   
			sTemp = pInfo->GetInfo(InfoTitle);
			if ((nLen = StrLen(sTemp, -1)) > 0)
                hash.Update( (const BYTE *)sTemp, nLen);

			// Subject   
			sTemp = pInfo->GetInfo(InfoSubject);
			if ((nLen = StrLen(sTemp, -1)) > 0)
                hash.Update( (const BYTE *)sTemp, nLen);

			// Keywords   
			sTemp = pInfo->GetInfo(InfoKeyWords);
			if ((nLen = StrLen(sTemp, -1)) > 0)
                hash.Update( (const BYTE *)sTemp, nLen);

			int nXrefEntriesCount = pXref->GetCount();
            hash.Update( (const BYTE *)&nXrefEntriesCount, sizeof(unsigned int));

		}
        CryptoPP::SecByteBlock buffer(hash.DigestSize());
        hash.Final(buffer);

		memcpy(pBuffer, buffer.BytePtr(), buffer.size());
    }
    std::string CEncryptDict::PadOrTrancatePassword(const std::wstring & wsPassword)
    {
        NSUnicodeConverter::CUnicodeConverter conv;
		std::string sNewPassword = conv.SASLprepToUtf8(wsPassword);

        if (sNewPassword.length() > 127)
            sNewPassword = sNewPassword.substr(0, 127);

		return sNewPassword;
	}
	void CEncryptDict::SetPasswords(const std::wstring & wsOwnerPassword, const std::wstring & wsUserPassword)
	{
        std::string sOwnerPassword	= PadOrTrancatePassword(wsOwnerPassword);
        std::string sUserPassword	= PadOrTrancatePassword(wsUserPassword);

        m_pEncrypt->SetPasswords(sUserPassword, sOwnerPassword);
	}
	void CEncryptDict::Prepare(CInfoDict* pInfo, CXref* pXref)
	{
		CreateId(pInfo, pXref, (BYTE*)m_pEncrypt->m_anEncryptID);
		
        m_pEncrypt->CreateEncryptionKey();
        m_pEncrypt->CreateUserKey();
        m_pEncrypt->CreateOwnerKey();

        Add("Filter", "Standard");
        Add("V", 5);
        Add("Length", m_pEncrypt->m_unKeyLen * 8);
        Add("R", 6);
        Add("P", m_pEncrypt->m_unPermission);

		CDictObject* pCF = new CDictObject();
		
		CDictObject* pStdCF = new CDictObject();
		pCF->Add("StdCF", pStdCF);

		pStdCF->Add("CFM", "AESV3");
		pStdCF->Add("AuthEvent", "DocOpen");
		pStdCF->Add("Length", m_pEncrypt->m_unKeyLen);

		Add("CF", pCF);
		Add("StmF", "StdCF");
		Add("StrF", "StdCF");

		CBinaryObject* pUserKey = new CBinaryObject(m_pEncrypt->m_anUserKey, 48);
        if (!pUserKey)
            return;

        CBinaryObject* pUserEncryptKey = new CBinaryObject(m_pEncrypt->m_anUserEncryptKey, 32);
        if (!pUserKey)
            return;

        Add("U", pUserKey);
        Add("UE", pUserEncryptKey);

        CBinaryObject* pOwnerKey = new CBinaryObject(m_pEncrypt->m_anOwnerKey, 48);
        if (!pOwnerKey)
            return;

        CBinaryObject* pOwnerEncryptKey = new CBinaryObject(m_pEncrypt->m_anOwnerEncryptKey, 32);
        if (!pOwnerKey)
            return;

        Add("O", pOwnerKey);
        Add("OE", pOwnerEncryptKey);

        CBinaryObject* pEncryptPerm = new CBinaryObject(m_pEncrypt->m_anPermEncrypt, 16);
        Add("Perms", pEncryptPerm);
    }
}
