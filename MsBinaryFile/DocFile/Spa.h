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
