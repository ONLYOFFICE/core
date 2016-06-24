#include "agg_svg_parse_real.h"

namespace agg
{
namespace svg
{

    namespace aux
    {
//-----------------------------------------------------------------------------
struct contains_helper
{
       contains_helper()
       {
               init_mask(m_set_mask, s_set);
       }

       bool operator()(unsigned c) const
       {
           return (m_set_mask[(c >> 3) & (256/8-1)] & (1 << (c & 7))) != 0;
       }

private:
       static void init_mask(unsigned char* mask, const unsigned char* char_set);

       unsigned char m_set_mask[256/8];

       static const unsigned char s_set[];

};

const unsigned char contains_helper::s_set[] = "0123456789";
//-----------------------------------------------------------------------------
void contains_helper::init_mask(unsigned char* mask, const unsigned
char* char_set)
{
   memset(mask, 0, 256/8);
   while(*char_set)
   {
       unsigned c = unsigned(*char_set++) & 0xFF;
       mask[c >> 3] |= 1 << (c & 7);
   }
}
    }

    namespace detail
{
bool isdigit(unsigned c)
{
    static aux::contains_helper helper;

       return helper(c);
}
}
}
}