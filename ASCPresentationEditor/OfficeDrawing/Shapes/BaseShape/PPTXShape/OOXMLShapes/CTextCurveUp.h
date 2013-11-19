#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CTextCurveUp : public CPPTXShape
	{
		public:
			CTextCurveUp()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
    _T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
      _T("<gd name=\"adj\" fmla=\"val 45977\" />")
    _T("</avLst>")
    _T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
      _T("<gd name=\"a\" fmla=\"pin 0 adj 56338\" />")
      _T("<gd name=\"dy\" fmla=\"*/ a h 100000\" />")
      _T("<gd name=\"gd1\" fmla=\"*/ dy 3 4\" />")
      _T("<gd name=\"gd2\" fmla=\"*/ dy 5 4\" />")
      _T("<gd name=\"gd3\" fmla=\"*/ dy 3 8\" />")
      _T("<gd name=\"gd4\" fmla=\"*/ dy 1 8\" />")
      _T("<gd name=\"gd5\" fmla=\"+- h 0 gd3\" />")
      _T("<gd name=\"gd6\" fmla=\"+- gd4 h 0\" />")
      _T("<gd name=\"y0\" fmla=\"+- t dy 0\" />")
      _T("<gd name=\"y1\" fmla=\"+- t gd1 0\" />")
      _T("<gd name=\"y2\" fmla=\"+- t gd2 0\" />")
      _T("<gd name=\"y3\" fmla=\"+- t gd3 0\" />")
      _T("<gd name=\"y4\" fmla=\"+- t gd4 0\" />")
      _T("<gd name=\"y5\" fmla=\"+- t gd5 0\" />")
      _T("<gd name=\"y6\" fmla=\"+- t gd6 0\" />")
      _T("<gd name=\"x1\" fmla=\"+- l wd3 0\" />")
      _T("<gd name=\"x2\" fmla=\"+- r 0 wd3\" />")
    _T("</gdLst>")
    _T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
      _T("<ahXY gdRefY=\"adj\" minY=\"0\" maxY=\"56338\">")
        _T("<pos x=\"l\" y=\"y0\" />")
      _T("</ahXY>")
    _T("</ahLst>")
    _T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
      _T("<path>")
        _T("<moveTo>")
          _T("<pt x=\"l\" y=\"y0\" />")
        _T("</moveTo>")
        _T("<cubicBezTo>")
          _T("<pt x=\"x1\" y=\"y2\" />")
          _T("<pt x=\"x2\" y=\"y1\" />")
          _T("<pt x=\"r\" y=\"t\" />")
        _T("</cubicBezTo>")
      _T("</path>")
      _T("<path>")
        _T("<moveTo>")
          _T("<pt x=\"l\" y=\"y5\" />")
        _T("</moveTo>")
        _T("<cubicBezTo>")
          _T("<pt x=\"x1\" y=\"y6\" />")
          _T("<pt x=\"x2\" y=\"y6\" />")
          _T("<pt x=\"r\" y=\"y5\" />")
        _T("</cubicBezTo>")
      _T("</path>")
    _T("</pathLst>")

					_T("</ooxml-shape>")
				);
			}
	};
}