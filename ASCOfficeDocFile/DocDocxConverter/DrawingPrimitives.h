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

#include <string>
#include <vector>

#include "IVisitable.h"

class VirtualStreamReader;

namespace DocFileFormat
{
	class DrawingPrimitive
	{
	public:
		DrawingPrimitive(VirtualStreamReader* reader, int length) ;
		virtual ~DrawingPrimitive() {}

		short	type;

		short	xa;
		short	ya;	
		short	dxa;
		short	dya;

		short	lineWeight;
		short	lineStyle;
		long	lineColor;

		long 	fillBack;
		long	fillFore;
		short	fillPattern;

		short	shadowInt;
		short	shadowX;
		short	shadowY;

		std::wstring strVmlType;

		void read_fill	(VirtualStreamReader* reader);
		void read_line	(VirtualStreamReader* reader);
		void read_shadow(VirtualStreamReader* reader);
		long read_color (VirtualStreamReader* reader);

		static 	DrawingPrimitive * Create(VirtualStreamReader* reader, int length, short dpk);
	};
	
	class DrawingPrimitiveGroup : public DrawingPrimitive
	{
	public:
		DrawingPrimitiveGroup(VirtualStreamReader* reader, int length, bool start);

		bool bStart;
		short val;
	};

	class DrawingPrimitiveLine : public DrawingPrimitive
	{
	public:
		DrawingPrimitiveLine(VirtualStreamReader* reader, int length, bool read_as_line = true) ;

		void read_epp (		VirtualStreamReader* reader);
		void read_start_end	(VirtualStreamReader* reader);

		short xaStart;
		short yaStart;
		short xaEnd;
		short yaEnd;

		short eppsStart;	//Start EndPoint Property Style
		short eppwStart;	//Start EndPoint Property Weight
		short epplStart;	//Start EndPoint Property length
		short eppsEnd;		//End EndPoint Property Style
		short eppwEnd;		//End EndPoint Property Weight
		short epplEnd;		//End EndPoint Property length

	};
	class DrawingPrimitiveRect: public DrawingPrimitive
	{
	public:
		DrawingPrimitiveRect(VirtualStreamReader* reader, int length);

		bool			fRoundCorners;
		unsigned short	zaShape;
	};

	class DrawingPrimitiveTextBox: public DrawingPrimitiveRect
	{
	public:
		DrawingPrimitiveTextBox(VirtualStreamReader* reader, int length);

	};

	class DrawingPrimitiveElipse: public DrawingPrimitive
	{
	public:
		DrawingPrimitiveElipse(VirtualStreamReader* reader, int length);

	};

	class DrawingPrimitivePolyline: public DrawingPrimitiveLine
	{
	public:
		DrawingPrimitivePolyline(VirtualStreamReader* reader, int length);

		bool	fPolygon;
		short	count;

		std::vector<std::pair<short, short>> arPoints;
	};

	class DrawingPrimitiveCTextBox: public DrawingPrimitive
	{
	public:
		DrawingPrimitiveCTextBox(VirtualStreamReader* reader, int length);
		virtual ~DrawingPrimitiveCTextBox();
		
		unsigned short	dzaOffset;
		unsigned short	dzaDescent;
		unsigned short	dzaLength;

		DrawingPrimitiveTextBox		*txbx;
		DrawingPrimitivePolyline	*polyline;
	};

	class DrawingPrimitiveArc: public DrawingPrimitive
	{
	public:
		DrawingPrimitiveArc(VirtualStreamReader* reader, int length);

		bool fLeft;
		bool fUp;
	};

//-------------------------------------------------------------------------------------
	class DrawingPrimitives : public IVisitable, public std::vector<DrawingPrimitive*>
	{
	public:
		DrawingPrimitives () {}

	};
}