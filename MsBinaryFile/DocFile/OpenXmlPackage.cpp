/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "NumberingMapping.h"
#include "CommentsMapping.h"
#include "EndnotesMapping.h"
#include "FootnotesMapping.h"
#include "FooterMapping.h"
#include "HeaderMapping.h"
#include "MainDocumentMapping.h"
#include "OleObjectMapping.h"
#include "VMLPictureMapping.h"
#include "FormFieldDataMapping.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

#include "../Common/Base/XmlTools.h"


namespace DocFileFormat
{
    OpenXmlPackage::OpenXmlPackage(const WordDocument* _docFile) :  m_strOutputPath(L""), relID(1), _imageCounter(0), _headerCounter(0), _footerCounter(0),
        _oleCounter(0), docFile(NULL), DocumentRelationshipsFile( L"word/_rels/document.xml.rels"), MainRelationshipsFile( L"_rels/.rels"),
        FootnotesRelationshipsFile( L"word/_rels/footnotes.xml.rels"), EndnotesRelationshipsFile( L"word/_rels/endnotes.xml.rels"),
        CommentsRelationshipsFile( L"word/_rels/comments.xml.rels"), NumberingRelationshipsFile( L"word/_rels/numbering.xml.rels")
	{
		docFile = _docFile;

        DocumentContentTypesFile._defaultTypes.insert( make_pair( L"rels", std::wstring( OpenXmlContentTypes::Relationships ) ) );
        DocumentContentTypesFile._defaultTypes.insert( make_pair( L"xml", std::wstring( OpenXmlContentTypes::Xml ) ) );

        MainRelationshipsFile.Relationships.push_back( Relationship( std::wstring( L"rId1"), OpenXmlRelationshipTypes::OfficeDocument, L"word/document.xml") );
        MainRelationshipsFile.Relationships.push_back( Relationship( std::wstring( L"rId2"), OpenXmlRelationshipTypes::CoreProperties, L"docProps/core.xml") );
        MainRelationshipsFile.Relationships.push_back( Relationship( std::wstring( L"rId3"), OpenXmlRelationshipTypes::ExtendedProperties, L"docProps/app.xml") );
	}


	void OpenXmlPackage::WritePackage()
	{
        NSDirectory::CreateDirectory(m_strOutputPath + FILE_SEPARATOR_STR +  L"_rels" );
		WriteRelsFile( MainRelationshipsFile );

        NSDirectory::CreateDirectory( m_strOutputPath + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"_rels" );

		WriteRelsFile( DocumentRelationshipsFile );

		WriteRelsFile( FootnotesRelationshipsFile );

		WriteRelsFile( EndnotesRelationshipsFile );

		WriteRelsFile( CommentsRelationshipsFile );

		WriteRelsFile( NumberingRelationshipsFile );

		for ( std::list<RelationshipsFile>::const_iterator iter = HeaderRelationshipsFiles.begin(); iter != HeaderRelationshipsFiles.end(); iter++ )
		{
			WriteRelsFile( *iter );
		}

		for ( std::list<RelationshipsFile>::const_iterator iter = FooterRelationshipsFiles.begin(); iter != FooterRelationshipsFiles.end(); iter++ )
		{
			WriteRelsFile( *iter );
		}

		WriteContentTypesFile( DocumentContentTypesFile );
	}

	void OpenXmlPackage::SaveToFile( const std::wstring& outputDir, const std::wstring& fileName, const std::wstring& XMLContent )
	{
		if ( !XMLContent.empty() )
		{
			NSFile::CFileBinary file;

			std::wstring pathFileName = outputDir + FILE_SEPARATOR_STR + fileName;
			if (file.CreateFileW(pathFileName))
			{
				std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
				file.WriteFile((BYTE*)root.c_str(), root.length());
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
	bool OpenXmlPackage::SaveEmbeddedObject( const std::wstring& fileName, const OleObjectFileStructure& object )
	{
		if (object.bNativeOnly)
		{
			POLE::Storage * storageOut = new POLE::Storage(fileName.c_str());			
			if ( (storageOut) && (storageOut->open(true, true)))
			{
				_UINT32 zero = 0, str_size = 0;
			//Ole
				BYTE dataOleInfo[] = {0x01,0x00,0x00,0x02,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
				POLE::Stream oStream3(storageOut, L"\001Ole", true, 20);
				oStream3.write(dataOleInfo, 20);
				oStream3.flush();
			//CompObj
				BYTE dataCompObjHeader[28] = {0x01,0x00,0xfe,0xff,0x03,0x0a,0x00,0x00,0xff,0xff,0xff,0xff
					,0x30,0x08,0x02
					//,0x0a,0x00,0x03
					,0x00,0x00,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x46};
				POLE::Stream oStream1(storageOut, L"\001CompObj", true, 28 +	(object.clsid.length() + 5) + 
																				(object.clsid.length() + 5) + 
																				(object.clsid.length() + 5) + 4 * 4);
				oStream1.write(dataCompObjHeader, 28);

				str_size = object.clsid.length() + 1;
				oStream1.write((BYTE*)&str_size, 4);
				oStream1.write((BYTE*)object.clsid.c_str(), object.clsid.length());
				oStream1.write((BYTE*)&zero, 1);

				str_size = object.clsid.length() + 1;
				oStream1.write((BYTE*)&str_size, 4);
				oStream1.write((BYTE*)object.clsid.c_str(), object.clsid.length());
				oStream1.write((BYTE*)&zero, 1);
				
				str_size = object.clsid.length() + 1;
				oStream1.write((BYTE*)&str_size, 4);
				oStream1.write((BYTE*)object.clsid.c_str(), object.clsid.length());
				oStream1.write((BYTE*)&zero, 1);

				_UINT32 nUnicodeMarker = 0x71B239F4;
				oStream1.write((BYTE*)&nUnicodeMarker, 4); 

				oStream1.write((BYTE*)&zero, 4); // UnicodeUserType
				oStream1.write((BYTE*)&zero, 4); // UnicodeClipboardFormat
				oStream1.write((BYTE*)&zero, 4); // 
				oStream1.flush();

			//ObjInfo
				BYTE dataObjInfo[] = {0x00,0x00,0x03,0x00,0x01,0x00};//{0x00,0x00,0x03,0x00,0x0D,0x00};
				POLE::Stream oStream2(storageOut, L"\003ObjInfo", true, 6);
				oStream2.write(dataObjInfo, 6);
				oStream2.flush();

			//Ole10Native
				size_t nativeDataSize = object.data.second;

				POLE::Stream streamData(storageOut, L"\001Ole10Native", true, nativeDataSize + 4);
				streamData.write((BYTE*)&nativeDataSize, 4);
				streamData.write((BYTE*)object.data.first.get(), nativeDataSize);
				streamData.flush();

				storageOut->close();
				delete storageOut;
			}
		}
		else
		{
			NSFile::CFileBinary file;
			file.CreateFileW(fileName);
			file.WriteFile((BYTE*)object.data.first.get(), (_UINT32)object.data.second);
			file.CloseFile();
		}
		return true;
	}

	bool OpenXmlPackage::SaveOLEObject( const std::wstring& fileName, const OleObjectFileStructure& object )
	{
		if (docFile == NULL) return false;

		POLE::Storage *storageOut = new POLE::Storage(fileName.c_str());
		if (storageOut == NULL) return false;
		
		if (storageOut->open(true, true)==false)
		{	
			delete storageOut;
			return false;
		}

		POLE::Storage *storageInp = docFile->GetStorage()->GetStorage();

		{
			POLE::Stream* oleStorage = new POLE::Stream(storageInp, object.objectID);

			if (oleStorage)
			{
				std::wstring path = L"ObjectPool/" + object.objectID;

				std::list<std::wstring> entries = storageInp->entries_with_prefix(path);
				for (std::list<std::wstring>::iterator it = entries.begin(); it != entries.end(); ++it)
				{
					std::wstring stream_name_open = (*it);
					std::wstring stream_name_create = (*it);

					if (stream_name_open[0] < 32)
						stream_name_open = stream_name_open.substr(1);

					POLE::Stream *stream_inp = new POLE::Stream(storageInp, path + L"/"+ stream_name_open);
					if (stream_inp == NULL)continue;

					POLE::uint64 size = stream_inp->size();
					
					POLE::Stream *stream_out = new POLE::Stream(storageOut, stream_name_create, true, size);

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
		return true;
	}
	void OpenXmlPackage::RegisterDocPr()
	{
        AddPart( L"docProps", L"app.xml", DocPrContentTypes::App, L"");
        AddPart( L"docProps", L"core.xml", DocPrContentTypes::Core, L"");
	}
	void OpenXmlPackage::RegisterDocument()
	{
        AddPart( L"word", L"document.xml", WordprocessingMLContentTypes::MainDocument, L"");
	}
	void OpenXmlPackage::RegisterDocumentMacros()
	{
        AddPart( L"word", L"document.xml", WordprocessingMLContentTypes::MainDocumentMacro, L"");
	}
	int OpenXmlPackage::RegisterVbaProject()
	{
		return AddPart( L"word", L"vbaProject.bin", MicrosoftWordContentTypes::VbaProject, OpenXmlRelationshipTypes::VbaProject );
	}
	int OpenXmlPackage::RegisterFontTable()
	{
        return AddPart( L"word", L"fontTable.xml", WordprocessingMLContentTypes::FontTable, OpenXmlRelationshipTypes::FontTable );
	}

	int OpenXmlPackage::RegisterNumbering()
	{
        return AddPart( L"word", L"numbering.xml", WordprocessingMLContentTypes::Numbering, OpenXmlRelationshipTypes::Numbering );
	}

	int OpenXmlPackage::RegisterSettings()
	{
        return AddPart( L"word", L"settings.xml", WordprocessingMLContentTypes::Settings, OpenXmlRelationshipTypes::Settings );
	}

	int OpenXmlPackage::RegisterStyleSheet()
	{
        return AddPart( L"word", L"styles.xml", WordprocessingMLContentTypes::Styles, OpenXmlRelationshipTypes::Styles );
	}	 

	int OpenXmlPackage::RegisterHeader()
	{
        HeaderRelationshipsFiles.push_back( RelationshipsFile( ( std::wstring( L"word/_rels/header") + FormatUtils::IntToWideString( ++_headerCounter ) + std::wstring( L".xml.rels" ) ) ) );

        return AddPart( L"word", ( std::wstring( L"header") + FormatUtils::IntToWideString( _headerCounter ) + std::wstring( L".xml" ) ), WordprocessingMLContentTypes::Header, OpenXmlRelationshipTypes::Header );
	}

	int OpenXmlPackage::AddHeaderPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		int relID = 0;

		if ( !HeaderRelationshipsFiles.empty() )
		{
            HeaderRelationshipsFiles.back().Relationships.push_back( Relationship( ( std::wstring( L"rId") + FormatUtils::IntToWideString( ++HeaderRelationshipsFiles.back().RelID ) ), relationshipType, fileName, targetMode ) );

			relID = HeaderRelationshipsFiles.back().RelID;
		}

		return relID;
	}

	int OpenXmlPackage::RegisterFooter()
	{
        FooterRelationshipsFiles.push_back( RelationshipsFile( ( std::wstring( L"word/_rels/footer" ) + FormatUtils::IntToWideString( ++_footerCounter ) + std::wstring( L".xml.rels" ) ) ) );

        return AddPart( L"word", ( std::wstring( L"footer" ) + FormatUtils::IntToWideString( _footerCounter ) + std::wstring( L".xml") ), WordprocessingMLContentTypes::Footer, OpenXmlRelationshipTypes::Footer );
	}

	int OpenXmlPackage::AddFooterPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		int relID = 0;

		if ( !FooterRelationshipsFiles.empty() )
		{
            FooterRelationshipsFiles.back().Relationships.push_back( Relationship( ( std::wstring( L"rId" ) + FormatUtils::IntToWideString( ++FooterRelationshipsFiles.back().RelID ) ), relationshipType, fileName, targetMode ) );

			relID = FooterRelationshipsFiles.back().RelID;
		}

		return relID;  
	}

	int OpenXmlPackage::RegisterFootnotes()
	{
        return AddPart( L"word", L"footnotes.xml", WordprocessingMLContentTypes::Footnotes, OpenXmlRelationshipTypes::Footnotes );
	}

	int OpenXmlPackage::AddFootnotesPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
        FootnotesRelationshipsFile.Relationships.push_back( Relationship( ( std::wstring( L"rId" ) + FormatUtils::IntToWideString( ++FootnotesRelationshipsFile.RelID ) ), relationshipType, fileName, targetMode ) );

		return FootnotesRelationshipsFile.RelID;
	}
	int OpenXmlPackage::RegisterEndnotes()
	{
        return AddPart( L"word", L"endnotes.xml", WordprocessingMLContentTypes::Endnotes, OpenXmlRelationshipTypes::Endnotes );
	}
	int OpenXmlPackage::AddEndnotesPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
        EndnotesRelationshipsFile.Relationships.push_back( Relationship( ( std::wstring( L"rId" ) + FormatUtils::IntToWideString( ++EndnotesRelationshipsFile.RelID ) ), relationshipType, fileName, targetMode ) );

		return EndnotesRelationshipsFile.RelID;
	}
	int OpenXmlPackage::RegisterComments()
	{
        return AddPart( L"word", L"comments.xml", WordprocessingMLContentTypes::Comments, OpenXmlRelationshipTypes::Comments );
	}
	int OpenXmlPackage::RegisterCommentsExtended()
	{
        return AddPart( L"word", L"commentsExtended.xml", WordprocessingMLContentTypes::CommentsExtended, OpenXmlRelationshipTypes::CommentsExtended );
	}
	int OpenXmlPackage::AddCommentsPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
        CommentsRelationshipsFile.Relationships.push_back( Relationship( ( std::wstring( L"rId" ) + FormatUtils::IntToWideString( ++CommentsRelationshipsFile.RelID ) ), relationshipType, fileName, targetMode ) );

		return CommentsRelationshipsFile.RelID;
	}

	int OpenXmlPackage::AddNumberingPart( const std::wstring& fileName, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
        NumberingRelationshipsFile.Relationships.push_back( Relationship( ( std::wstring( L"rId" ) + FormatUtils::IntToWideString( ++NumberingRelationshipsFile.RelID ) ), relationshipType, fileName, targetMode ) );

		return NumberingRelationshipsFile.RelID;
	}

	void OpenXmlPackage::WriteRelsFile( const RelationshipsFile& relationshipsFile )
	{
		if ( !relationshipsFile.Relationships.empty() )
		{
			XMLTools::CStringXmlWriter writer;

            writer.WriteNodeBegin( L"Relationships", TRUE );

			//write namespaces
            writer.WriteAttribute( L"xmlns", OpenXmlNamespaces::RelationshipsPackage );

            writer.WriteNodeEnd( L"", TRUE, FALSE );

			for ( std::list<Relationship>::const_iterator iter = relationshipsFile.Relationships.begin(); iter != relationshipsFile.Relationships.end(); iter++ )
			{
                writer.WriteNodeBegin( L"Relationship", TRUE );
                writer.WriteAttribute( L"Id", iter->Id );
                writer.WriteAttribute( L"Type", iter->Type );
                writer.WriteAttribute( L"Target", iter->Target );

				if ( !iter->TargetMode.empty() )
				{
                    writer.WriteAttribute( L"TargetMode", iter->TargetMode );
				}

                writer.WriteNodeEnd( L"", TRUE );
			}

            writer.WriteNodeEnd( L"Relationships" );

			std::wstring pathFileName = m_strOutputPath + FILE_SEPARATOR_STR + relationshipsFile.FileName;
			
			writer.SaveToFile(pathFileName, TRUE);
		}
	}

	void OpenXmlPackage::WriteContentTypesFile( const ContentTypesFile& contentTypesFile )
	{
		XMLTools::CStringXmlWriter writer;

		// write content types
        writer.WriteNodeBegin( L"Types", TRUE );

		//write namespaces
        writer.WriteAttribute( L"xmlns", OpenXmlNamespaces::ContentTypes );

        writer.WriteNodeEnd( L"", TRUE, FALSE );

		for ( std::map<std::wstring, std::wstring>::iterator iter = DocumentContentTypesFile._defaultTypes.begin(); iter != DocumentContentTypesFile._defaultTypes.end(); iter++ )
		{
            writer.WriteNodeBegin( L"Default", TRUE );
            writer.WriteAttribute( L"Extension", iter->first );
            writer.WriteAttribute( L"ContentType", DocumentContentTypesFile._defaultTypes[iter->first] );
            writer.WriteNodeEnd( L"", TRUE );
		}

		for ( std::map<std::wstring, std::wstring>::iterator iter = DocumentContentTypesFile._partOverrides.begin(); iter != DocumentContentTypesFile._partOverrides.end(); iter++ )
		{
            writer.WriteNodeBegin( L"Override", TRUE );
            writer.WriteAttribute( L"PartName", iter->first );
            writer.WriteAttribute( L"ContentType", DocumentContentTypesFile._partOverrides[iter->first] );
            writer.WriteNodeEnd( L"", TRUE );
		}

        writer.WriteNodeEnd( L"Types" );

        std::wstring pathFileName = m_strOutputPath + FILE_SEPARATOR_STR + L"[Content_Types].xml" ;
		
		writer.SaveToFile(pathFileName, TRUE);
	}

	int OpenXmlPackage::RegisterImage(const IMapping* mapping, Global::BlipType blipType)
	{
        std::wstring fileName = ( std::wstring( L"media/image" ) + FormatUtils::IntToWideString( ++_imageCounter ) + VMLPictureMapping::GetTargetExt( blipType ) );

		DocumentContentTypesFile._defaultTypes.insert( std::make_pair( VMLPictureMapping::GetTargetExt( blipType ).erase( 0, 1 ), VMLPictureMapping::GetContentType( blipType ) ) );
        DocumentContentTypesFile._defaultTypes.insert( std::make_pair( L"vml", std::wstring( OpenXmlContentTypes::Vml ) ));

        return AddPart( mapping, L"word", fileName, VMLPictureMapping::GetContentType( blipType ), OpenXmlRelationshipTypes::Image );
	}
	int OpenXmlPackage::RegisterHyperlink(const IMapping* mapping, const std::wstring& link)
	{
        return AddPart(mapping, L"", link, L"", OpenXmlRelationshipTypes::Hyperlink, L"External");
	}
	int OpenXmlPackage::RegisterOLEObject(const IMapping* mapping, const std::wstring& objectType)
	{
        std::wstring fileName = ( std::wstring( L"embeddings/oleObject" ) + FormatUtils::IntToWideString( ++_oleCounter ) + OleObjectMapping::GetTargetExt(objectType));

		DocumentContentTypesFile._defaultTypes.insert( std::make_pair( OleObjectMapping::GetTargetExt( objectType ).erase( 0, 1 ), OleObjectMapping::GetContentType(objectType)));

        return AddPart( mapping, L"word", fileName, OleObjectMapping::GetContentType( objectType ), OpenXmlRelationshipTypes::OleObject );
	}
	int OpenXmlPackage::RegisterPackage(const IMapping* mapping, const std::wstring& objectType)
	{
        std::wstring fileName = ( std::wstring( L"embeddings/oleObject") + FormatUtils::IntToWideString( ++_oleCounter ) + OleObjectMapping::GetTargetExt(objectType));

		DocumentContentTypesFile._defaultTypes.insert( std::make_pair( OleObjectMapping::GetTargetExt( objectType ).erase( 0, 1 ), OleObjectMapping::GetContentType(objectType)));

        return AddPart( mapping, L"word", fileName, OleObjectMapping::GetContentType( objectType ), OpenXmlRelationshipTypes::Package);
	}
	int OpenXmlPackage::RegisterExternalOLEObject(const IMapping* mapping, const std::wstring& objectType, const std::wstring& uri)
	{
        std::wstring fullUri	=	std::wstring(L"file:///") + uri;
        std::wstring fileName	=	ReplaceString(fullUri, L" ", L"%20");

		DocumentContentTypesFile._defaultTypes.insert(std::make_pair(OleObjectMapping::GetTargetExt(objectType).erase(0, 1), OleObjectMapping::GetContentType(objectType)));

        return AddPart(mapping, L"", fileName, OleObjectMapping::GetContentType(objectType), OpenXmlRelationshipTypes::OleObject, L"External");
	}

	int OpenXmlPackage::AddPart( const std::wstring& packageDir, const std::wstring& fileName, const std::wstring& contentType, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
        if (( contentType != L"" ) &&
			( contentType != OpenXmlContentTypes::Xml )				&&
			( contentType != OpenXmlContentTypes::MSWordDocx )		&& 
			( contentType != OpenXmlContentTypes::OleObject )		&& 
			( contentType != OpenXmlContentTypes::MSExcel )			&& 
			( contentType != OpenXmlContentTypes::MSWord )			&&
			( contentType != OpenXmlContentTypes::MSPowerpoint ) )
		{
			std::wstring  partOverride;

            if (packageDir != std::wstring ( L"") )
			{
                partOverride = std::wstring ( L"/" ) + packageDir + std::wstring( L"/");
			}

			partOverride += fileName; 

			DocumentContentTypesFile._partOverrides.insert( make_pair( partOverride, contentType ) );
		}

        if ( relationshipType != L"" )
		{
            DocumentRelationshipsFile.Relationships.push_back( Relationship( L"rId" + FormatUtils::IntToWideString( ++relID ), relationshipType, fileName, targetMode ) );
		}

		return relID;
	}

	int OpenXmlPackage::AddPart( const IMapping* mapping, const std::wstring& packageDir, const std::wstring& fileName, const std::wstring& contentType, const std::wstring& relationshipType, const std::wstring& targetMode )
	{
		int relID = 0;

		if ( mapping != NULL )
		{
			if ( typeid(*mapping) == typeid(MainDocumentMapping) )
			{
				relID = AddPart( packageDir, fileName, contentType, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(HeaderMapping) )
			{
				relID = AddHeaderPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(FooterMapping) )
			{
				relID = AddFooterPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(FootnotesMapping) )
			{
				relID = AddFootnotesPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(EndnotesMapping) )
			{
				relID = AddEndnotesPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(CommentsMapping) )
			{
				relID = AddCommentsPart( fileName, relationshipType, targetMode );
			}
			else if ( typeid(*mapping) == typeid(NumberingMapping) )
			{
				relID = AddNumberingPart( fileName, relationshipType, targetMode );
			}
		}

		return relID;
	}
}
