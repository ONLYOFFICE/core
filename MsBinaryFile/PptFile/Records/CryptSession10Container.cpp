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
#include "CryptSession10Container.h"

using namespace PPT;


CRecordEncryptedSummary::CRecordEncryptedSummary(){}

CRecordEncryptedSummary::~CRecordEncryptedSummary(){}

void CRecordEncryptedSummary::ReadFromStream(SRecordHeader &oHeader, const XLS::CFStreamPtr &pStream)
{
    m_oHeader = oHeader;

    unsigned char	flags;

    *pStream >> StreamOffset >> StreamSize >> Block >> NameSize >> flags;

    fStream	= GETBIT(flags, 0);

    if (NameSize > 0 && NameSize < 0xff)
    {
        StreamName = ReadStringW(pStream, NameSize);
    }
}

CEncryptionHeader::CEncryptionHeader(){}

CEncryptionHeader::~CEncryptionHeader(){}

void CEncryptionHeader::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    POLE::uint64 pos_start_record = pStream->tell();

    unsigned short vMajor = StreamUtils::ReadWORD(pStream);
    unsigned short vMinor = StreamUtils::ReadWORD(pStream);

    if(0x0001 == vMajor) // RC4 encryption header structure
    {
        bStandard = true;

        crypt_data_rc4.Salt.b1  = StreamUtils::ReadDWORD(pStream);
        crypt_data_rc4.Salt.b2  = StreamUtils::ReadDWORD(pStream);
        crypt_data_rc4.Salt.b3  = StreamUtils::ReadDWORD(pStream);
        crypt_data_rc4.Salt.b4  = StreamUtils::ReadDWORD(pStream);

        crypt_data_rc4.EncryptedVerifier.b1  = StreamUtils::ReadDWORD(pStream);
        crypt_data_rc4.EncryptedVerifier.b2  = StreamUtils::ReadDWORD(pStream);
        crypt_data_rc4.EncryptedVerifier.b3  = StreamUtils::ReadDWORD(pStream);
        crypt_data_rc4.EncryptedVerifier.b4  = StreamUtils::ReadDWORD(pStream);

        crypt_data_rc4.EncryptedVerifierHash.b1  = StreamUtils::ReadDWORD(pStream);
        crypt_data_rc4.EncryptedVerifierHash.b2  = StreamUtils::ReadDWORD(pStream);
        crypt_data_rc4.EncryptedVerifierHash.b3  = StreamUtils::ReadDWORD(pStream);
        crypt_data_rc4.EncryptedVerifierHash.b4  = StreamUtils::ReadDWORD(pStream);
    }
    else
    {
        bStandard				= false;
        crypt_data_aes.bAgile	= false;

        _UINT32 flags = StreamUtils::ReadDWORD(pStream);
        unsigned char *pDataRead = NULL;

        bool fCryptoAPI	= GETBIT(flags, 1);
        bool fDocProps	= GETBIT(flags, 2);
        bool fExternal	= GETBIT(flags, 3);
        bool fAES		= GETBIT(flags, 4);

        //EncryptionHeader
        int HeaderSize	= StreamUtils::ReadDWORD(pStream);
        int Flags		= StreamUtils::ReadDWORD(pStream);
        int SizeExtra	= StreamUtils::ReadDWORD(pStream);
        int AlgID		= StreamUtils::ReadDWORD(pStream);
        int AlgIDHash	= StreamUtils::ReadDWORD(pStream);
        int KeySize		= StreamUtils::ReadDWORD(pStream);
        int ProviderType= StreamUtils::ReadDWORD(pStream);
        int Reserved1	= StreamUtils::ReadDWORD(pStream);
        int Reserved2	= StreamUtils::ReadDWORD(pStream);

        // skip csp name
        pStream->seek(pos_start_record + 12 + HeaderSize); 

        //EncryptionVerifier
        crypt_data_aes.saltSize = StreamUtils::ReadDWORD(pStream);
        crypt_data_aes.saltValue = StreamUtils::ReadStringA(pStream, crypt_data_aes.saltSize);

        crypt_data_aes.encryptedVerifierInput = StreamUtils::ReadStringA(pStream, 0x10);

        crypt_data_aes.hashSize = StreamUtils::ReadDWORD(pStream);

        int szEncryptedVerifierHash = (ProviderType == 0x0001) ? 0x14 : 0x20;
        crypt_data_aes.encryptedVerifierValue = StreamUtils::ReadStringA(pStream, szEncryptedVerifierHash);

        //------------------------------------------------------------------------------------------
        switch(AlgIDHash)
        {
        case 0x8003: crypt_data_aes.hashAlgorithm = CRYPT_METHOD::MD5;	break;
        case 0x0000:
        case 0x8004: crypt_data_aes.hashAlgorithm = CRYPT_METHOD::SHA1;	break;
        }
        crypt_data_aes.spinCount = 0;

        switch(AlgID)
        {
        case 0x6801:
            crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::RC4;
            crypt_data_aes.keySize = KeySize / 8;
            if (crypt_data_aes.keySize == 0)
                crypt_data_aes.keySize = 5; // 40 bit
            break;
        case 0x660E:
            crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
            crypt_data_aes.keySize	= 128 /8;
            break;
        case 0x660F:
            crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
            crypt_data_aes.keySize	= 192 /8;
            break;
        case 0x6610:
            crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
            crypt_data_aes.keySize	= 256 /8;
            break;
        }

        //switch(ProviderType)
        //{
        //	case 0x0001:	crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::RC4;		break;
        //	case 0x0018:	crypt_data_aes.cipherAlgorithm = CRYPT_METHOD::AES_ECB; break;
        //}
    }
    POLE::uint64 pos_end_record = pStream->tell();

    //int sz_header = (pos_end_record - pos_start_record);
}
