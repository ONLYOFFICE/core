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

#include "IVisitable.h"

namespace DocFileFormat
{
	typedef enum _ShadingPattern
	{
		Automatic = 0x0000,
		Solid = 0x0001,
		Percent_5 = 0x0002,
		Percent_10 = 0x0003,
		Percent_20 = 0x0004,
		Percent_25 = 0x0005,
		Percent_30 = 0x0006,
		Percent_40 = 0x0007,
		Percent_50 = 0x0008,
		Percent_60 = 0x0009,
		Percent_70 = 0x000A,
		Percent_75 = 0x000B,
		Percent_80 = 0x000C,
		Percent_90 = 0x000D,
		DarkHorizontal = 0x000E,
		DarkVertical = 0x000F,
		DarkForwardDiagonal = 0x0010,
		DarkBackwardDiagonal = 0x0011,
		DarkCross = 0x0012,
		DarkDiagonalCross = 0x0013,
		Horizontal = 0x0014,
		Vertical = 0x0015,
		ForwardDiagonal = 0x0016,
		BackwardDiagonal = 0x0017,
		Cross = 0x0018,
		DiagonalCross = 0x0019,
		Percent_2_5 = 0x0023,
		Percent_7_5 = 0x0024,
		Percent_12_5 = 0x0025,
		Percent_15 = 0x0026,
		Percent_17_5 = 0x0027,
		Percent_22_5 = 0x0028,
		Percent_27_5 = 0x0029,
		Percent_32_5 = 0x002A,
		Percent_35 = 0x002B,
		Percent_37_5 = 0x002C,
		Percent_42_5 = 0x002D,
		Percent_45 = 0x002E,
		Percent_47_5 = 0x002F,
		Percent_52_5 = 0x0030,
		Percent_55 = 0x0031,
		Percent_57_5 = 0x0032,
		Percent_62_5 = 0x0033,
		Percent_65 = 0x0034,
		Percent_67_5 = 0x0035,
		Percent_72_5 = 0x0036,
		Percent_77_5 = 0x0037,
		Percent_82_5 = 0x0038,
		Percent_85 = 0x0039,
		Percent_87_5 = 0x003A,
		Percent_92_5 = 0x003B,
		Percent_95 = 0x003C,
		Percent_97_5 = 0x003D,
		Nil = 0xFFFF
	} ShadingPattern;

	enum ShadingType
	{
		shadingTypeShd,
		shadingTypeShd80
	};

	enum ShadingSpecialValue
	{
		shadingSpecialValueNormal,
		shadingSpecialValueShdAuto,
		shadingSpecialValueShdNil,
		shadingSpecialValueShd80Nil
	};

	class ShadingDescriptor: public IVisitable
	{
		friend class PropertiesMapping;

	private:   
		unsigned int		cvFore;			// 24-bit foreground color
		bool				cvForeAuto;
		int					icoFore;		// Foreground color.
		unsigned int		cvBack;
		bool				cvBackAuto;
		int					icoBack;
		ShadingPattern		ipat;
		ShadingType			shadingType;
		ShadingSpecialValue shadingSpecialValue;

	public:
		/// Creates a new ShadingDescriptor with default values
		ShadingDescriptor();

		/// Parses the bytes to retrieve a ShadingDescriptor.
		ShadingDescriptor(unsigned char* bytes, int size);

	private:
		inline unsigned int RGB2 (unsigned char r, unsigned char g, unsigned char b)
		{
			return ( (r<<16) | (g<<8) | (b) );
		}

		void setDefaultValues();
	};
}
