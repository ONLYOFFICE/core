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

#include "ECMACryptFile.h"
#include "CryptTransform.h"

#include "../../Common/3dParty/pole/pole.h"
#include "../../OOXML/Base/Base.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

#include "../../MsBinaryFile/DocFile/MemoryStream.h"
#include "simple_xml_writer.h"
#include "../../Common/cfcpp/compoundfile.h"

//CRYPT::_ecmaCryptData cryptDataGlobal; for Test

#define USE_MSSTORAGE

using namespace CRYPT;

#define GETBIT(from, num)				((from & (1 << num)) != 0)
#define SETBIT(to, num, setorclear)		{setorclear ? to |= (1 << num) : to &= ~(1 << num);}

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
		delete []ptr;
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

	for (_UINT32 i = 0 ; i < refCount; i++)
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

	NSFile::CBase64Converter::Decode(value.c_str(), (int)value.length(), pData, nLength);
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

	NSFile::CBase64Converter::Encode((BYTE*)value.c_str(), (int)value.length(), pData, nLength, NSBase64::B64_BASE64_FLAG_NOCRLF);
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
							_keyEncryptor k;
							 
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
	
	cryptData.saltValue              = DecodeBase64(keyEncryptors[0].saltValue);
	cryptData.encryptedKeyValue      = DecodeBase64(keyEncryptors[0].encryptedKeyValue);
	cryptData.encryptedVerifierInput = DecodeBase64(keyEncryptors[0].encryptedVerifierHashInput);
	cryptData.encryptedVerifierValue = DecodeBase64(keyEncryptors[0].encryptedVerifierHashValue);
	
		 if (keyEncryptors[0].hashAlgorithm == "SHA1")		cryptData.hashAlgorithm = CRYPT_METHOD::SHA1;
	else if (keyEncryptors[0].hashAlgorithm == "SHA224")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA224;
	else if (keyEncryptors[0].hashAlgorithm == "SHA256")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA256;
	else if (keyEncryptors[0].hashAlgorithm == "SHA384")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA384;
	else if (keyEncryptors[0].hashAlgorithm == "SHA512")	cryptData.hashAlgorithm = CRYPT_METHOD::SHA512;
	else if (keyEncryptors[0].hashAlgorithm == "MD5")		cryptData.hashAlgorithm = CRYPT_METHOD::MD5;

	if (keyEncryptors[0].cipherAlgorithm == "AES")
	{
		cryptData.cipherAlgorithm = CRYPT_METHOD::AES_CBC;
		//if (keyEncryptors[0].cipherChaining == "ChainingModeCBC")	cryptData.cipherAlgorithm = CRYPT_METHOD::AES_CBC;
		if (keyEncryptors[0].cipherChaining == "ChainingModeCFB")	cryptData.dataCipherAlgorithm = CRYPT_METHOD::AES_CFB;
	}
	else if (keyEncryptors[0].cipherAlgorithm == "RC4")
	{
		cryptData.dataCipherAlgorithm = CRYPT_METHOD::RC4;
	}
	else if (keyEncryptors[0].cipherAlgorithm == "DES")
	{
		cryptData.dataCipherAlgorithm = CRYPT_METHOD::DES_CBC;
		//if (keyEncryptors[0].cipherChaining == "ChainingModeCBC")	cryptData.cipherAlgorithm = CRYPT_METHOD::DES_CBC;
		if (keyEncryptors[0].cipherChaining == "ChainingModeECB")	cryptData.cipherAlgorithm = CRYPT_METHOD::DES_ECB;
	}
//---------------------------------------------------------------------------------------------------------------
	cryptData.encryptedHmacKey = DecodeBase64(dataIntegrity.encryptedHmacKey);
	cryptData.encryptedHmacValue = DecodeBase64(dataIntegrity.encryptedHmacValue);
//---------------------------------------------------------------------------------------------------------------
	cryptData.dataSaltValue			= DecodeBase64(keyData.saltValue);
	cryptData.dataBlockSize			= atoi(keyData.blockSize.c_str());
	cryptData.dataHashSize			= atoi(keyData.hashSize.c_str());
	cryptData.dataSaltSize			= atoi(keyData.saltSize.c_str());
	cryptData.dataKeySize			= atoi(keyData.keyBits.c_str()) / 8;

	if (keyData.cipherAlgorithm == "AES")
	{
		cryptData.cipherAlgorithm = CRYPT_METHOD::AES_CBC;
		//if (keyData.cipherChaining == "ChainingModeCBC")	cryptData.cipherAlgorithm = CRYPT_METHOD::AES_CBC;
		if (keyData.cipherChaining == "ChainingModeCFB")	cryptData.dataCipherAlgorithm = CRYPT_METHOD::AES_CFB;
	}
	else if (keyData.cipherAlgorithm == "RC4")
	{
		cryptData.dataCipherAlgorithm = CRYPT_METHOD::RC4;
	}
	else if (keyData.cipherAlgorithm == "DES")
	{
		cryptData.dataCipherAlgorithm = CRYPT_METHOD::DES_CBC;
		//if (keyData.cipherChaining == "ChainingModeCBC")	cryptData.cipherAlgorithm = CRYPT_METHOD::DES_CBC;
		if (keyData.cipherChaining == "ChainingModeECB")	cryptData.cipherAlgorithm = CRYPT_METHOD::DES_ECB;
	}
		 if (keyData.hashAlgorithm == "SHA1")	cryptData.dataHashAlgorithm = CRYPT_METHOD::SHA1;
	else if (keyData.hashAlgorithm == "SHA224")	cryptData.dataHashAlgorithm = CRYPT_METHOD::SHA224;
	else if (keyData.hashAlgorithm == "SHA256")	cryptData.dataHashAlgorithm = CRYPT_METHOD::SHA256;
	else if (keyData.hashAlgorithm == "SHA384")	cryptData.dataHashAlgorithm = CRYPT_METHOD::SHA384;
	else if (keyData.hashAlgorithm == "SHA512")	cryptData.dataHashAlgorithm = CRYPT_METHOD::SHA512;
	else if (keyData.hashAlgorithm == "MD5")	cryptData.dataHashAlgorithm = CRYPT_METHOD::MD5;

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

	switch(cryptData.cipherAlgorithm)
	{
	case CRYPT_METHOD::RC4:
		keyData.cipherAlgorithm = "RC4"; 
	break;
	case CRYPT_METHOD::AES_CBC:
		keyData.cipherAlgorithm = "AES";
		keyData.cipherChaining = "ChainingModeCBC";	
	break;
	case CRYPT_METHOD::AES_ECB:
		keyData.cipherAlgorithm = "AES";
		keyData.cipherChaining = "ChainingModeECB";
	break;
	case CRYPT_METHOD::AES_CFB:
		keyData.cipherAlgorithm = "AES";
		keyData.cipherChaining = "ChainingModeCFB";
		break;
	case CRYPT_METHOD::DES_CBC:
		keyData.cipherAlgorithm = "DES";
		keyData.cipherChaining = "ChainingModeCBC";
	break;
	case CRYPT_METHOD::DES_ECB:
		keyData.cipherAlgorithm = "DES";
		keyData.cipherChaining = "ChainingModeECB";
	break;
	break;

	}

	switch(cryptData.hashAlgorithm)
	{
		case CRYPT_METHOD::SHA1:	keyData.hashAlgorithm = "SHA1";		break;
		case CRYPT_METHOD::SHA224:	keyData.hashAlgorithm = "SHA224";	break;
		case CRYPT_METHOD::SHA256:	keyData.hashAlgorithm = "SHA256";	break;
		case CRYPT_METHOD::SHA384:	keyData.hashAlgorithm = "SHA384";	break;
		case CRYPT_METHOD::SHA512:	keyData.hashAlgorithm = "SHA512";	break;
		case CRYPT_METHOD::MD5:		keyData.hashAlgorithm = "MD5";		break;
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
				
				if (false == keyData.cipherChaining.empty())
				{
					CP_XML_ATTR("cipherChaining",	keyData.cipherChaining);
				}
				
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
bool WriteStandartEncryptionInfo(unsigned char* data, int &size, _ecmaCryptData & cryptData)
{
	if (!data || size < 1) return false;
	MemoryStream mem_stream(data, size, false);

	_UINT32 SizeHeader = 0, Flags = 0, SizeExtra = 0, AlgID = 0, AlgIDHash = 0, KeySize = 0, ProviderType = 0, Reserved1 = 0, Reserved2 = 0;

	bool fCryptoAPI = true, fDocProps = false, fExternal = false, fAES = cryptData.cipherAlgorithm != CRYPT_METHOD::RC4;

	SETBIT(Flags, 2, fCryptoAPI); 
	SETBIT(Flags, 3, fDocProps);  
	SETBIT(Flags, 4, fExternal);  
	SETBIT(Flags, 5, fAES);  

	mem_stream.WriteUInt32(SizeHeader);	

	KeySize = (cryptData.keySize == 5) ? 0 : cryptData.keySize * 8;
	
	std::string provider;// to utf16
	switch(cryptData.cipherAlgorithm)
	{
		case CRYPT_METHOD::RC4:
		{
			ProviderType = 0x0001; 
			AlgID = 0x6801;
		}break;
		case CRYPT_METHOD::DES_ECB:
		case CRYPT_METHOD::DES_CBC:
		{
			ProviderType = 0;//0x0018; 
			AlgID = 0x6601;
		}break;
		case CRYPT_METHOD::AES_ECB:
		case CRYPT_METHOD::AES_CBC:
		{
			ProviderType = 0x0018;
			switch(KeySize)
			{
			case 128: AlgID = 0x660E; break;
			case 192: AlgID = 0x660F; break;
			case 256: AlgID = 0x6610; break;
			}
			break;
		}break;
	}
	//ProviderType = 0x0018;

	switch(ProviderType)
	{
	case 0x0001: provider = "Microsoft Strong Cryptographic Provider"; break;
	case 0x0018: provider = "Microsoft Enhanced RSA and AES Cryptographic Provider"; break;
	}
	switch(cryptData.hashAlgorithm)
	{
		case CRYPT_METHOD::MD5:		AlgIDHash = 0x8003; break;
		case CRYPT_METHOD::SHA1:	AlgIDHash = 0x8004; break;
		case CRYPT_METHOD::SHA256:	AlgIDHash = 0x8004; break;
		case CRYPT_METHOD::SHA384:	AlgIDHash = 0x800D; break;
		case CRYPT_METHOD::SHA512:	AlgIDHash = 0x800E; break;
	}

	mem_stream.WriteUInt32(Flags);
	mem_stream.WriteUInt32(SizeExtra); 
	mem_stream.WriteUInt32(AlgID);
	mem_stream.WriteUInt32(AlgIDHash);
	mem_stream.WriteUInt32(KeySize);
	mem_stream.WriteUInt32(ProviderType);
	mem_stream.WriteUInt32(Reserved1);
	mem_stream.WriteUInt32(Reserved2);

	for (size_t i = 0; i < provider.length(); ++i)
	{
		mem_stream.WriteByte((unsigned char)provider[i]);
		mem_stream.WriteByte((unsigned char)0);
	}
	mem_stream.WriteByte((unsigned char)0); //null terminate
	mem_stream.WriteByte((unsigned char)0);

	SizeHeader	= mem_stream.GetPosition() - 4;

//EncryptionVerifier
	mem_stream.WriteUInt32((_UINT32)cryptData.saltSize);
	
	mem_stream.WriteBytes((unsigned char*)cryptData.saltValue.c_str(), cryptData.saltSize);
	
	mem_stream.WriteBytes((unsigned char*)cryptData.encryptedVerifierInput.c_str(), 0x10);

	mem_stream.WriteUInt32((_UINT32)cryptData.hashSize);
			
	//int szEncryptedVerifierHash = (ProviderType == 0x0001) ? 0x14 : 0x20; //RC4 | AES(DES) ..  md5?
	mem_stream.WriteBytes((unsigned char*)cryptData.encryptedVerifierValue.c_str(), (int)cryptData.encryptedVerifierValue.length()/*szEncryptedVerifierHash*/);

	size = mem_stream.GetPosition();

	mem_stream.Seek(0);
	mem_stream.WriteUInt32(SizeHeader);

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
			
	unsigned long szEncryptedVerifierHash		= (ProviderType == 0x0001) ? 0x14 : 0x20; // RC4 | AES(DES)
	unsigned long szEncryptedVerifierHashMax	= mem_stream.GetSize() - mem_stream.GetPosition();

	cryptData.encryptedVerifierValue = std::string((char*)data + mem_stream.GetPosition(), (std::max)(szEncryptedVerifierHash, szEncryptedVerifierHashMax));
	mem_stream.ReadBytes(szEncryptedVerifierHash, false);

	pos = mem_stream.GetPosition();
	
//------------------------------------------------------------------------------------------
	switch(AlgIDHash)
	{
	case 0x8003:
		cryptData.hashAlgorithm = CRYPT_METHOD::MD5; break;
	case 0x0000:
	case 0x8004:
		cryptData.hashAlgorithm = CRYPT_METHOD::SHA1; break;
	case 0x800C:
		cryptData.hashAlgorithm = CRYPT_METHOD::SHA256; break;
	case 0x800D:
		cryptData.hashAlgorithm = CRYPT_METHOD::SHA384; break;
	case 0x800E:
		cryptData.hashAlgorithm = CRYPT_METHOD::SHA512; break;
	}
	cryptData.spinCount = 50000;

	switch(AlgID)
	{
	case 0x6801:	
		cryptData.cipherAlgorithm = CRYPT_METHOD::RC4;		
		if (KeySize == 0)	cryptData.keySize = 5; //40 bit
		else				cryptData.keySize = KeySize / 8;
		break;
	case 0x6601:	
		cryptData.cipherAlgorithm = CRYPT_METHOD::DES_ECB;		
		cryptData.keySize = 8; 
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
	cryptData.dataHashAlgorithm = cryptData.hashAlgorithm;
	cryptData.dataBlockSize = cryptData.blockSize;
	cryptData.dataCipherAlgorithm = cryptData.cipherAlgorithm;
	cryptData.dataHashSize = cryptData.hashSize;
	cryptData.dataKeySize = cryptData.keySize;
	cryptData.dataSaltSize = cryptData.saltSize;
	return true;
}

bool ReadExtensibleEncryptionInfo(unsigned char* data, int size, _ecmaCryptData & cryptData)
{
	return false;
}


//--------------------------------------------------------------
bool ECMACryptFile::EncryptOfficeFile(const std::wstring &file_name_inp, const std::wstring &file_name_out, const std::wstring &password, const std::wstring &documentID)
{
	_ecmaCryptData cryptData;
	
	std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvMethodEncrypt);

	//if (sApplication == L"Weak")
	//{
	//	cryptData.bAgile			= false;
	//	cryptData.cipherAlgorithm	= CRYPT_METHOD::DES_ECB;
	//	cryptData.hashAlgorithm		= CRYPT_METHOD::SHA1;
	//	cryptData.keySize			= 0x08;
	//	cryptData.hashSize			= 0x14;
	//	cryptData.blockSize			= 0x10;
	//	cryptData.saltSize			= 0x10;
	//	cryptData.spinCount			= 50000;
	//}
	//else
	{
		cryptData.bAgile			= true;
		cryptData.spinCount			= 100000;
		cryptData.cipherAlgorithm	= cryptData.dataCipherAlgorithm = CRYPT_METHOD::AES_CBC;
		cryptData.hashAlgorithm		= cryptData.dataHashAlgorithm	= CRYPT_METHOD::SHA512;
		cryptData.keySize			= cryptData.dataKeySize			= 0x20;
		cryptData.hashSize			= cryptData.dataHashSize		= 0x40;
		cryptData.blockSize			= cryptData.dataBlockSize		= 0x10;
		cryptData.saltSize			= cryptData.dataSaltSize		= 0x10;
	}
	//cryptData.bAgile				= true;
	//cryptData.cipherAlgorithm		= CRYPT_METHOD::DES_CBC;
	////cryptData.hashAlgorithm		= CRYPT_METHOD::SHA512;
	////cryptData.keySize			= 0x08;
	////cryptData.hashSize			= 0x40;
	////cryptData.blockSize			= 0x10;
	////cryptData.saltSize			= 0x10;
	//cryptData.hashAlgorithm		= CRYPT_METHOD::SHA1;
	//cryptData.keySize				= 0x08;
	//cryptData.hashSize			= 0x14;
	//cryptData.blockSize			= 0x10;
	//cryptData.saltSize			= 0x10;

	//cryptData.bAgile			= false;
	//cryptData.cipherAlgorithm	= CRYPT_METHOD::AES_ECB;
	//cryptData.hashAlgorithm	= CRYPT_METHOD::SHA1;
	//cryptData.keySize			= 0x10;
	//cryptData.hashSize		= 0x14;
	//cryptData.blockSize		= 0x10;
	//cryptData.saltSize		= 0x10;
	//cryptData.spinCount		= 50000;

	//cryptData.bAgile				= false;
	//cryptData.cipherAlgorithm		= CRYPT_METHOD::RC4;
	//cryptData.hashAlgorithm		= CRYPT_METHOD::SHA1;
	//cryptData.keySize				= 7;
	//cryptData.hashSize			= 0x14;
	//cryptData.blockSize			= 0x10;
	//cryptData.saltSize			= 0x10;
	//cryptData.spinCount			= 50000;

	//cryptData.bAgile				= false;
	//cryptData.cipherAlgorithm		= CRYPT_METHOD::DES_ECB;
	//cryptData.hashAlgorithm		= CRYPT_METHOD::SHA1;
	//cryptData.keySize				= 0x08;
	//cryptData.hashSize			= 0x14;
	//cryptData.blockSize			= 0x10;
	//cryptData.saltSize			= 0x10;
	//cryptData.spinCount			= 50000;

	ECMAEncryptor cryptor;	

	//cryptor.SetCryptData(cryptDataGlobal);	//for test !!! 
	cryptor.SetCryptData(cryptData);			//basic settings
	
	cryptor.SetPassword(password);

	NSFile::CFileBinary file;
	if (!file.OpenFile(file_name_inp)) return false;

	_UINT64 lengthFileSize = file.GetFileSize();
	DWORD lengthData = lengthFileSize, lengthDataRead = 0 ;

	unsigned char* data = new unsigned char[lengthData];
	unsigned char* data_out	= NULL;
	
	file.ReadFile(data, lengthData, lengthDataRead);

	file.CloseFile();
	
	lengthData = cryptor.Encrypt(data, lengthData, data_out);

	delete[]data; data = NULL;
	
	if (NULL == data_out)
	{
		return false;
	}
	cryptor.UpdateDataIntegrity(data_out, lengthData);	
	
	bool bLargeFile = (lengthData > 3 * 1024 * 1024);

	bLargeFile = true; // test ??? 

//-------------------------------------------------------------------
	POLE::Storage *pStorage = NULL;
	CFCPP::CompoundFile *pStorageNew = NULL;

	if (bLargeFile)
	{
		pStorageNew = new CFCPP::CompoundFile(CFCPP::Ver_3, CFCPP::Default);
	}
	else
	{
		pStorage = new POLE::Storage(file_name_out.c_str());
		if (!pStorage)return false;

		if (!pStorage->open(true, true))
		{
			delete pStorage;
			return false;
		}
	}
//-------------------------------------------------------------------
	if (bLargeFile)
	{
		std::shared_ptr<CFCPP::CFStream> oPackage = pStorageNew->RootStorage()->AddStream(L"EncryptedPackage");
		oPackage->Write((char*)data_out, 0, lengthData);
	}
	else
	{
		POLE::Stream *pStream = new POLE::Stream(pStorage, L"EncryptedPackage", true, lengthData);

		pStream->write(data_out, lengthData);

		pStream->flush();
		delete pStream;
	}
//-------------------------------------------------------------------

	if (data_out)
	{
		delete []data_out;
		data_out = NULL;
	}	
	
	cryptor.GetCryptData(cryptData);

	if (bLargeFile)
	{
		std::shared_ptr<CFCPP::CFStream> oInfo = pStorageNew->RootStorage()->AddStream(L"EncryptionInfo");

		if (cryptData.bAgile)
		{
			_UINT16 VersionInfoMajor = 0x0004, VersionInfoMinor = 0x0004; //agile

			std::streamsize position = 0;
			oInfo->Write((char*)&VersionInfoMajor, position, 2); position += 2;
			oInfo->Write((char*)&VersionInfoMinor, position, 2); position += 2;

			_UINT32 nEncryptionInfoFlags = 64;
			oInfo->Write((char*)&nEncryptionInfoFlags, position, 4); position += 4;

			std::string strXml;
			WriteXmlEncryptionInfo(cryptData, strXml);

			oInfo->Write(strXml.c_str(), position, strXml.length()); position += strXml.length();
		}
		else
		{
			_UINT16 VersionInfoMajor = 0x0004, VersionInfoMinor = 0x0002; // standart

			std::streamsize position = 0;
			oInfo->Write((char*)&VersionInfoMajor, position, 2); position += 2;
			oInfo->Write((char*)&VersionInfoMinor, position, 2); position += 2;

			_UINT32 nEncryptionInfoFlags = 0;
			bool fCryptoAPI = true, fDocProps = false, fExternal = false, fAES = cryptData.cipherAlgorithm != CRYPT_METHOD::RC4;

			SETBIT(nEncryptionInfoFlags, 2, fCryptoAPI);
			SETBIT(nEncryptionInfoFlags, 3, fDocProps);
			SETBIT(nEncryptionInfoFlags, 4, fExternal);
			SETBIT(nEncryptionInfoFlags, 5, fAES);

			oInfo->Write((char*)&nEncryptionInfoFlags, position, 4); position += 4;

			int nEncryptionInfoSize = 4096;
			unsigned char* byteEncryptionInfo = new unsigned char[nEncryptionInfoSize];

			WriteStandartEncryptionInfo(byteEncryptionInfo, nEncryptionInfoSize, cryptData);

			oInfo->Write((char*)byteEncryptionInfo, position, 4); position += nEncryptionInfoSize;
			delete[]byteEncryptionInfo;
		}
	}
	else
	{
		POLE::Stream *pStream = new POLE::Stream(pStorage, L"EncryptionInfo", true);

		if (cryptData.bAgile)
		{
			_UINT16 VersionInfoMajor = 0x0004, VersionInfoMinor = 0x0004; //agile

			pStream->write((unsigned char*)&VersionInfoMajor, 2);
			pStream->write((unsigned char*)&VersionInfoMinor, 2);

			_UINT32 nEncryptionInfoFlags = 64;
			pStream->write((unsigned char*)&nEncryptionInfoFlags, 4);

			std::string strXml;
			WriteXmlEncryptionInfo(cryptData, strXml);

			pStream->write((unsigned char*)strXml.c_str(), strXml.length());
		}
		else
		{
			_UINT16 VersionInfoMajor = 0x0004, VersionInfoMinor = 0x0002; // standart

			pStream->write((unsigned char*)&VersionInfoMajor, 2);
			pStream->write((unsigned char*)&VersionInfoMinor, 2);

			_UINT32 nEncryptionInfoFlags = 0;
			bool fCryptoAPI = true, fDocProps = false, fExternal = false, fAES = cryptData.cipherAlgorithm != CRYPT_METHOD::RC4;

			SETBIT(nEncryptionInfoFlags, 2, fCryptoAPI);
			SETBIT(nEncryptionInfoFlags, 3, fDocProps);
			SETBIT(nEncryptionInfoFlags, 4, fExternal);
			SETBIT(nEncryptionInfoFlags, 5, fAES);

			pStream->write((unsigned char*)&nEncryptionInfoFlags, 4);

			int nEncryptionInfoSize = 4096;
			unsigned char* byteEncryptionInfo = new unsigned char[nEncryptionInfoSize];

			WriteStandartEncryptionInfo(byteEncryptionInfo, nEncryptionInfoSize, cryptData);

			pStream->write(byteEncryptionInfo, nEncryptionInfoSize);
			delete[]byteEncryptionInfo;

		}
		pStream->flush();
		delete pStream;
	}
//-------------------------------------------------------------------
	if (false == documentID.empty())
	{
		std::string utfDocumentID = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(documentID);
		if (bLargeFile)
		{

			std::shared_ptr<CFCPP::CFStream> oDocumentID = pStorageNew->RootStorage()->AddStream(L"DocumentID");
			oDocumentID->Write(utfDocumentID.c_str(), 0, utfDocumentID.length());
		}
		else
		{
			POLE::Stream *pStream = new POLE::Stream(pStorage, L"DocumentID", true, utfDocumentID.length());

			pStream->write((BYTE*)utfDocumentID.c_str(), utfDocumentID.length());

			pStream->flush();
			delete pStream;
		}
	}
//-------------------------------------------------------------------
	if (bLargeFile)
	{
		pStorageNew->Save(file_name_out);
		pStorageNew->Close();
		delete pStorageNew;
	}
	else
	{
		pStorage->close();
		delete pStorage;
	}
//
////test back---------------------------------------------------------------------------------test back
//	ECMADecryptor decryptor;
//	
//	decryptor.SetCryptData(cryptData);
//	
//	if (decryptor.SetPassword(password))
//	{
//		unsigned char* data_out2 = NULL;
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
bool ECMACryptFile::DecryptOfficeFile(const std::wstring &file_name_inp, const std::wstring &file_name_out, const std::wstring &password, bool & bDataIntegrity)
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
		if (pStream->fail())
		{
			delete pStream;
			delete pStorage;
			return false;
		}
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
			bool fCryptoAPI	= GETBIT(nEncryptionInfoFlags, 2); 
			bool fDocProps	= GETBIT(nEncryptionInfoFlags, 3); 
			bool fExternal	= GETBIT(nEncryptionInfoFlags, 4); 
			bool fAES		= GETBIT(nEncryptionInfoFlags, 5); 
			
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
	ECMADecryptor decryptor;
	
	decryptor.SetCryptData(cryptData);
	
	if (!decryptor.SetPassword(password))
	{
		if (password.empty())
		{
			if (!decryptor.SetPassword(L"VelvetSweatshop"))
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

		unsigned char* data = new unsigned char[lengthRead];
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

std::string ECMACryptFile::ReadAdditional(const std::wstring &file_name, const std::wstring &addit_name)
{
	POLE::Storage *pStorage = new POLE::Storage(file_name.c_str());
	
	if (!pStorage->open(false, false))
	{
		delete pStorage;
		return "";
	}
	std::string result;
	POLE::Stream *pStream = new POLE::Stream(pStorage, addit_name);
	if ((pStream) && (pStream->size() > 0))
	{
		_UINT64 lengthData, size = pStream->size();

		char* data = new char[size];
		
		lengthData = pStream->read((unsigned char*)data, size); 

		result = std::string(data, lengthData);
		delete []data;
		delete pStream;
	}
	delete pStorage;
	
	return result;
}
bool ECMACryptFile::WriteAdditional(const std::wstring &file_name, const std::wstring &addit_name, const std::string &addit_info)
{
	try
	{
		CFCPP::CompoundFile *pStorage = new CFCPP::CompoundFile(file_name, CFCPP::Update, (CFCPP::SectorRecycle | CFCPP::NoValidationException | CFCPP::EraseFreeSectors));
		if (!pStorage)return false;
		std::shared_ptr<CFCPP::CFStream> pAddit = pStorage->RootStorage()->GetStream(addit_name);

		if (pAddit)
		{
			pStorage->RootStorage()->Delete(addit_name);
		}
		pAddit = pStorage->RootStorage()->AddStream(addit_name);

		pAddit->Write(addit_info.c_str(), 0, addit_info.size());
		pStorage->Commit();
//todooo_2 flush
		pStorage->Save(file_name + L"~");
		pStorage->Close();
		delete pStorage;
//todooo_1 rename
		//NSFile::CFileBinary::Rename(file_name + L"~", file_name);
		NSFile::CFileBinary::Copy(file_name + L"~", file_name);
		NSFile::CFileBinary::Remove(file_name + L"~");
	}
	catch (...)
	{
		return false;
	}

	//POLE::Storage *pStorage = new POLE::Storage(file_name.c_str());
	//
	//if (!pStorage)return false;

	//if (!pStorage->open(true, false))
	//{
	//	delete pStorage;
	//	return false;
	//}

	//POLE::Stream *pStream = new POLE::Stream(pStorage, addit_name, true, addit_info.size());
	//
	//pStream->write((unsigned char*)addit_info.c_str(), addit_info.size());
	//pStream->setSize(addit_info.size());

	//pStream->flush();
	//delete pStream;

	//pStorage->close();
	//delete pStorage;

	return true;
}
