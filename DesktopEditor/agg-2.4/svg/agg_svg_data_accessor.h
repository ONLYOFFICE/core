#ifndef AGG_SVG_DATA_ACCESSOR_INCLUDED
#define AGG_SVG_DATA_ACCESSOR_INCLUDED

#include "agg_svg_basics.h"

namespace agg
{
namespace svg
{
	//-------------------------------------------------------------------------
    // Data accessor. An analog of a simple const pointer or an iterator used with
    // plain arrays or standard containers. 
    template<class Container> class data_accessor
    {
    public:
        typedef Container container_type;
        typedef typename container_type::value_type value_type;
        //------------------------------------------------------------------------
        data_accessor() : m_container(0), m_pos(0), m_size(0) {}
        data_accessor(const container_type& c) :
            m_container(&c), m_pos(0), m_size(c.size()) {}
        data_accessor(const container_type& c, unsigned pos, unsigned size_) :
            m_container(&c), m_pos(pos), m_size(size_) {}

        void reset()
        {
            m_container = 0;
            m_pos       = 0;
            m_size      = 0;
        }

        void init(const container_type& c)
        {
            m_container = &c;
            m_pos       = 0;
            m_size      = c.size();
        }

        void init(const container_type& c, unsigned pos, unsigned size_)
        {
            m_container = &c;
            m_pos       = pos;
            m_size      = size_;
        }

        //------------------------------------------------------------------------
        const container_type*  container() const { return m_container; }

        unsigned   pos()         const { return m_pos;  }
        unsigned   size()        const { return m_size; }
        value_type value()       const { return m_container->value_at(m_pos); }
        value_type operator * () const { return m_container->value_at(m_pos); }

        value_type operator [] (unsigned i) const 
        { 
            return m_container->value_at(m_pos + i);
        }

        value_type at(unsigned i) const 
        { 
            return m_container->value_at(m_pos + i);
        }

        void inc()
        {
            ++m_pos;
            --m_size;
        }

        void operator ++ ()
        {
            ++m_pos;
            --m_size;
        }

        void operator += (unsigned n)
        {
            m_pos += n;
            m_size -= n; 
        }

        value_type read_value()
        { 
            --m_size;
            return m_container->value_at(m_pos++); 
        }

        //------------------------------------------------------------------------
        template <class T>
        double units_value(const T& attr) const
        {
            return attr.conv_units(double(coord_at(0)), 
                                   units2_e(int8u_at(sizeof(coord_type))));
        }

        //------------------------------------------------------------------------
        void value_at(unsigned i, char* v) const
        { 
            *v = (char)at(i);
        }

        void value_at(unsigned i, agg::int8* v) const
        { 
            *v = (agg::int8)at(i);
        }

        void value_at(unsigned i, agg::int8u* v) const
        { 
            *v = (agg::int8u)at(i);
        }

        void value_at(unsigned i, agg::int16* v) const
        { 
            ((agg::int8u*)v)[0] = at(i+0);
            ((agg::int8u*)v)[1] = at(i+1);
        }

        void value_at(unsigned i, agg::int16u* v) const
        { 
            ((agg::int8u*)v)[0] = at(i+0);
            ((agg::int8u*)v)[1] = at(i+1);
        }

        void value_at(unsigned i, agg::int32* v) const
        { 
            ((agg::int8u*)v)[0] = at(i+0);
            ((agg::int8u*)v)[1] = at(i+1);
            ((agg::int8u*)v)[2] = at(i+2);
            ((agg::int8u*)v)[3] = at(i+3);
        }

        void value_at(unsigned i, agg::int32u* v) const
        {
            ((agg::int8u*)v)[0] = at(i+0);
            ((agg::int8u*)v)[1] = at(i+1);
            ((agg::int8u*)v)[2] = at(i+2);
            ((agg::int8u*)v)[3] = at(i+3);
        }

        void value_at(unsigned i, float* v) const
        {
            ((agg::int8u*)v)[0] = at(i+0);
            ((agg::int8u*)v)[1] = at(i+1);
            ((agg::int8u*)v)[2] = at(i+2);
            ((agg::int8u*)v)[3] = at(i+3);
        }

        void value_at(unsigned i, double* v) const
        {
            ((agg::int8u*)v)[0] = at(i+0);
            ((agg::int8u*)v)[1] = at(i+1);
            ((agg::int8u*)v)[2] = at(i+2);
            ((agg::int8u*)v)[3] = at(i+3);
            ((agg::int8u*)v)[4] = at(i+4);
            ((agg::int8u*)v)[5] = at(i+5);
            ((agg::int8u*)v)[6] = at(i+6);
            ((agg::int8u*)v)[7] = at(i+7);
        }

        void value_at(unsigned i, agg::rgba8* v) const
        {
            v->r = at(i+0);
            v->g = at(i+1);
            v->b = at(i+2);
            v->a = at(i+3);
        }


        template<class T> void read_value(T* v)
        {
            value_at(0, v);
            m_pos += sizeof(T);
            m_size -= sizeof(T);
        }

        agg::int8   int8_at(unsigned i)   const { return (agg::int8)at(i);  }
        agg::int8u  int8u_at(unsigned i)  const { return (agg::int8u)at(i); }
        agg::int16  int16_at(unsigned i)  const { agg::int16  v; value_at(i, &v); return v; }
        agg::int16u int16u_at(unsigned i) const { agg::int16u v; value_at(i, &v); return v; }
        agg::int32  int32_at(unsigned i)  const { agg::int32  v; value_at(i, &v); return v; }
        agg::int32u int32u_at(unsigned i) const { agg::int32u v; value_at(i, &v); return v; }
        coord_type  coord_at(unsigned i)  const { coord_type  v; value_at(i, &v); return v; }
        agg::rgba8  color_at(unsigned i)  const { agg::rgba8  v; value_at(i, &v); return v; }

        agg::int8   read_int8()   { return (agg::int8)read_value();  }
        agg::int8u  read_int8u()  { return (agg::int8u)read_value(); }
        agg::int16  read_int16()  { agg::int16  v; read_value(&v); return v; }
        agg::int16u read_int16u() { agg::int16u v; read_value(&v); return v; }
        agg::int32  read_int32()  { agg::int32  v; read_value(&v); return v; }
        agg::int32u read_int32u() { agg::int32u v; read_value(&v); return v; }
        coord_type  read_coord()  { coord_type  v; read_value(&v); return v; }
        agg::rgba8  read_rgba8()  { agg::rgba8  v; read_value(&v); return v; }

    private:
        const container_type* m_container;
        unsigned      m_pos;
        unsigned      m_size;
    };

    //-------------------------------------------------------------------------
    template<class T1, class T2>
    inline 
    bool operator<(data_accessor<T1> const & lhs, data_accessor<T2> const & rhs)
    {
        const agg::int32u count1 = lhs.size();
        const agg::int32u count2 = rhs.size();
        agg::int32u i = 0;
        agg::int32u j = 0;

        for (; i < count1 && j < count2; ++i, ++j)
            if (lhs[i] < rhs[j])
                return true;
            else if (rhs[j] < lhs[i])
                return false;

        return (i == count1 && j != count2);
    }

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_DATA_ACCESSOR_INCLUDED
