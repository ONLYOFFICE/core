#pragma once

#include <string>
#include <vector>

namespace cpdoccore { 
namespace odf {

class style_ref
{
public:
    style_ref() 
    {
    } 

    style_ref(const ::std::wstring & StyleName) : style_name_(StyleName) 
    {
    }

    bool empty() const
    {
        return style_name_.empty();
    }

    const ::std::wstring & style_name() const 
    { 
        return style_name_;
    }

    ::std::wstring & style_name()
    { 
        return style_name_;
    }

    
private:
    ::std::wstring style_name_;

};
std::wostream & operator << (std::wostream & _Wostream, const style_ref & _styleReft);

typedef ::std::vector<style_ref> style_ref_array;

inline std::wistream & operator >> (std::wistream & _Istream, style_ref & _styleRef)
{
    _Istream >> _styleRef.style_name();
    return _Istream;
}

}
}
