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

#include "DrawingPrimitives.h"
#include "VirtualStreamReader.h"


namespace DocFileFormat
{
	DrawingPrimitive * DrawingPrimitive::Create(VirtualStreamReader* reader, int length, short dpk)
	{
		DrawingPrimitive * dp = NULL;

		int pos1 = reader->GetPosition();
		switch(dpk)
		{
		case 0x0000:
			dp = new DrawingPrimitiveGroup(reader, length, true);
			break;
		case 0x0001:
			dp = new DrawingPrimitiveLine(reader, length);
			break;
		case 0x0002: //textbox
			dp = new DrawingPrimitiveTextBox(reader, length);
			break;
		case 0x0003: //rectangle
			dp = new DrawingPrimitiveRect(reader, length);
			break;
		case 0x0004: //elipse
			dp = new DrawingPrimitiveElipse(reader, length);
			break;
		case 0x0005: //arc
			dp = new DrawingPrimitiveArc(reader, length);
			break;
		case 0x0006: //polyline
			dp = new DrawingPrimitivePolyline(reader, length);
			break;
		case 0x0007: //callout textbox
			dp = new DrawingPrimitiveCTextBox(reader, length);
			break;
		case 0x0008: //callout textbox
			dp = new DrawingPrimitiveGroup(reader, length, false);
			break;
		case 0x0009: //sample primitive holding default values
		default:
			dp = new DrawingPrimitive(reader, length);
			break;
		}
		if (dp)
		{
			dp->type = dpk;
		}

		int pos2 = reader->GetPosition();
		if (pos2 - pos1 < length && dpk != 0 && dpk != 8)
		{
			int sz = length - (pos2 - pos1);
			unsigned char * data = reader->ReadBytes(sz, true);
			if (data)
				delete []data;
		}

		return dp;
	}

	DrawingPrimitive::DrawingPrimitive(VirtualStreamReader *reader, int length) :
						lineWeight(0), lineStyle(0), lineColor(0), fillBack(0), fillFore(0), fillPattern(0), shadowX(0), shadowY(0)
	{
		xa		= reader->ReadInt16();
		ya		= reader->ReadInt16();
		dxa		= reader->ReadInt16();
		dya		= reader->ReadInt16();
	}
	
	void DrawingPrimitive::read_fill(VirtualStreamReader* reader)
	{
		fillFore	= read_color(reader); 
		fillBack	= read_color(reader); 
		fillPattern	= reader->ReadInt16();
	}
	void DrawingPrimitive::read_shadow(VirtualStreamReader* reader)
	{
		shadowInt	= reader->ReadInt16();
		shadowX		= reader->ReadInt16();
		shadowY		= reader->ReadInt16();
	}
	void DrawingPrimitive::read_line(VirtualStreamReader* reader)
	{
		lineColor	= read_color(reader); 
		lineWeight	= reader->ReadInt16();
		lineStyle	= reader->ReadInt16();
	}
	const static int shemeDefaultColor[] = 
	{
		0x00000000,	0x00FFFFFF,	0x00FF0000,	0x0000FF00,	0x000000FF,	0x00FFFF00,	0x00FF00FF,	0x0000FFFF,	
		0x00000000,	0x00FFFFFF,	0x00FF0000,	0x0000FF00,	0x000000FF,	0x00FFFF00,	0x00FF00FF,	0x0000FFFF,	
		0x00800000,	0x00008000,	0x00000080,	0x00808000,	0x00800080,	0x00008080,	0x00C0C0C0,	0x00808080,	
		0x009999FF,	0x00993366,	0x00FFFFCC,	0x00CCFFFF,	0x00660066,	0x00FF8080,	0x000066CC,	0x00CCCCFF,	
		0x00000080,	0x00FF00FF,	0x00FFFF00,	0x0000FFFF,	0x00800080,	0x00800000,	0x00008080,	0x000000FF,	
		0x0000CCFF,	0x00CCFFFF,	0x00CCFFCC,	0x00FFFF99,	0x0099CCFF,	0x00FF99CC,	0x00CC99FF,	0x00FFCC99,	
		0x003366FF,	0x0033CCCC,	0x0099CC00,	0x00FFCC00,	0x00FF9900,	0x00FF6600,	0x00666699,	0x00969696,	
		0x00003366,	0x00339966,	0x00003300,	0x00333300,	0x00993300,	0x00993366,	0x00333399,	0x00333333
	};

	long DrawingPrimitive::read_color(VirtualStreamReader* reader)
	{
		//int ind = reader->ReadUInt32();

		//return shemeDefaultColor[ind];

		unsigned r = reader->ReadByte();
		unsigned g = reader->ReadByte();
		unsigned b = reader->ReadByte();
		unsigned a = reader->ReadByte();
		return ( (r<<16) | (g<<8) | (b) );
	}
	
	DrawingPrimitiveGroup::DrawingPrimitiveGroup(VirtualStreamReader *reader, int length, bool start) : DrawingPrimitive(reader, length)
	{
		strVmlType	= L"v:group";
		
		bStart = start;

		val	= reader->ReadInt16();
	}

	DrawingPrimitiveLine::DrawingPrimitiveLine(VirtualStreamReader *reader, int length, bool read_as_line) : DrawingPrimitive(reader, length)
	{
		strVmlType	= L"v:line";

		if (!read_as_line) return;

		read_start_end (reader);
		read_line(reader);
		read_epp(reader);
		read_shadow(reader);
	}
	
	void DrawingPrimitiveLine::read_epp(VirtualStreamReader* reader)
	{
		unsigned short f = reader->ReadUInt16();

		eppsStart	= FormatUtils::BitmaskToInt (f, 0x0003);
		eppwStart	= FormatUtils::BitmaskToInt (f, 0x000c);
		epplStart	= FormatUtils::BitmaskToInt (f, 0x0030);

		f = reader->ReadUInt16();

		eppsEnd		= FormatUtils::BitmaskToInt (f, 0x0003);	
		eppwEnd		= FormatUtils::BitmaskToInt (f, 0x000c);	
		epplEnd		= FormatUtils::BitmaskToInt (f, 0x0030);	
	}
	
	void DrawingPrimitiveLine::read_start_end(VirtualStreamReader* reader)
	{
		xaStart		= reader->ReadInt16();
		yaStart		= reader->ReadInt16();
		xaEnd		= reader->ReadInt16();
		yaEnd		= reader->ReadInt16();
	}

	DrawingPrimitiveRect::DrawingPrimitiveRect(VirtualStreamReader *reader, int length) : DrawingPrimitive(reader, length)
	{
		strVmlType	= L"v:rect";

		read_line(reader);
		read_fill(reader);
		read_shadow(reader);

		unsigned short f = reader->ReadUInt16();

		fRoundCorners	= FormatUtils::BitmaskToInt (f, 0x0001);
		zaShape			= FormatUtils::BitmaskToInt (f, 0x000e);
	}
	DrawingPrimitiveArc::DrawingPrimitiveArc(VirtualStreamReader *reader, int length) : DrawingPrimitive(reader, length)
	{
		strVmlType	= L"v:oval";//L"v:shape";

		read_line	(reader);
		read_fill	(reader);
		read_shadow	(reader);
		
		unsigned short f = reader->ReadUInt16();

		fLeft	= FormatUtils::BitmaskToInt (f, 0x00ff);
		fUp		= FormatUtils::BitmaskToInt (f, 0xff00);
	}

	DrawingPrimitiveElipse::DrawingPrimitiveElipse(VirtualStreamReader *reader, int length) : DrawingPrimitive(reader, length)
	{
		strVmlType	= L"v:oval";

		read_line	(reader);
		read_fill	(reader);
		read_shadow	(reader);
	}
	DrawingPrimitiveTextBox::DrawingPrimitiveTextBox(VirtualStreamReader *reader, int length) : DrawingPrimitiveRect(reader, length)
	{
		strVmlType	= L"v:rect";
		//strVmlType	= L"v:shape";
	}

	DrawingPrimitiveCTextBox::DrawingPrimitiveCTextBox(VirtualStreamReader *reader, int length) : DrawingPrimitive(reader, length)
	{
		strVmlType	= L"v:shape";
	}

	DrawingPrimitivePolyline::DrawingPrimitivePolyline(VirtualStreamReader *reader, int length) : DrawingPrimitiveLine(reader, length, false)
	{
		strVmlType	= L"v:shape";

		read_line(reader);
		read_fill(reader);
		read_epp(reader);	
		read_shadow(reader);

		unsigned short f = reader->ReadUInt16();

		fPolygon	= FormatUtils::BitmaskToInt (f, 0x0001);	
		count		= FormatUtils::BitmaskToInt (f, 0x00fe);	

		read_start_end(reader);

		for (int i = 0; i < count; i++)
		{
			std::pair<short, short> point;

			point.first		= reader->ReadInt16();
			point.second	= reader->ReadInt16();
			
			arPoints.push_back(point);
		}
	}
}
