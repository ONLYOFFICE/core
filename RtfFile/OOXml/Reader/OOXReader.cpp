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
#include "OOXReader.h"
#include "OOXSettingsReader.h"
#include "OOXFontTableReader.h"
#include "OOXStyleTableReader.h"
#include "OOXNumberingReader.h"
#include "OOXDocumentReader.h"
#include "OOXAppReader.h"
#include "OOXCoreReader.h"
#include "OOXFootnotesReader.h"
#include "OOXThemeReader.h"

OOXReader::OOXReader( RtfDocument& oDocument, std::wstring  path ): m_oDocument(oDocument)
{
	m_sPath = path;
	m_nCurItap = 0;
	m_nCurOleChartId = 0;
	m_nCurFittextId = 0;

	m_convertationManager = NULL;
	m_currentContainer = NULL;
}
bool OOXReader::Parse()
{
	OOX::CDocx inputDocxFile;
	inputDocxFile.Read(m_sPath);
	
	ReaderParameter oReaderParameter;

	oReaderParameter.oDocx		= &inputDocxFile;
	oReaderParameter.oRtf		= &m_oDocument;
	oReaderParameter.oReader	= this;

	if (inputDocxFile.m_oMain.settings)
	{ //important that settings are read first (e.g. for footnote)
		OOXSettingsReader oSettingsReader( inputDocxFile.m_oMain.settings );
		oSettingsReader.Parse( oReaderParameter );
	}

	if (inputDocxFile.m_oMain.fontTable)
	{
		OOXFontTableReader oFontTableReader( inputDocxFile.m_oMain.fontTable );
		oFontTableReader.Parse( oReaderParameter );
	}
	
	if (inputDocxFile.m_pTheme)
	{
		OOXThemeReader oThemeReader( inputDocxFile.m_pTheme );
		oThemeReader.Parse( oReaderParameter );
	}

	if (inputDocxFile.m_oMain.numbering)
	{
		OOXNumberingReader oNumberingReader( inputDocxFile.m_oMain.numbering );
		oNumberingReader.Parse(oReaderParameter );
	}
	
	if (inputDocxFile.m_oMain.styles)
	{
		OOXStyleTableReader oStyleTableReader( inputDocxFile.m_oMain.styles );
		oStyleTableReader.Parse( oReaderParameter );
	}
	
	if (inputDocxFile.m_pApp)
	{
		OOXAppReader oDocPropAppReader( inputDocxFile.m_pApp );
		oDocPropAppReader.Parse( oReaderParameter );
	}
	
	if (inputDocxFile.m_pCore)
	{
		OOXCoreReader oDocPropCoreReader( inputDocxFile.m_pCore );
		oDocPropCoreReader.Parse(oReaderParameter  );
	}
	
	if (inputDocxFile.m_oMain.footnotes)
	{
		OOXFootnotesReader oFootnotesReader( inputDocxFile.m_oMain.footnotes );
		oFootnotesReader.Parse( oReaderParameter);
	}

	if (inputDocxFile.m_oMain.endnotes)
	{
		OOXFootnotesReader oEndnotesReader( inputDocxFile.m_oMain.endnotes );
		oEndnotesReader.Parse( oReaderParameter);  
	}

	if (inputDocxFile.m_oMain.document)
	{
		m_currentContainer = dynamic_cast<OOX::IFileContainer*>(inputDocxFile.m_oMain.document);
		
		OOXDocumentReader oDocReader( inputDocxFile.m_oMain.document );
		oDocReader.Parse( oReaderParameter );
	}


	return true;
}
std::wstring OOXReader::GetFolder( std::wstring sDocPath )
{
    size_t nLastSlash = sDocPath.rfind('\\');
	return nLastSlash != std::wstring::npos ? sDocPath.substr(0, nLastSlash ) : sDocPath;
}
