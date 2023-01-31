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

					// .... если будут документы с такими цветовыми палитрами

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
