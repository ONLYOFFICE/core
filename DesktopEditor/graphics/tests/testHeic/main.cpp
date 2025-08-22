#include "../../pro/Graphics.h"

int main(int argc, char *argv[])
{
	Aggplus::CImage img(L"C:\\Users\\KProkhorov\\Work\\core\\DesktopEditor\\graphics\\tests\\testHeic\\image1.heic");
	img.SaveFile(L"C:\\Users\\KProkhorov\\Work\\core\\DesktopEditor\\graphics\\tests\\testHeic\\image1.bmp", 1);

	return 0;
}
