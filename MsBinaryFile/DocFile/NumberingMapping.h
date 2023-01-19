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
