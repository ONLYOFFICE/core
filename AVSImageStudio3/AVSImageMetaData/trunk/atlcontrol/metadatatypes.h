#ifndef _AVS_IMAGEMETADATA_METADATATYPES_H_INCLUDED_
#define _AVS_IMAGEMETADATA_METADATATYPES_H_INCLUDED_
#pragma once 

namespace AVS
{
	namespace Image
	{
		class MetaDataTypes
		{
		public:
			static const unsigned int Invalid = 0;
			static const unsigned int Exif = 1;
			static const unsigned int IPTC = 2;
		};
	}
}


#endif
