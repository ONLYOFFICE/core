#ifndef _CPDOCCORE_ODF_ERRORS_H_
#define _CPDOCCORE_ODF_ERRORS_H_

#include <exception>
#include <boost/exception/all.hpp>

namespace cpdoccore { namespace odf { 

namespace errors {

struct error : 
    virtual std::exception,
    virtual boost::exception
{ 
};

struct invalid_attribute : virtual error {};

}

} }

#endif
