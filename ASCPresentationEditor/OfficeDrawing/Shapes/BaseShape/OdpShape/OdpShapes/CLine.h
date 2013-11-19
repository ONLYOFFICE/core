#pragma once
#include "../OdpShape.h"

namespace OdpShapes
{
	class CLine : public COdpShape
	{
		public:
			CLine()
			{
				LoadFromXML(
//Модификаторы (все в миллиметрах):
//1 - начальный икс
//2 - начальный игрек
//3 - ширина рамки-прямоугольника
//4 - высота рамки-прямоугольника
//5 - ширина слайда
//6 - высота слайда

					_T("<draw:enhanced-geometry xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\" svg:viewBox=\"0 0 21600 21600\" draw:type=\"non-primitive-line\" draw:modifiers=\"0 0 21600 21600 21600 21600\" draw:enhanced-path=\"M ?f0 ?f1 L ?f4 ?f5 Z N\">")
					  _T("<draw:equation draw:name=\"f0\" draw:formula=\"$0*21600/$4\" />") 
					  _T("<draw:equation draw:name=\"f1\" draw:formula=\"$1*21600/$5\" />") 
					  _T("<draw:equation draw:name=\"f2\" draw:formula=\"$2*21600/$4\" />") 
					  _T("<draw:equation draw:name=\"f3\" draw:formula=\"$3*21600/$5\" />") 
					  _T("<draw:equation draw:name=\"f4\" draw:formula=\"?f0 + ?f2\" />") 
					  _T("<draw:equation draw:name=\"f5\" draw:formula=\"?f1 + ?f3\" />") 
					_T("</draw:enhanced-geometry>")
				);
			}
	};
}