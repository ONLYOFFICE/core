#pragma once

#include "RecordFactory.h"

namespace DocFileFormat
{
	/// Regular containers are containers with Record children.
	/// There also is containers that only have a zipped XML payload.
	class RegularContainer: public Record
	{
	public: 
		RegularContainer() :	Record()
		{
		}

		virtual ~RegularContainer()
		{
			for (vector<Record*>::iterator iter = Children.begin(); iter != Children.end(); ++iter)
			{
				RELEASEOBJECT (*iter);
			}
		}

		RegularContainer (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record (_reader, size, typeCode, version, instance)
		{
			unsigned int readSize = 0;
			unsigned int idx = 0;

			while ( readSize < this->BodySize )
			{
				Record* child = NULL;

				child = RecordFactory::ReadRecord( this->Reader, idx );

				if ( child != NULL )
				{
					this->Children.push_back( child );
					child->_ParentRecord = this;

					readSize += child->GetTotalSize();

					idx++;
				}
				else
				{
					readSize += this->BodySize;
				}
			}
		}

		/// Finds the first child of the given type.
		template<class T> T* FirstChildWithType() const
		{
			T* firstChildWithType = NULL;

			for ( vector<Record*>::const_iterator iter = this->Children.begin(); iter != this->Children.end(); iter++ )
			{
				if ( (*iter != NULL) && (typeid(T) == typeid(**iter)) )
				{
					firstChildWithType = static_cast<T*>(*iter);

					break;
				}
			}

			return firstChildWithType;
		}

	public: 

		vector<Record*> Children;
	};
}