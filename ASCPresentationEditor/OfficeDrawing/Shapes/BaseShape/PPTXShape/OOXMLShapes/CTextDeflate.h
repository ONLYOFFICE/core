#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CTextDeflate : public CPPTXShape
	{
		public:
			CTextDeflate()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 18750\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 0 adj 37500\" />")
					  _T("<gd name=\"dy\" fmla=\"*/ a ss 100000\" />")
					  _T("<gd name=\"gd0\" fmla=\"*/ dy 4 3\" />")
					  _T("<gd name=\"gd1\" fmla=\"+- h 0 gd0\" />")
					  _T("<gd name=\"adjY\" fmla=\"+- t dy 0\" />")
					  _T("<gd name=\"y0\" fmla=\"+- t gd0 0\" />")
					  _T("<gd name=\"y1\" fmla=\"+- t gd1 0\" />")
					  _T("<gd name=\"x0\" fmla=\"+- l wd3 0\" />")
					  _T("<gd name=\"x1\" fmla=\"+- r 0 wd3\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj\" minY=\"0\" maxY=\"37500\">")
						_T("<pos x=\"hc\" y=\"adjY\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"x0\" y=\"y0\" />")
						  _T("<pt x=\"x1\" y=\"y0\" />")
						  _T("<pt x=\"r\" y=\"t\" />")
						_T("</cubicBezTo>")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"b\" />")
						_T("</moveTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"x0\" y=\"y1\" />")
						  _T("<pt x=\"x1\" y=\"y1\" />")
						  _T("<pt x=\"r\" y=\"b\" />")
						_T("</cubicBezTo>")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}