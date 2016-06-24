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


class OOXContentTypesWriter
{
public:
	void AddWriter()
	{
	}
	void AddContent( CString sType, CString sTarget )
	{
		for( int i = 0 ;i < (int)m_aTargets.size(); i++ )
			if( sTarget == m_aTargets[i] )
				return;
		m_aTargets.push_back( sTarget );
		m_aTypes.push_back( sType );
	}
	void AddExtension( CString sType, CString sTarget )
	{
		for( int i = 0 ;i < (int)m_aExtensions.size(); i++ )
			if( sTarget == m_aExtensions[i] )
				return;
		m_aExtensions.push_back( sTarget );
		m_aExtTypes.push_back( sType );
	}
	bool Save(CString sFolder)
	{
		CFile file;

        if (file.CreateFile(sFolder + FILE_SEPARATOR_STR + _T("[Content_Types].xml")) != S_OK) return false;

		 CString sXml = CreateXml();

         std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.GetBuffer());

         file.WriteFile((void*)sXmlUTF.c_str(), sXmlUTF.length());
		 
		 file.CloseFile();
		 return true;
	}

private: 
	std::vector< CString > m_aTargets;
	std::vector< CString > m_aTypes;

	std::vector< CString > m_aExtensions;
	std::vector< CString > m_aExtTypes;

	CString CreateXml()
	{
		CString sResult;
		sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>") );
		sResult.AppendChar('\n');
		sResult.Append( _T("<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">") );

		sResult.Append( _T("<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>") );
		sResult.Append( _T("<Default Extension=\"xml\" ContentType=\"application/xml\"/>") );

		for( int i = 0; i < (int)m_aExtensions.size(); i++ )
            sResult.AppendFormat( _T("<Default Extension=\"%ls\" ContentType=\"%ls\"/>"), m_aExtensions[i].GetBuffer(), m_aExtTypes[i].GetBuffer());

		for( int i = 0; i < (int)m_aTargets.size(); i++ )
            sResult.AppendFormat( _T("<Override PartName=\"%ls\" ContentType=\"%ls\"/>"), m_aTargets[i].GetBuffer(), m_aTypes[i].GetBuffer());
		sResult.Append( _T("</Types>") );
		return sResult;
	}
};
