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

#include "ByteStructure.h"
#include "DrawingPrimitives.h"

class VirtualStreamReader;

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
		friend class DocumentMapping;
		
		static const int STRUCTURE_SIZE		= 26;
		static const int STRUCTURE_SIZE_OLD = 6;

		static const int GetSize(int nWordVersion)
		{
			return (nWordVersion > 0) ? STRUCTURE_SIZE_OLD : STRUCTURE_SIZE;
		}

		Spa();
		virtual ~Spa();

        virtual ByteStructure* ConstructObject(VirtualStreamReader* reader, int length);
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
		bool fBelowText;	// true: shape is below text
		bool fAnchorLock;	// true: anchor is locked
		int cTxbx;			// Count of textboxes in shape (undo doc only)

		DrawingPrimitives primitives;		

		void read_primitives(Spa* pSpa, VirtualStreamReader* reader, int length);
	};
}
