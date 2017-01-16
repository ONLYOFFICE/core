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

#include "PPTFormatLib.h"


#include "./Reader/PPTFileReader.h"
#include "./PPTXWriter/Converter.h"

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

bool COfficePPTFile::OpenFile(std::wstring sFileName)
{
	CloseFile();
	
    POLE::Storage *pStgFrom = new POLE::Storage(sFileName.c_str());
	if (pStgFrom == NULL) return FALSE;
	
	pStgFrom->open(false,false);

	m_pReader = new CPPTFileReader(pStgFrom, m_strTempDirectory);
    ((CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_strFileDirectory = GetDirectory(sFileName.c_str());
	
	if	(!((CPPTFileReader*)m_pReader)->IsPowerPoint()) 
	{ 
		CPPTFileReader* r = (CPPTFileReader*)m_pReader;	RELEASEOBJECT(r);
		m_pReader = NULL;
		return FALSE; 
	} 

	((CPPTFileReader*)m_pReader)->ReadPersistDirectory(); 
	((CPPTFileReader*)m_pReader)->ReadSlideList();

	m_Status = READMODE;

	return TRUE;
}
bool COfficePPTFile::CloseFile()
{
	m_Status = NULLMODE;

	CPPTFileReader* r = (CPPTFileReader*)m_pReader;	RELEASEOBJECT(r);
	m_pReader = NULL;
	return S_OK;
}

HRESULT COfficePPTFile::LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath)
{
    if (m_strTempDirectory.length() < 1)
    {
        m_strTempDirectory = NSDirectory::GetTempPath();
    }

    bool bRes = OpenFile(sSrcFileName);
	if (!bRes)
    {
		CloseFile();
		m_Status = NULLMODE;
		return S_FALSE;
	}
	if (!((CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_arUsers.empty())
	{
		NSPresentationEditor::CPPTXWriter	oPPTXWriter;
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
