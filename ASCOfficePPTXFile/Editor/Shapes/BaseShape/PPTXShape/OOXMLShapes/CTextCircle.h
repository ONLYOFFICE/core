#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CTextCircle : public CPPTXShape
	{
		public:
			CTextCircle()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 10800000\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adval\" fmla=\"pin 0 adj 21599999\" />")
					  _T("<gd name=\"d0\" fmla=\"+- adval 0 10800000\" />")
					  _T("<gd name=\"d1\" fmla=\"+- 10800000 0 adval\" />")
					  _T("<gd name=\"d2\" fmla=\"+- 21600000 0 adval\" />")
					  _T("<gd name=\"d3\" fmla=\"?: d1 d1 10799999\" />")
					  _T("<gd name=\"d4\" fmla=\"?: d0 d2 d3\" />")
					  _T("<gd name=\"swAng\" fmla=\"*/ d4 2 1\" />")
					  _T("<gd name=\"wt1\" fmla=\"sin wd2 adj\" />")
					  _T("<gd name=\"ht1\" fmla=\"cos hd2 adj\" />")
					  _T("<gd name=\"dx1\" fmla=\"cat2 wd2 ht1 wt1\" />")
					  _T("<gd name=\"dy1\" fmla=\"sat2 hd2 ht1 wt1\" />")
					  _T("<gd name=\"x1\" fmla=\"+- hc dx1 0\" />")
					  _T("<gd name=\"y1\" fmla=\"+- vc dy1 0\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahPolar gdRefAng=\"adj\" minAng=\"0\" maxAng=\"21599999\">")
						_T("<pos x=\"x1\" y=\"y1\" />")
					  _T("</ahPolar>")
					_T("</ahLst>")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"x1\" y=\"y1\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"wd2\" hR=\"hd2\" stAng=\"adval\" swAng=\"swAng\" />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}