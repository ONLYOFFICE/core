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
#include "Encrypt.h"
#include "Objects.h"

#include "../../Common/3dParty/cryptopp/modes.h"
#include "../../Common/3dParty/cryptopp/aes.h"
#include "../../Common/3dParty/cryptopp/sha.h"
#include "../../Common/3dParty/cryptopp/md5.h"
#include "../../Common/3dParty/cryptopp/arc4.h"
#include "../../Common/3dParty/cryptopp/filters.h"
#include "../../Common/3dParty/cryptopp/osrng.h"

namespace PdfWriter
{
    static void MD5(unsigned char *sMessage, int nMessageLen, unsigned char *sDigest)
    {
        CryptoPP::MD5 hash;

        hash.Update( sMessage, nMessageLen);

        CryptoPP::SecByteBlock buffer(hash.DigestSize());
        hash.Final(buffer);

        memcpy(sDigest, buffer.BytePtr(), buffer.size());
        return;
    }
    static int SHA(int type, unsigned char *sMessage, int nMessageLen, unsigned char *sDigest)
    {
        int res = 0;
        switch(type)
        {
            case 0:
            case 256:
            {
                CryptoPP::SHA256 hash;
                hash.Update( sMessage, nMessageLen > 0 ? nMessageLen : hash.DigestSize());

                CryptoPP::SecByteBlock buffer(res = hash.DigestSize());
                hash.Final(buffer);

                memcpy(sDigest, buffer.data(), buffer.size());
            }break;
            case 1:
            case 384:
            {
                CryptoPP::SHA384 hash;
                hash.Update( sMessage, nMessageLen > 0 ? nMessageLen : hash.DigestSize());

                CryptoPP::SecByteBlock buffer(res = hash.DigestSize());
                hash.Final(buffer);

                memcpy(sDigest, buffer.data(), buffer.size());
            }break;
            case 2:
            case 512:
            {
                CryptoPP::SHA512 hash;
                hash.Update( sMessage, nMessageLen > 0 ? nMessageLen : hash.DigestSize());

                CryptoPP::SecByteBlock buffer(res = hash.DigestSize());
                hash.Final(buffer);

                memcpy(sDigest, buffer.data(), buffer.size());
            }break;
        }
        return res;
    }
    static unsigned char passwordPad[32] =
    {
        0x28, 0xbf, 0x4e, 0x5e, 0x4e, 0x75, 0x8a, 0x41,
        0x64, 0x00, 0x4e, 0x56, 0xff, 0xfa, 0x01, 0x08,
        0x2e, 0x2e, 0x00, 0xb6, 0xd0, 0x68, 0x3e, 0x80,
        0x2f, 0x0c, 0xa9, 0xfe, 0x64, 0x53, 0x69, 0x7a
    };

    class CEncrypt::Impl
    {
    public:
        Impl() : m_nCryptAlgorithm(2), aesEncryption(NULL), rc4Encryption(NULL), streamEncryption(NULL)
		{
			m_unEncryptionKeyLength = 32;
			MemSet(m_anEncryptionKey, 0, m_unEncryptionKeyLength);
		}
        virtual ~Impl()
        {
            if (streamEncryption)
                delete streamEncryption;
            if (aesEncryption)
                delete aesEncryption;
            if (rc4Encryption)
                delete rc4Encryption;
        }
        void Reset()
        {
            if (streamEncryption)
                delete streamEncryption;
            if (aesEncryption)
                delete aesEncryption;
            if (rc4Encryption)
                delete rc4Encryption;

			CryptoPP::RandomPool prng;
			CryptoPP::SecByteBlock iv(16);
			CryptoPP::OS_GenerateRandomBlock(false, iv, iv.size());
			prng.IncorporateEntropy(iv, iv.size());

			memcpy(streamInitialization, iv, iv.size());

            if (m_nCryptAlgorithm != 2) // cryptAES256
            {
                memcpy(m_anObjectKey, m_anEncryptionKey, m_unEncryptionKeyLength);

                m_anObjectKey[m_unEncryptionKeyLength + 0] =  m_nObjNum & 0xff;
                m_anObjectKey[m_unEncryptionKeyLength + 1] = (m_nObjNum >> 8) & 0xff;
                m_anObjectKey[m_unEncryptionKeyLength + 2] = (m_nObjNum >> 16) & 0xff;
                m_anObjectKey[m_unEncryptionKeyLength + 3] =  m_nObjGen & 0xff;
                m_anObjectKey[m_unEncryptionKeyLength + 4] = (m_nObjGen >> 8) & 0xff;

                int nLen = 0;
                if (m_nCryptAlgorithm == 1) // cryptAES
                {
                    m_anObjectKey[m_unEncryptionKeyLength + 5] = 0x73; // 's'
                    m_anObjectKey[m_unEncryptionKeyLength + 6] = 0x41; // 'A'
                    m_anObjectKey[m_unEncryptionKeyLength + 7] = 0x6c; // 'l'
                    m_anObjectKey[m_unEncryptionKeyLength + 8] = 0x54; // 'T'

                    nLen = m_unEncryptionKeyLength + 9;
                }
                else if (m_nCryptAlgorithm == 0) // cryptRC4
                    nLen = m_unEncryptionKeyLength + 5;
                MD5(m_anObjectKey, nLen, m_anObjectKey);

                if ((m_unObjectKeyLength = m_unEncryptionKeyLength + 5) > 16)
                    m_unObjectKeyLength = 16;

                if (m_nCryptAlgorithm == 0) // cryptRC4
                {
                    rc4Encryption = new CryptoPP::ARC4::Encryption();
                    rc4Encryption->SetKey(m_anObjectKey, m_unObjectKeyLength);
                }
                else if (m_nCryptAlgorithm == 1) // cryptAES
                {
                    aesEncryption       = new CryptoPP::AES::Encryption(m_anObjectKey, m_unObjectKeyLength);
                    streamEncryption    = new CryptoPP::CBC_Mode_ExternalCipher::Encryption( *aesEncryption, streamInitialization);
                }

                return;
            }
			
			aesEncryption       = new CryptoPP::AES::Encryption(m_anEncryptionKey, m_unEncryptionKeyLength);
            streamEncryption    = new CryptoPP::CBC_Mode_ExternalCipher::Encryption( *aesEncryption, streamInitialization);
        }

        std::string     m_sOwnerPassword;
        std::string     m_sUserPassword;
        BYTE            m_anEncryptionKey[32];
        unsigned int    m_unEncryptionKeyLength;
        BYTE            m_anObjectKey[32];
        unsigned int    m_unObjectKeyLength;
        int             m_nCryptAlgorithm;
        int             m_nObjNum;
        int             m_nObjGen;

        unsigned char streamInitialization[16];
        CryptoPP::AES::Encryption       *aesEncryption;
        CryptoPP::ARC4::Encryption      *rc4Encryption;
        CryptoPP::StreamTransformation  *streamEncryption;
    };
    CEncrypt::CEncrypt() : m_unKeyLen(32), m_unVersion(5), m_unRevision(6)
    {
        impl = new Impl();
		
		m_unPermission = ENABLE_PRINT | ENABLE_EDIT_ALL | ENABLE_COPY | ENABLE_EDIT | PERMISSION_PAD;
		
		MemSet(m_anEncryptID, 0, ID_LEN);
	}
    CEncrypt::~CEncrypt()
    {
        delete impl;
    }
    void CEncrypt::SetPasswords(const std::string &sUserPassword, const std::string &sOwnerPassword)
    {
        impl->m_sUserPassword = sUserPassword;
        impl->m_sOwnerPassword = sOwnerPassword;
    }
    bool CEncrypt::MakeFileKey(int nCryptAlgorithm)
    {
        impl->m_nCryptAlgorithm = nCryptAlgorithm;
        if (m_unRevision == 5 || m_unRevision == 6)
        {
            CryptoPP::SHA256 hash;

            hash.Update( (unsigned char*) impl->m_sOwnerPassword.c_str(), impl->m_sOwnerPassword.length());
            hash.Update( m_anOwnerKey + 32, 8);
            hash.Update( m_anUserKey, 48);

            CryptoPP::SecByteBlock pHashData(hash.DigestSize());
            hash.Final(pHashData);

            bool bValidate = true;
            if (m_unRevision == 6)
                bValidate = MakeFileKey3(impl->m_sOwnerPassword, pHashData.data(), pHashData.size(), m_anUserKey, 48);

            if (bValidate && 0 == memcmp(pHashData.data(), m_anOwnerKey, 32))
            {
                hash.Update( (unsigned char*) impl->m_sOwnerPassword.c_str(), impl->m_sOwnerPassword.length());
                hash.Update( m_anOwnerKey + 40, 8);
                hash.Update( m_anUserKey, 48);

                CryptoPP::SecByteBlock pHashKeyData(hash.DigestSize());
                hash.Final(pHashKeyData);

                if (m_unRevision == 6)
                    bValidate = MakeFileKey3(impl->m_sOwnerPassword, pHashKeyData.data(), pHashKeyData.size(), m_anUserKey, 48);
                unsigned char empty[16] = {};

                CryptoPP::AES::Decryption aesDecryption(pHashKeyData.data(), pHashKeyData.size());
                CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, empty);

                CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::ArraySink( impl->m_anEncryptionKey, 32), CryptoPP::StreamTransformationFilter::NO_PADDING );
                stfDecryptor.Put2(m_anOwnerEncryptKey, 32, 1, true);
                stfDecryptor.MessageEnd();
                return true;
            }
            else
            {
                hash.Update( (unsigned char*) impl->m_sUserPassword.c_str(), impl->m_sUserPassword.length());
                hash.Update( m_anUserKey + 32, 8);

                CryptoPP::SecByteBlock pHashData(hash.DigestSize());
                hash.Final(pHashData);

                if (m_unRevision == 6)
                    bValidate = MakeFileKey3(impl->m_sUserPassword, pHashData.data(), pHashData.size());

                if (bValidate && 0 == memcmp(pHashData.data(), m_anUserKey, 32))
                {
                    hash.Update( (unsigned char*) impl->m_sUserPassword.c_str(), impl->m_sUserPassword.length());
                    hash.Update( m_anUserKey + 40, 8);

                    CryptoPP::SecByteBlock pHashKeyData(hash.DigestSize());
                    hash.Final(pHashKeyData);

                    if (m_unRevision == 6)
                        bValidate = MakeFileKey3(impl->m_sUserPassword, pHashKeyData.data(), pHashKeyData.size());
                    unsigned char empty[16] = {};

                    CryptoPP::AES::Decryption aesDecryption(pHashKeyData.data(), pHashKeyData.size());
                    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, empty);

                    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::ArraySink( impl->m_anEncryptionKey, 32), CryptoPP::StreamTransformationFilter::NO_PADDING );
                    stfDecryptor.Put2(m_anUserEncryptKey, 32, 1, true);
                    stfDecryptor.MessageEnd();
                    return true;
                }
            }
        }
        else if (impl->m_sOwnerPassword.empty())
            return MakeFileKey2(impl->m_sUserPassword);
        else
        {
            int nLen = impl->m_sOwnerPassword.length();
            unsigned char arrOwnerPass[32];
            if (nLen < 32)
            {
                memcpy(arrOwnerPass, impl->m_sOwnerPassword.c_str(), nLen);
                memcpy(arrOwnerPass + nLen, passwordPad, 32 - nLen);
            }
            else
                memcpy(arrOwnerPass, impl->m_sOwnerPassword.c_str(), 32);
            MD5(arrOwnerPass, 32, arrOwnerPass);

            if (m_unRevision >= 3)
                for (int nIndex = 0; nIndex < 50; ++nIndex)
                    MD5(arrOwnerPass, 16, arrOwnerPass);

            unsigned char arrOwnerKey[32];
            if (m_unRevision == 2)
            {
                CryptoPP::ARC4::Decryption rc4Decryption;
                rc4Decryption.SetKey(arrOwnerPass, impl->m_unEncryptionKeyLength);

                rc4Decryption.ProcessData(arrOwnerKey, m_anOwnerKey, 32);
            }
            else
            {
                memcpy(arrOwnerKey, m_anOwnerKey, 32);
                for (int nIndex = 19; nIndex >= 0; --nIndex)
                {
                    unsigned char arrTempKey[16];
                    for (unsigned int nJ = 0; nJ < impl->m_unEncryptionKeyLength; ++nJ)
                        arrTempKey[nJ] = arrOwnerPass[nJ] ^ nIndex;
                    CryptoPP::ARC4::Decryption rc4Decryption;

                    rc4Decryption.SetKey(arrTempKey, impl->m_unEncryptionKeyLength);
                    rc4Decryption.ProcessData(arrOwnerKey, arrOwnerKey, 32);
                }
            }
            std::string sUserPassword2((char *)arrOwnerKey, 32);

            return MakeFileKey2(sUserPassword2);
        }
        return false;
    }
    bool CEncrypt::MakeFileKey3(const std::string &sPassword, unsigned char *pHash, int nHashSize, unsigned char *pHash2, int nHashSize2)
    {
        if (!pHash) return false;

        int size = 64 * (sPassword.length() + 64 + nHashSize2); // max

        unsigned char K[64];	//max size sha
        unsigned char *K1 = new unsigned char[size];
        unsigned char *E = new unsigned char[size];

        int hash_size = nHashSize;
        memcpy(K, pHash, nHashSize);

        int iteration = 0;

        while( (iteration < 64) || (iteration < E[size - 1] + 32))
        {
            size = 0;
            for (int i = 0; i < 64; i++)
            {
                memcpy(K1 + size, sPassword.c_str(), sPassword.length()); size += sPassword.length();
                memcpy(K1 + size, K, hash_size); size += hash_size;
                if (pHash2)
                {
                    memcpy(K1 + size, pHash2, nHashSize2); size += nHashSize2;
                }
            }

            CryptoPP::AES::Encryption aesEncryption(K, 16);
            CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, K + 16);

            CryptoPP::StreamTransformationFilter stfEncryption(cbcEncryption, new CryptoPP::ArraySink( E, size), CryptoPP::StreamTransformationFilter::NO_PADDING);

            stfEncryption.Put( K1, size);
            stfEncryption.MessageEnd();
//----------------------------------------------------------
            int E_mod_3 = 0;
            for (unsigned int i = 0; i < 16; ++i)
            {
                E_mod_3 += E[i];
            }
            E_mod_3 %= 3;

            hash_size = SHA(E_mod_3, E, size, K);

            iteration++;
        }

        delete []K1;
        delete []E;

        memcpy (pHash, K, 32); // pHash - from sha256
        return true;
    }
    bool CEncrypt::MakeFileKey2(const std::string &sUserPassword)
    {
        unsigned char sTest[32];
        unsigned char sTempKey[16];
        int nLen = 0;
        bool bResult = true;

        unsigned char* pBuffer = new unsigned char[72 + 16];

        if (false == sUserPassword.empty())
        {
            nLen = sUserPassword.length();
            if (nLen < 32)
            {
                memcpy(pBuffer, sUserPassword.c_str(), nLen);
                memcpy(pBuffer + nLen, passwordPad, 32 - nLen);
            }
            else
                memcpy(pBuffer, sUserPassword.c_str(), 32);
        }
        else
            memcpy(pBuffer, passwordPad, 32);

        memcpy(pBuffer + 32, m_anOwnerKey, 32);

        pBuffer[64] =  m_unPermission & 0xff;
        pBuffer[65] = (m_unPermission >> 8) & 0xff;
        pBuffer[66] = (m_unPermission >> 16) & 0xff;
        pBuffer[67] = (m_unPermission >> 24) & 0xff;

        memcpy(pBuffer + 68, m_anEncryptID, 16);
        nLen = 68 + 16;
        if (!true)
        {
            pBuffer[nLen++] = 0xff;
            pBuffer[nLen++] = 0xff;
            pBuffer[nLen++] = 0xff;
            pBuffer[nLen++] = 0xff;
        }
        MD5(pBuffer, nLen, impl->m_anEncryptionKey);
        if (m_unRevision >= 3)
            for (int nIndex = 0; nIndex < 50; ++nIndex)
                MD5(impl->m_anEncryptionKey, impl->m_unEncryptionKeyLength, impl->m_anEncryptionKey);

        if (m_unRevision == 2)
        {
            CryptoPP::ARC4::Decryption rc4Decryption;
            rc4Decryption.SetKey(impl->m_anEncryptionKey, impl->m_unEncryptionKeyLength);

            rc4Decryption.ProcessData(sTest, m_anUserKey, 32);

            bResult = (memcmp(sTest, passwordPad, 32) == 0);
        }
        else if (m_unRevision >= 3)
        {
            memcpy(sTest, m_anUserKey, 32);
            for (int nIndex = 19; nIndex >= 0; --nIndex)
            {
                for (unsigned int nJ = 0; nJ < impl->m_unEncryptionKeyLength; ++nJ)
                    sTempKey[nJ] = impl->m_anEncryptionKey[nJ] ^ nIndex;
                CryptoPP::ARC4::Decryption rc4Decryption;

                rc4Decryption.SetKey(sTempKey, impl->m_unEncryptionKeyLength);
                rc4Decryption.ProcessData(sTest, sTest, 32);
            }
            memcpy(pBuffer, passwordPad, 32);
            memcpy(pBuffer + 32, m_anEncryptID, 16);

            MD5(pBuffer, 32 + 16, pBuffer);

            bResult = (memcmp(sTest, pBuffer, 16) == 0);
        }
        else
            bResult = false;

        delete[] pBuffer;
        return bResult;
    }

    void CEncrypt::CreateUserKey()
	{
        CryptoPP::RandomPool prng;

        CryptoPP::SecByteBlock salt(16);
        CryptoPP::OS_GenerateRandomBlock(false, salt, salt.size());
        prng.IncorporateEntropy(salt, salt.size());

		memcpy(m_anUserKey + 32, salt.data(), salt.size());

        CryptoPP::SHA256 hash;

        hash.Update( (unsigned char*) impl->m_sUserPassword.c_str(), impl->m_sUserPassword.length());
        hash.Update( m_anUserKey + 32, 8);

        CryptoPP::SecByteBlock pHashData(hash.DigestSize());
        hash.Final(pHashData);

        if (MakeFileKey3(impl->m_sUserPassword, pHashData.data(), pHashData.size()))
        {
            memcpy(m_anUserKey, pHashData.data(), pHashData.size());

            hash.Update( (unsigned char*) impl->m_sUserPassword.c_str(), impl->m_sUserPassword.length());
            hash.Update( m_anUserKey + 40, 8);

            CryptoPP::SecByteBlock pHashKeyData(hash.DigestSize());
            hash.Final(pHashKeyData);

            MakeFileKey3(impl->m_sUserPassword, pHashKeyData.data(), pHashKeyData.size());
            unsigned char empty[16] = {};

            CryptoPP::AES::Encryption aesEncryption(pHashKeyData.data(), pHashKeyData.size());
            CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, empty);

            CryptoPP::StreamTransformationFilter stfEncryption(cbcEncryption, new CryptoPP::ArraySink( m_anUserEncryptKey, 32), CryptoPP::StreamTransformationFilter::NO_PADDING );
            stfEncryption.Put2(impl->m_anEncryptionKey, 32, 1, true);
            stfEncryption.MessageEnd();
        }
	}
	void CEncrypt::CreateOwnerKey()
	{
        CryptoPP::RandomPool prng;

        CryptoPP::SecByteBlock salt(16);
        CryptoPP::OS_GenerateRandomBlock(false, salt, salt.size());
        prng.IncorporateEntropy(salt, salt.size());

        memcpy(m_anOwnerKey + 32, salt.data(), salt.size());

        CryptoPP::SHA256 hash;

        hash.Update( (unsigned char*) impl->m_sOwnerPassword.c_str(), impl->m_sOwnerPassword.length());
        hash.Update( m_anOwnerKey + 32, 8);
        hash.Update( m_anUserKey, 48);

        CryptoPP::SecByteBlock pHashData(hash.DigestSize());
        hash.Final(pHashData);

        if (MakeFileKey3(impl->m_sOwnerPassword, pHashData.data(), pHashData.size(), m_anUserKey, 48))
        {
            memcpy(m_anOwnerKey, pHashData.data(), pHashData.size());

            hash.Update( (unsigned char*) impl->m_sOwnerPassword.c_str(), impl->m_sOwnerPassword.length());
            hash.Update( m_anOwnerKey + 40, 8);
            hash.Update( m_anUserKey, 48);

            CryptoPP::SecByteBlock pHashKeyData(hash.DigestSize());
            hash.Final(pHashKeyData);

            MakeFileKey3(impl->m_sOwnerPassword, pHashKeyData.data(), pHashKeyData.size(), m_anUserKey, 48);
            unsigned char empty[16] = {};

            CryptoPP::AES::Encryption aesEncryption(pHashKeyData.data(), pHashKeyData.size());
            CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, empty);

            CryptoPP::StreamTransformationFilter stfEncryption(cbcEncryption, new CryptoPP::ArraySink( m_anOwnerEncryptKey, 32), CryptoPP::StreamTransformationFilter::NO_PADDING );
            stfEncryption.Put2(impl->m_anEncryptionKey, 32, 1, true);
            stfEncryption.MessageEnd();
        }
	}
	void CEncrypt::CreateEncryptionKey()
	{
        CryptoPP::RandomPool prng;

        CryptoPP::SecByteBlock key(32);
        CryptoPP::OS_GenerateRandomBlock(false, key, key.size());
        prng.IncorporateEntropy(key, key.size());

		memcpy(impl->m_anEncryptionKey, key.data(), key.size());
//-------------------------------------------------------------------

		unsigned long long extended_perms = 0xffffffff00000000LL | m_unPermission;
		for (int i = 0; i < 8; ++i)
		{
			m_anPermEncrypt[i] = static_cast<unsigned char>(extended_perms & 0xff);
			extended_perms >>= 8;
		}
		m_anPermEncrypt[8]	= /*m_bEncryptMetadata ? 'T' : */'F';
		m_anPermEncrypt[9]	= 'a';
		m_anPermEncrypt[10]	= 'd';
		m_anPermEncrypt[11]	= 'b';
        
        CryptoPP::SecByteBlock p(4);
        CryptoPP::OS_GenerateRandomBlock(false, p, p.size());
        prng.IncorporateEntropy(p, p.size());

		memcpy(m_anPermEncrypt + 12, p.data(), p.size());

		unsigned char empty[16] = {};
		
		CryptoPP::AES::Encryption aesEncryption(impl->m_anEncryptionKey, 32);
		
		CryptoPP::CipherModeFinalTemplate_ExternalCipher<CryptoPP::ECB_OneWay> ecbEncryption(aesEncryption, empty ); 

        CryptoPP::StreamTransformationFilter stfEncryption(ecbEncryption, new CryptoPP::ArraySink( m_anPermEncrypt, 16), CryptoPP::StreamTransformationFilter::NO_PADDING );
        stfEncryption.Put2(m_anPermEncrypt, 16, 1, true);
        stfEncryption.MessageEnd();
	}
	void CEncrypt::InitKey(unsigned int unObjectId, unsigned short unGenNo)
	{
        impl->m_nObjNum = unObjectId;
        impl->m_nObjGen = unGenNo;
	}
	void CEncrypt::Reset()
    {
        impl->Reset();
	}
 #define PADDING_SIZE 16
         unsigned int CEncrypt::CryptBuf(const BYTE* pSrc, BYTE* pDst, unsigned int unLen)
        {
            if (impl->m_nCryptAlgorithm == 0) // cryptRC4
            {
                impl->rc4Encryption->ProcessData(pDst, pSrc, unLen);
                // the algorithm does not change the length of the data
                return unLen;
            }
            // cryptAES & cryptAES256

            // Note that the pad is present when M is evenly divisible by 16
            unsigned int unLenOut = (unLen / PADDING_SIZE + 1) * PADDING_SIZE;

			memcpy(pDst, impl->streamInitialization, 16);

            CryptoPP::StreamTransformationFilter stfEncryption(*impl->streamEncryption, new CryptoPP::ArraySink( pDst + 16, unLenOut), CryptoPP::StreamTransformationFilter::NO_PADDING );

            stfEncryption.Put2(pSrc, unLen, 0, true);
            
			if (unLenOut != unLen)
            {
				unsigned char empty[16] = {};
				// EXAMPLE A 9-byte message has a pad of 7 bytes, each with the value 0x07.
				memset(empty, unLenOut - unLen, 16);
				stfEncryption.Put2(empty, unLenOut - unLen, 0, true);

            }
			stfEncryption.MessageEnd();

            return unLenOut + 16;
        }
		 //unsigned int CEncrypt::CryptBuf(const BYTE* pSrc, BYTE* pDst, unsigned int unLen, bool bFirst, bool bLast)
   //     {
   //         unsigned int unLenOut = unLen;

   //         if (unLenOut % PADDING_SIZE != 0 && bLast)
   //             unLenOut = (unLen / PADDING_SIZE + 1) * PADDING_SIZE;

   //         if(bFirst)
   //             memcpy(pDst, impl->streamInitialization, 16);

   //         CryptoPP::StreamTransformationFilter stfEncryption(*impl->streamEncryption, new CryptoPP::ArraySink( pDst + (bFirst ? 16 : 0), unLenOut), CryptoPP::StreamTransformationFilter::NO_PADDING );

   //         stfEncryption.Put2(pSrc, unLen, 0, true);
   //         
			//if (unLenOut != unLen && bLast)
   //         {
			//	unsigned char empty[16] = {};
			//	stfEncryption.Put2(empty, unLenOut - unLen, 0, true);

   //         }
   //         if (bLast)
   //         {
   //             stfEncryption.MessageEnd();
   //         }
   //         return unLenOut + (bFirst ? 16 : 0);
   //     }
    void CEncrypt::SetKeyLength(unsigned int unLen)
    {
        impl->m_unEncryptionKeyLength = unLen / 8;
    }
}
