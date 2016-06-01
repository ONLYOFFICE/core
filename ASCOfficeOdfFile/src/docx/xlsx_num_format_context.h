#pragma once

#include <string>
#include <iosfwd>
#include <boost/noncopyable.hpp>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
	namespace odf_reader
	{
		class odf_read_context;
	}
namespace oox {

class num_format_context : boost::noncopyable
{
public:
	num_format_context(odf_reader::odf_read_context & odfContext);
    ~num_format_context();

public:
    void start_format(const std::wstring & style_name);
    std::wostream & output();
    void end_format();

    std::wstring get_last_format() const;

    void start_complex_format();
    void add_format(std::wstring const & cond, std::wstring const & format);
    void end_complex_format();

	//std::wstring default_data_style(int type);
	odf_reader::odf_read_context & odf_context_;
private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;

    class Impl2;
    _CP_SCOPED_PTR(Impl2) impl2_;
};

}
}
