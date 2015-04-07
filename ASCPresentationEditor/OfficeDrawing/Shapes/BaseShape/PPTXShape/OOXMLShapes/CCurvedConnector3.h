#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CCurvedConnector3 : public CPPTXShape
	{
		public:
			CCurvedConnector3()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj1\" fmla=\"val 50000\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"x2\" fmla=\"*/ w adj1 100000\" />")
					  _T("<gd name=\"x1\" fmla=\"+/ l x2 2\" />")
					  _T("<gd name=\"x3\" fmla=\"+/ r x2 2\" />")
					  _T("<gd name=\"y3\" fmla=\"*/ h 3 4\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefX=\"adj1\" minX=\"-2147483647\" maxX=\"2147483647\">")
						_T("<pos x=\"x2\" y=\"vc\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<rect l=\"l\" t=\"t\" r=\"r\" b=\"b\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path fill=\"none\">")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"x1\" y=\"t\" />")
						  _T("<pt x=\"x2\" y=\"hd4\" />")
						  _T("<pt x=\"x2\" y=\"vc\" />")
						_T("</cubicBezTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"x2\" y=\"y3\" />")
						  _T("<pt x=\"x3\" y=\"b\" />")
						  _T("<pt x=\"r\" y=\"b\" />")
						_T("</cubicBezTo>")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}