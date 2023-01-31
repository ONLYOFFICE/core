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

#include "PictureDescriptor.h"
#include "OfficeDrawing/MetafilePictBlip.h"

#include "../../DesktopEditor/raster/BgraFrame.h"

#ifndef MM_ISOTROPIC
	#define MM_ISOTROPIC 7
#endif

#ifndef MM_ANISOTROPIC
	#define MM_ANISOTROPIC 8
#endif

namespace ImageHelper
{
	Global::_BlipType SaveImageToFileFromDIB(unsigned char* data, int size, std::wstring& file_name)//without ext
	{
		Global::_BlipType result = Global::msoblipERROR;

		CBgraFrame oFrame;
		int offset = 0, biSizeImage = 0;

		__BITMAPINFOHEADER * header = (__BITMAPINFOHEADER*)data;
		if (!header) return result;

		result = Global::msoblipDIB;

		if (header->biWidth > 100000 || header->biHeight > 100000 || header->biSize != 40)
		{
			__BITMAPCOREHEADER * header_core = (__BITMAPCOREHEADER *)data;
			if (header_core->bcSize != 12)
			{
				result = Global::msoblipWMF;
			}
			else
			{
				offset = 12; //sizeof(BITMAPCOREHEADER)			
			
				oFrame.put_Height	(header_core->bcHeight );
				oFrame.put_Width	(header_core->bcWidth );
				
				int sz_bitmap = header_core->bcHeight * header_core->bcWidth * header_core->bcBitCount/ 8;
				
				//if (header_core->bcWidth % 2 != 0 && sz_bitmap < size - offset)
				//	header_core->bcWidth++;
				///???? todooo непонятно .. в biff5 нужно флипать картинку, в biff8 не ясно ( - 
				
				int stride =  -(size - offset) / header_core->bcHeight;
				oFrame.put_Stride	(stride/*header_core->bcBitCount * header_core->bcWidth /8 */);

				biSizeImage = size - offset;
				
				if (-stride >= header_core->bcWidth && header_core->bcBitCount >=24 )
				{
					result = Global::msoblipPNG;
				}
			}
		}
		else
		{
			offset = 40; //sizeof(BITMAPINFOHEADER)

			oFrame.put_Height	(header->biHeight );
			oFrame.put_Width	(header->biWidth );
			
			int sz_bitmap = header->biHeight * header->biWidth * header->biBitCount/ 8;
			
			//if (header->biWidth % 2 != 0 && sz_bitmap < size -offset)
			//	header->biWidth++;

			if (header->biClrUsed > 0)
			{
				oFrame.put_Palette((unsigned char*)data + offset, header->biClrUsed);
			
				offset += header->biClrUsed * 4;
			}
			
			int stride = -(size - offset) / header->biHeight;

			if (-stride >= header->biWidth/* && header->biBitCount >= 24*/)
			{
				result = Global::msoblipPNG;
			}
			oFrame.put_Stride(stride);
			
			biSizeImage = header->biSizeImage > 0 ? header->biSizeImage : (size - offset);
		}
		
//------------------------------------------------------------------------------------------

		if (result == Global::msoblipPNG)
		{
			oFrame.put_Data((unsigned char*)data + offset);
			
			file_name += L".png";
			if (!oFrame.SaveFile(file_name, 4/*CXIMAGE_FORMAT_PNG*/))
			{
				result = Global::msoblipDIB;
			}
			oFrame.put_Data(NULL);
		}
		else if (result == Global::msoblipWMF)
		{
			file_name += L".wmf";
			NSFile::CFileBinary file;
            if (file.CreateFileW(file_name))
			{
				file.WriteFile((BYTE*)data, size);
				file.CloseFile();
			}
		}
		if (biSizeImage > 0 && result == Global::msoblipDIB)
		{
			file_name += L".bmp";
			NSFile::CFileBinary file;
            if (file.CreateFileW(file_name))
			{
                _UINT16 vtType		= 0x4D42;				file.WriteFile((BYTE*)&vtType,	2);
                _UINT32 dwLen		= biSizeImage;			file.WriteFile((BYTE*)&dwLen,	4);
                _UINT32 dwRes		= 0;					file.WriteFile((BYTE*)&dwRes,	4);
                _UINT32 dwOffset	= 2;					file.WriteFile((BYTE*)&dwOffset, 4);
			
				file.WriteFile((BYTE*)data, size);
				file.CloseFile();
			}
		}
		return result;
	}
}
namespace DocFileFormat
{
	/// Parses the CHPX for a fcPic an loads the PictureDescriptor at this offset
	PictureDescriptor::PictureDescriptor(CharacterPropertyExceptions* chpx, POLE::Stream* stream, int size, int nWordVersion) 
		: 
		dxaGoal(0), dyaGoal(0), mx(0), my(0), Type(jpg), mfp(), dxaCropLeft(0), dyaCropTop(0),
		dxaCropRight(0), dyaCropBottom(0), brcTop(NULL), brcLeft(NULL), brcBottom(NULL), brcRight(NULL), dxaOrigin(0), dyaOrigin(0),
		cProps(0), shapeContainer(NULL), blipStoreEntry(NULL), embeddedData(NULL), embeddedDataSize(0), embeddedDataHeader(NULL)
	{
		//Get start and length of the PICT
		int fc = GetFcPic( chpx );

		if ( fc >= 0 )
		{
			parse( stream, fc, size, nWordVersion);
		}
	}
	PictureDescriptor::PictureDescriptor()
		: 
		dxaGoal(0), dyaGoal(0), mx(0), my(0), Type(jpg), mfp(), dxaCropLeft(0), dyaCropTop(0),
		dxaCropRight(0), dyaCropBottom(0), brcTop(NULL), brcLeft(NULL), brcBottom(NULL), brcRight(NULL), dxaOrigin(0), dyaOrigin(0),
		cProps(0), shapeContainer(NULL), blipStoreEntry(NULL), embeddedData(NULL), embeddedDataSize(0), embeddedDataHeader(NULL)
	{
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
		
		RELEASEARRAYOBJECTS(embeddedData);
		RELEASEARRAYOBJECTS(embeddedDataHeader);
	}
	void PictureDescriptor::parse(POLE::Stream* stream, int fc, int sz, int nWordVersion)
	{
		Clear();

		VirtualStreamReader reader(stream, fc, nWordVersion);

		int sz_stream = reader.GetSize();

		int lcb = reader.ReadInt32();
		
		int pos_start = reader.GetPosition();
		
		if (lcb > 10000000) 
			return;

		if (lcb > sz && sz != 1 && sz != 2) //bullet picture с неверным размером
		{
			unsigned char* bytes = reader.ReadBytes(sz - fc - 4, false);
			if ( bytes )
			{
				RELEASEARRAYOBJECTS( bytes );
			}
			return;
		}

		if (lcb < 10)
			return;
	
		int cbHeader	=	reader.ReadUInt16();

		mfp.mm			=	reader.ReadInt16();
		mfp.xExt		=	reader.ReadInt16();
		mfp.yExt		=	reader.ReadInt16();
		mfp.hMf			=	reader.ReadInt16();

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

		int brcl			=	reader.ReadInt16();

	// borders
		int bytesCount		=	(nWordVersion > 0) ? 2 : 4;
		
		bytes				=	reader.ReadBytes( bytesCount, true );
		brcTop				=	new BorderCode( bytes, bytesCount );
		RELEASEARRAYOBJECTS( bytes );

		bytes				=	reader.ReadBytes( bytesCount, true );
		brcLeft				=	new BorderCode( bytes, bytesCount );
		RELEASEARRAYOBJECTS( bytes );

		bytes				=	reader.ReadBytes( bytesCount, true );
		brcBottom			=	new BorderCode( bytes, bytesCount );
		RELEASEARRAYOBJECTS( bytes );

		bytes				=	reader.ReadBytes( bytesCount, true );
		brcRight			=	new BorderCode( bytes, bytesCount );
		RELEASEARRAYOBJECTS( bytes );

		dxaOrigin			=	reader.ReadInt16();
		dyaOrigin			=	reader.ReadInt16();

		int pos_end = reader.GetPosition();
		if (nWordVersion > 0)
		{
			int flag = brcl;

			brcl = FormatUtils::BitmaskToBool(flag, 0x000F);
			//(	0	single 	1	thick	2	double	3	shadow )

			bool fFrameEmpty	= FormatUtils::BitmaskToBool(flag, 0x0010);//	picture consists of a single frame
			bool fBitmap		= FormatUtils::BitmaskToBool(flag, 0x0020);//	==1, when picture is just a bitmap
			bool fDrawHatch		= FormatUtils::BitmaskToBool(flag, 0x0040);//	==1, when picture is an active OLE object
			bool fError			= FormatUtils::BitmaskToBool(flag, 0x0080);//	==1, when picture is just an error message
			short bpp			= FormatUtils::BitmaskToBool(flag, 0x8000);//	bits per pixel
				//(0 unknown 1 monochrome 4 VGA)

			int sz_hdr = pos_end - pos_start;

			int header_size = 114;

			embeddedDataSize	=	lcb - sz_hdr - header_size;	
			embeddedDataHeader	=	reader.ReadBytes( header_size, true);
			embeddedData		=	reader.ReadBytes( embeddedDataSize, true );
		}
		else
		{
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

	/// Returns the fcPic into the "data" stream, where the PIC begins.
	/// Returns -1 if the CHPX has no fcPic.
	int PictureDescriptor::GetFcPic(const CharacterPropertyExceptions* chpx)
	{
		int ret = -1, ret1 = -1;

		for ( std::list<SinglePropertyModifier>::const_iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
		{
			switch ( iter->OpCode )
			{
			case sprmOldCPicLocation:
			case sprmCPicLocation:
				ret = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				break;

			case sprmOldCHps:
			case sprmCHps:
				ret1 = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				break;

			case sprmCHsp:
				ret = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				break;

			case sprmOldCFData:
			case sprmCFData:
				break;
			default:
				break;
			}
		}

		return ret;
	}
}