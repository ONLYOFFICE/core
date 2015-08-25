#pragma once

#include "RegularContainer.h"
#include "ShapeOptions.h"
#include "Shape.h"

namespace DocFileFormat
{
	class ShapeContainer: public RegularContainer
	{
	public:
		static const unsigned short TYPE_CODE_0xF004 = 0xF004;

		int Index;

		ShapeContainer():
		RegularContainer(), Index(0)
		{
		}

		ShapeContainer( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ):
		RegularContainer( _reader, size, typeCode, version, instance ), Index(0) 
		{ 
		}

		int getShapeType()
		{
			int ret = 0;

			for ( vector<Record*>::const_iterator iter = this->Children.begin(); iter != this->Children.end(); iter++ )
			{
				Shape* sh = dynamic_cast<Shape*>( *iter );
				if (sh)
				{
					if (sh->shapeType) return sh->shapeType->GetTypeCode();
					else return 0;
				}
			}
			return 0;
		}

		virtual ~ShapeContainer()
		{
		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new ShapeContainer( _reader, bodySize, typeCode, version, instance );
		}

		/// Searches all OptionEntry in the ShapeContainer and puts them into a list.
		list<OptionEntry> ExtractOptions() const
		{
			list<OptionEntry> ret;

			//build the list of all option entries of this shape
			for ( vector<Record*>::const_iterator iter = this->Children.begin(); iter != this->Children.end(); iter++ )
			{
				ShapeOptions* opt = dynamic_cast<ShapeOptions*>( *iter );

				if ( opt != NULL )
				{
					for ( vector<OptionEntry>::iterator oeIter = opt->Options.begin(); oeIter != opt->Options.end(); oeIter++ )
					{
						ret.push_back( *oeIter );
					}
				}
			}

			return ret;
		}
	};
}