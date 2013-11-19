#include "stdafx.h"
#include "WordprocessingDocument.h"

namespace DocFileFormat
{
	static bool ClearDirectory( const WCHAR* dir, bool delDir = false )
	{
		if ( dir != NULL )
		{
			size_t size = wcslen( dir );
			WCHAR* _dir = new WCHAR[size + 2];
			wcsncpy( _dir, dir, size );
			_dir[size++] = L'\0';
			_dir[size] = L'\0';

			SHFILEOPSTRUCT lpFileOp;

			lpFileOp.hwnd = NULL;
			lpFileOp.wFunc = FO_DELETE;
			lpFileOp.pFrom = _dir;
			lpFileOp.pTo = NULL;
			lpFileOp.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT;
			lpFileOp.fAnyOperationsAborted = FALSE;
			lpFileOp.hNameMappings = NULL;
			lpFileOp.lpszProgressTitle = NULL;

			int err = SHFileOperation( &lpFileOp );

			if ( !delDir )
			{
				CreateDirectory( dir, NULL );
			}

			RELEASEARRAYOBJECTS (_dir);

			return true;
		}
		else
		{
			return false;
		}
	} 


	WordprocessingDocument::WordprocessingDocument( const WCHAR* _fileName, const WordDocument* _docFile ):
	FileName( _fileName ), OpenXmlPackage( _docFile ), FontTableXML( _T( "" ) ), DocumentXML( _T( "" ) ), 
		StyleSheetXML( _T( "" ) ), FootnotesXML ( _T( "" ) ), NumberingXML( _T( "" ) ),
		CommentsXML( _T( "" ) ), SettingsXML( _T( "" ) ), CommandTableXML ( _T( "" ) )
	{
	}

	WordprocessingDocument::~WordprocessingDocument()
	{
	}

	void WordprocessingDocument::SaveDocument()
	{
		SaveDocument(FileName.c_str() );
	}

	void WordprocessingDocument::SaveDocument( const WCHAR *file )
	{
		this->_fileName = wstring( file );

		GetOutputDir();

		size_t fileNameSize = this->_fileName.length() + 1;
		char *Buf = new char[fileNameSize];
		size_t convertedCout;
		wcstombs_s( &convertedCout, Buf, fileNameSize, this->_fileName.c_str(), fileNameSize );

#ifdef CREATE_ZIPPED_DOCX

		this->zf = zipOpen( Buf, 0 );

#endif  // CREATE_ZIPPED_DOCX

		RELEASEARRAYOBJECTS( Buf );

#ifdef CREATE_UNZIPPED_XMLS

		CreateDirectory( ( this->_outputDir + wstring( _T( "\\word" ) ) ).c_str(), NULL );

#endif //CREATE_UNZIPPED_XMLS

#ifdef CREATE_ZIPPED_DOCX

		this->WritePackage( this->zf );

		//Write main content. (word directory)
		SaveToFile( this->_outputDir, wstring( _T( "word/document.xml" ) ), this->zf, this->DocumentXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/fontTable.xml" ) ), this->zf, this->FontTableXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/styles.xml" ) ), this->zf, this->StyleSheetXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/footnotes.xml" ) ), this->zf, this->FootnotesXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/endnotes.xml" ) ), this->zf, this->EndnotesXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/numbering.xml" ) ), this->zf, this->NumberingXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/comments.xml" ) ), this->zf, this->CommentsXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/settings.xml" ) ), this->zf, this->SettingsXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/customizations.xml" ) ), this->zf, this->CommandTableXML );

#else

		this->WritePackage();

		//Write main content. (word directory)
		SaveToFile( this->_outputDir, wstring( _T( "word/document.xml" ) ), this->DocumentXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/fontTable.xml" ) ), this->FontTableXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/styles.xml" ) ), this->StyleSheetXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/footnotes.xml" ) ), this->FootnotesXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/endnotes.xml" ) ), this->EndnotesXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/numbering.xml" ) ), this->NumberingXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/comments.xml" ) ), this->CommentsXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/settings.xml" ) ), this->SettingsXML );
		SaveToFile( this->_outputDir, wstring( _T( "word/customizations.xml" ) ), this->CommandTableXML );

#endif  // CREATE_ZIPPED_DOCX

		if ( !this->ImagesList.empty() )
		{

#ifdef CREATE_UNZIPPED_XMLS

			CreateDirectory( ( this->_outputDir + wstring( _T( "\\word\\media" ) ) ).c_str(), NULL );

#endif //CREATE_UNZIPPED_XMLS

			int i = 1;

			for ( list<ImageFileStructure>::iterator iter = this->ImagesList.begin(); iter != this->ImagesList.end(); iter++ )
			{
				byte *bytes = NULL;
				bytes = new byte[iter->data.size()];
				copy( iter->data.begin(), iter->data.end(), bytes );

#ifdef CREATE_ZIPPED_DOCX

				SaveToFile( this->_outputDir, ( wstring( _T( "\\word\\media\\image" ) ) + FormatUtils::IntToWideString( i++ ) + iter->ext ), this->zf, (void*)bytes, (unsigned int)iter->data.size() );

#else

				SaveToFile( this->_outputDir, ( wstring( _T( "\\word\\media\\image" ) ) + FormatUtils::IntToWideString( i++ ) + iter->ext ), (void*)bytes, (unsigned int)iter->data.size() );

#endif // CREATE_ZIPPED_DOCX

				RELEASEARRAYOBJECTS( bytes );
			}
		}

		if ( !this->OleObjectsList.empty() )
		{

#ifdef CREATE_UNZIPPED_XMLS

			CreateDirectory( ( this->_outputDir + wstring( _T( "\\word\\embeddings" ) ) ).c_str(), NULL );

#endif //CREATE_UNZIPPED_XMLS

			int i = 1;

			for ( list<OleObjectFileStructure>::iterator iter = this->OleObjectsList.begin(); iter != this->OleObjectsList.end(); iter++ )
			{
				wstring fileName = ( this->_outputDir + wstring( _T( "\\word\\embeddings\\oleObject" ) ) + FormatUtils::IntToWideString( i++ ) + iter->ext );

#ifdef CREATE_ZIPPED_DOCX

#error OLE Objects saving not implemented!!!

#else

				this->SaveOLEObject( fileName, *iter );	

#endif // CREATE_ZIPPED_DOCX

			}
		}

		int headersCount = 0;
		int footersCount = 0;

		for ( list<wstring>::iterator iter = this->HeaderXMLList.begin(); iter != this->HeaderXMLList.end(); iter++ )
		{
			SaveToFile( this->_outputDir, ( wstring( _T( "word/header" ) ) + FormatUtils::IntToWideString( ++headersCount ) + wstring( _T( ".xml" ) ) ), *iter );
		}

		for ( list<wstring>::iterator iter = this->FooterXMLList.begin(); iter != this->FooterXMLList.end(); iter++ )
		{
			SaveToFile( this->_outputDir, ( wstring( _T( "word/footer" ) ) + FormatUtils::IntToWideString( ++footersCount ) + wstring( _T( ".xml" ) ) ), *iter );
		}

#ifdef CREATE_ZIPPED_DOCX

		int err = zipClose( this->zf, NULL );

#endif // CREATE_ZIPPED_DOCX
	}
}