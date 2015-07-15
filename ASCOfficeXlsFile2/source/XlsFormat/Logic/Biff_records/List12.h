#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FrtHeader.h>
#include <Logic/Biff_structures/List12BlockLevel.h>
#include <Logic/Biff_structures/List12TableStyleClientInfo.h>
#include <Logic/Biff_structures/List12DisplayName.h>

namespace XLS
{;


// Logical representation of List12 record in BIFF8
class List12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(List12)
	BASE_OBJECT_DEFINE_CLASS_NAME(List12)
public:
	List12();
	~List12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

public:
	FrtHeader frtHeader;
	BIFF_WORD lsd;
	BIFF_DWORD idList;

	List12BlockLevel rgbList12BlockLevel;
	List12TableStyleClientInfo rgbList12TableStyleClientInfo;
	List12DisplayName rgbList12DisplayName;

public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_COMPLEX(frtHeader)
		//BO_ATTRIB_MARKUP_ATTRIB(lsd)
		//BO_ATTRIB_MARKUP_ATTRIB(idList)

		//switch (lsd)
		//{
		//	case 0:
		//		//BO_ATTRIB_MARKUP_COMPLEX(rgbList12BlockLevel)
		//		break;
		//	case 1:
		//		//BO_ATTRIB_MARKUP_COMPLEX(rgbList12TableStyleClientInfo)
		//		break;
		//	case 2:
		//		//BO_ATTRIB_MARKUP_COMPLEX(rgbList12DisplayName)
		//		break;
		//}

	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

