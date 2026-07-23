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
#include "ConvertationManager.h"

#pragma once

#include "RtfReader.h"
#include "RtfDocument.h"
#include "RtfWriter.h"
#include "../OOXml/Writer/OOXWriter.h"
#include "../OOXml/Reader/OOXReader.h"

#include "../../Common/OfficeFileErrorDescription.h"

const double g_cdMaxReadRtfPercent = 0.70;
const double g_cdMaxWriteRtfPercent = 0.30;
const double g_cdMaxReadOoxPercent = 0.70;
const double g_cdMaxWriteOoxPercent = 0.30;

//#define MEMORY_SAFE_CONVERTATION

#if defined (_WIN32) || defined(_WIN64)
    #pragma comment(lib, "Gdi32.lib")
#endif

RtfConvertationManager::RtfConvertationManager( )
{
	m_nUserLCID = -1;

	m_poOOXWriter = NULL;
	m_poOOXReader = NULL;

	m_poRtfWriter = NULL;
	m_poRtfReader = NULL;

	m_nDefaultFontSize = 0;
}
_UINT32 RtfConvertationManager::ConvertRtfToOOX( std::wstring sSrcFileName, std::wstring sDstPath )
{
    m_bParseFirstItem = true;

    RtfDocument oDocument;
	
	oDocument.m_nUserLCID = m_nUserLCID;
    oDocument.m_oProperty.SetDefaultRtf();

    RtfReader oReader( oDocument, sSrcFileName );
    OOXWriter oWriter( oDocument, sDstPath );

    if (m_sTempFolder.empty())
        m_sTempFolder = NSDirectory::GetTempPath();

	std::wstring sTempFolder = m_sTempFolder;

    oReader.m_sTempFolder = NSDirectory::CreateDirectoryWithUniqueName(sTempFolder);
    oWriter.m_sTempFolder = NSDirectory::CreateDirectoryWithUniqueName(sTempFolder);

	if (oReader.m_sTempFolder.empty())
	{
		oReader.m_sTempFolder = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetTempPath());
	}
	if (oWriter.m_sTempFolder.empty())
	{
		oWriter.m_sTempFolder = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetTempPath());
	}

	m_poRtfReader = &oReader;
	m_poOOXWriter = &oWriter;

	if (false == oReader.Load())
		return AVS_FILEUTILS_ERROR_CONVERT;
	if (!m_sDefaultFontName.empty())
		oDocument.m_oProperty.m_sDefFontName = m_sDefaultFontName;
	if (m_nDefaultFontSize > 0)
		oDocument.m_oProperty.m_nDefFontSize = m_nDefaultFontSize;
	oWriter.Save();

	NSDirectory::DeleteDirectory(oReader.m_sTempFolder);
	NSDirectory::DeleteDirectory(oWriter.m_sTempFolder);

	return S_OK;
}
_UINT32 RtfConvertationManager::ConvertOOXToRtf( std::wstring sDstFileName, std::wstring sSrcPath )
{
    m_bParseFirstItem = true;

    RtfDocument oDocument;
    oDocument.m_oProperty.SetDefaultOOX();

    OOXReader oReader( oDocument, sSrcPath );
    RtfWriter oWriter( oDocument, sDstFileName, sSrcPath );

    if (m_sTempFolder.length() < 1)
        m_sTempFolder = NSDirectory::GetTempPath();

	std::wstring sTempFolder = m_sTempFolder;

    oReader.m_sTempFolder = NSDirectory::CreateDirectoryWithUniqueName(sTempFolder);
    oWriter.m_sTempFolder = NSDirectory::CreateDirectoryWithUniqueName(sTempFolder);

	if (oReader.m_sTempFolder.empty())
	{
		oReader.m_sTempFolder = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetTempPath());
	}
	if (oWriter.m_sTempFolder.empty())
	{
		oWriter.m_sTempFolder = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetTempPath());
	}
	m_poOOXReader = &oReader;
    m_poRtfWriter = &oWriter;

    m_poOOXReader->m_convertationManager = this;

    bool result = oReader.Parse( );
    if( result )
    {
		result = oWriter.Save( );
    }

    NSDirectory::DeleteDirectory(oReader.m_sTempFolder);
    NSDirectory::DeleteDirectory(oWriter.m_sTempFolder);

    if ( result ) 
		return 0;
    else 
		return AVS_FILEUTILS_ERROR_CONVERT;
}
void RtfConvertationManager::OnCompleteItemRtf()
{
    if( true == m_bParseFirstItem )
    {
        m_bParseFirstItem = false;
        m_poOOXWriter->SaveByItemStart( );
    }
    m_poOOXWriter->SaveByItem();
}
void RtfConvertationManager::OnCompleteItemOOX()
{
    if( true == m_bParseFirstItem )
    {
        m_bParseFirstItem = false;
        m_poRtfWriter->SaveByItemStart( );
    }
    m_poRtfWriter->SaveByItem( );
}

