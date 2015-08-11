
#include "TxO.h"
#include <Logic/Biff_structures/ODRAW/SimpleOfficeArtContainers.h>

namespace XLS
{;

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
	
	if ( cbRuns )
	{
		std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
		if ( recs.size() )
		{
			while( !recs.empty() )
			{
				record.appendRawData(recs.front());
				recs.pop_front();
			}
						
			commentText.setSize(cchText);
			record >> commentText;

			TxOruns.m_runCount = cbRuns / 8 - 1;
			TxOruns.load(record);
		

			if (record.getRdPtr() <  record.getDataSize())
			{
				m_OfficeArtSpContainer = ODRAW::OfficeArtRecordPtr(new ODRAW::OfficeArtSpContainer(ODRAW::OfficeArtRecord::CA_Sheet));
				record >> *m_OfficeArtSpContainer; //todooo !!! сделать проверку на тип

			}
		}
	}
}

} // namespace XLS

