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