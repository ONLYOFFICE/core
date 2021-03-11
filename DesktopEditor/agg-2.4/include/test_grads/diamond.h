template <class ColorT>
class gradient_base
{
public:
    typedef ColorT color_type;
    typedef NSStructures::GradientInfo ginfo;

protected:
    enum
    {
        StateInit = 0,
        StateReady = 1,

        MaxColorIndex = 512
    };

protected:
    int m_state;

    ginfo m_oGradientInfo;
    agg::point_d m_center;
    double m_factor;

    agg::trans_affine m_trans;

protected:
    const color_type *m_pSubColors;
    const float *m_pPosSubColors;
    int m_nCountSubColors;

    color_type m_color_table[MaxColorIndex + 1];
    bool m_valid_table[MaxColorIndex + 1];

protected:
    

    virtual inline double calculate_param(const double &_x, const double &_y) = 0;

public:
    gradient_base()
        : m_state(StateInit),
          m_factor(0),
          m_center(0, 0),
          m_pSubColors(0),
          m_pPosSubColors(0),
          m_nCountSubColors(0)
    {
    }
    virtual ~gradient_base() {}
    void SetGradientInfo(ginfo _g)
    {
        m_oGradientInfo = _g;
    }

    void SetSubColors(const color_type *colors, const float *positions, int count)
    {
        m_pSubColors = colors;
        m_pPosSubColors = positions;
        m_nCountSubColors = count;
    }

    void prepare()
    {
        if (m_state != StateReady)
        {
            m_state = StateReady;
            memset(m_valid_table, 0, sizeof m_valid_table);
        }
    }

    void generate(color_type *span, int x, int y, unsigned len)
    {
        for (unsigned count = 0; count < len; ++count, ++x)
        {
            double _x = x;
            double _y = y;
            m_trans.transform(&_x, &_y);
            double t = calculate_param(_x, _y);

            int index = int(t * MaxColorIndex + 0.5);
            if (!m_valid_table[index])
                CalcColor(index);

            *span++ = m_color_table[index];
        }
    }

    void SetDirection(const agg::rect_d &bounds, const agg::trans_affine &trans)
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

protected:
    void CalcColor(int index)
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

            if (t < m_pPosSubColors[i])
            {
                t = (t - m_pPosSubColors[i - 1]) / (m_pPosSubColors[i] - m_pPosSubColors[i - 1]);
                m_color_table[index] = m_pSubColors[i - 1].gradient(m_pSubColors[i], t);

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

    inline double triagle_saw(double x)
    {
        return fabs(2 * asin(sin(x * pi)) / pi);
    }
};

template <class ColorT>
class my_test_gradient : public gradient_base<ColorT> {
    inline double calculate_param(const double &x, const double &y) override
    {
        double cx = this->m_center.x;
        double cy = this->m_center.y;
        double t = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) * this->m_factor;
        if (this->m_oGradientInfo.periods != 0)
        {
            t = this->triagle_saw(this->m_oGradientInfo.periods * t);
        }
        if (fabs(this->m_oGradientInfo.discrete_step) > FLT_EPSILON)
        {
            t = this->m_oGradientInfo.discrete_step * floor(t / this->m_oGradientInfo.discrete_step);
        }
        if (t < 0)
            return 0;
        if (t > 1)
            return 1;
        return t;
    }
};

template <class ColorT>
class gradient_radial_span : public gradient_base<ColorT> {
    inline double calculate_param(const double &x, const double &y) override
    {
        double cx = this->m_center.x;
        double cy = this->m_center.y;
        double t = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) * this->m_factor;
        if (this->m_oGradientInfo.periods != 0)
        {
            t = this->triagle_saw(this->m_oGradientInfo.periods * t);
        }
        if (fabs(this->m_oGradientInfo.discrete_step) > FLT_EPSILON)
        {
            t = this->m_oGradientInfo.discrete_step * floor(t / this->m_oGradientInfo.discrete_step);
        }
        if (t < 0)
            return 0;
        if (t > 1)
            return 1;
        return t;
    }
};

template <class ColorT>
class conical_gradient_span : public gradient_base<ColorT> {
    inline double calculate_param(const double &x, const double &y) override
    {
        double t = 2 * fabs(atan2(double(y), double(x)))/ pi;
        if (this->m_oGradientInfo.periods != 0)
        {
            t = this->triagle_saw(this->m_oGradientInfo.periods * t);
        }
        if (fabs(this->m_oGradientInfo.discrete_step) > FLT_EPSILON)
        {
            t = this->m_oGradientInfo.discrete_step * floor(t / this->m_oGradientInfo.discrete_step);
        }
        if (t < 0)
            return 0;
        if (t > 1)
            return 1;
        return t;
    }
};