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

	void start_view	();
	void end_view	();
	void set_current_view(int id);

	void start_table(std::wstring name);
	void end_table	();

	void add_property(std::wstring name, std::wstring type, std::wstring value);
private:
	struct _table
	{
		std::wstring name;
		std::vector<office_element_ptr> content;
	};
	struct _view
	{
		std::vector<_table>				tables;
		std::vector<office_element_ptr>	content;
	};

	std::vector<_view>				views_;

	std::vector<office_element_ptr>	config_content_;
	odf_conversion_context*			odf_context_;

	int								current_table_;
	int								current_view_;

};


}
}
