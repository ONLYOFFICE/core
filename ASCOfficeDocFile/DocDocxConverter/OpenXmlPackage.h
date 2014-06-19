#pragma once

#include "ContentTypes.h"
#include "Global.h"
#include "WordDocument.h"
#include "IMapping.h"

#include <atlstr.h>

#include "..\..\Common\DocxFormat\Source\XML\XmlUtils.h"

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace DocFileFormat
{
	struct Relationship
	{
		wstring Id;
		wstring Type;
		wstring Target;
		wstring TargetMode;

		Relationship()
		{
		}

		Relationship( const wstring& id,  const wstring& type, const wstring& target, const wstring& targetMode = _T( "" ) ):
		Id( id ), Type( type ), Target( target ), TargetMode( targetMode )
		{
		}
	};

	struct RelationshipsFile
	{
		wstring FileName;
		list<Relationship> Relationships;
		int RelID;

		RelationshipsFile():
		RelID(0)
		{
		}

		RelationshipsFile( const wstring& fileName ):
		RelID(0), FileName( fileName )
		{
		}

		RelationshipsFile( int relID, const wstring& fileName, const list<Relationship>& relationships ):
		RelID(relID), FileName( fileName ), Relationships( relationships )
		{
		}
	};

	struct ContentTypesFile
	{
		map<wstring, wstring> _defaultTypes;
		map<wstring, wstring> _partOverrides;
	};

	struct ImageFileStructure
	{
		wstring ext;
		vector<byte> data;

		ImageFileStructure()
		{
		}

		ImageFileStructure( const wstring& _ext, const vector<byte>& _data ):
		ext(_ext), data(_data)
		{
		}
	};

	struct OleObjectFileStructure
	{
		wstring ext;
		wstring objectID;
		CLSID clsid;

		OleObjectFileStructure()
		{
			this->clsid = GUID_NULL;
		}

		OleObjectFileStructure( const wstring& _ext, const wstring& _objectID, REFCLSID _clsid ):
		ext(_ext), objectID(_objectID), clsid(_clsid)
		{
		} 
	};

	class OpenXmlPackage
	{
	private: 
		ContentTypesFile DocumentContentTypesFile;
		RelationshipsFile MainRelationshipsFile;
		RelationshipsFile DocumentRelationshipsFile;
		RelationshipsFile FootnotesRelationshipsFile;
		RelationshipsFile EndnotesRelationshipsFile;
		RelationshipsFile CommentsRelationshipsFile;
		RelationshipsFile NumberingRelationshipsFile;
		list<RelationshipsFile> HeaderRelationshipsFiles;
		list<RelationshipsFile> FooterRelationshipsFiles;

		int relID;

		int _imageCounter;
		int _headerCounter;
		int _footerCounter;
		int _oleCounter;

		const WordDocument* docFile;

		int AddHeaderPart( const wstring& fileName, const wstring& relationshipType = _T( "" ), const wstring& targetMode = _T( "" ) );
		int AddFooterPart( const wstring& fileName, const wstring& relationshipType = _T( "" ), const wstring& targetMode = _T( "" ) );
		int AddFootnotesPart( const wstring& fileName, const wstring& relationshipType = _T( "" ), const wstring& targetMode = _T( "" ) );
		int AddEndnotesPart( const wstring& fileName, const wstring& relationshipType = _T( "" ), const wstring& targetMode = _T( "" ) );
		int AddCommentsPart( const wstring& fileName, const wstring& relationshipType = _T( "" ), const wstring& targetMode = _T( "" ) );
		int AddNumberingPart( const wstring& fileName, const wstring& relationshipType = _T( "" ), const wstring& targetMode = _T( "" ) );

		void WriteRelsFile( const RelationshipsFile& relationshipsFile );
		void WriteContentTypesFile( const ContentTypesFile& contentTypesFile );

		int AddPart( const wstring& packageDir, const wstring& fileName, const wstring& contentType = _T( "" ), const wstring& relationshipType = _T( "" ), const wstring& targetMode = _T( "" ) );
		int AddPart( const IMapping* mapping, const wstring& packageDir, const wstring& fileName, const wstring& contentType = _T( "" ), const wstring& relationshipType = _T( "" ), const wstring& targetMode = _T( "" ) );

	protected:	  

		std::wstring m_strOutputPath;
		std::wstring m_strFileName;

		OpenXmlPackage( const WordDocument* _docFile );

		void GetOutputDir();

#ifdef CREATE_ZIPPED_DOCX

		void WritePackage( zipFile zf );
		void SaveToFile( const wstring& outputDir, const wstring& fileName, zipFile zf, const wstring& XMLContent );
		void SaveToFile( const wstring& outputDir, const wstring& fileName, zipFile zf, const void* buf, unsigned int size );

#else

		void WritePackage();
		void SaveToFile( const wstring& outputDir, const wstring& fileName, const wstring& XMLContent );
		void SaveToFile( const wstring& outputDir, const wstring& fileName, const void* buf, unsigned int size );
		HRESULT SaveOLEObject( const wstring& fileName, const OleObjectFileStructure& oleObjectFileStructure );

#endif // CREATE_ZIPPED_DOCX

		int RegisterDocument();
		int RegisterFontTable();
		int RegisterNumbering();
		int RegisterSettings();
		int RegisterStyleSheet();
		int RegisterHeader();
		int RegisterFooter();
		int RegisterFootnotes();
		int RegisterEndnotes();
		int RegisterComments();
		int RegisterImage( const IMapping* mapping, Global::BlipType blipType );
		int RegisterOLEObject( const IMapping* mapping, const wstring& objectType );
		int RegisterExternalOLEObject( const IMapping* mapping, const wstring& objectType, const wstring& uri );
	};
}