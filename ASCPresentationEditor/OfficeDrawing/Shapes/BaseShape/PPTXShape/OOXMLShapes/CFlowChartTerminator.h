#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CFlowChartTerminator : public CPPTXShape
	{
		public:
			CFlowChartTerminator()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"il\" fmla=\"*/ w 1018 21600\" />")
					  _T("<gd name=\"ir\" fmla=\"*/ w 20582 21600\" />")
					  _T("<gd name=\"it\" fmla=\"*/ h 3163 21600\" />")
					  _T("<gd name=\"ib\" fmla=\"*/ h 18437 21600\" />")
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
					_T("<rect l=\"il\" t=\"it\" r=\"ir\" b=\"ib\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path w=\"21600\" h=\"21600\">")
						_T("<moveTo>")
						  _T("<pt x=\"3475\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"18125\" y=\"0\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"3475\" hR=\"10800\" stAng=\"3cd4\" swAng=\"cd2\" />")
						_T("<lnTo>")
						  _T("<pt x=\"3475\" y=\"21600\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"3475\" hR=\"10800\" stAng=\"cd4\" swAng=\"cd2\" />")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}