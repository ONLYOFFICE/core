//-----------------------------------------------------------------------------
#ifndef AGG_SVG_PARSE_REAL_INCLUDED
#define AGG_SVG_PARSE_REAL_INCLUDED
//-----------------------------------------------------------------------------
#include <cstring>
#include <cmath>
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{
    //-------------------------------------------------------------------------
    namespace detail
    {
        bool isdigit(unsigned c);
    }
    //-------------------------------------------------------------------------
    template<int Radix>
    struct radix_traits;
    //-------------------------------------------------------------------------
    ////////////////////////////////// Decimal
    template<>
    struct radix_traits<10>
    {
        static bool is_valid(char ch)
        {
            return detail::isdigit(ch);
        }

        static int digit(char ch)
        {
            return ch - '0';
        }
    };
    //-------------------------------------------------------------------------
    template<int Radix>
    struct positive_accumulate
    {
        //  Use this accumulator if number is positive
        template <typename T>
        static bool check(T const& n, T const& prev)
        {
            return n < prev;
        }

        template <typename T, typename CharT>
        static void add(T& n, CharT ch)
        {
            n += radix_traits<Radix>::digit(ch);
        }
    };
    //-------------------------------------------------------------------------
    template<int Radix>
    struct negative_accumulate
    {
        //  Use this accumulator if number is negative

        template <typename T>
        static bool check(T const& n, T const& prev)
        {
            return n > prev;
        }

        template <typename T, typename CharT>
        static void add(T& n, CharT ch)
        {
            n -= radix_traits<Radix>::digit(ch);
        }
    };
    //-------------------------------------------------------------------------
    template <int Radix, typename Accumulate>
    struct extract_int_base
    {
        //  Common code for extract_int specializations
        template <typename T>
        static bool f(const char* scan, T& n)
        {
            T prev = n;
            n *= Radix;
            if (Accumulate::check(n, prev))
                return false;   //  over/underflow!
            prev = n;
            Accumulate::add(n, *scan);
            if (Accumulate::check(n, prev))
                return false;   //  over/underflow!
            return true;
        }
    };
    //-------------------------------------------------------------------------
    template <int Radix = 10, typename Accumulate = positive_accumulate<Radix> >
    struct extract_int
    {
        template <typename T>
			static bool f(const char* & scan, const char * const end, T& n, size_t& count)
        {
            typedef extract_int_base<Radix, Accumulate> base;
            typedef radix_traits<Radix> check;

            size_t i = 0;
            for (; scan != end && check::is_valid(*scan);
                ++i, ++scan, ++count)
            {
                if (!base::f(scan, n))
                    return false;   //  over/underflow!
            }
            return i >= 1;
        }
    };
    //-------------------------------------------------------------------------
    class parse_real
    {
    private:
    //-------------------------------------------------------------------------
        const bool allow_leading_dot;
        const bool allow_trailing_dot;
        const bool expect_dot;
    public:
    //-------------------------------------------------------------------------
        parse_real() 
            : allow_leading_dot(true),
              allow_trailing_dot(true),
              expect_dot(false)
        {
        }
    private:
        //---------------------------------------------------------------------
        bool parse_sign(const char * & src)
        {
            bool neg = *src == '-';
            if (neg || (*src == '+'))
            {
                ++src;
                return neg;
            }
            return false;
        }
        //---------------------------------------------------------------------
        bool parse_dot(const char* & src)
        {
            if (*src == '.')
            {
                ++src;
                return true;
            }
            return false;
        }
        //---------------------------------------------------------------------
        bool parse_exp(const char* & src)
        {
            if (*src == 'e' || *src == 'E')
            {
                ++src;
                return true;
            }
            return false;
        }
        //---------------------------------------------------------------------
        template <class T>
        bool parse_uint(const char * & beg, const char * const end, T& n, size_t& count)
        {
            count = 0;
            bool result = extract_int<>::f(beg, end, n, count);
            return result;
        }
        //---------------------------------------------------------------------
        template <class T>
        bool parse_int(const char * & beg, const char * const end, T& n, size_t& count)
        {
            typedef extract_int<10,negative_accumulate<10> > extract_int_neg_t;
            typedef extract_int<>                            extract_int_pos_t;

            if (*beg)
            {
                count = 0;
                n     = 0;
                const char* save = beg;

                bool hit = parse_sign(beg);

                if (hit)
                    hit = extract_int_neg_t::f(beg, end, n, count);
                else
                    hit = extract_int_pos_t::f(beg, end, n, count);

                if (hit)
                    return true;
                else
                    beg = save;
                // return no-match if number overflows or underflows
            }
            return false;
        }
    public:
        //-------------------------------------------------------------------------
        bool parse(const char* beg, const char * const end, 
                double* d, const char** endptr)
        {
            const char * src = beg;

            if (src == 0 || *src == 0)
                return false;

            double result = 0;

            size_t count = 0;

            bool neg = parse_sign(src);
            
            bool got_a_number = parse_uint(src, end, result, count) && count;

            if (!got_a_number && !allow_leading_dot)
                return false;

            bool exp_hit = false;
            int  exp     = 0;

            if (parse_dot(src))
            {
                //  We got the decimal point. Now we will try to parse
                //  the fraction if it is there. If not, it defaults
                //  to zero (0) only if we already got a number.

                if (parse_uint(src, end, result, count))
                {
                    exp = (int)count;
                    exp = -exp;
                }
                else if (!got_a_number || !allow_trailing_dot)
                    return false;

                exp_hit = parse_exp(src);
            }
            else
            {
                //  We have reached a point where we
                //  still haven't seen a number at all.
                //  We return early with a no-match.
                if (!got_a_number)
                    return false;

                //  If we must expect a dot and we didn't see
                //  an exponent, return early with a no-match.
                exp_hit = parse_exp(src);
                if (expect_dot && !exp_hit)
                    return false;
            }

            if (exp_hit)
            {
                //  We got the exponent prefix. Now we will try to parse the
                //  actual exponent. It is an error if it is not there.
                int temp;
                if (parse_int(src, end, temp, count))
                {
                    exp += temp;
                }
                else
                {
                    return false;
                }
            }

            *d = result;

            if (neg)
                *d = -(*d);

            *d *= pow(10., exp);

            *endptr = src;

            return true;//src == end;
        }
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_PARSE_REAL_INCLUDED

