#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CFlowChartOfflineStorage : public CPPTXShape
	{
		public:
			CFlowChartOfflineStorage()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"x4\" fmla=\"*/ w 3 4\" />")
					_T("</gdLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"x4\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"hc\" y=\"b\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"wd4\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"t\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"wd4\" t=\"t\" r=\"x4\" b=\"vc\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path stroke=\"false\" extrusionOk=\"false\" w=\"2\" h=\"2\">")
						_T("<moveTo>")
						  _T("<pt x=\"0\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"2\" y=\"0\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"1\" y=\"2\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					  _T("<path fill=\"none\" extrusionOk=\"false\" w=\"5\" h=\"5\">")
						_T("<moveTo>")
						  _T("<pt x=\"2\" y=\"4\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"3\" y=\"4\" />")
						_T("</lnTo>")
					  _T("</path>")
					  _T("<path fill=\"none\" extrusionOk=\"true\" w=\"2\" h=\"2\">")
						_T("<moveTo>")
						  _T("<pt x=\"0\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"2\" y=\"0\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"1\" y=\"2\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}