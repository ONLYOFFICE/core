#pragma once

#include "IRunItem.h"

namespace AVSDocFileFormat
{
	class Text: public IRunItem
	{
	public:

		Text ();
		Text (const wchar_t& wChar);
		Text (const std::wstring& _text);
		Text (const Text& _text);
		virtual ~Text();
		bool operator == ( const Text& _text );
		bool operator != ( const Text& _text );
		Text& operator = ( const Text& _text );
		Text& operator += ( const Text& _text );
		const WCHAR operator [] ( unsigned int index ) const;
		virtual unsigned long GetTextSize() const;
		virtual wstring GetAllText() const;
		virtual vector<Prl> GetRunProperties() const;
		virtual unsigned int PrlSize() const;
		virtual IVirtualConstructor* New() const;
		virtual IVirtualConstructor* Clone() const;
		void SetText( const WCHAR* _text );
		void SetString (const std::wstring& source);
		void AddProperty( short sprm, void* operand );
		void AddProperty( const Prl& prl );
		void AddProperties( const vector<Prl>& prls );
		void RemoveProperty( short sprm, void* operand );
		void RemoveAllProperties();
	
	protected:
		
		std::wstring text;
		std::list<Prl> properties;
	};
}