#ifndef AGG_SVG_PERCENT_INCLUDE
#define AGG_SVG_PERCENT_INCLUDE
//-----------------------------------------------------------------------------
namespace agg
{
namespace svg
{        
	struct percent
    {
        static double decrypt(double val)
        {
            return (val > 1.)?
                (val - (double)difference_helper) / 100.:val;
        }
        static double encrypt(double val)
        {
            return (double)difference_helper + val;
        }
    private:
        enum {difference_helper = 2};
    };

} // namespace svg
} // namespace agg

#endif // #ifndef AGG_SVG_PERCENT_INCLUDE