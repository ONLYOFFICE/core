#ifndef _CPDOCCORE_XLSX_BORDERS_H_INCLUDED_da8e414a
#define _CPDOCCORE_XLSX_BORDERS_H_INCLUDED_da8e414a

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
namespace odf_reader {
class style_table_cell_properties_attlist;
}
}
                    
namespace cpdoccore {
namespace oox {

    class xlsx_borders
    {
    public:
        xlsx_borders();
        ~xlsx_borders();

        size_t size() const;
        size_t borderId(const odf_reader::style_table_cell_properties_attlist * cellProp);
        size_t borderId(const odf_reader::style_table_cell_properties_attlist * cellProp, bool & is_default);

        void xlsx_serialize(std::wostream & _Wostream);

    private:
        class Impl;
        _CP_SCOPED_PTR(Impl) impl_;

     };

    void xlsx_serialize(std::wostream & _Wostream, xlsx_borders & borders);

}
}

#endif
