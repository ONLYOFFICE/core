#ifndef AGG_SVG_DOM_STORAGE_READ_PROTOCOL_INCLUDE
#define AGG_SVG_DOM_STORAGE_READ_PROTOCOL_INCLUDE
//-----------------------------------------------------------------------------
#include "agg_svg_basics.h"
#include "agg_svg_defines.h"
#include "agg_svg_tags.h"
#include "agg_svg_iomemstream.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    class dom_storage;
    //-------------------------------------------------------------------------
    class read_protocol
    {
    private:
        read_protocol(read_protocol const&);
        read_protocol operator=(read_protocol const&);

    public:
        //---------------------------------------------------------------------
        explicit read_protocol(dom_storage const&);
        //---------------------------------------------------------------------
        void rewind(agg::int32u idx);
        //---------------------------------------------------------------------
        bool end()const;
        //---------------------------------------------------------------------
        void unread(unsigned offset = 1) const;
        //---------------------------------------------------------------------
        agg::int32u current_location() const;
        //---------------------------------------------------------------------
        tag_e read_tag()const;
        //---------------------------------------------------------------------
        void element_bin(agg::int32u*  idx, element_e* code)const;
        //---------------------------------------------------------------------
        void attribute_bin(data_container const ** cont, attr_e* type, 
                           agg::int32u* data_idx, agg::int32u* size) const;
        //---------------------------------------------------------------------
        void attribute_bin_short(data_container const ** cont, attr_e* type, 
                                 agg::int32u* data_idx,agg::int32u* size)const;
        //---------------------------------------------------------------------
        void attribute_bin_byte(data_container const ** cont, attr_e* type, 
                                agg::int32u* data_idx)const;
        //---------------------------------------------------------------------
		void end_element() const;
        //---------------------------------------------------------------------
    private:
        const data_container & m_storage;

        agg::int32u         m_size;
        mutable agg::int32u m_idx;
    };
	//-------------------------------------------------------------------------
	inline void read_protocol::rewind(agg::int32u idx) 
	{ 
		m_idx = idx; 
	}
	//-------------------------------------------------------------------------
    inline bool read_protocol::end()const 
	{ 
		return m_idx >= m_size; 
	}
	//-------------------------------------------------------------------------
    inline void read_protocol::unread(unsigned offset) const
    {
        m_idx -= offset;
    }
	//-------------------------------------------------------------------------
    inline agg::int32u read_protocol::current_location() const 
    {
        return m_idx;
    }

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_DOM_STORAGE_READ_PROTOCOL_INCLUDE

