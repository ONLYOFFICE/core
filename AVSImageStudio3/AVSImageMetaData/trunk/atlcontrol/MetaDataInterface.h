#ifndef _AVS_IMAGEMETADATA_METADATAINTERFACE_H_INCLUDED_
#define _AVS_IMAGEMETADATA_METADATAINTERFACE_H_INCLUDED_
#pragma once
#include <string>
#include "..\metadataparsers\metadataparser.h"

namespace AVS
{
	namespace Image
	{
		namespace Details
		{
			class ImageMetaDataCommonImpl
			{
			public:
				void ApplyData(AVS::Image::ImageMetaDataParser * parser)
				{
					_ASSERTE(NULL != parser);
					m_XML = parser->GetXML();
				}

			protected:
				std::wstring & _GetXML()
				{
					return m_XML;
				}

			private:
				std::wstring m_XML;
			};
		}
	}
}

#endif