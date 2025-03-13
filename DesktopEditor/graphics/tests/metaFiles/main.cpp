/**
 * @file main.cpp
 * @brief Metafile converter
 *
 * @details Convert metafile(wmf, emf, svg, svn) to raster graphics like bmp, png, etc.
 */

#include "../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../pro/Graphics.h"
#include "../../../common/File.h"

/**
 * @param argv[1] - path to the metafile
 * @param argv[2] - path of the saved file
 */
int main(int argc, char *argv[])
{
	if (argc < 2) return 1;
	NSUnicodeConverter::CUnicodeConverter converter;

	NSFonts::IApplicationFonts* app_fonts = NSFonts::NSApplication::Create();
	app_fonts->Initialize();
	MetaFile::IMetaFile* meta_file = MetaFile::Create(app_fonts);
	if (!meta_file->LoadFromFile(converter.toUnicode(std::string(argv[1]), 1).c_str()))
		return 1;

	double x, y, w, h;
	meta_file->GetBounds(&x, &y, &w, &h);
	meta_file->ConvertToRaster(converter.toUnicode(std::string(argv[2]), 1).c_str(), 1,
							   static_cast<int>(w) + 1,
							   static_cast<int>(h) + 1);

	RELEASEOBJECT(app_fonts);
	RELEASEOBJECT(meta_file);

	return 0;
}
