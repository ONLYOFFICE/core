#include <iostream>
#include <math.h>

#ifndef	M_PI
#define	M_PI		3.14159265358979323846
#endif

#ifndef SHADING_INFO
#define SHADING_INFO

namespace NSStructures
{
    /**
     * 
	 * */
    template <class ColorT = agg::rgba8>
    class ColorFunction
    /**
     *  Реализацию произвольной функции в рантайме я решил сделать как массив, тк так проще всего
     * я еще не совсем понял как точно передается в пдфе функция, но такая реализация, позволяет пользователю
     * выбрать любой способ.
     * 
     *  Пока все копируется, т.к. в большинсве случаев вектор 2кБ по размеру и проблем нету
     * только если использовать двумерную функцию размер возрастает до МБ, но не хочется возится с укузателями
     * ради этого, т.к. судя по всему случай исключительный(только 1 шейдинг требует такую функцию).
     *  Если надо будет, наверно можно будет переписать на юникптр.
     * 
     * Есть возможность выставить обычную линейную интерполяцию, просто для тестирования 
     * + так реализуется градиент стандартный.
     * 
     * Пока у меня конструкторы по умолчанию, чтото заполняют, для тестирования опятьже
     * потом стоит все убрать, чтобы в кисти не таскать это все, когда оно не нужно,
     * если не выделять память то там в сумме будет <100B гдето, не думаю, что это будет так много,
     * чтобы писать отдельный интерфейс для кисти
     * 
     * Плюс я вообще не знаю как обрабатывать, внештатные ситуации, в целом, можно вообще просто
     * эксепшены кидать если что или ничего не делать.
    */
    {
    public:
        ColorFunction() : RESOLUTION(0), x_domain_min(0.0f), x_domain_max(0.0f)
        {

        }

        ColorFunction(size_t res, float xmin, float xmax) : RESOLUTION(res), x_domain_min(xmin), x_domain_max(xmax)

        {
            values = std::vector<std::vector<ColorT>>(1, std::vector<ColorT>(RESOLUTION));
            for (size_t i = 0; i < RESOLUTION; i++)
            {
                unsigned int value = (unsigned int)(255 - (255 * ((float)i / RESOLUTION)));
                values[0][i] = ColorT(value, value, value);
            }
        }

        ColorFunction(size_t res, float xmin, float xmax, float ymin, float ymax) : RESOLUTION(res), x_domain_min(xmin), x_domain_max(xmax), y_domain_min(ymin), y_domain_max(ymax)
        {
            values = std::vector<std::vector<ColorT>>(RESOLUTION, std::vector<ColorT>(RESOLUTION));
            for (size_t i = 0; i < RESOLUTION; i++)
            {
                for (size_t j = 0; j < RESOLUTION; j++)
                {
                    unsigned int value = (unsigned int)(255 * sin(i * j * M_PI / RESOLUTION));
                    values[j][i] = ColorT(255, 0, 0, 255);
                }
            }
        }

        float get_x_min()
        {
            return x_domain_min;
        }
        float get_x_max()
        {
            return x_domain_max;
        }
        float get_y_min()
        {
            return y_domain_min;
        }
        float get_y_max()
        {
            return y_domain_max;
        }
        ColorT get_color(float x)
        {
            int index = get_x_index(x);
            return values[0][index];
        }

        //used only in shading type 1
        ColorT get_color(float x, float y)
        {
            int xi = get_x_index(x);
            int yi = get_y_index(y);
            //std::cout << x << ' ' << y << std::endl;
            return values[yi][xi];
        }

        void set_color(float x, int r, int g, int b, int a)
        {
            int index = get_x_index(x); // pls dont set color out of bounds, it wont crush, but will work not as you max expected
            values[0][index].r = r;
            values[0][index].g = g;
            values[0][index].b = b;
            values[0][index].a = a;
        }
        void set_color(float x, float y, int r, int g, int b, int a)
        {
            int xindex = get_x_index(x);
            int yindex = get_y_index(y);
            values[yindex][xindex].r = r;
            values[yindex][xindex].g = g;
            values[yindex][xindex].b = b;
            values[yindex][xindex].a = a;
        }
        void set_color(size_t xindex, int r, int g, int b, int a)
        {
            values[0][xindex].r = r;
            values[0][xindex].g = g;
            values[0][xindex].b = b;
            values[0][xindex].a = a;
        }
        void set_color(size_t xindex, size_t yindex, int r, int g, int b, int a)
        {
            values[yindex][xindex].r = r;
            values[yindex][xindex].g = g;
            values[yindex][xindex].b = b;
            values[yindex][xindex].a = a;
        }
        // position must be sorted by incr ub otherwise
        // only for 1 in function
        int set_linear_interpolation(const std::vector<uint32_t> &colors, const std::vector<float> &positions)
        {
            if (colors.size() != positions.size())
            {
                return -1; // error
            }
            std::vector<int> indexes;
            for (float x : positions)
            {
                indexes.push_back(get_x_index(x));
            }
            for (int i = 0; i < colors.size(); i++)
            {
                values[0][indexes[i]].r = hex2r(colors[i]);
                values[0][indexes[i]].g = hex2g(colors[i]);
                values[0][indexes[i]].b = hex2b(colors[i]);
                values[0][indexes[i]].a = hex2a(colors[i]);

            }
            for (int i = 0; i < positions.size() - 1; i++) 
            {
                interpolate_indexes(indexes[i], indexes[i + 1]);
            }
            return 0;
        }
        size_t get_resolution() const
        {
            return RESOLUTION;
        }
    private:
        size_t RESOLUTION;
        std::vector<std::vector<ColorT>> values;
        float x_domain_min, x_domain_max;
        float y_domain_min, y_domain_max;
        int get_x_index(float x)
        {
            int x_index = (int)(RESOLUTION - 1) * (x - x_domain_min) / (x_domain_max - x_domain_min);
            if (x_index < 0)
                return 0;
            if (x_index > RESOLUTION - 1)
                return RESOLUTION - 1;
            return x_index;
        }
        int get_y_index(float y)
        {
            int y_index = (int)(RESOLUTION - 1) * (y - y_domain_min) / (y_domain_max - y_domain_min);
            if (values.size() < RESOLUTION)
            {
                return 0;
            }
            if (y_index < 0)
                return 0;
            if (y_index > RESOLUTION - 1)
                return RESOLUTION - 1;
            return y_index;
        }
        /** 
         * Линейная интерполяция для построения цветовой функции. 
         */
        int interpolate_indexes(size_t first, size_t second, size_t line = 0)
        {
            size_t len = second - first;
            ColorT f = values[line][first];
            ColorT s = values[line][second];
            for(size_t i = first + 1; i < second; i++) {
                values[line][i].r = f.r * (1 - (float)(i - first) / len ) + s.r * ((float)(i - first) / len );
                values[line][i].g = f.g * (1 - (float)(i - first) / len ) + s.g * ((float)(i - first) / len ); ;
                values[line][i].b = f.b * (1 - (float)(i - first) / len ) + s.b * ((float)(i - first) / len ); ;
                values[line][i].a = f.a * (1 - (float)(i - first) / len ) + s.a * ((float)(i - first) / len ); ;
            }
            return 0;
        }

        unsigned int hex2a(uint32_t c)
        {
            unsigned int a = (c >> 24) & 0xFF;
            return a;
        }

        unsigned int hex2r(uint32_t c)
        {
            return (c >> 16) & 0xFF;
        }
        unsigned int hex2g(uint32_t c)
        {
            return (c >> 8) & 0xFF;
        }
        unsigned int hex2b(uint32_t c)
        {
            return c & 0xFF;
        }
    };

    struct Point 
    {
        Point():x(0),y(0){}
        Point(const float& _x, const float& _y):x(_x),y(_y){}
        Point(const int& _x, const int& _y):x((float)_x),y((float)_y){}
        Point(const double& _x, const double& _y):x((float)_x),y((float)_y){}
        float x, y;

        Point& operator+=(const Point &a)
        {
            x += a.x;
            y += a.y;
            return *this;
        }


        /** 
         * Костыль от ошибок линковки. Чтобы время не терять пока что.
        */
        friend Point operator*(const Point &a, float t)
        {
            return Point(a.x * t, a.y * t);
        }
        friend Point operator*(float t, const Point &a)
        {
            return Point(a.x * t, a.y * t);
        }
        friend Point operator+(const Point &a, const Point &b)
        {
            return Point(a.x + b.x, a.y + b.y);
        }
        friend Point operator-(const Point &a, const Point &b)
        {
            return Point(a.x - b.x, a.y - b.y);
        }
    };

    

    /**
     *  Тут хранится информация спецефичная для рендера ПДФ.
     * 
     * Взял новую реализацию преобразований, т.к. готовая была на даблах,
     * а в такой точности смысла нету особо.
     * 
     * Для шейдеров требуется поддерживать два способа вычисления (с параметром и без),
     * поэтому требуется много дополнительной инфы.
     * 
     * Так же шейдер будет получать, в качетве параметров, границы, тут я пока не решил, вообще
     * можно оставить соблюдение границ, на откуп пользователю, т.к. все равно заполенение в конечном итоге будет 
     * выполняться с помощью рисования замкнутого пути и команды Fill
     * */
    struct ShadingInfo
    {
        public:
        ShadingInfo() : shading_type(Parametric), f_type(UseNew), inv_map(6){}
        enum ShadingType 
        {
            FunctionOnly,
            Parametric,
            TriangleInterpolation,
            CurveInterpolation,
            TensorCurveInterpolation
        } shading_type;

        // if UseOld old function is called, look for IGraphicsRender.put_BrushGradientColors;
        enum ColorFunctionType
        {
            UseOld, UseNew
        } f_type; 
        ColorFunction<agg::rgba8> function;
        
        // Обратное преобразование из картинки в цветовую функцию
        std::vector<float> mapping;
        std::vector<float> inv_map;

        // Линейный градиент задается в pdf 2 точками
        bool set_two_points;
        Point point1, point2;


        // triangle shading
        std::vector<Point> triangle;
        std::vector<agg::rgba8> triangle_colors;
        std::vector<float> triangle_parameters;

        /** 
         *  Матрица 4 на 4 заполняется как в документации к пдф 7 тип
         * Если выбран тип 6 то значения (1,2) (2,1) (1,1) (2,2)
         * В массиве игнормруется и заполняются автоматически, следите за переданным типом градинта
         * (Нумерация от нуля)
         * 
         * Наверное напишу адаптор который переводит порядок точек из 6 типа в общий.
         */
        std::vector<std::vector<Point>> patch; 
        std::vector<std::vector<agg::rgba8>> patch_colors; // 2 на 2 цвета по углам
        std::vector<std::vector<float>> patch_parameters; // 2 на 2 параметра
    };

    // Containing additional info about gradient
    struct GradientInfo
    {
        GradientInfo() : littleRadius(0.0f), largeRadius(1.0f),
                         centerX(0.0f), centerY(0.0f),
                         angle(0.0f),
                         discrete_step(0.0f),
                         reflected(false),
                         periods(0.5f), periodic(false),
                         xsize(1.0f), ysize(1.0f),
                         linstretch(1.0f), linoffset(0.0f),
                         continue_shading_f(false),
                         continue_shading_b(false)

        {
        }
        void setAngleDegrees(float deg)
        {
            angle = deg / 180.f * (float)M_PI;
        }
        float getAngleDegrees() const
        {
            return angle / (float)M_PI * 180.f;
        }
        void setStepByNum(int n) // recommended to use
        {
            discrete_step = 1.0f / n;
        }

        Point p0, p1;
        float r0, r1;
        
        float littleRadius, largeRadius; 
        float centerX, centerY;          // used in radial, diamond and conical gradient - offset relative to figure center
        float angle;                     // used in linear and conical gradient (rad)
        float discrete_step;             // used to make discrete gradient. <= 0 to make continuous
        float xsize, ysize;              // stretch image; can be negative to reflect relative to other axis; cannot be zero
        bool reflected;                  // 1234567 ->  1357531 works kind of like this
        bool periodic;
        float periods;    // number of periods (best with to colours, works as saw function in color space)
        float linstretch; // stretch linear gradient, can be negative (eq angle = 180) can not be zero
        float linoffset;  // offset relative to image size
        float continue_shading_b, continue_shading_f;
        ShadingInfo shading;
    };


    /** 
     * Создает объект класса GradientInfo по заданным параметрам. 
     * 
     * Цветовую функцию надо заполнять вручную
     */
    class GInfoConstructor {
        public:
        static GradientInfo get_functional(float xmin, float xmax, float ymin, float ymax,
                                            std::vector<float> mapping)
        {
            GradientInfo ginfo;
            ginfo.shading.function = ColorFunction<agg::rgba8>(256, xmin, xmax, ymin, ymax);
            ginfo.shading.f_type = ShadingInfo::UseNew;
            ginfo.shading.shading_type = ShadingInfo::FunctionOnly;

            ginfo.shading.mapping = mapping;

            return ginfo;
        }
        static GradientInfo get_linear(const Point &p1, const Point &p2, float t0 = 0.0f, float t1 = 1.0f, 
                                        bool continue_shading_b = false, bool continue_shading_f = false)
        {
            GradientInfo ginfo;
            ginfo.shading.f_type = ShadingInfo::UseNew;
            ginfo.shading.shading_type = ShadingInfo::Parametric;
            ginfo.continue_shading_f = continue_shading_f;
            ginfo.continue_shading_b = continue_shading_b;

            ginfo.shading.function = ColorFunction<agg::rgba8>(256, t0, t1);

            ginfo.shading.set_two_points = true;
            ginfo.shading.point1 = p1;
            ginfo.shading.point2 = p2;


            return ginfo;
        }
        static GradientInfo get_radial(const Point &c0, const Point &c1, float r0, float r1, 
                                        float t0 = 0.0f, float t1 = 1.0f, 
                                        bool continue_shading_b = false, bool continue_shading_f = false)
        {
            GradientInfo ginfo;
            ginfo.shading.f_type = ShadingInfo::UseNew;
            ginfo.shading.shading_type = ShadingInfo::Parametric;
            ginfo.continue_shading_f = continue_shading_f;
            ginfo.continue_shading_b = continue_shading_b;
            ginfo.shading.function = ColorFunction<agg::rgba8>(256, t0, t1);
            ginfo.p0 = c0;
            ginfo.p1 = c1;
            ginfo.r0 = r0;
            ginfo.r1 = r1;
            return ginfo;


        }
        static GradientInfo get_triangle(const std::vector<Point> &points, 
                                         const std::vector<agg::rgba8> &colors,
                                         const std::vector<float> &params,
                                         bool parametric,
                                         float t0 = 0.0f, float t1 = 1.0f)
        {
            GradientInfo ginfo;
            ginfo.shading.triangle = points;
            if (parametric)
            {
                ginfo.shading.triangle_parameters = params;
                ginfo.shading.f_type = ShadingInfo::UseNew;
                ginfo.shading.function = ColorFunction<agg::rgba8>(256, t0, t1);
                ginfo.shading.shading_type = ShadingInfo::Parametric;
                ginfo.continue_shading_f = false;
                ginfo.continue_shading_b = false;
            }
            else
            {
                ginfo.shading.triangle_colors = colors;
                ginfo.shading.shading_type = ShadingInfo::TriangleInterpolation;
            }
            return ginfo;
        }
        
        /** 
         * Набор из 12 точек для построения границ в порядке указанном в стандарте,
         * Порядок цветов или параметров как в стандарте.  
         */
        static GradientInfo get_curve(const std::vector<Point> &curve_points,
                                        const std::vector<float> &curve_parametrs,
                                        const std::vector<agg::rgba8> &curve_colors,
                                        bool parametric,
                                        float t0 = 0.0f, float t1 = 1.0f)
        {
            GradientInfo ginfo;
            ginfo.shading.patch.resize(4, std::vector<Point>(4));
            ginfo.shading.patch[0][0] = curve_points[0];
            ginfo.shading.patch[0][1] = curve_points[1];
            ginfo.shading.patch[0][2] = curve_points[2];

            ginfo.shading.patch[0][3] = curve_points[3];
            ginfo.shading.patch[1][3] = curve_points[4];
            ginfo.shading.patch[2][3] = curve_points[5];

            ginfo.shading.patch[3][3] = curve_points[6];
            ginfo.shading.patch[3][2] = curve_points[7];
            ginfo.shading.patch[3][1] = curve_points[8];

            ginfo.shading.patch[3][0] = curve_points[9];
            ginfo.shading.patch[2][0] = curve_points[10];
            ginfo.shading.patch[1][0] = curve_points[11];



            if (parametric)
            {
                ginfo.shading.patch_parameters.resize(2, std::vector<float>(2));
                ginfo.shading.patch_parameters[0][0] = curve_parametrs[0];
                ginfo.shading.patch_parameters[0][1] = curve_parametrs[1];
                ginfo.shading.patch_parameters[1][0] = curve_parametrs[3];
                ginfo.shading.patch_parameters[1][1] = curve_parametrs[2];
                ginfo.shading.f_type = ShadingInfo::UseNew;
                ginfo.shading.function = ColorFunction<agg::rgba8>(256, t0, t1);
                ginfo.shading.shading_type = ShadingInfo::Parametric;
                ginfo.continue_shading_f = false;
                ginfo.continue_shading_b = false;
            }
            else 
            {
                ginfo.shading.patch_colors.resize(2, std::vector<agg::rgba8>(2));
                ginfo.shading.patch_colors[0][0] = curve_colors[0];
                ginfo.shading.patch_colors[0][1] = curve_colors[1];
                ginfo.shading.patch_colors[1][0] = curve_colors[3];
                ginfo.shading.patch_colors[1][1] = curve_colors[2];
                ginfo.shading.shading_type = ShadingInfo::CurveInterpolation;
            }
            return ginfo;
        }
        static GradientInfo get_tensor_curve(const std::vector<std::vector<Point>> &curve_poits,
                                             const std::vector<std::vector<float>> &curve_parametrs,
                                             const std::vector<std::vector<agg::rgba8>> &curve_colors,
                                             bool parametric,
                                             float t0 = 0.0f, float t1 = 1.0f)
        {
            GradientInfo ginfo;
            
            ginfo.shading.patch = curve_poits;
            
            if (parametric)
            {
                ginfo.shading.patch_parameters = curve_parametrs;
                ginfo.shading.f_type = ShadingInfo::UseNew;
                ginfo.shading.function = ColorFunction<agg::rgba8>(256, t0, t1);
                ginfo.shading.shading_type = ShadingInfo::Parametric;
                ginfo.continue_shading_f = false;
                ginfo.continue_shading_b = false;
            }
            else
            {
                ginfo.shading.patch_colors = curve_colors;
                ginfo.shading.shading_type = ShadingInfo::TensorCurveInterpolation;
            }
            return ginfo;
        }   
    };
}

#endif
