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

#include "../../Common/3dParty/pole/pole.h"
#include "../../Common/DocxFormat/Source/Base/Types_32.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

#include "../../OfficeCryptTransform/CryptTransform.h"

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
void ReadMapEntry(POLE::Stream *pStream, CryptReader::_mapEntry & m)
{
	if (!pStream) return;

	_UINT32 length = 0;
	pStream->read((unsigned char*)&length, 4); 

	_UINT32 refCount = 0;
	pStream->read((unsigned char*)&refCount, 4); 

	for (int i = 0 ; i < refCount; i++)
	{
		CryptReader::_refComponent r;
		pStream->read((unsigned char*)&r.type, 4); 

		r.ref = ReadUnicodeLP(pStream);
		m.refComponents.push_back(r);
	}
	m.dataSpaceName= ReadUnicodeLP(pStream);
}
//--------------------------------------------------------------
bool CryptReader::DecryptFile(std::wstring file_name, std::wstring folder_out, std::wstring password)
{
	POLE::Storage *pStorage = new POLE::Storage(file_name.c_str());
	
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
	Decryptor decryptor(1);

	decryptor.SetCryptData(keyData.saltValue, keyData.encryptedVerifierHashInput, keyData.encryptedVerifierHashValue);

	if (!decryptor.SetPassword(password)) 
	{
		delete pStorage;
		return false;
	}

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

	pStream = new POLE::Stream(pStorage, "EncryptionPackage");
	if (pStream)
	{
		_UINT64 length;
		pStream->read((unsigned char*)&length, 8); 

		while (true)
		{
		}

		delete pStream;
	}
//-------------------------------------------------------------------


	delete pStorage;

	return result;
}

bool CryptReader::ReadEncryptionInfo(std::string & xml_string)
{
	XmlUtils1::CXmlLiteReader xmlReader;

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

		}
		else if (L"keyEncryptors" == sName)
		{
			while( xmlReader.ReadNextSiblingNode( nCurDepth + 1 ) )
			{
				 if (L"keyEncryptor" == sName)
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
		return true;
	}
}

