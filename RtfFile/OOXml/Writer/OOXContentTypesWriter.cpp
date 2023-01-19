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

#include "OOXContentTypesWriter.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../OOXML/Base/Base.h"

void OOXContentTypesWriter::AddWriter()
{
}
void OOXContentTypesWriter::AddContent( std::wstring sType, std::wstring sTarget )
{
	for (size_t i = 0 ;i < (int)m_aTargets.size(); i++ )
		if( sTarget == m_aTargets[i] )
			return;
	m_aTargets.push_back( sTarget );
	m_aTypes.push_back( sType );
}
void OOXContentTypesWriter::AddExtension( std::wstring sType, std::wstring sTarget )
{
	for (size_t i = 0 ;i < (int)m_aExtensions.size(); i++ )
		if( sTarget == m_aExtensions[i] )
			return;
	m_aExtensions.push_back( sTarget );
	m_aExtTypes.push_back( sType );
}
bool OOXContentTypesWriter::Save(std::wstring sFolder)
{
	NSFile::CFileBinary file;

	if (false == file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("[Content_Types].xml"))) return false;

	std::wstring sXml = CreateXml();

	std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

	file.WriteFile((const void*)sXmlUTF.c_str(), (DWORD)sXmlUTF.length());

	file.CloseFile();
	return true;
}
std::wstring OOXContentTypesWriter::CreateXml()
{
	std::wstring sResult;
	sResult += _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n");

	sResult += _T("<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">");

	sResult += _T("<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>");
	sResult += _T("<Default Extension=\"xml\" ContentType=\"application/xml\"/>");

	for (size_t i = 0; i < m_aExtensions.size(); i++ )
	{
		sResult += _T("<Default Extension=\"");
		sResult += m_aExtensions[i];
		sResult += _T("\" ContentType=\"");
		sResult += m_aExtTypes[i];
		sResult += _T("\"/>");
	}

	for (size_t i = 0; i < m_aTargets.size(); i++ )
	{
		sResult += _T("<Override PartName=\"");
		sResult += m_aTargets[i];
		sResult += _T("\" ContentType=\"");
		sResult += m_aTypes[i];
		sResult += _T("\"/>");
	}
	sResult += _T("</Types>");
	return sResult;
}

