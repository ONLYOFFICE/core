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
#ifndef PPTX_LOGIC_COLORBASE_INCLUDE_H_
#define PPTX_LOGIC_COLORBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "ColorModifier.h"

namespace PPTX
{
	const double cd16 = 1.0/6.0;
	const double cd13 = 1.0/3.0;
	const double cd23 = 2.0/3.0;

	namespace Logic
	{
		class ClrMap;
		class ColorBase : public WrapperWritingElement
		{
		public:
			ColorBase();
			ColorBase(const ColorBase& oSrc);
			ColorBase& operator=(const ColorBase& oSrc);
			virtual ~ColorBase();

			//These functions return color with modifiers applied
			virtual DWORD GetRGBA(DWORD RGBA) const;
			virtual DWORD GetARGB(DWORD ARGB) const;
			virtual DWORD GetBGRA(DWORD BGRA) const;
			virtual DWORD GetABGR(DWORD ABGR) const;

			virtual DWORD GetRGBColor(NSCommon::smart_ptr<PPTX::Theme>& _oTheme, NSCommon::smart_ptr<PPTX::Logic::ClrMap>& _oClrMap, DWORD ARGB = 0);
			void SetRGB(const BYTE& R, const BYTE& G, const BYTE& B);

			//Need to do similar for "origin color" and setters
			//No, no and no!!! Setters only in UniColor, because when setting color its type changes!

			enum ColorType {ctRGBA, ctARGB, ctBGRA, ctABGR};

			virtual DWORD ApplyModifiers(const ColorType ct) const;

			//Use these functions for filling "origin color"
			void SetRGB2HSL();
			void SetHSL2RGB();

			//Use these when applying modifiers
			static void RGB2HSL(unsigned char* RGB, unsigned char* HSL);
			static void HSL2RGB(unsigned char* HSL, unsigned char* RGB);
			static double Hue_2_RGB(double v1,double v2,double vH);             //Function Hue_2_RGB

			const int HexChar2Int(const char value);
			const int HexString2Int(const std::wstring& value);

			void SetHexString(const std::wstring& val);
			void ReadModsFromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			//origin color
			unsigned char alpha;
			unsigned char red;
			unsigned char green;
			unsigned char blue;
			unsigned char hue;
			unsigned char saturation;
			unsigned char luminance;

			std::vector<ColorModifier> Modifiers;

			friend class UniColor;
			std::wstring m_namespace;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_COLORBASE_INCLUDE_H
