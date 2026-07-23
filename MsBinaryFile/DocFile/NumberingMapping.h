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

#include "WordDocument.h"
#include "ListTable.h"
#include "StyleSheetMapping.h"
#include "ParagraphPropertiesMapping.h"
#include "CharacterPropertiesMapping.h"
#include "RevisionData.h"
#include "AbstractOpenXmlMapping.h"
#include "ConversionContext.h"
#include "IMapping.h"

namespace DocFileFormat
{
	//typedef enum _LevelJustification
	//{
	//	left = 0,
	//	center,
	//	right
	//} LevelJustification;

	static const wchar_t LevelJustificationMap[3][7] =
	{
        L"left",
        L"center",
        L"right"
	};

	class NumberingMapping: public AbstractOpenXmlMapping, public IMapping
	{
	public: 
		NumberingMapping(ConversionContext* ctx);
		void Apply(IVisitable* visited);
		
		static int FindIndexbyId(std::vector<ListDataPtr>& listData, int id);
		/// Converts the number format code of the binary format.
		static std::wstring GetNumberFormatWideString(int nfc, int nWordVersion = 0);
		
		virtual ~NumberingMapping();

	private:
		// Converts the number text of the binary format to the number text of OOXML.
		// OOXML uses different placeholders for the numbers.
		std::wstring GetLvlText(const ListLevel* lvl, bool bIsSymbol) const;
		std::wstring GetLvlText(NumberingDescriptorPtr& lvl, bool bIsSymbol, int Before, int After) const;
		
		static bool IsPlaceholder(wchar_t symbol);
		
		void LevelMapping(const ListLevel* lvl, unsigned int level, short styleIndex);
		void LevelMapping(NumberingDescriptorPtr & lvl, unsigned int level);

		void PictureBulletsMapping();
		void WriteLevelPictureBullet(const CharacterPropertyExceptions* grpprlChpx);
		bool IsPictureBullet(const CharacterPropertyExceptions* grpprlChpx);

	private:	
		ConversionContext*		m_context;
		WordDocument*			m_document;
		WordprocessingDocument*	m_xmldocument;	
	};
}
