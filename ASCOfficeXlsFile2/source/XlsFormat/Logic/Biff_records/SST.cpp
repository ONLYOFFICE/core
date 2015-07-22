#include "SST.h"

#include <simple_xml_writer.h>

namespace XLS
{;

SST::SST(const unsigned short code_page)
:	code_page_(code_page)
{
}


SST::~SST()
{
}


BaseObjectPtr SST::clone()
{
	return BaseObjectPtr(new SST(*this));
}


void SST::writeFields(CFRecord& record)
{
	record << cstTotal << cstUnique;

	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	for (BiffStructurePtrVector::iterator it = rgb.begin(), itEnd = rgb.end(); it != itEnd ; ++it)
	{
		XLUnicodeRichExtendedStringPtr element = boost::dynamic_pointer_cast<XLUnicodeRichExtendedString>(*it);
		if(!element)
		{
			throw;// EXCEPT::LE::WrongAPIUsage("boost::shared_dynamic_cast failed to cast to XLUnicodeRichExtendedStringPtr", __FUNCTION__);
		}
		CFRecord& current_record = recs.size() ? *recs.back() : record; // Points to the original 'record' or the last created 'Continue'

		unsigned int ssss;
		if(!current_record.checkFitWriteSafe(ssss = element->getNonVariablePartSize())) // If we cannot write non-variable part into the current record
		{
			current_record.commitData();
			CFRecordPtr cont_record(new CFRecord(rt_Continue, record.getGlobalWorkbookInfo()));
			recs.push_back(cont_record);
			*cont_record << *element;
		}
		else
		{
			current_record << *element;
		}
	}
	if(recs.size())
	{
		recs.back()->commitData();
	}
}


void SST::readFields(CFRecord& record)
{
	record >> cstTotal >> cstUnique;
	long total = cstTotal;
	long unique = cstUnique;
		
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	while(!record.isEOF() || !recs.empty())
	{
		XLUnicodeRichExtendedStringPtr element(new XLUnicodeRichExtendedString(recs));
		if(record.isEOF()) // If the break is at the XLUnicodeRichExtendedString boundary
		{
			element->appendNextContinue(record, false);
		}
		record >> *element;
		rgb.push_back(element);
	}
}


int SST::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"sst")
		{
			CP_XML_ATTR(L"uniqueCount", rgb.size());
			CP_XML_ATTR(L"xmlns", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");

			for (long i=0; i < rgb.size(); i++)
			{
				CP_XML_NODE(L"si")
				{				
					CP_XML_NODE(L"t")
					{		
						XLUnicodeRichExtendedString *richText = dynamic_cast<XLUnicodeRichExtendedString *>(rgb[i].get());
						CP_XML_STREAM() << richText->str_;
					}
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

