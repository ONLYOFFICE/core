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
#pragma once

#include "OleObject.h"
#include "WordDocument.h"

namespace DocFileFormat
{
OleObject::OleObject( const CharacterPropertyExceptions* chpx, WordDocument* document)
{
	if (!document) return;
	if (!chpx) return;
	
	nWordVersion = document->nWordVersion;		

	ObjectId = getOleEntryName( chpx );	

	StructuredStorageReader* docStorage = document->GetStorage();
	
	oleStorage = docStorage->GetStorage();
	if (!oleStorage) return;

	std::wstring sObjectId( ObjectId.begin(), ObjectId.end() );

	std::wstring name = L"ObjectPool/" + sObjectId + L"/";
	
	bool bOle		= processOleStream( name + L"Ole" );
	bool bCompObj	= isLinked ? processLinkInfoStream( name + L"LinkInfo" ):
								processCompObjStream( name + L"CompObj" );
	if (bOle || bCompObj)
	{
		processPICStream( name + L"PIC" );
		processMETAStream( name + L"META" );
		processEquationNativeStream( name + L"Equation Native" );
		processPackageStream(name + L"Package");
	}
	else if (nWordVersion > 0)
	{
		int fc = pictureDesciptor.GetFcPic( chpx );

		if ( fc >= 0 )
		{
			POLE::Stream* pOleStream = document->GetDocumentStream();
			pictureDesciptor.parse( pOleStream, fc, 0xffffff, nWordVersion);
		
			VirtualStreamReader reader(pOleStream, pOleStream->tell(), nWordVersion);

			int pos = reader.GetPosition();

			short a1 = reader.ReadInt16();
			short a2 = reader.ReadInt16();
			short a3 = reader.ReadInt16();

			int lcb = reader.ReadInt32();
			//short a4 = reader.ReadInt16();
			//short a5 = reader.ReadInt16();
			//short a6 = reader.ReadInt16();
			//short a7 = reader.ReadInt16();
			int lcb1 = reader.ReadInt32();

			ClipboardFormat = Program = reader.ReadLengthPrefixedAnsiString(0xffff);

			short a10 = reader.ReadInt16();
			short a11 = reader.ReadInt16();
			short a12 = reader.ReadInt16();
			short a14 = reader.ReadInt16();

			//int lcb = 5000;//reader.ReadInt32();

			int szHeader  = reader.GetPosition() - pos;
			size_t szData  = reader.ReadInt32();
			if (szData > lcb)
			{
				szData = szData >> 16;
			}
			char* bytes = (szData < 0xffff) ? (char*)reader.ReadBytes( szData, true ) : NULL;
			if (bytes)
			{
				boost::shared_array<char> buffer(bytes);
				embeddedData = std::make_pair(buffer, szData);
			}
		}

	}

}

bool OleObject::processLinkInfoStream( const std::wstring& linkStream )
{
	bool res = false;
	try
	{
		POLE::Stream* pLinkStream = new POLE::Stream(oleStorage, linkStream);

		if (( pLinkStream ) && (false == pLinkStream->fail()))
		{
			VirtualStreamReader reader( pLinkStream, 0, false);
			processLinkInfoStream(reader);

			delete pLinkStream;
			res = true;
		}
		if (pLinkStream) delete pLinkStream;
	}
	catch (...)
	{
	}
	return res;
}
      
void OleObject::processEquationNativeStream( const std::wstring& eqStream )
{
	try
	{
	  POLE::Stream* pCompStream = new POLE::Stream(oleStorage, eqStream);

	  if ((pCompStream) && (false == pCompStream->fail()))
	  {
		  VirtualStreamReader reader(pCompStream, 0, false);

		  int sz = reader.GetSize();

		  unsigned char *Buffer = reader.ReadBytes(sz, true);

		  if (Buffer && sz > 0)
		  {
			  isEquation = true;
			  delete[]Buffer;
		  }
	  }
	  if (pCompStream) delete pCompStream;
	}
	catch (...)
	{
	}
}
bool OleObject::processPackageStream(const std::wstring& packageStream)
{
	bool res = true;
	try
	{
		POLE::Stream* pPackageStream = new POLE::Stream(oleStorage, packageStream);

		if ((pPackageStream) && (false == pPackageStream->fail()))
		{
			VirtualStreamReader reader(pPackageStream, 0, false);

			size_t sz = reader.GetSize();
			
			char* bytes = (sz < 0xffffff) ? (char*)reader.ReadBytes(sz, true) : NULL;
			if (bytes)
			{
				boost::shared_array<char> buffer(bytes);
				embeddedData = std::make_pair(buffer, sz);
			}

			isPackage = true;
		}
		if (pPackageStream) delete pPackageStream;
	}
	catch (...)
	{
	}
	return res;
}
void OleObject::processMETAStream( const std::wstring& metaStream )
{
	try
	{
		POLE::Stream* pMETAStream = new POLE::Stream(oleStorage, metaStream);

		if (( pMETAStream ) && (false == pMETAStream->fail()))
		{
			pictureDesciptor.Type = wmf;
			VirtualStreamReader reader( pMETAStream, 0, false);

			pictureDesciptor.mfp.mm		= reader.ReadUInt16();
			pictureDesciptor.mfp.xExt	= reader.ReadUInt16();
			pictureDesciptor.mfp.yExt	= reader.ReadUInt16();
			pictureDesciptor.mfp.hMf	= reader.ReadUInt16();
			
			pictureDesciptor.embeddedDataSize	= reader.GetSize() - 8;
			pictureDesciptor.embeddedData		= reader.ReadBytes( pictureDesciptor.embeddedDataSize, true );
		}
		if (pMETAStream) delete pMETAStream;
	}
	catch (...)
	{
	}
}
void OleObject::processPICStream( const std::wstring& picStream )
{
	try
	{
		POLE::Stream* pPICStream = new POLE::Stream(oleStorage, picStream);

		if ((pPICStream) && (false == pPICStream->fail()))
		{
			VirtualStreamReader reader( pPICStream, 0, false);

			int sz = reader.GetSize();

			int cbHeader =	reader.ReadUInt32();

			unsigned char* bytes = NULL;
				
			pictureDesciptor.mfp.mm		=	reader.ReadInt16();
			pictureDesciptor.mfp.xExt	=	reader.ReadInt16();
			pictureDesciptor.mfp.yExt	=	reader.ReadInt16();
			pictureDesciptor.mfp.hMf	=	reader.ReadInt16();

			int x	=	reader.ReadUInt32();
			int y	=	reader.ReadUInt32();

			pictureDesciptor.dxaGoal	=	reader.ReadUInt32();
			pictureDesciptor.dyaGoal	=	reader.ReadUInt32();
	
			unsigned char* data = reader.ReadBytes(16, true);
			delete []data;
			
			pictureDesciptor.mx		=	reader.ReadUInt32();
			pictureDesciptor.my		=	reader.ReadUInt32();

			pictureDesciptor.dxaCropLeft		=	reader.ReadInt32();
			pictureDesciptor.dyaCropTop			=	reader.ReadInt32();
			pictureDesciptor.dxaCropRight		=	reader.ReadInt32();
			pictureDesciptor.dyaCropBottom		=	reader.ReadInt32();

		// borders
			int bytesCount = (nWordVersion > 0) ? 2 : 4;
			
			bytes =	reader.ReadBytes( bytesCount, true );
			pictureDesciptor.brcTop	= new BorderCode( bytes, bytesCount );
			RELEASEARRAYOBJECTS( bytes );

			bytes =	reader.ReadBytes( bytesCount, true );
			pictureDesciptor.brcLeft = new BorderCode( bytes, bytesCount );
			RELEASEARRAYOBJECTS( bytes );

			bytes =	reader.ReadBytes( bytesCount, true );
			pictureDesciptor.brcBottom = new BorderCode( bytes, bytesCount );
			RELEASEARRAYOBJECTS( bytes );

			bytes =	reader.ReadBytes( bytesCount, true );
			pictureDesciptor.brcRight =	new BorderCode( bytes, bytesCount );
			RELEASEARRAYOBJECTS( bytes );
			
			int etc = sz - reader.GetPosition();
			unsigned char* data2 = reader.ReadBytes(etc, true);
			delete []data2;			
		}
	}
	catch (...)
	{
	}
}

bool OleObject::processCompObjStream( const std::wstring& compStream )
{
	bool res = false;
	try
	{
		POLE::Stream* pCompStream = new POLE::Stream(oleStorage, compStream);

		if ( (pCompStream) && (false == pCompStream->fail()) )
		{
			VirtualStreamReader reader( pCompStream, 0, false);
			processCompObjStream(reader);
			
			res = true;
		}
		if (pCompStream) delete pCompStream;
	}
	catch (...)
	{
	}
	return res;
}

bool OleObject::processOleStream( const std::wstring& oleStreamName )
{
	try
	{
	  POLE::Stream* pOleStream;
	  HRESULT res = S_OK;

	  pOleStream  = new POLE::Stream(oleStorage, oleStreamName);

	  if ( (pOleStream) && (!pOleStream->fail()))
	  {
			VirtualStreamReader reader( pOleStream, 0, false );
			processOleStream(reader);

			delete pOleStream;

			return true;
		}
	}
	catch (...)
	{ 
	}

	return false;
}

void OleObject::processOleStream( VirtualStreamReader& reader )
{
	reader.ReadBytes( 4, false );	//skip version

	//read the embedded/linked flag
	int flag = reader.ReadInt32();
	isLinked = FormatUtils::BitmaskToBool( flag, 0x1 );

	//Link update option
	this->updateMode = (LinkUpdateOption)reader.ReadInt32();

	switch ( this->updateMode )
	{
		case NoLink:	UpdateMode = L"NoLink"; break;	
		case Always:	UpdateMode = L"Always"; break;	
		case OnCall:	UpdateMode = L"OnCall"; break;	

	}
}
void OleObject::processLinkInfoStream( VirtualStreamReader& reader )
{
	short cch = reader.ReadInt16();
	unsigned char* str = reader.ReadBytes( cch, true );
	FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &this->Link, str, cch, ENCODING_WINDOWS_1250 );
	RELEASEARRAYOBJECTS( str );
    
	//skip the terminating zero of the ANSI string
	//even if the characters are ANSI chars, the terminating zero has 2 bytes
	reader.ReadBytes( 2, false );

	//skip the next 4 bytes (flags?)
	reader.ReadBytes( 4, false );

	//Read the Unicode version
	this->Link.clear();

	cch = reader.ReadInt16();
	str = reader.ReadBytes( ( cch * 2 ), true );
	FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &this->Link, str, ( cch * 2 ), ENCODING_UTF16 );
	RELEASEARRAYOBJECTS( str );

	//skip the terminating zero of the Unicode string
	reader.ReadBytes( 2, false );
}
void OleObject::processCompObjStream( VirtualStreamReader& reader )
{
	//skip the CompObjHeader
	reader.ReadBytes( 28, false );

	unsigned int sz_obj = reader.GetSize() - reader.GetPosition();

	if (sz_obj > 4)
	{
		UserType = reader.ReadLengthPrefixedAnsiString(sz_obj);

		sz_obj = reader.GetSize() - reader.GetPosition();
		if (sz_obj > 4)
			ClipboardFormat	= reader.ReadLengthPrefixedAnsiString(sz_obj);

		sz_obj = reader.GetSize() - reader.GetPosition();
		if (sz_obj > 4)
			Program = reader.ReadLengthPrefixedAnsiString(sz_obj);
	}
}

std::wstring OleObject::getOleEntryName( const CharacterPropertyExceptions* chpx )
{
	std::wstring ret;

	if ( chpx != NULL )
	{
	  for ( std::list<SinglePropertyModifier>::const_iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
	  {
		if ( iter->OpCode == sprmCPicLocation || iter->OpCode == sprmOldCPicLocation)
		{
		  ret = ( L"_" + FormatUtils::IntToWideString( FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize ) ) );
	      
		  break;
		}
	  }
	}

	return ret;
}


}
