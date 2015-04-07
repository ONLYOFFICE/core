#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CTextDeflateBottom : public CPPTXShape
	{
		public:
			CTextDeflateBottom()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 50000\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 6250 adj 100000\" />")
					  _T("<gd name=\"dy\" fmla=\"*/ a ss 100000\" />")
					  _T("<gd name=\"dy2\" fmla=\"+- h 0 dy\" />")
					  _T("<gd name=\"y1\" fmla=\"+- t dy 0\" />")
					  _T("<gd name=\"cp\" fmla=\"+- y1 0 dy2\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj\" minY=\"6250\" maxY=\"100000\">")
						_T("<pos x=\"hc\" y=\"y1\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"t\" />")
						_T("</lnTo>")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"b\" />")
						_T("</moveTo>")
						_T("<quadBezTo>")
						  _T("<pt x=\"hc\" y=\"cp\" />")
						  _T("<pt x=\"r\" y=\"b\" />")
						_T("</quadBezTo>")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}