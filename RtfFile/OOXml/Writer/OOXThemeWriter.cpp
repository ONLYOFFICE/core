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

#include "OOXThemeWriter.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../OOXML/Base/Base.h"

OOXThemeWriter::OOXThemeWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
{
}
bool OOXThemeWriter::Save( std::wstring sFolder ) //todo доделать очистку поле ошибок
{
	std::wstring pathWord = sFolder + FILE_SEPARATOR_STR + _T("word");

	NSDirectory::CreateDirectory(pathWord) ;
	NSDirectory::CreateDirectory(pathWord + FILE_SEPARATOR_STR + _T("theme")) ;

	//todooo default theme !!!!!
	//
	//if( true == RtfUtility:: SaveResourceToFile( IDR_THEME, L"XML", sFolder + _T("\\word\\theme\\theme1.xml") ) )
	//{
	//	m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme"), _T("theme/theme1.xml") );
	//	m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.theme+xml"), _T("/word/theme/theme1.xml") );
	//}
	//else
	//{
	//	RemoveDirectory( sFolder + _T("\\word\\theme") );
	//}
	return true;
}
