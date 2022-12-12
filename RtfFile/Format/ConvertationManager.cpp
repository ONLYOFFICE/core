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

    //m_poRtfReader->m_convertationManager = this;

    if (false == oReader.Load( )) return AVS_FILEUTILS_ERROR_CONVERT;

    //сохранение будет поэлементое в обработчике OnCompleteItemRtf
    //надо только завершить
    //if( true == m_bParseFirstItem )
    //{
    //    m_bParseFirstItem = false;
    //    oWriter.SaveByItemStart( );
    //}
    //m_poOOXWriter->SaveByItem();
    //oWriter.SaveByItemEnd( );

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

    bool succes = oReader.Parse( );
    if( true == succes)
    {
        succes = oWriter.Save( );
    }

    NSDirectory::DeleteDirectory(oReader.m_sTempFolder);
    NSDirectory::DeleteDirectory(oWriter.m_sTempFolder);

    if( true == succes) return 0;
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

