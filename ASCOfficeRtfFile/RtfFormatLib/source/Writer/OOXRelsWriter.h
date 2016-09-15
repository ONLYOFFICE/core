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

#include "../RtfDocument.h"

class OOXRelsWriter
{
private: 
	std::vector< CString >	m_aTargets;
	std::vector< CString >	m_aTypes;
	std::vector< CString >	m_aIDs;
	std::vector< bool >		m_aModes;
	CString					m_sFileName;
	RtfDocument&			m_oDocument;

public:		
	OOXRelsWriter( CString sFileName, RtfDocument& oDocument ):m_oDocument(oDocument)
	{
		m_sFileName = sFileName;
	}

	CString AddRelationship( CString sType, CString sTarget, bool bTargetModeInternal = true )
	{
		for( int i = 0 ;i < (int)m_aTargets.size(); i++ )
			if( sTarget == m_aTargets[i] )
				return m_aIDs[i];
		m_aTargets.push_back( sTarget );
		m_aTypes.push_back( sType );
		CString sId = m_oDocument.m_oIdGenerator.Generate_rId();
		m_aIDs.push_back( sId );
		m_aModes.push_back( bTargetModeInternal );
		return sId;
	}
	CString CreateXml()
	{
		CString sResult;
		sResult += _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		sResult.AppendChar('\n');
		sResult += _T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");
		
		for( int i = 0; i < (int)m_aTargets.size(); i++ )
		{
            sResult += _T("<Relationship Id=\"");
			sResult += m_aIDs[i];
			sResult += _T("\" Type=\"");
			sResult += m_aTypes[i];
			sResult += _T("\" Target=\"");
			sResult += m_aTargets[i];
			sResult += _T("\"");
			if( false == m_aModes[i] )
				sResult += _T(" TargetMode=\"External\"");
			sResult += _T("/>");
		}
		sResult += _T("</Relationships>");
		return sResult;
	}

	bool Save( CString sFolder )
	{
		if( m_aTargets.size() < 1 )return false;
		
		CString pathRels = sFolder + FILE_SEPARATOR_STR + _T("_rels");
        FileSystem::Directory::CreateDirectory(pathRels) ;

		CFile file;
        if (file.CreateFile(pathRels + FILE_SEPARATOR_STR + m_sFileName + _T(".rels"))) return false;

		CString sXml = CreateXml();

        std::string sXmlUTF = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.GetBuffer());

        file.WriteFile((void*)sXmlUTF.c_str(), sXmlUTF.length());
		file.CloseFile();
	}
};
typedef boost::shared_ptr<OOXRelsWriter> OOXRelsWriterPtr;
