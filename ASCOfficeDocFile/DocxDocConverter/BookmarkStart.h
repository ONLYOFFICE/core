#pragma once

#include "IParagraphItem.h"

namespace ASCDocFileFormat
{
	class BookmarkStart : public IParagraphItem
	{
	public:
		BookmarkStart()
		{
		}

		explicit BookmarkStart (std::wstring& _id, std::wstring& _name) : id(_id), name(_name)
		{
		}

		virtual ~BookmarkStart()
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

		virtual vector<Chpx> GetRunProperties( vector<unsigned int>* runOffsets ) const
		{
			return vector<Chpx>();
		}

		virtual unsigned int PrlSize () const
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
			return new BookmarkStart();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new BookmarkStart(*this);
		}
		//	
		inline std::wstring GetID() const
		{
			return id;
		}

		inline std::wstring GetName() const
		{
			return name;
		}

	
	private:
	
		std::wstring id;
		std::wstring name;
	};
}