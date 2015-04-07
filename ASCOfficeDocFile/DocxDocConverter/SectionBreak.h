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
