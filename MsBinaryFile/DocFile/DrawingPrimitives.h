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