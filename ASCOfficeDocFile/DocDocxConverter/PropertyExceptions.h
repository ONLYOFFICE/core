#pragma once

#include "IVisitable.h"
#include "SinglePropertyModifier.h"

#include <list>

namespace DocFileFormat
{
	class PropertyExceptions: public IVisitable
	{
	public:
		/// A list of the sprms that encode the differences between 
		/// CHP for a character and the PAP for the paragraph style used.
		std::list<SinglePropertyModifier> *grpprl;

		virtual ~PropertyExceptions();
		PropertyExceptions();
		PropertyExceptions( const std::list<SinglePropertyModifier>& grpprl );
		PropertyExceptions( unsigned char* bytes, int size );
	};
}
