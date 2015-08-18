#pragma once

#include "CompositeObject.h"
#include "SummaryInformationStream/Structures/CodePageOle.h"
#include "GlobalWorkbookInfo.h"

namespace XLS
{;

// Logical representation of Workbook stream
class WorkbookStreamObject: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(WorkbookStreamObject)
public:
	WorkbookStreamObject();
	WorkbookStreamObject(const unsigned short code_page);

	~WorkbookStreamObject();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

    static const unsigned short DefaultCodePage = OLEPS::CodePageOle::DefaultCodePage;

	void set_code_page(const unsigned short code_page);

	static const ElementType	type = typeWorkbookStreamObject;
	
	BaseObjectPtr						m_GlobalsSubstream;

	std::vector<BaseObjectPtr>			m_WorksheetSubstream;
	std::vector<BaseObjectPtr>			m_ChartSheetSubstream;
	std::vector<BaseObjectPtr>			m_MacroSheetSubstream;
	
	unsigned short code_page_;
};

} // namespace XLS
