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

			//Эти функции возвращают цвет с учетом модификаторов
			virtual DWORD GetRGBA(DWORD RGBA) const;
			virtual DWORD GetARGB(DWORD ARGB) const;
			virtual DWORD GetBGRA(DWORD BGRA) const;
			virtual DWORD GetABGR(DWORD ABGR) const;

			virtual DWORD GetRGBColor(NSCommon::smart_ptr<PPTX::Theme>& _oTheme, NSCommon::smart_ptr<PPTX::Logic::ClrMap>& _oClrMap, DWORD ARGB = 0);
			void SetRGB(const BYTE& R, const BYTE& G, const BYTE& B);

			//Надо сделать примерно также для "origin color" и setter'ы
			//Нет, нет и нет!!! Setter'ы делать только в УниКолор, т.к. при установке цвета меняется его тип!

			enum ColorType {ctRGBA, ctARGB, ctBGRA, ctABGR};

			virtual DWORD ApplyModifiers(const ColorType ct) const;

			//Эти функции использовать для заполнения "origin color"
			void SetRGB2HSL();
			void SetHSL2RGB();

			//Эти использовать при применении модификаторов
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
