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

#include "LPStshiGrpPrl.h"

//!!!A structure that has no effect and MUST be ignored.!!!

namespace ASCDocFileFormat
{
	class STSHIB: public IOperand
	{
	private:
		LPStshiGrpPrl grpprlChpStandard;
		LPStshiGrpPrl grpprlPapStandard;

		unsigned char* bytes;
		unsigned int sizeInBytes;

	public:
		STSHIB():
		  grpprlChpStandard(), grpprlPapStandard(), bytes(NULL), sizeInBytes(0)
		  {
		  }

		  explicit STSHIB( const LPStshiGrpPrl& _grpprlChpStandard, const LPStshiGrpPrl& _grpprlPapStandard ):
		  grpprlChpStandard(_grpprlChpStandard), grpprlPapStandard(_grpprlPapStandard), bytes(NULL), sizeInBytes(0)
		  {
			  this->sizeInBytes = ( grpprlChpStandard.Size() + grpprlPapStandard.Size() );

			  this->bytes = new unsigned char[this->sizeInBytes];

			  if ( this->bytes != NULL )
			  {
				  memset( this->bytes, 0, this->sizeInBytes );
				  memcpy( this->bytes, (unsigned char*)(this->grpprlChpStandard), this->grpprlChpStandard.Size() );
				  memcpy( ( this->bytes + this->grpprlChpStandard.Size() ), (unsigned char*)(this->grpprlPapStandard), this->grpprlPapStandard.Size() );
			  }
		  }

		  STSHIB( const STSHIB& _stshib ):
		  grpprlChpStandard(_stshib.grpprlChpStandard), grpprlPapStandard(_stshib.grpprlPapStandard), bytes(NULL), sizeInBytes(_stshib.sizeInBytes)
		  {
			  this->bytes = new unsigned char[this->sizeInBytes];

			  if ( this->bytes != NULL )
			  {
				  memset( this->bytes, 0, this->sizeInBytes );
				  memcpy( this->bytes, _stshib.bytes, this->sizeInBytes );
			  }
		  }

		  virtual ~STSHIB()
		  {
			  RELEASEARRAYOBJECTS( bytes );
		  }

		  virtual operator unsigned char*() const
		  {
			  return this->bytes;
		  }

		  virtual operator const unsigned char*() const
		  {
			  return (const unsigned char*)this->bytes;
		  }

		  virtual unsigned int Size() const
		  {
			  return this->sizeInBytes;
		  }
	};
}