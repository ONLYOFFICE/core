﻿/*
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

	if (inputDocxFile.m_pSettings)
	{ //важно чтобы первыми читались именно settings (например для footnote)
		OOXSettingsReader oSettingsReader( inputDocxFile.m_pSettings );
		oSettingsReader.Parse( oReaderParameter );
	}

	if (inputDocxFile.m_pFontTable)
	{
		OOXFontTableReader oFontTableReader( inputDocxFile.m_pFontTable );
		oFontTableReader.Parse( oReaderParameter );
	}
	
	if (inputDocxFile.m_pTheme)
	{
		OOXThemeReader oThemeReader( inputDocxFile.m_pTheme );
		oThemeReader.Parse( oReaderParameter );
	}

	if (inputDocxFile.m_pNumbering)
	{
		OOXNumberingReader oNumberingReader( inputDocxFile.m_pNumbering );
		oNumberingReader.Parse(oReaderParameter );
	}
	
	if (inputDocxFile.m_pStyles)
	{
		OOXStyleTableReader oStyleTableReader( inputDocxFile.m_pStyles );
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
	
	if (inputDocxFile.m_pFootnotes)
	{
		OOXFootnotesReader oFootnotesReader( inputDocxFile.m_pFootnotes );
		oFootnotesReader.Parse( oReaderParameter);
	}

	if (inputDocxFile.m_pEndnotes)
	{
		OOXFootnotesReader oEndnotesReader( inputDocxFile.m_pEndnotes );
		oEndnotesReader.Parse( oReaderParameter);  
	}

	if (inputDocxFile.m_pDocument)
	{
		m_currentContainer = dynamic_cast<OOX::IFileContainer*>(inputDocxFile.m_pDocument);
		
		OOXDocumentReader oDocReader( inputDocxFile.m_pDocument );
		oDocReader.Parse( oReaderParameter );
	}


	return true;
}
std::wstring OOXReader::GetFolder( std::wstring sDocPath )
{
    int nLastSlash = sDocPath.rfind('\\');
    std::wstring sLeft = sDocPath.substr(0, nLastSlash  );
	return sLeft;
}
