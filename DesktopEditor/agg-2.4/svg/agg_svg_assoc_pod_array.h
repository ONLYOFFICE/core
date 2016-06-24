#ifndef AGG_SVG_ASSOC_POD_ARRAY_INCLUDE
#define AGG_SVG_ASSOC_POD_ARRAY_INCLUDE
//-----------------------------------------------------------------------------
#pragma warning(disable:4786)

#include <algorithm>
#include <functional>
#include <vector>
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    enum sort_state_e {off,on};
    //-------------------------------------------------------------------------
    template<class T1, class T2> 
    struct pair 
    {
	    typedef T1 first_type;
	    typedef T2 second_type;
	
        pair() : first(T1()), second(T2()) {}
	    pair(const T1& v1, const T2& v2) : first(v1), second(v2) {}
	
        T1 first;
	    T2 second;
	};
    //-------------------------------------------------------------------------
    template<class KeyT, class T, class CompareT = std::less<KeyT> >
    class assoc_pod_array : private std::vector<pair<KeyT, T> > 
    {
    public:
        //---------------------------------------------------------------------
        typedef std::vector<pair<KeyT, T> > base_type;
        //---------------------------------------------------------------------
        typedef typename base_type::value_type value_type;
        //---------------------------------------------------------------------
        typedef KeyT        key_type;
        typedef T           mapped_type;
        typedef CompareT    key_compare;
        //---------------------------------------------------------------------
        typedef base_type::iterator         iterator;
        typedef base_type::const_iterator   const_iterator;
        //---------------------------------------------------------------------
        iterator begin()
        {
            return base_type::begin();
        }
        //---------------------------------------------------------------------
        const_iterator begin() const
        {
            return base_type::begin();
        }
        //---------------------------------------------------------------------
        iterator end()
        {
            return base_type::end();
        }
        //---------------------------------------------------------------------
        const_iterator end() const
        {
            return base_type::end();
        }
        //---------------------------------------------------------------------
        class value_compare
        {
            //-----------------------------------------------------------------
            friend class assoc_pod_array;
        //private:
		public:
            //-----------------------------------------------------------------
            CompareT m_comp;
            //-----------------------------------------------------------------
            value_compare(CompareT c) : m_comp(c) {}
        public:
            //-----------------------------------------------------------------
            template <class T>
            bool operator()(const value_type& x, const T& y) const
            {
                return m_comp(x.first, y);
            }
            //-----------------------------------------------------------------
            template <class T>
            bool operator()(const T& x, const value_type& y) const
            {
                return m_comp(x, y.first);
            }
            //-----------------------------------------------------------------
            bool operator()(const value_type& x, const typename value_type::first_type& y) const 
            {
                return m_comp(x.first, y);
            }
            //-----------------------------------------------------------------
            bool operator()(const typename value_type::first_type& x, const value_type& y) const 
            {
                return m_comp(x, y.first);
            }
            //-----------------------------------------------------------------
            bool operator()(const value_type& x, const value_type& y) const 
            {
                return m_comp(x.first, y.first);
            }
        };
        //---------------------------------------------------------------------
        class value_compare_for_sort
        {
            //-----------------------------------------------------------------
            friend class assoc_pod_array;
        //private:
		public:
            //-----------------------------------------------------------------
            CompareT m_comp;
            //-----------------------------------------------------------------
            value_compare_for_sort(CompareT c) : m_comp(c) {}
        public:
            //-----------------------------------------------------------------
            bool operator()(const value_type& x, const typename value_type::first_type& y) const 
            {
                return m_comp(x.first, y);
            }
            //-----------------------------------------------------------------
            bool operator()(const typename value_type::first_type& x, const value_type& y) const 
            {
                return m_comp(x, y.first);
            }
            //-----------------------------------------------------------------
            bool operator()(const value_type& x, const value_type& y) const 
            {
                return m_comp(x.first, y.first);
            }
        };
        //-----------------------------------------------------------------
        explicit assoc_pod_array(const CompareT& comp = key_compare()) 
            : m_sort_state(off), m_compare(comp)
        {
        }
        //-----------------------------------------------------------------
        void sort_state(sort_state_e state)
        {
            m_sort_state = state;
            if (m_sort_state == on)
            {
                std::sort(begin(), end(), value_compare_for_sort(m_compare));
            }
        }
        //-----------------------------------------------------------------
        unsigned size() const
        {
            return base_type::size();
        }
        //---------------------------------------------------------------------
        void clear()
        {
            m_sort_state = off;
            base_type::clear();
        }
        //---------------------------------------------------------------------
        // Inserts val if and only if there is no element in the container with
        // key equivalent to the key.
        // return value indicates indicates whether the insertion takes place 
        bool insert(KeyT const & key, T const & val)
        {
            bool find(false);

            if (m_sort_state == off)
            {
                //base_type::add(value_type(key, val));
                base_type::push_back(value_type(key, val));
            }
            else
            {
                iterator it = std::lower_bound(begin(), end(), 
                                            key,value_compare(m_compare));
                if (it == end())
                {
                    base_type::insert(it, value_type(key, val));
                } 
                else 
                if (!(!m_compare(it->first,key) && !m_compare(key,it->first)))
                {
                    base_type::insert(it, value_type(key, val));
                }
                else
                {
                    find = true;
                }
            }
            return !find;
        }
        //---------------------------------------------------------------------
        template <class Key>
        iterator find(const Key& key) 
        {
            assert(m_sort_state == on);

            std::pair<iterator, iterator> p = std::equal_range(begin(), end(), 
                                                key, value_compare(m_compare));
            return p.first != p.second ? p.first : end();
        }
        //-----------------------------------------------------------------------------
    private:
        sort_state_e m_sort_state;
        CompareT     m_compare;
    };

} 
}

#endif 

