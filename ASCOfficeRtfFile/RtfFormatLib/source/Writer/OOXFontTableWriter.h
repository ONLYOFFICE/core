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
#pragma once

#include "OOXRelsWriter.h"

class OOXFontTableWriter
{
public: 
	OOXRelsWriterPtr m_oRelsWriter;
	OOXFontTableWriter(OOXWriter& oWriter,RtfDocument& oDocument ):m_oWriter(oWriter)
	{
		m_oRelsWriter = OOXRelsWriterPtr( new OOXRelsWriter(  _T("fontTable.xml"), oDocument ) );
		oWriter.m_oCustomRelsWriter.push_back( m_oRelsWriter );
	}

	void AddContent( CString sText )
	{
		m_sFileXml.Append( sText );
	}
	bool Save(  CString sFolder )
	{
		CString pathWord = sFolder + FILE_SEPARATOR_STR + _T("word");

		if( false == m_sFileXml.IsEmpty() )
		{
			CFile file;
            if (file.CreateFile(pathWord + FILE_SEPARATOR_STR + _T("fontTable.xml"))) return false;

			m_oWriter.m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable"), _T("fontTable.xml") );
			m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml"), _T("/word/fontTable.xml") );

            std::wstring sXml = CreateXml();

            std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml);

            file.WriteFile((void*)sXmlUTF.c_str(), sXmlUTF.length());

			file.CloseFile();
			return true;
		}
		else
		{
			//todooo - default fontTable !!!
		}
		return false;
	}
	bool IsEmpty()
	{
		return m_sFileXml.IsEmpty();
	}
private: 
	CString m_sFileXml;

    OOXWriter& m_oWriter;

    std::wstring CreateXml()
	{
        std::wstring sResult;
        sResult.append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
        sResult.append( _T("<w:fonts xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">") );
        sResult.append( m_sFileXml.GetBuffer() );
        sResult.append( _T("</w:fonts>") );
		return  sResult;
	}
};
