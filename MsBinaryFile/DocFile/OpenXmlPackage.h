/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
		std::vector<Relationship> Relationships;
		int						RelID;

		RelationshipsFile():
		RelID(0)
		{
		}

		RelationshipsFile( const std::wstring& fileName ):
		RelID(0), FileName( fileName )
		{
		}

		RelationshipsFile( int relID, const std::wstring& fileName, const std::vector<Relationship>& relationships ):
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
		ImageFileStructure(const std::wstring& _ext, boost::shared_array<unsigned char> _data, unsigned int _size, Global::BlipType _blipType = Global::msoblipUNKNOWN) : ext(_ext), data(_data), size(_size), blipType(_blipType)
		{

		}
		ImageFileStructure(const std::wstring& _ext, unsigned char* _data, unsigned int _size, Global::BlipType _blipType = Global::msoblipUNKNOWN) : ext(_ext), size(_size), blipType(_blipType)
		{
			data = boost::shared_array<unsigned char>(new unsigned char[size]);
			memcpy(data.get(), _data, size);
		}
		std::wstring						ext;
		boost::shared_array<unsigned char>	data;
		unsigned int						size;
		Global::BlipType					blipType;	
	};

	struct OleObjectFileStructure
	{
		bool bNativeOnly = false;
		
		std::wstring ext;
		std::wstring objectID;
        std::wstring clsid;
		
		std::pair<boost::shared_array<char>, size_t> data;

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
		
		std::vector<RelationshipsFile> HeaderRelationshipsFiles;
		std::vector<RelationshipsFile> FooterRelationshipsFiles;

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

		bool SaveOLEObject( const std::wstring& fileName, const OleObjectFileStructure& oleObjectFileStructure );
		bool SaveEmbeddedObject( const std::wstring& fileName, const OleObjectFileStructure& oleObjectFileStructure );

		void RegisterDocPr();
		void RegisterDocument();
		void RegisterDocumentMacros();
		int RegisterFontTable();
		int RegisterNumbering();
		int RegisterSettings();
		int RegisterStyleSheet();
		int RegisterHeader();
		int RegisterFooter();
		int RegisterFootnotes();
		int RegisterEndnotes();
		int RegisterComments();
		int RegisterCommentsExtended();
		int RegisterImage			( const IMapping* mapping, Global::BlipType blipType );
		int RegisterHyperlink		( const IMapping* mapping, const std::wstring& link);
		int RegisterOLEObject		( const IMapping* mapping, const std::wstring& objectType );
		int RegisterPackage			( const IMapping* mapping, const std::wstring& objectType);
		int RegisterExternalOLEObject( const IMapping* mapping, const std::wstring& objectType, const std::wstring& uri );
		int RegisterVbaProject();
	};
}
