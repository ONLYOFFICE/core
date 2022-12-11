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

#include "PPTFormatLib.h"


#include "../Reader/PPTFileReader.h"
#include "../PPTXWriter/Converter.h"

#include "../../../Common/OfficeFileErrorDescription.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../DesktopEditor/common/Directory.h"

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

_UINT32 COfficePPTFile::OpenFile(const std::wstring & sFileName, const std::wstring & password, bool &bMacros)
{
	CloseFile();
	
    POLE::Storage *pStgFrom = new POLE::Storage(sFileName.c_str());
	if (pStgFrom == NULL) return AVS_ERROR_FILEFORMAT;
	
	try
	{
		if (false == pStgFrom->open(false, false))
		{
			return AVS_ERROR_FILEACCESS; //file access error or not compound file
		}
	}
	catch(...)
	{
		return AVS_ERROR_FILEACCESS; 
	}

    m_pReader = new PPT::CPPTFileReader(pStgFrom, m_strTempDirectory);
    PPT::CPPTFileReader* pptReader = (PPT::CPPTFileReader*)m_pReader;
    
	pptReader->m_oDocumentInfo.m_strTmpDirectory	= m_strTempDirectory;
	pptReader->m_oDocumentInfo.m_strPassword		= password;
	pptReader->m_oDocumentInfo.m_bMacros			= bMacros;
		
	if	(pptReader->IsPowerPoint() == false) 
	{ 
		RELEASEOBJECT(pptReader);
		m_pReader = NULL;
		return AVS_ERROR_FILEFORMAT; 
	} 

	if (pptReader->ReadPersists() == false) 
	{
		if (pptReader->IsEncrypted())
		{
			if (password.empty())	return AVS_ERROR_DRM;
			else					return AVS_ERROR_PASSWORD;
		}
		return AVS_ERROR_FILEFORMAT;
	}

	//pptReader->ReadEncryptedSummary();
	pptReader->ReadDocument();

	bMacros	= pptReader->m_oDocumentInfo.m_bMacros;
	m_Status = READMODE;

	return S_OK;
}
bool COfficePPTFile::CloseFile()
{
	m_Status = NULLMODE;

    PPT::CPPTFileReader* r = (PPT::CPPTFileReader*)m_pReader;	RELEASEOBJECT(r);
	m_pReader = NULL;
	return S_OK;
}

_UINT32 COfficePPTFile::LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath, std::wstring password, bool &bMacros)
{
    if (m_strTempDirectory.empty())
    {
        m_strTempDirectory = NSDirectory::GetTempPath();
    }

    _UINT32 nResult = OpenFile(sSrcFileName, password, bMacros);
	if (nResult != S_OK)
    {
		CloseFile();
		m_Status = NULLMODE;
		return nResult;
	}
    if (!((PPT::CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_arUsers.empty())
	{
		PPT::CPPTXWriter oPPTXWriter;
        oPPTXWriter.m_strTempDirectory = sDstPath;
		
        oPPTXWriter.m_xmlApp  = ((PPT::CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_app_xml;
        oPPTXWriter.m_xmlCore = ((PPT::CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_core_xml;

        oPPTXWriter.CreateFile(((PPT::CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_arUsers[0]);
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
