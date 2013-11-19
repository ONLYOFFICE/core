#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CFlowChartDocument : public CPPTXShape
	{
		public:
			CFlowChartDocument()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"y1\" fmla=\"*/ h 17322 21600\" />")
					  _T("<gd name=\"y2\" fmla=\"*/ h 20172 21600\" />")
					_T("</gdLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"t\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"l\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"hc\" y=\"y2\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"r\" y=\"vc\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"l\" t=\"t\" r=\"r\" b=\"y1\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path w=\"21600\" h=\"21600\">")
						_T("<moveTo>")
						  _T("<pt x=\"0\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"21600\" y=\"0\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"21600\" y=\"17322\" />")
						_T("</lnTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"10800\" y=\"17322\" />")
						  _T("<pt x=\"10800\" y=\"23922\" />")
						  _T("<pt x=\"0\" y=\"20172\" />")
						_T("</cubicBezTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}