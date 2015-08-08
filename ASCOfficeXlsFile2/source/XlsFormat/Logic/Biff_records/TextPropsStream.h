#pragma once

#include <Logic/Biff_structures/FrtHeader.h>
#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of TextPropsStream record in BIFF8
class TextPropsStream: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TextPropsStream)
	BASE_OBJECT_DEFINE_CLASS_NAME(TextPropsStream)
public:
	TextPropsStream();
	~TextPropsStream();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeTextPropsStream;

	FrtHeader		frtHeader;
	unsigned int	dwChecksum;

	std::string		xml_;
};

} // namespace XLS

