#pragma once

#include "Run.h"
#include "ParagraphItem.h"
#include "CP.h"

namespace AVSDocFileFormat
{
	class Hyperlink : public IParagraphItem
	{
	public:
		typedef list<ParagraphItem>::const_iterator const_iterator;
		typedef list<ParagraphItem>::iterator iterator;
		typedef list<ParagraphItem>::const_reverse_iterator const_reverse_iterator;
		typedef list<ParagraphItem>::reverse_iterator reverse_iterator;
		typedef list<ParagraphItem>::const_reference const_reference;
		typedef list<ParagraphItem>::reference reference;

	public:
		Hyperlink();
		explicit Hyperlink( const vector<Run>& _runs );
	protected:
		void BuildUrlRuns() const;

	public:

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
		void AddRun( const Run& _run );
		void SetURL( const WCHAR* _url );
		wstring GetURL() const;
		void SetLocationInTheFile( const WCHAR* _locationInTheFile );
		wstring GetLocationInTheFile() const;
		wstring GetHyperlinkText() const;
		virtual ~Hyperlink();
		virtual unsigned long GetTextSize() const;
		virtual std::wstring GetAllText() const;
		virtual vector<Chpx> GetRunProperties( vector<unsigned int>* runOffsets ) const;
		virtual unsigned int PrlSize() const;
		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* runOffsets ) const;
		virtual IVirtualConstructor* New() const;
		virtual IVirtualConstructor* Clone() const;
		vector<CP> GetFieldCharactersPositions() const;
		vector<Fld> GetFieldCharactersProperties() const;

	private:
		static const byte CFSpec = 1;

		std::list<ParagraphItem> textRuns;
		mutable std::vector<ParagraphItem> specialRuns;
		std::wstring url;
		std::wstring locationInTheFile;
	};
}