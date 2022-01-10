#include <algorithm>
#include <time.h>
#include <math.h>
#include "../../../graphics/AggPlusEnums.h"
#include "../../../graphics/structures.h"
#include <cmath>
#ifndef M_1_PI
#define M_1_PI 0.318309886183790671538
#endif

#ifndef NAN_FLOAT
#ifdef NAN
#define NAN_FLOAT NAN
#else
#define NAN_FLOAT 0.0f/0.0f
#endif
#endif

#ifndef SHADING_PDF
#define SHADING_PDF

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


    /** 
     * Абстрактный класс для параметризации.
     * 
     * Чтобы показать что данный пиксель не надо закрашивать используется nan 
     */
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

    /** 
     * Параметр радисального градиента. 
     */
    class calcRadial : public calcBase
    {
    public:
        calcRadial(const NSStructures::GradientInfo &_gi) : extend_to_inf_constant(10)
        {
            r0 = _gi.r0;
            r1 = _gi.r1;
            p0 = _gi.p0;
            p1 = _gi.p1;
            ginfo = _gi;
            extend_back_coef = extend_front_coef = 0;
           if (ginfo.continue_shading_b)
           {
               if (r0 < r1)
               {
                    extend_back_coef = r0 / (r1 - r0);
               }

                else
                {
                    extend_back_coef = extend_to_inf_constant;
                }
           }

            if (ginfo.continue_shading_f)
            {
                if (r1 < r0)
               {
                    extend_front_coef = r1 / (r0 - r1);
               }

                else
                {
                    extend_front_coef = extend_to_inf_constant;
                }
            }

            var_segment = 1 / (1 + extend_back_coef + extend_front_coef);
        }

        virtual float eval(float x, float y) override
        {
            //return ginfo.shading.function.get_x_max();
            // if (r0 * r0 > (x - p0.x)*(x - p0.x) + (y - p0.y)*(y - p0.y))
            // {
            //     //return ginfo.shading.function.get_x_min() - 1;
            // }
            // if (r1 * r1 < (x - p1.x)*(x - p1.x) + (y - p1.y)*(y - p1.y))
            // {
            //     //return ginfo.shading.function.get_x_max() + 1;
            // }

            float a = (r1 - r0) * (r1 - r0) - (p1.x - p0.x) * (p1.x - p0.x) - (p1.y - p0.y) * (p1.y - p0.y);

            float b = 2 * r0 * (r1 - r0) + 2 * (p1.x - p0.x) * (x - p0.x) + 2 * (p1.y - p0.y) * (y - p0.y);

            float c = r0 * r0 - (x - p0.x) * (x - p0.x) - (y - p0.y) * (y - p0.y);

            float D = b * b - 4 * a * c;
            if (D < 0)
            {
                return NAN_FLOAT;
            }
            float x2 = (-b + sqrtf(D)) / 2 / a; 
            float x1 = (-b - sqrtf(D)) / 2 / a;

            if (abs(a) < FLT_EPSILON)
                x1 = -c / b;

            
            if (0 <= x1 && x1 <= 1)
            {
                return ginfo.shading.function.get_x_min() + 
                    (ginfo.shading.function.get_x_max() - ginfo.shading.function.get_x_min()) * x1;
            }
            if (0 <= x2 && x2 <= 1)
            {
                 return ginfo.shading.function.get_x_min() + 
                    (ginfo.shading.function.get_x_max() - ginfo.shading.function.get_x_min()) * x2;
            }
            
            if (ginfo.continue_shading_b && 
                (0 >= x1 && x1 >= -extend_back_coef ||
                0 >= x2 && x2 >= -extend_back_coef))
            {
                return ginfo.shading.function.get_x_min();
            }


            if (ginfo.continue_shading_f &&
                (1 <= x1 && x1 <= 1 + extend_front_coef ||
                1 <= x2 && x2 <= 1 + extend_front_coef))
            {
                return ginfo.shading.function.get_x_max();
            }
            
            return NAN_FLOAT;
        }
        virtual ~calcRadial() {}

    private:
        float r0, r1;
        NSStructures::Point p0,p1;
        NSStructures::GradientInfo ginfo;

        float extend_front_coef, extend_back_coef;
        float var_segment;
        float extend_to_inf_constant;
    };

     /** 
     * Параметр конусного градиента. Не используется в пдф. 
     */
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
    
    /** 
     * Параметр квадратного градиента. Не используется в пдф. 
     */
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

    /** 
     * Параметр линейного градиента.
     */
    class calcNewLinear : public calcBase
    {
    public:
        calcNewLinear(const NSStructures::GradientInfo &_gi) : ginfo(_gi)
        {
            p0 = _gi.shading.point1;
            p1 = _gi.shading.point2;
        }
        virtual float eval(float x, float y) override
        {

            float t = (x - p0.x) * (p1.x - p0.x) + (y - p0.y) * (p1.y - p0.y);
            t /= (p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y);

            if (t < 0 && ginfo.continue_shading_b)
                return ginfo.shading.function.get_x_min();

            if (t > 1 && ginfo.continue_shading_f)
                return ginfo.shading.function.get_x_max();

            t = ginfo.shading.function.get_x_min() + 
                (ginfo.shading.function.get_x_max() - ginfo.shading.function.get_x_min()) * t;
            return t; 
        }

    private:

        NSStructures::Point p0, p1;
        float cx, cy, len;
        float invXlen, invStretch;
        NSStructures::GradientInfo ginfo;
    };

    /** 
     * Треугольный шейдинг, закрашивает всю область.  
     */
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
            return w[0] * ginfo.shading.triangle_parameters[0] +
                   w[1] * ginfo.shading.triangle_parameters[1] +
                   w[2] * ginfo.shading.triangle_parameters[2];
        }

        /**
         *  Вычисляем веса по барицетрическим координатам
         *  https://codeplea.com/triangular-interpolation
        */
        static std::vector<float>
        baricentric_weights(const NSStructures::Point &P, const std::vector<NSStructures::Point> &t)
        {
            std::vector<float> r(3);
            r[0] = ((t[1].y - t[2].y) * (P.x - t[2].x) + (t[2].x - t[1].x) * (P.y - t[2].y)) /
                   ((t[1].y - t[2].y) * (t[0].x - t[2].x) + (t[2].x - t[1].x) * (t[0].y - t[2].y));
            r[1] = ((t[2].y - t[0].y) * (P.x - t[2].x) + (t[0].x - t[2].x) * (P.y - t[2].y)) /
                   ((t[1].y - t[2].y) * (t[0].x - t[2].x) + (t[2].x - t[1].x) * (t[0].y - t[2].y));
            r[2] = 1 - r[0] - r[1];
            return r;
        }
        NSStructures::GradientInfo ginfo;
    };

    /** 
     * Шейдинг гладкой поверхности, закрашивает только саму поверхность.
     * 
     * Алгоритм работы примерно следующий. 
     * Проходимся по плоскости значений параметра, и для каждой точки закрашиваем квадрат с границой.
     * 
     * P(u,v), P(u + du, u + dv) - углы.
     * 
     * Это позволяет изьежать вычисления обратной функции, которую можно в общем случаем почтитать только методом Ньютона.
     */
    class calcCurve : public calcBase
    {
    public:
        // В конструкторе происходит весь прекалк, т.е. экзепляр класса полностью готов к работе.
        calcCurve(const NSStructures::GradientInfo &_g, bool calculate_tensor_coefs = true) : tensor_size(4)
        {
            ginfo = _g;
            if (calculate_tensor_coefs)
                calculate_tensor();
            RES = ginfo.shading.function.get_resolution();
            float delta = 1.0 / RES;
            float u = 0, v = 0;
            auto start_p = get_p(u, v);
            xmax = xmin = start_p.x;
            ymax = ymin = start_p.y;
            for (; u <= 1; u += delta)
            {
                for (v = 0; v <= 1; v += delta)
                {
                    auto p = get_p(u, v);
                    xmax = std::max(p.x, xmax);
                    ymax = std::max(p.y, ymax);
                    xmin = std::min(p.x, xmin);
                    ymin = std::min(p.y, ymin);
                }
            }

            int nRES = (int)std::max(ymax - ymin, xmax - xmin);
            if (nRES <= 0)
            {
                RES = 1;
            }
            else 
            {
                RES = nRES;
            }
            precalc = std::vector<std::vector<float>>(RES, std::vector<float>(RES, NAN_FLOAT));
            delta = 1.0f / RES;
            std::vector<std::pair<int, int>> next_indexes(RES + 1);
            u = 0;
            for (int i = 0; i < RES; ++i)
            {   
                v = 0;
                std::vector<std::pair<int, int>> cur_next_indexes(RES + 1);
                for (int j = 0; j < RES; ++j)
                {
                    NSStructures::Point p;
                    std::pair<int, int> index1;
                    if (i == 0 || j == 0)
                    {
                        p = get_p(u, v);
                        index1 = get_index(p.x, p.y);
                    }
                    else
                    {
                        index1 = next_indexes[j];
                    }
                    
                    p = get_p(u + delta, v + delta);
                    std::pair<int, int> index2 = get_index(p.x, p.y);
                    cur_next_indexes[j + 1] = index2;

                    float t =
                        ginfo.shading.patch_parameters[0][0] * (1 - v) * (1 - u) +
                        ginfo.shading.patch_parameters[0][1] * (1 - v) * u +
                        ginfo.shading.patch_parameters[1][0] * v * (1 - u) +
                        ginfo.shading.patch_parameters[1][1] * v * u;

                    fill_square(index1, index2, t);
                    v += delta;
                }
                next_indexes = cur_next_indexes;
                u += delta;
            }
        }

        // Заполняем квадрат в один цвет, чтобы небыло пропусков.
        void fill_square(std::pair<int, int> index1, std::pair<int, int> index2, float t)
        {
            for (int i = std::min(index1.first, index2.first); i <= std::max(index1.first, index2.first); i++)
            {
                for (int j = std::min(index1.second, index2.second); j <= std::max(index1.second, index2.second); j++)
                {
                    if (i < RES && j < RES)
                    {
                        precalc[i][j] = t;
                    }
                }
            }
        }

        virtual float eval(float x, float y) override
        {
            auto i = get_index(x, y);
            if (i.first > RES - 1 || i.second > RES - 1 || i.first < 0 || i.second < 0)
                return NAN_FLOAT; // Не закрашиваем

            return precalc[i.first][i.second];
        }

      
        // Через полинамы такого вида определяется нужная нам поверхность.  
        static float berstein_polynomial(float t, int i)
        {
            switch (i)
            {
            case 0:
                return (1 - t) * (1 - t) * (1 - t);
            case 1:
                return 3 * t * (1 - t) * (1 - t);
            case 2:
                return 3 * t * t * (1 - t);
            case 3:
                return t * t * t;
            }
        }

    private:
        NSStructures::Point get_p(float u, float v)
        {
            NSStructures::Point p;
            for (int i = 0; i < tensor_size; i++)
            {
                for (int j = 0; j < tensor_size; j++)
                {
                    p += ginfo.shading.patch[i][j] * berstein_polynomial(u, i) * berstein_polynomial(v, j);
                }
            }
            return p;
        }

        /** 
         * т.к. шестой тип шейдинга по сути является частным случаем седьмого, только с пересчетом некоторый точек.
         * Этот метод позволяет вычислить нужные параметры, подробнее в стандарте пдф 208с. 
         */
        void calculate_tensor()
        {
            auto p = ginfo.shading.patch;
            //-----------------------------------------------------------------------------
            p[1][1] = (1.0 / 9) * (-4.0 * p[0][0] + 6 * (p[0][1] + p[1][0])

                                   - 2 * (p[0][3] + p[3][0]) + 3 * (p[3][1] + p[1][3]) - p[3][3]);
            //-----------------------------------------------------------------------------
            p[1][2] = (1.0 / 9) * (-4.0 * p[0][3] + 6 * (p[0][2] + p[1][3])

                                   - 2 * (p[0][0] + p[3][3]) + 3 * (p[3][2] + p[1][0]) - p[3][0]);
            //-----------------------------------------------------------------------------
            p[2][1] = (1.0 / 9) * (-4.0 * p[3][0] + 6 * (p[3][1] + p[2][0])

                                   - 2 * (p[3][3] + p[0][0]) + 3 * (p[0][1] + p[2][3]) - p[0][3]);
            //-----------------------------------------------------------------------------
            p[2][2] = (1.0 / 9) * (-4.0 * p[3][3] + 6 * (p[3][2] + p[2][3])

                                   - 2 * (p[3][0] + p[0][3]) + 3 * (p[0][2] + p[2][0]) - p[0][0]);
            //-----------------------------------------------------------------------------
            ginfo.shading.patch = p;
        }

        std::pair<int, int> get_index(float x, float y)
        {
            size_t x_index = (int)(RES - 1) * (x - xmin) / (xmax - xmin);
            size_t y_index = (int)(RES - 1) * (y - ymin) / (ymax - ymin);

            return {x_index, y_index};
        }


        NSStructures::GradientInfo ginfo;
        std::vector<std::vector<float>> precalc;
        const size_t tensor_size;
        float xmin, xmax, ymin, ymax;
        size_t RES;
    };


    /** 
     * Основной класс отвечающий за градиенты. 
     */
    template <class ColorT>
    class gradient_base
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

        NSStructures::GradientInfo m_oGradientInfo;
        float invStep;
        agg::point_d m_center;
        float m_factor;
        calcBase *calculate;

        agg::trans_affine m_trans;

    private:
        const color_type *m_pSubColors;
        const float *m_pPosSubColors;
        int m_nCountSubColors;
        float xmin, xmax;
        float ymin, ymax;
        color_type m_color_table[MaxColorIndex + 1];
        bool m_valid_table[MaxColorIndex + 1];
        bool m_bSwapRGB;

    private:

        // Параметризация, с некторыми обертками.
        inline float calculate_param(const float &x, const float &y)
        {
            float t = calculate->eval(x, y);
            if(std::isnan(t))
                return t;
            
            if (t < m_oGradientInfo.shading.function.get_x_min() && !m_oGradientInfo.continue_shading_b)
                return NAN_FLOAT;
            if (t > m_oGradientInfo.shading.function.get_x_max() && !m_oGradientInfo.continue_shading_f)
                return NAN_FLOAT;

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

            if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::TensorCurveInterpolation)
                initialise_curve(false);

            if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::CurveInterpolation)
                initialise_curve();

            if (m_oGradientInfo.shading.shading_type != NSStructures::ShadingInfo::Parametric)
            {
                return;
            }
            switch (bType)
            {
            case Aggplus::BrushTypeRadialGradient:
                calculate = new calcRadial(m_oGradientInfo);
                break;

            case Aggplus::BrushTypeConicalGradient:
                calculate = new calcConical(m_oGradientInfo, m_center.x, m_center.y, m_factor);
                break;

            case Aggplus::BrushTypeDiamondGradient:
                calculate = new calcDiamond(m_oGradientInfo, m_center.x, m_center.y, m_factor);
                break;

            case Aggplus::BrushTypeNewLinearGradient:
                calculate = new calcNewLinear(m_oGradientInfo);
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

            case Aggplus::BrushTypeMyTestGradient:
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
                    float _x = x ;
                    float _y = y ;
                    _x += m_oGradientInfo.shading.inv_map[4];
                    _y += m_oGradientInfo.shading.inv_map[5];

                    _x = _x * m_oGradientInfo.shading.inv_map[0] + _y * m_oGradientInfo.shading.inv_map[2];
                    _y = _x * m_oGradientInfo.shading.inv_map[1] + _y * m_oGradientInfo.shading.inv_map[3];

                    *span++ = ifswapRGB(m_oGradientInfo.shading.function.get_color(_x, _y)); //m_color_table[index];
                }
                else if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::TriangleInterpolation)
                {
                    *span++ = ifswapRGB(this->triangle(x, y));
                }
                else if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::CurveInterpolation)
                {
                    *span++ = ifswapRGB(this->curve_eval(x, y));
                }
                else if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::TensorCurveInterpolation)
                {
                    *span++ = ifswapRGB(this->curve_eval(x, y));
                }
                else if (m_oGradientInfo.shading.shading_type == NSStructures::ShadingInfo::Parametric)
                {
                    float t = calculate_param(x, y);
                    if (std::isnan(t))
                    {
                        *span++ = {0, 0, 0, 0};
                    }
                    else if (m_oGradientInfo.shading.f_type == NSStructures::ShadingInfo::UseNew)
                    {
                        *span++ = ifswapRGB(m_oGradientInfo.shading.function.get_color(t));
                    }
                    else if (m_oGradientInfo.shading.f_type == NSStructures::ShadingInfo::UseOld)
                    {
                        int index = int(t * MaxColorIndex + 0.5);
                        if (!m_valid_table[index])
                            CalcColor(index);
                        *span++ = ifswapRGB(m_color_table[index]);
                    }
                }
            }
        }

        /** 
         * Выставляем всякую инфу про наш градиент. 
         */
        void SetDirection(const agg::rect_d &bounds, const agg::trans_affine &trans, bool bSwapRGB)
        {
            m_bSwapRGB = bSwapRGB;
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


        // Нужно для безопасного сложения цветов.
        int limit8bit(int a)
        {
            if (a < 0)
                return 0;
            if (a > 0xFF)
                return 0xFF;
            return a;
        }
       
         
        // Умножение цвета на число (теплейт поэтому не перегрузка *) 
        ColorT mul(ColorT c1, float t)
        {
            return ColorT(limit8bit(c1.r * t), limit8bit(c1.g * t), limit8bit(c1.b * t), limit8bit(c1.a * t));
        }
        // Сумма двух цветов 
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
            if (w[0] < 0 || w[1] < 0 || w[2] < 0)
            {
                return {0,0,0,0};
            }
            ColorT c1 = mul(m_oGradientInfo.shading.triangle_colors[0], fabs(w[0]));
            ColorT c2 = mul(m_oGradientInfo.shading.triangle_colors[1], fabs(w[1]));
            ColorT c3 = mul(m_oGradientInfo.shading.triangle_colors[2], fabs(w[2]));
            return sum(c1, sum(c2, c3));
        }


        /** 
         * Здесь все относящееся к кривой поверхности, будет дублирование кода, зато потом в теории будет проще работать.
         * И логика чуть менее запутанная.
         * Основной алгоритм - разбить поверхность на множество квадратиков. И забить туда значения сразу. Чтобы обратную фнкцию не считать.
         *  
         * Основня цель повтора - сделать единый интерфейс параметризации и все остальное.
         * Т.к. просто добавляя разные параметризации, можно добавить большое количесво нужных градиентов.
         * 
         * Все с кривыми работает так же как и параметры, только интерполируется не парамет, а цвет.
         */

        std::vector<std::vector<ColorT>> precalc;
        size_t tensor_size;
        float xmin_curve, xmax_curve, ymin_curve, ymax_curve;
        size_t RES;

        void initialise_curve(bool calculate_tensor_coefs = true)
        {
            tensor_size = 4;
            if (calculate_tensor_coefs)
                calculate_tensor();

            float minxres = m_oGradientInfo.shading.patch[0][0].x;
            float minyres = m_oGradientInfo.shading.patch[0][0].y;
            float maxxres = m_oGradientInfo.shading.patch[0][0].x;
            float maxyres = m_oGradientInfo.shading.patch[0][0].y;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (m_oGradientInfo.shading.patch[i][j].x > maxxres)
                    {
                        maxxres = m_oGradientInfo.shading.patch[i][j].x;
                    }
                    if (m_oGradientInfo.shading.patch[i][j].y > maxyres)
                    {
                        maxyres = m_oGradientInfo.shading.patch[i][j].y;
                    }
                    if (m_oGradientInfo.shading.patch[i][j].x < minxres)
                    {
                        minxres = m_oGradientInfo.shading.patch[i][j].x;
                    }
                    if (m_oGradientInfo.shading.patch[i][j].y < minyres)
                    {
                        minyres = m_oGradientInfo.shading.patch[i][j].y;
                    }
                }
            }

            RES = std::max(1.0f, std::max(maxxres - minxres, maxyres - minyres) / 3);
            float delta = 1.0 / RES;
            float u = 0, v = 0;
            auto start_p = get_p_curve(u, v);
            xmax_curve = xmin_curve = start_p.x;
            ymax_curve = ymin_curve = start_p.y;
            precalc = std::vector<std::vector<ColorT>>(RES, std::vector<ColorT>(RES, {0, 0, 0, 0}));

            /*
             * Небольшая оптимизация основанная на том, что данная фигура не выходит за границы своих опорных точек.
             * */

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (m_oGradientInfo.shading.patch[i][j].x > xmax_curve)
                    {
                        xmax_curve = m_oGradientInfo.shading.patch[i][j].x;
                    }
                    if (m_oGradientInfo.shading.patch[i][j].y > ymax_curve)
                    {
                        ymax_curve = m_oGradientInfo.shading.patch[i][j].y;
                    }
                    if (m_oGradientInfo.shading.patch[i][j].x < xmin_curve)
                    {
                        xmin_curve = m_oGradientInfo.shading.patch[i][j].x;
                    }
                    if (m_oGradientInfo.shading.patch[i][j].y < ymin_curve)
                    {
                        ymin_curve = m_oGradientInfo.shading.patch[i][j].y;
                    }
                }
            }
            int nRES = (int)std::max(ymax - ymin, xmax - xmin);
            if (nRES <= 0)
            {
                RES = 1;
            }
            else 
            {
                RES = nRES;
            }
            precalc = std::vector<std::vector<ColorT>>(RES, std::vector<ColorT>(RES, {0, 0, 0, 0}));
            delta = 1.0f / RES;
             std::vector<std::pair<int, int>> next_indexes(RES + 1);
            u = 0;
            for (int i = 0; i < RES; ++i)
            {   
                v = 0;
                std::vector<std::pair<int, int>> cur_next_indexes(RES + 1);
                for (int j = 0; j < RES; ++j)
                {
                    NSStructures::Point p;
                    std::pair<int, int> index1;
                    if (i == 0 || j == 0)
                    {
                        p = get_p_curve(u + delta / 2, v + delta / 2);
                        index1 = get_index_curve(p.x, p.y);
                    }
                    else
                    {
                        index1 = next_indexes[j];
                    }
                    
                    p = get_p_curve(u + delta, v + delta);
                    std::pair<int, int> index2 = get_index_curve(p.x, p.y);
                    cur_next_indexes[j + 1] = index2;

                    ColorT c00 = mul(m_oGradientInfo.shading.patch_colors[0][0], (1 - u) * (1 - v));
                    ColorT c01 = mul(m_oGradientInfo.shading.patch_colors[0][1], u * (1 - v));
                    ColorT c10 = mul(m_oGradientInfo.shading.patch_colors[1][0], (1 - u) * v);
                    ColorT c11 = mul(m_oGradientInfo.shading.patch_colors[1][1], u * v);
                    fill_square(index1, index2, sum(c00, sum(c01, sum(c10, c11))));
                    v += delta;
                }
                next_indexes = cur_next_indexes;
                u += delta;
            }
        }
        ColorT ifswapRGB(const ColorT &c)
        {


            if (m_bSwapRGB) {
                return c;
            }
            ColorT newC;
            newC.r = c.b;
            newC.g = c.g;
            newC.b = c.r;
            newC.a = c.a;
            return newC;
        }
        ColorT curve_eval(float x, float y)
        {
            auto i = get_index_curve(x, y);
            if (i.first > RES - 1 || i.second > RES - 1 || i.first < 0 || i.second < 0)
                return {0, 0, 0, 0};

            return precalc[i.first][i.second];
        }

        NSStructures::Point get_p_curve(float u, float v)
        {
            NSStructures::Point p;
            for (int i = 0; i < tensor_size; i++)
            {
                for (int j = 0; j < tensor_size; j++)
                {
                    p += m_oGradientInfo.shading.patch[i][j] *
                         calcCurve::berstein_polynomial(u, i) * calcCurve::berstein_polynomial(v, j);
                }
            }
            return p;
        }

        // pdf 208p.
        void calculate_tensor()
        {
            auto p = m_oGradientInfo.shading.patch;
            //-----------------------------------------------------------------------------
            p[1][1] = (1.0 / 9) * (-4.0 * p[0][0] + 6 * (p[0][1] + p[1][0])

                                   - 2 * (p[0][3] + p[3][0]) + 3 * (p[3][1] + p[1][3]) - p[3][3]);
            //-----------------------------------------------------------------------------
            p[1][2] = (1.0 / 9) * (-4.0 * p[0][3] + 6 * (p[0][2] + p[1][3])

                                   - 2 * (p[0][0] + p[3][3]) + 3 * (p[3][2] + p[1][0]) - p[3][0]);
            //-----------------------------------------------------------------------------
            p[2][1] = (1.0 / 9) * (-4.0 * p[3][0] + 6 * (p[3][1] + p[2][0])

                                   - 2 * (p[3][3] + p[0][0]) + 3 * (p[0][1] + p[2][3]) - p[0][3]);
            //-----------------------------------------------------------------------------
            p[2][2] = (1.0 / 9) * (-4.0 * p[3][3] + 6 * (p[3][2] + p[2][3])

                                   - 2 * (p[3][0] + p[0][3]) + 3 * (p[0][2] + p[2][0]) - p[0][0]);
            //-----------------------------------------------------------------------------
            m_oGradientInfo.shading.patch = p;
        }

        std::pair<size_t, size_t> get_index_curve(float x, float y)
        {
            size_t x_index = (size_t)(RES - 1) * (x - xmin_curve) / (xmax_curve - xmin_curve);
            size_t y_index = (size_t)(RES - 1) * (y - ymin_curve) / (ymax_curve - ymin_curve);

            return {x_index, y_index};
        }

        void fill_square(const std::pair<int, int> &index1, const std::pair<int, int> &index2, const ColorT &c)
        {
            for (int i = std::min(index1.first, index2.first); i <= std::max(index1.first, index2.first); i++)
            {
                for (int j = std::min(index1.second, index2.second); j <= std::max(index1.second, index2.second); j++)
                {
                    if (i < RES && j < RES)
                    {
                        precalc[i][j] = c;
                    }
                }
            }
        }
    };
}

#endif
