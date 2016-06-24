#ifndef AGG_SVG_INDEXATION_INTERPRETER
#define AGG_SVG_INDEXATION_INTERPRETER
//-----------------------------------------------------------------------------
#include "agg_svg_defines.h"
#include "agg_svg_dom_storage.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    class indexation_interpreter
    {
    public:
        //---------------------------------------------------------------------
		explicit indexation_interpreter(dom_storage::map_type& map) 
			: 
			m_attr2element_map(map)
        {
        }
        //---------------------------------------------------------------------
        bool start_element(const dom_storage&, const element_data& el)
        {
            for (unsigned i = 0; i < el.num_attr; ++i)
            {
                if (el.attr[i].code == attr_id)
                {
					dom_storage::element_location eloc;
					eloc.code = el.code;
					eloc.idx  = el.index;

                    m_attr2element_map.insert(el.attr[i].data, eloc);
                    break;
                }
            }
            return true; 
        }
        //---------------------------------------------------------------------
        bool end_element(const dom_storage&, const element_data&)
        {
            return true;
        }
        //---------------------------------------------------------------------

    private:
        dom_storage::map_type& m_attr2element_map;
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_INDEXATION_INTERPRETER

