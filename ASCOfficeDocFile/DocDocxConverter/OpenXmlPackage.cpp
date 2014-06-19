#include "stdafx.h"

#include "OpenXmlPackage.h"
#include "MainDocumentMapping.h"
#include "HeaderMapping.h"
#include "FooterMapping.h"

inline static std::wstring ReplaceString(std::wstring subject, const std::wstring& search, const std::wstring& replace) 
{
	size_t pos = 0;

	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}

	return subject;
}

namespace DocFileFormat
{
	void OpenXmlPackage::GetOutputDir()
	{
		wstring::size_type pos = m_strFileName.find_last_of( _T( "\\" ) );
		m_strOutputPath = std::wstring( pos, NULL );
		copy(m_strFileName.begin(), (m_strFileName.begin() + pos ),m_strOutputPath.begin() );
	}

	OpenXmlPackage::OpenXmlPackage(const WordDocument* _docFile) : m_strFileName(_T("")), m_strOutputPath(_T("")), relID(1), _imageCounter(0), _headerCounter(0), _footerCounter(0),
		_oleCounter(0), docFile(NULL), DocumentRelationshipsFile( _T( "word/_rels/document.xml.rels" ) ), MainRelationshipsFile( _T( "_rels/.rels" ) ),
		FootnotesRelationshipsFile( _T( "word/_rels/footnotes.xml.rels" ) ), EndnotesRelationshipsFile( _T( "word/_rels/endnotes.xml.rels" ) ),
		CommentsRelationshipsFile( _T( "word/_rels/comments.xml.rels" ) ), NumberingRelationshipsFile( _T( "word/_rels/numbering.xml.rels" ) )
	{
		this->docFile = _docFile;

		this->DocumentContentTypesFile._defaultTypes.insert( make_pair( _T( "rels" ), std::wstring( OpenXmlContentTypes::Relationships ) ) );
		this->DocumentContentTypesFile._defaultTypes.insert( make_pair( _T( "xml" ), std::wstring( OpenXmlContentTypes::Xml ) ) );

		this->MainRelationshipsFile.Relationships.push_back( Relationship( std::wstring( _T( "rId1" ) ), OpenXmlRelationshipTypes::OfficeDocument, _T( "word/document.xml" ) ) );
	}

#ifdef CREATE_ZIPPED_DOCX

	void OpenXmlPackage::WritePackage( zipFile zf )

#else

	void OpenXmlPackage::WritePackage()

#endif  // CREATE_ZIPPED_DOCX

	{
#ifdef CREATE_UNZIPPED_XMLS

		CreateDirectory( (m_strOutputPath + std::wstring( _T( "\\_rels" ) ) ).c_str(), NULL );

#endif //CREATE_UNZIPPED_XMLS

		this->WriteRelsFile( this->MainRelationshipsFile );

#ifdef CREATE_UNZIPPED_XMLS

		CreateDirectory( (m_strOutputPath + std::wstring( _T( "\\word\\_rels" ) ) ).c_str(), NULL );

#endif //CREATE_UNZIPPED_XMLS

		this->WriteRelsFile( this->DocumentRelationshipsFile );

		this->WriteRelsFile( this->FootnotesRelationshipsFile );

		this->WriteRelsFile( this->EndnotesRelationshipsFile );

		this->WriteRelsFile( this->CommentsRelationshipsFile );

		this->WriteRelsFile( this->NumberingRelationshipsFile );

		for ( list<RelationshipsFile>::const_iterator iter = this->HeaderRelationshipsFiles.begin(); iter != this->HeaderRelationshipsFiles.end(); iter++ )
		{
			this->WriteRelsFile( *iter );
		}

		for ( list<RelationshipsFile>::const_iterator iter = this->FooterRelationshipsFiles.begin(); iter != this->FooterRelationshipsFiles.end(); iter++ )
		{
			this->WriteRelsFile( *iter );
		}

		this->WriteContentTypesFile( this->DocumentContentTypesFile );
	}

#ifdef CREATE_ZIPPED_DOCX

	void OpenXmlPackage::SaveToFile( const std::wstring& outputDir, const std::wstring& fileName, zipFile zf, const std::wstring& XMLContent )

#else

	void OpenXmlPackage::SaveToFile( const std::wstring& outputDir, const std::wstring& fileName, const std::wstring& XMLContent )

#endif  //CREATE_ZIPPED_DOCX 

	{
		if ( !XMLContent.empty() )
		{
			size_t textSize = XMLContent.length()*4 + 1;
			char *UTF8Text = new char[textSize];
			WideCharToMultiByte( CP_UTF8, 0, XMLContent.c_str(), -1,
				UTF8Text, textSize, NULL, NULL );

#ifdef CREATE_UNZIPPED_XMLS

			FILE* pFile = NULL;

			if ( _wfopen_s( &pFile, ( outputDir + std::wstring( _T( "\\" ) ) + fileName ).c_str(), _T( "w" ) ) == 0 )
			{
				fprintf( pFile, "%s", UTF8Text );
				fclose( pFile );
			}

#endif //CREATE_UNZIPPED_XMLS

#ifdef CREATE_ZIPPED_DOCX

			wstring zipFileName( fileName );

			replace( zipFileName.begin(), zipFileName.end(), _T( '\\' ), _T( '/' ) );

			if ( zipFileName[0] == _T( '/' ) )
			{
				copy( ( zipFileName.begin() + 1 ), zipFileName.end(), zipFileName.begin() );
				*( zipFileName.end() - 1 ) = _T( '\0' );
			}

			zip_fileinfo zi;

			zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
				zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
			zi.dosDate = 0;
			zi.internal_fa = 0;
			zi.external_fa = 0;

			SYSTEMTIME currTime;

			GetLocalTime( &currTime );

			zi.tmz_date.tm_sec = currTime.wSecond;
			zi.tmz_date.tm_min = currTime.wMinute;
			zi.tmz_date.tm_hour = currTime.wHour;
			zi.tmz_date.tm_mday = currTime.wDay;
			zi.tmz_date.tm_mon = currTime.wMonth;
			zi.tmz_date.tm_year = currTime.wYear;

			unsigned int zipFileNameSize = zipFileName.length() + 1;
			int err = ZIP_ERRNO;

			char *Buf = new char[zipFileNameSize];
			size_t convertedCout;
			wcstombs_s( &convertedCout, Buf, zipFileNameSize, zipFileName.c_str(), zipFileNameSize );

			err = zipOpenNewFileInZip( zf, Buf, &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION );

			err = zipWriteInFileInZip( zf, UTF8Text, strnlen( UTF8Text, textSize ) );

			err = zipCloseFileInZip( zf );

			RELEASEARRAYOBJECTS( Buf );

#endif // CREATE_ZIPPED_DOCX

			RELEASEARRAYOBJECTS( UTF8Text );
		}
	}

#ifdef CREATE_ZIPPED_DOCX

	void OpenXmlPackage::SaveToFile( const std::wstring& outputDir, const std::wstring& fileName, zipFile zf, const void* buf, unsigned int size )

#else

	void OpenXmlPackage::SaveToFile( const std::wstring& outputDir, const std::wstring& fileName, const void* buf, unsigned int size )

#endif // CREATE_ZIPPED_DOCX

	{
		if ( buf != NULL )
		{

#ifdef CREATE_UNZIPPED_XMLS

			int         fileHandle = 0;
			unsigned    bytesWritten = 0;

			if ( _wsopen_s( &fileHandle, ( outputDir + fileName ).c_str(), ( _O_WRONLY | _O_CREAT | _O_BINARY | _O_TRUNC ), _SH_DENYWR, _S_IWRITE ) == 0 )
			{
				bytesWritten = _write( fileHandle, buf, size );

				_close( fileHandle );
			}

#endif //CREATE_UNZIPPED_XMLS

#ifdef CREATE_ZIPPED_DOCX

			wstring zipFileName( fileName );

			replace( zipFileName.begin(), zipFileName.end(), _T( '\\' ), _T( '/' ) );

			if ( zipFileName[0] == _T( '/' ) )
			{
				copy( ( zipFileName.begin() + 1 ), zipFileName.end(), zipFileName.begin() );
				*( zipFileName.end() - 1 ) = _T( '\0' );
			}

			zip_fileinfo zi;

			zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
				zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
			zi.dosDate = 0;
			zi.internal_fa = 0;
			zi.external_fa = 0;

			SYSTEMTIME currTime;

			GetLocalTime( &currTime );

			zi.tmz_date.tm_sec = currTime.wSecond;
			zi.tmz_date.tm_min = currTime.wMinute;
			zi.tmz_date.tm_hour = currTime.wHour;
			zi.tmz_date.tm_mday = currTime.wDay;
			zi.tmz_date.tm_mon = currTime.wMonth;
			zi.tmz_date.tm_year = currTime.wYear;

			unsigned int zipFileNameSize = zipFileName.length() + 1;
			int err = ZIP_ERRNO;

			char *Buf = new char[zipFileNameSize];
			size_t convertedCout;
			wcstombs_s( &convertedCout, Buf, zipFileNameSize, zipFileName.c_str(), zipFileNameSize );

			err = zipOpenNewFileInZip( zf, Buf, &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION );

			err = zipWriteInFileInZip( zf, buf, size );

			err = zipCloseFileInZip( zf );

			RELEASEARRAYOBJECTS( Buf );

#endif // CREATE_ZIPPED_DOCX

		}
	}

	HRESULT OpenXmlPackage::SaveOLEObject( const std::wstring& fileName, const OleObjectFileStructure& oleObjectFileStructure )
	{
		HRESULT res = S_FALSE;
		CComPtr<IStorage> ppstg;

		res = StgCreateStorageEx( fileName.c_str(), ( STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE ), STGFMT_DOCFILE, 0, NULL, NULL, IID_IStorage, (void**)&(ppstg) );

		if ( ( res == S_OK ) && ( this->docFile != NULL ) )
		{
			CComPtr<IStorage> objectPoolStorage;
			CComPtr<IStorage> oleStorage;

			res = docFile->GetStorage()->GetStorage()->OpenStorage( _T( "ObjectPool" ), NULL, STGM_SHARE_EXCLUSIVE, NULL, 0, &objectPoolStorage );

			if ( res == S_OK )
			{
				res = objectPoolStorage->OpenStorage( oleObjectFileStructure.objectID.c_str(), NULL, STGM_SHARE_EXCLUSIVE, NULL, 0, &oleStorage );  
			}

			if ( res == S_OK )
			{
				res = oleStorage->CopyTo( 0, NULL, NULL, ppstg );

				//!!!TODO: For Excel.Chart!!!
				if ( oleObjectFileStructure.clsid == GUID_NULL )
				{
					res = ppstg->SetClass( oleObjectFileStructure.clsid );
				}
			}

			res = ppstg->Commit( STGC_OVERWRITE );
		}

		return res;
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

			for ( list<Relationship>::const_iterator iter = relationshipsFile.Relationships.begin(); iter != relationshipsFile.Relationships.end(); iter++ )
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

#ifdef CREATE_ZIPPED_DOCX

			SaveToFile(m_strOutputPath, relationshipsFile.FileName, zf, std::wstring( writer.GetXmlString() ) );

#else

			SaveToFile(m_strOutputPath, relationshipsFile.FileName, std::wstring( writer.GetXmlString() ) );

#endif // CREATE_ZIPPED_DOCX
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

		for ( map<wstring, std::wstring>::iterator iter = this->DocumentContentTypesFile._defaultTypes.begin(); iter != this->DocumentContentTypesFile._defaultTypes.end(); iter++ )
		{
			writer.WriteNodeBegin( _T( "Default" ), TRUE );
			writer.WriteAttribute( _T( "Extension" ), iter->first.c_str() );
			writer.WriteAttribute( _T( "ContentType" ), this->DocumentContentTypesFile._defaultTypes[iter->first].c_str() );
			writer.WriteNodeEnd( _T( "" ), TRUE );
		}

		for ( map<wstring, std::wstring>::iterator iter = this->DocumentContentTypesFile._partOverrides.begin(); iter != this->DocumentContentTypesFile._partOverrides.end(); iter++ )
		{
			writer.WriteNodeBegin( _T( "Override" ), TRUE );
			writer.WriteAttribute( _T( "PartName" ), iter->first.c_str() );
			writer.WriteAttribute( _T( "ContentType" ), this->DocumentContentTypesFile._partOverrides[iter->first].c_str() );
			writer.WriteNodeEnd( _T( "" ), TRUE );
		}

		writer.WriteNodeEnd( _T( "Types" ) );

#ifdef CREATE_ZIPPED_DOCX

		SaveToFile(m_strOutputPath, std::wstring( _T( "[Content_Types].xml" ) ), zf, std::wstring( writer.GetXmlString() ) );

#else

		SaveToFile(m_strOutputPath, std::wstring( _T( "[Content_Types].xml" ) ), std::wstring( writer.GetXmlString() ) );

#endif // CREATE_ZIPPED_DOCX
	}

	int OpenXmlPackage::RegisterImage(const IMapping* mapping, Global::BlipType blipType)
	{
		std::wstring fileName = ( std::wstring( _T( "media/image" ) ) + FormatUtils::IntToWideString( ++_imageCounter ) + VMLPictureMapping::GetTargetExt( blipType ) );

		DocumentContentTypesFile._defaultTypes.insert( make_pair( VMLPictureMapping::GetTargetExt( blipType ).erase( 0, 1 ), VMLPictureMapping::GetContentType( blipType ) ) );
		DocumentContentTypesFile._defaultTypes.insert( make_pair( std::wstring( _T( "vml" ) ), std::wstring( OpenXmlContentTypes::Vml ) ) );

		return AddPart( mapping, _T( "word" ), fileName, VMLPictureMapping::GetContentType( blipType ), OpenXmlRelationshipTypes::Image );
	}

	int OpenXmlPackage::RegisterOLEObject(const IMapping* mapping, const std::wstring& objectType)
	{
		std::wstring fileName = ( std::wstring( _T( "embeddings/oleObject" ) ) + FormatUtils::IntToWideString( ++_oleCounter ) + OleObjectMapping::GetTargetExt(objectType));

		DocumentContentTypesFile._defaultTypes.insert( make_pair( OleObjectMapping::GetTargetExt( objectType ).erase( 0, 1 ), OleObjectMapping::GetContentType(objectType)));

		return AddPart( mapping, _T( "word" ), fileName, OleObjectMapping::GetContentType( objectType ), OpenXmlRelationshipTypes::OleObject );
	}

	int OpenXmlPackage::RegisterExternalOLEObject(const IMapping* mapping, const std::wstring& objectType, const std::wstring& uri)
	{
		std::wstring fullUri	=	std::wstring(_T("file:///")) + uri;
		std::wstring fileName	=	ReplaceString(fullUri, _T(" "), _T("%20"));

		DocumentContentTypesFile._defaultTypes.insert(make_pair(OleObjectMapping::GetTargetExt(objectType).erase(0, 1), OleObjectMapping::GetContentType(objectType)));

		return AddPart(mapping, _T(""), fileName, OleObjectMapping::GetContentType(objectType), OpenXmlRelationshipTypes::OleObject, _T("External"));
	}

	int OpenXmlPackage::AddPart( const std::wstring& packageDir, const std::wstring& fileName, const std::wstring& contentType, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		if ( ( contentType != _T( "" ) ) && ( contentType != OpenXmlContentTypes::OleObject ) && 
			( contentType != OpenXmlContentTypes::MSExcel ) && ( contentType != OpenXmlContentTypes::MSWord ) &&
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