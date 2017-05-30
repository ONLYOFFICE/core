/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#include <iostream>
#include <iomanip>

#include "CryptTransform.h"

#include "../../Common/3dParty/cryptopp/modes.h"
#include "../../Common/3dParty/cryptopp/aes.h"
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

#include "../../Common/DocxFormat/Source/Base/unicode_util.h"
#include "../../Common/DocxFormat/Source/Base/Types_32.h"

static const unsigned char encrVerifierHashInputBlockKey[8]			= { 0xfe, 0xa7, 0xd2, 0x76, 0x3b, 0x4b, 0x9e, 0x79 };
static const unsigned char encrVerifierHashValueBlockKey[8]			= { 0xd7, 0xaa, 0x0f, 0x6d, 0x30, 0x61, 0x34, 0x4e };
static const unsigned char encrKeyValueBlockKey[8]					= { 0x14, 0x6e, 0x0b, 0xe7, 0xab, 0xac, 0xd0, 0xd6 };

static const unsigned char encrDataIntegritySaltBlockKey[8]			= { 0x5f, 0xb2, 0xad, 0x01, 0x0c, 0xb9, 0xe1, 0xf6 };
static const unsigned char encrDataIntegrityHmacValueBlockKey[8]	= { 0xa0, 0x67, 0x7f, 0x02, 0xb2, 0x2c, 0x84, 0x33 };

using namespace CryptoPP;

class _buf
{
public:
	unsigned char	*ptr;
	int				size;
//-----------------------------------------------------------------------------------
	_buf()													: ptr(NULL), size(0), 	bDelete(true){}
	_buf(int sz)											: ptr(NULL), size(0), 	bDelete(true)
	{
		ptr = new unsigned char [sz]; 
		size = sz; 
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
	if (algorithm == CRYPT_METHOD::SHA1)
	{
		CryptoPP::HMAC<CryptoPP::SHA1> hmac(buf.ptr, buf.size);

		//return _buf(mac.BytePtr(), mac.SizeInBytes());
	}
	else if (algorithm == CRYPT_METHOD::SHA256)
	{
		CryptoPP::HMAC<CryptoPP::SHA256> hmac(buf.ptr, buf.size);

		//return _buf(mac.BytePtr(), mac.SizeInBytes());
	}
	else if (algorithm == CRYPT_METHOD::SHA512)
	{
		CryptoPP::HMAC<CryptoPP::SHA512> hmac(buf.ptr, buf.size);
		
		std::string mac;
		CryptoPP::StringSource(plain, true, 
					new CryptoPP::HashFilter(hmac,
						new CryptoPP::StringSink(mac)
					) // HashFilter      
				); // StringSource

		return _buf(mac);

	}
	//else
		return _buf();
}


_buf HashAppend(_buf &  hashBuf, _buf & block, CRYPT_METHOD::_hashAlgorithm algorithm)
{//todooo переделать
	if (algorithm == CRYPT_METHOD::SHA1)
	{
		//MD5 hash;

		//if (hashBuf.ptr && hashBuf.size > 0)	hash.Update( hashBuf.ptr, hashBuf.size);
		//if (block.ptr	&& block.size > 0)		hash.Update( block.ptr	, block.size);

		//SecByteBlock buffer(hash.DigestSize());
		//hash.Final(buffer);

		//return _buf(buffer.BytePtr(), buffer.SizeInBytes());
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

_buf GenerateHashKey(_buf & salt, _buf & password, int hashSize, int spin, CRYPT_METHOD::_hashAlgorithm algorithm)
{
	_buf empty (NULL, 0, false);
	_buf pHashBuf = HashAppend(salt, password, algorithm);
		
	int i = 0;
	for (i = 0; i < spin; i++)
	{
        _buf iterator((unsigned char*)&i, 4, false);
        pHashBuf = HashAppend(iterator, pHashBuf, algorithm);
	}
	i = 0;
    _buf iterator((unsigned char*)&i, 4, false);
    pHashBuf = HashAppend(pHashBuf, iterator, algorithm);

	_buf derivedKey(64);
	for (int i = 0; i < derivedKey.size; i++)
	{
		derivedKey.ptr[i] = (i < pHashBuf.size ? 0x36 ^ pHashBuf.ptr[i] : 0x36);
	}

	pHashBuf = HashAppend(derivedKey, empty, algorithm);

	return _buf(pHashBuf.ptr, hashSize);
}

bool EncryptCipher(_buf & key, _buf & iv, _buf & data_inp, _buf & data_out, CRYPT_METHOD::_cipherAlgorithm algorithm,
				    StreamTransformationFilter::BlockPaddingScheme padding = StreamTransformationFilter::PKCS_PADDING)
{	
	if (algorithm == CRYPT_METHOD::XOR)
	{
		return false;
	}
	else if (algorithm == CRYPT_METHOD::RC4)
	{
		return false;
	}
	else //AES
	{
		StreamTransformation *modeEncryption = NULL;
		AES::Encryption aesEncryption(key.ptr, key.size);

		switch(algorithm)
		{
		case CRYPT_METHOD::AES_ECB:
			 modeEncryption = new ECB_Mode_ExternalCipher::Encryption(aesEncryption, iv.ptr ); 
			 break;
		case CRYPT_METHOD::AES_CBC:
			 modeEncryption = new CBC_Mode_ExternalCipher::Encryption(aesEncryption, iv.ptr ); 
			 break;
		case CRYPT_METHOD::AES_CFB:
			 modeEncryption = new CFB_Mode_ExternalCipher::Encryption(aesEncryption, iv.ptr ); 
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
	if (algorithm == CRYPT_METHOD::XOR)
	{
		return false;
	}
	else if (algorithm == CRYPT_METHOD::RC4)
	{
		CryptoPP::ARC4 rc4(key.ptr, key.size);
		data_out.ptr = new unsigned char[data_inp.size];
		data_out.size = data_inp.size;
		rc4.ProcessData(data_out.ptr, data_inp.ptr, data_inp.size);
	}
	else //AES
	{
		StreamTransformation *modeDecryption = NULL;
		AES::Decryption aesDecryption(key.ptr, key.size);

		switch(algorithm)
		{
		case CRYPT_METHOD::AES_ECB:
			 modeDecryption = new ECB_Mode_ExternalCipher::Decryption(aesDecryption, iv.ptr ); 
			 break;
		case CRYPT_METHOD::AES_CBC:
			 modeDecryption = new CBC_Mode_ExternalCipher::Decryption(aesDecryption, iv.ptr ); 
			 break;
		case CRYPT_METHOD::AES_CFB:
			 modeDecryption = new CFB_Mode_ExternalCipher::Decryption(aesDecryption, iv.ptr ); 
		}
	
		if (!modeDecryption) return false;
		    
		if (!data_out.ptr)
		{
			data_out = _buf(data_inp.size);
		}
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

ECMADecryptor::ECMADecryptor()
{
	bVerify = false;
}

bool ECMADecryptor::SetPassword(std::wstring _password)
{
	bVerify		= false;
	password	= _password;

	if (password.empty()) return false;
	
	if (cryptData.bAgile)
	{	
		_buf pPassword		(password);
		_buf pSalt			(cryptData.saltValue);
		_buf pInputBlockKey ((unsigned char*)encrVerifierHashInputBlockKey, 8);
		_buf pValueBlockKey ((unsigned char*)encrVerifierHashValueBlockKey, 8);
		_buf empty			(NULL, 0, false);
		
		_buf pEncVerInput	(cryptData.encryptedVerifierInput);
 		_buf pEncVerValue	(cryptData.encryptedVerifierValue);
		
		_buf verifierInputKey = GenerateAgileKey( pSalt, pPassword, pInputBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm );
		_buf decryptedVerifierHashInputBytes;
		
		DecryptCipher(verifierInputKey, pSalt, pEncVerInput, decryptedVerifierHashInputBytes, cryptData.cipherAlgorithm);
//--------------------------------------------

		_buf hashBuf = HashAppend(decryptedVerifierHashInputBytes, empty, cryptData.hashAlgorithm);

//--------------------------------------------
		_buf decryptedVerifierHashBytes;
		
		_buf verifierHashKey = GenerateAgileKey(pSalt, pPassword, pValueBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);
		DecryptCipher(verifierHashKey, pSalt, pEncVerValue, decryptedVerifierHashBytes, cryptData.cipherAlgorithm);
//--------------------------------------------
		bVerify	= (decryptedVerifierHashBytes==hashBuf);
	}
	else
	{
		_buf pPassword		(password);
		_buf pSalt			(cryptData.saltValue);
		_buf empty			(NULL, 0, false);

		_buf pEncVerInput	(cryptData.encryptedVerifierInput);
 		_buf pEncVerValue	(cryptData.encryptedVerifierValue);

		_buf hashKey = GenerateHashKey(pSalt, pPassword, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);
		
		_buf decryptedVerifierHashInputBytes;		
		DecryptCipher(hashKey, empty, pEncVerInput, decryptedVerifierHashInputBytes, cryptData.cipherAlgorithm);

	//--------------------------------------------

		_buf hashBuf = HashAppend(decryptedVerifierHashInputBytes, empty, cryptData.hashAlgorithm);

	//--------------------------------------------
		_buf decryptedVerifierHashBytes;
		
		DecryptCipher(hashKey, empty, pEncVerValue, decryptedVerifierHashBytes, cryptData.cipherAlgorithm);

		bVerify	= (decryptedVerifierHashBytes==hashBuf);
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
void ECMADecryptor::Decrypt(char* data	, const size_t size, const unsigned long stream_pos)
{
	if (bVerify)
	{
		unsigned char* data_out = NULL;
		Decrypt((unsigned char*)data, size, data_out);
		
		if (data_out)
		{
			memcpy(data, data_out, size);
			delete []data_out;
		}
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
	_buf iv1 = HashAppend(pDataSalt, pBlockHmacKey, cryptData.hashAlgorithm);
	CorrectHashSize(iv1, cryptData.blockSize, 0x36);
	
	_buf iv2 = HashAppend(pDataSalt, pBlockHmacValue, cryptData.hashAlgorithm);
	CorrectHashSize(iv2, cryptData.blockSize, 0x36);

	_buf salt;
	DecryptCipher(secretKey,  iv1, pEncHmacKey, salt, cryptData.cipherAlgorithm);
	
	_buf expected;
	DecryptCipher(secretKey,  iv2, pEncHmacValue, expected, cryptData.cipherAlgorithm);

	std::string sData((char*)data, size);
	_buf hmac = Hmac(salt, cryptData.hashAlgorithm, sData);
		
	return (hmac == expected);
}
void ECMADecryptor::Decrypt(unsigned char* data_ptr, int  data_size, unsigned char*& data_out)
{
	data_out = NULL;
	
	_buf pPassword	(password);
	_buf pSalt		(cryptData.saltValue);
	_buf empty		(NULL, 0, false);

	int				size	 = data_size - 8;
	unsigned char*	data_inp = data_ptr + 8;

	data_out = new unsigned char[size];

	if (cryptData.bAgile)
	{	
		_buf pBlockKey	((unsigned char*)encrKeyValueBlockKey, 8);	
		_buf pDataSalt	(cryptData.dataSaltValue);
		_buf pKeyValue	(cryptData.encryptedKeyValue);

		_buf agileKey = GenerateAgileKey( pSalt, pPassword, pBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);  

		_buf pDecryptedKey;
		DecryptCipher( agileKey, pSalt, pKeyValue, pDecryptedKey, cryptData.cipherAlgorithm);  

		_buf iv(cryptData.blockSize);
		memset( iv.ptr, 0x00, cryptData.blockSize );

		int i = 0, sz = 4096, pos = 0;

		while (pos < size)
		{
			if (pos + sz > size) 
				sz = size - pos;
			
			_buf pIndex((unsigned char*)&i, 4);
			iv = HashAppend(pDataSalt, pIndex, cryptData.hashAlgorithm);

			CorrectHashSize(iv, cryptData.blockSize, 0x36);
			
			_buf pInp(data_inp + pos, sz, false);
			_buf pOut(data_out + pos, sz, false);

			DecryptCipher(pDecryptedKey,  iv, pInp, pOut, cryptData.cipherAlgorithm);
			
			pos += sz; i++;
		}
	//--------------------------------------------
		_buf pEncVerKeyMac		(cryptData.encryptedHmacKey);
 		_buf pEncVerValueMac	(cryptData.encryptedHmacValue);
		
		_buf iv1(cryptData.blockSize);
		memset( iv1.ptr, 0x00, cryptData.blockSize );

		i = 0;
		_buf pIndex1((unsigned char*)&i, 4);
		iv1 = HashAppend(pDataSalt, pIndex1, cryptData.hashAlgorithm);

		CorrectHashSize(iv1, cryptData.blockSize, 0x36);
		
		_buf pOut1(pEncVerKeyMac.size);
		_buf pOut2(pEncVerValueMac.size);

		DecryptCipher(pDecryptedKey,  iv, pEncVerKeyMac, pOut1, cryptData.cipherAlgorithm);
		DecryptCipher(pDecryptedKey,  iv, pEncVerValueMac, pOut2, cryptData.cipherAlgorithm);

	}
	else
	{
		_buf hashKey = GenerateHashKey(pSalt, pPassword, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);
		
		_buf pInp(data_inp, size, false);
		_buf pOut(data_out, size, false);
		
		DecryptCipher(hashKey, empty, pInp, pOut, cryptData.cipherAlgorithm);
	}
}

//-----------------------------------------------------------------------------------------------------------
ECMAEncryptor::ECMAEncryptor()
{
}

void ECMAEncryptor::SetPassword(std::wstring _password)
{
	password = _password;
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
	
	_buf agileKey = GenerateAgileKey( pSalt, pPassword, pBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);  

	_buf secretKey;
	DecryptCipher( agileKey, pSalt, pKeyValue, secretKey, cryptData.cipherAlgorithm);  
	
	_buf iv1 = HashAppend(pDataSalt, pBlockHmacKey, cryptData.hashAlgorithm);
	CorrectHashSize(iv1, cryptData.blockSize, 0x36);
	
	_buf iv2 = HashAppend(pDataSalt, pBlockHmacValue, cryptData.hashAlgorithm);
	CorrectHashSize(iv2, cryptData.blockSize, 0x36);

//----
	std::string sData((char*)data, size);
	_buf hmac = Hmac(pSalt, cryptData.hashAlgorithm, sData);

	_buf pEncHmacKey;
	EncryptCipher(secretKey,  iv1, pSalt, pEncHmacKey, cryptData.cipherAlgorithm);

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
	if (size_out % 8 != 0) 
		size_out = (size_out / 8 + 1) * 8;
	
	data_out_ptr = new unsigned char[size_out + 8]; // real size + padding + size for realsize
	
	_UINT64 nSize = size;
	memcpy(data_out_ptr, (unsigned char*)&nSize, 8);

	unsigned char* data_inp = data_inp_ptr;
	unsigned char* data_out = data_out_ptr + 8;

	_buf pBlockKey	((unsigned char*)encrKeyValueBlockKey, 8);	
	_buf pDataSalt	(cryptData.dataSaltValue);
	_buf pKeyValue	(cryptData.encryptedKeyValue);
	
//------------------------------------------------------------------------------------------------
	//соль нужно сгенерить

	_buf agileKey = GenerateAgileKey( pSalt, pPassword, pBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);  

	//тут нужно именно дешифрованый генерить  - пока их файла берем
	_buf pDecryptedKey;
	DecryptCipher( agileKey, pSalt, pKeyValue, pDecryptedKey, cryptData.cipherAlgorithm);  

	//зашифровать ключь
	_buf pEncryptedKey;
	EncryptCipher( agileKey, pSalt, pDecryptedKey, pEncryptedKey, cryptData.cipherAlgorithm);  

	//??? pEncryptedKey == pKeyValue;

//-------------------------------------------------------------------------------------------------
	_buf iv(cryptData.blockSize);
	memset( iv.ptr, 0x00, cryptData.blockSize );

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
			
			if (sz % 8 != 0) 
				sz = (sz / 8 + 1) * 8;
			
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

	return enc_size + 8;
}




}