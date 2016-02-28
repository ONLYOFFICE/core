
#include "TxO.h"

#include <Logic/Biff_records/Font.h>

#include <utils.h>

#include "../../../XlsXlsxConverter/XlsConverter.h"
#include "../../../XlsXlsxConverter/xlsx_conversion_context.h"


namespace XLS
{

TxO::~TxO()
{
}


BaseObjectPtr TxO::clone()
{
	return BaseObjectPtr(new TxO(*this));
}


void TxO::writeFields(CFRecord& record)
{
}


void TxO::readFields(CFRecord& record)
{
	pGlobalWorkbookInfoPtr = record.getGlobalWorkbookInfo();

	unsigned short flags;
	record >> flags;
	
	hAlignment	= static_cast<unsigned char>(GETBITS(flags, 1, 3));
	vAlignment	= static_cast<unsigned char>(GETBITS(flags, 4, 6));	// reserved2 (2 bits)	
	
	fLockText	= GETBIT(flags, 9);	// reserved3 (4 bits)	
	fJustLast	= GETBIT(flags, 14);
	fSecretEdit = GETBIT(flags, 15);

	record >> rot;
	record.skipNunBytes(6);	// reserved4 + reserved5
	//record >> controlInfo;	// The field MUST exist if and only if the value of cmo.ot in the preceding Obj record is 0, 5, 7, 11, 12, or 14.
	record >> cchText;

	if (cchText != 0)
		record >> cbRuns;
	
	record >> ifntEmpty;
	fmla.load(record);
	
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];

	int sz = cchText;
	if ( cbRuns )
	{	
		while (record.getRdPtr() + cchText > record.getDataSize() && !recs.empty())
		{
			record.appendRawData(recs.front());
			recs.pop_front();
		}
		commentText.setSize(cchText);
		record >> commentText;

		TxOruns.set_records(&recs);

		TxOruns.m_runCount = cbRuns / 8 - 1;
		TxOruns.load(record);
	}

	while( !recs.empty() )
	{
		sp_enabled	= true;
		mso_drawing_->storeRecordAndDecideProceeding(recs.front());
		recs.pop_front();
	}
}


int TxO::serialize (std::wostream & _stream)
{
	std::wstring str_ = commentText.value();
	int str_size = str_.size();

	int Fmt = 0; 
	
	std::wstring namespace_ = L"a:";
	oox::external_items::Type type = pGlobalWorkbookInfoPtr->xls_converter->xlsx_context->get_drawing_context().getType();

	if (type == oox::external_items::typeComment) 
		namespace_.clear();
	
	CP_XML_WRITER(_stream)    
	{
		for (int i = 0 ; i < TxOruns.rgTxoRuns.size(); i++)
		{
			Run *run = dynamic_cast<Run*>(TxOruns.rgTxoRuns[i].get());
			if (run == NULL) continue;

			int end_string = str_size;

			if ( i < TxOruns.rgTxoRuns.size() - 1)
			{
				Run *run_next = dynamic_cast<Run*>(TxOruns.rgTxoRuns[i+1].get());
				if (run_next)
					end_string = run_next->formatRun.ich;
			}

			CP_XML_NODE(namespace_ + L"r")
			{
				Fmt = run->formatRun.ifnt;
				
				serialize_rPr(CP_XML_STREAM(), Fmt, namespace_);

				CP_XML_NODE(namespace_ + L"t")
				{
					if (preserve_enabled)
					{
						CP_XML_ATTR(L"xml:space", L"preserve");
					}

					if (run->formatRun.ich > str_.length())
					{
						//ошибка
						run->formatRun.ich = 0;
					}

					std::wstring str_part = str_.substr( run->formatRun.ich, end_string - run->formatRun.ich);

					CP_XML_STREAM() << xml::utils::replace_text_to_xml(str_part);
				}
			}
		}
	}
	
	return 0;
}
int TxO::serialize_rPr	(std::wostream & _stream, int iFmt, std::wstring namespace_)
{
	if (!pGlobalWorkbookInfoPtr)			return 0;
	if (!pGlobalWorkbookInfoPtr->m_arFonts) return 0;

	int sz = pGlobalWorkbookInfoPtr->m_arFonts->size();
	if (iFmt -1 > sz || iFmt < 1) return 0;

	Font * font = dynamic_cast<Font*>(pGlobalWorkbookInfoPtr->m_arFonts->at(iFmt-1).get());

	if (!font) return 0;

	if (namespace_.empty())
	{
		CP_XML_WRITER(_stream)    
		{	
			CP_XML_NODE( L"rPr")
			{
				font->serialize_properties(CP_XML_STREAM(), true);
			}
		}
	}
	else
		font->serialize_rPr(_stream, false, false, namespace_);
	
	return 0;
}

} // namespace XLS

