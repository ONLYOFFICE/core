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

#include "PPTFormatLib.h"


#include "./Reader/PPTFileReader.h"
#include "./PPTXWriter/Converter.h"

#include "../../../Common/OfficeFileErrorDescription.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"

#ifndef READMODE
    #define READMODE 1
#endif

#ifndef NULLMODE
    #define NULLMODE 0
#endif

COfficePPTFile::~COfficePPTFile()
{
    CloseFile();
}

long COfficePPTFile::OpenFile(const std::wstring & sFileName, const std::wstring & password)
{
	CloseFile();
	
    POLE::Storage *pStgFrom = new POLE::Storage(sFileName.c_str());
	if (pStgFrom == NULL) return AVS_ERROR_FILEFORMAT;
	
	pStgFrom->open(false, false);

	m_pReader = new CPPTFileReader(pStgFrom, m_strTempDirectory);
	CPPTFileReader* pptReader = (CPPTFileReader*)m_pReader;	
    
	pptReader->m_oDocumentInfo.m_strFileDirectory	= GetDirectory(sFileName.c_str());
	pptReader->m_oDocumentInfo.m_strPassword		= password;
		
	if	(pptReader->IsPowerPoint() == false) 
	{ 
		RELEASEOBJECT(pptReader);
		m_pReader = NULL;
		return AVS_ERROR_FILEFORMAT; 
	} 

	if (pptReader->ReadPersists() == false) 
		return AVS_ERROR_FILEFORMAT;

	if (pptReader->IsEncrypted())
	{
		CEncryptionHeader *pEncryptionHeader =  pptReader->GetEncryptionHeader();
		if (!pEncryptionHeader)	return AVS_ERROR_FILEFORMAT;
		if (password.empty())	return AVS_ERROR_DRM;

		if (pEncryptionHeader->bStandard)
		{
			CRYPT::RC4Decryptor Decryptor(pEncryptionHeader->crypt_data_rc4, password, 1);

			if (Decryptor.IsVerify() == false) 
			{
				return AVS_ERROR_PASSWORD;
			}			
			if (DecryptOfficeFile(&Decryptor) == false)	return AVS_ERROR_DRM;
			
			return AVS_ERROR_PASSWORD;
			//return OpenFile(m_sTempDecryptFileName, L"");
		}
		else
		{
			CRYPT::ECMADecryptor Decryptor;

			Decryptor.SetCryptData(pEncryptionHeader->crypt_data_aes);

			if (Decryptor.SetPassword(password) == false)
			{
				return AVS_ERROR_PASSWORD;
			}
			if (DecryptOfficeFile(&Decryptor) == false)	return AVS_ERROR_DRM;

			return AVS_ERROR_PASSWORD;
			pptReader->ReadDocument(&Decryptor);
			//return OpenFile(m_sTempDecryptFileName, L"");
		}
	}
	else
	{	
		pptReader->ReadDocument(NULL);

		m_Status = READMODE;
	}

	return S_OK;
}
bool COfficePPTFile::CloseFile()
{
	m_Status = NULLMODE;

	CPPTFileReader* r = (CPPTFileReader*)m_pReader;	RELEASEOBJECT(r);
	m_pReader = NULL;
	return S_OK;
}

HRESULT COfficePPTFile::LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath, std::wstring password)
{
    if (m_strTempDirectory.empty())
    {
        m_strTempDirectory = NSDirectory::GetTempPath();
    }

    long nResult = OpenFile(sSrcFileName, password);
	if (nResult != S_OK)
    {
		CloseFile();
		m_Status = NULLMODE;
		return nResult;
	}
	if (!((CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_arUsers.empty())
	{
		NSPresentationEditor::CPPTXWriter oPPTXWriter;
        oPPTXWriter.m_strTempDirectory = sDstPath;
		
		
		oPPTXWriter.CreateFile(((CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_arUsers[0]);	
		oPPTXWriter.CloseFile();
	}
	return S_OK;
}

std::wstring COfficePPTFile::GetDirectory(std::wstring strFileName)
{
    int nIndex = strFileName.rfind(FILE_SEPARATOR_CHAR);
	if (-1 != nIndex)
	{
        return strFileName.substr(0, nIndex + 1);
	}
	return strFileName;
}

bool COfficePPTFile::DecryptOfficeFile(CRYPT::Decryptor* Decryptor)
{
	if (!m_pReader) return false;
	CPPTFileReader* pptReader = (CPPTFileReader*)(m_pReader);

	if (m_strTempDirectory.empty())
	{
        m_strTempDirectory = NSFile::CFileBinary::GetTempPath();
	}
	m_sTempDecryptFileName	= m_strTempDirectory + FILE_SEPARATOR_STR + L"~tempFile.ppt";
	
	POLE::Storage *storageIn	= pptReader->m_pPowerPointStg;
	POLE::Storage *storageOut	= new POLE::Storage(m_sTempDecryptFileName.c_str());

	if (!storageOut || !storageIn) return false;
	
	if (!storageOut->open(true, true))
	{
		delete storageOut;
		return false;
	}
	std::list<std::string> listStream = storageIn->entries();

	for (std::list<std::string>::iterator it = listStream.begin(); it != listStream.end(); it++)
	{
		if (storageIn->isDirectory(*it)) 
		{
			std::list<std::string> list_entry = storageIn->GetAllStreams(*it);
			
			for (std::list<std::string>::iterator it2 = list_entry.begin(); it2 != list_entry.end(); it2++)
			{
				DecryptStream(Decryptor, *it2, storageIn, storageOut);
			}
		}
		else 
			DecryptStream(Decryptor, *it, storageIn, storageOut);

	}
	storageOut->close();
	delete storageOut;

	return true;
}


bool COfficePPTFile::DecryptStream(CRYPT::Decryptor* Decryptor, std::string streamName, POLE::Storage * storageIn, POLE::Storage * storageOut)
{
	POLE::Stream *stream = new POLE::Stream(storageIn, streamName);
	if (!stream) return false;

	stream->seek(0);
	int sz_stream = stream->size();
	
	POLE::Stream *streamNew = new POLE::Stream(storageOut, streamName, true, sz_stream);
	if (!streamNew) return false;

	unsigned char* data_stream = new unsigned char[sz_stream];
	stream->read(data_stream, sz_stream);

	unsigned char* data_store = NULL;
	int sz_data_store = 0;
	
	//if ("WordDocument" == streamName)
	//{
	//	sz_data_store = 68;
	//	data_store = new unsigned char[sz_data_store];
	//}
	
	if (data_store)
		memcpy(data_store, data_stream, sz_data_store);

	Decryptor->Decrypt((char*)data_stream, sz_stream, 0);
	
	if (data_store)
		memcpy(data_stream, data_store, sz_data_store);

	streamNew->write(data_stream, sz_stream);

	RELEASEARRAYOBJECTS(data_store);
	RELEASEARRAYOBJECTS(data_stream);

	streamNew->flush();
			
	delete streamNew;
	delete stream;
	
	return true;
}


