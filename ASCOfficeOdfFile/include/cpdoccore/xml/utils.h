#pragma once

#include <string>

namespace cpdoccore {
namespace xml {

namespace utils {

	namespace details {
		std::wstring replace_text_to_xml_ImplRegEx(const std::wstring & Text);
		std::string replace_text_to_xml_ImplRegEx(const std::string & Text);

		std::wstring replace_xml_to_text_ImplRegEx(const std::wstring & Text);
		std::string replace_xml_to_text_ImplRegEx(const std::string & Text);

		std::wstring replace_text_to_xml_ImplReplace(const std::wstring & Text);
		std::string replace_text_to_xml_ImplReplace(const std::string & Text);

		std::wstring replace_amp_text_to_xml_ImplReplace(const std::wstring & Text);
		std::string replace_amp_text_to_xml_ImplReplace(const std::string & Text);

		std::wstring replace_xml_to_text_ImplReplace(const std::wstring & Text);
		std::string replace_xml_to_text_ImplReplace(const std::string & Text);
	}

	std::wstring replace_text_to_xml(const std::wstring & Text);
	std::string replace_text_to_xml(const std::string & Text);

	std::wstring replace_amp_text_to_xml(const std::wstring & Text);

	std::wstring replace_lt_gt(const std::wstring & Text);

	std::wstring replace_xml_to_text(const std::wstring & Text);
	std::string replace_xml_to_text(const std::string & Text);
}

}
}
