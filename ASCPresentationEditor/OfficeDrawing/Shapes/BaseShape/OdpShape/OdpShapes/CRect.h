#pragma once
#include "../OdpShape.h"

namespace OdpShapes
{
	class CRect : public COdpShape
	{
		public:
			CRect()
			{
				LoadFromXML(
					_T("<draw:enhanced-geometry xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\" svg:viewBox=\"0 0 21600 21600\" draw:type=\"rectangle\" draw:enhanced-path=\"M 0 0 L 21600 0 21600 21600 0 21600 0 0 Z N\" />")
//					_T("</draw:enhanced-geometry>")
				);
			}
	};
}