#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/Cell.h>

namespace XLS
{


// Logical representation of Label record in BIFF8
class Label: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Label)
	BASE_OBJECT_DEFINE_CLASS_NAME(Label)
public:
	Label();
	~Label();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	int serialize(std::wostream & stream);

	static const ElementType	type = typeLabel;

	Cell				cell;
	XLUnicodeString 	st;	
//-----------------------------

	GlobalWorkbookInfoPtr	global_info_;
	int						isst_;
};


class CommentText: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CommentText)
	BASE_OBJECT_DEFINE_CLASS_NAME(CommentText)
public:
	CommentText();
	~CommentText();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	int serialize(std::wostream & stream);

	static const ElementType	type = typeLabel;

	Cell				cell;
	XLUnicodeString 	st;	
//-----------------------------

	GlobalWorkbookInfoPtr	global_info_;
	int						isst_;
};

} // namespace XLS

