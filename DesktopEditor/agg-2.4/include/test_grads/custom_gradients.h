#include <algorithm>
#include <time.h>

#ifndef M_1_PI
#define M_1_PI 0.318309886183790671538
#endif



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
        virtual float eval(float , float ) = 0;
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
        virtual float eval(float , float ) override
        {
            return rand() / (float )RAND_MAX;
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
            float t = fabs(atan2(x, y)) * M_1_PI;
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
            auto w = baricentric_weights({x, y}, ginfo.shading.triangle);
            return w[0] * ginfo.shading.triangle_parametrs[0] +
                   w[1] * ginfo.shading.triangle_parametrs[1] +
                   w[2] * ginfo.shading.triangle_parametrs[2];
        }

        /**
         *  Вычисляем веса по барицетрическим координатам
         *  https://codeplea.com/triangular-interpolation
        */
        static std::vector<float>
        baricentric_weights(const NSStructures::Point &P, const std::vector<NSStructures::Point> &t)
        {
            std::vector<float> r(3);
            r[0] = ( (t[1].y - t[2].y) * (P.x - t[2].x) + (t[2].x - t[1].x) * (P.y - t[2].y) ) /
                    ((t[1].y - t[2].y) * (t[0].x - t[2].x) + (t[2].x - t[1].x) * (t[0].y - t[2].y));
            r[1] = ( (t[2].y - t[0].y) * (P.x - t[2].x) + (t[0].x - t[2].x) * (P.y - t[2].y) ) /
                    ((t[1].y - t[2].y) * (t[0].x - t[2].x) + (t[2].x - t[1].x) * (t[0].y - t[2].y));
            r[2] = 1 - r[0] - r[1];
            return r;
        }
        NSStructures::GradientInfo ginfo;
    };

    class calcCurve : public calcBase
    {
        /** 
         * Пока наивный метод, из недостатков - долгая инициализация, из плюсов если рендерить несколько раз
         * то будет быстро, достаточно будет преобразование старых координат в новые. И как то сохранить результат инициализации.
         * Пока не буду ничего этого делать, тк. наверное все равно придется поменять.
         */
    public:
        calcCurve(const NSStructures::GradientInfo &_g, bool calculate_tensor_coefs = true) : tensor_size(4)
        {
            ginfo = _g;
            if (calculate_tensor_coefs)
                calculate_tensor();
            RES = 10; //ginfo.shading.function.get_resolution();
            float delta = 1.0 / RES;
            float u = 0, v = 0;
            auto start_p = get_p(u, v);
            xmax = xmin = start_p.x;
            ymax = ymin = start_p.y;
            precalc = std::vector<std::vector<float>>(RES, std::vector<float>(RES, 0.0f));
            for (;u <= 1; u += delta) {
                for (;v <= 1; v+= delta) {
                    auto p = get_p(u, v);
                    xmax = std::max(p.x, xmax);
                    ymax = std::max(p.y, ymax);
                    xmin = std::min(p.x, xmin);
                    ymin = std::min(p.y, ymin);
                }
            }
            for (u = 0.0; u <= 1; u += delta) {
                for (v = 0.0; v <= 1; v+= delta) {
                    auto p = get_p(u, v);
                    auto i = get_index(p.x, p.y);
                    if (i.first == 0 || i.first == RES - 1 || i.second == 0 || i.second == RES - 1)
                        precalc[i.first][i.second] = 0;
                    else 
                        precalc[i.first][i.second] = 1;
                }
            }
        }
        virtual float eval(float x, float y) override
        {
            auto i = get_index(x, y);
            return precalc[i.first][i.second];
        }

    private:
        NSStructures::Point get_p(float u, float v)
        {
            NSStructures::Point p;
            for (int i = 0; i < tensor_size; i++)
            {
                for (int j = 0; j < tensor_size; j++) {
                    p+= ginfo.shading.patch[i][j] * berstein_polynomial(u, i) * berstein_polynomial(v, j);
                }
            }
            return p;
        }        
        float berstein_polynomial(float t, int i) 
        {
            switch (i)
            {
            case 0:
                return (1 - t) * (1 - t) * (1- t);
            case 1:
                return 3 * t * (1 - t) * (1- t);
            case 2:
                return 3 * t * t * (1- t);
            case 3:
                return t * t * t;
            
            default:
                printf("!!!!!!");
            }
        }
        
        // pdf 208p.
        void calculate_tensor()
        {
            auto p = ginfo.shading.patch;
            //-----------------------------------------------------------------------------
            p[1][1] = (1.0 / 9) * ( -4.0 * p[0][0] + 6 * ( p[0][1] + p[1][0] ) 

                        - 2 * ( p[0][3] + p[3][0] ) + 3 * ( p[3][1] + p[1][3] ) - p[3][3] );
            //-----------------------------------------------------------------------------
            p[1][2] = (1.0 / 9) * ( -4.0 * p[0][3] + 6 * ( p[0][2] + p[1][3] ) 

                        - 2 * ( p[0][0] + p[3][3] ) + 3 * ( p[3][2] + p[1][0] ) - p[3][0] );
            //-----------------------------------------------------------------------------
            p[2][1] = (1.0 / 9) * ( -4.0 * p[3][0] + 6 * ( p[3][1] + p[2][0] ) 

                        - 2 * ( p[3][3] + p[0][0] ) + 3 * ( p[0][1] + p[2][3] ) - p[0][3] );
            //-----------------------------------------------------------------------------
            p[2][2] = (1.0 / 9) * ( -4.0 * p[3][3] + 6 * ( p[3][2] + p[2][3] ) 

                        - 2 * ( p[3][0] + p[0][3] ) + 3 * ( p[0][2] + p[2][0] ) - p[0][0] );
            //-----------------------------------------------------------------------------
            ginfo.shading.patch = p;
        }
        
        std::pair<size_t, size_t> get_index(float x, float y)
        {
            size_t x_index = (size_t)(RES - 1) * (x - xmin) / (xmax - xmin);
            if (x_index < 0)
                x_index = 0;
            if (x_index > RES - 1)
                x_index = RES - 1;

            size_t y_index = (size_t)(RES - 1) * (y - ymin) / (ymax - ymin);
            if (y_index < 0)
                y_index = 0;
            if (y_index > RES - 1)
                y_index = RES - 1;
            
            return {x_index, y_index};
        }

        NSStructures::GradientInfo ginfo;
        std::vector<std::vector<float>> precalc;
        const size_t tensor_size;
        float xmin, xmax, ymin, ymax;
        size_t RES;
    };

    template <class ColorT>
    class gradient_base
    {
    public:
        typedef ColorT color_type;
    protected:
        enum
        {
            StateInit = 0,
            StateReady = 1,

            MaxColorIndex = 512
        };

    protected:
        int m_state;

        NSStructures::GradientInfo m_oGradientInfo;
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

        /** 
         * Выбор варианта параметризации. И получение основной инфы о градиенте. 
         */
        void SetGradientInfo(const NSStructures::GradientInfo &_g, Aggplus::BrushType bType)
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

            case Aggplus::BrushTypeTriagnleMeshGradient:
                calculate = new calcTriangle(m_oGradientInfo);
                break;

            case Aggplus::BrushTypeCurveGradient:
                calculate = new calcCurve(m_oGradientInfo);
                break;

            case Aggplus::BrushTypeTensorCurveGradient:
                calculate = new calcCurve(m_oGradientInfo, false);
                break;

            default:
                fprintf(stderr, "WRONG BRUSH TYPE");
                calculate = new calcRandom();
                break;
            }
        }

        /** 
         * Для старой цветовой функции. 
         */
        void SetSubColors(const color_type *colors, const float *positions, int count)
        {
            m_pSubColors = colors;
            m_pPosSubColors = positions;
            m_nCountSubColors = count;
        }

        /** 
         * Нужно для функции отрисовки. 
         */
        void prepare()
        {
            if (m_state != StateReady)
            {
                m_state = StateReady;
                memset(m_valid_table, 0, sizeof m_valid_table);
            }
        }

        /** 
         * Генерирует цвет пикселя для рендерной функции. 
         */
        void generate(color_type *span, int x, int y, unsigned len)
        {
            for (unsigned count = 0; count < len; ++count, ++x)
            {
                if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::FunctionOnly) 
                {
                    float _x = x;
                    float _y = y;
                    _x = (_x - xmin) / (xmax - xmin);
                    _y = (_y - ymin) / (ymax - xmin);
                    *span++ = m_oGradientInfo.shading.function.get_color(_x, _y); //m_color_table[index];
                }
                else if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::TriangleInterpolation)
                {
                    *span++ = this->triangle(x, y);
                }
                else if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::CurveInterpolation)
                {
                    *span++ = this->triangle(x,y); // todo
                }
                else if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::TesorCurveInterpolation)
                {
                    *span++ = this->triangle(x,y); // todo
                }
                else if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::Parametric)
                {
                    float t = calculate_param(x,y);
                    if (m_oGradientInfo.shading.f_type == NSStructures::ShadingInfo::UseNew)
                    {
                        *span++ = m_oGradientInfo.shading.function.get_color(t);
                    }
                    if (m_oGradientInfo.shading.f_type == NSStructures::ShadingInfo::UseOld)
                    {
                        int index = int(t * MaxColorIndex + 0.5);
                        if (!m_valid_table[index])
                            CalcColor(index);
                        *span++ = m_color_table[index];
                    }
                }
            }
        }
        
        /** 
         * Выставляем всякую инфу про наш градиент. 
         */
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
        /**
         * Для старой цветовой функции
         */
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

        /** 
         * Треугольная пила, нужна для периодических градиентов. 
         */
        inline float triagle_saw(float x)
        {
            return fabs(2 * asinf(sinf(x * pi)) * M_1_PI);
        }

        int limit8bit(int a) {
            if (a >= 0x100)
                return 0xFF;
            return a;
        }
        /** 
         * Умножение цвета на число (теплейт поэтому не перегрузка *) 
         */
        ColorT mul(ColorT c1, float t)
        {
            return ColorT(limit8bit(c1.r * t), limit8bit(c1.g * t), limit8bit(c1.b * t), limit8bit(c1.a * t));
        }
        /** 
         * Сумма двух цветов 
         */
        ColorT sum(ColorT c1, ColorT c2) 
        {
            return ColorT(limit8bit(c1.r + c2.r), limit8bit(c1.g + c2.g), 
                        limit8bit(c1.b + c2.b), limit8bit(c1.a + c2.a));
        }
        /** 
         * Треугольная интерполяция на цветовой функции.
         * Вычисляется по барицентрическим координатам. 
         */
        ColorT triangle(float x, float y)
        {
            auto w = calcTriangle::baricentric_weights({x, y}, m_oGradientInfo.shading.triangle);
            ColorT c1 = mul(m_oGradientInfo.shading.triangle_colors[0], fabs(w[0]));
            ColorT c2 = mul(m_oGradientInfo.shading.triangle_colors[1], fabs(w[1]));
            ColorT c3 = mul(m_oGradientInfo.shading.triangle_colors[2], fabs(w[2]));
            return sum(c1, sum(c2,c3));
        }


    };
}
