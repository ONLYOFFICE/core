#pragma once

namespace cpdoccore { 

// поиск ближайшего цвета из 
// Black, Green, White, Blue, Magenta, Yellow, Cyan, Red
std::wstring RGBToString(int r, int g, int b);

// разбор HEX-строки с цветом
bool HEXStringToRGB(std::wstring const & str, int &r, int &g, int &b);

}

