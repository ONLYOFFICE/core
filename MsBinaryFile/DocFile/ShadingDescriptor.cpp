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

#include "ShadingDescriptor.h"
#include "../Common/Base/FormatUtils.h"

namespace DocFileFormat
{		
	/// Creates a new ShadingDescriptor with default values
	ShadingDescriptor::ShadingDescriptor()
	{
		setDefaultValues();
	}

	/// Parses the bytes to retrieve a ShadingDescriptor.
	ShadingDescriptor::ShadingDescriptor(unsigned char* bytes, int size)
	{
		setDefaultValues();

		if (NULL != bytes)
		{
			if (10 == size)
			{
				//it's a Word 2000/2003 descriptor
				unsigned char cvForeBytes[4] = { bytes[2], bytes[1], bytes[0], 0 };
				unsigned char cvBackBytes[4] = { bytes[6], bytes[5], bytes[4], 0 };

				cvFore = FormatUtils::BytesToUInt32( cvForeBytes, 0, 4 );

				if ( bytes[3] == 0xFF )
				{
					cvForeAuto = true;
				}

				cvBack = FormatUtils::BytesToUInt32( cvBackBytes, 0, 4 );

				if ( bytes[7] == 0xFF )
				{
					cvBackAuto = true;
				}

				ipat = (ShadingPattern)FormatUtils::BytesToUInt16( bytes, 8, size );

				shadingType = shadingTypeShd;

				if ( ( cvFore == 0 ) && ( bytes[3] == 0xFF ) && ( cvBack == 0 ) && ( bytes[7] == 0xFF ) && ( ipat == Automatic ) )
				{
					shadingSpecialValue = shadingSpecialValueShdAuto;
				}
				else if ( ( cvFore == 0xFFFFFF ) && ( bytes[3] == 0xFF ) && ( cvBack == 0xFFFFFF ) && ( bytes[7] == 0xFF ) &&	( ipat == Automatic ) )
				{
					shadingSpecialValue = shadingSpecialValueShdNil;
				}
			}
			else if (2 == size)
			{
				//it's a Word 97 SPRM
				short val	=	FormatUtils::BytesToInt16(bytes, 0, size);

				icoFore		=	GETBITS(val, 0, 4);
				icoBack		=	GETBITS(val, 5, 9);
				ipat		=	(ShadingPattern) GETBITS(val, 10, 15);

				shadingType	=	shadingTypeShd80;

				if ((icoFore == 0x1F) && (icoBack == 0x1F) && (ipat == 0x3F))
				{
					shadingSpecialValue	= shadingSpecialValueShd80Nil;
				}
				else
				{
					shadingType	=	shadingTypeShd;

					cvForeAuto	=	false;
					cvBackAuto	=	false;

					if (0x00 == icoFore)	{ cvFore = RGB2 (0x00, 0x00, 0x00); cvForeAuto = true; }
					else if (0x01 == icoFore) { cvFore = RGB2 (0x00, 0x00, 0x00); }
					else if (0x02 == icoFore) { cvFore = RGB2 (0x00, 0x00, 0xFF); }
					else if (0x03 == icoFore) { cvFore = RGB2 (0x00, 0xFF, 0xFF); }
					else if (0x04 == icoFore) { cvFore = RGB2 (0x00, 0xFF, 0x00); }
					else if (0x05 == icoFore) { cvFore = RGB2 (0xFF, 0x00, 0xFF); }
					else if (0x06 == icoFore) { cvFore = RGB2 (0xFF, 0x00, 0x00); }
					else if (0x07 == icoFore) { cvFore = RGB2 (0xFF, 0xFF, 0x00); }
					else if (0x08 == icoFore) { cvFore = RGB2 (0xFF, 0xFF, 0xFF); }
					else if (0x09 == icoFore) { cvFore = RGB2 (0x00, 0x00, 0x80); }
					else if (0x0A == icoFore) { cvFore = RGB2 (0x00, 0x80, 0x80); }
					else if (0x0B == icoFore) { cvFore = RGB2 (0x00, 0x80, 0x00); }
					else if (0x0C == icoFore) { cvFore = RGB2 (0x80, 0x00, 0x80); }
					else if (0x0D == icoFore) { cvFore = RGB2 (0x80, 0x00, 0x80); }
					else if (0x0E == icoFore) { cvFore = RGB2 (0x80, 0x80, 0x00); }
					else if (0x0F == icoFore) { cvFore = RGB2 (0x80, 0x80, 0x80); }
					else if (0x10 == icoFore) { cvFore = RGB2 (0xC0, 0xC0, 0xC0); }

					if (0x00 == icoBack)	{ cvBack = RGB2 (0xFF, 0xFF, 0xFF); cvBackAuto = true; }
					else if (0x01 == icoBack) { cvBack = RGB2 (0x00, 0x00, 0x00); }
					else if (0x02 == icoBack) { cvBack = RGB2 (0x00, 0x00, 0xFF); }
					else if (0x03 == icoBack) { cvBack = RGB2 (0x00, 0xFF, 0xFF); }
					else if (0x04 == icoBack) { cvBack = RGB2 (0x00, 0xFF, 0x00); }
					else if (0x05 == icoBack) { cvBack = RGB2 (0xFF, 0x00, 0xFF); }
					else if (0x06 == icoBack) { cvBack = RGB2 (0xFF, 0x00, 0x00); }
					else if (0x07 == icoBack) { cvBack = RGB2 (0xFF, 0xFF, 0x00); }
					else if (0x08 == icoBack) { cvBack = RGB2 (0xFF, 0xFF, 0xFF); }
					else if (0x09 == icoBack) { cvBack = RGB2 (0x00, 0x00, 0x80); }
					else if (0x0A == icoBack) { cvBack = RGB2 (0x00, 0x80, 0x80); }
					else if (0x0B == icoBack) { cvBack = RGB2 (0x00, 0x80, 0x00); }
					else if (0x0C == icoBack) { cvBack = RGB2 (0x80, 0x00, 0x80); }
					else if (0x0D == icoBack) { cvBack = RGB2 (0x80, 0x00, 0x80); }
					else if (0x0E == icoBack) { cvBack = RGB2 (0x80, 0x80, 0x00); }
					else if (0x0F == icoBack) { cvBack = RGB2 (0x80, 0x80, 0x80); }
					else if (0x10 == icoBack) { cvBack = RGB2 (0xC0, 0xC0, 0xC0); }

					// .... if there are documents with such color palettes

					//if ((cvFore == 0) && (icoFore == 0x0) && (cvBack == 0) && (icoBack == 0x0) && (ipat == Automatic))
					//{
					//	shadingSpecialValue = shadingSpecialValueShdAuto;
					//}
					//else if ((cvFore == 0xFFFFFF) && (icoFore == 0x0) && (cvBack == 0xFFFFFF) && (icoBack == 0x0) && (ipat == Automatic))
					//{
					//	shadingSpecialValue = shadingSpecialValueShdNil;
					//}
				}
			}
			else
			{
			}
		}
	}	

	void ShadingDescriptor::setDefaultValues()
	{
		cvBack				=	0;
		cvBackAuto			=	false;
		cvFore				=	0;
		cvForeAuto			=	false;
		icoBack				=	0;
		icoFore				=	0;
		ipat				=	Automatic;

		shadingType			=	shadingTypeShd80;
		shadingSpecialValue =	shadingSpecialValueNormal;
	}
}
