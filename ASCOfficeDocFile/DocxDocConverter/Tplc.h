#pragma once

#include "Constants.h"
#include "LID.h"

namespace ASCDocFileFormat
{
	struct Tplc
	{
		virtual operator unsigned int() const = 0;
		virtual ~Tplc() {}
	};

	class TplcBuildIn: public Tplc
	{
	private:
		static const bool fBuildIn = true;
		Constants::NumberingFormat ilgpdM1;
		LID lid;

	public:
		TplcBuildIn():
		  ilgpdM1(Constants::numberingFormatNone), lid()
		  {
		  }

		  explicit TplcBuildIn( Constants::NumberingFormat _ilgpdM1, const LID& _lid ):
		  ilgpdM1(_ilgpdM1), lid(_lid)
		  {
		  }

		  explicit TplcBuildIn( unsigned int _tplcBuildIn ):
		  ilgpdM1(Constants::numberingFormatNone), lid()
		  {
			  this->ilgpdM1 = (Constants::NumberingFormat)FormatUtils::GetIntFromBits( _tplcBuildIn, 1, 15 );
			  this->lid = LID( (DocFileFormat::LanguageCode)FormatUtils::GetIntFromBits( _tplcBuildIn, 16, 16 ) );
		  }

		  virtual ~TplcBuildIn()
		  {
		  }

		  virtual operator unsigned int() const
		  {
			  unsigned int tplcValue = 1;

			  unsigned int ilgpdM1Value = ( ( (unsigned int)this->ilgpdM1 ) << 1 );
			  unsigned int lidValue = ( ( (unsigned int)this->lid ) << 16 );

			  tplcValue |= ( ilgpdM1Value | lidValue );

			  return tplcValue;
		  }
	};

	class TplcUser: public Tplc
	{
	private:
		static const bool fBuildIn = false;
		unsigned int wRandom;

	public:
		TplcUser():
		  wRandom(0)
		  {
		  }

		  explicit TplcUser( unsigned int _wRandom ):
		  wRandom(0)
		  {
			  this->wRandom = _wRandom;
		  }

		  virtual ~TplcUser()
		  {
		  }

		  virtual operator unsigned int() const
		  {
			  return this->wRandom;
		  }
	};

	struct TplcFactory
	{
		static Tplc* CreateTplc( unsigned int _tplc )
		{
			bool fBuildIn = FormatUtils::GetBitFromInt( _tplc, 0 );

			Tplc* tplc = NULL;

			if ( fBuildIn )
			{
				tplc = new TplcBuildIn( _tplc );
			}
			else
			{
				tplc = new TplcUser( _tplc );
			}

			return tplc;
		}
	};
}