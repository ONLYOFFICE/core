/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

_UINT32 COfficePPTFile::OpenFile(const std::wstring & sFileName, const std::wstring & password, bool &bMacro)
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
    
	pptReader->m_oDocumentInfo.m_strPassword = password;
	pptReader->m_oDocumentInfo.m_bMacroEnabled = bMacro;
		
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

	bMacro	= pptReader->m_oDocumentInfo.m_bMacroEnabled;
	m_Status = READMODE;

	return S_OK;
}
bool COfficePPTFile::CloseFile()
{
	m_Status = NULLMODE;

    PPT::CPPTFileReader* r = (PPT::CPPTFileReader*)m_pReader;	RELEASEOBJECT(r);
	m_pReader = NULL;
	return true;
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
		PPT::CPPTXWriter oPPTXWriter(sDstPath);

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
