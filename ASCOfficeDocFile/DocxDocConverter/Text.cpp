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


	const wchar_t Text::operator [] ( unsigned int index ) const
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

	void Text::SetText (const wchar_t* _text)
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
		Prl prl( sprm, reinterpret_cast<unsigned char*>( operand ) );
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
		Prl prl( sprm, reinterpret_cast<unsigned char*>( operand ) );
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