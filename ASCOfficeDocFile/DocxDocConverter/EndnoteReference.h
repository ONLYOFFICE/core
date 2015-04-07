#pragma once

#include "IRunItem.h"
#include "CSymbolOperand.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
	class EndnoteReference: public IRunItem
	{
	private:
		static const unsigned char CFSpec = 1;
		short aEndIdx;

		wstring text;
		list<Prl> properties;

	public:
		explicit EndnoteReference( short _aEndIdx = 0, unsigned short _ftc = 0, wchar_t _xchar = 0x0000 ):
		aEndIdx(_aEndIdx)
		{
			if ( this->aEndIdx == 0 )
			{
				//!!!TODO!!!
				this->properties.push_back( Prl( (short)DocFileFormat::sprmCSymbol, (unsigned char*)CSymbolOperand( _ftc, _xchar ) ) );
			}
			else
			{
				this->text = wstring( &TextMark::AutoNumberedEndnoteReference );
				this->properties.push_back( Prl( (short)DocFileFormat::sprmCFSpec, (unsigned char*)(&CFSpec) ) );
			}
		}

		EndnoteReference( const EndnoteReference& _endnoteReference ):
		aEndIdx(_endnoteReference.aEndIdx), text(_endnoteReference.text), properties(_endnoteReference.properties)
		{
		}

		short GetIndex() const
		{
			return this->aEndIdx;
		}

		virtual ~EndnoteReference()
		{
		}

		virtual IVirtualConstructor* New() const
		{
			return new EndnoteReference();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new EndnoteReference(*this);
		}

		virtual unsigned long GetTextSize() const
		{
			return this->text.size();
		}

		virtual wstring GetAllText() const
		{
			return this->text;
		}

		virtual vector<Prl> GetRunProperties() const
		{
			vector<Prl> prls;

			for ( list<Prl>::const_iterator iter = this->properties.begin(); iter != this->properties.end(); iter++ )
			{
				prls.push_back( *iter );
			}

			return prls;
		}

		virtual unsigned int PrlSize () const
		{
			return (unsigned int)properties.size();
		}
	};
}