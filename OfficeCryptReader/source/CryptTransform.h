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
#pragma once

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace CRYPT_METHOD
{
	enum _hashAlgorithm
	{
		MD5,
		SHA1,
		SHA224,
		SHA256,
		SHA384,
		SHA512
	};

	enum _cipherAlgorithm
	{
		XOR,
		RC4,
		AES_CBC,
		AES_CFB,
		AES_ECB,
		Blowfish_CFB,
		DES_CBC,
		DES_ECB
	};
}
namespace CRYPT
{
class Decryptor
{
	public:
		virtual void Decrypt(char* data, const size_t size, const unsigned long stream_pos, const size_t block_size) = 0;
		virtual void Decrypt(char* data, const size_t size, const unsigned long block_index) = 0;
		virtual bool SetPassword(std::wstring password) = 0;
		virtual void Init(const unsigned long val) = 0;
		virtual bool IsVerify() = 0;

};
typedef boost::shared_ptr<Decryptor> DecryptorPtr;

struct _ecmaWriteProtectData
{
	CRYPT_METHOD::_hashAlgorithm hashAlgorithm = CRYPT_METHOD::SHA512;
	int spinCount = 100000;
	std::string saltValue;
	std::string hashValue;
};
struct _ecmaCryptData
{
//default ms2010		
	//_ecmaCryptData() :	cipherAlgorithm(), hashAlgorithm(), spinCount(100000), 
	//				keySize(0x10), hashSize(), blockSize(0x10), saltSize(0x10), bAgile(true)
//default ms2013/ms2016
	//_cryptData(): cipherAlgorithm(CRYPT_METHOD::AES_CBC), hashAlgorithm(CRYPT_METHOD::SHA256), spinCount(100000), 
	//				keySize(0x20), hashSize(0x40), blockSize(0x10), saltSize(0x10), bAgile(true)
	//{
	//}
	CRYPT_METHOD::_cipherAlgorithm	cipherAlgorithm = CRYPT_METHOD::AES_CBC;
	CRYPT_METHOD::_hashAlgorithm	hashAlgorithm = CRYPT_METHOD::SHA1;

	int			spinCount	= 100000;
	int			keySize		= 0x10;
	int			hashSize	= 0x14;
	int			blockSize	= 0x10;
	int			saltSize	= 0x10;
	
	std::string saltValue;
	std::string encryptedKeyValue;
	std::string encryptedVerifierInput;
	std::string encryptedVerifierValue;
	  
	std::string encryptedHmacKey;
	std::string encryptedHmacValue;

//---------------------------------------------------------------------------------------------
	CRYPT_METHOD::_hashAlgorithm	dataHashAlgorithm = CRYPT_METHOD::SHA1;
	CRYPT_METHOD::_cipherAlgorithm	dataCipherAlgorithm = CRYPT_METHOD::AES_CBC;
	std::string dataSaltValue;
	int			dataKeySize = 0x10;
	int			dataHashSize = 0x14;
	int			dataBlockSize = 0x10;
	int			dataSaltSize = 0x10;

	bool bAgile	= true;

//..........

	bool fDocProps = true;
};
struct _odfCryptData
{
	CRYPT_METHOD::_cipherAlgorithm	cipherAlgorithm = CRYPT_METHOD::AES_CBC;
	
	CRYPT_METHOD::_hashAlgorithm	start_hashAlgorithm = CRYPT_METHOD::SHA1;
	int								start_hashSize		= 20;

	int			spinCount	= 1024;
	int			keySize		= 16;

	std::string saltValue;
	std::string initializationVector;
	
	std::string						checksum;
	int								checksum_size = 1024;
	CRYPT_METHOD::_hashAlgorithm	checksum_hashAlgorithm = CRYPT_METHOD::SHA1;
};
//---------------------------------------------------------------------------------------------------
class ECMAWriteProtect
{
public:
	ECMAWriteProtect(){}
	virtual ~ECMAWriteProtect(){}

	void SetPassword (const std::wstring &password);
	
	void SetCryptData(_ecmaWriteProtectData &data);
	void GetCryptData(_ecmaWriteProtectData &data);

	void Generate();
	bool Verify();

	bool VerifyWrike();
private:
	std::wstring			password;
	_ecmaWriteProtectData	data;
};

//---------------------------------------------------------------------------------------------------
class odfWriteProtect
{
public:
	odfWriteProtect() : hash(CRYPT_METHOD::SHA1) {}

	void SetPassword (const std::wstring & password);
	void SetProtectKey (const std::string & protect_key);
	void SetProtectAlgorithm (const CRYPT_METHOD::_hashAlgorithm & hash);

	void Generate();
	bool Verify();
private:
	std::wstring password;
	CRYPT_METHOD::_hashAlgorithm hash;
	std::string protect_key;
};
//---------------------------------------------------------------------------------------------------
class ECMAEncryptor 
{
public:
	ECMAEncryptor();
	virtual ~ECMAEncryptor(){}

	void SetPassword (std::wstring password);
	
	void SetCryptData(_ecmaCryptData &data);
	void GetCryptData(_ecmaCryptData &data);

	int Encrypt (unsigned char* data, int  size, unsigned char*& data_out);

	void UpdateDataIntegrity(unsigned char* data, int  size);

private:
	std::wstring	password;
	_ecmaCryptData	cryptData;
};

class ECMADecryptor : public Decryptor
{
public:
	ECMADecryptor();
	virtual ~ECMADecryptor();

	virtual void Init(const unsigned long val) {}
	
	virtual void Decrypt (char* data, const size_t size, const unsigned long stream_pos, const size_t block_size);
	virtual void Decrypt (char* data, const size_t size, const unsigned long start_iv_block);
	
	virtual bool SetPassword (std::wstring password);
	virtual bool IsVerify();

	bool CheckDataIntegrity(unsigned char* data, int  size);

	void SetCryptData(_ecmaCryptData &data);
	
	void Decrypt (unsigned char* data, int  size, unsigned char*& data_out, unsigned long start_iv_block);

private:

	std::wstring	password;
	_ecmaCryptData	cryptData;
	bool			bVerify;
};

class ODFDecryptor
{
public:
	ODFDecryptor();
	virtual ~ODFDecryptor(){}

	void SetCryptData(_odfCryptData &data);
	
	bool Decrypt (const std::wstring &wpassword, unsigned char* data_inp, int size_inp, unsigned char*& data_out, int &size_out);

private:
	_odfCryptData	cryptData;
	bool			bVerify;
};
class ODFEncryptor 
{
public:
	ODFEncryptor(){}
	virtual ~ODFEncryptor(){}
	
	void SetCryptData(_odfCryptData &data);
	void GetCryptData(_odfCryptData &data);

	int Encrypt (const std::wstring &password, unsigned char* data, int  size, unsigned char*& data_out);

private:
	_odfCryptData	cryptData;
};


}
