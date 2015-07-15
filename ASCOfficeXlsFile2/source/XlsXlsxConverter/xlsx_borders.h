#pragma once

#include <iosfwd>
#include <boost/scoped_ptr.hpp>

namespace oox {

    class xlsx_borders
    {
    public:
        xlsx_borders();
        ~xlsx_borders();

        size_t size() const;
        size_t borderId();
        size_t borderId(bool & is_default);

        void xlsx_serialize(std::wostream & _Wostream);

    private:
        class Impl;
		boost::scoped_ptr<Impl> impl_;

     };

    void xlsx_serialize(std::wostream & _Wostream, xlsx_borders & borders);

}
