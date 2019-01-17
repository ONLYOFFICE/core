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

#include "ITextItem.h"
#include "../Common/TextMark.h"

namespace ASCDocFileFormat
{
	class SectionBreak: public ITextItem
	{
	private:
		static const wchar_t sectionBreakText = TextMark::PageBreakOrSectionMark;

	public:
		SectionBreak()
		{
		}

		SectionBreak( const SectionBreak& _sectionBreak )
		{
		}

		virtual ~SectionBreak()
		{
		}

		virtual wstring GetAllText() const
		{
			wstring allText;

			allText.push_back( sectionBreakText );

			return allText;
		}

		virtual operator wstring() const
		{
			wstring allText;

			allText.push_back( sectionBreakText );

			return allText;  
		}

		virtual vector<TextItemPtr> GetAllParagraphsCopy() const
		{
			vector<TextItemPtr> allParagraphsCopy;

			allParagraphsCopy.push_back( TextItemPtr( reinterpret_cast<ITextItem*>( NULL ) ) );

			return allParagraphsCopy;
		}

		virtual vector<ITextItem*> GetAllParagraphs()
		{
			vector<ITextItem*> allParagraphs;

			allParagraphs.push_back( NULL );

			return allParagraphs;   
		}

		virtual vector<PapxInFkp> GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const
		{
			vector<PapxInFkp> allParagraphsProperties;

			if ( allParagraphsOffsets != NULL )
			{
				allParagraphsProperties.push_back( PapxInFkp() );
				allParagraphsOffsets->push_back( 0 );
			}

			return allParagraphsProperties;
		}

		virtual vector<Chpx> GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const
		{
			vector<Chpx> AllChpx;

			if ( allRunsOffsets != NULL )
			{
				AllChpx.push_back( Chpx() );

				allRunsOffsets->push_back( 0 );
			}

			return AllChpx;
		}

		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const
		{
			vector<IParagraphItemPtr> allRuns;

			if ( allRunsOffsets != NULL )
			{
				wstring text;

				text.push_back( sectionBreakText );

				allRuns.push_back( IParagraphItemPtr( dynamic_cast<IParagraphItem*>(Run( Text( text.c_str() ) ).Clone()) ) );
				allRunsOffsets->push_back( 0 );
			}

			return allRuns;
		}

		virtual vector<IParagraphItemPtr> GetAllParagraphItemsCopy (vector<unsigned int>* allParagraphItemsOffsets) const
		{
			vector<IParagraphItemPtr> allParagraphItems;

			if ( allParagraphItemsOffsets != NULL )
			{
				wstring text;

				text.push_back( sectionBreakText );

				allParagraphItems.push_back( IParagraphItemPtr( dynamic_cast<IParagraphItem*>(Run( Text( text.c_str() ) ).Clone()) ) );
				allParagraphItemsOffsets->push_back( 0 );
			}

			return allParagraphItems;
		}

		virtual IVirtualConstructor* New() const
		{
			return new SectionBreak();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new SectionBreak( *this );
		}
	};
}
