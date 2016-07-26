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

#include "ECMACryptReader.h"
#include "CryptTransform.h"

#include "../../Common/3dParty/pole/pole.h"
#include "../../Common/DocxFormat/Source/Base/Types_32.h"
#include "../../Common/DocxFormat/Source/XML/xmlutils.h"

#include "../../DesktopEditor/common/File.h"

#define WritingElement_ReadAttributes_Start(Reader) \
	if ( Reader.GetAttributesCount() <= 0 )\
		return false;\
	if ( !Reader.MoveToFirstAttribute() )\
		return false;\
		std::string wsName = Reader.GetNameA();\
    while( !wsName.empty() )\
	{

#define WritingElement_ReadAttributes_Read_if(Reader, AttrName, Value) \
		if ( AttrName == wsName )\
        {\
            Value = Reader.GetTextA();\
        }

#define WritingElement_ReadAttributes_Read_else_if(Reader, AttrName, Value) \
		else if ( AttrName == wsName )\
            Value = Reader.GetTextA();

#define WritingElement_ReadAttributes_ReadSingle(Reader, AttrName, Value) \
		if ( AttrName == wsName )\
		{\
            Value = Reader.GetTextA();\
			break;\
		}

#define WritingElement_ReadAttributes_End(Reader) \
		if ( !Reader.MoveToNextAttribute() ) \
			break;\
		wsName = Reader.GetNameA();\
	}\
	Reader.MoveToElement();

std::wstring ReadUnicodeLP(POLE::Stream *pStream)
{
	if (!pStream) return L"";

	_UINT32 length = 0;
	pStream->read((unsigned char*)&length, 4); 

	unsigned char* Data = new unsigned char[length * 2];
	pStream->read(Data, length * 2); 

	std::wstring res ((wchar_t*)Data, length);

	return res;

}
void ReadMapEntry(POLE::Stream *pStream, ECMACryptReader::_mapEntry & m)
{
	if (!pStream) return;

	_UINT32 length = 0;
	pStream->read((unsigned char*)&length, 4); 

	_UINT32 refCount = 0;
	pStream->read((unsigned char*)&refCount, 4); 

	for (int i = 0 ; i < refCount; i++)
	{
		ECMACryptReader::_refComponent r;
		pStream->read((unsigned char*)&r.type, 4); 

		r.ref = ReadUnicodeLP(pStream);
		m.refComponents.push_back(r);
	}
	m.dataSpaceName= ReadUnicodeLP(pStream);
}

std::string DecodeBase64(const std::string & value)
{
	int nLength = 0;
	unsigned char *pData = NULL;
	std::string result;

	NSFile::CBase64Converter::Decode(value.c_str(), value.length(), pData, nLength);
	if (pData)
	{
		result = std::string((char*)pData, nLength);
		delete []pData; pData = NULL;
	}
	return result;
}
//--------------------------------------------------------------
bool ECMACryptReader::DecryptOfficeFile(std::wstring file_name_inp, std::wstring file_name_out, std::wstring password)
{
	POLE::Storage *pStorage = new POLE::Storage(file_name_inp.c_str());
	
	if (!pStorage)return false;

	if (!pStorage->open())
	{
		delete pStorage;
		return false;
	}

	POLE::Stream *pStream = new POLE::Stream(pStorage, "EncryptionInfo");
	if (pStream)
	{
		_UINT32 nEncryptionInfoSize = 0;
		int sz = pStream->read((unsigned char*)&nEncryptionInfoSize, 4); //size uncrypt ??

		_UINT32 nEncryptionInfoSize1 = 0;
		sz = pStream->read((unsigned char*)&nEncryptionInfoSize1, 4); //??? (64)

		unsigned char* byteEncryptionInfo = new unsigned char[nEncryptionInfoSize];
		if (!byteEncryptionInfo)
		{
			delete pStream;
			delete pStorage;
			return false;
		}
		sz = pStream->read(byteEncryptionInfo, nEncryptionInfoSize);

		std::string xml_string((char*) byteEncryptionInfo, sz);
		delete []byteEncryptionInfo;
		delete pStream;
	
		if (!ReadEncryptionInfo(xml_string))
		{
			delete pStorage;
			return false;
		}
	}

	ECMADecryptor decryptor;

	ECMADecryptor::_cryptData cryptData;
	
	cryptData.spinCount				 = atoi(keyEncryptors[0].spinCount.c_str());
	cryptData.blockSize				 = atoi(keyEncryptors[0].blockSize.c_str());
	cryptData.hashSize				 = atoi(keyEncryptors[0].hashSize.c_str());
	cryptData.saltSize				 = atoi(keyEncryptors[0].saltSize.c_str());
	cryptData.keySize				 = atoi(keyEncryptors[0].keyBits.c_str() ) / 8;
	
	cryptData.dataSaltValue          = DecodeBase64(keyData.saltValue);
	cryptData.saltValue              = DecodeBase64(keyEncryptors[0].saltValue);
	cryptData.encryptedKeyValue      = DecodeBase64(keyEncryptors[0].encryptedKeyValue);
	cryptData.encryptedVerifierInput = DecodeBase64(keyEncryptors[0].encryptedVerifierHashInput);
	cryptData.encryptedVerifierValue = DecodeBase64(keyEncryptors[0].encryptedVerifierHashValue);
	  
	cryptData.encryptedHmacKey       = DecodeBase64(dataIntegrity.encryptedHmacKey);
	cryptData.encryptedHmacValue     = DecodeBase64(dataIntegrity.encryptedHmacValue);

	if (keyData.cipherAlgorithm == "AES")
	{
		if (keyData.cipherChaining == "ChainingModeCBC")	cryptData.cipherAlgorithm = CRYPT_METHOD::AES_CBC;
		if (keyData.cipherChaining == "ChainingModeCFB")	cryptData.cipherAlgorithm = CRYPT_METHOD::AES_CFB;
	}
	else
	{
	}

	if (keyData.hashAlgorithm == "SHA1")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA1;
	if (keyData.hashAlgorithm == "SHA224")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA224;
	if (keyData.hashAlgorithm == "SHA256")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA256;
	if (keyData.hashAlgorithm == "SHA384")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA384;
	if (keyData.hashAlgorithm == "SHA512")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA512;

	decryptor.SetCryptData(cryptData);
	
	if (!decryptor.SetPassword(password))
		return false;

	//pStream = new POLE::Stream(pStorage, "DataSpaces/DataSpaceMap"); // савершенно ненужная инфа
	//if (pStream)
	//{
	//	_UINT32 size	= 0;
	//	_UINT32 count	= 0;
	//	
	//	pStream->read((unsigned char*)&size, 4); 
	//	pStream->read((unsigned char*)&count, 4); 

	//	for (int i = 0 ; i < count; i++)
	//	{
	//		_mapEntry m;
	//		ReadMapEntry(pStream, m);

	//		mapEntries.push_back(m);
	//	}
	//	delete pStream;
	//}

	bool result = false;

	pStream = new POLE::Stream(pStorage, "EncryptedPackage");
	if (pStream->size() > 0)
	{
		_UINT64 lengthData, lengthRead = pStream->size() - 8;
		pStream->read((unsigned char*)&lengthData, 8); 

		unsigned char* data		= new unsigned char[lengthRead];
		unsigned char* data_out	= NULL;

		pStream->read(data, lengthRead);

		decryptor.Decrypt(data, lengthRead, data_out);//todoo сделать покусочное чтение декриптование
		delete pStream;

		if (data_out)
		{
			NSFile::CFileBinary f;
			f.CreateFile(file_name_out);
			f.WriteFile(data_out, lengthData);
			f.CloseFile();

			result = true;
		}
	}
//-------------------------------------------------------------------
	delete pStorage;
	return result;
}


bool ECMACryptReader::ReadEncryptionInfo(const std::string & xml_string)
{
	XmlUtils::CXmlLiteReader xmlReader;

	if (!xmlReader.FromStringA(xml_string))
		return false;

	if ( !xmlReader.ReadNextNode() )
		return false;

	int nCurDepth = xmlReader.GetDepth();
	while( xmlReader.ReadNextSiblingNode( nCurDepth ) )
	{
		std::wstring sName = xmlReader.GetName();
		if ( L"keyData" == sName )
		{
			WritingElement_ReadAttributes_Start( xmlReader)
				WritingElement_ReadAttributes_Read_if		( xmlReader, "saltSize",		keyData.saltSize )
				WritingElement_ReadAttributes_Read_else_if	( xmlReader, "blockSize",		keyData.blockSize )
				WritingElement_ReadAttributes_Read_else_if	( xmlReader, "keyBits",			keyData.keyBits )
				WritingElement_ReadAttributes_Read_else_if	( xmlReader, "hashSize",		keyData.hashSize )
				WritingElement_ReadAttributes_Read_else_if	( xmlReader, "cipherAlgorithm",	keyData.cipherAlgorithm )
				WritingElement_ReadAttributes_Read_else_if	( xmlReader, "cipherChaining",	keyData.cipherChaining )
				WritingElement_ReadAttributes_Read_else_if	( xmlReader, "hashAlgorithm",	keyData.hashAlgorithm )
				WritingElement_ReadAttributes_Read_else_if	( xmlReader, "saltValue",		keyData.saltValue )
			WritingElement_ReadAttributes_End( xmlReader )
		}
		else if ( L"dataIntegrity" == sName )
		{
			WritingElement_ReadAttributes_Start( xmlReader)
				WritingElement_ReadAttributes_Read_if		( xmlReader, "encryptedHmacKey",	dataIntegrity.encryptedHmacKey)
				WritingElement_ReadAttributes_Read_else_if	( xmlReader, "encryptedHmacValue",	dataIntegrity.encryptedHmacValue)
			WritingElement_ReadAttributes_End( xmlReader )
		}
		else if (L"keyEncryptors" == sName)
		{
			while( xmlReader.ReadNextSiblingNode( nCurDepth + 1 ) )
			{
				sName = xmlReader.GetName();
				if (L"keyEncryptor" == sName)
				 {
					while( xmlReader.ReadNextSiblingNode( nCurDepth + 2 ) )
					{
						sName = xmlReader.GetName();
						if (L"p:encryptedKey" == sName)
						 {
							_keyEncryptor k={};
							 
							WritingElement_ReadAttributes_Start( xmlReader)
								WritingElement_ReadAttributes_Read_if     ( xmlReader, "spinCount",			k.spinCount )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "saltSize",			k.saltSize )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "blockSize",			k.blockSize )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "keyBits",			k.keyBits )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "hashSize",			k.hashSize )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "cipherAlgorithm",	k.cipherAlgorithm )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "cipherChaining",	k.cipherChaining )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "hashAlgorithm",		k.hashAlgorithm )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "saltValue",			k.saltValue )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "encryptedVerifierHashInput",	k.encryptedVerifierHashInput )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "encryptedVerifierHashValue",	k.encryptedVerifierHashValue )
								WritingElement_ReadAttributes_Read_else_if( xmlReader, "encryptedKeyValue",	k.encryptedKeyValue )
							WritingElement_ReadAttributes_End( xmlReader )
						 
							keyEncryptors.push_back(k);
						}
					}
				}
			}
		}
	}
	return true;
}

