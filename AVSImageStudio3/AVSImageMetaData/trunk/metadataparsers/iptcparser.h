#ifndef _AVS_IMAGEMETADATA_IPTCPARSER_H_INCLUDED_
#define _AVS_IMAGEMETADATA_IPTCPARSER_H_INCLUDED_
#pragma once 


#include "metadataparser.h"
#include "parserguids.h"
#include <string>
#include <sstream>

extern "C" {
	struct _IptcData;
	typedef struct _IptcData        IptcData;
}

namespace AVS
{
	namespace Image
	{
		namespace Exception
		{
			class iptc_parse_error: public metadata_parse_error
			{
			public:
				iptc_parse_error()
				{}
			};
		}

		class CImageIPTCParser: public ImageMetaDataParser
		{
		public:
			static const GUID * GetGuid()
			{
				return &PARSER_IPTC_JPEG;
			}
		private:
			virtual bool _DoParseFile(std::wstring & filename) override;
			virtual std::wstring _DoGetXML() override;
		private:
			std::wstring m_internalXMLEntry;
			std::wstring _PrintIPTCData(IptcData * _data);

		public:
			static ImageMetaDataParser::AutoPtr createRoutine()
			{
				return ImageMetaDataParser::AutoPtr(new CImageIPTCParser());
			}
		};
	}
}

#endif