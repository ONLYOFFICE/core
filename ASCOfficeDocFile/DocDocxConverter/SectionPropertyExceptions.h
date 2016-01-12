#pragma once

#include "PropertyExceptions.h"

namespace DocFileFormat
{
	class SectionPropertyExceptions: public PropertyExceptions
	{
		public:  
		/// Parses the bytes to retrieve a SectionPropertyExceptions
		SectionPropertyExceptions( unsigned char* bytes, int size ):
		PropertyExceptions( bytes, size ), isBidi(false)
		{
			for ( list<SinglePropertyModifier>::iterator iter = this->grpprl->begin(); iter != this->grpprl->end(); iter++ )
			{
				SinglePropertyModifier sprm( *iter );
				  
				if( sprm.OpCode == sprmSFBiDi ) 
				{
					isBidi = true;
				}
			}
		}

		virtual ~SectionPropertyExceptions()
		{
		}

		bool isBidi;
	};
}