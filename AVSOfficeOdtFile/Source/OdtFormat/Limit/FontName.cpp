
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FontName.h"


namespace Odt
{
	namespace Limit
	{

		FontName::FontName()
		{
			add("@Arial Unicode MS");
			add("'@Arial Unicode MS'");
			add("@MS Mincho 12");
			add("'@MS Mincho 12'");
			add("Times New Roman");
			add("'Times New Roman'");
			add("Agency FB");
			add("'Agency FB'");
			add("Arial");
			add("Arial Black");
			add("'Arial Black'");
			add("Aston-F1");
			add("'Aston-F1'");
			add("Bradley Hand ITC");
			add("'Bradley Hand ITC'");
			add("Bookshelf Symbol 7");
			add("'Bookshelf Symbol 7'");
			add("comic");
			add("Calibri");
			add("'Calibri'");
			add("Comic Sans MS");
			add("'Comic Sans MS'");
			add("Courier New");
			add("'Courier New'");
			add("Curlz MT");
			add("'Curlz MT'");
			add("DejaVu Sans");
			add("'DejaVu Sans'");
			add("DejaVu Sans Condensed");
			add("'DejaVu Sans Condensed'");
			add("DejaVu Sans Light");
			add("'DejaVu Sans Light'");
			add("DejaVu Sans Mono");
			add("'DejaVu Sans Mono'");
			add("DejaVu Serif");
			add("'DejaVu Serif'");
			add("DejaVu Serif Condensed");
			add("'DejaVu Serif Condensed'");
			add("Estrangelo Edessa");
			add("'Estrangelo Edessa'");
			add("Franklin Gothic Medium");
			add("'Franklin Gothic Medium'");
			add("Gautam");
			add("Georgi");
			add("Impact");
			add("Latha");
			add("'Liberation Serif'");
			add("Lucida Console");
			add("'Lucida Console'");
			add("Lucida Sans Unicode");
			add("'Lucida Sans Unicode'");
			add("Mangal");
			add("Marlett");
			add("Microsoft Sans Serif");
			add("'Microsoft Sans Serif'");
			add("Modern");
			add("MT Extra");
			add("'MT Extra'");
			add("MV Boli");
			add("'MV Boli'");
			add("OpenSymbol");
			add("Palatino Linotype");
			add("'Palatino Linotype'");
			add("Raavi");
			add("Roman");
			add("Script");
			add("Shruti");
			add("StarSymbol");
			add("Sylfaen");
			add("Symbol");
			add("Tahoma");
			add("Tahoma1");
			add("Thorndale");
			add("Trebuchet MS");
			add("'Trebuchet MS'");
			add("Tunga");
			add("Verdana");
			add("Webdings");
			add("Wingdings");
		}


		const std::string FontName::no_find() const
		{
			return "Times New Roman";
		}

	} // namespace Limit
} // namespace Odt