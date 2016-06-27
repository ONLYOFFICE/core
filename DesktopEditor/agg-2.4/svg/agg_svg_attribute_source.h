//-----------------------------------------------------------------------------
#ifndef AGG_SVG_ATTRIBUTE_SOURCE_INCLUDED
#define AGG_SVG_ATTRIBUTE_SOURCE_INCLUDED
//-----------------------------------------------------------------------------
#include <cstring> // for memcpy
#include "agg_svg_basics.h"
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    class attribute_byte
    {
    public:
        //---------------------------------------------------------------------
        attribute_byte(attr_e attr, agg::int8u data) 
        {
            m_storage[0] = tag_attribute_bin_byte;
            m_storage[1] = attr;
            m_storage[2] = data;
        }
        //---------------------------------------------------------------------
        const agg::int8u* head() const { return m_storage; }
        //---------------------------------------------------------------------
        agg::int32u head_size() const  { return buf_size - 1;  }
        //---------------------------------------------------------------------
        const agg::int8u* data() const { return &m_storage[2]; }
        //---------------------------------------------------------------------
        agg::int32u data_size() const  { return 1; }       
        //---------------------------------------------------------------------
    private:
        //---------------------------------------------------------------------
        enum 
        {
            buf_size =  1 // tag
                      + 1 // attr_e
                      + 1 // data 
        };
        //---------------------------------------------------------------------
        agg::int8u m_storage[buf_size];
    };
    //-------------------------------------------------------------------------
    class attribute_bin
    {
    public:
        //---------------------------------------------------------------------
        attribute_bin(attr_e        attr, 
                      const void *  data, 
                      agg::int32u   size) 
            
            : m_data(static_cast<const agg::int8u*>(data)), 
              m_size(size),
              m_with_type(false)
        {
            fill_head(attr);
        }
        //---------------------------------------------------------------------
        attribute_bin(attr_e        attr, 
                      agg::int8u    type, 
                      const void *  data, 
                      agg::int32u   size) 

            : m_data(static_cast<const agg::int8u*>(data)), 
              m_size(size + 1),
              m_with_type(true)
        {
            fill_head(attr);
            m_storage[2 + sizeof(agg::int32u)] = type;
            --m_size;
        }
        //---------------------------------------------------------------------
        const agg::int8u* head() const { return m_storage; }
        //---------------------------------------------------------------------
        agg::int32u head_size() const  
        { 
            return m_with_type ? buf_size : buf_size - 1; 
        }
        //---------------------------------------------------------------------
        const agg::int8u* data() const { return m_data; }
        //---------------------------------------------------------------------
        agg::int32u data_size() const  { return m_size; }       
        //---------------------------------------------------------------------
    private:
        //---------------------------------------------------------------------
        void fill_head(attr_e attr)
        {
            m_storage[0] = tag_attribute_bin;
            m_storage[1] = attr;
            memcpy(&m_storage[2], &m_size, sizeof(agg::int32u));
        }
        //---------------------------------------------------------------------
        enum 
        {
            buf_size =  1                   // tag
                      + 1                   // attr_e
                      + sizeof(agg::int32u) // size 
                      + 1                   // type
        };
        //---------------------------------------------------------------------
        agg::int8u          m_storage[buf_size];
        const agg::int8u*   m_data;
        agg::int32u         m_size;
        bool                m_with_type;
    };
    //-------------------------------------------------------------------------
    class attribute_bin_short
    {
    public:
        //---------------------------------------------------------------------
        attribute_bin_short(attr_e attr, const void* data, agg::int8u size)
            : m_data(static_cast<const agg::int8u*>(data)),
              m_size(size),
              m_with_type(false)
        {
            fill_head(attr);
        }
        //---------------------------------------------------------------------
        attribute_bin_short(attr_e attr, agg::int8u type, const void* data, agg::int8u size)
            : m_data(static_cast<const agg::int8u*>(data)),
              m_size(size + 1),
              m_with_type(true)
        {
            fill_head(attr);
            m_storage[3] = type;
            --m_size;
        }
        //---------------------------------------------------------------------
        const agg::int8u* head() const { return m_storage; }
        //---------------------------------------------------------------------
        agg::int32u head_size() const  
        { 
            return m_with_type ? buf_size : buf_size - 1; 
        }
        //---------------------------------------------------------------------
        const agg::int8u* data() const { return m_data; }
        //---------------------------------------------------------------------
        agg::int32u data_size() const  { return m_size; }       
        //---------------------------------------------------------------------
    private:
        //---------------------------------------------------------------------
        void fill_head(attr_e attr)
        {
            m_storage[0] = tag_attribute_bin_short;
            m_storage[1] = attr;
            m_storage[2] = m_size;
        }
        //---------------------------------------------------------------------
        enum 
        {
            buf_size =  1  // tag
                      + 1  // attr_e
                      + 1  // size 
                      + 1  // type
        };
        //---------------------------------------------------------------------
        agg::int8u          m_storage[buf_size];
        const agg::int8u*   m_data;
        agg::int32u         m_size;
        bool                m_with_type;
    };
    //-------------------------------------------------------------------------      
 
} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_ATTRIBUTE_SOURCE_INCLUDED
