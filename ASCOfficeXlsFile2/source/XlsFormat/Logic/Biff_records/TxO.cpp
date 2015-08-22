
#include "TxO.h"

#include <Logic/Biff_structures/ODRAW/SimpleOfficeArtContainers.h>
#include <Logic/Biff_records/Font.h>

#include <simple_xml_writer.h>
#include <utils.h>

namespace XLS
{

TxO::TxO():fmla(false)
{
	cbRuns = 0;
}


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
	hAlignment = static_cast<unsigned char>(GETBITS(flags, 1, 3));
	vAlignment = static_cast<unsigned char>(GETBITS(flags, 4, 6));	// reserved2 (2 bits)	
	fLockText = GETBIT(flags, 9);	// reserved3 (4 bits)	
	fJustLast = GETBIT(flags, 14);
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
	if ( recs.size() )
	{
		while( !recs.empty() )
		{
			record.appendRawData(recs.front());
			recs.pop_front();
		}

	}

	if ( cbRuns )
	{					
		commentText.setSize(cchText);
		record >> commentText;

		TxOruns.m_runCount = cbRuns / 8 - 1;
		TxOruns.load(record);
	}

	if (record.getRdPtr() <  record.getDataSize() - 8)
	{
		ODRAW::OfficeArtRecordHeader rh_child;
		record >> rh_child;
		record.RollRdPtrBack(rh_child.size());

		if (rh_child.recType == ODRAW::OfficeArtContainer::SpContainer)
		{
			m_OfficeArtSpContainer = ODRAW::OfficeArtRecordPtr(new ODRAW::OfficeArtSpContainer(ODRAW::OfficeArtRecord::CA_Sheet));
			record >> *m_OfficeArtSpContainer;
		}
	}
}


int TxO::serialize (std::wostream & _stream)
{
	std::wstring str_ = commentText.value();
	int str_size = str_.size();

	int Fmt = 0; 
	
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

			CP_XML_NODE(L"a:r")
			{
				Fmt = run->formatRun.ifnt;
				serialize_rPr(CP_XML_STREAM(), Fmt );

				CP_XML_NODE(L"a:t")
				{		
					//CP_XML_ATTR(L"xml:space", L"preserve");

					std::wstring str_part = str_.substr( run->formatRun.ich, end_string - run->formatRun.ich);

					CP_XML_STREAM() << xml::utils::replace_text_to_xml(str_part);
				}
			}
		}
	}
	
	return 0;
}
int TxO::serialize_rPr	(std::wostream & _stream, int iFmt)
{
	if (!pGlobalWorkbookInfoPtr)		return 0;
	if (!pGlobalWorkbookInfoPtr->fonts) return 0;

	int sz = pGlobalWorkbookInfoPtr->fonts->size();
	if (iFmt -1 > sz || iFmt < 1) return 0;

	Font * font = dynamic_cast<Font*>(pGlobalWorkbookInfoPtr->fonts->at(iFmt-1).get());

	if (font) font->serialize_rPr(_stream);
	
	return 0;
}

} // namespace XLS

