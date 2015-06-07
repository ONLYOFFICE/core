#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf { 

class length
{
public:
    enum unit 
    {
        none,
        cm,
        mm,
        inch,
        px,
        pt,
        rel,
        emu
    };

public:
    length(double v = 0.0, unit u = none) : value_(v), unit_(u)
    {}

    double get_value() const 
    { 
        return value_; 
    }
    
    unit get_unit() const 
    { 
        return unit_; 
    }

    double get_value_unit(unit Unit) const;

    static length parse(const std::wstring & Str);

private:
    double value_;
    unit unit_;
};

std::wostream & operator<< (std::wostream & _Wostream, const length::unit _Unit);
std::wostream & operator<< (std::wostream & _Wostream, const length & _Length);

length operator+ (length & _Length1, double val);
length operator+ (length & _Length1, length & _Length2);
length operator- (length & _Length1, length & _Length2);
length operator/ (length & _Length1, double val);
length operator* (length & _Length1, double val);
bool operator== (length & _Length1, length & _Length2);

typedef length coordinate;

}

APPLY_PARSE_XML_ATTRIBUTES(odf::length);

}

