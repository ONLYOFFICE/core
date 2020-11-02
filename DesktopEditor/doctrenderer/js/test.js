
var MyGraphics = new CNativeGraphics();

// Задаём размеры холста и всё будет полупрозрачное
MyGraphics.init(null, 1000, 1000, 1000 * 25.4 / 96, 1000 * 25.4 / 96);

MyGraphics.DrawHeaderEdit(10);
MyGraphics.DrawFooterEdit(250);

MyGraphics.put_GlobalAlpha(true, 0.5);

// Рисуем желтый квадрат с красной пунктирной обводкой и толщиной 2
MyGraphics.p_color(255, 0, 0, 255);
MyGraphics.p_width(2);
MyGraphics.p_dash([5, 5]);
MyGraphics.b_color1(255, 255, 0, 255);
MyGraphics._s();
MyGraphics._m(10, 10);
MyGraphics._l(100, 10);
MyGraphics._l(100, 100);
MyGraphics._l(10, 100);
MyGraphics._z();
MyGraphics.df();
MyGraphics.ds();

// Поварачиваем плоскость рисования
MyGraphics.transform(1, 0.5, 0.1, 1, 0, 0);

// Рисуем розовый квадрат с бардовой сплошной обводкой и толщиной 1
MyGraphics.p_color(111, 0, 53, 255);
MyGraphics.p_width(1);
MyGraphics.p_dash();
MyGraphics.b_color1(222, 155, 176, 255);
MyGraphics.rect(75, 50, 100, 100)
MyGraphics.df();
MyGraphics.ds();

// Восстанавливаем плоскость рисования до нормальной и убираем полупрозрачность
MyGraphics.reset();
MyGraphics.put_GlobalAlpha(true, 1);

MyGraphics.SetFont({FontFamily : {Name : "Arial", Index : -1}, FontSize : 12, Italic : false, Bold : false});
MyGraphics.FillText(10, 110, "A");

// Рисуем кривую с одной контрольной точкой
MyGraphics.b_color1(66, 255, 158, 255);
MyGraphics._s();
MyGraphics._m(10, 10);
MyGraphics._c2(100, 50, 10, 100);
MyGraphics._z();
MyGraphics.df();
MyGraphics.ds();

// Отсекаем область рисования последним контруром и снова полупрозрачность
MyGraphics.clip();
MyGraphics.put_GlobalAlpha(true, 0.5);

// Рисуем кривую с двумя контрольными точками
MyGraphics.b_color1(121, 160, 255, 255);
MyGraphics._s();
MyGraphics._m(100, 10);
MyGraphics._c(10, 10, 10, 100, 100, 100);
MyGraphics._z();
MyGraphics.df();
MyGraphics.ds();

// Восстанавливаем область рисования и поворачиваем плоскость рисования
MyGraphics.restore();
MyGraphics.transform3({ sx : 1, shy : 0.5, shx : 0.1, sy : 1, tx : 0, ty : 0 }, false);

// Рисуем изображение из файла
MyGraphics.drawImage2("image1.png", 95, 76, 60, 48, 0, null);

var imageBase64 = MyGraphics.toDataURL("png");
imageBase64;
