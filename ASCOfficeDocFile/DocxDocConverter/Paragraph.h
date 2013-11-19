#pragma once

#include "ParagraphItem.h"
#include "TextItem.h"

namespace AVSDocFileFormat
{
	class Paragraph : public ITextItem
	{
		friend class TableCell;

	private:
		list<ParagraphItem> paragraphItems;
		unsigned long runsOffset;
		short style;
		list<Prl> properties;

		unsigned int depth;

	protected:
		void SetDepth( unsigned int _depth );
		unsigned int GetDepth() const;

	public:
		typedef list<ParagraphItem>::const_iterator const_iterator;
		typedef list<ParagraphItem>::iterator iterator;
		typedef list<ParagraphItem>::const_reverse_iterator const_reverse_iterator;
		typedef list<ParagraphItem>::reverse_iterator reverse_iterator;
		typedef list<ParagraphItem>::const_reference const_reference;
		typedef list<ParagraphItem>::reference reference;

		const_iterator begin() const;
		const_iterator end() const;
		iterator begin();
		iterator end();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;
		reverse_iterator rbegin();
		reverse_iterator rend();
		const_reference front() const;
		const_reference back() const;
		reference front();
		reference back();

	public:
		Paragraph( short _style = 0 );
		void AddParagraphItem( const IParagraphItem& _paragraphItem );
		unsigned long GetTextSize() const;
		virtual ~Paragraph();
		virtual wstring GetAllText() const;
		virtual operator wstring() const;
		virtual vector<Chpx> GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const;
		virtual vector<PapxInFkp> GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const;
		virtual vector<TextItemPtr> GetAllParagraphsCopy() const;
		virtual vector<ITextItem*> GetAllParagraphs();
		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const;
		virtual vector<IParagraphItemPtr> GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const;
		virtual IVirtualConstructor* New() const;
		virtual IVirtualConstructor* Clone() const;
		void AddProperty( short sprm, void* operand );
		void AddProperty( const Prl& prl );
		void AddProperties( const vector<Prl>& prls );
		void AddOrReplaceProperties( const vector<Prl>& prls );
		void RemoveProperty( short sprm, void* operand );
		void RemovePropertyByCode( unsigned short sprm );
		void RemoveAllProperties();
		void SetStyle( short _style );
		short GetStyle() const;
		PapxInFkp GetProperties() const;
		unsigned int GetParagraphPropertiesCount() const;
	};
}