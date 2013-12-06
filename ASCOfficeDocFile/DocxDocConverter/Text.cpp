#include "stdafx.h"
#include "Text.h"

namespace ASCDocFileFormat
{
	Text::Text()
	{

	}

	Text::Text (const wchar_t& wChar)
	{
		text.clear();
		text.push_back (wChar);
	}

	Text::Text (const std::wstring& _text )
	{
		text = _text;
	}

	Text::Text (const Text& _text) : text(_text.text), properties(_text.properties)
	{

	}

	Text::~Text()
	{

	}

	bool Text::operator == ( const Text& _text )
	{
		if ( text != _text.text )
		{
			return false;
		}
		else if (properties.size() != _text.properties.size())
		{
			return false;
		}
		else
		{
			list<Prl>::iterator thisIter		=	properties.begin();
			list<Prl>::const_iterator textIter	=	_text.properties.begin();

			for ( ; thisIter != properties.end(); thisIter++, textIter++ )
			{
				if ( *thisIter != *textIter )
				{
					return false;
				}
			}
		}

		return true;
	}

	bool Text::operator != ( const Text& _text )
	{
		return !( *this == _text );
	}

	Text& Text::operator = ( const Text& _text )
	{
		if ( *this != _text )
		{
			text		=	_text.text;
			properties	=	_text.properties;
		}

		return *this;
	}

	Text& Text::operator += ( const Text& _text )
	{
		text += _text.text;

		for ( list<Prl>::const_iterator iter = _text.properties.begin(); iter != _text.properties.end(); iter++ )
			properties.push_back(*iter);

		return *this;
	}


	const WCHAR Text::operator [] ( unsigned int index ) const
	{
		return text[index];
	}

	unsigned long Text::GetTextSize() const
	{
		return (unsigned long)text.size();
	}

	wstring Text::GetAllText() const
	{
		return text;
	}

	void Text::SetText (const WCHAR* _text)
	{
		if (_text)
		{
			text = std::wstring (_text);
		}
	}

	void Text::SetString (const std::wstring& source)
	{
		text	=	source;
	}

	vector<Prl> Text::GetRunProperties() const
	{
		vector<Prl> prls;

		for (list<Prl>::const_iterator iter = properties.begin(); iter != properties.end(); ++iter)
			prls.push_back( *iter );

		return prls;
	}

	unsigned int Text::PrlSize () const
	{
		return (unsigned int) properties.size();
	}

	void Text::AddProperty (short sprm, void* operand)
	{
		Prl prl( sprm, reinterpret_cast<byte*>( operand ) );
		properties.push_back( prl );
	}

	void Text::AddProperty (const Prl& prl)
	{
		properties.push_back( prl );
	}

	void Text::AddProperties (const vector<Prl>& prls)
	{
		for (vector<Prl>::const_iterator iter = prls.begin(); iter != prls.end(); ++iter)
		{
			properties.push_back( *iter ); 
		}
	}

	void Text::RemoveProperty( short sprm, void* operand )
	{
		Prl prl( sprm, reinterpret_cast<byte*>( operand ) );
		properties.remove( prl );  
	}

	void Text::RemoveAllProperties()
	{
		properties.clear();
	}

	IVirtualConstructor* Text::New() const
	{
		return new Text();
	}

	IVirtualConstructor* Text::Clone() const
	{
		return new Text(*this);
	}
}