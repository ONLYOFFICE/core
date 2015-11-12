#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CFlowChartAlternateProcess : public CPPTXShape
	{
		public:
			CFlowChartAlternateProcess()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"x2\" fmla=\"+- r 0 ssd6\" />")
					  _T("<gd name=\"y2\" fmla=\"+- b 0 ssd6\" />")
					  _T("<gd name=\"il\" fmla=\"*/ ssd6 29289 100000\" />")
					  _T("<gd name=\"ir\" fmla=\"+- r 0 il\" />")
					  _T("<gd name=\"ib\" fmla=\"+- b 0 il\" />")
					_T("</gdLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"t\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"l\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"hc\" y=\"b\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"r\" y=\"vc\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"il\" t=\"il\" r=\"ir\" b=\"ib\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"ssd6\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"ssd6\" hR=\"ssd6\" stAng=\"cd2\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"x2\" y=\"t\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ssd6\" hR=\"ssd6\" stAng=\"3cd4\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"y2\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ssd6\" hR=\"ssd6\" stAng=\"0\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"ssd6\" y=\"b\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ssd6\" hR=\"ssd6\" stAng=\"cd4\" swAng=\"cd4\" />")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}