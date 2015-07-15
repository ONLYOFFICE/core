#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

#include "progressCallback.h"

namespace oox
{
	class xlsx_conversion_context;
	namespace package
	{
		class xlsx_document;
	}

}
namespace XLS
{
	class BaseObject;
	class WorkbookStreamObject;
	class WorksheetSubstream;
	class GlobalsSubstream;
	class GlobalWorkbookInfo;
	class FORMATTING;
	
	typedef boost::shared_ptr<BaseObject> BaseObjectPtr;
	typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;
}

class XlsConverter
{
public:
	XlsConverter(const std::wstring & path, const ProgressCallback* ffCallBack);
	~XlsConverter() ;

	const	ProgressCallback* pCallBack;
	short	bUserStopConvert;
	bool	UpdateProgress(long nComplete);

	void convertDocument();
	void write(const std::wstring & path);

	void convert(XLS::BaseObject	*xls_unknown);

private:		
	oox::package::xlsx_document		*output_document;
    oox::xlsx_conversion_context	*xlsx_context;

	XLS::BaseObjectPtr				xls_document;	
	XLS::GlobalWorkbookInfoPtr		xls_global_info;

	void convert(XLS::WorkbookStreamObject* woorkbook);
	void convert(XLS::WorksheetSubstream* sheet);
	void convert(XLS::GlobalsSubstream* elem);
	void convert(XLS::FORMATTING* formating);
	

};
