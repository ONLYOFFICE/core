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
#include "Streams.h"

#include <ctime>

#include "../../Common/3dParty/cryptopp/md5.h"
#include "../../UnicodeConverter/UnicodeConverter.h"

#define SET_BINARY_PARAM(Name, set_func) \
    pObj = Get(Name);\
    if (pObj && pObj->GetType() == object_type_BINARY)\
        m_pEncrypt->set_func(((CBinaryObject*)pObj)->GetValue(), ((CBinaryObject*)pObj)->GetLength());
#define SET_NUMBER_PARAM(Name, set_func) \
    pObj = Get(Name);\
    if (pObj && pObj->GetType() == object_type_NUMBER)\
        m_pEncrypt->set_func(((CNumberObject*)pObj)->Get());

namespace PdfWriter
{
    //----------------------------------------------------------------------------------------
	// CEncryptDict
	//----------------------------------------------------------------------------------------
	CEncryptDict::CEncryptDict()
	{
		m_pEncrypt = new CEncrypt();
	}
	CEncryptDict::CEncryptDict(CXref* pXref)
	{
		m_pEncrypt = new CEncrypt();

		pXref->Add(this);
	}
    void CEncryptDict::Fix()
    {
        CObjectBase* pObj = NULL;
        SET_BINARY_PARAM("O", SetO);
        SET_BINARY_PARAM("U", SetU);
        SET_BINARY_PARAM("OE", SetOE);
        SET_BINARY_PARAM("UE", SetUE);
        SET_BINARY_PARAM("Perms", SetPerms);
        SET_BINARY_PARAM("ID", SetID);
        Remove("ID");

        SET_NUMBER_PARAM("P", SetPermission);
        SET_NUMBER_PARAM("R", SetRevision);
        SET_NUMBER_PARAM("V", SetVersion);
        SET_NUMBER_PARAM("Length", SetKeyLength);
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
    void CEncryptDict::UpdateKey(int nCryptAlgorithm)
    {
        m_pEncrypt->MakeFileKey(nCryptAlgorithm);
    }
    //----------------------------------------------------------------------------------------
    // CSignatureDict
    //----------------------------------------------------------------------------------------
    CSignatureDict::CSignatureDict(CXref* pXref)
    {
        pXref->Add(this);

        Add("Type", "Sig");
        Add("Filter", "Adobe.PPKLite");
        Add("SubFilter", "adbe.pkcs7.detached");

        unsigned int unDigestLength = 15000;
        BYTE* pDigest = new BYTE[unDigestLength];
        memset(pDigest, 0, unDigestLength);
        Add("Contents", new CBinaryObject(pDigest, unDigestLength));
        RELEASEARRAYOBJECTS(pDigest);

        CArrayObject* pByteRange = new CArrayObject();
        if (!pByteRange)
            return;
        Add("ByteRange", pByteRange);
        pByteRange->Add(0);
        pByteRange->Add(1234567890);
        pByteRange->Add(1234567890);
        pByteRange->Add(1234567890);

        // Reference - Массив справочных словарей сигнатур

        // Changes - Массив из трех чисел, который указывает изменения в документе в порядке: количество измененных страниц,
        // количество измененных полей и количество заполненных полей
        // Порядок подписей определяется значением ByteRange. Поскольку каждая подпись приводит к добавочному сохранению,
        // более поздние подписи имеют большее значение длины

        // Prop_Build - Словарь, который может использоваться обработчиком подписи для записи информации о состоянии компьютерной среды,
        // используемой для подписи, такой как имя обработчика, используемого для создания подписи, дата сборки программного обеспечения,
        // версия, и операционная система. Спецификация словаря PDF Signature Build содержит рекомендации по использованию этого словаря

        m_nLen1 = 0;
        m_nOffset2 = 0;
        m_nByteRangeBegin = 0;
        m_nByteRangeEnd = 0;
    }
    CSignatureDict::~CSignatureDict()
    {
    }
    void CSignatureDict::SetByteRange(int nLen1, int nOffset2)
    {
        m_nLen1 = nLen1;
        m_nOffset2 = nOffset2;
    }
    void CSignatureDict::ByteRangeOffset(int nBegin, int nEnd)
    {
        m_nByteRangeBegin = nBegin;
        m_nByteRangeEnd   = nEnd;
    }
    void CSignatureDict::WriteToStream(CStream* pStream, int nFileEnd)
    {
        // Запись ByteRange
        if (m_nByteRangeBegin > 0 && m_nByteRangeEnd > 0 && m_nByteRangeBegin < m_nByteRangeEnd && m_nByteRangeEnd < nFileEnd)
        {
            CArrayObject* pByteRange = new CArrayObject();
            if (!pByteRange)
                return;
            if (m_nLen1 > 0 && m_nOffset2 > 0 && m_nLen1 < m_nOffset2 && m_nOffset2 < nFileEnd)
            {
                pByteRange->Add(0);
                pByteRange->Add(m_nLen1);
                pByteRange->Add(m_nOffset2);
                pByteRange->Add(nFileEnd - m_nOffset2);

                pStream->Seek(m_nByteRangeBegin, EWhenceMode::SeekSet);
                pStream->Write(pByteRange, NULL);

                int nEnd = pStream->Tell();
                if (nEnd < m_nByteRangeEnd)
                {
                    unsigned int nLength = m_nByteRangeEnd - nEnd;
                    BYTE* pDifference = new BYTE[nLength];
                    MemSet(pDifference, ' ', nLength);

                    pStream->Write(pDifference, nLength);

                    RELEASEARRAYOBJECTS(pDifference);
                }
            }
            RELEASEOBJECT(pByteRange);
        }
        // Запись Contents
        if (m_pCertificate && m_nLen1 > 0 && m_nOffset2 > 0 && m_nLen1 < m_nOffset2 && m_nOffset2 < nFileEnd)
        {
            DWORD dwLenDataForSignature = m_nLen1 + nFileEnd - m_nOffset2;
            BYTE* pDataForSignature = new BYTE[dwLenDataForSignature];
            if (!pDataForSignature)
                return;

            pStream->Seek(0, EWhenceMode::SeekSet);
            unsigned int dwLenReadData = m_nLen1;
            pStream->Read(pDataForSignature, &dwLenReadData);
            if ((int)dwLenReadData != m_nLen1)
            {
                RELEASEARRAYOBJECTS(pDataForSignature);
                return;
            }

            pStream->Seek(m_nOffset2, EWhenceMode::SeekSet);
            dwLenReadData = nFileEnd - m_nOffset2;
            pStream->Read(pDataForSignature + m_nLen1, &dwLenReadData);
            if ((int)dwLenReadData != nFileEnd - m_nOffset2)
            {
                RELEASEARRAYOBJECTS(pDataForSignature);
                return;
            }

            BYTE* pDatatoWrite;
            unsigned int dwLenDatatoWrite;
            m_pCertificate->SignPKCS7(pDataForSignature, dwLenDataForSignature, pDatatoWrite, dwLenDatatoWrite);
            RELEASEARRAYOBJECTS(pDataForSignature);
            if (!pDatatoWrite)
                return;

            pStream->Seek(m_nLen1, EWhenceMode::SeekSet);
            CBinaryObject* pContents = new CBinaryObject(pDatatoWrite, dwLenDatatoWrite);
            RELEASEARRAYOBJECTS(pDatatoWrite);
            if (!pContents)
                return;
            // Цифровая подпись не шифруется
            pStream->Write(pContents, NULL);
            RELEASEOBJECT(pContents);

            // Стереть лишний >
            BYTE cChar = '0';
            pStream->Seek(pStream->Tell() - 1, EWhenceMode::SeekSet);
            pStream->Write(&cChar, 1);
        }
    }
    void CSignatureDict::SetCert(ICertificate* pCert)
    {
        m_pCertificate = pCert;
    }
    void CSignatureDict::SetName(const std::string& sName)
    {
        // Name - Cтрока, Имя лица или органа, подписавшего документ.
        // Значение следует использовать когда невозможно извлечь имя из подписи или сертификата подписавшего.
        Add("Name", new CStringObject(sName.c_str()));
    }
    void CSignatureDict::SetReason(const std::string& sReason)
    {
        // Reason - Строка, Причина подписания, например (Я согласен)
        Add("Reason", new CStringObject(sReason.c_str()));
    }
    void CSignatureDict::SetContact(const std::string& sContacts)
    {
        // ContactInfo - Строка, Информация, предоставленная подписывающей стороной,
        // чтобы получатель мог связаться с подписывающей стороной для проверки подписи, например (номер_телефона)
        Add("ContactInfo", new CStringObject(sContacts.c_str()));
    }
    void CSignatureDict::SetDate()
    {
        // M - Дата, Время подписания
        // Значение следует использовать когда время подписания недоступно в подписи
        char sTemp[DATE_TIME_STR_LEN + 1];
        char* pTemp = NULL;

        MemSet(sTemp, 0, DATE_TIME_STR_LEN + 1);
        time_t oTime = time(0);
        struct tm* oNow = gmtime(&oTime);

        pTemp = (char*)MemCpy((BYTE*)sTemp, (BYTE*)"D:", 2);
        *pTemp++;
        *pTemp++;
        pTemp = ItoA2(pTemp, oNow->tm_year + 1900, 5);
        pTemp = ItoA2(pTemp, oNow->tm_mon + 1, 3);
        pTemp = ItoA2(pTemp, oNow->tm_mday, 3);
        pTemp = ItoA2(pTemp, oNow->tm_hour, 3);
        pTemp = ItoA2(pTemp, oNow->tm_min, 3);
        pTemp = ItoA2(pTemp, oNow->tm_sec, 3);
        *pTemp++ = '+';
        pTemp = ItoA2(pTemp, 0, 3);
        *pTemp++ = '\'';
        pTemp = ItoA2(pTemp, 0, 3);
        *pTemp++ = '\'';
        *pTemp = 0;

        Add("M", new CStringObject(sTemp));
    }
}
