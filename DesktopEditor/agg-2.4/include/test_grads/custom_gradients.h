namespace agg
{
    /**
     *  Основной смысл в том, что есть два этапа обсчета, это параметризация и вычисление цвета.
     * 
     *  В частности радиальный и линейный градиенты, требуют параметризацию, (x,y)-> t,
     * чтобы затем перевести параметр в цвет.
     * 
     * У шейдеров параметризация опциональная, поэтому для них отдельно есть способ с параметризацией и без нее
     * 
     * */

    class calcBase
    {
    public:
        virtual float eval(float, float) = 0;
        virtual ~calcBase() {}
        void set_rotation(float angle)
        {
            ca = cosf(angle);
            sa = sinf(angle);
        }
        void rotate(float &x, float &y)
        {

            float newx = x * ca + y * sa;
            float newy = -x * sa + y * ca;
            x = newx;
            y = newy;
        }

    private:
        float ca, sa;
    };

    class calcRandom : public calcBase
    {
    public:
        calcRandom()
        {
            srand(time(NULL));
        }
        virtual float eval(float, float) override
        {
            return rand() / (float)RAND_MAX;
        }
    };

    class calcRadial : public calcBase
    {
    public:
        calcRadial(const NSStructures::GradientInfo &_gi, float _cx, float _cy, float _factor) : ginfo(_gi), cx(_cx), cy(_cy), factor(_factor), inverseFactor(1.0f / _factor),
                                                                                                 invXsize(1.0f / _gi.xsize), invYsize(1.0f / _gi.ysize)
        {
            cx += ginfo.centerX * inverseFactor;
            cy += ginfo.centerY * inverseFactor;
            set_rotation(ginfo.angle);
        }

        virtual float eval(float x, float y) override
        {
            x -= cx;
            y -= cy;
            rotate(x, y);
            x *= invXsize;
            y *= invYsize;
            float t = sqrt(x * x + y * y) * factor;
            return t;
        }
        virtual ~calcRadial() {}

    private:
        NSStructures::GradientInfo ginfo;
        float cx, cy, factor;
        float inverseFactor;
        float invXsize, invYsize;
    };

    class calcConical : public calcBase
    {
    public:
        calcConical(const NSStructures::GradientInfo &_gi, float _cx, float _cy, float _factor) : ginfo(_gi), cx(_cx), cy(_cy), factor(_factor), inverseFactor(1.0f / _factor),
                                                                                                  invXsize(1.0f / _gi.xsize), invYsize(1.0f / _gi.ysize), m1pi((float)M_1_PI)
        {
            cx += ginfo.centerX * inverseFactor;
            cy += ginfo.centerY * inverseFactor;
            set_rotation(ginfo.angle);
        }
        virtual float eval(float x, float y) override
        {
            x -= cx;
            y -= cy;
            rotate(x, y);
            x *= invXsize;
            y *= invYsize;
            float t = fabs(atan2(x, y)) * m1pi;
            return t;
        }

    private:
        NSStructures::GradientInfo ginfo;
        float cx, cy, factor;
        float inverseFactor;
        float invXsize, invYsize;
        float m1pi;
    };

    class calcDiamond : public calcBase
    {
    public:
        calcDiamond(const NSStructures::GradientInfo &_gi, float _cx, float _cy, float _factor)
            : ginfo(_gi), cx(_cx), cy(_cy), factor(_factor), inverseFactor(1.0f / _factor),
              invXsize(1.0f / _gi.xsize), invYsize(1.0f / _gi.ysize)
        {
            cx += ginfo.centerX * inverseFactor;
            cy += ginfo.centerY * inverseFactor;
            set_rotation(ginfo.angle);
        }
        virtual float eval(float x, float y) override
        {
            x -= cx;
            y -= cy;
            rotate(x, y);
            x *= invXsize;
            y *= invYsize;

            float t = std::max(fabs(x * factor), fabs(y * factor));
            return t;
        }

    private:
        NSStructures::GradientInfo ginfo;
        float cx, cy, factor;
        float inverseFactor;
        float invXsize, invYsize;
    };

    class calcNewLinear : public calcBase
    {
    public:
        calcNewLinear(const NSStructures::GradientInfo &_gi,
                      float _xmin, float _ymin,
                      float _xmax, float _ymax) : ginfo(_gi),
                                                  xmin(_xmin), ymin(_ymin), xmax(_xmax), ymax(_ymax)
        {
            cx = (xmin + xmax) * 0.5;
            cy = (ymin + ymax) * 0.5;
            xlen = xmax - xmin;
            invXlen = 1.0f / xlen;
            invStretch = 1.0f / ginfo.linstretch;
            set_rotation(ginfo.angle);
        }
        virtual float eval(float x, float y) override
        {
            x -= cx;
            y -= cy;
            rotate(x, y);
            float t = (x + 0.5 * xlen) * invXlen * invStretch - ginfo.linoffset;
            return t;
        }

    private:
        float xmin, ymin, xmax, ymax;
        float cx, cy, xlen;
        float invXlen, invStretch;
        NSStructures::GradientInfo ginfo;
    };

    class calcTriangle : public calcBase
    {
    public:
        calcTriangle(const NSStructures::GradientInfo &_g)
        {
            ginfo = _g;
        }
        virtual float eval(float x, float y) override
        {
            // тут можно любую интерполяцию сделать, там не написано какая должна быть
            // пока просто по проекциям берет среднее todo refactor
            float t1 = relative_project_len(x, y, ginfo.shading.triangle[0], ginfo.shading.triangle[1]);
            float t2 = relative_project_len(x, y, ginfo.shading.triangle[1], ginfo.shading.triangle[2]);
            float t3 = relative_project_len(x, y, ginfo.shading.triangle[2], ginfo.shading.triangle[0]);
            return 0.333333333 * (ginfo.shading.triangle_parametrs[0] * (t2 + 1 - t1) + 
            ginfo.shading.triangle_parametrs[1] * (t3 + 1 - t2) + 
            ginfo.shading.triangle_parametrs[2] * (t1 + 1 - t3));
        }
        // x first, наверное это стоит немного отрефакторить пока просто пробую чтобы работало
        //todo
        static float relative_project_len(float x, float y, std::pair<float, float> start, std::pair<float, float> finish)
        {
            return ((x - start.first) * (finish.first - start.first) + (y - start.second) * (finish.second - start.second))
             / ((finish.first - start.first) * (finish.first - start.first) +
             (finish.second - start.second) * (finish.second - start.second)); //(a, b) / b^2
        }
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
        float invStep;
        agg::point_d m_center;
        float m_factor;
        calcBase *calculate;

        agg::trans_affine m_trans;

    protected:
        const color_type *m_pSubColors;
        const float *m_pPosSubColors;
        int m_nCountSubColors;
        float xmin, xmax;
        float ymin, ymax;
        color_type m_color_table[MaxColorIndex + 1];
        bool m_valid_table[MaxColorIndex + 1];

    protected:
        inline float calculate_param(const float &x, const float &y)
        {
            float t = calculate->eval(x, y);

            if (m_oGradientInfo.discrete_step > FLT_EPSILON)
            {
                if (t >= 1)
                    t -= FLT_EPSILON;
                t = m_oGradientInfo.discrete_step * floor(t * invStep) + 0.5 * m_oGradientInfo.discrete_step;
            }

            if (m_oGradientInfo.periodic)
            {
                t = triagle_saw(m_oGradientInfo.periods * t);
            }

            if (m_oGradientInfo.reflected)
            {
                t *= 2;
                if (t > 1 + FLT_EPSILON)
                {
                    t = 2. - t;
                }
            }
            if (t > m_oGradientInfo.largeRadius)
                return 1;
            if (t < m_oGradientInfo.littleRadius)
                return 0;
            if (m_oGradientInfo.largeRadius - m_oGradientInfo.littleRadius < FLT_EPSILON)
                return t;

            t = (t - m_oGradientInfo.littleRadius) / (m_oGradientInfo.largeRadius - m_oGradientInfo.littleRadius); // TODO optimize
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
            invStep = 1.0f / m_oGradientInfo.discrete_step;
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
                calculate = new calcNewLinear(m_oGradientInfo, xmin, ymin, xmax, ymax);
                break;
            case Aggplus::BrushTypeMyTestGradient:
                calculate = new calcTriangle(m_oGradientInfo);
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
                float t = calculate_param((float)_x, (float)_y);
                if (m_oGradientInfo.shading.parametrised_triangle_shading) {
                    *span++ = this->triangle(x, y);
                }
                else if (m_oGradientInfo.shading.f_type == NSStructures::ShadingInfo::UseXY2Color)
                {
                    _x = (_x - xmin) / (xmax - xmin);
                    _y = (_y - ymin) / (ymax - xmin);
                    *span++ = m_oGradientInfo.shading.function.get_color(_x, _y); //m_color_table[index];
                }
                else if (m_oGradientInfo.shading.f_type == NSStructures::ShadingInfo::UseT2Color)
                {
                    *span++ = m_oGradientInfo.shading.function.get_color(t);
                }
                else
                {
                    int index = int(t * MaxColorIndex + 0.5);
                    if (!m_valid_table[index])
                        CalcColor(index);
                    *span++ = m_color_table[index];
                }
            }
        }
        //todo refactor
        ColorT mul(ColorT c1, float t)
        {
            return ColorT(c1.r * t, c1.g * t, c1.b * t, c1.a * t);
        }
        ColorT sum(ColorT c1, ColorT c2) 
        {
            return ColorT(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a);
        }
        ColorT triangle(float x, float y)
        {
            float t1 = calcTriangle::relative_project_len(x, y, m_oGradientInfo.shading.triangle[0], m_oGradientInfo.shading.triangle[1]);
            float t2 = calcTriangle::relative_project_len(x, y, m_oGradientInfo.shading.triangle[1], m_oGradientInfo.shading.triangle[2]);
            float t3 = calcTriangle::relative_project_len(x, y, m_oGradientInfo.shading.triangle[2], m_oGradientInfo.shading.triangle[0]);

            ColorT c1 = mul(m_oGradientInfo.shading.triangle_colors[0], (t2 + 1 - t1)/3);
            ColorT c2 = mul(m_oGradientInfo.shading.triangle_colors[1], (t3 + 1 - t2)/3);
            ColorT c3 = mul(m_oGradientInfo.shading.triangle_colors[2], (t2 + 1 - t1)/3);
            return sum(c1, sum(c2,c3));
        }
        void SetDirection(const agg::rect_d &bounds, const agg::trans_affine &trans)
        {
            m_trans = trans;

            xmax = std::max(bounds.x1, bounds.x2);
            xmin = std::min(bounds.x1, bounds.x2);
            ymax = std::max(bounds.y1, bounds.y2);
            ymin = std::min(bounds.y1, bounds.y2);
            m_center.x = (bounds.x1 + bounds.x2) / 2;
            m_center.y = (bounds.y1 + bounds.y2) / 2;

            float dmax = (abs(bounds.x1 - bounds.x2));
            if (dmax < abs(bounds.y1 - bounds.y2))
                dmax = abs(bounds.y1 - bounds.y2);
            m_factor = 0.0f;

            if (dmax > FLT_EPSILON)
                m_factor = 2.0f / dmax;
        }

    protected:
        void CalcColor(int index)
        {
            float t = index * (1.0 / MaxColorIndex);

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

        inline float triagle_saw(float x)
        {
            return fabs(2 * asinf(sinf(x * pi)) * M_1_PI);
        }
    };
}