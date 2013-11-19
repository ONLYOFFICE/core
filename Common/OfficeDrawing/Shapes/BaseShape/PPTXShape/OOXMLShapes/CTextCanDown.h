#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CTextCanDown : public CPPTXShape
	{
		public:
			CTextCanDown()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 14286\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 0 adj 33333\" />")
					  _T("<gd name=\"dy\" fmla=\"*/ a h 100000\" />")
					  _T("<gd name=\"y0\" fmla=\"+- t dy 0\" />")
					  _T("<gd name=\"y1\" fmla=\"+- b 0 dy\" />")
					  _T("<gd name=\"ncd2\" fmla=\"*/ cd2 -1 1\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj\" minY=\"0\" maxY=\"33333\">")
						_T("<pos x=\"hc\" y=\"y0\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"wd2\" hR=\"dy\" stAng=\"cd2\" swAng=\"ncd2\" />")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"y1\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"wd2\" hR=\"dy\" stAng=\"cd2\" swAng=\"ncd2\" />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}