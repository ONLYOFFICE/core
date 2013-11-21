#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/PropertyBagStore.h>
#include <XLS_logic/Biff_structures/EnhancedProtection.h>

namespace XLS
{;


// Logical representation of FeatHdr record in BIFF8
class FeatHdr: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FeatHdr)
	BASE_OBJECT_DEFINE_CLASS_NAME(FeatHdr)
public:
	FeatHdr(const bool is_called_from_Globals);
	~FeatHdr();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BOOL is_contained_in_Globals;
	SharedFeatureType isf;
	BIFF_DWORD cbHdrData;
	EnhancedProtection protection;
	OSHARED::PropertyBagStore prop;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(isf)
		BO_ATTRIB_MARKUP_ATTRIB(cbHdrData)
		if(cbHdrData)
		{
			switch(isf)
			{
			case SharedFeatureType::ISFPROTECTION:
				BO_ATTRIB_MARKUP_COMPLEX(protection)
				break;
			case SharedFeatureType::ISFFACTOID:
				if(is_contained_in_Globals)
				{
					BO_ATTRIB_MARKUP_COMPLEX(prop)
				}
				break;
			}
		}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

