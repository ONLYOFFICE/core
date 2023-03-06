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
#include "PPTFileReader.h"

#include "../../XlsFile/Format/Crypt/Decryptor.h"
#include "../../Common/SummaryInformation/PropertySetStream.h"

#include "../../../DesktopEditor/common/Directory.h"
#include "../Records/Drawing/ArtBlip.h"

#define CURRENT_USER_STREAM		L"Current User" 

#define DOCUMENT_STREAM			L"PowerPoint Document" 

#define PICTURE_STREAM			L"Pictures" 
#define HEADER_STREAM			L"Header" 

#define PP97_DUALSTORAGE		L"PP97_DUALSTORAGE"

#define ENCRYPTED_SUMMARY_STREAM	L"EncryptedSummary" 
#define DOCUMENT_SUMMARY_STREAM		L"DocumentSummaryInformation"
#define SUMMARY_STREAM				L"SummaryInformation"

using namespace PPT;

CPPTFileReader::CPPTFileReader(POLE::Storage *pStorage, std::wstring strTemp):
	   m_pStorage(pStorage),  
       m_bIsPPTFile(false),
	   m_nPresentationCodePage(1250),
	   m_pDocumentStream(),  m_pPictureStream(), m_pDocumentSummaryStream(), m_pEncryptedSummaryStream(),
	   m_strTmpDirectory(strTemp),
	   m_oDocumentInfo()
{ 
	m_bDualStorage = false;

	POLE::Stream *pStm = new POLE::Stream( m_pStorage, CURRENT_USER_STREAM);
			
	if ( ReadCurrentUser(pStm) ) 
	{
		m_bIsPPTFile = true; 
	}
	else
	{
		RELEASEOBJECT(pStm);
	
		std::wstring stream_name = std::wstring(PP97_DUALSTORAGE) + std::wstring(L"/") + std::wstring(CURRENT_USER_STREAM);
		pStm = new POLE::Stream( m_pStorage, stream_name);
		
		if (pStm == NULL)
			return;
		
		m_bDualStorage = true;
		if ( ReadCurrentUser(pStm))
		{
			m_bIsPPTFile = true; 
		}
	}
	
	RELEASEOBJECT(pStm);

    if (m_strTmpDirectory.empty())
	{
        m_strTmpDirectory = NSDirectory::GetTempPath();
	}

    m_strTmpDirectory = NSDirectory::CreateDirectoryWithUniqueName(m_strTmpDirectory);

}
CPPTFileReader::~CPPTFileReader()
{
	RELEASEOBJECT(m_pStorage);
	
    NSDirectory::DeleteDirectory(m_strTmpDirectory);
}

bool CPPTFileReader::IsPowerPoint()
{ 
	return m_bIsPPTFile;
} 
bool CPPTFileReader::IsEncrypted()
{
	if (m_oDocumentInfo.m_arUsers.empty()) 
		return m_oDocumentInfo.m_oCurrentUser.m_bIsEncrypt; //wps не выставляет флаг!

	return m_oDocumentInfo.m_arUsers[0]->m_bEncrypt;
}

bool CPPTFileReader::ReadPersists()
{
	XLS::CFStreamPtr pStream = GetDocumentStream();
	if (!pStream) return false;

	return m_oDocumentInfo.ReadFromStream(&m_oCurrentUser, pStream->stream_);
}
void CPPTFileReader::ReadDocument()
{
	ReadPictures();
	ReadDocumentSummary();
	
	m_oDocumentInfo.LoadDocument(m_strTmpDirectory);
}

bool CPPTFileReader::ReadCurrentUser(POLE::Stream *pStm)
{
	if (!pStm) return false;

	SRecordHeader oHeader; 
    bool isPP = false;
	
	if( oHeader.ReadFromStream(pStm))
	{ 
		m_oCurrentUser.ReadFromStream(oHeader, pStm);
		
		isPP = (m_oCurrentUser.m_nSize == 0x00000014 && (m_oCurrentUser.m_nLenUserName <= 255)); 
	} 

	return isPP; 
}

XLS::CFStreamPtr CPPTFileReader::GetDocumentStream()
{ 
	if (!m_pDocumentStream) 
	{ 
		m_pDocumentStream = GetStreamByName(DOCUMENT_STREAM);
	} 
	return m_pDocumentStream; 
}
XLS::CFStreamPtr CPPTFileReader::GetPictureStream()
{ 
	if (!m_pPictureStream) 
	{ 
		m_pPictureStream = GetStreamByName(PICTURE_STREAM);
	} 
	return m_pPictureStream; 
}
XLS::CFStreamPtr CPPTFileReader::GetStreamByName(const std::wstring & name)
{
	if (!m_bIsPPTFile) 
		return XLS::CFStreamPtr();

	std::wstring stream_name;

	if (m_bDualStorage)	stream_name = std::wstring(PP97_DUALSTORAGE) + std::wstring(L"/");

	try
	{
		POLE::Stream *pStream = new POLE::Stream(m_pStorage, stream_name + name);

		if (pStream->fail())
		{
			RELEASEOBJECT(pStream);
			return XLS::CFStreamPtr();
		}
		return XLS::CFStreamPtr(new XLS::CFStream(pStream));
	}
	catch (...)
	{
		return XLS::CFStreamPtr();
	}
}
XLS::CFStreamPtr CPPTFileReader::GetEncryptedSummaryStream()
{ 
	if (m_pEncryptedSummaryStream == NULL) 
	{ 
		m_pEncryptedSummaryStream = GetStreamByName(ENCRYPTED_SUMMARY_STREAM);
	} 
	return m_pEncryptedSummaryStream; 
}
XLS::CFStreamPtr CPPTFileReader::GetSummaryStream()
{
	if (!m_pDocumentSummaryStream)
	{
		m_pDocumentSummaryStream = GetStreamByName(SUMMARY_STREAM);
	}
	return m_pDocumentSummaryStream;
}
XLS::CFStreamPtr CPPTFileReader::GetDocumentSummaryStream()
{ 
	if (!m_pDocumentSummaryStream) 
	{ 
		m_pDocumentSummaryStream = GetStreamByName(DOCUMENT_SUMMARY_STREAM);
	} 
	return m_pDocumentSummaryStream; 
}
void CPPTFileReader::ReadEncryptedSummary()
{
	XLS::CFStreamPtr pStream = GetEncryptedSummaryStream();
	if (!pStream) return;

	SRecordHeader oHeader;

    if (oHeader.ReadFromStream(pStream) == false )
	{
		return;
	}
	CRecordEncryptedSummary info;

	info.ReadFromStream(oHeader, pStream);
}

void CPPTFileReader::ReadDocumentSummary()
{
	OLEPS::PropertySetStream summary_info;
	
	XLS::CFStreamPtr pStream = GetSummaryStream();
	if (pStream)
		summary_info.read(pStream);

	pStream = GetDocumentSummaryStream();
	if (pStream)
		summary_info.read(pStream, true);
	
	m_nPresentationCodePage = summary_info.GetCodePage();

	if (m_nPresentationCodePage == 0)
		m_nPresentationCodePage = 1250;

	m_oDocumentInfo.m_app_xml = summary_info.GetApp();
	m_oDocumentInfo.m_core_xml = summary_info.GetCore();
}

void CPPTFileReader::ReadPictures()
{
	if (m_oDocumentInfo.m_arUsers.empty()) return;
	
	XLS::CFStreamPtr pStream = GetPictureStream();
	if (!pStream) return;

	CRYPT::ECMADecryptor *pDecryptor = m_oDocumentInfo.m_arUsers[0]->m_pDecryptor;

	while (true)
	{
		//if (pStream->isEOF()) случаются неверно записанные стримы 
		//	break;

		unsigned long pos = pStream->getStreamPointer();	
		if (pos >= pStream->getStreamSize())
			break;

		SRecordHeader oHeader;
		if (pDecryptor)
		{
			BYTE pHeader[8];
			pStream->read(pHeader, 8); 

			pDecryptor->Decrypt((char*)pHeader, 8, 0);
		
			unsigned short rec = 0;
			memcpy(&rec,			pHeader	+ 0, 2);
			memcpy(&oHeader.RecType,pHeader + 2, 2);
			memcpy(&oHeader.RecLen,	pHeader + 4, 4);
			
			oHeader.RecInstance = rec >> 4;
			oHeader.RecVersion	= rec - (oHeader.RecInstance << 4);
		}
		else
			oHeader.ReadFromStream(pStream->stream_);

		if (oHeader.RecType == 0 && oHeader.RecLen == 0 )
			break;// окончание стрима забито нулями (выравнивание)

		CRecordOfficeArtBlip art_blip;
		art_blip.m_strTmpDirectory	= m_strTmpDirectory;
		art_blip.m_oDocumentInfo	= &m_oDocumentInfo;
			
		art_blip.ReadFromStream(oHeader, pStream->stream_);	
		m_oDocumentInfo.m_mapStoreImageFile[ pos ] = art_blip.m_sFileName;

		pos += (oHeader.RecLen + 8);
		pStream->seekFromBegin(pos);
	}
}
