#pragma once

#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	template<typename T> struct IOfficeArtAbstractContainer: public IOfficeArtRecord
	{
		//!!!TODO: Add or remove methods!!!
		virtual void PushBack( const T& element ) = 0;
		virtual unsigned int Count() const = 0;
		virtual void Clear() = 0;
		virtual ~IOfficeArtAbstractContainer() {}
	};
}