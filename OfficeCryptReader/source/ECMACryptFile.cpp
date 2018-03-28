/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include "ECMACryptFile.h"
#include "CryptTransform.h"

#include "../../Common/3dParty/pole/pole.h"
#include "../../Common/DocxFormat/Source/Base/Types_32.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

#include "../../ASCOfficeDocFile/DocDocxConverter/MemoryStream.h"
#include "simple_xml_writer.h"

//CRYPT::_ecmaCryptData cryptDataGlobal; for Test

using namespace CRYPT;

#define GETBIT(from, num) ((from & (1 << num)) != 0)

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

	std::wstring res;
	
	if (sizeof(wchar_t) == 4)
	{
		unsigned int nLength = length;
		
		wchar_t* ptr = new wchar_t [length]; 

		UTF16* pStrUtf16 = (UTF16*)		Data;
		UTF32 *pStrUtf32 = (UTF32 *)	ptr;

        const UTF16 *pStrUtf16_Conv		= pStrUtf16;
        UTF32 *pStrUtf32_Conv			= pStrUtf32;

        ConversionResult eUnicodeConversionResult = ConvertUTF16toUTF32 (&pStrUtf16_Conv, &pStrUtf16[nLength]
                , &pStrUtf32_Conv, &pStrUtf32 [nLength], strictConversion);

        if (conversionOK != eUnicodeConversionResult)
        {
        }
		res = std::wstring(ptr, length);
		delete ptr;
	}
	else
		res = std::wstring((wchar_t*)Data, length);


	return res;

}

void ReadMapEntry(POLE::Stream *pStream, ECMACryptFile::_mapEntry & m)
{
	if (!pStream) return;

	_UINT32 length = 0;
	pStream->read((unsigned char*)&length, 4); 

	_UINT32 refCount = 0;
	pStream->read((unsigned char*)&refCount, 4); 

	for (int i = 0 ; i < refCount; i++)
	{
		ECMACryptFile::_refComponent r;
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
std::string EncodeBase64(const std::string & value)
{
	int nLength = 0;
	char *pData = NULL;
	std::string result;

	NSFile::CBase64Converter::Encode((BYTE*)value.c_str(), value.length(), pData, nLength, NSBase64::B64_BASE64_FLAG_NOCRLF);
	if (pData)
	{
		result = std::string(pData, nLength);
		delete []pData; pData = NULL;
	}
	return result;
}
//-----------------------------------------------------------------------------------------------------------------------
struct _keyEncryptor
{
	std::string	spinCount;
	std::string	saltSize;
	std::string	blockSize;
	std::string	keyBits;
	std::string hashSize;

	std::string cipherAlgorithm;
	std::string cipherChaining;
	std::string hashAlgorithm;

	std::string saltValue;
	std::string encryptedVerifierHashInput;
	std::string encryptedVerifierHashValue;
	std::string encryptedKeyValue;
};
struct _dataIntegrity
{
	std::string	encryptedHmacKey;
	std::string	encryptedHmacValue;
};
bool ReadXmlEncryptionInfo(const std::string & xml_string, _ecmaCryptData & cryptData)
{
	XmlUtils::CXmlLiteReader xmlReader;

	if (!xmlReader.FromStringA(xml_string))
		return false;

	if ( !xmlReader.ReadNextNode() )
		return false;

	_dataIntegrity				dataIntegrity;
	_keyEncryptor				keyData;
	std::vector<_keyEncryptor>	keyEncryptors;

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

	if (keyEncryptors.empty()) return false;

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

	if (keyData.hashAlgorithm == "SHA1")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA1;
	if (keyData.hashAlgorithm == "SHA224")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA224;
	if (keyData.hashAlgorithm == "SHA256")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA256;
	if (keyData.hashAlgorithm == "SHA384")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA384;
	if (keyData.hashAlgorithm == "SHA512")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA512;

	return true;
}

bool WriteXmlEncryptionInfo(const _ecmaCryptData & cryptData, std::string & xml_string)
{
	XmlUtils::CXmlWriter	xmlWriter;

	_dataIntegrity		dataIntegrity;
	_keyEncryptor		keyData;

	keyData.spinCount	= std::to_string(cryptData.spinCount);
	keyData.blockSize	= std::to_string(cryptData.blockSize);
	keyData.hashSize	= std::to_string(cryptData.hashSize);
	keyData.saltSize	= std::to_string(cryptData.saltSize);
	keyData.keyBits		= std::to_string(cryptData.keySize * 8);
	keyData.saltValue	= EncodeBase64(cryptData.dataSaltValue);

	keyData.cipherAlgorithm = "AES";

	if (keyData.cipherAlgorithm == "AES")
	{
		if (cryptData.cipherAlgorithm == CRYPT_METHOD::AES_CBC)	keyData.cipherChaining = "ChainingModeCBC";	
		if (cryptData.cipherAlgorithm == CRYPT_METHOD::AES_CFB)	keyData.cipherChaining = "ChainingModeCFB";
	}

	switch(cryptData.hashAlgorithm)
	{
		case CRYPT_METHOD::SHA1:	keyData.hashAlgorithm = "SHA1";		break;
		case CRYPT_METHOD::SHA224:	keyData.hashAlgorithm = "SHA224";	break;
		case CRYPT_METHOD::SHA256:	keyData.hashAlgorithm = "SHA256";	break;
		case CRYPT_METHOD::SHA384:	keyData.hashAlgorithm = "SHA384";	break;
		case CRYPT_METHOD::SHA512:	keyData.hashAlgorithm = "SHA512";	break;
	}

	std::vector<_keyEncryptor>	keyEncryptors;
	keyEncryptors.push_back(keyData);
	
	keyEncryptors[0].saltValue					= EncodeBase64(cryptData.saltValue);
	keyEncryptors[0].encryptedKeyValue			= EncodeBase64(cryptData.encryptedKeyValue);
	keyEncryptors[0].encryptedVerifierHashInput	= EncodeBase64(cryptData.encryptedVerifierInput);
	keyEncryptors[0].encryptedVerifierHashValue	= EncodeBase64(cryptData.encryptedVerifierValue);
	  
	dataIntegrity.encryptedHmacKey				= EncodeBase64(cryptData.encryptedHmacKey);
	dataIntegrity.encryptedHmacValue			= EncodeBase64(cryptData.encryptedHmacValue);

	std::stringstream stream;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE("encryption")
		{
			CP_XML_ATTR("xmlns",			"http://schemas.microsoft.com/office/2006/encryption");
			CP_XML_ATTR("xmlns:p",			"http://schemas.microsoft.com/office/2006/keyEncryptor/password");
			CP_XML_ATTR("xmlns:c",			"http://schemas.microsoft.com/office/2006/keyEncryptor/certificate");

			CP_XML_NODE("keyData")
			{
				CP_XML_ATTR("saltSize",			keyData.saltSize);
				CP_XML_ATTR("blockSize",		keyData.blockSize);
				CP_XML_ATTR("keyBits",			keyData.keyBits);
				CP_XML_ATTR("hashSize",			keyData.hashSize);
				CP_XML_ATTR("cipherAlgorithm",	keyData.cipherAlgorithm);
				CP_XML_ATTR("cipherChaining",	keyData.cipherChaining);
				CP_XML_ATTR("hashAlgorithm",	keyData.hashAlgorithm);
				CP_XML_ATTR("saltValue",		keyData.saltValue);
			}
			CP_XML_NODE("dataIntegrity")
			{
				CP_XML_ATTR("encryptedHmacKey",		dataIntegrity.encryptedHmacKey);
				CP_XML_ATTR("encryptedHmacValue",	dataIntegrity.encryptedHmacValue);
			}
			int i = 0;
			CP_XML_NODE("keyEncryptors")
			{
				CP_XML_NODE("keyEncryptor")
				{	
					CP_XML_ATTR("uri", "http://schemas.microsoft.com/office/2006/keyEncryptor/password");

					CP_XML_NODE("p:encryptedKey")
					{
						CP_XML_ATTR("spinCount",					keyEncryptors[i].spinCount );
						CP_XML_ATTR("saltSize",						keyEncryptors[i].saltSize );
						CP_XML_ATTR("blockSize",					keyEncryptors[i].blockSize );
						CP_XML_ATTR("keyBits",						keyEncryptors[i].keyBits );
						CP_XML_ATTR("hashSize",						keyEncryptors[i].hashSize );
						CP_XML_ATTR("cipherAlgorithm",				keyEncryptors[i].cipherAlgorithm );
						CP_XML_ATTR("cipherChaining",				keyEncryptors[i].cipherChaining );
						CP_XML_ATTR("hashAlgorithm",				keyEncryptors[i].hashAlgorithm );
						CP_XML_ATTR("saltValue",					keyEncryptors[i].saltValue );
						CP_XML_ATTR("encryptedVerifierHashInput",	keyEncryptors[i].encryptedVerifierHashInput );
						CP_XML_ATTR("encryptedVerifierHashValue",	keyEncryptors[i].encryptedVerifierHashValue );
						CP_XML_ATTR("encryptedKeyValue",			keyEncryptors[i].encryptedKeyValue );
					}
				}
			}
		}
	}

	xml_string = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n" + stream.str();

	return true;
}

bool ReadStandartEncryptionInfo(unsigned char* data, int size, _ecmaCryptData & cryptData)
{
	if (!data || size < 1) return false;
	MemoryStream mem_stream(data, size, false);

//EncryptionHeader
	int HeaderSize	= mem_stream.ReadUInt32();
	int Flags		= mem_stream.ReadUInt32();
	int SizeExtra	= mem_stream.ReadUInt32();
	int AlgID		= mem_stream.ReadUInt32();
	int AlgIDHash	= mem_stream.ReadUInt32();
	int KeySize		= mem_stream.ReadUInt32();
	int ProviderType= mem_stream.ReadUInt32();
	int Reserved1	= mem_stream.ReadUInt32();
	int Reserved2	= mem_stream.ReadUInt32();

	int pos = mem_stream.GetPosition();

	while(pos  < size - 1)
	{
		if (data[pos] == 0 && data[pos + 1] == 0)
		{
			break;
		}
		pos+=2;//unicode null-terminate string
	}
	int szCSPName = pos - mem_stream.GetPosition() + 2;

	unsigned char* strData = mem_stream.ReadBytes(szCSPName, true);
	if (strData)
	{
		delete []strData;
	}
//EncryptionVerifier
	cryptData.saltSize = mem_stream.ReadUInt32(); 
	
	cryptData.saltValue	= std::string((char*)data + mem_stream.GetPosition(), cryptData.saltSize);	
	mem_stream.ReadBytes(cryptData.saltSize, false);
	
	cryptData.encryptedVerifierInput = std::string((char*)data + mem_stream.GetPosition(), 0x10);
	mem_stream.ReadBytes(0x10, false);

	cryptData.hashSize = mem_stream.ReadUInt32();
			
	int szEncryptedVerifierHash = (ProviderType == 0x0001) ? 0x14 : 0x20;
	cryptData.encryptedVerifierValue = std::string((char*)data + mem_stream.GetPosition(), szEncryptedVerifierHash);
	mem_stream.ReadBytes(szEncryptedVerifierHash, false);

	pos = mem_stream.GetPosition();
	
//------------------------------------------------------------------------------------------
	cryptData.hashAlgorithm = CRYPT_METHOD::SHA1; //by AlgIDHash -> 0x0000 || 0x8004
	cryptData.spinCount		= 50000;

	switch(AlgID)
	{
	case 0x6801:	
		cryptData.cipherAlgorithm = CRYPT_METHOD::RC4;		
		cryptData.keySize = KeySize / 8;
		break;
	case 0x660E:	
		cryptData.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
		cryptData.keySize	= 128 /8;	
		break;
	case 0x660F:	
		cryptData.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
		cryptData.keySize	= 192 /8;	
		break;
	case 0x6610:	
		cryptData.cipherAlgorithm = CRYPT_METHOD::AES_ECB;
		cryptData.keySize	= 256 /8;	
		break;
	}
	return true;
}

bool ReadExtensibleEncryptionInfo(unsigned char* data, int size, _ecmaCryptData & cryptData)
{
	return false;
}


//--------------------------------------------------------------
bool ECMACryptFile::EncryptOfficeFile(std::wstring file_name_inp, std::wstring file_name_out, std::wstring password)
{
	_ecmaCryptData cryptData;
	
	cryptData.bAgile		= true;
	cryptData.hashAlgorithm = CRYPT_METHOD::SHA512;
	cryptData.keySize		= 0x20;
	cryptData.hashSize		= 0x40;
	cryptData.blockSize		= 0x10;
	cryptData.saltSize		= 0x10;

	ECMAEncryptor cryptor;	

	//cryptor.SetCryptData(cryptDataGlobal);	//for test !!! 
	cryptor.SetCryptData(cryptData);			//basic settings
	
	cryptor.SetPassword(password);

	NSFile::CFileBinary file;
	if (!file.OpenFile(file_name_inp)) return false;

	_UINT64 lengthFileSize = file.GetFileSize();
	DWORD lengthData = lengthFileSize, lengthDataRead = 0 ;

	unsigned char* data		= new unsigned char[lengthData];
	unsigned char* data_out	= NULL;
	
	file.ReadFile(data, lengthData, lengthDataRead);

	file.CloseFile();
	
	lengthData = cryptor.Encrypt(data, lengthData, data_out);

	cryptor.UpdateDataIntegrity(data_out, lengthData);
	
	if (!data_out)
	{
		delete []data;
		return false;
	}
//-------------------------------------------------------------------
	POLE::Storage *pStorage = new POLE::Storage(file_name_out.c_str());
	
	if (!pStorage)return false;

	if (!pStorage->open(true, true))
	{
		delete pStorage;
		return false;
	}
//-------------------------------------------------------------------
	POLE::Stream *pStream = new POLE::Stream(pStorage, L"EncryptionInfo", true);

	cryptor.GetCryptData(cryptData);

	std::string strXml;
	WriteXmlEncryptionInfo(cryptData, strXml);

	_UINT16 VersionInfoMajor = 0x0004, VersionInfoMinor = 0x0004; //agile standart
	
	pStream->write((unsigned char*)&VersionInfoMajor, 2);
	pStream->write((unsigned char*)&VersionInfoMinor, 2);

	_UINT32 nEncryptionInfoFlags = 64;
	pStream->write((unsigned char*)&nEncryptionInfoFlags, 4); 
	
	pStream->write((unsigned char*)strXml.c_str(), strXml.length()); 
	
	pStream->flush();
	delete pStream;
//-------------------------------------------------------------------
	pStream = new POLE::Stream(pStorage, L"EncryptedPackage", true, lengthData);
	
	pStream->write(data_out, lengthData);

	pStream->flush();
	delete pStream;
	
	pStorage->close();
	delete pStorage;

//
////test back---------------------------------------------------------------------------------test back
//	ECMADecryptor decryptor;
//	
//	decryptor.SetCryptData(cryptData);
//	
//	if (decryptor.SetPassword(password))
//	{
//		unsigned char* data_out2	= NULL;
//		decryptor.Decrypt(data_out, lengthData, data_out2, 0);
//		
//		bool bDataIntegrity = decryptor.CheckDataIntegrity(data_out, lengthData);
//		
//		NSFile::CFileBinary test;
//
//		test.CreateFileW(file_name_out + L"-back.oox");
//		test.WriteFile(data_out2, lengthFileSize);
//		test.CloseFile();
//	}
////test back---------------------------------------------------------------------------------test back

	return true;
}

bool ECMACryptFile::DecryptOfficeFile(std::wstring file_name_inp, std::wstring file_name_out, std::wstring password, bool & bDataIntegrity)
{
	bDataIntegrity = false;

	POLE::Storage *pStorage = new POLE::Storage(file_name_inp.c_str());
	
	if (!pStorage)return false;

	if (!pStorage->open())
	{
		delete pStorage;
		return false;
	}
//-------------------------------------------------------------------------------------------
	_ecmaCryptData cryptData;	
	bool result = false;

	POLE::Stream *pStream = new POLE::Stream(pStorage, L"EncryptionInfo");

	if (pStream)
	{
		_UINT16 VersionInfoMajor = 0, VersionInfoMinor = 0;
		
		pStream->read((unsigned char*)&VersionInfoMajor, 2);
		pStream->read((unsigned char*)&VersionInfoMinor, 2);

		_UINT32 nEncryptionInfoFlags = 0;
		pStream->read((unsigned char*)&nEncryptionInfoFlags, 4); 

		int  nEncryptionInfoSize = pStream->size() - 8;
		unsigned char* byteEncryptionInfo = new unsigned char[nEncryptionInfoSize];
		if (!byteEncryptionInfo)
		{
			delete pStream;
			delete pStorage;
			return false;
		}
		nEncryptionInfoSize = pStream->read(byteEncryptionInfo, nEncryptionInfoSize);
		delete pStream;

		if (VersionInfoMajor == 0x0004 && VersionInfoMinor == 0x0004)
		{//agile info
			std::string xml_string((char*) byteEncryptionInfo, nEncryptionInfoSize);
			delete []byteEncryptionInfo;	
			
			cryptData.bAgile = true;
			result = ReadXmlEncryptionInfo(xml_string, cryptData);
		}
		else
		{
			cryptData.bAgile = false;
			bool fCryptoAPI	= GETBIT(nEncryptionInfoFlags, 1); 
			bool fDocProps	= GETBIT(nEncryptionInfoFlags, 2); 
			bool fExternal	= GETBIT(nEncryptionInfoFlags, 3); 
			bool fAES		= GETBIT(nEncryptionInfoFlags, 4); 
			
			if ((VersionInfoMajor == 0x0003 || VersionInfoMajor == 0x0004) && VersionInfoMinor == 0x0003)		//extensible info
			{
				result = ReadExtensibleEncryptionInfo(byteEncryptionInfo, nEncryptionInfoSize, cryptData);
			}
			else if ((VersionInfoMajor == 0x0003 || VersionInfoMajor == 0x0004) && VersionInfoMinor == 0x0002)	//standart info
			{
				result = ReadStandartEncryptionInfo(byteEncryptionInfo, nEncryptionInfoSize, cryptData);
			}
			else
			{
				// look in DocFormat
			}
			delete []byteEncryptionInfo;
		}
	}
	if (!result) 
	{
		delete pStorage;
		return false;
	}
//------------------------------------------------------------------------------------------------------------
	//pStream = new POLE::Stream(pStorage, "DataSpaces/DataSpaceMap"); 
	//if (pStream)
	//{
	//	delete pStream;
	//	pStorage->deleteByName("DataSpaces");

	//	//_UINT32 size	= 0;
	//	//_UINT32 count	= 0;
	//	//
	//	//pStream->read((unsigned char*)&size, 4); 
	//	//pStream->read((unsigned char*)&count, 4); 

	//	//for (int i = 0 ; i < count; i++)
	//	//{
	//	//	_mapEntry m;
	//	//	ReadMapEntry(pStream, m);

	//	//	mapEntries.push_back(m);
	//	//}
	//	//delete pStream;
	//}
//------------------------------------------------------------------------------------------------------------
	ECMADecryptor decryptor;
	
	decryptor.SetCryptData(cryptData);
	
	if (!decryptor.SetPassword(password))
	{
		if (password.empty())
		{
			password = L"VelvetSweatshop";
			if (!decryptor.SetPassword(password))
				return false;
		}
		else
			return false;
	}
//------------------------------------------------------------------------------------------------------------
	pStream = new POLE::Stream(pStorage, L"EncryptedPackage");
	if ((pStream) && (pStream->size() > 0))
	{
		_UINT64 lengthData, lengthRead = pStream->size();

		unsigned char* data		= new unsigned char[lengthRead];
		unsigned char* data_out	= NULL;
		
		int readTrue = pStream->read(data, lengthRead); 
		int readData = readTrue - 8; 

		lengthData = *((_UINT64*)data);

		decryptor.Decrypt(data + 8, readData, data_out, 0);//todoo сделать покусочное чтение декриптование

		if (data_out)
		{
			NSFile::CFileBinary f;
            f.CreateFileW(file_name_out);
			f.WriteFile(data_out, lengthData);
			f.CloseFile();

			delete []data_out;
			result = true;
		}

		bDataIntegrity = decryptor.CheckDataIntegrity(data, readTrue);
		
		delete pStream;
		delete []data;
	}
//-------------------------------------------------------------------
	delete pStorage;

	//cryptDataGlobal = cryptData; // for encrypt like sample & test

	return result;
}
