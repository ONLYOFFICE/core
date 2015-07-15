#pragma once

#include <string>
#include <iosfwd>
#include <boost/scoped_ptr.hpp>


namespace oox {

class xlsx_shared_strings: boost::noncopyable
{
public:
    xlsx_shared_strings();
    ~xlsx_shared_strings();

public:
    void xlsx_serialize(std::wostream & _Wostream) const;
    size_t add(const std::wstring & content);

private:
    class Impl;
	boost::scoped_ptr<Impl> impl_;
};

}
