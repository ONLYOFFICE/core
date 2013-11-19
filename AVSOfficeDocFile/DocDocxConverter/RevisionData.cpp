#include "stdafx.h"
#include "RevisionData.h"

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
		this->Changes = new list<SinglePropertyModifier>();
	}

	/*========================================================================================================*/

	/// Collects the revision data of a CHPX
	RevisionData::RevisionData( CharacterPropertyExceptions* chpx ):
	Dttm(), Isbt(0), Type(NoRevision), Changes(NULL), RsidDel(0), RsidProp(0), Rsid(0)
	{
		bool collectRevisionData = true;
		this->Changes = new list<SinglePropertyModifier>();

		for ( list<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
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