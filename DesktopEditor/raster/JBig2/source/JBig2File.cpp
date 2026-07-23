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

// JBig2File.cpp : Implementation of CJBig2File


#include "JBig2File.h"

#include "Encoder/jbig2encoder.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <tchar.h>
#endif

#include "../../../common/File.h"

CJBig2File::CJBig2File()
{
	m_bDuplicateLineRemoval = false;
	m_bPDFMode				= true;
	m_bSymbolMode			= false;
	m_bRefine				= false;
	m_bUpscale2x			= false;
	m_bUpscale4x			= false;
	m_bSegment				= false;
	
	m_dTreshold				= 0.85;	
	m_nBwTreshold			= 188;
}

bool CJBig2File::MemoryToJBig2(unsigned char* pBufferBGRA ,int BufferSize, int nWidth, int nHeight, std::wstring sDstFileName, bool isBGRA)
{
	// check for valid input parameters

///////////////////////////////////////////////////////////
	if ( NULL == pBufferBGRA )	return false;

	int lBufferSize   = BufferSize;
	unsigned char *pSourceBuffer = pBufferBGRA;

	PIX  *pSource = pixCreate( nWidth, nHeight, 32 );
	if ( !pSource )	return false;

    unsigned char ShiftR = isBGRA ? 2 : 0;
    unsigned char ShiftG = 1;
    unsigned char ShiftB = isBGRA ? 0 : 2;
	for ( int nY = 0; nY < nHeight; nY++ )
	{
		for ( int nX = 0; nX < nWidth; nX++, pSourceBuffer += 3 )//todooo make 3 ? 4
		{
            pixSetRGBPixel( pSource, nX, nY, pSourceBuffer[ ShiftR ], pSourceBuffer[ ShiftG ], pSourceBuffer[ ShiftB ] );
		}
	}


	jbig2ctx *pContext = jbig2_init( m_dTreshold, 0.5, 0, 0, ! m_bPDFMode,  m_bRefine ? 10 : -1 );

	// For now write single image to JBig2
	// TODO: need to implement writing multiple images to 1 JBig2 file

	// Remove ColorMap
	PIX *pPixL = NULL;
	if ( NULL == ( pPixL = pixRemoveColormap( pSource, REMOVE_CMAP_BASED_ON_SRC ) ) ) 
	{
		pixDestroy( &pSource );
		jbig2_destroy( pContext );
		return false;
	}
	pixDestroy( &pSource );

	PIX *pPixT = NULL;
	if ( pPixL->d > 1 ) 
	{
		PIX *pGray = NULL;

		if ( pPixL->d > 8 ) 
		{
			pGray = pixConvertRGBToGrayFast( pPixL );
			if ( !pGray )
			{
				pixDestroy( &pSource );
				jbig2_destroy( pContext );
				return false;
			}
		} 
		else 
		{
			pGray = pixClone( pPixL );
		}

		if (  m_bUpscale2x ) 
		{
			pPixT = pixScaleGray2xLIThresh( pGray,  m_nBwTreshold );
		} 
		else if (  m_bUpscale4x ) 
		{
			pPixT = pixScaleGray4xLIThresh( pGray,  m_nBwTreshold );
		} 
		else 
		{
			pPixT = pixThresholdToBinary( pGray,  m_nBwTreshold );
		}

		pixDestroy( &pGray );
	} 
	else 
	{
		pPixT = pixClone( pPixL );
	}

	if ( m_sOutputTreshold.length() > 0 ) 
	{
		pixWrite( m_sOutputTreshold.c_str(), pPixT, IFF_BMP );
	}

	if (  m_bSegment && pPixL->d > 1 ) 
	{
		PIX *pGraphics = segment_image( pPixT, pPixL );
		if ( pGraphics ) 
		{
			char *sFilename;
			asprintf( &sFilename, "%s.%04d.%s", m_sBaseName.c_str(), 0, ".bmp" );
			pixWrite( sFilename, pGraphics, IFF_BMP );
			free( sFilename );
		} 
		if ( !pPixT ) 
		{
			// Do nothing
			return true;
		}
	}

	pixDestroy( &pPixL );

	if ( !m_bSymbolMode ) 
	{
		int nLength = 0;
		uint8_t *pBuffer = jbig2_encode_generic( pPixT, !m_bPDFMode, 0, 0, m_bDuplicateLineRemoval, &nLength );

		bool bRes = true;
        NSFile::CFileBinary file;
        if (file.CreateFileW(sDstFileName ) == true )
        {
            file.WriteFile(pBuffer, nLength);
            file.CloseFile();
			bRes = true;
        }
		else
			bRes = false;

		pixDestroy( &pPixT );
		if ( pBuffer ) free( pBuffer );
		jbig2_destroy( pContext );

		return bRes;
	}

	int nNumPages = 1;
	jbig2_add_page( pContext, pPixT );
	pixDestroy( &pPixT );

	int nLength = 0;
	uint8_t *pBuffer = jbig2_pages_complete( pContext, &nLength );
	if ( !pBuffer )
	{
		jbig2_destroy( pContext );
		return false;
	}

	if ( m_bPDFMode ) 
	{
		std::wstring sFileName = sDstFileName;//m_sBaseName + _T(".sym");

        NSFile::CFileBinary file;
        if ( file.CreateFileW(sFileName) == false)
		{
			free( pBuffer );
			jbig2_destroy( pContext );
			return false;
		}
        file.WriteFile( pBuffer, nLength );
        file.CloseFile();
	}
	free( pBuffer );

	for ( int nIndex = 0; nIndex < nNumPages; ++nIndex ) 
	{
		pBuffer = jbig2_produce_page( pContext, nIndex, -1, -1, &nLength );
		if ( m_bPDFMode ) 
		{
            std::wstring sFileName = m_sBaseName + L".0000";

            NSFile::CFileBinary file;
            if ( file.CreateFileW(sFileName) ==false)
            {
				free( pBuffer );
				jbig2_destroy( pContext );
				return false;
			}
            file.WriteFile( pBuffer, nLength );
            file.CloseFile();
		} 
		free( pBuffer );
	}

	jbig2_destroy( pContext );

	return true;
}


// CJBig2File

