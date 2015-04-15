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
		sResult.Append( _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>") );
		sResult.AppendChar('\n');
		sResult.Append( _T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">") );
		for( int i = 0; i < (int)m_aTargets.size(); i++ )
		{
            sResult.AppendFormat( _T("<Relationship Id=\"%ls\" Type=\"%ls\" Target=\"%ls\""), m_aIDs[i].GetBuffer(), m_aTypes[i].GetBuffer(), m_aTargets[i].GetBuffer());
			if( false == m_aModes[i] )
				sResult.Append( _T(" TargetMode=\"External\"") );
			sResult.Append( _T("/>") );
		}
		sResult.Append( _T("</Relationships>") );
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
