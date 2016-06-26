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


#include "OpenXmlPackage.h"
#include "Converter.h"

#include "OleObjectMapping.h"
#include "VMLPictureMapping.h"

#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../DesktopEditor/common/File.h"

#include "../Common/XmlTools.h"


namespace DocFileFormat
{
	OpenXmlPackage::OpenXmlPackage(const WordDocument* _docFile) :  m_strOutputPath(_T("")), relID(1), _imageCounter(0), _headerCounter(0), _footerCounter(0),
		_oleCounter(0), docFile(NULL), DocumentRelationshipsFile( _T( "word/_rels/document.xml.rels" ) ), MainRelationshipsFile( _T( "_rels/.rels" ) ),
		FootnotesRelationshipsFile( _T( "word/_rels/footnotes.xml.rels" ) ), EndnotesRelationshipsFile( _T( "word/_rels/endnotes.xml.rels" ) ),
		CommentsRelationshipsFile( _T( "word/_rels/comments.xml.rels" ) ), NumberingRelationshipsFile( _T( "word/_rels/numbering.xml.rels" ) )
	{
		this->docFile = _docFile;

		this->DocumentContentTypesFile._defaultTypes.insert( make_pair( _T( "rels" ), std::wstring( OpenXmlContentTypes::Relationships ) ) );
		this->DocumentContentTypesFile._defaultTypes.insert( make_pair( _T( "xml" ), std::wstring( OpenXmlContentTypes::Xml ) ) );

		this->MainRelationshipsFile.Relationships.push_back( Relationship( std::wstring( _T( "rId1" ) ), OpenXmlRelationshipTypes::OfficeDocument, _T( "word/document.xml" ) ) );
	}


	void OpenXmlPackage::WritePackage()

	{
        FileSystem::Directory::CreateDirectory( std_string2string(m_strOutputPath) + FILE_SEPARATOR_STR +  _T( "_rels" ));
		this->WriteRelsFile( this->MainRelationshipsFile );

        FileSystem::Directory::CreateDirectory( std_string2string(m_strOutputPath)
                                                 + FILE_SEPARATOR_STR + _T( "word" )
                                                 + FILE_SEPARATOR_STR + _T( "_rels") );


		this->WriteRelsFile( this->DocumentRelationshipsFile );

		this->WriteRelsFile( this->FootnotesRelationshipsFile );

		this->WriteRelsFile( this->EndnotesRelationshipsFile );

		this->WriteRelsFile( this->CommentsRelationshipsFile );

		this->WriteRelsFile( this->NumberingRelationshipsFile );

		for ( std::list<RelationshipsFile>::const_iterator iter = this->HeaderRelationshipsFiles.begin(); iter != this->HeaderRelationshipsFiles.end(); iter++ )
		{
			this->WriteRelsFile( *iter );
		}

		for ( std::list<RelationshipsFile>::const_iterator iter = this->FooterRelationshipsFiles.begin(); iter != this->FooterRelationshipsFiles.end(); iter++ )
		{
			this->WriteRelsFile( *iter );
		}

		this->WriteContentTypesFile( this->DocumentContentTypesFile );
	}

	void OpenXmlPackage::SaveToFile( const std::wstring& outputDir, const std::wstring& fileName, const std::wstring& XMLContent )
	{
		if ( !XMLContent.empty() )
		{
			NSFile::CFileBinary file;

			std::wstring pathFileName = outputDir + FILE_SEPARATOR_STR + fileName;
			if (file.CreateFileW(pathFileName))
			{
				file.WriteStringUTF8(XMLContent);
				file.CloseFile();
			}
		}
	}


	void OpenXmlPackage::SaveToFile( const std::wstring& outputDir, const std::wstring& fileName, const void* buf, unsigned int size )
	{
		if ( buf != NULL )
		{

			int         fileHandle = 0;
			unsigned    bytesWritten = 0;

			std::wstring pathFileName = outputDir + FILE_SEPARATOR_STR + fileName;
			NSFile::CFileBinary file;

			if (file.CreateFileW(pathFileName))
			{
				file.WriteFile( (BYTE*)buf, size );

				file.CloseFile();
			}

		}
	}
	HRESULT OpenXmlPackage::SaveEmbeddedObject( const std::wstring& fileName, const std::string& data )
	{
		NSFile::CFileBinary file;
		file.CreateFileW(fileName);
		file.WriteFile((BYTE*)data.c_str(), data.size());
		file.CloseFile();
		return S_OK;
	}
	HRESULT OpenXmlPackage::SaveOLEObject( const std::wstring& fileName, const OleObjectFileStructure& oleObjectFileStructure )
	{
		POLE::Storage *storageOut = new POLE::Storage(fileName.c_str());
		
		if (storageOut == NULL || docFile == NULL) return S_FALSE;
		if (storageOut->open(true, true)==false)
		{	
			delete storageOut;
			return S_FALSE;
		}

		POLE::Storage *storageInp = docFile->GetStorage()->GetStorage();

		{
			std::string id(oleObjectFileStructure.objectID.begin(),oleObjectFileStructure.objectID.end());

			POLE::Stream* oleStorage = new POLE::Stream(storageInp, id);

			if (oleStorage)
			{
				std::string path = "ObjectPool/" + id;
				std::list<std::string> entries = storageInp->entries(path);
				for (std::list<std::string>::iterator it = entries.begin(); it != entries.end(); it++)
				{
					POLE::Stream *stream_inp = new POLE::Stream(storageInp, path + "/"+ (*it));
					if (stream_inp == NULL)continue;

					int size = stream_inp->size();
					
					POLE::Stream *stream_out = new POLE::Stream(storageOut, *it, true, size);

					if (stream_out)
					{
						unsigned char* buffer = new unsigned char[size];
						if (buffer)
						{
							stream_inp->read(buffer,size);
							stream_out->write(buffer,size);
							delete []buffer;
						}
					}
					stream_out->flush();

					delete stream_inp;
					delete stream_out;
				}
			}
		}

		storageOut->close();
		delete storageOut;
		return S_OK;
	}

	int OpenXmlPackage::RegisterDocument()
	{
		return AddPart( _T( "word" ), _T( "document.xml" ), WordprocessingMLContentTypes::MainDocument, _T( "" ) );
	}

	int OpenXmlPackage::RegisterFontTable()
	{
		return this->AddPart( _T( "word" ), _T( "fontTable.xml" ), WordprocessingMLContentTypes::FontTable, OpenXmlRelationshipTypes::FontTable );
	}

	int OpenXmlPackage::RegisterNumbering()
	{
		return this->AddPart( _T( "word" ), _T( "numbering.xml" ), WordprocessingMLContentTypes::Numbering, OpenXmlRelationshipTypes::Numbering );
	}

	int OpenXmlPackage::RegisterSettings()
	{
		return this->AddPart( _T( "word" ), _T( "settings.xml" ), WordprocessingMLContentTypes::Settings, OpenXmlRelationshipTypes::Settings );
	}

	int OpenXmlPackage::RegisterStyleSheet()
	{
		return this->AddPart( _T( "word" ), _T( "styles.xml" ), WordprocessingMLContentTypes::Styles, OpenXmlRelationshipTypes::Styles );
	}	 

	int OpenXmlPackage::RegisterHeader()
	{
		this->HeaderRelationshipsFiles.push_back( RelationshipsFile( ( std::wstring( _T( "word/_rels/header" ) ) + FormatUtils::IntToWideString( ++this->_headerCounter ) + std::wstring( _T( ".xml.rels" ) ) ) ) );

		return this->AddPart( _T( "word" ), ( std::wstring( _T( "header" ) ) + FormatUtils::IntToWideString( this->_headerCounter ) + std::wstring( _T( ".xml" ) ) ).c_str(), WordprocessingMLContentTypes::Header, OpenXmlRelationshipTypes::Header );
	}

	int OpenXmlPackage::AddHeaderPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		int relID = 0;

		if ( !this->HeaderRelationshipsFiles.empty() )
		{
			this->HeaderRelationshipsFiles.back().Relationships.push_back( Relationship( ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( ++this->HeaderRelationshipsFiles.back().RelID ) ), relationshipType, fileName, targetMode ) );

			relID = this->HeaderRelationshipsFiles.back().RelID;
		}

		return relID;
	}

	int OpenXmlPackage::RegisterFooter()
	{
		this->FooterRelationshipsFiles.push_back( RelationshipsFile( ( std::wstring( _T( "word/_rels/footer" ) ) + FormatUtils::IntToWideString( ++this->_footerCounter ) + std::wstring( _T( ".xml.rels" ) ) ) ) );

		return this->AddPart( _T( "word" ), ( std::wstring( _T( "footer" ) ) + FormatUtils::IntToWideString( _footerCounter ) + std::wstring( _T( ".xml" ) ) ).c_str(), WordprocessingMLContentTypes::Footer, OpenXmlRelationshipTypes::Footer );
	}

	int OpenXmlPackage::AddFooterPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		int relID = 0;

		if ( !this->FooterRelationshipsFiles.empty() )
		{
			this->FooterRelationshipsFiles.back().Relationships.push_back( Relationship( ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( ++this->FooterRelationshipsFiles.back().RelID ) ), relationshipType, fileName, targetMode ) );

			relID = this->FooterRelationshipsFiles.back().RelID;
		}

		return relID;  
	}

	int OpenXmlPackage::RegisterFootnotes()
	{
		return this->AddPart( _T( "word" ), _T( "footnotes.xml" ), WordprocessingMLContentTypes::Footnotes, OpenXmlRelationshipTypes::Footnotes );
	}

	int OpenXmlPackage::AddFootnotesPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		this->FootnotesRelationshipsFile.Relationships.push_back( Relationship( ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( ++FootnotesRelationshipsFile.RelID ) ), relationshipType, fileName, targetMode ) );

		return this->FootnotesRelationshipsFile.RelID;
	}

	int OpenXmlPackage::RegisterEndnotes()
	{
		return this->AddPart( _T( "word" ), _T( "endnotes.xml" ), WordprocessingMLContentTypes::Endnotes, OpenXmlRelationshipTypes::Endnotes );
	}

	int OpenXmlPackage::AddEndnotesPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		this->EndnotesRelationshipsFile.Relationships.push_back( Relationship( ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( ++this->EndnotesRelationshipsFile.RelID ) ), relationshipType, fileName, targetMode ) );

		return this->EndnotesRelationshipsFile.RelID;
	}

	int OpenXmlPackage::RegisterComments()
	{
		return this->AddPart( _T( "word" ), _T( "comments.xml" ), WordprocessingMLContentTypes::Comments, OpenXmlRelationshipTypes::Comments );
	}

	int OpenXmlPackage::AddCommentsPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		this->CommentsRelationshipsFile.Relationships.push_back( Relationship( ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( ++this->CommentsRelationshipsFile.RelID ) ), relationshipType, fileName, targetMode ) );

		return this->CommentsRelationshipsFile.RelID;
	}

	int OpenXmlPackage::AddNumberingPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		this->NumberingRelationshipsFile.Relationships.push_back( Relationship( ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( ++this->NumberingRelationshipsFile.RelID ) ), relationshipType, fileName, targetMode ) );

		return this->NumberingRelationshipsFile.RelID;
	}

	void OpenXmlPackage::WriteRelsFile( const RelationshipsFile& relationshipsFile )
	{
		if ( !relationshipsFile.Relationships.empty() )
		{
			XmlUtils::CXmlWriter writer;

			writer.WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) );
			writer.WriteNodeBegin( _T( "Relationships" ), TRUE );

			//write namespaces
			writer.WriteAttribute( _T( "xmlns" ), OpenXmlNamespaces::RelationshipsPackage );

			writer.WriteNodeEnd( _T( "" ), TRUE, FALSE );

			for ( std::list<Relationship>::const_iterator iter = relationshipsFile.Relationships.begin(); iter != relationshipsFile.Relationships.end(); iter++ )
			{
				writer.WriteNodeBegin( _T( "Relationship" ), TRUE );
				writer.WriteAttribute( _T( "Id" ), iter->Id.c_str() );
				writer.WriteAttribute( _T( "Type" ), iter->Type.c_str() );
				writer.WriteAttribute( _T( "Target" ), iter->Target.c_str() );

				if ( !iter->TargetMode.empty() )
				{
					writer.WriteAttribute( _T( "TargetMode" ), iter->TargetMode.c_str() );
				}

				writer.WriteNodeEnd( _T( "" ), TRUE );
			}

			writer.WriteNodeEnd( _T( "Relationships" ) );

			std::wstring pathFileName = m_strOutputPath + FILE_SEPARATOR_STR + relationshipsFile.FileName;
			
			writer.SaveToFile(std_string2string(pathFileName),TRUE);
		}
	}

	void OpenXmlPackage::WriteContentTypesFile( const ContentTypesFile& contentTypesFile )
	{
		XmlUtils::CXmlWriter writer;

		// write content types
		writer.WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) );
		writer.WriteNodeBegin( _T( "Types" ), TRUE );

		//write namespaces
		writer.WriteAttribute( _T( "xmlns" ), OpenXmlNamespaces::ContentTypes );

		writer.WriteNodeEnd( _T( "" ), TRUE, FALSE );

		for ( std::map<std::wstring, std::wstring>::iterator iter = this->DocumentContentTypesFile._defaultTypes.begin(); iter != this->DocumentContentTypesFile._defaultTypes.end(); iter++ )
		{
			writer.WriteNodeBegin( _T( "Default" ), TRUE );
			writer.WriteAttribute( _T( "Extension" ), iter->first.c_str() );
			writer.WriteAttribute( _T( "ContentType" ), this->DocumentContentTypesFile._defaultTypes[iter->first].c_str() );
			writer.WriteNodeEnd( _T( "" ), TRUE );
		}

		for ( std::map<std::wstring, std::wstring>::iterator iter = this->DocumentContentTypesFile._partOverrides.begin(); iter != this->DocumentContentTypesFile._partOverrides.end(); iter++ )
		{
			writer.WriteNodeBegin( _T( "Override" ), TRUE );
			writer.WriteAttribute( _T( "PartName" ), iter->first.c_str() );
			writer.WriteAttribute( _T( "ContentType" ), this->DocumentContentTypesFile._partOverrides[iter->first].c_str() );
			writer.WriteNodeEnd( _T( "" ), TRUE );
		}

		writer.WriteNodeEnd( _T( "Types" ) );

		std::wstring pathFileName = m_strOutputPath + FILE_SEPARATOR_STR + _T( "[Content_Types].xml" );
		
		writer.SaveToFile(std_string2string(pathFileName),TRUE);
	}

	int OpenXmlPackage::RegisterImage(const IMapping* mapping, Global::BlipType blipType)
	{
		std::wstring fileName = ( std::wstring( _T( "media/image" ) ) + FormatUtils::IntToWideString( ++_imageCounter ) + VMLPictureMapping::GetTargetExt( blipType ) );

		DocumentContentTypesFile._defaultTypes.insert( std::make_pair( VMLPictureMapping::GetTargetExt( blipType ).erase( 0, 1 ), VMLPictureMapping::GetContentType( blipType ) ) );
		DocumentContentTypesFile._defaultTypes.insert( std::make_pair( std::wstring( _T( "vml" ) ), std::wstring( OpenXmlContentTypes::Vml ) ) );

		return AddPart( mapping, _T( "word" ), fileName, VMLPictureMapping::GetContentType( blipType ), OpenXmlRelationshipTypes::Image );
	}

	int OpenXmlPackage::RegisterOLEObject(const IMapping* mapping, const std::wstring& objectType)
	{
		std::wstring fileName = ( std::wstring( _T( "embeddings/oleObject" ) ) + FormatUtils::IntToWideString( ++_oleCounter ) + OleObjectMapping::GetTargetExt(objectType));

		DocumentContentTypesFile._defaultTypes.insert( std::make_pair( OleObjectMapping::GetTargetExt( objectType ).erase( 0, 1 ), OleObjectMapping::GetContentType(objectType)));

		return AddPart( mapping, _T( "word" ), fileName, OleObjectMapping::GetContentType( objectType ), OpenXmlRelationshipTypes::OleObject );
	}
	int OpenXmlPackage::RegisterPackage(const IMapping* mapping, const std::wstring& objectType)
	{
		std::wstring fileName = ( std::wstring( _T( "embeddings/oleObject" ) ) + FormatUtils::IntToWideString( ++_oleCounter ) + OleObjectMapping::GetTargetExt(objectType));

		DocumentContentTypesFile._defaultTypes.insert( std::make_pair( OleObjectMapping::GetTargetExt( objectType ).erase( 0, 1 ), OleObjectMapping::GetContentType(objectType)));

		return AddPart( mapping, _T( "word" ), fileName, OleObjectMapping::GetContentType( objectType ), OpenXmlRelationshipTypes::Package);
	}
	int OpenXmlPackage::RegisterExternalOLEObject(const IMapping* mapping, const std::wstring& objectType, const std::wstring& uri)
	{
		std::wstring fullUri	=	std::wstring(_T("file:///")) + uri;
		std::wstring fileName	=	ReplaceString(fullUri, _T(" "), _T("%20"));

		DocumentContentTypesFile._defaultTypes.insert(std::make_pair(OleObjectMapping::GetTargetExt(objectType).erase(0, 1), OleObjectMapping::GetContentType(objectType)));

		return AddPart(mapping, _T(""), fileName, OleObjectMapping::GetContentType(objectType), OpenXmlRelationshipTypes::OleObject, _T("External"));
	}

	int OpenXmlPackage::AddPart( const std::wstring& packageDir, const std::wstring& fileName, const std::wstring& contentType, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		if (( contentType != _T( "" ) ) && 
			( contentType != OpenXmlContentTypes::Xml )				&&
			( contentType != OpenXmlContentTypes::MSWordDocx )		&& 
			( contentType != OpenXmlContentTypes::OleObject )		&& 
			( contentType != OpenXmlContentTypes::MSExcel )			&& 
			( contentType != OpenXmlContentTypes::MSWord )			&&
			( contentType != OpenXmlContentTypes::MSPowerpoint ) )
		{
			std::wstring  partOverride;

			if (packageDir != std::wstring ( _T( "" ) ) )
			{
				partOverride = std::wstring ( _T( "/" ) ) + packageDir + std::wstring( _T( "/" ) ); 
			}

			partOverride += fileName; 

			this->DocumentContentTypesFile._partOverrides.insert( make_pair( partOverride, contentType ) );
		}

		if ( relationshipType != _T( "" ) )
		{
			this->DocumentRelationshipsFile.Relationships.push_back( Relationship( (std::wstring ( _T( "rId" ) ) + FormatUtils::IntToWideString( ++this->relID ) ), relationshipType, fileName, targetMode ) );
		}

		return this->relID;
	}

	int OpenXmlPackage::AddPart( const IMapping* mapping, const std::wstring& packageDir, const std::wstring& fileName, const std::wstring& contentType, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		int relID = 0;

		if ( mapping != NULL )
		{
			if ( typeid(*mapping) == typeid(MainDocumentMapping) )
			{
				relID = this->AddPart( packageDir, fileName, contentType, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(HeaderMapping) )
			{
				relID = this->AddHeaderPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(FooterMapping) )
			{
				relID = this->AddFooterPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(FootnotesMapping) )
			{
				relID = this->AddFootnotesPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(EndnotesMapping) )
			{
				relID = this->AddEndnotesPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(CommentsMapping) )
			{
				relID = this->AddCommentsPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(NumberingMapping) )
			{
				relID = this->AddNumberingPart( fileName, relationshipType, targetMode );
			}
		}

		return relID;
	}
}
