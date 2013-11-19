#pragma once

#include "IVisitable.h"
#include "SinglePropertyModifier.h"

namespace DocFileFormat
{
	class PropertyExceptions: public IVisitable
	{
	public:
		/// A list of the sprms that encode the differences between 
		/// CHP for a character and the PAP for the paragraph style used.
		list<SinglePropertyModifier> *grpprl;

		virtual ~PropertyExceptions();
		PropertyExceptions();
		PropertyExceptions( const list<SinglePropertyModifier>& grpprl );
		PropertyExceptions( byte* bytes, int size );
	};
}
