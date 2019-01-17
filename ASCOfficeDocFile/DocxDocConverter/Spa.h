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

#include "../Common/FormatUtils.h"

namespace ASCDocFileFormat
{
	// File Shape Address (FSPA)
	class Spa : public IOperand
	{
	public:	
		typedef enum AnchorType
		{
			MARGIN,
			PAGE,
			TEXT
		} AnchorType;

		struct Rca
		{
			int left;
			int top;
			int right;
			int bottom;
		};

		static const int SIZE_IN_BYTES = 26;

	public:

		Spa () : m_lid(-1), wr(0), wrk(0), cTxbx (0)
		{
			fBelowText		=	false;
			fAnchorLock		=	false;
			
			m_bFlip			=	FALSE;
		}

		Spa (int id, int nX, int nY, int nWidth, int nHeight)
		{
			m_lid			=	id; 
			m_rca.left		=	nX; 
			m_rca.top		=	nY;
			m_rca.right		=	nX + nWidth;
			m_rca.bottom	=	nY + nHeight;
			bx				=	TEXT;
			by				=	TEXT;
			wr				=	3;
			wrk				=	0;

			fBelowText		=	false;
			fAnchorLock		=	false;

			cTxbx			=	0;

			m_bFlip			=	FALSE;

			Update ();
		}

		Spa (const Spa& spa)
		{
			m_lid		=	spa.m_lid;
			m_rca		=	spa.m_rca;
			fHdr		=	spa.fHdr;
			bx			=	spa.bx;
			by			=	spa.by;
			wr			=	spa.wr;
			wrk			=	spa.wrk;
			fRcaSimple	=	spa.fRcaSimple;
			fBelowText	=	spa.fBelowText;
			fAnchorLock	=	spa.fAnchorLock;
			cTxbx		=	spa.cTxbx;
			m_bFlip		=	spa.m_bFlip;

			memset(bytes, 0, SIZE_IN_BYTES);
			memcpy(bytes, spa.bytes, SIZE_IN_BYTES);
		}

		virtual operator unsigned char*() const
		{
			return (unsigned char*)(bytes);
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)bytes;
		}

		virtual unsigned int Size() const
		{
			return sizeof(bytes);
		}

		inline void Update ()
		{
			memset(bytes, 0, SIZE_IN_BYTES);

			unsigned int offset = 0;
			
			FormatUtils::SetBytes((bytes + offset), m_lid);				offset += sizeof(m_lid);

			if (m_bFlip) // FLIP
			{
				int dX		=	(m_rca.left+m_rca.right)/2;
				int dY		=	(m_rca.top+m_rca.bottom)/2;
				int dWidth	=	abs(m_rca.left - m_rca.right)/2;
				int dHeight	=	abs(m_rca.bottom - m_rca.top)/2;
				
				FormatUtils::SetBytes((bytes + offset), dX - dHeight);	offset += sizeof(int);
				FormatUtils::SetBytes((bytes + offset), dY - dWidth);	offset += sizeof(int);
				FormatUtils::SetBytes((bytes + offset), dX + dHeight);	offset += sizeof(int);
				FormatUtils::SetBytes((bytes + offset), dY + dWidth);	offset += sizeof(int);
			}
			else
			{
				FormatUtils::SetBytes((bytes + offset), m_rca.left);	offset += sizeof(m_rca.left);
				FormatUtils::SetBytes((bytes + offset), m_rca.top);		offset += sizeof(m_rca.top);
				FormatUtils::SetBytes((bytes + offset), m_rca.right);	offset += sizeof(m_rca.right);
				FormatUtils::SetBytes((bytes + offset), m_rca.bottom);	offset += sizeof(m_rca.bottom);
			}

			ASCDocFormatUtils::BitSet oBits(2);

			oBits.SetBit(false,				0);	//	A			ignored

			if (MARGIN == bx)
			{
				oBits.SetBit(false,			1); //	bx
				oBits.SetBit(false,			2);
			} 
			else if (PAGE == bx)
			{
				oBits.SetBit(true,			1); //	bx
				oBits.SetBit(false,			2);
			}
			else if (TEXT == bx)
			{
				oBits.SetBit(false,			1); //	bx
				oBits.SetBit(true,			2);
			}
			else
			{
				oBits.SetBit(true,			1); //	bx
				oBits.SetBit(true,			2);
			}

			if (MARGIN == by)
			{
				oBits.SetBit(false,			3); //	by
				oBits.SetBit(false,			4);
			} 
			else if (PAGE == by)
			{
				oBits.SetBit(true,			3); //	by
				oBits.SetBit(false,			4);
			}
			else if (TEXT == by)
			{
				oBits.SetBit(false,			3); //	by
				oBits.SetBit(true,			4);
			}

			oBits.SetBits<unsigned short> (wr, 5, 4);
			oBits.SetBits<unsigned short> (wrk, 9, 4);

			oBits.SetBit(false,			13); //	ignored
			oBits.SetBit(fBelowText,	14);	
			oBits.SetBit(fAnchorLock,	15);	

			FormatUtils::SetBytes((bytes + offset),	FormatUtils::BytesToInt16 (oBits.GetBytes(), 0, sizeof(unsigned short)));

			offset += sizeof(oBits.GetSize());
			offset += sizeof(int);
		}

		inline void SetFlip (bool bFlip)
		{
			m_bFlip	=	bFlip;
			
			Update ();
		}

	public:

		unsigned char bytes[SIZE_IN_BYTES];

		/// Shape Identifier. Used in conjunction with the office art data 
		/// (found via fcDggInfo in the FIB) to find the actual data for newObject shape.
		int m_lid;
		/// An Rca structure that specifies the rectangle where the drawing exists. The 
		/// coordinates of rca are in twips. 
		Rca m_rca;		
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

		bool	m_bFlip;		//	если объект повернут по определенному углу то нужно флипнуть RCA структуру
	};
}
