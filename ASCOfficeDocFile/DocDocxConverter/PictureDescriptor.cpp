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

#include "PictureDescriptor.h"

namespace DocFileFormat
{
	/// Parses the CHPX for a fcPic an loads the PictureDescriptor at this offset
	PictureDescriptor::PictureDescriptor(CharacterPropertyExceptions* chpx, POLE::Stream* stream, int size, bool oldVersion) 
		: 
		dxaGoal(0), dyaGoal(0), mx(0), my(0), Type(jpg), Name( _T( "" ) ), mfp(), dxaCropLeft(0), dyaCropTop(0),
		dxaCropRight(0), dyaCropBottom(0), brcTop(NULL), brcLeft(NULL), brcBottom(NULL), brcRight(NULL), dxaOrigin(0), dyaOrigin(0),
		cProps(0), shapeContainer(NULL), blipStoreEntry(NULL)
	{
		//Get start and length of the PICT
		int fc = GetFcPic( chpx );


		if ( fc >= 0 )
		{
			parse( stream, fc, size, oldVersion);
		}
	}

	PictureDescriptor::~PictureDescriptor()
	{
		Clear();
	}
	void PictureDescriptor::Clear()
	{
		RELEASEOBJECT(brcTop);
		RELEASEOBJECT(brcLeft);
		RELEASEOBJECT(brcBottom);
		RELEASEOBJECT(brcRight);
	
		RELEASEOBJECT(shapeContainer);
		RELEASEOBJECT(blipStoreEntry);
	}
	void PictureDescriptor::parse(POLE::Stream* stream, int fc, int sz, bool oldVersion)
	{
		Clear();

		VirtualStreamReader reader(stream, fc, oldVersion);

		int sz_stream = reader.GetSize();

		int lcb = reader.ReadInt32();
		
		if (lcb > 10000000) 
			return;

		if (lcb > sz && sz != 2) //bullet picture с неверным размером
		{
			unsigned char* bytes = reader.ReadBytes(sz - fc - 4, false);
			if ( bytes )
			{
				RELEASEARRAYOBJECTS( bytes );
			}
			return;
		}

		if (lcb >= 10)
		{
			int cbHeader	=	 reader.ReadUInt16();

			mfp.mm			=	reader.ReadInt16();
			mfp.xExt		=	reader.ReadInt16();
			mfp.yExt		=	reader.ReadInt16();
			mfp.hMf			=	reader.ReadInt16();

			if (mfp.mm >= 98 || oldVersion)
			{
				unsigned char* bytes	=	reader.ReadBytes(14, true);
				rcWinMf					=	std::vector<unsigned char>(bytes, (bytes + 14));
				RELEASEARRAYOBJECTS(bytes);

				//dimensions
				dxaGoal				=	reader.ReadInt16();
				dyaGoal				=	reader.ReadInt16();
				mx					=	reader.ReadUInt16();
				my					=	reader.ReadUInt16();

				//cropping
				dxaCropLeft			=	reader.ReadInt16();
				dyaCropTop			=	reader.ReadInt16();
				dxaCropRight		=	reader.ReadInt16();
				dyaCropBottom		=	reader.ReadInt16();

				short brcl			=	reader.ReadInt16();

				// borders

				int bytesCount		=	4;
				bytes				=	reader.ReadBytes( bytesCount, true );
				
				brcTop				=	new BorderCode( bytes, bytesCount );
				RELEASEARRAYOBJECTS( bytes );

				bytes				=	reader.ReadBytes( bytesCount, true );
				brcLeft				=	new BorderCode( bytes, 4 );
				RELEASEARRAYOBJECTS( bytes );

				bytes				=	reader.ReadBytes( bytesCount, true );
				brcBottom			=	new BorderCode( bytes, 4 );
				RELEASEARRAYOBJECTS( bytes );

				bytes				=	reader.ReadBytes( bytesCount, true );
				brcRight			=	new BorderCode( bytes, 4 );
				RELEASEARRAYOBJECTS( bytes );

				dxaOrigin			=	reader.ReadInt16();
				dyaOrigin			=	reader.ReadInt16();
				cProps				=	reader.ReadInt16();

				if (mfp.mm == MM_SHAPEFILE)
				{
					unsigned char cchPicName	=	reader.ReadByte();
					unsigned char* stPicName	=	reader.ReadBytes(cchPicName, true);

					if ( stPicName != NULL )
					{
						std::wstring picName;
						FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &picName, stPicName, cchPicName, ENCODING_WINDOWS_1250 );
						RELEASEARRAYOBJECTS(stPicName);
					}
				}

				if (oldVersion)
				{
					////blipStoreEntry = new BlipStoreEntry();

					//blipStoreEntry = new BlipStoreEntry(&reader,lcb, Global::msoblipDIB,0,0);
					//long pos = reader.GetPosition();

					//unsigned char* pPicData	=	reader.ReadBytes(lcb - pos, true);

					//int pos1 = 0;

					//BITMAPINFOHEADER *bm = (BITMAPINFOHEADER *)(pPicData + pos1);

					//NSFile::CFileBinary f;
					//
					//f.CreateFile(L"d:\\test.jpg");
					//f.WriteFile(pPicData + pos1, lcb - pos - pos1);
					//f.CloseFile();

					//RELEASEARRAYOBJECTS(pPicData);

				}
				else
				{
					//Parse the OfficeDrawing Stuff
					shapeContainer	=	dynamic_cast<ShapeContainer*>(RecordFactory::ReadRecord(&reader, 0));

					long pos = reader.GetPosition();

					if( pos < ( fc + lcb ))
					{
						Record* rec = RecordFactory::ReadRecord( &reader, 0 );

						if ((rec) && ( typeid(*rec) == typeid(BlipStoreEntry) ))
						{
							blipStoreEntry = dynamic_cast<BlipStoreEntry*>( rec );
						}
						else
						{
							RELEASEOBJECT(rec);
						}
					}
				}
			}
		}
	}

	/// Returns the fcPic into the "data" stream, where the PIC begins.
	/// Returns -1 if the CHPX has no fcPic.
	int PictureDescriptor::GetFcPic(const CharacterPropertyExceptions* chpx)
	{
		int ret = -1;

		for ( std::list<SinglePropertyModifier>::const_iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
		{
			switch ( iter->OpCode )
			{
			case sprmOldCPicLocation:
			case sprmCPicLocation:
				ret = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				break;

			case sprmCHsp:
				ret = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				break;

			case sprmOldCFData:
			case sprmCFData:
				break;
			}
		}

		return ret;
	}
}