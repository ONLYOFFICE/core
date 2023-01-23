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
	{ //важно чтобы первыми читались именно settings (например для footnote)
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
