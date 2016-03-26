
#include "FtPictFmla.h"
#include <Binary/CFRecord.h>

namespace XLS
{


FtPictFmla::FtPictFmla()
:	fmla(true)
{
}

BiffStructurePtr FtPictFmla::clone()
{
	return BiffStructurePtr(new FtPictFmla(*this));
}



void FtPictFmla::store(CFRecord& record, FtPioGrbit& pictFlags)
{
	unsigned short ft = 0x0009; // reserved
	record << ft;
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cbFmla*/);
	size_t start_ptr = record.getDataSize();

	fmla.store(record);

	if(fmla.fmla.HasPtgTbl())
	{
		record << lPosInCtlStm;
	}
	if(pictFlags.fPrstm)
	{
		record << cbBufInCtlStm;
	}
	if(pictFlags.fPrstm)
	{
		key.store(record);
	}

	size_t data_size = record.getDataSize() - start_ptr;
	record.registerDelayedDataSource(data_size, rt_Obj);
}

void FtPictFmla::load(CFRecord& record, int linkSize)
{
	bool  bLinked	= false;
 	bool  bEmbedded = false;

	if( linkSize >= 6 )
    {
        _UINT16 nFmlaSize;
        record >> nFmlaSize;

        if( nFmlaSize > 0) 
        {
            record.skipNunBytes(4); 
            _UINT16 nToken;
            record >> nToken;

            if( nToken == 1)//XclTokenArrayHelper::GetTokenId( EXC_TOKID_NAMEX, EXC_TOKCLASS_REF ) )
            {
                bLinked = true;
                _UINT16 nRefIdx, nNameIdx;

                record >> nRefIdx;
				record.skipNunBytes(8); 
                record >> nNameIdx;
				record.skipNunBytes(12); 
                
				//const ExtName* pExtName = GetOldRoot().pExtNameBuff->GetNameByIndex( nRefIdx, nNameIdx );
    //            if( pExtName && pExtName->IsOLE() )
    //                mnStorageId = pExtName->nStorageId;
            }
            else if( nToken == 2)//XclTokenArrayHelper::GetTokenId( EXC_TOKID_TBL, EXC_TOKCLASS_NONE ) )
            {
                bEmbedded = true;

				record.skipNunBytes( nFmlaSize - 1 );      // token ID already read
                if( nFmlaSize & 1 )
                    record.skipNunBytes( 1 );              // padding byte

                // a class name may follow inside the picture link
                if( record.getRdPtr() + 2 <= record.getDataSize() )
                {
                    _UINT16 nLen;
                    record >> nLen;
                    if( nLen > 0 )
					{
                        //maClassName =  rStrm.ReadRawByteString( nLen );
					}
                }
            }
        }
    }
}

void FtPictFmla::load(CFRecord& record, FtPioGrbit& pictFlags)
{
	record.skipNunBytes(4); // reserved

	fmla.load(record);

	if(fmla.fmla.HasPtgTbl())
	{
		record >> lPosInCtlStm;
	}
	record >> lPosInCtlStm;
	if(pictFlags.fPrstm)
	{
		record >> cbBufInCtlStm;
	}
	if(pictFlags.fPrstm)
	{
		key.load(record);
	}
}


} // namespace XLS

