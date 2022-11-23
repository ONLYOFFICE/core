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
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/format.hpp>

#include "CryptTransform.h"

#include "../../Common/3dParty/cryptopp/modes.h"
#include "../../Common/3dParty/cryptopp/aes.h"
#include "../../Common/3dParty/cryptopp/des.h"
#include "../../Common/3dParty/cryptopp/sha.h"
#include "../../Common/3dParty/cryptopp/md5.h"
#include "../../Common/3dParty/cryptopp/rsa.h"
#include "../../Common/3dParty/cryptopp/rc2.h"
#include "../../Common/3dParty/cryptopp/arc4.h"
#include "../../Common/3dParty/cryptopp/rc5.h"
#include "../../Common/3dParty/cryptopp/pwdbased.h"
#include "../../Common/3dParty/cryptopp/filters.h"
#include "../../Common/3dParty/cryptopp/osrng.h"
#include "../../Common/3dParty/cryptopp/hex.h"
#include "../../Common/3dParty/cryptopp/blowfish.h"
#include "../../Common/3dParty/cryptopp/zinflate.h"
#include "../../Common/3dParty/cryptopp/zdeflate.h"

#include "../../OOXML/Base/unicode_util.h"
#include "../../OOXML/Base/Base.h"

#include "../../DesktopEditor/common/File.h"
static const unsigned char encrVerifierHashInputBlockKey[8]			= { 0xfe, 0xa7, 0xd2, 0x76, 0x3b, 0x4b, 0x9e, 0x79 };
static const unsigned char encrVerifierHashValueBlockKey[8]			= { 0xd7, 0xaa, 0x0f, 0x6d, 0x30, 0x61, 0x34, 0x4e };
static const unsigned char encrKeyValueBlockKey[8]					= { 0x14, 0x6e, 0x0b, 0xe7, 0xab, 0xac, 0xd0, 0xd6 };

static const unsigned char encrDataIntegritySaltBlockKey[8]			= { 0x5f, 0xb2, 0xad, 0x01, 0x0c, 0xb9, 0xe1, 0xf6 };
static const unsigned char encrDataIntegrityHmacValueBlockKey[8]	= { 0xa0, 0x67, 0x7f, 0x02, 0xb2, 0x2c, 0x84, 0x33 };

#define PADDING_SIZE 16

using namespace CryptoPP;

class _buf
{
public:
	unsigned char	*ptr;
	int				size;
//-----------------------------------------------------------------------------------
	_buf()													: ptr(NULL), size(0), 	bDelete(true){}
	_buf(int sz, bool zero_set = false)						: ptr(NULL), size(0), 	bDelete(true)
	{
		ptr = new unsigned char [sz]; 
		size = sz; 
		
		if (ptr && zero_set)
		{
			memset(ptr, 0, size);
		}
	}
	_buf(unsigned char * buf, int sz, bool bDelete_ = true ): ptr(NULL), size(0), 	bDelete(true)
	{
		CreateCopy(buf, sz, bDelete_);
	}
	_buf(const std::string  & str)							: ptr(NULL), size(0), 	bDelete(true)
	{
		CreateCopy((unsigned char*)str.c_str(), str.length(), true);
	}
	_buf(const std::wstring & str)							: ptr(NULL), size(0), 	bDelete(true)
	{
		if (sizeof(wchar_t) == 4)
		{
			unsigned int nLength = str.length();
			
			size = nLength * 2; 
			ptr = new unsigned char [size]; 

			UTF16* pStrUtf16 = (UTF16*)		ptr;
			UTF32 *pStrUtf32 = (UTF32 *)	str.c_str();

            UTF16 *pStrUtf16_Conv		= pStrUtf16;
            const UTF32 *pStrUtf32_Conv = pStrUtf32;

            ConversionResult eUnicodeConversionResult = ConvertUTF32toUTF16 (&pStrUtf32_Conv, &pStrUtf32[nLength]
                    , &pStrUtf16_Conv, &pStrUtf16 [nLength], strictConversion);

            if (conversionOK != eUnicodeConversionResult)
            {
            }
		}
		else
		{
			CreateCopy((unsigned char*)str.c_str(), str.length() * 2, true);
		}
	}
	
	virtual ~_buf()	
	{
		Clear();
	}
	_buf& operator=(const _buf& oSrc)
	{
		Clear();

		size	= oSrc.size;
		ptr		= new unsigned char [oSrc.size];
		memcpy(ptr, oSrc.ptr, oSrc.size);
		bDelete = true;

		return *this;
	}

	void Clear()
	{
		if (bDelete && ptr) delete []ptr; 
		ptr = NULL; size = 0;

		bDelete = true;
	}
//----------------------------------------------------------------------
private:
	bool bDelete;
	void CreateCopy(unsigned char* buf, int sz, bool bDelete_)
	{
		Clear();
		bDelete = bDelete_;
		if (bDelete)
		{
			ptr = new unsigned char [sz]; size = sz;
			memcpy(ptr, buf , sz);
		}
		else
		{
			ptr = buf; size = sz; 
		}

	}
};

bool operator==(const _buf& oBuf1, const _buf& oBuf2)
{
	if (!oBuf1.ptr || !oBuf2.ptr) return false;

	return 0 == memcmp(oBuf1.ptr, oBuf2.ptr, (std::min)(oBuf1.size, oBuf2.size));
}

void CorrectHashSize(_buf & hashBuf, int size, unsigned char padding)
{
	if (hashBuf.size < size)
	{
		unsigned char *newPtr = new unsigned char[size];
		memset(newPtr, padding, size);
		memcpy(newPtr, hashBuf.ptr, hashBuf.size);
		
		hashBuf.Clear();
		
		hashBuf.ptr		= newPtr;
		hashBuf.size	= size;
	}
	else if (hashBuf.size > size)
	{
		hashBuf.size = size;
	}
}
_buf Hmac(_buf &  buf, CRYPT_METHOD::_hashAlgorithm algorithm, std::string & plain)
{
	std::string mac;
	if (algorithm == CRYPT_METHOD::SHA1)
	{
        HMAC<SHA1> hmac(buf.ptr, buf.size);
        StringSource(plain, true,
                    new HashFilter(hmac,
                        new StringSink(mac)
					) // HashFilter      
                ); // StringSource

	}
	else if (algorithm == CRYPT_METHOD::SHA256)
	{
        HMAC<SHA256> hmac(buf.ptr, buf.size);
        StringSource(plain, true,
                    new HashFilter(hmac,
                        new StringSink(mac)
					) // HashFilter      
				); // StringSource

	}
	else if (algorithm == CRYPT_METHOD::SHA512)
	{
        HMAC<SHA512> hmac(buf.ptr, buf.size);
		
        StringSource(plain, true,
                    new HashFilter(hmac,
                        new StringSink(mac)
					) // HashFilter      
				); // StringSource

	}
	
	return _buf(mac);
}


_buf HashAppend(_buf &  hashBuf, _buf & block, CRYPT_METHOD::_hashAlgorithm algorithm)
{//todooo переделать
	if (algorithm == CRYPT_METHOD::MD5)
	{		
		MD5 hash;

		if (hashBuf.ptr && hashBuf.size > 0)	hash.Update( hashBuf.ptr, hashBuf.size);
		if (block.ptr	&& block.size > 0)		hash.Update( block.ptr	, block.size);

		SecByteBlock buffer(hash.DigestSize());
		hash.Final(buffer);

		return _buf(buffer.BytePtr(), buffer.SizeInBytes());	
	}
	else if (algorithm == CRYPT_METHOD::SHA1)
	{
		SHA1 hash;

		if (hashBuf.ptr && hashBuf.size > 0)	hash.Update( hashBuf.ptr, hashBuf.size);
		if (block.ptr	&& block.size > 0)		hash.Update( block.ptr	, block.size);

		SecByteBlock buffer(hash.DigestSize());
		hash.Final(buffer);

		return _buf(buffer.BytePtr(), buffer.SizeInBytes());
	}
	else if (algorithm == CRYPT_METHOD::SHA256)
	{
		SHA256 hash;

		if (hashBuf.ptr && hashBuf.size > 0)	hash.Update( hashBuf.ptr, hashBuf.size);
		if (block.ptr	&& block.size > 0)		hash.Update( block.ptr	, block.size);

		SecByteBlock buffer(hash.DigestSize());
		hash.Final(buffer);

		return _buf(buffer.BytePtr(), buffer.SizeInBytes());
	}
	else if (algorithm == CRYPT_METHOD::SHA512)
	{
		SHA512 hash;

		if (hashBuf.ptr && hashBuf.size > 0)	hash.Update( hashBuf.ptr, hashBuf.size);
		if (block.ptr	&& block.size > 0)		hash.Update( block.ptr	, block.size);

		SecByteBlock buffer(hash.DigestSize());
		hash.Final(buffer);

		return _buf(buffer.BytePtr(), buffer.SizeInBytes());
	}
	else
		return _buf();
}

_buf GenerateAgileKey(_buf & salt, _buf & password, _buf & blockKey, int hashSize, int spin, CRYPT_METHOD::_hashAlgorithm algorithm)
{
	//if (hashSize < 8)
	//	blockKey = 8;

	_buf pHashBuf = HashAppend(salt, password, algorithm);

	for (int i = 0; i < spin; i++)
	{
        _buf iterator((unsigned char*)&i, 4, false);
        pHashBuf = HashAppend(iterator, pHashBuf, algorithm);
	}

	pHashBuf = HashAppend(pHashBuf, blockKey, algorithm);

	CorrectHashSize(pHashBuf, hashSize, 0x36);

	return _buf(pHashBuf.ptr, pHashBuf.size);
}
_buf GenerateOdfKey(_buf & pSalt, _buf & pPassword, int keySize, int spin, CRYPT_METHOD::_hashAlgorithm algorithm)
{
	_buf pKey		(keySize);
	_buf empty		(NULL, 0, false);

	_buf pPassword_hash = HashAppend(pPassword, empty, algorithm);

	PKCS5_PBKDF2_HMAC<SHA1> pbkdf;
		
	pbkdf.DeriveKey(pKey.ptr, pKey.size, 0, pPassword_hash.ptr, pPassword_hash.size, pSalt.ptr, pSalt.size, spin);

	return _buf(pKey.ptr, pKey.size);
}
    
_buf GenerateHashKey(_buf & salt, _buf & password, int hashSize, int keySize, int spin, CRYPT_METHOD::_hashAlgorithm algorithm, int block_index = 0)
{
	_buf block	((unsigned char*)&block_index, 4, false);

	_buf empty	(NULL, 0, false);
	_buf pHashBuf = HashAppend(salt, password, algorithm);
		
	for (int i = 0; i < spin; i++)
	{
        _buf iterator((unsigned char*)&i, 4, false);
        pHashBuf = HashAppend(iterator, pHashBuf, algorithm);
	}

    pHashBuf = HashAppend( pHashBuf, block, algorithm);
	
	if (spin == 0)
	{
		return _buf(pHashBuf.ptr, pHashBuf.size);
	}

	_buf derivedKey1(64);
	memset (derivedKey1.ptr, 0x36, 64);
	for (int i = 0; i < pHashBuf.size; i++)
	{
		derivedKey1.ptr[i] ^= pHashBuf.ptr[i];
	}

	_buf derivedKey2(64);
	memset (derivedKey2.ptr, 0x5c, 64);
	for (int i = 0; i < pHashBuf.size; i++)
	{
		derivedKey2.ptr[i] ^= pHashBuf.ptr[i];
	}

	_buf pHashBuf1 = HashAppend(derivedKey1, empty, algorithm);
	_buf pHashBuf2 = HashAppend(derivedKey2, empty, algorithm);

	_buf pHashBuf3(2 * hashSize);
	
	memcpy(pHashBuf3.ptr + 0,			pHashBuf1.ptr, hashSize);
	memcpy(pHashBuf3.ptr + hashSize,	pHashBuf2.ptr, hashSize);

	CorrectHashSize(pHashBuf3, keySize, 0);
	
	if (keySize == 5)
		CorrectHashSize(pHashBuf3, 16, 0); //40-bit crypt key !!!				

	return _buf(pHashBuf3.ptr, pHashBuf3.size);
}
ARC4::Decryption rc4Decryption; // todooo -> in impl
ARC4::Encryption rc4Encryption;

bool EncryptCipher(_buf & key, _buf & iv, _buf & data_inp, _buf & data_out, CRYPT_METHOD::_cipherAlgorithm algorithm,
				    StreamTransformationFilter::BlockPaddingScheme padding = StreamTransformationFilter::PKCS_PADDING)
{	
	if (algorithm == CRYPT_METHOD::XOR)
	{
		return false;
	}
	else if (algorithm == CRYPT_METHOD::RC4)
	{
		if (!data_out.ptr)
		{
			data_out = _buf(data_inp.size);
		}
		
		rc4Encryption.ProcessData(data_out.ptr, data_inp.ptr, data_inp.size);

	}
	else if (algorithm == CRYPT_METHOD::Blowfish_CFB)
	{
		CFB_Mode<Blowfish>::Encryption encryption(key.ptr, key.size, iv.ptr);
		encryption.ProcessData(data_out.ptr, data_inp.ptr, data_inp.size);
	}
	else if (algorithm == CRYPT_METHOD::DES_CBC)
	{
		if (!iv.ptr)
		{
			iv = _buf(16, true);
		}
		DES::Encryption desEncryption(key.ptr, key.size == 7 ? 8 : key.size);
		StreamTransformation *modeEncryption = new CipherModeFinalTemplate_ExternalCipher<CBC_Encryption>(desEncryption, iv.ptr ); 
		if (!data_out.ptr)
		{
			data_out = _buf(data_inp.size);
		}
		StreamTransformationFilter stfEncryption(*modeEncryption, new ArraySink( data_out.ptr, data_out.size), padding);
	 
		stfEncryption.Put( data_inp.ptr, data_inp.size );
		stfEncryption.MessageEnd();

		delete modeEncryption;
	}
	else if (algorithm == CRYPT_METHOD::DES_ECB)
	{
		if (!iv.ptr)
		{
			iv = _buf(16, true);
		}
		DES::Encryption desEncryption(key.ptr, key.size == 7 ? 8 : key.size);
		StreamTransformation *modeEncryption = new CipherModeFinalTemplate_ExternalCipher<ECB_OneWay>(desEncryption, iv.ptr ); 
		if (!data_out.ptr)
		{
			data_out = _buf(data_inp.size);
		}
		StreamTransformationFilter stfEncryption(*modeEncryption, new ArraySink( data_out.ptr, data_out.size), padding);
	 
		stfEncryption.Put( data_inp.ptr, data_inp.size );
		stfEncryption.MessageEnd();

		delete modeEncryption;
	}
	else //AES
	{
		StreamTransformation *modeEncryption = NULL;
		AES::Encryption aesEncryption(key.ptr, key.size);

		switch(algorithm)
		{
		case CRYPT_METHOD::AES_ECB:
			 modeEncryption = new CipherModeFinalTemplate_ExternalCipher<ECB_OneWay>(aesEncryption, iv.ptr ); 
			 break;
		case CRYPT_METHOD::AES_CBC:
			 modeEncryption = new CipherModeFinalTemplate_ExternalCipher<CBC_Encryption>(aesEncryption, iv.ptr ); 
			 break;
//		case CRYPT_METHOD::AES_CFB:
//#if defined(_IOS)
//            return false;
//#else
//			modeEncryption = new CFB_Mode_ExternalCipher::Encryption(aesEncryption, iv.ptr );
//#endif
			break;
		}
	
		if (!modeEncryption) return false;
		    
		if (!data_out.ptr)
		{
			data_out = _buf(data_inp.size);
		}
		StreamTransformationFilter stfEncryption(*modeEncryption, new ArraySink( data_out.ptr, data_out.size), padding);
	 
		stfEncryption.Put( data_inp.ptr, data_inp.size );
		stfEncryption.MessageEnd();

		delete modeEncryption;
	}

	return true;
}

bool DecryptCipher(_buf & key, _buf & iv, _buf & data_inp, _buf & data_out,  CRYPT_METHOD::_cipherAlgorithm algorithm, 
				  StreamTransformationFilter::BlockPaddingScheme padding = StreamTransformationFilter::NO_PADDING)
{	
	if (!data_out.ptr)
	{
		data_out = _buf(data_inp.size);
	}

	if (algorithm == CRYPT_METHOD::XOR)
	{
		return false;
	}
	else if (algorithm == CRYPT_METHOD::RC4)
	{
		rc4Decryption.ProcessData(data_out.ptr, data_inp.ptr, data_inp.size);
	}
	else if (algorithm == CRYPT_METHOD::Blowfish_CFB)
	{
		CFB_Mode<Blowfish>::Decryption decryption(key.ptr, key.size, iv.ptr);
		decryption.ProcessData(data_out.ptr, data_inp.ptr, data_inp.size);
	}
	else if (algorithm == CRYPT_METHOD::DES_CBC)
	{
		if (!iv.ptr)
		{
			iv = _buf(16, true);
		}
		DES::Decryption desDecryption(key.ptr, key.size == 7 ? 8 : key.size);
		StreamTransformation *modeDecryption = new CipherModeFinalTemplate_ExternalCipher<CBC_Decryption>(desDecryption, iv.ptr );
		StreamTransformationFilter stfDecryptor(*modeDecryption, new ArraySink( data_out.ptr, data_out.size), padding);
	 
		stfDecryptor.Put( data_inp.ptr, data_inp.size );
		stfDecryptor.MessageEnd();

		delete modeDecryption;
	}
	else if (algorithm == CRYPT_METHOD::DES_ECB)
	{
		if (!iv.ptr)
		{
			iv = _buf(16, true);
		}
		DES::Decryption desDecryption(key.ptr, key.size == 7 ? 8 : key.size);
		StreamTransformation *modeDecryption = new CipherModeFinalTemplate_ExternalCipher<ECB_OneWay>(desDecryption, iv.ptr );
		StreamTransformationFilter stfDecryptor(*modeDecryption, new ArraySink( data_out.ptr, data_out.size), padding);
	 
		stfDecryptor.Put( data_inp.ptr, data_inp.size );
		stfDecryptor.MessageEnd();

		delete modeDecryption;
	}
	else //AES
	{
		StreamTransformation *modeDecryption = NULL;
		AES::Decryption aesDecryption(key.ptr, key.size);

		switch(algorithm)
		{
		case CRYPT_METHOD::AES_ECB:
			 modeDecryption = new CipherModeFinalTemplate_ExternalCipher<ECB_OneWay>(aesDecryption, iv.ptr ); 
			 break;
		case CRYPT_METHOD::AES_CBC:
			 modeDecryption = new CipherModeFinalTemplate_ExternalCipher<CBC_Decryption>(aesDecryption, iv.ptr );
			 break;
//		case CRYPT_METHOD::AES_CFB:
//#if defined(_IOS)
//             return false;
//#else
//			 modeDecryption = new CFB_Mode_ExternalCipher::Decryption(aesDecryption, iv.ptr );
//#endif
		}
	
		if (!modeDecryption) return false;
		    
		StreamTransformationFilter stfDecryptor(*modeDecryption, new ArraySink( data_out.ptr, data_out.size), padding);
	 
		stfDecryptor.Put( data_inp.ptr, data_inp.size );
		stfDecryptor.MessageEnd();

		delete modeDecryption;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------------------------------
namespace CRYPT
{
//-----------------------------------------------------------------------------------------------------------
ECMADecryptor::ECMADecryptor()
{
	bVerify = false;
}

ECMADecryptor::~ECMADecryptor()
{
}

bool ECMADecryptor::SetPassword(std::wstring _password)
{
	bVerify		= false;
	password	= _password;

	if (password.empty()) return false;
	
	_buf pPassword		(password);
	_buf pSalt			(cryptData.saltValue);
	_buf empty			(NULL, 0, false);

	_buf pEncVerInput	(cryptData.encryptedVerifierInput);
	_buf pEncVerValue	(cryptData.encryptedVerifierValue);
	
	if (cryptData.bAgile)
	{	
		_buf pInputBlockKey ((unsigned char*)encrVerifierHashInputBlockKey, 8);
		_buf pValueBlockKey ((unsigned char*)encrVerifierHashValueBlockKey, 8);		
		
		_buf verifierInputKey	= GenerateAgileKey( pSalt, pPassword, pInputBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm );
		_buf verifierHashKey	= GenerateAgileKey( pSalt, pPassword, pValueBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm );
		
		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Decryption.SetKey(verifierInputKey.ptr, cryptData.keySize);
		}
//--------------------------------------------
		_buf decryptedVerifierHashInputBytes;		
		DecryptCipher(verifierInputKey, pSalt, pEncVerInput, decryptedVerifierHashInputBytes, cryptData.cipherAlgorithm);
//--------------------------------------------
		_buf hashBuf = HashAppend(decryptedVerifierHashInputBytes, empty, cryptData.hashAlgorithm);
//--------------------------------------------
		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Decryption.SetKey(verifierHashKey.ptr, cryptData.keySize);
		}		
		_buf decryptedVerifierHashBytes;		
		DecryptCipher(verifierHashKey, pSalt, pEncVerValue, decryptedVerifierHashBytes, cryptData.cipherAlgorithm);
//--------------------------------------------
		bVerify	= (decryptedVerifierHashBytes==hashBuf);
	}
	else
	{
		_buf verifierKey = GenerateHashKey(pSalt, pPassword, cryptData.hashSize, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);		

		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Decryption.SetKey(verifierKey.ptr, cryptData.keySize);
		}
//--------------------------------------------
		_buf decryptedVerifierHashInputBytes;		
		DecryptCipher(verifierKey, pSalt, pEncVerInput, decryptedVerifierHashInputBytes, cryptData.cipherAlgorithm);

	//--------------------------------------------
		_buf hashBuf = HashAppend(decryptedVerifierHashInputBytes, empty, cryptData.hashAlgorithm);
	//--------------------------------------------
		
		_buf decryptedVerifierHashBytes;		
		DecryptCipher(verifierKey, pSalt, pEncVerValue, decryptedVerifierHashBytes, cryptData.cipherAlgorithm);

		bVerify	= (decryptedVerifierHashBytes == hashBuf);
	}
	return bVerify;
}

bool ECMADecryptor::IsVerify()
{
	return bVerify;
}

void ECMADecryptor::SetCryptData(_ecmaCryptData	& data)
{
	cryptData = data;
}
void ECMADecryptor::Decrypt(char* data	, const size_t size, const unsigned long start_iv_block)
{
	if (!bVerify) return;
	
	unsigned char* data_out = NULL;
	Decrypt((unsigned char*)data, size, data_out, start_iv_block);
	
	if (data_out)
	{
		memcpy(data, data_out, size);
		delete []data_out;
	}
}
void ECMADecryptor::Decrypt(char* data	, const size_t size, const unsigned long stream_pos, const size_t block_size)
{
	if (!bVerify) return;
//rc4 only
	if (cryptData.cipherAlgorithm != CRYPT_METHOD::RC4) return;
	
	unsigned char* data_out = new unsigned char[size];

	unsigned char*			pnCurrDest	= data_out;
	const unsigned char*	pnCurrSrc	= (unsigned char* )data;
	long					nCurrPos	= stream_pos;
	unsigned short			nBytesLeft	= size;
	
	while(nBytesLeft > 0)
	{
		// initialize codec for current stream position

		int block_index = (nCurrPos / block_size);
		{
			_buf pPassword	(password);
			_buf pSalt		(cryptData.saltValue);			
			
			_buf hashKey = GenerateHashKey(pSalt, pPassword, cryptData.hashSize, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm, block_index);
			
			rc4Decryption.SetKey(hashKey.ptr, cryptData.keySize);
		}

		const long offset = nCurrPos % block_size;
		{//skip
			unsigned char pnDummy[ 1024 ];

			size_t nBytesLeft = offset;
			bool bResult = true;
			while(bResult && (nBytesLeft > 0))
			{
				size_t nBlockLen = nBytesLeft < sizeof(pnDummy) ? nBytesLeft : sizeof(pnDummy);
				rc4Decryption.ProcessData(pnDummy, pnDummy, nBlockLen);
				nBytesLeft -= nBlockLen;
			}
		}
		unsigned short nBlockLeft = static_cast<unsigned short>(block_size - offset);
		unsigned short nDecBytes = nBytesLeft < nBlockLeft ? nBytesLeft : nBlockLeft;
		
		rc4Decryption.ProcessData(pnCurrDest, pnCurrSrc, static_cast<int>(nDecBytes));

		pnCurrDest	+= nDecBytes;
		pnCurrSrc	+= nDecBytes;
		nCurrPos	+= nDecBytes;

		nBytesLeft = nBytesLeft - nDecBytes;
	}	
	if (data_out)
	{
		memcpy(data, data_out, size);
		delete []data_out;
	}
}
bool ECMADecryptor::CheckDataIntegrity(unsigned char* data, int  size)
{
	if (cryptData.bAgile == false) return true;

	_buf pBlockKey		((unsigned char*)encrKeyValueBlockKey, 8);	
	_buf pBlockHmacKey	((unsigned char*)encrDataIntegritySaltBlockKey, 8);	
	_buf pBlockHmacValue((unsigned char*)encrDataIntegrityHmacValueBlockKey, 8);	
	
	_buf pPassword		(password);
	_buf pSalt			(cryptData.saltValue);
	_buf empty			(NULL, 0, false);

	_buf pDataSalt		(cryptData.dataSaltValue);
	_buf pKeyValue		(cryptData.encryptedKeyValue);
	_buf pEncHmacKey	(cryptData.encryptedHmacKey);
	_buf pEncHmacValue	(cryptData.encryptedHmacValue);

	_buf agileKey = GenerateAgileKey( pSalt, pPassword, pBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);  

	_buf secretKey;
	DecryptCipher( agileKey, pSalt, pKeyValue, secretKey, cryptData.cipherAlgorithm);  
//----			
	_buf iv1 = HashAppend(pDataSalt, pBlockHmacKey, cryptData.dataHashAlgorithm);
	CorrectHashSize(iv1, cryptData.dataBlockSize, 0x36);
	
	_buf iv2 = HashAppend(pDataSalt, pBlockHmacValue, cryptData.dataHashAlgorithm);
	CorrectHashSize(iv2, cryptData.dataBlockSize, 0x36);

	_buf pSaltHmac;
	DecryptCipher(secretKey,  iv1, pEncHmacKey, pSaltHmac, cryptData.dataCipherAlgorithm);
	
	_buf expected;
	DecryptCipher(secretKey,  iv2, pEncHmacValue, expected, cryptData.dataCipherAlgorithm);

	std::string sData((char*)data, size);
	_buf hmac = Hmac(pSaltHmac, cryptData.dataHashAlgorithm, sData);
		
	return (hmac == expected);
}

void ECMADecryptor::Decrypt(unsigned char* data_inp, int  size, unsigned char*& data_out, unsigned long start_iv_block)
{
	data_out = new unsigned char[size];
	
	_buf pPassword	(password);
	_buf pSalt		(cryptData.saltValue);
	_buf empty		(NULL, 0, false);

	if (cryptData.bAgile)
	{	
		_buf pBlockKey	((unsigned char*)encrKeyValueBlockKey, 8);	
		_buf pDataSalt	(cryptData.dataSaltValue);
		_buf pKeyValue	(cryptData.encryptedKeyValue);

		_buf agileKey = GenerateAgileKey( pSalt, pPassword, pBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);  

		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Decryption.SetKey(agileKey.ptr, cryptData.keySize);
		}
		_buf pDecryptedKey;
		DecryptCipher( agileKey, pSalt, pKeyValue, pDecryptedKey, cryptData.cipherAlgorithm);  

		_buf iv(cryptData.blockSize, true);

		int i = start_iv_block, sz = 4096, pos = 0;//aes block size = 4096

		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Decryption.SetKey(pDecryptedKey.ptr, cryptData.keySize);
		}
		while (pos < size)
		{
			if (pos + sz > size) 
				sz = size - pos;
			
			_buf pIndex((unsigned char*)&i, 4);
			iv = HashAppend(pDataSalt, pIndex, cryptData.dataHashAlgorithm);

			CorrectHashSize(iv, cryptData.dataBlockSize, 0x36);
			
			_buf pInp(data_inp + pos, sz, false);
			_buf pOut(data_out + pos, sz, false);

			DecryptCipher(pDecryptedKey,  iv, pInp, pOut, cryptData.cipherAlgorithm);
			
			pos += sz; i++;
		}
	}
	else
	{				
		_buf hashKey = GenerateHashKey(pSalt, pPassword, cryptData.hashSize, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm, start_iv_block);
		
		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{			
			rc4Decryption.SetKey(hashKey.ptr, cryptData.keySize);
		}
	
		_buf pInp(data_inp, size, false);
		_buf pOut(data_out, size, false);

		DecryptCipher(hashKey, empty, pInp, pOut, cryptData.cipherAlgorithm);
	}
}
//-----------------------------------------------------------------------------------------------------------
void odfWriteProtect::SetPassword (const std::wstring &password_)
{
	password = password_;
}
void odfWriteProtect::SetProtectKey (const std::string &key)
{
	protect_key = key;
}
void odfWriteProtect::SetProtectAlgorithm (const CRYPT_METHOD::_hashAlgorithm &alg)
{
	hash = alg;
}
void odfWriteProtect::Generate()
{
	_buf pPassword	(password);
	_buf empty		(NULL, 0, false);		

	_buf pHashTest = HashAppend(empty, pPassword, hash);
		
	protect_key = std::string((char*)pHashTest.ptr, pHashTest.size);
}
bool odfWriteProtect::Verify()
{
	_buf pPassword	(password);
	_buf empty		(NULL, 0, false);		
	_buf pHash		(protect_key);

	_buf pHashTest = HashAppend(empty, pPassword, hash);
		
	return (pHashTest == pHash);
}
//----------------------------------------------------------------------------------------------------------
void ECMAWriteProtect::SetPassword (const std::wstring &password_)
{
	password = password_;
}
void ECMAWriteProtect::SetCryptData(_ecmaWriteProtectData &_data)
{
	data = _data;
}
void ECMAWriteProtect::GetCryptData(_ecmaWriteProtectData &_data)
{
	_data = data;
}
void ECMAWriteProtect::Generate()
{
	//сгенерить соль
	RandomPool prng;
	SecByteBlock seed_salt(16);
	OS_GenerateRandomBlock(false, seed_salt, seed_salt.size());
	if (prng.CanIncorporateEntropy())
	{
		prng.IncorporateEntropy(seed_salt, seed_salt.size());
	}
	_buf pPassword	(password);
	_buf empty		(NULL, 0, false);		
	_buf pSalt		(seed_salt.data(), seed_salt.size());

	_buf pHashBuf = HashAppend(pSalt, pPassword, data.hashAlgorithm);
		
	for (_UINT32 i = 0; i < data.spinCount; i++)
	{
        _buf iterator((unsigned char*)&i, 4, false);
        pHashBuf = HashAppend(pHashBuf, iterator, data.hashAlgorithm);
	}

	data.saltValue = std::string((char*)pSalt.ptr, pSalt.size);
	data.hashValue = std::string((char*)pHashBuf.ptr, pHashBuf.size);
}
bool ECMAWriteProtect::VerifyWrike()
{
	std::string p = std::string(password.begin(), password.end());

    unsigned int wPasswordHash = 0;
  
    const char* pch = p.c_str() + p.length();
    while (pch-- != p.c_str())
    {
        wPasswordHash = ((wPasswordHash >> 14) & 0x01) | 
                        ((wPasswordHash << 1) & 0x7fff);
        wPasswordHash ^= *pch;
    }
 
    wPasswordHash = ((wPasswordHash >> 14) & 0x01) | 
                    ((wPasswordHash << 1) & 0x7fff);
 
    wPasswordHash ^= (0x8000 | ('N' << 8) | 'K');
    wPasswordHash ^= p.length();

	std::stringstream sstream;
	sstream << boost::format("%4.4X") % wPasswordHash;
	std::string sPasswordHash(sstream.str());

	return data.hashValue == sPasswordHash;
}
bool ECMAWriteProtect::Verify()
{
	_buf pPassword	(password);
	_buf empty		(NULL, 0, false);		
	_buf pSalt		(data.saltValue);
	_buf pHash		(data.hashValue);

	_buf pHashTest = HashAppend(pSalt, pPassword, data.hashAlgorithm);
		
	for (_UINT32 i = 0; i < data.spinCount; i++)
	{
        _buf iterator((unsigned char*)&i, 4, false);
        pHashTest = HashAppend(pHashTest, iterator, data.hashAlgorithm);
	}
	return (pHashTest == pHash);
}
//-----------------------------------------------------------------------------------------------------------
ECMAEncryptor::ECMAEncryptor()
{
}

void ECMAEncryptor::SetPassword(std::wstring _password)
{
	password = _password;

//---------
    RandomPool prng;
	
	//сгенерить соль
    SecByteBlock seed_salt(cryptData.saltSize);
    OS_GenerateRandomBlock(false, seed_salt, seed_salt.size());
	if (prng.CanIncorporateEntropy())
	{
		prng.IncorporateEntropy(seed_salt, seed_salt.size());
	}
    SecByteBlock seed_datasalt(cryptData.saltSize);
    OS_GenerateRandomBlock(false, seed_datasalt, seed_datasalt.size());
	if (prng.CanIncorporateEntropy())
	{
		prng.IncorporateEntropy(seed_datasalt, seed_datasalt.size());
	}
	//сгенерить ключ
    SecByteBlock seed_key(cryptData.keySize);
    OS_GenerateRandomBlock(false, seed_key, seed_key.size());
	if (prng.CanIncorporateEntropy())
	{
		prng.IncorporateEntropy(seed_key, seed_key.size());
	}
	//сгенерить проверочный
    SecByteBlock seed_verify(cryptData.saltSize);
    OS_GenerateRandomBlock(false, seed_verify, seed_verify.size());
	if (prng.CanIncorporateEntropy())
	{
		prng.IncorporateEntropy(seed_verify, seed_verify.size());
	}
//---------
	_buf pPassword		(password);
	_buf empty			(NULL, 0, false);
		
	_buf pSalt			(seed_salt.data(), seed_salt.size());
	_buf pDataSalt		(seed_datasalt.data(), seed_datasalt.size());
	_buf pDecryptedKey	(seed_key.data(), seed_key.size());
	
//------------------------------------------------------------------------------------------------
	if (cryptData.bAgile == true)
	{
		_buf pBlockKey		((unsigned char*)encrKeyValueBlockKey, 8);	
		_buf pInputBlockKey ((unsigned char*)encrVerifierHashInputBlockKey, 8);
		_buf pValueBlockKey ((unsigned char*)encrVerifierHashValueBlockKey, 8);
		
		_buf agileKey = GenerateAgileKey( pSalt, pPassword, pBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);  

		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Encryption.SetKey(agileKey.ptr, cryptData.keySize);
		}
		_buf pKeyValue;
		EncryptCipher( agileKey, pSalt, pDecryptedKey, pKeyValue, cryptData.cipherAlgorithm);  
		
	//--------------------------------------------
		_buf decryptedVerifierHashInputBytes(seed_verify.data(), seed_verify.size());
		_buf verifierInputKey = GenerateAgileKey( pSalt, pPassword, pInputBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm );
		
		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Encryption.SetKey(verifierInputKey.ptr, cryptData.keySize);
		}
		_buf pEncVerInput;
		EncryptCipher( verifierInputKey, pSalt, decryptedVerifierHashInputBytes, pEncVerInput, cryptData.cipherAlgorithm);  
	//--------------------------------------------

		_buf decryptedVerifierHashBytes = HashAppend(decryptedVerifierHashInputBytes, empty, cryptData.hashAlgorithm);
		_buf verifierHashKey = GenerateAgileKey(pSalt, pPassword, pValueBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);
		
		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Encryption.SetKey(verifierHashKey.ptr, cryptData.keySize);
		}
		else if (decryptedVerifierHashBytes.size % PADDING_SIZE != 0) 
		{
			CorrectHashSize(decryptedVerifierHashBytes, (decryptedVerifierHashBytes.size / PADDING_SIZE + 1) * PADDING_SIZE, 0);
		}
		_buf pEncVerValue;
		EncryptCipher( verifierHashKey, pSalt, decryptedVerifierHashBytes, pEncVerValue, cryptData.cipherAlgorithm);  

		cryptData.saltValue					= std::string((char*)pSalt.ptr, pSalt.size);
		cryptData.dataSaltValue				= std::string((char*)pDataSalt.ptr, pDataSalt.size);
		cryptData.encryptedKeyValue			= std::string((char*)pKeyValue.ptr, pKeyValue.size);
		cryptData.encryptedVerifierInput	= std::string((char*)pEncVerInput.ptr, pEncVerInput.size);
		cryptData.encryptedVerifierValue	= std::string((char*)pEncVerValue.ptr, pEncVerValue.size);
	}
	else
	{
		_buf verifierKey = GenerateHashKey(pSalt, pPassword, cryptData.hashSize, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);		

		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{			
			rc4Encryption.SetKey(verifierKey.ptr, cryptData.keySize);
		}	
		_buf decryptedVerInput(seed_verify.data(), seed_verify.size());
		
		_buf pEncVerInput;
		EncryptCipher( verifierKey, pSalt, decryptedVerInput, pEncVerInput, cryptData.cipherAlgorithm);  
		
		_buf hashBuf = HashAppend(decryptedVerInput, empty, cryptData.hashAlgorithm);
		if (cryptData.cipherAlgorithm != CRYPT_METHOD::RC4 && hashBuf.size % PADDING_SIZE != 0) 
		{
			CorrectHashSize(hashBuf, (hashBuf.size / PADDING_SIZE + 1) * PADDING_SIZE, 0);
		}
		
		_buf pEncVerValue;
		EncryptCipher( verifierKey, pSalt, hashBuf, pEncVerValue, cryptData.cipherAlgorithm);  

		cryptData.saltValue					= std::string((char*)pSalt.ptr, pSalt.size);
		cryptData.encryptedVerifierInput	= std::string((char*)pEncVerInput.ptr, pEncVerInput.size);
		cryptData.encryptedVerifierValue	= std::string((char*)pEncVerValue.ptr, pEncVerValue.size);
	}
}

void ECMAEncryptor::SetCryptData(_ecmaCryptData	& data)
{
	cryptData = data;
}

void ECMAEncryptor::GetCryptData(_ecmaCryptData &data)
{
	data = cryptData;
}

void ECMAEncryptor::UpdateDataIntegrity(unsigned char* data, int  size)
{
	if (cryptData.bAgile == false) return;

	_buf pBlockKey		((unsigned char*)encrKeyValueBlockKey, 8);	
	_buf pBlockHmacKey	((unsigned char*)encrDataIntegritySaltBlockKey, 8);	
	_buf pBlockHmacValue((unsigned char*)encrDataIntegrityHmacValueBlockKey, 8);	
	
	_buf pPassword		(password);
	_buf pSalt			(cryptData.saltValue);
	_buf empty			(NULL, 0, false);

	_buf pDataSalt		(cryptData.dataSaltValue);
	_buf pKeyValue		(cryptData.encryptedKeyValue);
	
//----
	_buf agileKey = GenerateAgileKey( pSalt, pPassword, pBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);  

	if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
	{
		rc4Encryption.SetKey(agileKey.ptr, cryptData.keySize);
	}

	_buf secretKey;
	DecryptCipher( agileKey, pSalt, pKeyValue, secretKey, cryptData.cipherAlgorithm);  
	
	_buf iv1 = HashAppend(pDataSalt, pBlockHmacKey, cryptData.hashAlgorithm);
	CorrectHashSize(iv1, cryptData.blockSize, 0x36);
	
	_buf iv2 = HashAppend(pDataSalt, pBlockHmacValue, cryptData.hashAlgorithm);
	CorrectHashSize(iv2, cryptData.blockSize, 0x36);

//----
    RandomPool prng;
    SecByteBlock seed(cryptData.hashSize);

    OS_GenerateRandomBlock(false, seed, seed.size());
	prng.IncorporateEntropy(seed, seed.size());
	
	_buf pSaltHmac(seed.data(), seed.size());

	std::string sData((char*)data, size);
	_buf hmac = Hmac(pSaltHmac, cryptData.hashAlgorithm, sData);

	if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
	{
		rc4Encryption.SetKey(secretKey.ptr, cryptData.keySize);
	}
	if (pSaltHmac.size % PADDING_SIZE != 0) 
	{
		CorrectHashSize(pSaltHmac, (pSaltHmac.size / PADDING_SIZE + 1) * PADDING_SIZE, 0);
	}
	_buf pEncHmacKey;
	EncryptCipher(secretKey,  iv1, pSaltHmac, pEncHmacKey, cryptData.cipherAlgorithm);

	if (hmac.size % PADDING_SIZE != 0) 
	{
		CorrectHashSize(hmac, (hmac.size / PADDING_SIZE + 1) * PADDING_SIZE, 0);
	}
	_buf pEncHmacValue;
	EncryptCipher(secretKey,  iv2, hmac, pEncHmacValue, cryptData.cipherAlgorithm);

	cryptData.encryptedHmacKey		= std::string((char*)pEncHmacKey.ptr, pEncHmacKey.size);
	cryptData.encryptedHmacValue	= std::string((char*)pEncHmacValue.ptr, pEncHmacValue.size);
}

int ECMAEncryptor::Encrypt(unsigned char* data_inp_ptr, int size, unsigned char*& data_out_ptr)
{
	data_out_ptr = NULL;
	
	_buf pPassword	(password);
	_buf pSalt		(cryptData.saltValue);
	_buf empty		(NULL, 0, false);

	int size_out = size;

	if (cryptData.cipherAlgorithm != CRYPT_METHOD::RC4 && size_out % PADDING_SIZE != 0) 
		size_out = (size_out / PADDING_SIZE + 1) * PADDING_SIZE;
	
	data_out_ptr = new unsigned char[size_out +	PADDING_SIZE]; // real size + padding + size for realsize
	
	_UINT64 nSize = size;
	memcpy(data_out_ptr, (unsigned char*)&nSize, 8);

	unsigned char* data_inp = data_inp_ptr;
	unsigned char* data_out = data_out_ptr + 8;
	
	int result_size_out = 0;

	if (cryptData.bAgile)
	{	
		_buf pBlockKey	((unsigned char*)encrKeyValueBlockKey, 8);	
		_buf pDataSalt	(cryptData.dataSaltValue);
		_buf pKeyValue	(cryptData.encryptedKeyValue);
		
	//------------------------------------------------------------------------------------------------
		_buf agileKey = GenerateAgileKey( pSalt, pPassword, pBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);  

		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Decryption.SetKey(agileKey.ptr, cryptData.keySize);
		}
		_buf pDecryptedKey;
		DecryptCipher( agileKey, pSalt, pKeyValue, pDecryptedKey, cryptData.cipherAlgorithm);  
		
		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Encryption.SetKey(pDecryptedKey.ptr, cryptData.keySize);
		}
	//-------------------------------------------------------------------------------------------------
		_buf iv(cryptData.blockSize, true);

		int i = 0, sz = 4096, enc_size = 0;

		while (enc_size < size)
		{
			if (enc_size + sz > size) 
			{
				sz = size - enc_size;
			}
			
			_buf pIndex((unsigned char*)&i, 4);
			iv = HashAppend(pDataSalt, pIndex, cryptData.hashAlgorithm);

			CorrectHashSize(iv, cryptData.blockSize, 0x36);
			
			if (sz < 4096)
			{
				_buf pInp(4096); 
					memcpy(pInp.ptr, data_inp, sz );
					pInp.size = sz;
				_buf pOut(4096);

				EncryptCipher(pDecryptedKey,  iv, pInp, pOut, cryptData.cipherAlgorithm);
				
				if (sz % PADDING_SIZE != 0) 
					sz = (sz / PADDING_SIZE + 1) * PADDING_SIZE;
				
				memcpy(data_out, pOut.ptr, sz);

			}
			else
			{
				_buf pInp(data_inp, sz, false);
				_buf pOut(data_out, sz, false);
				
				EncryptCipher(pDecryptedKey,  iv, pInp, pOut, cryptData.cipherAlgorithm);
			}
			data_inp += sz;
			data_out += sz;

			enc_size += sz; i++;
		}
		result_size_out = enc_size + 8;
	}
	else
	{
		_buf hashKey = GenerateHashKey(pSalt, pPassword, cryptData.hashSize, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);
		
		if (cryptData.cipherAlgorithm == CRYPT_METHOD::RC4)
		{
			rc4Decryption.SetKey(hashKey.ptr, cryptData.keySize);
			rc4Encryption.SetKey(hashKey.ptr, cryptData.keySize);
		}
	
		_buf pInp(data_inp, size, false);
		_buf pOut(data_out, size_out, false);

		EncryptCipher(hashKey, empty, pInp, pOut, cryptData.cipherAlgorithm/*, StreamTransformationFilter::ZEROS_PADDING*/);	
	
		result_size_out = size_out + 8;
	}
	return result_size_out;
}
//-----------------------------------------------------------------------------------------------------------
ODFDecryptor::ODFDecryptor()
{
	bVerify = false;
}

void ODFDecryptor::SetCryptData(_odfCryptData	& data)
{
	cryptData = data;
}

bool ODFDecryptor::Decrypt(const std::wstring & wspassword, unsigned char* data_inp, int size_inp, unsigned char*& data_out, int &size_out)
{
	bVerify = false;

	std::string password = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wspassword);

	_buf pPassword	(password);
	_buf pSalt		(cryptData.saltValue);
	_buf ivi		(cryptData.initializationVector);
	_buf empty		(NULL, 0, false);
	
	_buf pKey = GenerateOdfKey(pSalt, pPassword, cryptData.keySize, cryptData.spinCount, cryptData.start_hashAlgorithm);

	_buf pInp(data_inp, size_inp, false);
	_buf pOut(size_inp);	

	bool bPadding = false;
	
	if ((cryptData.cipherAlgorithm == CRYPT_METHOD::AES_CBC) && (size_inp % PADDING_SIZE != 0))
	{
		bPadding = true;
	}

	if (false == DecryptCipher(pKey, ivi, pInp, pOut, cryptData.cipherAlgorithm, bPadding ? StreamTransformationFilter::W3C_PADDING : 
																							StreamTransformationFilter::NO_PADDING))
	{
		return false;
	}

	_buf pChecksum	(cryptData.checksum);
	
	_buf pOutLimit(pOut.ptr, (std::min)(cryptData.checksum_size, pOut.size), false);
	_buf pOutHash = HashAppend(pOutLimit, empty, cryptData.checksum_hashAlgorithm);

	bVerify	= (pChecksum == pOutHash);

	if (!bVerify && pOut.size < cryptData.checksum_size + 16)
	{
		for (int i = 0; !bVerify && i < pOut.size; i++)
		{
			_buf pOutLimit2(pOut.size - i, true);
			memcpy(pOutLimit2.ptr, pOut.ptr, pOut.size - i);

			_buf pOutHash2 = HashAppend(pOutLimit2, empty, cryptData.checksum_hashAlgorithm);

			bVerify	= (pChecksum == pOutHash2);
		}
	}

	try
	{
		if (bVerify)
		{
			data_out = new unsigned char[size_out + 1024];

			Inflator inflator(new ArraySink(data_out, size_out + 1024));

			inflator.Put(pOut.ptr, pOut.size);
			inflator.MessageEnd();

			bVerify = true;
		}
	}
	catch(...)
	{
		return false;
	}
	return bVerify;
}

//-----------------------------------------------------------------------------------------------------------
int ODFEncryptor::Encrypt (const std::wstring & wspassword, unsigned char* data, int  size, unsigned char*& data_out)
{
	std::string password = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wspassword);

	_buf pPassword	(password);
	_buf pSalt		(cryptData.saltValue);
	_buf ivi		(cryptData.initializationVector);
	_buf empty		(NULL, 0, false);

	_buf pKey = GenerateOdfKey(pSalt, pPassword, cryptData.keySize, cryptData.spinCount, cryptData.start_hashAlgorithm);
//---------
	_buf data_deflate(size + 1024); 
	ArraySink *sink = new ArraySink(data_deflate.ptr, data_deflate.size + 1024);

	Deflator deflator(sink, Deflator::DEFAULT_DEFLATE_LEVEL, Deflator::DEFAULT_LOG2_WINDOW_SIZE, true);
	deflator.Put2(data, size, 1, true);
	data_deflate.size = sink->TotalPutLength();
//---------	
	_buf pOutLimit(data_deflate.ptr, (std::min)(cryptData.checksum_size, data_deflate.size), false);
	_buf pChecksum = HashAppend(pOutLimit, empty, cryptData.checksum_hashAlgorithm);

	cryptData.checksum = std::string((char*)pChecksum.ptr, pChecksum.size);
//---------	
	int size_out = data_deflate.size;

	bool bPadding = (cryptData.cipherAlgorithm == CRYPT_METHOD::AES_CBC);
	
	if (bPadding && (size_out % PADDING_SIZE != 0))
		size_out = (size_out / PADDING_SIZE + 1) * PADDING_SIZE;

	data_out = new unsigned char[size_out]; 	
	
	_buf pOut (data_out, size_out, false);	

	EncryptCipher(pKey,  ivi, data_deflate, pOut, cryptData.cipherAlgorithm, bPadding ? StreamTransformationFilter::W3C_PADDING :
																						StreamTransformationFilter::NO_PADDING);
	return size_out;
}

void ODFEncryptor::SetCryptData(_odfCryptData	&data)
{
	cryptData = data;

    RandomPool prng;
	
	if (cryptData.saltValue.empty())
	{
        SecByteBlock seed_salt(16);
        OS_GenerateRandomBlock(false, seed_salt, seed_salt.size());
		if (prng.CanIncorporateEntropy())
		{
			prng.IncorporateEntropy(seed_salt, seed_salt.size());
		}
		cryptData.saltValue = std::string((char*)seed_salt.data(), seed_salt.size());
	}
	
	if (cryptData.initializationVector.empty())
	{
        SecByteBlock start_vector(16);
        OS_GenerateRandomBlock(false, start_vector, start_vector.size());
		if (prng.CanIncorporateEntropy())
		{
			prng.IncorporateEntropy(start_vector, start_vector.size());
		}
		cryptData.initializationVector = std::string((char*)start_vector.data(), start_vector.size());
	}
}

void ODFEncryptor::GetCryptData(_odfCryptData &data)
{
	data = cryptData;
}
}
