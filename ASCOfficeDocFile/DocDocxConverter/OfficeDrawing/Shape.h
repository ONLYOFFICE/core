#pragma once

#include "Record.h"
#include "ShapeTypeFactory.h"

namespace DocFileFormat
{
	class Shape: public Record
	{
	public: 
		int spid;
		/// This shape is a group shape 
		bool fGroup;
		/// Not a top-level shape 
		bool fChild;
		/// This is the topmost group shape.
		/// Exactly one of these per drawing. 
		bool fPatriarch; 
		/// The shape has been deleted 
		bool fDeleted;
		/// The shape is an OLE object 
		bool fOleShape;
		/// Shape has a hspMaster property 
		bool fHaveMaster;
		/// Shape is flipped horizontally 
		bool fFlipH;
		/// Shape is flipped vertically 
		bool fFlipV;
		/// Connector type of shape 
		bool fConnector;
		/// Shape has an anchor of some kind 
		bool fHaveAnchor;
		/// Background shape 
		bool fBackground;
		/// Shape has a shape type property
		bool fHaveSpt;
		/// The shape type of the shape
		ShapeType* shapeType;

	public:
		static const unsigned short TYPE_CODE_0xF00A = 0xF00A;

		Shape():
		Record(), spid(0), fGroup(false), fChild(false), fPatriarch(false), fDeleted(false), fOleShape(false),
			fHaveMaster(false), fFlipH(false), fFlipV(false), fConnector(false), fHaveAnchor(false), fBackground(false),
			fHaveSpt(false), shapeType(NULL)
		{
		}

		virtual ~Shape()
		{
			RELEASEOBJECT( this->shapeType );
		}

		Shape( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
		Record( _reader, size, typeCode, version, instance ), spid(0), fGroup(false), fChild(false), fPatriarch(false), fDeleted(false), fOleShape(false),
			fHaveMaster(false), fFlipH(false), fFlipV(false), fConnector(false), fHaveAnchor(false), fBackground(false),
			fHaveSpt(false), shapeType(NULL)
		{
			spid				=	Reader->ReadInt32();

			unsigned int flag	=	Reader->ReadUInt32();

			fGroup				=	FormatUtils::BitmaskToBool( flag, 0x1 );
			fChild				=	FormatUtils::BitmaskToBool( flag, 0x2 );
			fPatriarch			=	FormatUtils::BitmaskToBool( flag, 0x4 );
			fDeleted			=	FormatUtils::BitmaskToBool( flag, 0x8 );
			fOleShape			=	FormatUtils::BitmaskToBool( flag, 0x10 );
			fHaveMaster			=	FormatUtils::BitmaskToBool( flag, 0x20 );
			fFlipH				=	FormatUtils::BitmaskToBool( flag, 0x40 );
			fFlipV				=	FormatUtils::BitmaskToBool( flag, 0x80 );
			fConnector			=	FormatUtils::BitmaskToBool( flag, 0x100 );
			fHaveAnchor			=	FormatUtils::BitmaskToBool( flag, 0x200 );
			fBackground			=	FormatUtils::BitmaskToBool( flag, 0x400 );
			fHaveSpt			=	FormatUtils::BitmaskToBool( flag, 0x800 );
			shapeType			=	ShapeTypeFactory::NewShapeType((MSOSPT)Instance);
		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new Shape( _reader, bodySize, typeCode, version, instance );
		}

		inline int GetShapeID() const
		{
			return this->spid;
		}

		inline ShapeType* GetShapeType() const
		{
			return this->shapeType;
		}

		template<class T> bool is() const
		{
			bool isResult = false;

			if ( shapeType != NULL )
			{
				isResult = ( typeid(*shapeType) == typeid(T) );
			}

			return isResult;
		}
	};
}