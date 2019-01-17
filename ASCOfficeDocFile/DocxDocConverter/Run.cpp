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

#include "Run.h"

namespace ASCDocFileFormat
{
	Run::Run()
	{
	}

	/*========================================================================================================*/

	Run::Run( const IRunItem& _runItem )
	{
		this->items.push_back( RunItem( _runItem ) );

		vector<Prl> prls = _runItem.GetRunProperties();

		for ( vector<Prl>::const_iterator iter = prls.begin(); iter != prls.end(); iter++ )
		{
			this->properties.push_back( *iter );
		}
	}

	/*========================================================================================================*/

	Run::Run( const Run& _run ):
	properties(_run.properties)
	{
		for ( list<RunItem>::const_iterator iter = _run.items.begin(); iter != _run.items.end(); iter++ )
		{
			this->items.push_back( *iter );
		}
	}

	/*========================================================================================================*/

	Run& Run::operator = ( const Run& _run )
	{
		if ( this != &_run )
		{
			this->properties = _run.properties;
			this->items.clear();

			for ( list<RunItem>::const_iterator iter = _run.items.begin(); iter != _run.items.end(); iter++ )
			{
				this->items.push_back( *iter );
			}
		}

		return *this;
	}

	/*========================================================================================================*/

	void Run::AddRunItem( const IRunItem& _runItem )
	{
		this->items.push_back( RunItem( _runItem ) );

		vector<Prl> prls = _runItem.GetRunProperties();

		for ( vector<Prl>::const_iterator iter = prls.begin(); iter != prls.end(); iter++ )
		{
			this->properties.push_back( *iter );
		}
	}

	/*========================================================================================================*/

	Run::~Run()
	{
	}

	/*========================================================================================================*/

	unsigned long Run::GetTextSize() const
	{
		unsigned long textSize = 0;

		for ( list<RunItem>::const_iterator iter = this->items.begin(); iter != this->items.end(); iter++ )
		{
			textSize += (*iter)->GetTextSize();
		}

		return textSize;
	}

	/*========================================================================================================*/

	wstring Run::GetAllText() const
	{
		wstring allText;

		for ( list<RunItem>::const_iterator iter = this->items.begin(); iter != this->items.end(); iter++ )
		{
			allText += (*iter)->GetAllText();
		}

		return allText;
	}

	/*========================================================================================================*/

	vector<Chpx> Run::GetRunProperties( vector<unsigned int>* runOffsets ) const
	{
		vector<Chpx> chpxs;

		vector<Prl> prls;

		if ( runOffsets != NULL )
		{
			for ( list<Prl>::const_iterator iter = this->properties.begin(); iter != this->properties.end(); iter++ )
			{
				prls.push_back( *iter );
			}

			Chpx chpx( prls );

			chpxs.push_back( chpx );

			runOffsets->push_back( 0 );
		}

		return chpxs;
	}

	/*========================================================================================================*/

	unsigned int Run::PrlSize () const
	{
		return (unsigned int)properties.size();
	}

	/*========================================================================================================*/

	vector<IParagraphItemPtr> Run::GetAllRunsCopy( vector<unsigned int>* runOffsets ) const
	{
		vector<IParagraphItemPtr> paragraphItems;

		if ( runOffsets != NULL )
		{
			paragraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(this->Clone()) ) );
			runOffsets->push_back( 0 );
		}

		return paragraphItems;
	}

	/*========================================================================================================*/

	void Run::AddProperty( short sprm, void* operand )
	{
		Prl prl( sprm, reinterpret_cast<unsigned char*>( operand ) );
		this->properties.push_back( prl );
	}

	/*========================================================================================================*/

	void Run::AddProperty( const Prl& prl )
	{
		this->properties.push_back( prl );
	}

	/*========================================================================================================*/

	void Run::AddProperties( const vector<Prl>& prls )
	{
		for ( vector<Prl>::const_iterator iter = prls.begin(); iter != prls.end(); iter++ )
		{
			this->properties.push_back( *iter ); 
		}
	}

	/*========================================================================================================*/

	void Run::AddOrReplaceProperties( const vector<Prl>& prls )
	{
		for ( vector<Prl>::const_iterator iter = prls.begin(); iter != prls.end(); iter++ )
		{
			this->RemovePropertyByCode( iter->GetSprmCode() );

			this->AddProperty( *iter ); 
		}    
	}

	/*========================================================================================================*/

	void Run::RemoveProperty( short sprm, void* operand )
	{
		Prl prl( sprm, reinterpret_cast<unsigned char*>( operand ) );
		this->properties.remove( prl );  
	}

	/*========================================================================================================*/

	void Run::RemovePropertyByCode( unsigned short sprm )
	{
		for ( list<Prl>::iterator iter = this->properties.begin(); iter != this->properties.end(); iter++ )
		{
			if ( iter->GetSprmCode() == sprm )
			{
				this->properties.erase( iter );

				this->RemovePropertyByCode( sprm );

				break;
			}
		}
	}

	/*========================================================================================================*/

	void Run::RemoveAllProperties()
	{
		this->properties.clear();
	}

	/*========================================================================================================*/

	IVirtualConstructor* Run::New() const
	{
		return new Run();
	}

	/*========================================================================================================*/

	IVirtualConstructor* Run::Clone() const
	{
		return new Run( *this );
	}

	/*========================================================================================================*/

	Run::const_iterator Run::begin() const
	{
		return this->items.begin();
	}

	/*========================================================================================================*/

	Run::const_iterator Run::end() const
	{
		return this->items.end();
	}

	/*========================================================================================================*/

	Run::iterator Run::begin()
	{
		return this->items.begin();
	}

	/*========================================================================================================*/

	Run::iterator Run::end()
	{
		return this->items.end();
	}

	/*========================================================================================================*/

	Run::const_reverse_iterator Run::rbegin() const
	{
		return this->items.rbegin();
	}

	/*========================================================================================================*/

	Run::const_reverse_iterator Run::rend() const
	{
		return this->items.rend();
	}

	/*========================================================================================================*/

	Run::reverse_iterator Run::rbegin()
	{
		return this->items.rbegin();
	}

	/*========================================================================================================*/

	Run::reverse_iterator Run::rend()
	{
		return this->items.rend();
	}

	/*========================================================================================================*/

	Run::const_reference Run::front() const
	{
		return *this->begin();
	}

	/*========================================================================================================*/

	Run::const_reference Run::back() const
	{
		return *(--this->end());
	}

	/*========================================================================================================*/

	Run::reference Run::front()
	{
		return *this->begin();
	}

	/*========================================================================================================*/

	Run::reference Run::back()
	{
		return *(--this->end());
	}
}