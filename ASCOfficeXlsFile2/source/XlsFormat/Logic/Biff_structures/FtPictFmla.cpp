/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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

