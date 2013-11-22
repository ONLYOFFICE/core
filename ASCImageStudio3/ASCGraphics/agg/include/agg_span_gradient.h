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
#include "agg_math.h"
#include "agg_array.h"


namespace agg
{

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

		enum WrapMode
		{
			WrapModeTile = 0,
			WrapModeFlip = 1
		};

	private:
		enum
		{
			StateInit = 0,
			StateReady = 1,

			MaxColorIndex = 512
		};

	private:
		int m_state;
		int m_wrap;  // 0 - tile, 1 - flip

		agg::point_d m_factor;
		double m_offset;

		color_type m_color[2];
		agg::point_f m_focus; // x - position [-1..1], y - factor [0..1];

	private:	
		const color_type* m_pSubColors;
		const float* m_pPosSubColors;
		int m_nCountSubColors;

	private:
		int m_bFocusInvert;
		int m_bFocusUsed;
		double m_dFocusPosition;
		double m_dFocusFactor[2];

		color_type m_color_table[MaxColorIndex+1];
		bool m_valid_table[MaxColorIndex+1];

	public:
		my_span_gradient()
			: m_wrap( WrapModeTile ), 
			m_state( StateInit ), 
			m_focus( 1.0f, 1.0f ),
			m_factor( 0, 0 ),
			m_offset( 0 ),
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
		
		void SetDirection( const agg::rect_d& rect )
		{
			m_state = StateInit;
			
			m_factor.x = 0;
			m_factor.y = 0;
			m_offset = 0;

			double dx = rect.x2 - rect.x1;
			double dy = rect.y2 - rect.y1;

			if( abs(dx) < FLT_EPSILON && abs(dy) < FLT_EPSILON )
				return;

			double dot = dx * dx + dy * dy;
			dot += dot;

			if( dot < FLT_EPSILON )
				return;

			dx /= dot;
			dy /= dot;

			m_factor.x = dx;
			m_factor.y = dy;
			m_offset = -(rect.x1 * dx + rect.y1 * dy);
		}

		void SetDirection( const agg::rect_d& rect, float fRotationAngle, int bAngleScalable )
		{
			m_state = StateInit;
			
			m_factor.x = 0;
			m_factor.y = 0;
			m_offset = 0;

			double x1 = rect.x1;
			double x2 = rect.x2;
			double y1 = rect.y1;
			double y2 = rect.y2;

			double abs_dx = abs(x1 - x2);
			double abs_dy = abs(y1 - y2);

			if( abs_dx < FLT_EPSILON && abs_dy < FLT_EPSILON )
				return;

			double angle = fRotationAngle / 360;
			angle -= floor( angle );
			
			if( bAngleScalable && !(abs_dy < FLT_EPSILON) )
			{
				angle *= 4;
				int n = int(angle);
				switch( n )
				{
				case 1: angle = 2 - angle; break;
				case 2: angle = angle - 2; break;
				case 3: angle = 4 - angle; break;
				}
				
				angle *= pi/2;
				if( !(abs(angle - pi/2) < FLT_EPSILON) )
					angle = atan( abs_dx / abs_dy * tan( angle ) );

				switch( n )
				{
				case 1: angle = pi - angle; break;
				case 2: angle = angle + pi; break;
				case 3: angle = pi2 - angle; break;
				}
			}
			else
			{
				angle *= pi2;
			}

			double a2 = cos( angle );
			double b2 = sin( angle );

			if( a2 < 0 )
			{
				double tmp = x2; x2 = x1; x1 = tmp;
			}

			if( b2 < 0 )
			{
				double tmp = y2; y2 = y1; y1 = tmp;
			}

			double dx = x2 - x1;
			double dy = y2 - y1;
			double dot = dx * dx + dy * dy;
			dot += dot; // for wrap

			double a1 = -dy;
			double b1 = dx;
			double c1 = x1 * y2 - x2 * y1;

			double d = (a1 * b2 - a2 * b1) * dot;
			if( abs( d ) < FLT_EPSILON )
				return;

			m_factor.x = a2 * (a1 * dy - b1 * dx) / d;
			m_factor.y = b2 * (a1 * dy - b1 * dx) / d;
			m_offset   = c1 * (a2 * dy - b2 * dx) / d - (x1 * dx + y1 * dy) / dot;
		}
		
		void SetColors( const color_type& first, const color_type& last )
		{
			m_color[0] = first;
			m_color[1] = last;
			m_state = StateInit;
		}

		void SetWrapMode( int mode )
		{
			m_wrap = mode ? WrapModeFlip : WrapModeTile;
			m_state = StateInit;
		}

		void SetFocus( float position, float factor )
		{
			m_focus.x = max(-1.0f, min(position, 1.0f));
			m_focus.y = max( 0.0f, min(factor, 1.0f));

			m_state = StateInit;
		}

        void prepare()
		{
			if( m_state != StateReady )
			{
				m_state = StateReady;

				memset( m_valid_table, 0, sizeof m_valid_table );
				
				m_bFocusInvert = m_focus.x < 0;
				m_bFocusUsed = !(abs(m_focus.x) < FLT_EPSILON || (1 - abs(m_focus.x)) < FLT_EPSILON);
				if( m_bFocusUsed )
				{
					m_dFocusPosition = m_bFocusInvert ? (m_focus.x + 1) : m_focus.x;
					m_dFocusFactor[0] = m_focus.y / m_focus.x;
					m_dFocusFactor[1] = m_focus.y / (1 - m_focus.x);
				}
			}
		}

        void generate( color_type* span, int x, int y, unsigned len)
		{
			double dy = y * m_factor.y + m_offset;

			for( unsigned count = 0; count < len; ++count, ++x )
			{
				double t = dy + x * m_factor.x;
				t -= floor( t );
				
				int index = int( t * MaxColorIndex + 0.5 );
				if( !m_valid_table[index] )
					CalcColor( index );

				*span++ = m_color_table[index];
			}
		}

	private:
		void CalcColor( int index )
		{
			double t = index * (1.0 / MaxColorIndex);

			t += t;

			if( m_wrap )
			{
				t = 1 - abs(t - 1);
			}
			else
			{
				if( t > 1 )
					t -= 1;
			}

			if( m_bFocusInvert )
			{
				t = 1 - t;
			}

			if( m_bFocusUsed )
			{
				if( t < m_dFocusPosition )
					t *= m_dFocusFactor[0];
				else
					t = (1 - t) * m_dFocusFactor[1];
			}

			if( m_nCountSubColors > 1 )
			{
				bool bFindColor = false;

				for( int i = 1; i < m_nCountSubColors; i++ )
				{
					if( t < m_pPosSubColors[i] )
					{
						t = (t - m_pPosSubColors[i - 1]) / (m_pPosSubColors[i] - m_pPosSubColors[i - 1]);
						m_color_table[index] = m_pSubColors[i - 1].gradient( m_pSubColors[i], t );
						
						bFindColor = true;
						break;
					}
				}
				
				if( !bFindColor )
					m_color_table[index] = m_color[1];
			}
			else
			{
				m_color_table[index] = m_color[0].gradient( m_color[1], t );
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
		int m_state;
		int m_wrap;  // 0 - tile, 1 - flip

		color_type m_color[2];
		agg::point_f m_center;
		const agg::point_f* m_out_points;
		int m_count_points;

	public:
		my_span_path_gradient()
			: m_center( 0, 0 ),
			m_out_points( 0 ),
			m_count_points( 0 )
		{
		}

		void SetCenter( const agg::point_f& center )
		{
			m_center = center;
		}

		void SetColors( const color_type& first, const color_type& last )
		{
			m_color[0] = first;
			m_color[1] = last;
		}

		void SetPoints( const agg::point_f* points, int count )
		{
			m_out_points = points;
			m_count_points = count;
		}
 
		void prepare()
		{
			if( m_state != StateReady )
			{
				m_state = StateReady;
			}
		}

        void generate( color_type* span, int x, int y, unsigned len)
		{
			for( unsigned count = 0; count < len; ++count, ++x )
			{
				//*span++ = m_color_table[index];
			}
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
