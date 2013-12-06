#pragma once

#include "IRunItem.h"
#include "CSymbolOperand.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
	class EndnoteRef: public IRunItem
	{
	private:
		static const byte CFSpec = 1;
		short aEndIdx;

		wstring text;
		list<Prl> properties;

	public:
		explicit EndnoteRef (short _aEndIdx = 0, unsigned short _ftc = 0, WCHAR _xchar = 0x0000) : aEndIdx(_aEndIdx)
		{
			if ( this->aEndIdx == 0 )
			{
				//!!!TODO!!!
				this->properties.push_back( Prl( (short)DocFileFormat::sprmCSymbol, (byte*)CSymbolOperand( _ftc, _xchar ) ) );
			}
			else
			{
				this->text = wstring( &TextMark::AutoNumberedEndnoteReference );
				this->properties.push_back( Prl( (short)DocFileFormat::sprmCFSpec, (byte*)(&CFSpec) ) );
			}
		}

		EndnoteRef (const EndnoteRef& _endnoteRef) : aEndIdx(_endnoteRef.aEndIdx), text(_endnoteRef.text), properties(_endnoteRef.properties)
		{

		}

		short GetIndex() const
		{
			return this->aEndIdx;
		}

		virtual ~EndnoteRef()
		{
		}

		virtual IVirtualConstructor* New() const
		{
			return new EndnoteRef();
		}

		virtual IVirtualConstructor* Clone() const
		{
			return new EndnoteRef(*this);
		}

		virtual unsigned long GetTextSize() const
		{
			return text.size();
		}

		virtual wstring GetAllText() const
		{
			return text;
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
			return (unsigned int) properties.size();
		}
	};
}