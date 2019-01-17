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
#include "../../Common/3dParty/cryptopp/filters.h"
#include "../../Common/3dParty/cryptopp/osrng.h"

namespace PdfWriter
{
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

    class CEncrypt::Impl
    {
    public:
        Impl() : streamEncryption(NULL),  aesEncryption(NULL)
		{
			MemSet(m_anEncryptionKey, 0, 32);
		}
        virtual ~Impl()
        {
            if (streamEncryption)
                delete streamEncryption;
            if (aesEncryption)
                delete aesEncryption;
        }
        void Reset()
        {
            if (streamEncryption)
                delete streamEncryption;
            if (aesEncryption)
                delete aesEncryption;

			CryptoPP::RandomPool prng;
			CryptoPP::SecByteBlock iv(16);
			CryptoPP::OS_GenerateRandomBlock(false, iv, iv.size());
			prng.IncorporateEntropy(iv, iv.size());

			memcpy(streamInitialization, iv, iv.size());
			
			aesEncryption       = new CryptoPP::AES::Encryption(m_anEncryptionKey, 32);
            streamEncryption    = new CryptoPP::CBC_Mode_ExternalCipher::Encryption( *aesEncryption, streamInitialization);
        }

        std::string     m_sOwnerPassword;
        std::string     m_sUserPassword;
        BYTE			m_anEncryptionKey[32];

		unsigned char streamInitialization[16];
		CryptoPP::AES::Encryption       *aesEncryption;
        CryptoPP::StreamTransformation  *streamEncryption;
    };
    CEncrypt::CEncrypt() : m_unKeyLen(32)
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
	}
	void CEncrypt::Reset()
    {
        impl->Reset();
	}
 #define PADDING_SIZE 16
         unsigned int CEncrypt::CryptBuf(const BYTE* pSrc, BYTE* pDst, unsigned int unLen)
        {
            unsigned int unLenOut = unLen;

            if (unLenOut % PADDING_SIZE != 0)
                unLenOut = (unLen / PADDING_SIZE + 1) * PADDING_SIZE;

			memcpy(pDst, impl->streamInitialization, 16);

            CryptoPP::StreamTransformationFilter stfEncryption(*impl->streamEncryption, new CryptoPP::ArraySink( pDst + 16, unLenOut), CryptoPP::StreamTransformationFilter::NO_PADDING );

            stfEncryption.Put2(pSrc, unLen, 0, true);
            
			if (unLenOut != unLen)
            {
				unsigned char empty[16] = {};
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
        void CEncrypt::SetPermission(unsigned int unPermission)
        {
            m_unPermission = unPermission;
        }

}
