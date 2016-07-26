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
#pragma once

#include <string>
#include <vector>

namespace CRYPT_METHOD
{
	enum _hashAlgorithm
	{
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
		AES_CFB
	};
}

class ECMADecryptor
{
public:

	struct _cryptData
	{
		CRYPT_METHOD::_cipherAlgorithm	cipherAlgorithm;
		CRYPT_METHOD::_hashAlgorithm	hashAlgorithm;

		int			spinCount;
		int			keySize;
		int			hashSize;
		int			blockSize;
		int			saltSize;

		std::string dataSaltValue;
		std::string saltValue;
		std::string encryptedKeyValue;
		std::string encryptedVerifierInput;
		std::string encryptedVerifierValue;
		  
		std::string encryptedHmacKey;
		std::string encryptedHmacValue;

//..........

	};
	ECMADecryptor();
	virtual ~ECMADecryptor(){}

	void Decrypt(unsigned char* data, int  size, unsigned char*& data_out);

	bool IsVerify(){}

	bool SetPassword(std::wstring password);

	void SetCryptData(_cryptData	&data);
	
private:
	_cryptData		cryptData;
	std::wstring	password;
};
//
//class ECMAEncryptor
//{
//public:
//	ECMAEncryptor(int type);
//	virtual ~ECMAEncryptor();
//
//	void Encrypt(char* data, int size);
//
//	bool SetPassword(std::wstring password);
//};