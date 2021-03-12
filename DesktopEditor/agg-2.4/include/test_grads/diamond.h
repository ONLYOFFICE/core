
class calcBase
{
public:
    virtual double eval(double, double) = 0;
    virtual ~calcBase() {}
    void rotate(double &x, double &y, double angle)
    {
        double ca = cos(angle);
        double sa = sin(angle);
        double newx = x * ca + y * sa;
        double newy = -x * sa + y * ca;
        x = newx;
        y = newy;
    }
};

class calcRandom : public calcBase
{
public:
    calcRandom()
    {
        srand(time(NULL));
    }
    virtual double eval(double, double) override
    {
        return rand() / (double)RAND_MAX;
    }
};

class calcRadial : public calcBase
{
public:
    calcRadial(const NSStructures::GradientInfo &_gi, double _cx, double _cy, double _factor) : ginfo(_gi), cx(_cx), cy(_cy), factor(_factor)
    {
        cx += ginfo.centerX / factor;
        cy += ginfo.centerY / factor;
    }

    virtual double eval(double x, double y) override
    {
        x = x - cx;
        y = y - cy;
        x /= ginfo.xsize;
        y /= ginfo.ysize;
        rotate(x, y, ginfo.angle);
        double t = sqrt(x * x + y * y) * factor;
        if (t < ginfo.littleRadius || t < 0)
            return 0.;
        if (t > ginfo.largeRadius || t > 1)
        {
            return 1.;
        }
        return t;
    }
    virtual ~calcRadial() {}

private:
    NSStructures::GradientInfo ginfo;
    double cx, cy, factor;
};

class calcConical : public calcBase
{
public:
    calcConical(const NSStructures::GradientInfo &_gi, double _cx, double _cy, double _factor) : ginfo(_gi), cx(_cx), cy(_cy), factor(_factor)
    {
        cx += ginfo.centerX / factor;
        cy += ginfo.centerY / factor;
    }
    virtual double eval(double x, double y)
    {
        x = x - cx;
        y = y - cy;
        x /= ginfo.xsize;
        y /= ginfo.ysize;
        rotate(x, y, ginfo.angle);
        double t = fabs(atan2(x, y)) / pi;
        if (ginfo.angle > FLT_EPSILON)
        {
            if (t > 1 + FLT_EPSILON)
            {
                t -= floor(t);
            }
        }
        if (t > ginfo.largeRadius || t > 1)
            return 1;
        if (t < ginfo.littleRadius || t < 0)
            return 0;
        return t;
    }

private:
    NSStructures::GradientInfo ginfo;
    double cx, cy, factor;
};

class calcDiamond : public calcBase
{
public:
    calcDiamond(const NSStructures::GradientInfo &_gi, double _cx, double _cy, double _factor) : ginfo(_gi), cx(_cx), cy(_cy), factor(_factor)
    {
        cx += ginfo.centerX / factor;
        cy += ginfo.centerY / factor;
    }
    virtual double eval(double x, double y)
    {
        x = x - cx;
        y = y - cy;
        x /= ginfo.xsize;
        y /= ginfo.ysize;

        rotate(x, y, ginfo.angle);
        
        double t = std::max(fabs(x * factor), fabs(y * factor));

        if (t > ginfo.largeRadius || t > 1)
            return 1;
        if (t < ginfo.littleRadius || t < 0)
            return 0;
        return t;
    }

private:
    NSStructures::GradientInfo ginfo;
    double cx, cy, factor;
};

class calcNewLinear : public calcBase
{
public:
    calcNewLinear(const NSStructures::GradientInfo &_gi, double _start, double _end) : ginfo(_gi), start(_start), len(_end - _start)
    {
    }
    virtual double eval(double x, double y)
    {
        x = x - start;
        rotate(x, y, ginfo.angle);
        double t = x / len;

        if (t > ginfo.largeRadius || t > 1)
            return 1;
        if (t < ginfo.littleRadius || t < 0)
            return 0;
        return t;
    }

private:
    double start, len;
    NSStructures::GradientInfo ginfo;
};

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
    calcBase *calculate;

    agg::trans_affine m_trans;

protected:
    const color_type *m_pSubColors;
    const float *m_pPosSubColors;
    int m_nCountSubColors;
    double xmin, xmax;
    color_type m_color_table[MaxColorIndex + 1];
    bool m_valid_table[MaxColorIndex + 1];

protected:
    inline double calculate_param(const double &x, const double &y)
    {
        double t = calculate->eval(x, y);
        if (this->m_oGradientInfo.periods != 0.5)
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

public:
    gradient_base()
        : m_state(StateInit),
          m_factor(0),
          m_center(0, 0),
          m_pSubColors(0),
          m_pPosSubColors(0),
          m_nCountSubColors(0),
          calculate(nullptr)
    {
    }
    virtual ~gradient_base() {}
    void SetGradientInfo(ginfo _g, Aggplus::BrushType bType)
    {
        m_oGradientInfo = _g;
        switch (bType)
        {
        case Aggplus::BrushTypeRadialGradient:
            calculate = new calcRadial(m_oGradientInfo, m_center.x, m_center.y, m_factor);
            break;

        case Aggplus::BrushTypeConicalGradient:
            calculate = new calcConical(m_oGradientInfo, m_center.x, m_center.y, m_factor);
            break;

        case Aggplus::BrushTypeDiamondGradient:
            calculate = new calcDiamond(m_oGradientInfo, m_center.x, m_center.y, m_factor);
            break;

        case Aggplus::BrushTypeNewLinearGradient:
            calculate = new calcNewLinear(m_oGradientInfo, xmin, xmax);
            break;
        default:
            fprintf(stderr, "WRONG BRUSH TYPE");
            calculate = new calcRandom();
            break;
        }
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

        xmax = std::max(bounds.x1, bounds.x2);
        xmin = std::min(bounds.x1, bounds.x2);
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

// template <class ColorT>
// class my_test_gradient : public gradient_base<ColorT>
// {
//     inline double calculate_param(const double &x, const double &y) override
//     {
//         double cx = this->m_center.x;
//         double cy = this->m_center.y;
//         double t = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) * this->m_factor;
//         if (this->m_oGradientInfo.periods != 0)
//         {
//             t = this->triagle_saw(this->m_oGradientInfo.periods * t);
//         }
//         if (fabs(this->m_oGradientInfo.discrete_step) > FLT_EPSILON)
//         {
//             t = this->m_oGradientInfo.discrete_step * floor(t / this->m_oGradientInfo.discrete_step);
//         }
//         if (t < 0)
//             return 0;
//         if (t > 1)
//             return 1;
//         return t;
//     }
// };

// template <class ColorT>
// class gradient_radial_span : public gradient_base<ColorT>
// {
//     inline double calculate_param(const double &x, const double &y) override
//     {
//         double cx = this->m_center.x;
//         double cy = this->m_center.y;
//         double t = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) * this->m_factor;
//         if (this->m_oGradientInfo.periods != 0)
//         {
//             t = this->triagle_saw(this->m_oGradientInfo.periods * t);
//         }
//         if (fabs(this->m_oGradientInfo.discrete_step) > FLT_EPSILON)
//         {
//             t = this->m_oGradientInfo.discrete_step * floor(t / this->m_oGradientInfo.discrete_step);
//         }
//         if (t < 0)
//             return 0;
//         if (t > 1)
//             return 1;
//         return t;
//     }
// };

// template <class ColorT>
// class conical_gradient_span : public gradient_base<ColorT>
// {
//     inline double calculate_param(const double &x, const double &y) override
//     {
//         double t = 2 * fabs(atan2(double(y), double(x))) / pi;
//         if (this->m_oGradientInfo.periods != 0)
//         {
//             t = this->triagle_saw(this->m_oGradientInfo.periods * t);
//         }
//         if (fabs(this->m_oGradientInfo.discrete_step) > FLT_EPSILON)
//         {
//             t = this->m_oGradientInfo.discrete_step * floor(t / this->m_oGradientInfo.discrete_step);
//         }
//         if (t < 0)
//             return 0;
//         if (t > 1)
//             return 1;
//         return t;
//     }
// };
