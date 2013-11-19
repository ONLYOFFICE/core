#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CTextPlain : public CPPTXShape
	{
		public:
			CTextPlain()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 50000\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 30000 adj 70000\" />")
					  _T("<gd name=\"mid\" fmla=\"*/ a w 100000\" />")
					  _T("<gd name=\"midDir\" fmla=\"+- mid 0 hc\" />")
					  _T("<gd name=\"dl\" fmla=\"+- mid 0 l\" />")
					  _T("<gd name=\"dr\" fmla=\"+- r 0 mid\" />")
					  _T("<gd name=\"dl2\" fmla=\"*/ dl 2 1\" />")
					  _T("<gd name=\"dr2\" fmla=\"*/ dr 2 1\" />")
					  _T("<gd name=\"dx\" fmla=\"?: midDir dr2 dl2\" />")
					  _T("<gd name=\"xr\" fmla=\"+- l dx 0\" />")
					  _T("<gd name=\"xl\" fmla=\"+- r 0 dx\" />")
					  _T("<gd name=\"tlx\" fmla=\"?: midDir l xl\" />")
					  _T("<gd name=\"trx\" fmla=\"?: midDir xr r\" />")
					  _T("<gd name=\"blx\" fmla=\"?: midDir xl l\" />")
					  _T("<gd name=\"brx\" fmla=\"?: midDir r xr\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefX=\"adj\" minX=\"30000\" maxX=\"70000\">")
						_T("<pos x=\"mid\" y=\"b\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"tlx\" y=\"t\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"trx\" y=\"t\" />")
						_T("</lnTo>")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"blx\" y=\"b\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"brx\" y=\"b\" />")
						_T("</lnTo>")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}