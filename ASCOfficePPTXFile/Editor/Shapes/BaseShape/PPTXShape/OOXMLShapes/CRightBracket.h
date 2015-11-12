#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CRightBracket : public CPPTXShape
	{
		public:
			CRightBracket()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 8333\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"maxAdj\" fmla=\"*/ 50000 h ss\" />")
					  _T("<gd name=\"a\" fmla=\"pin 0 adj maxAdj\" />")
					  _T("<gd name=\"y1\" fmla=\"*/ ss a 100000\" />")
					  _T("<gd name=\"y2\" fmla=\"+- b 0 y1\" />")
					  _T("<gd name=\"dx1\" fmla=\"cos w 2700000\" />")
					  _T("<gd name=\"dy1\" fmla=\"sin y1 2700000\" />")
					  _T("<gd name=\"ir\" fmla=\"+- l dx1 0\" />")
					  _T("<gd name=\"it\" fmla=\"+- y1 0 dy1\" />")
					  _T("<gd name=\"ib\" fmla=\"+- b dy1 y1\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj\" minY=\"0\" maxY=\"maxAdj\">")
						_T("<pos x=\"r\" y=\"y1\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"l\" y=\"t\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"l\" y=\"b\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"r\" y=\"vc\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"l\" t=\"it\" r=\"ir\" b=\"ib\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path stroke=\"false\" extrusionOk=\"false\">")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"w\" hR=\"y1\" stAng=\"3cd4\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"y2\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"w\" hR=\"y1\" stAng=\"0\" swAng=\"cd4\" />")
						_T("<close />")
					  _T("</path>")
					  _T("<path fill=\"none\">")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"w\" hR=\"y1\" stAng=\"3cd4\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"y2\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"w\" hR=\"y1\" stAng=\"0\" swAng=\"cd4\" />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}