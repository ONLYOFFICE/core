#pragma once

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

class styles_context : boost::noncopyable
{
public:
    void start();
   
	std::wostream & text_style();
    std::wstringstream & paragraph_style();
    std::wostream & table_style();
    std::wostream & list_style();

    void docx_serialize_text_style(std::wostream & strm);
    void docx_serialize_table_style(std::wostream & strm);

private:
    std::wstringstream list_style_;
    std::wstringstream text_style_;
    std::wstringstream paragraph_style_;
    std::wstringstream table_style_;
};


}
}

