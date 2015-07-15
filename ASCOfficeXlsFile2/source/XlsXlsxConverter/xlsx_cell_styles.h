#pragma once

#include <boost/scoped_ptr.hpp>

namespace oox {

class xlsx_cell_styles
{
public:
    xlsx_cell_styles();
    ~xlsx_cell_styles();

public:
    void xlsx_serialize(std::wostream & _Wostream) const;

private:
    class Impl;
	boost::scoped_ptr<Impl> impl_;
};

}

