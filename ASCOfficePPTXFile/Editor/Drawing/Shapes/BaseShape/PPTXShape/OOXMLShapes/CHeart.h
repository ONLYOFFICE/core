#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CHeart : public CPPTXShape
	{
		public:
			CHeart()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"dx1\" fmla=\"*/ w 49 48\" />")
					  _T("<gd name=\"dx2\" fmla=\"*/ w 10 48\" />")
					  _T("<gd name=\"x1\" fmla=\"+- hc 0 dx1\" />")
					  _T("<gd name=\"x2\" fmla=\"+- hc 0 dx2\" />")
					  _T("<gd name=\"x3\" fmla=\"+- hc dx2 0\" />")
					  _T("<gd name=\"x4\" fmla=\"+- hc dx1 0\" />")
					  _T("<gd name=\"y1\" fmla=\"+- t 0 hd3\" />")
					  _T("<gd name=\"il\" fmla=\"*/ w 1 6\" />")
					  _T("<gd name=\"ir\" fmla=\"*/ w 5 6\" />")
					  _T("<gd name=\"ib\" fmla=\"*/ h 2 3\" />")
					_T("</gdLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"hd4\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"hc\" y=\"b\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"il\" t=\"hd4\" r=\"ir\" b=\"ib\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"hc\" y=\"hd4\" />")
						_T("</moveTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"x3\" y=\"y1\" />")
						  _T("<pt x=\"x4\" y=\"hd4\" />")
						  _T("<pt x=\"hc\" y=\"b\" />")
						_T("</cubicBezTo>")
						_T("<cubicBezTo>")
						  _T("<pt x=\"x1\" y=\"hd4\" />")
						  _T("<pt x=\"x2\" y=\"y1\" />")
						  _T("<pt x=\"hc\" y=\"hd4\" />")
						_T("</cubicBezTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}