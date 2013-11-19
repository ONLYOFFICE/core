#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CTextCascadeUp : public CPPTXShape
	{
		public:
			CTextCascadeUp()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 44444\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 28570 adj 100000\" />")
					  _T("<gd name=\"dy\" fmla=\"*/ a h 100000\" />")
					  _T("<gd name=\"y1\" fmla=\"+- t dy 0\" />")
					  _T("<gd name=\"dy2\" fmla=\"+- h 0 dy\" />")
					  _T("<gd name=\"dy3\" fmla=\"*/ dy2 1 4\" />")
					  _T("<gd name=\"y2\" fmla=\"+- t dy3 0\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj\" minY=\"28570\" maxY=\"100000\">")
						_T("<pos x=\"r\" y=\"y1\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"y2\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"t\" />")
						_T("</lnTo>")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"b\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"y1\" />")
						_T("</lnTo>")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}