#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CTextRingOutside : public CPPTXShape
	{
		public:
			CTextRingOutside()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 60000\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 50000 adj 99000\" />")
					  _T("<gd name=\"dy\" fmla=\"*/ a h 100000\" />")
					  _T("<gd name=\"y\" fmla=\"+- t dy 0\" />")
					  _T("<gd name=\"r\" fmla=\"*/ dy 1 2\" />")
					  _T("<gd name=\"y1\" fmla=\"+- t r 0\" />")
					  _T("<gd name=\"y2\" fmla=\"+- b 0 r\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj\" minY=\"50000\" maxY=\"99000\">")
						_T("<pos x=\"hc\" y=\"y\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"y1\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"wd2\" hR=\"r\" stAng=\"10800000\" swAng=\"-21599999\" />")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"y2\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"wd2\" hR=\"r\" stAng=\"10800000\" swAng=\"-21599999\" />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}