#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CCurvedConnector4 : public CPPTXShape
	{
		public:
			CCurvedConnector4()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj1\" fmla=\"val 50000\" />")
					  _T("<gd name=\"adj2\" fmla=\"val 50000\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"x2\" fmla=\"*/ w adj1 100000\" />")
					  _T("<gd name=\"x1\" fmla=\"+/ l x2 2\" />")
					  _T("<gd name=\"x3\" fmla=\"+/ r x2 2\" />")
					  _T("<gd name=\"x4\" fmla=\"+/ x2 x3 2\" />")
					  _T("<gd name=\"x5\" fmla=\"+/ x3 r 2\" />")
					  _T("<gd name=\"y4\" fmla=\"*/ h adj2 100000\" />")
					  _T("<gd name=\"y1\" fmla=\"+/ t y4 2\" />")
					  _T("<gd name=\"y2\" fmla=\"+/ t y1 2\" />")
					  _T("<gd name=\"y3\" fmla=\"+/ y1 y4 2\" />")
					  _T("<gd name=\"y5\" fmla=\"+/ b y4 2\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefX=\"adj1\" minX=\"-2147483647\" maxX=\"2147483647\">")
						_T("<pos x=\"x2\" y=\"y1\" />")
					  _T("</ahXY>")
					  _T("<ahXY gdRefY=\"adj2\" minY=\"-2147483647\" maxY=\"2147483647\">")
						_T("<pos x=\"x3\" y=\"y4\" />")
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
						  _T("<pt x=\"x2\" y=\"y2\" />")
						  _T("<pt x=\"x2\" y=\"y1\" />")
						_T("</cubicBezTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"x2\" y=\"y3\" />")
						  _T("<pt x=\"x4\" y=\"y4\" />")
						  _T("<pt x=\"x3\" y=\"y4\" />")
						_T("</cubicBezTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"x5\" y=\"y4\" />")
						  _T("<pt x=\"r\" y=\"y5\" />")
						  _T("<pt x=\"r\" y=\"b\" />")
						_T("</cubicBezTo>")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}