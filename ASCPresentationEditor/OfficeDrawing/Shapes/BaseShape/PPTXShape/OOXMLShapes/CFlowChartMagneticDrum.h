#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CFlowChartMagneticDrum : public CPPTXShape
	{
		public:
			CFlowChartMagneticDrum()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"x2\" fmla=\"*/ w 2 3\" />")
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
						_T("<pos x=\"x2\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"r\" y=\"vc\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"wd6\" t=\"t\" r=\"x2\" b=\"b\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path stroke=\"false\" extrusionOk=\"false\" w=\"6\" h=\"6\">")
						_T("<moveTo>")
						  _T("<pt x=\"1\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"5\" y=\"0\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"1\" hR=\"3\" stAng=\"3cd4\" swAng=\"cd2\" />")
						_T("<lnTo>")
						  _T("<pt x=\"1\" y=\"6\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"1\" hR=\"3\" stAng=\"cd4\" swAng=\"cd2\" />")
						_T("<close />")
					  _T("</path>")
					  _T("<path fill=\"none\" extrusionOk=\"false\" w=\"6\" h=\"6\">")
						_T("<moveTo>")
						  _T("<pt x=\"5\" y=\"6\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"1\" hR=\"3\" stAng=\"cd4\" swAng=\"cd2\" />")
					  _T("</path>")
					  _T("<path fill=\"none\" w=\"6\" h=\"6\">")
						_T("<moveTo>")
						  _T("<pt x=\"1\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"5\" y=\"0\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"1\" hR=\"3\" stAng=\"3cd4\" swAng=\"cd2\" />")
						_T("<lnTo>")
						  _T("<pt x=\"1\" y=\"6\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"1\" hR=\"3\" stAng=\"cd4\" swAng=\"cd2\" />")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}