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

#include "RevisionData.h"
#include "../../DesktopEditor/common/Types.h"

namespace DocFileFormat
{
	RevisionData::~RevisionData()
	{
		RELEASEOBJECT( Changes );
	}

	RevisionData::RevisionData():
	Dttm(), Isbt(0), Type(NoRevision), Changes(NULL), RsidDel(0), RsidProp(0), Rsid(0)
	{
		this->Changes = new std::vector<SinglePropertyModifier>();
	}

	RevisionData::RevisionData( CharacterPropertyExceptions* chpx ) : Dttm(), Isbt(0), Type(NoRevision), Changes(NULL), RsidDel(0), RsidProp(0), Rsid(0)
	{
		if (!chpx) return;
		if (!chpx->grpprl)
			return;

		bool collectRevisionData = true;
		
		Changes = new std::vector<SinglePropertyModifier>();

		for ( std::vector<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
		{
			switch ( iter->OpCode)
			{
				case sprmCPropRMark2:
				{
					collectRevisionData = false;
					this->Isbt = FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );
					this->Dttm = DateAndTime( ( iter->Arguments + 3 ), 4 );
				}break;		
				case sprmCFRMark:
				{
					collectRevisionData = false;
				}break;
				case sprmCIbstRMark:
				{
					this->Isbt = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
				}break;		
				case sprmCIbstRMarkDel:
				{
					this->Isbt = FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize);
				}break;
				case sprmCDttmRMark:
				{
					this->Dttm = DateAndTime( iter->Arguments, 4 );
				}break;	
				case sprmCDttmRMarkDel:
				{
					this->Dttm = DateAndTime(iter->Arguments, 4);
				}break;
				case sprmCFRMarkDel:
				{
					this->Type = Deleted;
				}break;	
				case sprmCRsidProp:
				{
					this->RsidProp = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				}break;
				case sprmCRsidText:
				{
					this->Rsid = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				}break;
				case sprmCRsidRMDel:
				{
					this->RsidDel = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				}break;				
				default:
					break;
			}

			//put the sprm on the revision stack
			if ( collectRevisionData )
			{
				this->Changes->push_back( *iter );
			}
		}

		//type
		if ( this->Type != Deleted )
		{
			if ( collectRevisionData )
			{
				//no mark was found, so this CHPX doesn't contain revision data
				this->Type = NoRevision;
			}
			else
			{
				if ( this->Changes->size() > 0 )
				{
					this->Type = Changed;
				}
				else
				{
					this->Type = Inserted;
					this->Changes->clear();
				}
			}
		}
	}
}
