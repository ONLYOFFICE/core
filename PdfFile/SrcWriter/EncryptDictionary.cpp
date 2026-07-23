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
#include "EncryptDictionary.h"
#include "Encrypt.h"
#include "Info.h"
#include "Streams.h"

#include <ctime>

#include "../../Common/3dParty/cryptopp/md5.h"
#include "../../UnicodeConverter/UnicodeConverter.h"

#define SET_BINARY_PARAM(Name, set_func, max_len) \
    pObj = Get(Name);\
    if (pObj && pObj->GetType() == object_type_BINARY)\
        m_pEncrypt->set_func(((CBinaryObject*)pObj)->GetValue(), std::min((int)((CBinaryObject*)pObj)->GetLength(), max_len));
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
		SET_BINARY_PARAM("O", SetO, 48);
		SET_BINARY_PARAM("U", SetU, 48);
		SET_BINARY_PARAM("OE", SetOE, 32);
		SET_BINARY_PARAM("UE", SetUE, 32);
		SET_BINARY_PARAM("Perms", SetPerms, 16);
		SET_BINARY_PARAM("ID", SetID, 16);
        pObj = Get("ID");
        if (pObj && pObj->GetType() == object_type_BINARY)
            m_pEncrypt->m_unIDLength = ((CBinaryObject*)pObj)->GetLength();
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

		// Create file identifier from Info elements.
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
    bool CEncryptDict::UpdateKey(int nCryptAlgorithm)
    {
        return m_pEncrypt->MakeFileKey(nCryptAlgorithm);
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

        m_nLen1 = 0;
        m_nOffset2 = 0;
        m_nByteRangeBegin = 0;
        m_nByteRangeEnd = 0;
    }
    CSignatureDict::~CSignatureDict()
    {
    }
	void CSignatureDict::WriteToStream(CStream* pStream, CEncrypt* pEncrypt)
	{
		for (std::map<std::string, CObjectBase*>::const_iterator oIter = m_mList.begin(); oIter != m_mList.end(); ++oIter)
		{
			CObjectBase* pObject = oIter->second;
			if (!pObject || pObject->IsHidden())
				continue;

			int nBegin, nEnd;
			pStream->WriteEscapeName(oIter->first.c_str());
			pStream->WriteChar(' ');
			nBegin = pStream->Tell();
			// Digital signature is not encrypted
			pStream->Write(pObject, oIter->first == "Contents" ? NULL : pEncrypt);
			nEnd = pStream->Tell();
			pStream->WriteStr("\012");
			if (oIter->first == "Contents")
			{
				m_nLen1  = nBegin;
				m_nOffset2 = nEnd;
			}
			if (oIter->first == "ByteRange")
			{
				m_nByteRangeBegin = nBegin;
				m_nByteRangeEnd   = nEnd;
			}
		}
	}
	void CSignatureDict::WriteToStream(CStream* pStream, int nFileEnd)
    {
        // Write ByteRange
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
    }
	bool CSignatureDict::FinalizeSignature(CStream* pStream, BYTE* pSignedData, DWORD dwDataLength)
	{
		if (!pSignedData)
			return false;

		// Write signed data to Contents
		if (dwDataLength > m_nContentsSize)
		{
			// Signature doesn't fit! Size calculation error
			return false;
		}

		pStream->Seek(m_nLen1, EWhenceMode::SeekSet);
		CBinaryObject* pContents = new CBinaryObject(pSignedData, dwDataLength);
		if (!pContents)
			return false;

		// Digital signature is not encrypted
		pStream->Write(pContents, NULL);
		delete pContents;

		// Erase extra '>' if needed
		BYTE cChar = '0';
		pStream->Seek(pStream->Tell() - 1, EWhenceMode::SeekSet);
		pStream->Write(&cChar, 1);

		return true;
	}
	void CSignatureDict::SetContentsSize(unsigned int nSize)
	{
		m_nContentsSize = nSize;

		BYTE* pDigest = new BYTE[m_nContentsSize];
		memset(pDigest, 0, m_nContentsSize);
		Add("Contents", new CBinaryObject(pDigest, m_nContentsSize, false));

		CArrayObject* pByteRange = new CArrayObject();
		if (!pByteRange)
			return;
		Add("ByteRange", pByteRange);
		pByteRange->Add(0);
		pByteRange->Add(1234567890);
		pByteRange->Add(1234567890);
		pByteRange->Add(1234567890);
	}
	void CSignatureDict::SetName(const std::string& sName)
    {
        // Name - String, Name of the person or authority signing the document.
        // This value should be used when the name cannot be extracted from the signature or signer's certificate.
        Add("Name", new CStringObject(sName.c_str()));
    }
	void CSignatureDict::SetLocation(const std::string& sLocation)
	{
		Add("Location", new CStringObject(sLocation.c_str()));
	}
    void CSignatureDict::SetReason(const std::string& sReason)
    {
        // Reason - String, Reason for signing, for example (I agree)
        Add("Reason", new CStringObject(sReason.c_str()));
    }
    void CSignatureDict::SetContact(const std::string& sContacts)
    {
        // ContactInfo - String, Information provided by the signer,
        // so that the recipient can contact the signer to verify the signature, for example (phone_number)
        Add("ContactInfo", new CStringObject(sContacts.c_str()));
    }
    void CSignatureDict::SetDate()
    {
        // M - Date, Time of signing
        // This value should be used when the signing time is not available in the signature

        Add("M", new CStringObject(DateNow().c_str()));
    }
}
