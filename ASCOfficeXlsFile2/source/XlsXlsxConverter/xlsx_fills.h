#pragma once

#include <string>
#include "xlsx_fill.h"

#include <iosfwd>

#include <boost/scoped_ptr.hpp>

namespace oox {

    class xlsx_fills
    {
    public:
        xlsx_fills();
        ~xlsx_fills();
    public:
        size_t size() const;
        size_t fillId(bool  default_set);

        size_t fillId(bool default_set,bool & is_default);

        void xlsx_serialize(std::wostream & _Wostream) const;

    private:
        class Impl;
		boost::scoped_ptr<Impl> impl_;

    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_fills & fills);
}

