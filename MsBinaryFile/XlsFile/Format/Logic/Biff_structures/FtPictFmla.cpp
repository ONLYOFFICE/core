/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "FtPictFmla.h"

namespace XLS
{

BiffStructurePtr FtPictFmla::clone()
{
	return BiffStructurePtr(new FtPictFmla(*this));
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
                
            }
            else if( nToken == 2)
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
	short ft, size;
	record >> ft; // must be 
	if (ft != 0x0009)
	{
		record.RollRdPtrBack(2);
		return;
	}
	fExist = true;

	record >> size;
	if (size < 1) return;

	fmla.load(record);

	if(fmla.fmla.HasPtgTbl())
	{
		record >> lPosInCtlStm;
	}
	if(pictFlags.fPrstm)
	{
		record >> cbBufInCtlStm;
	}
	if(pictFlags.fCtl)
	{
		key.load(record);
	}
}

void FtPictFmla::save(CFRecord& record, FtPioGrbit& pictFlags)
{
	unsigned short ft = 0x0009, cb = 0;
	record << ft;
	record.reserveNunBytes(2);
	auto sizePos = record.getRdPtr();

	fmla.save(record);
	if(fmla.fmla.HasPtgTbl())
	{
		record << lPosInCtlStm;
	}
	if(pictFlags.fPrstm)
	{
		record << cbBufInCtlStm;
	}
	if(pictFlags.fCtl)
	{
		key.save(record);
	}
	cb = record.getRdPtr() - sizePos;
	record.RollRdPtrBack(cb+2);
	record << cb;
	record.skipNunBytes(cb);
}

} // namespace XLS

