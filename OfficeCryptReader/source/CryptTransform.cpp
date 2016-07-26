/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#include "../../Common/3dParty/cryptopp/pwdbased.h"
#include "../../Common/3dParty/cryptopp/filters.h"


static const unsigned char encrVerifierHashInputBlockKey[8]			= { 0xfe, 0xa7, 0xd2, 0x76, 0x3b, 0x4b, 0x9e, 0x79 };
static const unsigned char encrVerifierHashValueBlockKey[8]			= { 0xd7, 0xaa, 0x0f, 0x6d, 0x30, 0x61, 0x34, 0x4e };
static const unsigned char encrKeyValueBlockKey[8]					= { 0x14, 0x6e, 0x0b, 0xe7, 0xab, 0xac, 0xd0, 0xd6 };
static const unsigned char encrDataIntegritySaltBlockKey[8]			= { 0x5f, 0xb2, 0xad, 0x01, 0x0c, 0xb9, 0xe1, 0xf6 };
static const unsigned char encrDataIntegrityHmacValueBlockKey[8]	= { 0xa0, 0x67, 0x7f, 0x02, 0xb2, 0x2c, 0x84, 0x33 };

ECMADecryptor::ECMADecryptor()
{
//default ms2010
	cryptData.cipherAlgorithm	= CRYPT_METHOD::AES_CBC;
	cryptData.hashAlgorithm		= CRYPT_METHOD::SHA1;
	cryptData.spinCount			= 100000; 
	cryptData.keySize			= 0x10;
	cryptData.hashSize			= 0x14;
	cryptData.blockSize			= 0x10;
	cryptData.saltSize			= 0x10;
//default ms2016
	//cryptData.cipherAlgorithm	= AES_CBC;
	//cryptData.hashAlgorithm	= SHA256;
	//cryptData.spinCount		= 100000; 
	//cryptData.keySize			= 0x20;
	//cryptData.hashSize		= 0x40;
	//cryptData.blockSize		= 0x10;
	//cryptData.saltSize		= 0x10;
}

class _buf
{
private:
	bool bDelete;
public:
	unsigned char	*ptr;
	int				size;
//-----------------------------------------------------------------------------------
	_buf()													{ptr = NULL; size = 0; bDelete = true;}
	_buf(int sz)											{ptr = new unsigned char [sz]; size = sz; bDelete = true;}
	_buf(unsigned char * p, int sz, bool bDelete_ = true )	
	{
		bDelete = bDelete_;
		if (bDelete)
		{
			ptr = new unsigned char [sz]; size = sz;
			memcpy(ptr, p , sz);
		}
		else
		{
			ptr = p; size = sz; 
		}
	}

	void Clear()
	{
		if (bDelete && ptr) delete []ptr; 
		ptr = NULL; size = 0;

		bDelete = true;
	}
	
	virtual ~_buf()	{Clear();}

	_buf& operator=(const _buf& oSrc)
	{
		Clear();

		size	= oSrc.size;
		ptr		= new unsigned char [oSrc.size];
		memcpy(ptr, oSrc.ptr, oSrc.size);
		bDelete = true;

		return *this;
	}

	_buf& operator=(_buf& oSrc)
	{
		Clear();

		size	= oSrc.size;
		ptr		= new unsigned char [oSrc.size];
		memcpy(ptr, oSrc.ptr, oSrc.size);
		bDelete = true;

		return *this;
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
		delete []hashBuf.ptr;
		
		hashBuf.ptr		= newPtr;
		hashBuf.size	= size;
	}
	else if (hashBuf.size > size)
	{
		hashBuf.size = size;
	}
}

_buf HashAppend(_buf &  hashBuf, _buf & block, CRYPT_METHOD::_hashAlgorithm algorithm)
{//todooo переделать
	if (algorithm == CRYPT_METHOD::SHA1)
	{
		CryptoPP::SHA1 hash;

		if (hashBuf.ptr && hashBuf.size > 0)	hash.Update( hashBuf.ptr, hashBuf.size);
		if (block.ptr	&& block.size > 0)		hash.Update( block.ptr	, block.size);

		CryptoPP::SecByteBlock buffer(hash.DigestSize());
		hash.Final(buffer);

		return _buf(buffer.BytePtr(), buffer.SizeInBytes());
	}
	else if (algorithm == CRYPT_METHOD::SHA256)
	{
		CryptoPP::SHA256 hash;

		if (hashBuf.ptr && hashBuf.size > 0)	hash.Update( hashBuf.ptr, hashBuf.size);
		if (block.ptr	&& block.size > 0)		hash.Update( block.ptr	, block.size);

		CryptoPP::SecByteBlock buffer(hash.DigestSize());
		hash.Final(buffer);

		return _buf(buffer.BytePtr(), buffer.SizeInBytes());
	}
	else if (algorithm == CRYPT_METHOD::SHA512)
	{
		CryptoPP::SHA512 hash;

		if (hashBuf.ptr && hashBuf.size > 0)	hash.Update( hashBuf.ptr, hashBuf.size);
		if (block.ptr	&& block.size > 0)		hash.Update( block.ptr	, block.size);

		CryptoPP::SecByteBlock buffer(hash.DigestSize());
		hash.Final(buffer);

		return _buf(buffer.BytePtr(), buffer.SizeInBytes());
	}
	else
		return _buf();
}

_buf GenerateKey(_buf & salt, _buf & password, _buf & blockKey, int hashSize, int spin, CRYPT_METHOD::_hashAlgorithm algorithm)
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

bool DecryptAES(_buf & key, _buf & iv, _buf & data_inp, _buf & data_out)
{
	CryptoPP::AES::Decryption						aesDecryption(key.ptr, key.size);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption	cbcDecryption( aesDecryption, iv.ptr );
    
	if (!data_out.ptr)
	{
		data_out = _buf(data_inp.size);
	}
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::ArraySink( data_out.ptr, data_out.size), CryptoPP::StreamTransformationFilter::NO_PADDING);
 
	stfDecryptor.Put( data_inp.ptr, data_inp.size );
    stfDecryptor.MessageEnd();

	return true;
}

bool ECMADecryptor::SetPassword(std::wstring password_)
{
	password = password_;
	
	_buf pPassword		((unsigned char*)password.c_str()				, password.length() * 2);
	_buf pSalt			((unsigned char*)cryptData.saltValue.c_str()	, cryptData.saltValue.length());
	_buf pInputBlockKey ((unsigned char*)encrVerifierHashInputBlockKey	, 8);
	_buf pValueBlockKey ((unsigned char*)encrVerifierHashValueBlockKey	, 8);
	
	_buf pEncVerInput	((unsigned char*)cryptData.encryptedVerifierInput.c_str()	, cryptData.encryptedVerifierInput.length());
 	_buf pEncVerValue	((unsigned char*)cryptData.encryptedVerifierValue.c_str()	, cryptData.encryptedVerifierValue.length());
	

	_buf verifierInputKey = GenerateKey( pSalt, pPassword, pInputBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm );
	_buf decryptedVerifierHashInputBytes;
	
	DecryptAES(verifierInputKey, pSalt, pEncVerInput, decryptedVerifierHashInputBytes);
//--------------------------------------------

    _buf empty(NULL,0,false);
    _buf hashBuf = HashAppend(decryptedVerifierHashInputBytes, empty, cryptData.hashAlgorithm);

//--------------------------------------------
	_buf decryptedVerifierHashBytes;
	
	_buf verifierHashKey = GenerateKey(pSalt, pPassword, pValueBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);
	DecryptAES(verifierHashKey, pSalt, pEncVerValue, decryptedVerifierHashBytes);

	return (decryptedVerifierHashBytes==hashBuf);
}

void ECMADecryptor::SetCryptData(_cryptData	&data)
{
	cryptData = data;
}

void ECMADecryptor::Decrypt(unsigned char* data_inp, int  size, unsigned char*& data_out)
{
	data_out = NULL;

	_buf pBlockKey	((unsigned char*)encrKeyValueBlockKey, 8);
	_buf pPassw		((unsigned char*)password.c_str(), password.length() * 2);
	
	_buf pDataSalt	((unsigned char*)cryptData.dataSaltValue.c_str(), cryptData.dataSaltValue.length());
	_buf pSalt		((unsigned char*)cryptData.saltValue.c_str(), cryptData.saltValue.length());
	_buf pKeyValue	((unsigned char*)cryptData.encryptedKeyValue.c_str(), cryptData.encryptedKeyValue.length());
	
	_buf Key = GenerateKey( pSalt, pPassw, pBlockKey, cryptData.keySize, cryptData.spinCount, cryptData.hashAlgorithm);  

	_buf pDecryptedKey;
	DecryptAES( Key, pSalt, pKeyValue, pDecryptedKey);  

	_buf iv(cryptData.blockSize);
	memset( iv.ptr, 0x00, cryptData.blockSize );

	int i = 0, sz = 4096, pos = 0;

	data_out = new unsigned char[size];
	
	while (pos < size)
	{
		if (pos + sz > size) 
			sz = size - pos;
		
		_buf pIndex((unsigned char*)&i, 4);
		iv = HashAppend(pDataSalt, pIndex, cryptData.hashAlgorithm);

		CorrectHashSize(iv, cryptData.blockSize, 0x36);
		
		_buf pInp(data_inp + pos, sz, false);
		_buf pOut(data_out + pos, sz, false);
			
		DecryptAES(pDecryptedKey,  iv, pInp, pOut);

		pos += sz; i++;
	}
}
