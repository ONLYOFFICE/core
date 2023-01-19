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

        POLE::uint64 pos	= pStream->tell();
        POLE::uint64 size	= pStream->size();

        std::vector<char> dataCSPName;
        while(pos  < size - 1)
        {
            dataCSPName.push_back(StreamUtils::ReadBYTE(pStream));
            dataCSPName.push_back(StreamUtils::ReadBYTE(pStream));
            if (dataCSPName[dataCSPName.size() - 1] == 0 && dataCSPName[dataCSPName.size() - 2] == 0)
            {
                break;
            }
            pos+=2;//unicode null-terminate string
        }
        //EncryptionVerifier
        crypt_data_aes.saltSize = StreamUtils::ReadDWORD(pStream);
        crypt_data_aes.saltValue = StreamUtils::ReadStringA(pStream, crypt_data_aes.saltSize);

        crypt_data_aes.encryptedVerifierInput = StreamUtils::ReadStringA(pStream, 0x10);

        crypt_data_aes.hashSize = StreamUtils::ReadDWORD(pStream);

        int szEncryptedVerifierHash = (ProviderType == 0x0001) ? 0x14 : 0x20;
        crypt_data_aes.encryptedVerifierValue = StreamUtils::ReadStringA(pStream, szEncryptedVerifierHash);

        pos = pStream->tell();

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
