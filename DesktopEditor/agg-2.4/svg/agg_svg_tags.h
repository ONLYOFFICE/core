#ifndef AGG_SVG_TAGS_INCLUDE
#define AGG_SVG_TAGS_INCLUDE

namespace agg
{
namespace svg
{
    enum tag_e
    {
        tag_end_element = 0, 

        tag_element_bin,    

        tag_attribute_bin,        
        tag_attribute_bin_short,  
        tag_attribute_bin_byte,  

        end_of_tags,
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_TAGS_INCLUDE


