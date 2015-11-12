#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CFlowChartPunchedCard : public CPPTXShape
	{
		public:
			CFlowChartPunchedCard()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
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
					_T("<rect l=\"l\" t=\"hd5\" r=\"r\" b=\"b\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path w=\"5\" h=\"5\">")
						_T("<moveTo>")
						  _T("<pt x=\"0\" y=\"1\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"1\" y=\"0\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"5\" y=\"0\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"5\" y=\"5\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"0\" y=\"5\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}