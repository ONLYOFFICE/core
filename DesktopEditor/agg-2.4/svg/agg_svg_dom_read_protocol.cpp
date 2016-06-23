//-----------------------------------------------------------------------------
#include "agg_svg_dom_read_protocol.h"
#include "agg_svg_dom_storage.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    read_protocol::read_protocol(dom_storage const& dom) 
        :	m_storage(dom.storage())
		,	m_size(dom.storage().size())
		,	m_idx(0)
    {
    }
    //-------------------------------------------------------------------------
    tag_e read_protocol::read_tag()const
    {
        data_container::value_type val = m_storage[m_idx++];

		tag_e tag = tag_e(val);

        assert(tag < end_of_tags);

        return tag;
    }
    //-------------------------------------------------------------------------
    void read_protocol::element_bin(agg::int32u* el_idx, element_e* code) const
    {
        *el_idx = m_idx - 1;
        *code   = (element_e)m_storage[m_idx++];
    }
    //-------------------------------------------------------------------------
    void read_protocol::attribute_bin(data_container const** cont,attr_e* type, 
                                 agg::int32u* data_idx, agg::int32u* size)const
    {
		*type = (attr_e)m_storage[m_idx++];

		data_accessor_type accessor(m_storage);

		accessor.value_at(m_idx, size);
		m_idx += sizeof(agg::int32u);
		*data_idx = m_idx;
		m_idx += *size;
		*cont = &m_storage;
    }
    //-------------------------------------------------------------------------
    void read_protocol::attribute_bin_short(data_container const ** cont, 
                                 attr_e* type, 
                                 agg::int32u* data_idx, agg::int32u* size)const
    {
		*type = (attr_e)m_storage[m_idx++];

		data_accessor_type accessor(m_storage);

		*size = m_storage[m_idx++];
		*data_idx = m_idx;
		m_idx += *size;
		*cont = &m_storage;
    }
    //-------------------------------------------------------------------------
    void read_protocol::attribute_bin_byte(data_container const ** cont, 
                                           attr_e* type, 
                                           agg::int32u* data_idx)const
    {
        *type = (attr_e)m_storage[m_idx++];
        *cont = &m_storage;
        *data_idx = m_idx;
        ++m_idx;
    }
    //-------------------------------------------------------------------------
	void read_protocol::end_element() const
	{
	}
    //-------------------------------------------------------------------------
} 
}