#pragma once

#include "OfficeArtRecordHeader.h"
#include "IOfficeArtRecord.h"

namespace OfficeArt
{
	class OfficeArtBStoreContainerFileBlock: public IOfficeArtRecord 
	{
	public:

		explicit OfficeArtBStoreContainerFileBlock( OfficeArtRecordHeader _rh ): rh(_rh) {}
		virtual ~OfficeArtBStoreContainerFileBlock() {}

	protected:

		OfficeArtRecordHeader rh;
	};

	typedef NSCommon::smart_ptr<OfficeArtBStoreContainerFileBlock> OfficeArtBStoreContainerFileBlockPtr;
}