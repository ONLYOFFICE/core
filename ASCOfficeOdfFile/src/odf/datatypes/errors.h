#pragma once

#include <exception>
#include <boost/exception/all.hpp>

namespace cpdoccore { namespace odf_types { 

namespace errors {

struct error : 
    virtual std::exception,
    virtual boost::exception
{ 
};

struct invalid_attribute : virtual error {};

}

} }

