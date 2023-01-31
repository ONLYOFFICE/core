/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
		this->Changes = new std::list<SinglePropertyModifier>();
	}

	RevisionData::RevisionData( CharacterPropertyExceptions* chpx ) : Dttm(), Isbt(0), Type(NoRevision), Changes(NULL), RsidDel(0), RsidProp(0), Rsid(0)
	{
		if (!chpx) return;
		if (!chpx->grpprl)
			return;

		bool collectRevisionData = true;
		
		Changes = new std::list<SinglePropertyModifier>();

		for ( std::list<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
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
