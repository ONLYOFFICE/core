
#include "PictureDescriptor.h"

namespace DocFileFormat
{
	/// Parses the CHPX for a fcPic an loads the PictureDescriptor at this offset
	PictureDescriptor::PictureDescriptor(CharacterPropertyExceptions* chpx, POLE::Stream* stream, int size) : 
		dxaGoal(0), dyaGoal(0), mx(0), my(0), Type(jpg), Name( _T( "" ) ), mfp(), dxaCropLeft(0), dyaCropTop(0),
		dxaCropRight(0), dyaCropBottom(0), brcTop(NULL), brcLeft(NULL), brcBottom(NULL), brcRight(NULL), dxaOrigin(0), dyaOrigin(0),
		cProps(0), shapeContainer(NULL), blipStoreEntry(NULL)
	{
		//Get start and length of the PICT
		int fc = GetFcPic( chpx );


		if ( fc >= 0 )
		{
			parse( stream, fc, size );
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
	void PictureDescriptor::parse(POLE::Stream* stream, int fc, int sz)
	{
		Clear();

		VirtualStreamReader reader(stream, fc);

		int lcb = reader.ReadInt32();
		
		if (lcb > 10000000) 
			return;

		if (lcb > sz) 
		{
			unsigned char* bytes = reader.ReadBytes(sz - fc - 4, false);
			if ( bytes )
			{
				RELEASEARRAYOBJECTS( bytes );
			}
			return;
		}

		if (lcb > 0)
		{
			unsigned short cbHeader	=	reader.ReadUInt16();

			mfp.mm					=	reader.ReadInt16();
			mfp.xExt				=	reader.ReadInt16();
			mfp.yExt				=	reader.ReadInt16();
			mfp.hMf					=	reader.ReadInt16();

			if (mfp.mm > 98)
			{
				unsigned char* bytes			=	reader.ReadBytes(14, true);
				rcWinMf				=	vector<unsigned char>(bytes, (bytes + 14));
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
						FormatUtils::GetSTLCollectionFromBytes<wstring>( &picName, stPicName, cchPicName, ENCODING_WINDOWS_1251 );
						RELEASEARRAYOBJECTS(stPicName);
					}
				}

				//Parse the OfficeDrawing Stuff
				shapeContainer	=	static_cast<ShapeContainer*>(RecordFactory::ReadRecord(&reader, 0));

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

	/// Returns the fcPic into the "data" stream, where the PIC begins.
	/// Returns -1 if the CHPX has no fcPic.
	int PictureDescriptor::GetFcPic(const CharacterPropertyExceptions* chpx)
	{
		int ret = -1;

		for ( list<SinglePropertyModifier>::const_iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
		{
			switch ( iter->OpCode )
			{
			case sprmCPicLocation:
				ret = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				break;

			case sprmCHsp:
				ret = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				break;

			case sprmCFData:
				break;
			}
		}

		return ret;
	}
}