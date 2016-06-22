#pragma once

#include <string>
#include <vector>
#include <map>

#include <cpdoccore/CPSharedPtr.h>


namespace cpdoccore {
namespace odf_writer {

class odf_conversion_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class odf_settings_context;
typedef shared_ptr<odf_settings_context>::Type odf_settings_context_ptr;

class odf_settings_context
{
public:
    odf_settings_context();
	void set_odf_context(odf_conversion_context * Context);

	void process_office_settings(office_element_ptr root );

private:
	struct views
	{
		std::map<std::wstring, std::vector<office_element_ptr>> tables;
		std::vector<office_element_ptr>							content;
	}views_;

	std::vector<office_element_ptr>	config_content_;
	odf_conversion_context*			odf_context_;

};


}
}
