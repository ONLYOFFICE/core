#pragma once

#include "DocumentMapping.h"
#include "TableInfo.h"

namespace DocFileFormat
{
	class HeaderMapping: public DocumentMapping
	{
	private: 
		CharacterRange _hdr;

	public: 
		HeaderMapping( ConversionContext* ctx, CharacterRange hdr );
		virtual void Apply( IVisitable* visited );
	};
}