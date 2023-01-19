//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------

#ifndef AGG_SPAN_GRADIENT_INCLUDED
#define AGG_SPAN_GRADIENT_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "../../graphics/structures.h"
#include "agg_math.h"
#include "agg_array.h"
#include "agg_trans_affine.h"
#include <iostream>
#include <cmath>
#include "./test_grads/custom_gradients.h"
namespace agg
{
    
#if !defined(_LINUX) && !(defined(_WIN32) || defined (_WIN64)) && !defined(__APPLE__)
    double _hypot(double x, double y)
    {
        return sqrt(x*x + y*y);
    }
    
#endif

    enum gradient_subpixel_scale_e
    {
        gradient_subpixel_shift = 4,                              //-----gradient_subpixel_shift
        gradient_subpixel_scale = 1 << gradient_subpixel_shift,   //-----gradient_subpixel_scale
        gradient_subpixel_mask  = gradient_subpixel_scale - 1     //-----gradient_subpixel_mask
    };



    //==========================================================span_gradient
    template<class ColorT>
	struct TBlendColor
	{
		ColorT color;
		float position; // [0..1]
	};

	struct TBlendFactor
	{
		float factor;   // [0..1]
		float position; // [0..1]
	};
	
	
	template<class ColorT,
             class Interpolator,
             class GradientF, 
             class ColorF>
    class span_gradient
    {
    public:
        typedef Interpolator interpolator_type;
        typedef ColorT color_type;

        enum downscale_shift_e
        {
            downscale_shift = interpolator_type::subpixel_shift - 
                              gradient_subpixel_shift
        };

        //--------------------------------------------------------------------
        span_gradient() {}

        //--------------------------------------------------------------------
        span_gradient(interpolator_type& inter,
                      const GradientF& gradient_function,
                      const ColorF& color_function,
                      double d1, double d2) : 
            m_interpolator(&inter),
            m_gradient_function(&gradient_function),
            m_color_function(&color_function),
            m_d1(iround(d1 * gradient_subpixel_scale)),
            m_d2(iround(d2 * gradient_subpixel_scale))
        {}

        //--------------------------------------------------------------------
        interpolator_type& interpolator() { return *m_interpolator; }
        const GradientF& gradient_function() const { return *m_gradient_function; }
        const ColorF& color_function() const { return *m_color_function; }
        double d1() const { return double(m_d1) / gradient_subpixel_scale; }
        double d2() const { return double(m_d2) / gradient_subpixel_scale; }

        //--------------------------------------------------------------------
        void interpolator(interpolator_type& i) { m_interpolator = &i; }
        void gradient_function(const GradientF& gf) { m_gradient_function = &gf; }
        void color_function(const ColorF& cf) { m_color_function = &cf; }
        void d1(double v) { m_d1 = iround(v * gradient_subpixel_scale); }
        void d2(double v) { m_d2 = iround(v * gradient_subpixel_scale); }

        //--------------------------------------------------------------------
        void prepare() {}

        //--------------------------------------------------------------------
        void generate(color_type* span, int x, int y, unsigned len)
        {   
            if (m_d2 <= m_d1)
				m_d2 = m_d1 + 1;

			int dd = m_d2 - m_d1;
            if(dd < 1) dd = 1;
            m_interpolator->begin(x+0.5, y+0.5, len);
            do
            {
                m_interpolator->coordinates(&x, &y);
                int d = m_gradient_function->calculate(x >> downscale_shift, 
                                                       y >> downscale_shift, m_d2);
                d = ((d - m_d1) * (int)m_color_function->size()) / dd;
                if(d < 0) d = 0;
                if(d >= (int)m_color_function->size()) d = m_color_function->size() - 1;
                *span++ = (*m_color_function)[d];
                ++(*m_interpolator);
            }
            while(--len);
        }

    private:
        interpolator_type* m_interpolator;
        const GradientF*   m_gradient_function;
        const ColorF*      m_color_function;
        int                m_d1;
        int                m_d2;
    };


	template<class ColorT>
    class my_span_gradient
	{
	public:
		typedef ColorT color_type;

	private:
		enum
		{
			StateInit = 0,
			StateReady = 1,

			MaxColorIndex = 512
		};

	private:
		int m_state;

		double m_cos;
		double m_sin;

		agg::point_d	m_factor;
		agg::point_d	m_offset;

		agg::trans_affine m_trans;

	private:	
		const color_type*	m_pSubColors;
		const float*		m_pPosSubColors;
		int					m_nCountSubColors;

		color_type	m_color_table[MaxColorIndex+1];
		bool		m_valid_table[MaxColorIndex+1];

	private:
		inline agg::point_d project(const double& x, const double& y)
		{
			// проекция точки (x, y) на прямую, с направляющим вектором (cos, sin)
			// перпендикулярная прямая x*m_cos + y*m_sin + c = 0;
			double c = -(x * m_cos + y * m_sin);
			point_d ret;
			ret.x = -c * m_cos;
			ret.y = -c * m_sin;
			return ret;
		}
		
		inline double calculate_param(const double& x, const double& y)
		{
			double t1 = fabs(m_factor.x * x + m_factor.y * y + m_offset.x);
			double t2 = fabs(m_factor.x * x + m_factor.y * y + m_offset.y);
			if (t1 >= 1 || t2 >= 1)
			{
				if (t1 > t2)
					return 0;
				return 1;
			}
			return t1;
		}
		
	public:
		my_span_gradient()
			: m_state( StateInit ), 
			m_factor( 0, 0 ),
			m_offset( 0, 0 ),
			m_pSubColors( 0 ),
			m_pPosSubColors( 0 ),
			m_nCountSubColors( 0 )
		{
		}

		void SetSubColors( const color_type* colors, const float* positions, int count )
		{
			m_pSubColors = colors;
			m_pPosSubColors = positions;
			m_nCountSubColors = count;
		}
		
        void prepare()
		{
			if( m_state != StateReady )
			{
				m_state = StateReady;
				memset( m_valid_table, 0, sizeof m_valid_table );
			}
		}

        void generate( color_type* span, int x, int y, unsigned len)
		{
			for( unsigned count = 0; count < len; ++count, ++x )
			{
				double _x = x;
				double _y = y;
				m_trans.transform(&_x, &_y);
				double t = calculate_param(_x, _y);
				int index = int( t * MaxColorIndex + 0.5 );
				if( !m_valid_table[index] )
					CalcColor( index );

				*span++ = m_color_table[index];
			}
		}

		void SetDirection(const agg::rect_d& bounds, const double& angleDegrees, const agg::trans_affine& trans)
		{
			m_trans = trans;

			double angle = angleDegrees / 360;
			angle -= floor( angle );
			angle *= pi2;

			m_cos = cos(angle);
			m_sin = sin(angle);

			agg::point_d projects[4];
			projects[0] = project(bounds.x1, bounds.y1);
			projects[1] = project(bounds.x2, bounds.y1);
			projects[2] = project(bounds.x1, bounds.y2);
			projects[3] = project(bounds.x2, bounds.y2);

			double min = projects[0].x * projects[0].x + projects[0].y * projects[0].y;
			if ((projects[0].x * m_cos + projects[0].y * m_sin) < 0)
				min = -min;

			double max = min;
			int iMin = 0;
			int iMax = 0;

			for (int i = 1; i < 4; ++i)
			{
				double tmp = projects[i].x * projects[i].x + projects[i].y * projects[i].y;
				if (projects[i].x * m_cos + projects[i].y * m_sin < 0)
					tmp = -tmp;

				if (tmp < min)
				{
					min = tmp;
					iMin = i;
				}
				if (tmp > max)
				{
					max = tmp;
					iMax = i;
				}
			}

			double dFactor = _hypot(projects[iMax].x - projects[iMin].x, projects[iMax].y - projects[iMin].y);
			
			double c1 = -(projects[iMin].x * m_cos + projects[iMin].y * m_sin);
			double c2 = -(projects[iMax].x * m_cos + projects[iMax].y * m_sin);

			if (abs(dFactor) > FLT_EPSILON)
			{
				m_factor.x = m_cos / dFactor;
				m_factor.y = m_sin / dFactor;
				m_offset.x = c1 / dFactor;
				m_offset.y = c2 / dFactor;
			}
		}

	private:
		void CalcColor( int index )
		{
			double t = index * (1.0 / MaxColorIndex);

			bool bFindColor = false;

			for (int i = 1; i < m_nCountSubColors; ++i)
			{
				if ( t < m_pPosSubColors[i] )
				{
					if (i == 1 && t < m_pPosSubColors[0])
					{
						// меньше меньшего
						m_color_table[index] = m_pSubColors[0];
						bFindColor = true;
						break;
					}

					t = (t - m_pPosSubColors[i - 1]) / (m_pPosSubColors[i] - m_pPosSubColors[i - 1]);
					m_color_table[index] = m_pSubColors[i - 1].gradient( m_pSubColors[i], t );
					
					bFindColor = true;
					break;
				}
			}
			if (!bFindColor)
			{
				if (m_pSubColors != NULL && m_nCountSubColors > 0)
                    m_color_table[index] = m_pSubColors[m_nCountSubColors - 1];
                else
                    m_color_table[index] = color_type();
			}

			m_valid_table[index] = true;
		}
	};

	
	template<class ColorT>
    class my_span_path_gradient
	{
	public:
		typedef ColorT color_type;

	private:
		enum
		{
			StateInit = 0,
			StateReady = 1,

			MaxColorIndex = 512
		};

	private:
		int m_state;

		agg::point_d		m_center;
		double				m_factor;

		agg::trans_affine	m_trans;

	private:	
		const color_type*	m_pSubColors;
		const float*		m_pPosSubColors;
		int					m_nCountSubColors;

		color_type	m_color_table[MaxColorIndex+1];
		bool		m_valid_table[MaxColorIndex+1];

	private:
		
		inline double calculate_param(const double& x, const double& y)
		{
			double t = sqrt((x - m_center.x) * (x - m_center.x) + (y - m_center.y) * (y - m_center.y)) * m_factor;
			if (t > 1)
				return 1;
			return t;
		}
		
	public:
		my_span_path_gradient()
			: m_state( StateInit ), 
			m_factor(0),
			m_center( 0, 0 ),
			m_pSubColors( 0 ),
			m_pPosSubColors( 0 ),
			m_nCountSubColors( 0 )
		{
		}

		void SetSubColors( const color_type* colors, const float* positions, int count )
		{
			m_pSubColors = colors;
			m_pPosSubColors = positions;
			m_nCountSubColors = count;
		}
		
        void prepare()
		{
			if( m_state != StateReady )
			{
				m_state = StateReady;
				memset( m_valid_table, 0, sizeof m_valid_table );
			}
		}

        void generate( color_type* span, int x, int y, unsigned len)
		{
			for( unsigned count = 0; count < len; ++count, ++x )
			{
				double _x = x;
				double _y = y;
				m_trans.transform(&_x, &_y);
				double t = calculate_param(_x, _y);
								
				int index = int( t * MaxColorIndex + 0.5 );
				if( !m_valid_table[index] )
					CalcColor( index );

				*span++ = m_color_table[index];
			}
		}

		void SetDirection(const agg::rect_d& bounds, const agg::trans_affine& trans)
		{
			m_trans = trans;

			m_center.x = (bounds.x1 + bounds.x2) / 2;
			m_center.y = (bounds.y1 + bounds.y2) / 2;
			
            double dmax = (abs(bounds.x1 - bounds.x2));
            if (dmax < abs(bounds.y1 - bounds.y2))
                dmax = abs(bounds.y1 - bounds.y2);
			m_factor = 0;
			
			if (dmax > FLT_EPSILON)
				m_factor = 2 / dmax;			
		}

	private:
		void CalcColor( int index )
		{
			double t = index * (1.0 / MaxColorIndex);

			bool bFindColor = false;

			for (int i = 1; i < m_nCountSubColors; ++i)
			{
				if (i == 1 && t < m_pPosSubColors[0])
				{
					// меньше меньшего
					m_color_table[index] = m_pSubColors[0];
					bFindColor = true;
					break;
				}

				if ( t < m_pPosSubColors[i] )
				{
					t = (t - m_pPosSubColors[i - 1]) / (m_pPosSubColors[i] - m_pPosSubColors[i - 1]);
					m_color_table[index] = m_pSubColors[i - 1].gradient( m_pSubColors[i], t );
					
					bFindColor = true;
					break;
				}
			}
			if (!bFindColor)
			{
				m_color_table[index] = m_pSubColors[m_nCountSubColors - 1];
			}

			m_valid_table[index] = true;
		}
	};


    //=====================================================gradient_linear_color
    template<class ColorT> 
    struct gradient_linear_color
    {
        typedef ColorT color_type;

        gradient_linear_color() {}
        gradient_linear_color(const color_type& c1, const color_type& c2, 
                              unsigned size = 256) :
            m_c1(c1), m_c2(c2), m_size(size) {}

        unsigned size() const { return m_size; }
        color_type operator [] (unsigned v) const 
        {
            return m_c1.gradient(m_c2, double(v) / double(m_size - 1));
        }

        void colors(const color_type& c1, const color_type& c2, unsigned size = 256)
        {
            m_c1 = c1;
            m_c2 = c2;
            m_size = size;
        }

        color_type m_c1;
        color_type m_c2;
        unsigned m_size;
    };






    //==========================================================gradient_circle
    class gradient_circle
    {
        // Actually the same as radial. Just for compatibility
    public:
        static AGG_INLINE int calculate(int x, int y, int)
        {
            return int(fast_sqrt(x*x + y*y));
        }
    };


    //==========================================================gradient_radial
    class gradient_radial
    {
    public:
        static AGG_INLINE int calculate(int x, int y, int)
        {
            return int(fast_sqrt(x*x + y*y));
        }
    };

    //========================================================gradient_radial_d
    class gradient_radial_d
    {
    public:
        static AGG_INLINE int calculate(int x, int y, int)
        {
            return uround(sqrt(double(x)*double(x) + double(y)*double(y)));
        }
    };

    //====================================================gradient_radial_focus
    class gradient_radial_focus
    {
    public:
        //---------------------------------------------------------------------
        gradient_radial_focus() : 
            m_r(100 * gradient_subpixel_scale), 
            m_fx(0), 
            m_fy(0)
        {
            update_values();
        }

        //---------------------------------------------------------------------
        gradient_radial_focus(double r, double fx, double fy) : 
            m_r (iround(r  * gradient_subpixel_scale)), 
            m_fx(iround(fx * gradient_subpixel_scale)), 
            m_fy(iround(fy * gradient_subpixel_scale))
        {
            update_values();
        }

        //---------------------------------------------------------------------
        void init(double r, double fx, double fy)
        {
            m_r  = iround(r  * gradient_subpixel_scale);
            m_fx = iround(fx * gradient_subpixel_scale);
            m_fy = iround(fy * gradient_subpixel_scale);
            update_values();
        }

        //---------------------------------------------------------------------
        double radius()  const { return double(m_r)  / gradient_subpixel_scale; }
        double focus_x() const { return double(m_fx) / gradient_subpixel_scale; }
        double focus_y() const { return double(m_fy) / gradient_subpixel_scale; }

        //---------------------------------------------------------------------
        int calculate(int x, int y, int) const
        {
            double dx = x - m_fx;
            double dy = y - m_fy;
            double d2 = dx * m_fy - dy * m_fx;
            double d3 = m_r2 * (dx * dx + dy * dy) - d2 * d2;
            return iround((dx * m_fx + dy * m_fy + sqrt(fabs(d3))) * m_mul);
        }

    private:
        //---------------------------------------------------------------------
        void update_values()
        {
            // Calculate the invariant values. In case the focal center
            // lies exactly on the gradient circle the divisor degenerates
            // into zero. In this case we just move the focal center by
            // one subpixel unit possibly in the direction to the origin (0,0)
            // and calculate the values again.
            //-------------------------
            m_r2  = double(m_r)  * double(m_r);
            m_fx2 = double(m_fx) * double(m_fx);
            m_fy2 = double(m_fy) * double(m_fy);
            double d = (m_r2 - (m_fx2 + m_fy2));
            if(d == 0)
            {
                if(m_fx) { if(m_fx < 0) ++m_fx; else --m_fx; }
                if(m_fy) { if(m_fy < 0) ++m_fy; else --m_fy; }
                m_fx2 = double(m_fx) * double(m_fx);
                m_fy2 = double(m_fy) * double(m_fy);
                d = (m_r2 - (m_fx2 + m_fy2));
            }
            m_mul = m_r / d;
        }

        int    m_r;
        int    m_fx;
        int    m_fy;
        double m_r2;
        double m_fx2;
        double m_fy2;
        double m_mul;
    };


    //==============================================================gradient_x
    class gradient_x
    {
    public:
        static int calculate(int x, int, int) { return x; }
    };


    //==============================================================gradient_y
    class gradient_y
    {
    public:
        static int calculate(int, int y, int) { return y; }
    };

    //========================================================gradient_diamond
    class gradient_diamond
    {
    public:
        static AGG_INLINE int calculate(int x, int y, int) 
        { 
            int ax = abs(x);
            int ay = abs(y);
            return ax > ay ? ax : ay; 
        }
    };

    //=============================================================gradient_xy
    class gradient_xy
    {
    public:
        static AGG_INLINE int calculate(int x, int y, int d) 
        { 
            return abs(x) * abs(y) / d; 
        }
    };

    //========================================================gradient_sqrt_xy
    class gradient_sqrt_xy
    {
    public:
        static AGG_INLINE int calculate(int x, int y, int) 
        { 
            return fast_sqrt(abs(x) * abs(y)); 
        }
    };

    //==========================================================gradient_conic
    class gradient_conic
    {
    public:
        static AGG_INLINE int calculate(int x, int y, int d) 
        { 
            return uround(fabs(atan2(double(y), double(x))) * double(d) / pi);
        }
    };

    //=================================================gradient_repeat_adaptor
    template<class GradientF> class gradient_repeat_adaptor
    {
    public:
        gradient_repeat_adaptor(const GradientF& gradient) : 
            m_gradient(&gradient) {}

        AGG_INLINE int calculate(int x, int y, int d) const
        {
            int ret = m_gradient->calculate(x, y, d) % d;
            if(ret < 0) ret += d;
            return ret;
        }

    private:
        const GradientF* m_gradient;
    };

    //================================================gradient_reflect_adaptor
    template<class GradientF> class gradient_reflect_adaptor
    {
    public:
        gradient_reflect_adaptor(const GradientF& gradient) : 
            m_gradient(&gradient) {}

        AGG_INLINE int calculate(int x, int y, int d) const
        {
            int d2 = d << 1;
            int ret = m_gradient->calculate(x, y, d) % d2;
            if(ret <  0) ret += d2;
            if(ret >= d) ret  = d2 - ret;
            return ret;
        }

    private:
        const GradientF* m_gradient;
    };
}

#endif
