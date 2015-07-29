
#include <string>
#include <boost/foreach.hpp>
#include <cmath>

#include <boost/optional/optional_io.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

namespace cpdoccore { 

namespace {

struct color
{
    color(int r, int g, int b): r_(r), g_(g), b_(b) {}
    color(int r, int g, int b, std::wstring const & name): r_(r), g_(g), b_(b), name_(name) {}
    int r_,g_,b_;
    std::wstring name_;
};

color black(0,0,0, L"Black");
color green(0,255,0, L"Green");
color white(255,255,255, L"White");

color blue(0,0,255, L"Blue");
color magenta(255,0,255, L"Magenta");
color yellow(255,255,0, L"Yellow");

color cyan(0,0,0, L"Cyan");
color red(255,0,0, L"Red");

color colors[] = {black, green, white, blue, magenta, yellow, cyan, red };

template <class V> V sqr(V v){ return v*v; }

double color_dist(color const & c1, color const & c2)
{
    return sqrt((double)(sqr(c1.r_ - c2.r_) + sqr(c1.g_ - c2.g_) + sqr(c1.b_ - c2.b_)));
}

}

std::wstring RGBToString(int r, int g, int b)
{
    std::wstring result = L"";
    color v(r, g, b);
    double minDist = (std::numeric_limits<double>::max)();
    
    
    BOOST_FOREACH(color const & c, colors)
    {
        double dist = color_dist(v, c);
        if (dist < minDist)
        {
            minDist = dist;
            result = c.name_;
        }
    }

    return result;
}

template <class C>
bool hex2int(C i1, C i2, int & res)
{
    std::wstringstream s;    
    int res_ = 0;
    if ( (s << std::wstring(i1, i2)) &&  (s >> std::hex >> res_) && s.eof() )
    {
        res = res_;
        return true;
    }
    else
        return false;
}

bool HEXStringToRGB(std::wstring const & str, int &r, int &g, int &b)
{
    if (str.length() != 6)
        return false;

    try 
    {
        int r_, g_, b_;
        if (hex2int(str.begin(), str.begin()+2, r_) && 
            hex2int(str.begin()+2, str.begin()+4, g_) &&
            hex2int(str.begin()+4, str.begin()+6, b_)
            )
        {
            r = r_; g = g_; b = b_;
            return true;
        }
    }
    catch(...)
    {     
    }
    return false;
}

}
