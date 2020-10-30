
var MyGraphics = new CNativeGraphics();

MyGraphics.init(null, 1000, 1000, 1000 * 25.4 / 96, 1000 * 25.4 / 96);

MyGraphics.p_color(255, 0, 0, 255);
MyGraphics.b_color1(255, 255, 0, 255);
MyGraphics._s();
MyGraphics._m(10, 10);
MyGraphics._l(100, 10);
MyGraphics._l(100, 100);
MyGraphics._l(10, 100);
MyGraphics._z();
MyGraphics.df();
MyGraphics.ds();

MyGraphics.p_color(111, 0, 53, 255);
MyGraphics.b_color1(222, 155, 176, 255);
MyGraphics.rect(75, 50, 100, 200)
MyGraphics.df();
MyGraphics.ds();

MyGraphics.b_color1(66, 255, 158, 255);
MyGraphics._s();
MyGraphics._m(10, 10);
MyGraphics._c(10, 10, 100, 50, 10, 100);
MyGraphics._z();
MyGraphics.df();
MyGraphics.ds();

MyGraphics.drawImage2("image1.png", 95, 126, 60, 48, 0, null);

var imageBase64 = MyGraphics.toDataURL("png");
imageBase64;
