#ifndef _AVS_IMAGEMETADATA_METADATAPARSER_H_INCLUDED_
#define _AVS_IMAGEMETADATA_METADATAPARSER_H_INCLUDED_
#pragma once 

#include <exception>
#include <string>
#include <memory>

namespace AVS
{
	namespace Image
	{
		namespace Exception
		{
			class metadata_parse_error: public std::exception
			{
			public:
				metadata_parse_error():std::exception("could not parse file")
				{}
			};
		}

		class ImageMetaDataParser
		{
		public:
			typedef std::auto_ptr<ImageMetaDataParser> AutoPtr;

			bool ParseFile(std::wstring & filename)
			{
				return _DoParseFile(filename);
			}

			std::wstring GetXML()
			{
				return _DoGetXML();
			}

		private:
			virtual bool _DoParseFile(std::wstring & filename) = 0;
			virtual std::wstring _DoGetXML() = 0;
		};
	}
}

#endif