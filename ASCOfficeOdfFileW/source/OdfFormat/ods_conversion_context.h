#pragma once

#include "odf_conversion_context.h"

namespace cpdoccore { 
namespace odf {


class ods_conversion_context : public odf_conversion_context
{
public:
	ods_conversion_context(package::odf_document * outputDocument) : odf_conversion_context(outputDocument){}
	
	virtual void start_document();

	void start_sheet(std::wstring & name);
	void end_sheet();
};


}
}