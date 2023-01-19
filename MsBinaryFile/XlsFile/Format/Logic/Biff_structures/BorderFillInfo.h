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

#include <sstream>
#include "../../../../../OOXML/Base/Base.h"

namespace XLS
{

struct FillInfoExt
{
	bool			enabled =false;

	unsigned char	xclrType = 0;
	unsigned char	icv = 0;
	short			nTintShade = 0;
	_UINT32			xclrValue = 0;
	
	bool operator == (const FillInfoExt & rVal) const;
};

struct FillInfo
{
	FillInfo(){}

	FillInfo(const unsigned char fls_set, const unsigned char icvFore_set, const unsigned char icvBack_set)
		: fls(fls_set), icvFore(icvFore_set), icvBack(icvBack_set)
	{	
	}

	unsigned char fls = 0;
	unsigned char icvFore = 0;
	unsigned char icvBack = 0;

	bool operator < (const FillInfo& right) const
	{
		return  fls < right.fls || fls == right.fls && 
				(icvFore < right.icvFore || icvFore == right.icvFore &&
				(icvBack < right.icvBack));
	}

	bool operator == (const FillInfo & rVal) const;
	bool operator != (const FillInfo & rVal) const;

	friend std::size_t hash_value(FillInfo const & val);

	int serialize(std::wostream & stream);

//additional (сравнение по базовому, расширения тока пишем)

	FillInfoExt foreFillInfo_;
	FillInfoExt backFillInfo_;

	std::wstring ext;
};

struct BorderInfo
{
	unsigned char dgLeft = 0;
	unsigned char dgRight = 0;
	unsigned char dgTop = 0;
	unsigned char dgBottom = 0;
	unsigned char dgDiag = 0;
	unsigned char grbitDiag = 0;
	unsigned char icvLeft = 0;
	unsigned char icvRight = 0;
	unsigned char icvTop = 0;
	unsigned char icvBottom = 0;
	unsigned char icvDiag = 0;

	FillInfoExt leftFillInfo_;
	FillInfoExt rightFillInfo_;
	FillInfoExt topFillInfo_;
	FillInfoExt bottomFillInfo_;

	BorderInfo() : dgLeft(0), dgRight(0), dgTop(0), dgBottom(0),  dgDiag(0), grbitDiag(0) {}

	BorderInfo(	const unsigned char dgLeft_set, const unsigned char dgRight_set, const unsigned char dgTop_set, 
			const unsigned char dgBottom_set, const unsigned char dgDiag_set, const unsigned char grbitDiag_set, 
			const unsigned char icvLeft_set, const unsigned char icvRight_set, const unsigned char icvTop_set, 
			const unsigned char icvBottom_set, const unsigned char icvDiag_set	)

		:	dgLeft(dgLeft_set), dgRight(dgRight_set), dgTop(dgTop_set), dgBottom(dgBottom_set), 
			dgDiag(dgDiag_set), grbitDiag(grbitDiag_set),
			icvLeft(icvLeft_set), icvRight(icvRight_set), icvTop(icvTop_set), icvBottom(icvBottom_set),
			icvDiag(icvDiag_set)
	{	
	}
	bool operator < (const BorderInfo& right) const;

	bool operator == (const BorderInfo & rVal) const;
	bool operator != (const BorderInfo & rVal) const;
	
	friend std::size_t hash_value(BorderInfo const & val);

	int serialize(std::wostream & stream);
};

struct FontInfo
{
	unsigned short dyHeight = 0; // measured in twips (1/20 of of a printer's point)

	bool fItalic	= false;
	bool fStrikeOut	= false;
	bool fOutline	= false;
	bool fShadow	= false;
	bool fCondense	= false;
	bool fExtend	= false;

	unsigned short icv = 0;
	unsigned short bls = 0;
	unsigned short sss = 0;
	unsigned short scheme = 0;
	unsigned char	uls;
	unsigned char	bFamily = 0;
	unsigned char	bCharSet = 0;

	FillInfoExt		color;
	std::wstring	name;

	bool operator == (const FontInfo & rVal) const;
	bool operator != (const FontInfo & rVal) const;

	friend std::size_t hash_value(FontInfo const & val);

	int serialize(std::wostream & stream);
};


} // namespace XLS

