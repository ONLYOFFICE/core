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

#ifndef STYLECONVERTER_H
#define STYLECONVERTER_H

#include "../HWPElements/HWPRecordParaShape.h"
#include "../HWPElements/HWPRecordCharShape.h"

#include "ConversionState.h"
#include "Types.h"

#include <unordered_map>

namespace HWP
{
enum class EJs
{
	Left,
	Center,
	Right,
	Both,
	Distribute
};

enum class ETextAlignment
{
	Top,
	Center,
	Bottom
};

enum class ELineRule
{
	Auto,
	Exact,
	AtLeast
};

template <class T>
class CProperty
{
public:
	CProperty();

	void UnSet();

	void SetValue(T oValue);
	bool IsSet() const;

	T GetValue() const;

	CProperty<T>& operator=(T oValue);
	CProperty<T>& operator-=(const CProperty<T>& oProperty);
	operator T() const;
private:
	bool m_bIsSet;
	T m_oValue;
};

struct TLineSpacing
{
	CProperty<ELineRule> m_eLineRule;
	CProperty<int> m_nLine;
	CProperty<int> m_nBefore;
	CProperty<int> m_nAfter;

	TLineSpacing& operator-=(const TLineSpacing& oLineSpacing);
};

struct TInd
{
	CProperty<int> m_nFirstLine;
	CProperty<int> m_nLeft;
	CProperty<int> m_nRight;

	TInd& operator-=(const TInd& oInd);
};

#define CREATE_METHODS_FOR_PROPERTY(type, name)\
	void Set##name(type oValue);\
	bool name##IsSet() const;\
	type Get##name() const

#define CREATE_METHODS_FOR_PROPERTY_BOOL(name)\
	void Set##name(bool oValue);\
	bool name##IsSet() const;\
	bool name() const

class CParagraphsStyle
{
public:
	CParagraphsStyle();
	CParagraphsStyle& operator-=(const CParagraphsStyle& oParagraphStyle);

	void Clear();
	bool Empty() const;

	CREATE_METHODS_FOR_PROPERTY_BOOL(KeepNext);
	CREATE_METHODS_FOR_PROPERTY(int, Ind);
	CREATE_METHODS_FOR_PROPERTY(EJs, Js);
	CREATE_METHODS_FOR_PROPERTY(ETextAlignment, TextAlignment);
	CREATE_METHODS_FOR_PROPERTY(ELineRule, SpacingLineRule);
	CREATE_METHODS_FOR_PROPERTY(int, FirstLine);
	CREATE_METHODS_FOR_PROPERTY(int, LeftInd);
	CREATE_METHODS_FOR_PROPERTY(int, RightInd);
	CREATE_METHODS_FOR_PROPERTY(int, Spacing);
	CREATE_METHODS_FOR_PROPERTY(int, SpacingBefore);
	CREATE_METHODS_FOR_PROPERTY(int, SpacingAfter);
private:
	CProperty<bool> m_bKeepNext;
	TInd m_oInd;
	CProperty<EJs> m_eJs;
	CProperty<ETextAlignment> m_eTextAlignment;
	TLineSpacing    m_oSpacing;
};

struct TRFonts
{
	CProperty<std::wstring> m_wsAscii;
	CProperty<std::wstring> m_wsEastAsia;

	TRFonts& operator-=(const TRFonts& oRFonts);
};

enum class EUType
{
	Single,
	Dash,
	Dotted,
	DotDash,
	DotDotDash,
	Double,
	Wave,
	WavyDouble,
	Thick
};

struct TU
{
	EUType m_eVal;
	TColor m_oColor;

	bool operator!=(const TU& oU) const;
};

enum class EStrikeType
{
	Single,
	Double
};

enum class EHighlightColors
{
	Black,
	Blue,
	Cyan,
	Green,
	Magenta,
	Red,
	Yellow,
	White,
	DarkBlue,
	DarkCyan,
	DarkGreen,
	DarkMagenta,
	DarkRed,
	DarkYellow,
	DarkGray,
	LightGray
};

enum class EVerticalAlignRun
{
	Baseline,
	Superscript,
	Subscript
};

struct CRunnerStyle
{
public:
	CRunnerStyle();
	CRunnerStyle& operator-=(const CRunnerStyle& oRunnerStyle);

	void Clear();
	bool Empty() const;

	CREATE_METHODS_FOR_PROPERTY(std::wstring, Ascii);
	CREATE_METHODS_FOR_PROPERTY(std::wstring, EastAsia);
	CREATE_METHODS_FOR_PROPERTY_BOOL(Bold);
	CREATE_METHODS_FOR_PROPERTY_BOOL(Italic);
	CREATE_METHODS_FOR_PROPERTY(int, Sz);
	CREATE_METHODS_FOR_PROPERTY(TColor, Color);
	CREATE_METHODS_FOR_PROPERTY(int, ShadeColor);
	CREATE_METHODS_FOR_PROPERTY(short, Ratio);
	CREATE_METHODS_FOR_PROPERTY(TU, U);
	CREATE_METHODS_FOR_PROPERTY(EStrikeType, Strike);
	CREATE_METHODS_FOR_PROPERTY(short, Spacing);
	CREATE_METHODS_FOR_PROPERTY(EHighlightColors, Highlight);
	CREATE_METHODS_FOR_PROPERTY(EVerticalAlignRun, VerticalAlign);
private:
	TRFonts m_oRFonts;
	CProperty<bool> m_bBold;
	CProperty<bool> m_bItalic;
	CProperty<int> m_nSz;
	CProperty<TColor> m_oColor;
	CProperty<int> m_nShadeColor;
	CProperty<short> m_shRatio;
	CProperty<TU> m_oU;
	CProperty<EStrikeType> m_eStrike;
	CProperty<short> m_shSpacing;
	CProperty<EHighlightColors> m_oHighlight;
	CProperty<EVerticalAlignRun> m_eVerticalAlign;
};

class CWriterContext;
class CStyleConverter
{
	std::unordered_map<std::wstring, int> m_mUsedStyleNames;
	std::map<unsigned short, std::wstring> m_mUsedStyles;

	unsigned short m_ushLastParaShapeId;
	unsigned short m_ushLastCharShapeId;

	NSStringUtils::CStringBuilder m_oStylesXml;

	std::wstring GenerateUniqueID(const std::wstring& wsName);

	static CParagraphsStyle GenerateParagraphStyle(const CHWPRecordParaShape& oParaShape);
	static CRunnerStyle GenerateRunnerStyle(const CHWPRecordCharShape& oCharShape);

	static void WriteParagraphsStyle(const CParagraphsStyle& oParagraphsStyle, const std::wstring& wsName, const std::wstring& wsLink, NSStringUtils::CStringBuilder& oBuilder);

	static void WriteRunnerStyle(const CRunnerStyle& oRunnerStyle, const std::wstring& wsName, const std::wstring& wsLink, NSStringUtils::CStringBuilder& oBuilder);
public:
	CStyleConverter();

	std::wstring CreateStyle(short shParaShapeId, short shParaStyleId, CWriterContext& oContext, TConversionState& oState);

	static void WriteParagraphProperties(const CParagraphsStyle& oParagraphsStyle, NSStringUtils::CStringBuilder& oBuilder);
	static void WriteRunnerProperties(const CRunnerStyle& oRunnerStyle, NSStringUtils::CStringBuilder& oBuilder);

	static bool WriteDifferenceParagraphStyles(short shFirtsParaShapeId, short shSecondParaShapeId, CWriterContext& oContext, NSStringUtils::CStringBuilder& oBuilder);
	static bool WriteDifferenceRunnerStyles(short shFirtsCharShapeId, short shSecondCharShapeId, CWriterContext& oContext, NSStringUtils::CStringBuilder& oBuilder);

	unsigned short GetLastParaShapeId() const;
	unsigned short GetLastCharShapeId() const;

	bool SaveToFile(const std::wstring& wsDirectory);
};

EHighlightColors NormalizeHighlightColor(const TColor& oCurrentColor);
}

#endif // STYLECONVERTER_H
