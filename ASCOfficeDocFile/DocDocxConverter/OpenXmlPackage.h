/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "ContentTypes.h"
#include "Global.h"
#include "WordDocument.h"
#include "IMapping.h"

namespace DocFileFormat
{
	struct Relationship
	{
		std::wstring Id;
		std::wstring Type;
		std::wstring Target;
		std::wstring TargetMode;

		Relationship()
		{
		}

        Relationship( const std::wstring& id,  const std::wstring& type, const std::wstring& target, const std::wstring& targetMode = L""):
		Id( id ), Type( type ), Target( target ), TargetMode( targetMode )
		{
		}
	};

	struct RelationshipsFile
	{
		std::wstring			FileName;
		std::list<Relationship> Relationships;
		int						RelID;

		RelationshipsFile():
		RelID(0)
		{
		}

		RelationshipsFile( const std::wstring& fileName ):
		RelID(0), FileName( fileName )
		{
		}

		RelationshipsFile( int relID, const std::wstring& fileName, const std::list<Relationship>& relationships ):
					RelID(relID), FileName( fileName ), Relationships( relationships )
		{
		}
	};

	struct ContentTypesFile
	{
		std::map<std::wstring, std::wstring> _defaultTypes;
		std::map<std::wstring, std::wstring> _partOverrides;
	};

	struct ImageFileStructure
	{
		ImageFileStructure()
		{

		}

		ImageFileStructure(const std::wstring& _ext, const std::vector<unsigned char>& _data, Global::BlipType	_blipType = Global::msoblipUNKNOWN) : ext(_ext), data(_data), blipType(_blipType)
		{

		}

		std::wstring				ext;
		std::vector<unsigned char>	data;
		Global::BlipType			blipType;	
	};

	struct OleObjectFileStructure
	{
		std::wstring ext;
		std::wstring objectID;
        std::wstring clsid;

		std::string	data;

		OleObjectFileStructure(){}


        OleObjectFileStructure( const std::wstring& _ext, const std::wstring& _objectID, const std::wstring&/*REFCLSID*/ _clsid ):
		ext(_ext), objectID(_objectID), clsid(_clsid){}

	};

	class OpenXmlPackage
	{
	public:
		const WordDocument* docFile;
	private: 
		ContentTypesFile DocumentContentTypesFile;
		RelationshipsFile MainRelationshipsFile;
		RelationshipsFile DocumentRelationshipsFile;
		RelationshipsFile FootnotesRelationshipsFile;
		RelationshipsFile EndnotesRelationshipsFile;
		RelationshipsFile CommentsRelationshipsFile;
		RelationshipsFile NumberingRelationshipsFile;
		
		std::list<RelationshipsFile> HeaderRelationshipsFiles;
		std::list<RelationshipsFile> FooterRelationshipsFiles;

		int relID;

		int _imageCounter;
		int _headerCounter;
		int _footerCounter;
		int _oleCounter;

        int AddHeaderPart( const std::wstring& fileName, const std::wstring& relationshipType = L"", const std::wstring& targetMode = L"" );
        int AddFooterPart( const std::wstring& fileName, const std::wstring& relationshipType = L"", const std::wstring& targetMode = L"" );
        int AddFootnotesPart( const std::wstring& fileName, const std::wstring& relationshipType = L"", const std::wstring& targetMode = L"" );
        int AddEndnotesPart( const std::wstring& fileName, const std::wstring& relationshipType = L"", const std::wstring& targetMode = L"" );
        int AddCommentsPart( const std::wstring& fileName, const std::wstring& relationshipType = L"", const std::wstring& targetMode = L"" );
        int AddNumberingPart( const std::wstring& fileName, const std::wstring& relationshipType = L"", const std::wstring& targetMode = L"" );

		void WriteRelsFile( const RelationshipsFile& relationshipsFile );
		void WriteContentTypesFile( const ContentTypesFile& contentTypesFile );

        int AddPart( const std::wstring& packageDir, const std::wstring& fileName, const std::wstring& contentType = L"", const std::wstring& relationshipType = L"", const std::wstring& targetMode = L"" );
        int AddPart( const IMapping* mapping, const std::wstring& packageDir, const std::wstring& fileName, const std::wstring& contentType = L"", const std::wstring& relationshipType = L"", const std::wstring& targetMode = L"" );

	protected:	  

		std::wstring m_strOutputPath;

		OpenXmlPackage( const WordDocument* _docFile );

		void WritePackage();
		void SaveToFile( const std::wstring& outputDir, const std::wstring& fileName, const std::wstring& XMLContent );
		void SaveToFile( const std::wstring& outputDir, const std::wstring& fileName, const void* buf, unsigned int size );

		HRESULT SaveOLEObject		( const std::wstring& fileName, const OleObjectFileStructure& oleObjectFileStructure );
		HRESULT SaveEmbeddedObject	( const std::wstring& fileName, const std::string& data );

		int RegisterDocument();
		int RegisterDocumentMacros();
		int RegisterFontTable();
		int RegisterNumbering();
		int RegisterSettings();
		int RegisterStyleSheet();
		int RegisterHeader();
		int RegisterFooter();
		int RegisterFootnotes();
		int RegisterEndnotes();
		int RegisterComments();
		int RegisterImage			( const IMapping* mapping, Global::BlipType blipType );
		int RegisterOLEObject		( const IMapping* mapping, const std::wstring& objectType );
		int RegisterPackage			( const IMapping* mapping, const std::wstring& objectType);
		int RegisterExternalOLEObject( const IMapping* mapping, const std::wstring& objectType, const std::wstring& uri );
		int RegisterVbaProject();
	};
}
