
var MyGraphics = new CNativeGraphics();

// Задаём размеры холста и всё будет полупрозрачное
MyGraphics.init(null, 1000, 1000, 1000 * 25.4 / 96, 1000 * 25.4 / 96);
MyGraphics.put_GlobalAlpha(true, 0.5);

// Рисуем желтый квадрат с красной пунктирной обводкой и толщиной 2
MyGraphics.p_color(255, 0, 0, 255);

var PenColor = MyGraphics.GetPenColor();
MyGraphics.p_color(PenColor.R, PenColor.G, PenColor.B, PenColor.A);

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

MyGraphics.SetFont({FontFamily : {Name : "Arial", Index : -1}, FontSize : 16, Italic : true, Bold : true});
MyGraphics.FillText(10, 110, "A");
MyGraphics.tg("A", 10, 130);
MyGraphics.t("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 10, 230, false);

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

MyGraphics.reset();
MyGraphics.DrawStringASCII("Times New Roman", 16, false, false, "Test TEXT TEXT TEXT TEXT", 10, 210, false);
MyGraphics.DrawHeaderEdit(10);
MyGraphics.DrawFooterEdit(250);
MyGraphics.DrawLockParagraph(null, 10, 110, 200);
MyGraphics.DrawLockObjectRect(null, 20, 110, 90, 90);
MyGraphics.DrawEmptyTableLine(10, 110, 110, 210);
MyGraphics.DrawSpellingLine(120, 10, 100, 5);
MyGraphics.drawHorLine(1, 130, 10, 110, 10);
MyGraphics.drawVerLine(1, 30, 110, 200, 10);
MyGraphics.drawHorLineExt(1, 170, 20, 110, 5, 20, 0);

MyGraphics.p_color(111, 0, 53, 255);
MyGraphics.p_width(3);
MyGraphics.b_color1(222, 155, 176, 255);
MyGraphics._s();
MyGraphics.TableRect(110, 10, 90, 90);
MyGraphics._z();

// Рисует замыкание со второй до предпоследней точки
MyGraphics.DrawPolygon({Points : [{X : 10, Y : 10}, {X : 20, Y : 20}, {X : 20, Y : 30}, {X : 10, Y : 40}, {X : 0, Y : 30}]}, 3, 0);

MyGraphics.DrawFootnoteRect(30, 30, 90, 90);

MyGraphics.b_color1(255, 0, 0, 255);
MyGraphics._s();
MyGraphics.AddClipRect(210, 10, 50, 50);
MyGraphics.ds();
MyGraphics._z();
MyGraphics.RemoveClipRect();

MyGraphics.SetClip({ x : 210, y : 70, w : 50, h : 50});
MyGraphics.RemoveClip();

var imageBase64 = MyGraphics.toDataURL("png");
MyGraphics.save();
MyGraphics.put_brushTexture("pathtobrush", 0);
MyGraphics.put_BrushTextureAlpha(255);
MyGraphics.put_BrushGradient({colors : [{R : 255, G : 255, B : 255, A : 255}, {R : 255, G : 255, B : 255, A : 255}, {R : 255, G : 255, B : 255, A : 255}, {R : 255, G : 255, B : 255, A : 255}, {R : 255, G : 255, B : 255, A : 255}, {R : 255, G : 255, B : 255, A : 255}]}, {x0 : 0, y0 : 0, x1 : 0, y1 : 0, r0 : 0, r1 : 0}, null);

MyGraphics.GetFont();
