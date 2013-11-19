#ifndef _CPDOCCORE_OOX_CONVERSION_CONTEXT_H_INCLUDED_
#define _CPDOCCORE_OOX_CONVERSION_CONTEXT_H_INCLUDED_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace cpdoccore { 
namespace oox {

class streams_man
{
public:
    streams_man(std::wostream & strm) : stream_(strm)
    {
    }

    std::wostream & get() { return stream_; }

    static boost::shared_ptr<streams_man> create(std::wostream & strm)
    {
        return boost::shared_ptr<streams_man>( new streams_man(strm) );
    }

private:
    std::wostream & stream_;
};

}
}

#endif
