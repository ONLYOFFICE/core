#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CFlowChartPunchedTape : public CPPTXShape
	{
		public:
			CFlowChartPunchedTape()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"y2\" fmla=\"*/ h 9 10\" />")
					  _T("<gd name=\"ib\" fmla=\"*/ h 4 5\" />")
					_T("</gdLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"hd10\" />")
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
					_T("<rect l=\"l\" t=\"hd5\" r=\"r\" b=\"ib\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path w=\"20\" h=\"20\">")
						_T("<moveTo>")
						  _T("<pt x=\"0\" y=\"2\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"5\" hR=\"2\" stAng=\"cd2\" swAng=\"-10800000\" />")
						_T("<arcTo wR=\"5\" hR=\"2\" stAng=\"cd2\" swAng=\"cd2\" />")
						_T("<lnTo>")
						  _T("<pt x=\"20\" y=\"18\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"5\" hR=\"2\" stAng=\"0\" swAng=\"-10800000\" />")
						_T("<arcTo wR=\"5\" hR=\"2\" stAng=\"0\" swAng=\"cd2\" />")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}