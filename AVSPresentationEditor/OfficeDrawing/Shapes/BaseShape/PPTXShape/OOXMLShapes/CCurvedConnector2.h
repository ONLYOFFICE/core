#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CCurvedConnector2 : public CPPTXShape
	{
		public:
			CCurvedConnector2()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<rect l=\"l\" t=\"t\" r=\"r\" b=\"b\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path fill=\"none\">")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"wd2\" y=\"t\" />")
						  _T("<pt x=\"r\" y=\"hd2\" />")
						  _T("<pt x=\"r\" y=\"b\" />")
						_T("</cubicBezTo>")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}