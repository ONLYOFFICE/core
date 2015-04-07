#pragma once

#include "OfficeArtBStoreContainerFileBlock.h"

#include <string>
#include <vector>

namespace OfficeArt
{
	class OfficeArtBlip : public OfficeArtBStoreContainerFileBlock 
	{
	public:
		explicit OfficeArtBlip (OfficeArtRecordHeader oHeader) : OfficeArtBStoreContainerFileBlock (oHeader) { }
		virtual ~OfficeArtBlip () {}
	};
}