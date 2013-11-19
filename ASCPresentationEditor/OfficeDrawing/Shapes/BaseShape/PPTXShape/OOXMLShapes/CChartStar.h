#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CChartStar : public CPPTXShape
	{
		public:
			CChartStar()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path w=\"10\" h=\"10\" fill=\"none\" extrusionOk=\"false\">")
						_T("<moveTo>")
						  _T("<pt x=\"0\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"10\" y=\"10\" />")
						_T("</lnTo>")
						_T("<moveTo>")
						  _T("<pt x=\"0\" y=\"10\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"10\" y=\"0\" />")
						_T("</lnTo>")
						_T("<moveTo>")
						  _T("<pt x=\"5\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"5\" y=\"10\" />")
						_T("</lnTo>")
					  _T("</path>")
					  _T("<path w=\"10\" h=\"10\" stroke=\"false\">")
						_T("<moveTo>")
						  _T("<pt x=\"0\" y=\"0\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"0\" y=\"10\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"10\" y=\"10\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"10\" y=\"0\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}