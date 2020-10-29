
var MyTest = new CTest(); // It's work
// MyTest.getA(); // It's work


var MyGraphics = new CNativeGraphics(null); // It's doesn't work. only works with comments

// var MyNativeGraphics = CreateNativeGraphics(); // It's work. This is CGraphicsEmbed::CreateObjectInContext("CreateNativeGraphics", context);
// var MyGraphics2 = new CNativeGraphics(MyNativeGraphics); // It's doesn't work either

MyGraphics.GetIntegerGrid();
