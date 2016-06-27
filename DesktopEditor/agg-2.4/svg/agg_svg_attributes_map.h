#ifndef AGG_SVG_ATTRIBUTES_MAP
#define AGG_SVG_ATTRIBUTES_MAP
//-----------------------------------------------------------------------------
#include "agg_svg_defines.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    class attributes_map
    {
    public:
        //---------------------------------------------------------------------
        attributes_map();
        //---------------------------------------------------------------------
        attr_e       get_type(const char * name);
        //---------------------------------------------------------------------
        const char * get_name(unsigned attr);
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_ATTRIBUTES_MAP
