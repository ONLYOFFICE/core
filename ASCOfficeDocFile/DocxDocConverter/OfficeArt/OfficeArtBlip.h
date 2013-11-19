#pragma once

#include "OfficeArtBStoreContainerFileBlock.h"

namespace OfficeArt
{
	class OfficeArtBlip : public OfficeArtBStoreContainerFileBlock 
	{
	public:
		explicit OfficeArtBlip (OfficeArtRecordHeader oHeader) : OfficeArtBStoreContainerFileBlock (oHeader) { }
		virtual ~OfficeArtBlip () {}
	};
}