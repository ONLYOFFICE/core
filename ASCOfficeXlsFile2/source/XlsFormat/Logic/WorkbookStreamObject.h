#pragma once

#include "CompositeObject.h"
#include "SummaryInformationStream/Structures/CodePage.h"
#include "GlobalWorkbookInfo.h"

namespace XLS
{;

// Logical representation of Workbook stream
class WorkbookStreamObject: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(WorkbookStreamObject)
public:
	WorkbookStreamObject();
	WorkbookStreamObject(const unsigned __int16 code_page);

	~WorkbookStreamObject();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const unsigned __int16 DefaultCodePage = OLEPS::CodePage::DefaultCodePage;

	void set_code_page(const unsigned __int16 code_page);

	static const ElementType	type = typeWorkbookStreamObject;
	

	unsigned __int16 code_page_;
};

} // namespace XLS
