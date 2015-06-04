#ifndef _CPDOCCORE_XLSX_DEFINED_NAMES_H_
#define _CPDOCCORE_XLSX_DEFINED_NAMES_H_

#include <iosfwd>
#include <string>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
namespace oox {

    class xlsx_defined_names
    {
    public:
        xlsx_defined_names();
        ~xlsx_defined_names();

    public:
        void add(std::wstring const & name, std::wstring const & ref);
        void xlsx_serialize(std::wostream & _Wostream);

    private:
        class Impl;
        _CP_SCOPED_PTR(Impl) impl_;


    };


}
}


#endif
