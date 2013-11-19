#pragma once

#include "IParagraphItem.h"

namespace AVSDocFileFormat
{
	class BookmarkEnd: public IParagraphItem
	{

	public:
		BookmarkEnd()
		{
		}

		explicit BookmarkEnd (std::wstring& _id) : id (_id)
		{
		}


		virtual ~BookmarkEnd()
		{
		}

		virtual unsigned long GetTextSize() const
		{
			return 0;
		}

		virtual std::wstring GetAllText() const
		{
			return std::wstring();
		}

		virtual vector<Chpx> GetRunProperties (vector<unsigned int>* runOffsets) const
		{
			return vector<Chpx>();
		}

		virtual unsigned int PrlSize() const
		{
			return 0;
		}

		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* runOffsets ) const
		{
			vector<IParagraphItemPtr> paragraphItems;

			if ( runOffsets != NULL )
			{
				paragraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(this->Clone()) ) );
				runOffsets->push_back( 0 );
			}

			return paragraphItems;
		}

		virtual IVirtualConstructor* New() const
		{
			return new BookmarkEnd();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new BookmarkEnd(*this);
		}
		
		//
		
		inline std::wstring GetID() const
		{
			return id;
		}
	
	private:
	
		std::wstring id;
	};
}