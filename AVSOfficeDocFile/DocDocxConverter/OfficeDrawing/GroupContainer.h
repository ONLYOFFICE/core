#pragma once

#include "ShapeContainer.h"

namespace DocFileFormat
{
	class GroupContainer: public RegularContainer
	{
	public:
		static const unsigned short TYPE_CODE_0xF003 = 0xF003;

		int Index;

		GroupContainer() : RegularContainer(), Index(0)
		{
		}

		GroupContainer(IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : RegularContainer(_reader, size, typeCode, version, instance), Index(0)
		{
			for ( unsigned int i = 0; i < this->Children.size(); i++ )
			{
				Record* groupChild = this->Children[i];

				if ( groupChild != NULL )
				{
					if ( groupChild->TypeCode == 0xF003 )
					{
						// the child is a subgroup
						GroupContainer* group = static_cast<GroupContainer*>(groupChild);
						group->Index = i;
						this->Children[i] = group;
					}
					else if ( groupChild->TypeCode == 0xF004 )
					{
						// the child is a shape
						ShapeContainer* shape = static_cast<ShapeContainer*>(groupChild);
						shape->Index = i;
						this->Children[i] = shape;
					}
				}
			}
		}

		virtual ~GroupContainer() 
		{
		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new GroupContainer( _reader, bodySize, typeCode, version, instance );
		}
	}; 
}