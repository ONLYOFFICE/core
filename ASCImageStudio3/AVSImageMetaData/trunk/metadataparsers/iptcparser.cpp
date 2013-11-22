#include "stdafx.h"
#include "iptcparser.h"
#include <libiptcdata/iptc-data.h>
#include <libiptcdata/iptc-jpeg.h>
#include <vector>
#include "..\common\utils.h"

namespace AVS
{
	namespace Image
	{
		bool CImageIPTCParser::_DoParseFile(std::wstring & filename)
		{
			m_internalXMLEntry = L"";
			unsigned char buf[256*256];
			IptcData * _IptcData = NULL;
			FILE * infile = NULL;

			if (0 != _wfopen_s(&infile, filename.c_str(), L"rb"))
				return false;

			int ps3_len = iptc_jpeg_read_ps3(infile, buf, sizeof(buf));
			fclose(infile);

			if (ps3_len <= 0)
				return false;

			unsigned int iptc_len = 0;
			int iptc_off = iptc_jpeg_ps3_find_iptc (buf, ps3_len, &iptc_len);

			if (iptc_off)
				_IptcData = iptc_data_new_from_data (buf + iptc_off, iptc_len);

			if (_IptcData)
			{
				m_internalXMLEntry = _PrintIPTCData(_IptcData);
				iptc_data_unref(_IptcData);
			}
			else
				//throw Exception::iptc_parse_error();
				return false;
			return true;
		}

		std::wstring CImageIPTCParser::_DoGetXML()
		{
			std::wstringstream resultXMLString;
			resultXMLString<< L"<?xml version=\"1.0\" encoding=\"utf-8\"?>" <<
				L"<AVSImageMetaDataIPTC>" << m_internalXMLEntry
				<< L"</AVSImageMetaDataIPTC>";
			return resultXMLString.str();				
		}


		namespace Details
		{
			static std::wstringstream & operator << (std::wstringstream & strm,const std::vector<char> & vect) 
			{
				std::vector<char>::const_iterator i1 = vect.begin();
				while (i1 != vect.end())
				{
					strm << (*i1);
					i1++;
				}
				return strm;
			}
		}

		std::wstring CImageIPTCParser::_PrintIPTCData(IptcData * _data)
		{
			using Details::operator <<;
			std::wstringstream outputXMLEntry;
			if (!_data->count)
				return L"";

			for (unsigned int i=0; i < _data->count; i++)
			{
				IptcDataSet * e = _data->datasets[i];

				outputXMLEntry << L"<IPTCTag" << L" recordid=\"" <<  e->record << L"\""
					<< L" tagid=\"" <<  e->tag << L"\""
					<< L" tagname=\"" << iptc_tag_get_title(e->record, e->tag) << L"\""
					<< L" typeid=\"" << (int)(iptc_dataset_get_format(e)) << L"\""
					<< L" typename=\"" << iptc_format_get_name(iptc_dataset_get_format(e)) << L"\""
					<< L" size=\"" << e->size << L"\""
					<< L">";

				std::string _buffer;
				_buffer.resize(e->size + 1);
				switch (iptc_dataset_get_format (e))
				{
				case IPTC_FORMAT_BYTE:
				case IPTC_FORMAT_SHORT:
				case IPTC_FORMAT_LONG:
					outputXMLEntry << iptc_dataset_get_value (e);
					break;
				case IPTC_FORMAT_BINARY:
					iptc_dataset_get_as_str (e, &(_buffer[0]), _buffer.size());
					outputXMLEntry << AVS::Utils::__StoW(_buffer.c_str());
					break;
				default:
					iptc_dataset_get_data (e, (unsigned char*)(&(_buffer[0])), _buffer.size());
					outputXMLEntry << AVS::Utils::__StoW(_buffer.c_str());
					break;
				}
				outputXMLEntry << L"</IPTCTag>";
			}
			return outputXMLEntry.str();
		}
	}
}