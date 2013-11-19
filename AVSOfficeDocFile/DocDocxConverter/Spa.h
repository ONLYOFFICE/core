#pragma once

#include "VirtualStreamReader.h"
#include "ByteStructure.h"

namespace DocFileFormat
{
	typedef enum AnchorType
	{
		MARGIN,
		PAGE,
		TEXT
	} AnchorType;

	class Spa : public ByteStructure
	{
	public:
		friend class VMLShapeMapping;
		static const int STRUCTURE_SIZE = 26;

		Spa()
		{

		}

		virtual ~Spa()
		{

		}

		virtual ByteStructure* ConstructObject(VirtualStreamReader* reader, int length)
		{
			Spa* pSpa				=	new Spa();  
			if (pSpa)
			{
				pSpa->spid			=	reader->ReadInt32();
				pSpa->xaLeft		=	reader->ReadInt32();
				pSpa->yaTop			=	reader->ReadInt32();
				pSpa->xaRight		=	reader->ReadInt32();
				pSpa->yaBottom		=	reader->ReadInt32();

				unsigned short flag	=	reader->ReadUInt16();

				pSpa->fHdr			=	FormatUtils::BitmaskToBool(flag, 0x0001);
				pSpa->bx			=	(AnchorType)FormatUtils::BitmaskToInt(flag, 0x0006);
				pSpa->by			=	(AnchorType)FormatUtils::BitmaskToInt(flag, 0x0018);
				pSpa->wr			=	(unsigned short)FormatUtils::BitmaskToInt(flag, 0x01E0);
				pSpa->wrk			=	(unsigned short)FormatUtils::BitmaskToInt(flag, 0x1E00);
				pSpa->fRcaSimple	=	FormatUtils::BitmaskToBool(flag, 0x2000);
				pSpa->fBelowText	=	FormatUtils::BitmaskToBool(flag, 0x4000);
				pSpa->fAnchorLock	=	FormatUtils::BitmaskToBool(flag, 0x8000);

				pSpa->cTxbx			=	reader->ReadInt32();

				return static_cast<ByteStructure*>(pSpa);
			}

			return NULL;
		}

		inline int GetShapeID() const
		{
			return spid;
		}

	private:

		/// Shape Identifier. Used in conjunction with the office art data 
		/// (found via fcDggInfo in the FIB) to find the actual data for newObject shape.
		int spid;
		/// Left of rectangle enclosing shape relative to the origin of the shape
		int xaLeft;
		/// Top of rectangle enclosing shape relative to the origin of the shape
		int yaTop;
		/// Right of rectangle enclosing shape relative to the origin of the shape
		int xaRight;
		/// Bottom of the rectangle enclosing shape relative to the origin of the shape
		int yaBottom;
		/// true in the undo doc when shape is from the header doc
		/// false otherwise (undefined when not in the undo doc)
		bool fHdr;
		/// X position of shape relative to anchor CP
		/// 0 relative to page margin
		/// 1 relative to top of page
		/// 2 relative to text (column for horizontal text; paragraph for vertical text)
		/// 3 reserved for future use
		AnchorType bx;
		/// Y position of shape relative to anchor CP
		/// 0 relative to page margin
		/// 1 relative to top of page
		/// 2 relative to text (column for horizontal text; paragraph for vertical text)
		/// 3 reserved for future use
		AnchorType by;
		/// Text wrapping mode
		/// 0 like 2, but doesn‘t require absolute object
		/// 1 no text next to shape
		/// 2 wrap around absolute object
		/// 3 wrap as if no object present
		/// 4 wrap tightly around object
		/// 5 wrap tightly, but allow holes
		/// 6-15 reserved for future use
		unsigned short wr;
		/// Text wrapping mode type (valid only for wrapping modes 2 and 4)
		/// 0 wrap both sides
		/// 1 wrap only on left
		/// 2 wrap only on right
		/// 3 wrap only on largest side
		unsigned short wrk;
		/// When set, temporarily overrides bx, by, 
		/// forcing the xaLeft, xaRight, yaTop, and yaBottom fields 
		/// to all be page relative.
		bool fRcaSimple;
		/// true: shape is below text
		/// false: shape is above text
		bool fBelowText;
		/// true: anchor is locked
		/// fasle: anchor is not locked
		bool fAnchorLock;
		/// Count of textboxes in shape (undo doc only)
		int cTxbx;
	};
}
