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

#include "RevisionData.h"
#include "../../DesktopEditor/common/Types.h"

namespace DocFileFormat
{
	RevisionData::~RevisionData()
	{
		RELEASEOBJECT( Changes );
	}

	/*========================================================================================================*/

	RevisionData::RevisionData():
	Dttm(), Isbt(0), Type(NoRevision), Changes(NULL), RsidDel(0), RsidProp(0), Rsid(0)
	{
		this->Changes = new std::list<SinglePropertyModifier>();
	}

	/*========================================================================================================*/

	/// Collects the revision data of a CHPX
	RevisionData::RevisionData( CharacterPropertyExceptions* chpx ):
	Dttm(), Isbt(0), Type(NoRevision), Changes(NULL), RsidDel(0), RsidProp(0), Rsid(0)
	{
		bool collectRevisionData = true;
		this->Changes = new std::list<SinglePropertyModifier>();

		for ( std::list<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
		{
			switch ( iter->OpCode)
			{
				//revision data
			case 0xCA89:
				{
					//revision mark
					collectRevisionData = false;
					//author 
					this->Isbt = FormatUtils::BytesToInt16( iter->Arguments, 1, iter->argumentsSize );
					//date
					this->Dttm = DateAndTime( ( iter->Arguments + 3 ), 4 );
				}
				break;

			case 0x0801:
				{
					//revision mark
					collectRevisionData = false;
				}
				break;

			case 0x4804:
				{
					//author
					this->Isbt = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
				}
				break;

			case 0x6805:
				{
					//date
					this->Dttm = DateAndTime( iter->Arguments, 4 );
				}
				break;

			case 0x0800:
				{
					//delete mark
					this->Type = Deleted;
				}
				break;

			case 0x6815:
				{
					this->RsidProp = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				}
				break;

			case 0x6816:
				{
					this->Rsid = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				}
				break;

			case 0x6817:
				{
					this->RsidDel = FormatUtils::BytesToInt32( iter->Arguments, 0, iter->argumentsSize );
				}
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
