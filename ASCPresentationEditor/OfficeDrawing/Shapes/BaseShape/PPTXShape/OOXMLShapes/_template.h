#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class _template : public CPPTXShape
	{
		public:
			_template()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("</ooxml-shape>")
				);
			}
	};
}