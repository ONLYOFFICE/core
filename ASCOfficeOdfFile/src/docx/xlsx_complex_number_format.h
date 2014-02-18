#ifndef CPDOCCORE_COMPLEX_NUMBER_FORMAT
#define CPDOCCORE_COMPLEX_NUMBER_FORMAT

#include <string>
#include <boost/algorithm/string.hpp>  
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
namespace oox {

class complex_number_format
{
public:
    complex_number_format();
    ~complex_number_format();
    void reset();

    // получить результирующий формат
    std::wstring result_value();

    // добавить условный формат числа
    void add_format(std::wstring const & condition, std::wstring const & format);

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}

#endif
